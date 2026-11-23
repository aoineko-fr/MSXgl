//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄      ▄        
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀ ▀▀██ ▄  ██▀▄  
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █ ██▄▄ ██ ██▀   
//______________________________▀▀  ___________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/MSXbin)
// under CC BY-SA free license (https://creativecommons.org/licenses/by-sa/2.0/)

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <ctime>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <cassert>
#include <map>

// MSXtk
#include "MSXtk.h"
#include "VGM.h"
#include "lVGM.h"

//=============================================================================
// DEFINES
//=============================================================================

// OPNA parameters
#define LVGM_OPNA_DATA_MAX_UNIT	64	// Max data block size in lVGM (in 4 bytes unit)

// OPL4 parameters
#define LVGM_OPL4_DATA_MAX		0xFFFF	// Max size of a DF block (OPL4_WaveLoad() size is 16-bit)
#define LVGM_OPL4_DATA_MIN		64		// Min size of a DF block at the end of a segment (else start it in the next segment)
#define LVGM_OPL4_RAM_BASE		0x200000 // OPL4 address of the RAM start (VGM RAM data block addresses are relative to it)
#define LVGM_OPL4_NEW			0x01	// Expansion register (part 1 R#05) NEW flag (OPL3 mode)
#define LVGM_OPL4_NEW2			0x02	// Expansion register (part 1 R#05) NEW2 flag (OPL4 mode)

// Register index
#define REG_ID(p, r)			(((p) * 256) + (r))

// lVGM chunk type
enum LVGM_CHUNK_TYPE
{
	LVGM_CHUNK_REG = 0,
	LVGM_CHUNK_FRAME,
	LVGM_CHUNK_LOOP,
	LVGM_CHUNK_END,
	LVGM_CHUNK_DATA, // Data block (Value = start address)
//.....................................
	LVGM_CHUNK_MAX,
};

// lVGM chunk structure
struct lVGM_Chunk
{
	LVGM_CHUNK_TYPE		Type;
	LVGM_CHIP			Chip;
	u8					Port;
	u8					Register;
	u16					Value;
	std::vector<u8>		Data; // Data block content (LVGM_CHUNK_DATA only)

	lVGM_Chunk() : Type(LVGM_CHUNK_REG), Chip(LVGM_CHIP_NONE), Port(0), Register(0), Value(0) {}
	lVGM_Chunk(LVGM_CHUNK_TYPE t, LVGM_CHIP c = LVGM_CHIP_NONE, u8 p = 0, u8 r = 0, u16 v = 0) : Type(t), Chip(c), Port(p), Register(r), Value(v) {}
};

// lVGM sequence structure
struct lVGM_Seq
{
	u8					FirstReg;
	u8					Count;
	std::vector<u8>		Values;
};

// lVGM detect structure
struct lVGM_Detect
{
	u8					ChipNum;
	std::string			ChipDesc;
	u8					Devices;
	bool				bMultiChip;
	u8					OPLSource; // Source of OPL4 data: 0 = OPL4 (or none), 2 = OPL2 (YM3812), 3 = OPL3 (YMF262)
};

// lVGM register check result
enum LVGM_CHECK_ID
{
	LVGM_CHECK_NEW,  // Keep the register if value is new
	LVGM_CHECK_KEEP, // Always keep the register
	LVGM_CHECK_SKIP, // Always skip the register
//.....................................
	LVGM_CHECK_MAX,
};

// Replay (FB) parameters
#define LVGM_REPLAY_MIN			5		// Min replay size (FB opcode is 4 bytes)
#define LVGM_REPLAY_MAX			260		// Max replay size
#define LVGM_REPLAY_DIST		0xFFFF	// Max distance between FB opcode and replayed data
#define LVGM_REPLAY_TRIES		256		// Max candidates checked per line

// Recorded output line (one opcode)
struct lVGM_Line
{
	std::vector<u8>		Data;
	std::string			Comment;
	bool				bDeferred; // Data block exported at the second pass
	u8					Chip;      // Current chip before data block
	std::vector<lVGM_Chunk>::iterator Chunk; // Data block chunk

	lVGM_Line() : bDeferred(false), Chip(LVGM_CHIP_NONE) {}
};

// Exporter recording each line (for replay detection)
class ExporterRecord : public MSX::ExporterInterface
{
public:
	std::vector<lVGM_Line> Lines;

	virtual void AddReturn() {}
	virtual void AddComment(std::string comment = "") {}
	virtual void StartSection(std::string name, u8 size = MSX::DATASIZE_8bits, std::string comment = "") { Sections.push_back(MSX::ExportSection()); }
	virtual void EndSection(std::string comment = "") {}
	virtual void StartLine() { Lines.push_back(lVGM_Line()); }
	virtual void AddByte(u8 data) { Lines.back().Data.push_back(data); Sections.back().Size++; }
	virtual void AddWord(u16 data) { AddByte((u8)data); AddByte((u8)(data >> 8)); }
	virtual void AddDouble(u32 data) { AddWord((u16)data); AddWord((u16)(data >> 16)); }
	virtual void EndLine(std::string comment = "") { Lines.back().Comment = comment; }
	virtual bool Export(std::string filename) const { return true; }
};

// Literal line already exported
struct lVGM_Literal
{
	u32					Line;
	u32					Pos;
};

// Function signature for register validation
typedef u8 (*lVGM_CheckReg)(u16);

// Register validation function prototypes
u8 lVGM_CheckRegPSG(u16 reg);
u8 lVGM_CheckRegMSXMusic(u16 reg);
u8 lVGM_CheckRegMSXAudio(u16 reg);
u8 lVGM_CheckRegSCC(u16 reg);
u8 lVGM_CheckRegOPL4(u16 reg);
u8 lVGM_CheckRegOPNA(u16 reg);


//=============================================================================
// VARIABLES
//=============================================================================

// VGM variables
const VGM_Header*       g_VGM_Header;
const u8*               g_VGM_Pointer;
const u8*               g_VGM_Loop;
u16                     g_VGM_Wait;

// lVGM options
LVGM_FREQ               g_lVGM_Frequency;
bool                    g_lVGM_AddHeader;
LVGM_SIMPLIFY           g_lVGM_Simplify;
u32						g_lVGM_Split;
bool					g_lVGM_Replay;

// Global variables
u8                      g_lVGM_CurChip;
u8                      g_lVGM_Default;
u8                      g_lVGM_CurPort;

// Register map
std::map<u8, std::map<u16, u8>> g_lVGM_RegMap;


//=============================================================================
// CONST DATA
//=============================================================================

// Function to check registers to always export
const std::map<u8, lVGM_CheckReg> lVGM_CheckRegs =
{
	{ LVGM_CHIP_PSG ,     lVGM_CheckRegPSG },
	{ LVGM_CHIP_MSXMUSIC, lVGM_CheckRegMSXMusic },
	{ LVGM_CHIP_MSXAUDIO, lVGM_CheckRegMSXAudio },
	{ LVGM_CHIP_SCC,      lVGM_CheckRegSCC },
	{ LVGM_CHIP_SCCI,     lVGM_CheckRegSCC },
	{ LVGM_CHIP_PSG2,     lVGM_CheckRegPSG },
	{ LVGM_CHIP_OPL4,     lVGM_CheckRegOPL4 },
	{ LVGM_CHIP_OPNA,     lVGM_CheckRegOPNA },
};


//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Add a device to the detection structure (description string, device flags and chip counter)
void AddDevice(lVGM_Detect& detect, const c8* name, u8 flag)
{
	if(detect.ChipNum)
		detect.ChipDesc += ", ";
	detect.ChipDesc += name;
	detect.Devices |= flag;
	detect.ChipNum++;
}

//-----------------------------------------------------------------------------
// Detect sound chips used by the source VGM from its header clock fields
// Returns false if no supported chip is found
bool DetectDevices(lVGM_Detect& detect)
{
	detect.ChipDesc = "";
	detect.ChipNum = 0;
	detect.Devices = 0;
	detect.OPLSource = 0;

	if ((g_VGM_Header->Version >= 0x0151) && g_VGM_Header->AY8910_clock)
		AddDevice(detect, "PSG", LVGM_CHIP_PSG);
	if ((g_VGM_Header->Version >= 0x0000) && g_VGM_Header->YM2413_clock)
		AddDevice(detect, "OPLL", LVGM_CHIP_MSXMUSIC);
	if ((g_VGM_Header->Version >= 0x0151) && g_VGM_Header->Y8950_clock)
		AddDevice(detect, "OPL1", LVGM_CHIP_MSXAUDIO);
	if ((g_VGM_Header->Version >= 0x0161) && g_VGM_Header->K051649_clock && (g_VGM_Header->K051649_clock & (1 << 31)))
		AddDevice(detect, "SCC+", LVGM_CHIP_SCCI);
	else if ((g_VGM_Header->Version >= 0x0161) && g_VGM_Header->K051649_clock)
		AddDevice(detect, "SCC", LVGM_CHIP_SCC);
	if ((g_VGM_Header->Version >= 0x0151) && g_VGM_Header->AY8910_clock && (g_VGM_Header->AY8910_clock & (1 << 30)))
		AddDevice(detect, "PSGx2", LVGM_CHIP_PSG2);
	if ((g_VGM_Header->Version >= 0x0151) && g_VGM_Header->YMF278B_clock)
		AddDevice(detect, "OPL4", LVGM_CHIP_OPL4);
	else if ((g_VGM_Header->Version >= 0x0151) && g_VGM_Header->YMF262_clock) // Converted to OPL4 FM
	{
		AddDevice(detect, "OPL3 (as OPL4)", LVGM_CHIP_OPL4);
		detect.OPLSource = 3;
		if (g_VGM_Header->YMF262_clock & (1 << 30))
			printf("Warning: Dual OPL3 is not supported! Only the first chip is converted\n");
	}
	else if ((g_VGM_Header->Version >= 0x0151) && g_VGM_Header->YM3812_clock) // Converted to OPL4 FM
	{
		AddDevice(detect, "OPL2 (as OPL4)", LVGM_CHIP_OPL4);
		detect.OPLSource = 2;
		if (g_VGM_Header->YM3812_clock & (1 << 30))
			printf("Warning: Dual OPL2 is not supported! Only the first chip is converted\n");
	}
	if ((g_VGM_Header->Version >= 0x0151) && g_VGM_Header->YM2608_clock)
		AddDevice(detect, "OPNA", LVGM_CHIP_OPNA);

	detect.bMultiChip = (detect.ChipNum > 1);

	return detect.ChipNum > 0;
}

//-----------------------------------------------------------------------------
// Count the number of consecutive writes to incremental registers of the same chip, starting at the given chunk (up to maxLen)
// bUnique is set to true if all the written values (of the counted sequence) are identical
u16 CountIncSequence(std::vector<lVGM_Chunk>::iterator& chunk, bool& bUnique, u16 maxLen)
{
	bUnique = true;
	u16 seqLen = 1;
	u8 regCnt = chunk->Register + 1;
	auto it = std::next(chunk);
	while ((seqLen < maxLen) && (it->Type == LVGM_CHUNK_REG) && (it->Chip == chunk->Chip) && (it->Port == chunk->Port) && (it->Register == regCnt))
	{
		if (it->Value != chunk->Value)
			bUnique = false;

		it = std::next(it);
		seqLen++;
		regCnt++;
	}
	return seqLen;
}

//-----------------------------------------------------------------------------
// Check if adding 'size' bytes would cross a data segment boundary (when split option is set)
// If so, add an end-of-segment notification followed by padding up to the next segment
void CheckSplit(MSX::ExporterInterface* exp, u16 size)
{
	if (g_lVGM_Split)
	{
		u32 curSize = exp->GetTotalSize() - 1;
		u32 targetSize = curSize + size + 2;
		if ((targetSize / g_lVGM_Split) > (curSize / g_lVGM_Split))
		{
			exp->AddByteList(std::vector<u8>{ LVGM_OP_NOTIFY, 0x00 }, "======== End of data segment");
			u32 padding = (g_lVGM_Split - (exp->GetTotalSize() % g_lVGM_Split)) % g_lVGM_Split; // No padding if the notification ends exactly at the segment end
			loopx(padding)
				exp->AddByteLine(0x00, "Padding");
		}
	}
}

//-----------------------------------------------------------------------------
// Export a single byte (with segment split check)
void AddByte(MSX::ExporterInterface* exp, u8 value, std::string comment = "")
{
	CheckSplit(exp, 1);

	exp->AddByteLine(value, comment);
}

//-----------------------------------------------------------------------------
// Export a list of bytes on a single line (with segment split check)
void AddList(MSX::ExporterInterface* exp, std::vector<u8> list, std::string comment = "")
{
	CheckSplit(exp, (u8)list.size());

	exp->AddByteList(list, comment);
}

//-----------------------------------------------------------------------------
// Export a register sequence: opcode, optional first register number, then the values of 'count' consecutive chunks
void AddSequence(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk, u16 count, u8 code, std::string comment = "", bool addReg = false)
{
	CheckSplit(exp, addReg ? count + 2 : count + 1);

	exp->StartLine();
	exp->AddByte(code);

	if (addReg)
	{
		exp->AddByte(chunk->Register);
	}

	auto it = chunk;
	for (u16 i = 0; i < count; i++)
	{
		exp->AddByte((u8)it->Value);
		it = std::next(it);
	}
	exp->EndLine(comment);
}

//-----------------------------------------------------------------------------
// Export a PSG (AY-3-8910) register write chunk (1st or 2nd PSG)
// Uses the most common value shortcut (Dn) and 4-bit packed opcodes when possible
void ExportPSG(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	assert(chunk->Type == LVGM_CHUNK_REG);
	assert((chunk->Chip == LVGM_CHIP_PSG) || (chunk->Chip == LVGM_CHIP_PSG2));
	assert(chunk->Register < 16);

	// Check chip section
	if (g_lVGM_CurChip != chunk->Chip)
	{
		g_lVGM_CurChip = chunk->Chip;
		if (chunk->Chip == LVGM_CHIP_PSG)
			AddByte(exp, LVGM_OP_PSG, "---- Start PSG section");
		else // if (chunk->Chip == LVGM_CHIP_PSG2)
			AddByte(exp, LVGM_OP_PSG2, "---- Start 2nd PSG section");
	}

	// Handle more common byte value
	u8 val = (u8)chunk->Value;
	if (val == g_lVGM_Default)
	{
		AddByte(exp, 0xD0 + chunk->Register, "R#n: Dn");
		return;
	}

	// Encode register access
	switch (chunk->Register)
	{
	case 0: // Register #0
		AddList(exp, std::vector<u8>{ 0x00, val }, "R#0: 00 nn");
		break;
	case 1: // Register #1
		AddByte(exp, (u8)(0x10 + (val & 0x0F)), "R#1: 1n");
		break;
	case 2: // Register #2
		AddList(exp, std::vector<u8>{ 0x02, val }, "R#2: 02 nn");
		break;
	case 3: // Register #3
		AddByte(exp, (u8)(0x20 + (val & 0x0F)), "R#3: 2n");
		break;
	case 4: // Register #4
		AddList(exp, std::vector<u8>{ 0x04, val }, "R#4: 04 nn");
		break;
	case 5: // Register #5
		AddByte(exp, (u8)(0x30 + (val & 0x0F)), "R#5: 3n");
		break;
	case 6: // Register #6
		if (val <= 0x0F)
			AddByte(exp, (u8)(0x40 + (val & 0x0F)), "R#6: 4n");
		else
			AddByte(exp, (u8)(0x50 + (val & 0x0F)), "R#6: 5n | 0x10");
		break;
	case 7: // Register #7
		AddList(exp, std::vector<u8>{ 0x07, val }, "R#7: 07 nn");
		break;
	case 8: // Register #8
		if (val <= 0x0F)
			AddByte(exp, (u8)(0x60 + (val & 0x0F)), "R#8: 6n");
		else
			AddByte(exp, (u8)(0x70 + (val & 0x0F)), "R#8: 7n | 0x10");
		break;
	case 9: // Register #9
		if (val <= 0x0F)
			AddByte(exp, (u8)(0x80 + (val & 0x0F)), "R#9: 8n");
		else
			AddByte(exp, (u8)(0x90 + (val & 0x0F)), "R#9: 9n | 0x10");
		break;
	case 10: // Register #10
		if (val <= 0x0F)
			AddByte(exp, (u8)(0xA0 + (val & 0x0F)), "R#10: An");
		else
			AddByte(exp, (u8)(0xB0 + (val & 0x0F)), "R#10: Bn | 0x10");
		break;
	case 11: // Register #11
		AddList(exp, std::vector<u8>{ 0x0B, val }, "R#11: 0B nn");
		break;
	case 12: // Register #12
		AddList(exp, std::vector<u8>{ 0x0C, val }, "R#12: 0C nn");
		break;
	case 13: // Register #13
		AddByte(exp, (u8)(0xC0 + (val & 0x0F)), "R#13: Cn");
		break;
	};
}

//-----------------------------------------------------------------------------
// Export an OPLL (YM2413) register write chunk
// Detects register sequences (instrument, channel groups, generic 3-7 registers) and advances 'chunk' accordingly
void ExportOPLL(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	assert(chunk->Type == LVGM_CHUNK_REG);
	assert(chunk->Chip == LVGM_CHIP_OPLL);

	// Check chip section
	if (g_lVGM_CurChip != LVGM_CHIP_OPLL)
	{
		g_lVGM_CurChip = LVGM_CHIP_OPLL;
		AddByte(exp, LVGM_OP_OPLL, "---- Start OPLL section");
	}

	u8 reg = chunk->Register;
	u8 val = (u8)chunk->Value;

	// Check for sequence
	bool bUnique;
	u16 seqLen = CountIncSequence(chunk, bUnique, 18); // 6n/7n opcodes can handle up to 18 registers
	if ((reg == 0x00) && (seqLen == 8))
	{
		if (bUnique)
			AddList(exp, std::vector<u8>{ 0x50, val }, "50 nn => R#0~7");
		else
			AddSequence(exp, chunk, seqLen, 0x40, "40 nn[8] => R#0~7");
		chunk += seqLen - 1;
		return;
	}
	else if ((reg == 0x10) && (seqLen == 9))
	{
		if (bUnique)
			AddList(exp, std::vector<u8>{ 0x51, val }, "51 nn => R#10~18");
		else
			AddSequence(exp, chunk, seqLen, 0x41, "41 nn[9] => R#10~18");
		chunk += seqLen - 1;
		return;
	}
	else if ((reg == 0x16) && (seqLen == 3))
	{
		if (bUnique)
			AddList(exp, std::vector<u8>{ 0x54, val }, "54 nn => R#16~18");
		else
			AddSequence(exp, chunk, seqLen, 0x44, "44 nn[3] => R#16~18");
		chunk += seqLen - 1;
		return;
	}
	else if ((reg == 0x20) && (seqLen == 9))
	{
		if (bUnique)
			AddList(exp, std::vector<u8>{ 0x52, val }, "52 nn => R#20~28");
		else
			AddSequence(exp, chunk, seqLen, 0x42, "42 nn[9] => R#20~28");
		chunk += seqLen - 1;
		return;
	}
	else if ((reg == 0x26) && (seqLen == 3))
	{
		if (bUnique)
			AddList(exp, std::vector<u8>{ 0x55, val }, "55 nn => R#26~28");
		else
			AddSequence(exp, chunk, seqLen, 0x45, "45 nn[3] => R#26~28");
		chunk += seqLen - 1;
		return;
	}
	else if ((reg == 0x30) && (seqLen == 9))
	{
		if (bUnique)
			AddList(exp, std::vector<u8>{ 0x53, val }, "53 nn => R#30~38");
		else
			AddSequence(exp, chunk, seqLen, 0x43, "43 nn[9] => R#30~38");
		chunk += seqLen - 1;
		return;
	}
	else if ((reg == 0x36) && (seqLen == 3))
	{
		if (bUnique)
			AddList(exp, std::vector<u8>{ 0x56, val }, "56 nn => R#36~38");
		else
			AddSequence(exp, chunk, seqLen, 0x46, "46 nn[3] => R#36~38");
		chunk += seqLen - 1;
		return;
	}
	else if (seqLen >= 3)
	{
		if (bUnique)
			AddList(exp, std::vector<u8>{ (u8)(0x60 + seqLen - 3), reg, val }, "6n rr vv => R#rr~");
		else
			AddSequence(exp, chunk, seqLen, (u8)(0x70 + seqLen - 3), "7n rr vv[] => R#rr~", true);
		chunk += seqLen - 1;
		return;
	}

	if(val)
		AddList(exp, std::vector<u8>{ chunk->Register, val }, "rr: nn");
	else
		AddByte(exp, chunk->Register | 0x80, "rr: 0");
}
	
//-----------------------------------------------------------------------------
// Export an OPL1 (Y8950) register write chunk (raw register/value pair)
void ExportOPL1(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	assert(chunk->Type == LVGM_CHUNK_REG);
	assert(chunk->Chip == LVGM_CHIP_OPL1);

	// Check chip section
	if (g_lVGM_CurChip != LVGM_CHIP_OPL1)
	{
		g_lVGM_CurChip = LVGM_CHIP_OPL1;
		AddByte(exp, LVGM_OP_OPL1, "---- Start OPL1 section");
	}

	u8 reg = chunk->Register;
	u8 val = (u8)chunk->Value;

	// Check for sequence (Dn opcode can handle up to 18 registers)
	bool bUnique;
	u16 seqLen = CountIncSequence(chunk, bUnique, 18);
	if (seqLen >= 3)
	{
		AddSequence(exp, chunk, seqLen, (u8)(0xD0 + seqLen - 3), "Dn rr vv[] => R#rr~", true);
		chunk += seqLen - 1;
		return;
	}

	AddList(exp, std::vector<u8>{ reg, val }, "rr: nn");
}

//-----------------------------------------------------------------------------
// Export an SCC (K051649) register write chunk
// Detects full waveform (32 bytes) and generic 3-18 registers sequences and advances 'chunk' accordingly
void ExportSCC(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	assert(chunk->Type == LVGM_CHUNK_REG);
	assert(chunk->Chip == LVGM_CHIP_SCC);

	// Check chip section
	if (g_lVGM_CurChip != LVGM_CHIP_SCC)
	{
		g_lVGM_CurChip = LVGM_CHIP_SCC;
		AddByte(exp, LVGM_OP_SCC, "---- Start SCC section");
	}

	u8 reg = chunk->Register;
	u8 val = (u8)chunk->Value;

	// Check for sequence
	bool bUnique;
	u16 seqLen = CountIncSequence(chunk, bUnique, 32); // Full waveform
	if ((reg == 0x00) && (seqLen >= 32))
	{
		if (bUnique)
			AddList(exp, std::vector<u8>{ 0xB8, val }, "B8 nn => 9800h~");
		else
			AddSequence(exp, chunk, 32, 0xB0, "B0 nn[32] => 9800h~");
		chunk += 31;
		return;
	}
	else if ((reg == 0x20) && (seqLen >= 32))
	{
		if (bUnique)
			AddList(exp, std::vector<u8>{ 0xB9, val }, "B9 nn => 9820h~");
		else
			AddSequence(exp, chunk, 32, 0xB1, "B1 nn[32] => 9820h~");
		chunk += 31;
		return;
	}
	else if ((reg == 0x40) && (seqLen >= 32))
	{
		if (bUnique)
			AddList(exp, std::vector<u8>{ 0xBA, val }, "BA nn => 9840h~");
		else
			AddSequence(exp, chunk, 32, 0xB2, "B2 nn[32] => 9840h~");
		chunk += 31;
		return;
	}
	else if ((reg == 0x60) && (seqLen >= 32))
	{
		if (bUnique)
			AddList(exp, std::vector<u8>{ 0xBB, val }, "BB nn => 9860h~");
		else
			AddSequence(exp, chunk, 32, 0xB3, "B3 nn[32] => 9860h~");
		chunk += 31;
		return;
	}
	else if ((reg == 0x80) && (seqLen >= 32))
	{
		printf("Warning: Channel 5's waveform data found in SCC set! (SCC+ only feature)\n");
	}

	// Generic sequence (Cn/Dn opcodes can handle up to 18 registers)
	seqLen = CountIncSequence(chunk, bUnique, 18);
	if (seqLen >= 3)
	{
		if (bUnique)
			AddList(exp, std::vector<u8>{ (u8)(0xC0 + seqLen - 3), reg, val }, "Cn rr vv => R#rr~");
		else
			AddSequence(exp, chunk, seqLen, (u8)(0xD0 + seqLen - 3), "Dn rr vv[] => R#rr~", true);
		chunk += seqLen - 1;
		return;
	}

	AddList(exp, std::vector<u8>{ reg, (u8)chunk->Value }, "rr: nn");
}

//-----------------------------------------------------------------------------
// Export an SCC+ (K052539) register write chunk (raw port/register/value)
void ExportSCCI(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	if (g_lVGM_CurChip != LVGM_CHIP_SCCI)
	{
		g_lVGM_CurChip = LVGM_CHIP_SCCI;
		AddByte(exp, LVGM_OP_SCCI, "---- Start SCC+ section");
	}

	AddList(exp, std::vector<u8>{ chunk->Port, chunk->Register, (u8)chunk->Value }, "");
}


//-----------------------------------------------------------------------------
// Export OPL4 (YMF278B) register write chunks
// Consecutive writes to the same part are grouped in a single block: xx {rr vv}[n+1] with xx = [pp nnnnnn] (pp: part, n+1: number of pairs)
void ExportOPL4(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	assert(chunk->Type == LVGM_CHUNK_REG);
	assert(chunk->Chip == LVGM_CHIP_OPL4);
	assert(chunk->Port <= VGM_PORT_OPL4_WAVE);

	// Check for chip switch
	if (g_lVGM_CurChip != LVGM_CHIP_OPL4)
	{
		g_lVGM_CurChip = LVGM_CHIP_OPL4;
		AddByte(exp, LVGM_OP_OPL4, "---- Start OPL4 section");
	}

	// Count consecutive writes to the same part (up to 64 pairs)
	u8 part = chunk->Port;
	u16 count = 1;
	auto it = std::next(chunk);
	while ((count < 64) && (it->Type == LVGM_CHUNK_REG) && (it->Chip == LVGM_CHIP_OPL4) && (it->Port == part))
	{
		it = std::next(it);
		count++;
	}

	// Build the block
	std::vector<u8> block;
	block.push_back((u8)((part << 6) | (count - 1)));
	it = chunk;
	for (u16 i = 0; i < count; i++)
	{
		block.push_back(it->Register);
		block.push_back((u8)it->Value);
		it = std::next(it);
	}
	chunk += count - 1;

	static const c8* partName[] = { "Part 0", "Part 1", "Wave" };
	AddList(exp, block, MSX::Format("xx {rr vv}[] => %s (%i)", partName[part], count));
}

//-----------------------------------------------------------------------------
// Export an OPL4 (YMF278B) wave data block chunk: DF aa aa aa nn nn data[nnnn+1] (24-bit address and 16-bit size, LSB first)
// The chunk holds the whole VGM data block (Register: address bits 23-16, Value: address bits 15-0)
// Data is split in several DF blocks to fill each segment up to its end
void ExportOPL4Data(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	assert(chunk->Type == LVGM_CHUNK_DATA);
	assert(chunk->Chip == LVGM_CHIP_OPL4);
	assert(!chunk->Data.empty());

	// Check for chip switch
	if (g_lVGM_CurChip != LVGM_CHIP_OPL4)
	{
		g_lVGM_CurChip = LVGM_CHIP_OPL4;
		AddByte(exp, LVGM_OP_OPL4, "---- Start OPL4 section");
	}

	const u32 header = 6; // DF aa aa aa nn nn
	u32 addr = ((u32)chunk->Register << 16) | chunk->Value;
	u32 len = (u32)chunk->Data.size();
	u32 ofs = 0;
	while (ofs < len)
	{
		u32 piece = std::min<u32>(len - ofs, LVGM_OPL4_DATA_MAX);
		if (g_lVGM_Split)
		{
			// Room left in the current segment (end-of-segment notification FD 00 excluded)
			u32 room = g_lVGM_Split - (exp->GetTotalSize() % g_lVGM_Split) - 2;
			if (room >= header + std::min<u32>(piece, LVGM_OPL4_DATA_MIN))
				piece = std::min<u32>(piece, room - header); // Fill the current segment
			else
				piece = std::min<u32>(piece, g_lVGM_Split - 2 - header); // Not enough room: CheckSplit will go to the next segment
		}

		CheckSplit(exp, (u16)(header + piece));
		u32 a = addr + ofs;
		u32 n = piece - 1;
		exp->AddByteList(std::vector<u8>{ LVGM_OP_OPL4_DATA, (u8)a, (u8)(a >> 8), (u8)(a >> 16), (u8)n, (u8)(n >> 8) }, MSX::Format("DF aa aa aa nn nn => Wave data (addr:%06Xh size:%i)", a, piece));
		for (u32 i = 0; i < piece; i += 16)
		{
			std::vector<u8> line(chunk->Data.begin() + ofs + i, chunk->Data.begin() + ofs + std::min<u32>(i + 16, piece));
			exp->AddByteList(line);
		}
		ofs += piece;
	}
}

//-----------------------------------------------------------------------------
// Export an OPNA (YM2608) register write chunk (raw port/register/value)
void ExportOPNA(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	assert(chunk->Type == LVGM_CHUNK_REG);
	assert(chunk->Chip == LVGM_CHIP_OPNA);

	// Check for chip switch
	if (g_lVGM_CurChip != LVGM_CHIP_OPNA)
	{
		g_lVGM_CurChip = LVGM_CHIP_OPNA;
		AddByte(exp, LVGM_OP_OPNA, "---- Start OPNA section");
	}

	// Check for port switch
	if (g_lVGM_CurPort != chunk->Port)
	{
		g_lVGM_CurPort = chunk->Port;
		AddByte(exp, (u8)(0xBE + chunk->Port), "Select port");
	}

	// Check for sequence
	// Rhythm key register (part 0, R#16) must not be in a sequence: the replayer needs to wait for the chip after writing it
	u16 maxLen = 18; // Cn/Dn opcodes can handle up to 18 registers
	if ((chunk->Port == 0) && (chunk->Register <= 0x10))
		maxLen = 0x10 - chunk->Register; // Stop before R#16 (0 for R#16 itself: CountIncSequence then returns 1)

	bool bUnique;
	u16 seqLen = CountIncSequence(chunk, bUnique, maxLen);
	if (seqLen >= 3)
	{
		if (bUnique)
			AddList(exp, std::vector<u8>{ (u8)(0xC0 + seqLen - 3), chunk->Register, (u8)chunk->Value }, "Cn rr vv => R#rr~");
		else
			AddSequence(exp, chunk, seqLen, (u8)(0xD0 + seqLen - 3), "Dn rr vv[] => R#rr~", true);
		chunk += seqLen - 1;
		return;
	}

	AddList(exp, std::vector<u8>{ chunk->Register, (u8)chunk->Value }, "rr: nn");
}

//-----------------------------------------------------------------------------
// Export an OPNA (YM2608) ADPCM data block chunk (BD aa aa nn data[(nn+1)*4])
void ExportOPNAData(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	assert(chunk->Type == LVGM_CHUNK_DATA);
	assert(chunk->Chip == LVGM_CHIP_OPNA);
	assert(!chunk->Data.empty() && (chunk->Data.size() % 4 == 0) && (chunk->Data.size() <= LVGM_OPNA_DATA_MAX_UNIT * 4));

	// Check for chip switch
	if (g_lVGM_CurChip != LVGM_CHIP_OPNA)
	{
		g_lVGM_CurChip = LVGM_CHIP_OPNA;
		AddByte(exp, LVGM_OP_OPNA, "---- Start OPNA section");
	}

	u8 units = (u8)(chunk->Data.size() / 4);
	CheckSplit(exp, (u16)(4 + chunk->Data.size()));
	exp->AddByteList(std::vector<u8>{ 0xBD, (u8)chunk->Value, (u8)(chunk->Value >> 8), (u8)(units - 1) }, MSX::Format("BD aa aa nn => ADPCM data (addr:%04Xh size:%i)", chunk->Value, units));
	for (u32 i = 0; i < chunk->Data.size(); i += 16)
	{
		std::vector<u8> line(chunk->Data.begin() + i, chunk->Data.begin() + std::min<u32>(i + 16, (u32)chunk->Data.size()));
		exp->AddByteList(line);
	}
}

//-----------------------------------------------------------------------------
// Check if a line can be part of a replayed data (FB, FD, FE and FF are excluded)
bool IsReplayable(const lVGM_Line& line)
{
	return !line.bDeferred && !line.Data.empty() && ((line.Data[0] < LVGM_OP_REPLAY) || (line.Data[0] == LVGM_OP_WAIT));
}

//-----------------------------------------------------------------------------
// Find the longest already exported literal data matching the lines starting at 'cur'
// Return the number of matching lines (0 if none), with replayed data size and position
u32 FindReplay(const std::vector<lVGM_Line>& lines, u32 cur, const std::vector<lVGM_Literal>& hist, const std::map<std::vector<u8>, std::vector<u32>>& map, u32 pos, u32& bestSize, u32& bestPos)
{
	u32 bestCnt = 0;
	bestSize = 0;

	auto it = map.find(lines[cur].Data);
	if (it == map.end())
		return 0;

	const std::vector<u32>& cand = it->second;
	for (u32 n = (u32)cand.size(), tries = 0; (n > 0) && (tries < LVGM_REPLAY_TRIES); tries++)
	{
		u32 h = cand[--n];
		const lVGM_Literal& src = hist[h];
		if (pos - src.Pos > LVGM_REPLAY_DIST)
			break;
		if (g_lVGM_Split && (pos / g_lVGM_Split != src.Pos / g_lVGM_Split))
			break;

		u32 size = 0, cnt = 0;
		while ((cur + cnt < lines.size()) && (h + cnt < hist.size()))
		{
			const lVGM_Literal& lit = hist[h + cnt];
			if (cnt && ((lit.Line != hist[h + cnt - 1].Line + 1) || (lit.Pos != src.Pos + size))) // Not contiguous
				break;
			const lVGM_Line& line = lines[cur + cnt];
			if (!IsReplayable(line) || (line.Data != lines[lit.Line].Data) || (size + line.Data.size() > LVGM_REPLAY_MAX))
				break;
			size += (u32)line.Data.size();
			cnt++;
		}

		if (size > bestSize)
		{
			bestSize = size;
			bestCnt = cnt;
			bestPos = src.Pos;
		}
	}

	return (bestSize >= LVGM_REPLAY_MIN) ? bestCnt : 0;
}

//-----------------------------------------------------------------------------
// Export recorded lines, replacing repeated data with replay opcode (FB nn dddd)
// Replayed data only contains literal lines (no nested replay)
void ExportReplay(MSX::ExporterInterface* exp, std::vector<lVGM_Line>& lines)
{
	std::vector<lVGM_Literal> hist;
	std::map<std::vector<u8>, std::vector<u32>> map;

	for (u32 i = 0; i < lines.size(); )
	{
		lVGM_Line& line = lines[i];
		if (line.bDeferred)
		{
			g_lVGM_CurChip = line.Chip;
			if (line.Chunk->Chip == LVGM_CHIP_OPL4)
				ExportOPL4Data(exp, line.Chunk);
			else
				ExportOPNAData(exp, line.Chunk);
			i++;
			continue;
		}

		u32 cnt = 0, size, src;
		bool bNewSeg = false;
		if (IsReplayable(line))
		{
			u32 pos = exp->GetTotalSize();
			cnt = FindReplay(lines, i, hist, map, pos, size, src);
			if (cnt)
			{
				CheckSplit(exp, 4);
				bNewSeg = (exp->GetTotalSize() != pos);
				if (bNewSeg)
					cnt = FindReplay(lines, i, hist, map, exp->GetTotalSize(), size, src);
			}
		}

		if (cnt)
		{
			u32 dist = exp->GetTotalSize() - src;
			exp->AddByteList(std::vector<u8>{ LVGM_OP_REPLAY, (u8)(size - LVGM_REPLAY_MIN), (u8)dist, (u8)(dist >> 8) }, MSX::Format("FB nn dddd => Replay %i bytes (-%i)", size, dist));
			i += cnt;
		}
		else
		{
			if (!bNewSeg) // CheckSplit at a segment start would add an empty segment
				CheckSplit(exp, (u16)line.Data.size());
			if (IsReplayable(line))
			{
				hist.push_back({ i, exp->GetTotalSize() });
				map[line.Data].push_back((u32)hist.size() - 1);
			}
			exp->AddByteList(line.Data, line.Comment);
			i++;
		}
	}
}

// //-----------------------------------------------------------------------------
// // Simplify register writes inside each frame according to g_lVGM_Simplify option
// //  LVGM_SIMPLIFY_ORDER:     sort writes by port/register (keeps all writes)
// //  LVGM_SIMPLIFY_DUPLICATE: keep only the last write to each chip/port/register
// void Simplify(std::vector<lVGM_Chunk>& chunkList)
// {
// 	std::vector<lVGM_Chunk> workList;
// 	std::map<u32, lVGM_Chunk> workMap;

// 	u32 count = 0;
// 	for (const lVGM_Chunk& chunk : chunkList)
// 	{
// 		switch (chunk.Type)
// 		{
// 		case LVGM_CHUNK_REG:
// 			if (g_lVGM_Simplify == LVGM_SIMPLIFY_ORDER)
// 			{
// 				u32 id = (chunk.Port << 24) + (chunk.Register << 16) + (count++ & 0xFFFF);
// 				workMap[id] = chunk;
// 			}
// 			else //if (g_lVGM_Simplify == LVGM_SIMPLIFY_DUPLICATE)
// 			{
// 				u32 id = ((u8)chunk.Chip << 16) + (chunk.Port << 8) + chunk.Register;
// 				workMap.insert_or_assign(id, chunk);
// 			}
// 			//else
// 			//	workList.push_back(chunk);
// 			break;
// 		case LVGM_CHUNK_LOOP:
// 		case LVGM_CHUNK_FRAME:
// 		case LVGM_CHUNK_END:
// 		case LVGM_CHUNK_DATA:
// 			if (workMap.size())
// 			{
// 				for (const auto& pair : workMap)
// 					workList.push_back(pair.second);
// 				workMap.clear();
// 				count = 0;
// 			}
// 			workList.push_back(chunk);
// 			break;
// 		}
// 	}

// 	chunkList = workList;
// }

//-----------------------------------------------------------------------------
// Output the register writes of a frame grouped by chip (writes order for a given chip is preserved)
// The first group is the chip active at the end of the previous frame. lastChip is updated with the last group chip
void FlushFrameByChip(std::vector<lVGM_Chunk>& frame, std::vector<lVGM_Chunk>& workList, u8& lastChip)
{
	if (frame.empty())
		return;

	// Build chip order: last active chip first (if used in this frame), then other chips by first appearance
	std::vector<u8> order;
	for (const lVGM_Chunk& chunk : frame)
	{
		if (chunk.Chip == lastChip)
		{
			order.push_back(lastChip);
			break;
		}
	}
	for (const lVGM_Chunk& chunk : frame)
	{
		if (std::find(order.begin(), order.end(), chunk.Chip) == order.end())
			order.push_back(chunk.Chip);
	}

	// Output writes grouped by chip
	for (u8 chip : order)
	{
		for (const lVGM_Chunk& chunk : frame)
		{
			if (chunk.Chip == chip)
				workList.push_back(chunk);
		}
	}

	lastChip = order.back();
	frame.clear();
}

//-----------------------------------------------------------------------------
// Group register writes by chip inside each frame to minimize chip section markers
// FRAME, LOOP and END chunks are barriers (no write is moved across them)
void GroupByChip(std::vector<lVGM_Chunk>& chunkList)
{
	std::vector<lVGM_Chunk> workList;
	std::vector<lVGM_Chunk> frame; // Register writes of the current frame
	u8 lastChip = LVGM_CHIP_PSG;   // Default chip selected by the replayer

	for (const lVGM_Chunk& chunk : chunkList)
	{
		if (chunk.Type == LVGM_CHUNK_REG)
			frame.push_back(chunk);
		else
		{
			FlushFrameByChip(frame, workList, lastChip);
			workList.push_back(chunk);
		}
	}
	FlushFrameByChip(frame, workList, lastChip);

	chunkList = workList;
}

//-----------------------------------------------------------------------------
// Check if the given PSG (AY-3-8910) register must always be exported (KEEP), always be removed (SKIP) or exported only when its value changes (NEW)
u8 lVGM_CheckRegPSG(u16 reg)
{
	if (reg == 13) // Envelope shape (writing restarts the envelope)
		return LVGM_CHECK_KEEP;

	if (reg > 13) // R#14~15: I/O ports A/B (joystick, kana LED, etc.); R#16+: not existing
		return LVGM_CHECK_SKIP;

	return LVGM_CHECK_NEW;
}

//-----------------------------------------------------------------------------
// Check if the given MSX-MUSIC (YM2413) register must always be exported (KEEP), always be removed (SKIP) or exported only when its value changes (NEW)
u8 lVGM_CheckRegMSXMusic(u16 reg)
{
	if ((reg >= 0x08) && (reg <= 0x0D)) // Not existing
		return LVGM_CHECK_SKIP;
	if (reg == 0x0F) // Test
		return LVGM_CHECK_SKIP;
	if ((reg >= 0x19) && (reg <= 0x1F)) // Not existing
		return LVGM_CHECK_SKIP;
	if ((reg >= 0x29) && (reg <= 0x2F)) // Not existing
		return LVGM_CHECK_SKIP;
	if (reg >= 0x39) // Not existing (would also be decoded as 4x~7x opcodes)
		return LVGM_CHECK_SKIP;

	return LVGM_CHECK_NEW;
}

//-----------------------------------------------------------------------------
// Check if the given MSX-AUDIO (Y8950) register must always be exported (KEEP), always be removed (SKIP) or exported only when its value changes (NEW)
u8 lVGM_CheckRegMSXAudio(u16 reg)
{
	static const std::vector<u16> toKeep =
	{
		0x07, // ADPCM control (start, record, repeat, reset)
		0x0F, // ADPCM data (CPU data stream)
	};
	if (std::find(toKeep.begin(), toKeep.end(), reg) != toKeep.end())
		return LVGM_CHECK_KEEP;

	static const std::vector<u16> toSkip =
	{
		0x00, // Not existing
		0x01, // Test
		0x02, // Timer 1 value
		0x03, // Timer 2 value
		0x04, // Flag control (IRQ reset, flag masks, timers start)
		0x05, // Keyboard input
		0x06, // Keyboard output
		0x13, // Not existing
		0x14, // Not existing
		0x18, // I/O port direction
		0x19, // I/O port data
		0x1A, // PCM data (AD/DA conversion)
	};
	if (std::find(toSkip.begin(), toSkip.end(), reg) != toSkip.end())
		return LVGM_CHECK_SKIP;
	if ((reg >= 0x1B) && (reg <= 0x1F)) // Not existing
		return LVGM_CHECK_SKIP;
	if (reg >= 0xC9) // Not existing (would also be decoded as Dx opcodes)
		return LVGM_CHECK_SKIP;

	return LVGM_CHECK_NEW;
}

//-----------------------------------------------------------------------------
// Check if the given SCC/SCC+ (K051649/K052539) register must always be exported (KEEP), always be removed (SKIP) or exported only when its value changes (NEW)
u8 lVGM_CheckRegSCC(u16 reg)
{
	if (reg >= 0xE0) // Test (VGM port 5)
		return LVGM_CHECK_SKIP;

	return LVGM_CHECK_NEW;
}

//-----------------------------------------------------------------------------
// Check if the given OPL4 (YMF278B) register must always be exported (KEEP), always be removed (SKIP) or exported only when its value changes (NEW)
// Also used for OPL2 (YM3812) and OPL3 (YMF262) sources converted to OPL4
u8 lVGM_CheckRegOPL4(u16 reg)
{
	// FM part 0
	if (reg == REG_ID(VGM_PORT_OPL4_PART0, 0x01)) // LSI test (OPL2 WSE bit has no effect on OPL4)
		return LVGM_CHECK_SKIP;
	if ((reg >= REG_ID(VGM_PORT_OPL4_PART0, 0x02)) && (reg <= REG_ID(VGM_PORT_OPL4_PART0, 0x04))) // Timer 1 value, timer 2 value, timer/IRQ control (no CSM mode on OPL4)
		return LVGM_CHECK_SKIP;

	// FM part 1
	if (reg == REG_ID(VGM_PORT_OPL4_PART1, 0x01)) // LSI test
		return LVGM_CHECK_SKIP;

	// Wave part
	if ((reg == REG_ID(VGM_PORT_OPL4_WAVE, 0x00)) || (reg == REG_ID(VGM_PORT_OPL4_WAVE, 0x01))) // LSI test
		return LVGM_CHECK_SKIP;
	if ((reg >= REG_ID(VGM_PORT_OPL4_WAVE, 0x02)) && (reg <= REG_ID(VGM_PORT_OPL4_WAVE, 0x06))) // Memory control, memory address (H/M/L) and memory data (address auto-increments)
		return LVGM_CHECK_KEEP;
	if ((reg >= REG_ID(VGM_PORT_OPL4_WAVE, 0x08)) && (reg <= REG_ID(VGM_PORT_OPL4_WAVE, 0x1F))) // Wave number (writing loads the wave header)
		return LVGM_CHECK_KEEP;
	if ((reg >= REG_ID(VGM_PORT_OPL4_WAVE, 0x68)) && (reg <= REG_ID(VGM_PORT_OPL4_WAVE, 0x7F))) // Key on, damp, LFO reset, output channel, panpot
		return LVGM_CHECK_KEEP;
	if ((reg >= REG_ID(VGM_PORT_OPL4_WAVE, 0x80)) && (reg <= REG_ID(VGM_PORT_OPL4_WAVE, 0xF7))) // LFO, vibrato, envelope and AM (overwritten by the wave header loading)
		return LVGM_CHECK_KEEP;

	return LVGM_CHECK_NEW;
}

//-----------------------------------------------------------------------------
// Check if the given OPNA (YM2608) register must always be exported (KEEP), always be removed (SKIP) or exported only when its value changes (NEW)
u8 lVGM_CheckRegOPNA(u16 reg)
{
	static const std::vector<u16> toKeep = 
	{
		REG_ID(VGM_PORT_OPNA_PART0, 0x0D), // SSG envelope shape (writing restarts the envelope)
		REG_ID(VGM_PORT_OPNA_PART0, 0x10), // Rhythm key on/off
		REG_ID(VGM_PORT_OPNA_PART0, 0x28), // FM key on/off
		REG_ID(VGM_PORT_OPNA_PART1, 0x00), // ADPCM control 1 (start, record, repeat, reset)
		REG_ID(VGM_PORT_OPNA_PART1, 0x08), // ADPCM data (CPU data stream)
		REG_ID(VGM_PORT_OPNA_PART1, 0x10), // Flag control (IRQ reset, flag masks)
	};
	if (std::find(toKeep.begin(), toKeep.end(), reg) != toKeep.end())
		return LVGM_CHECK_KEEP;
	// FM frequency (both parts): the MSB (A4~A6, AC~AE) goes to a latch shared by the part channels, and is applied when the LSB (A0~A2, A8~AA) is written
	u8 r = reg & 0xFF;
	if ((r >= 0xA0) && (r <= 0xAE) && (r != 0xA3) && (r != 0xA7) && (r != 0xAB))
		return LVGM_CHECK_KEEP;

	static const std::vector<u16> toSkip =
	{
		REG_ID(VGM_PORT_OPNA_PART0, 0x0E), // SSG I/O port A
		REG_ID(VGM_PORT_OPNA_PART0, 0x0F), // SSG I/O port B
		REG_ID(VGM_PORT_OPNA_PART0, 0x21), // LSI test
		REG_ID(VGM_PORT_OPNA_PART0, 0x24), // Timer A value (MSB)
		REG_ID(VGM_PORT_OPNA_PART0, 0x25), // Timer A value (LSB)
		REG_ID(VGM_PORT_OPNA_PART0, 0x26), // Timer B value
	};
	if (std::find(toSkip.begin(), toSkip.end(), reg) != toSkip.end())
		return LVGM_CHECK_SKIP;
	if ((reg >= REG_ID(VGM_PORT_OPNA_PART1, 0x11)) && (reg <= REG_ID(VGM_PORT_OPNA_PART1, 0x2F))) // Not existing
		return LVGM_CHECK_SKIP;
	if ((reg & 0xFF) > 0xB6) // Not existing (both parts)
		return LVGM_CHECK_SKIP;

	return LVGM_CHECK_NEW;
}

//-----------------------------------------------------------------------------
// Check if the register need to be export or not (because value didn't changed for example)
bool ShouldExport(u8 chip, u8 port, u8 reg, u8 val)
{
	u16 r = REG_ID(port, reg);

	// Check for 'always export' and 'always skip' registers (chip specific)
	auto check = lVGM_CheckRegs.find(chip);
	if (check != lVGM_CheckRegs.end())
	{
		u8 result = check->second(r);
		if (result == LVGM_CHECK_KEEP)
			return true;
		if (result == LVGM_CHECK_SKIP)
			return false;
	}

	// Check if value changed since last set
	if ((g_lVGM_RegMap[chip].find(r) == g_lVGM_RegMap[chip].end()) || (g_lVGM_RegMap[chip][r] != val))
	{
		g_lVGM_RegMap[chip][r] = val;
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Convert VGM data to lVGM format and export it through the given exporter
// Parse the VGM commands into a chunk list (register writes, frame waits, loop and end markers), then encode each chunk
// Returns false if the input is not a valid VGM or if no supported chip is found
bool ExportlVGM(std::string name, MSX::ExporterInterface* exp, const std::vector<u8>& data)
{
	g_VGM_Header = (const VGM_Header*)&data[0];

	if ((u32)g_VGM_Header->Ident != (u32)VGM_IDENT)
	{
		printf("Error: VGM header expected, but not found!\n");
		return false;
	}

	g_VGM_Pointer = (const u8*)&g_VGM_Header->Data_offset;
	g_VGM_Pointer += g_VGM_Header->Data_offset;

	if (g_VGM_Header->Loop_offset)
	{
		g_VGM_Loop = (const u8*)&g_VGM_Header->Loop_offset;
		g_VGM_Loop += g_VGM_Header->Loop_offset;
	}
	else
		g_VGM_Loop = 0;

	g_VGM_Wait = 0;

	// Check all supported chips
	lVGM_Detect detect;
	if(!DetectDevices(detect))
	{
		printf("Error: No supported sound chip detected!\n");
		return false;
	}

	// File info
	exp->AddComment(MSX::Format("VGM version: %01x.%02x (chips:%s)", (g_VGM_Header->Version >> 8) & 0xF, g_VGM_Header->Version & 0xFF, detect.ChipDesc.c_str()));

	//............................................................................
	// Parse data

	g_lVGM_CurChip = LVGM_CHIP_PSG;
	g_lVGM_Default = 0;
	g_lVGM_CurPort = 0;
	g_lVGM_RegMap.clear();
	bool bLoop = false;
	u32 sampleFrame = (g_lVGM_Frequency == LVGM_FREQ_60HZ) ? VGM_WAIT_60HZ : VGM_WAIT_50HZ;
	u32 sampleCount = 0;
	std::vector<lVGM_Chunk> chunkList;
	std::map<u8, u16> psgValFreq;

	// Specific initialisation
	if (detect.OPLSource == 2) 	// OPL2 converted to OPL4: set OPL4 mode and enable left and right outputs of all FM channels (OPL2 songs don't write CHA/CHB bits)
	{
		if (ShouldExport(LVGM_CHIP_OPL4, VGM_PORT_OPL4_PART1, 0x05, LVGM_OPL4_NEW | LVGM_OPL4_NEW2))
			chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_OPL4, VGM_PORT_OPL4_PART1, 0x05, LVGM_OPL4_NEW | LVGM_OPL4_NEW2));
		for (u8 reg = 0xC0; reg <= 0xC8; reg++)
			if (ShouldExport(LVGM_CHIP_OPL4, VGM_PORT_OPL4_PART0, reg, 0x30))
				chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_OPL4, VGM_PORT_OPL4_PART0, reg, 0x30));
	}

	// Parse input data
	const u8* dataEnd = &data[0] + data.size();
	while (1)
	{
		// Check for end of input data (missing end command)
		if (g_VGM_Pointer >= dataEnd)
		{
			printf("Warning: End of VGM data reached without end command!\n");
			chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_END));
			break;
		}

		// Check loop position (before processing the command located at loop offset)
		if (g_VGM_Loop && !bLoop && (g_VGM_Pointer >= g_VGM_Loop))
		{
			bLoop = true;
			chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_LOOP));
			g_lVGM_RegMap.clear(); // Registers state is unknown when coming back from the end of the song
		}

		//-----------------------------------------------------------------------------
		if (*g_VGM_Pointer == VGM_CMD_AY8910) // PSG/AY8910, write value dd to register aa
		{
			LVGM_CHIP chip = (g_VGM_Pointer[1] & 0x80) ? LVGM_CHIP_PSG2 : LVGM_CHIP_PSG;
			u8 reg = g_VGM_Pointer[1] & 0x7F;
			u8 val = g_VGM_Pointer[2];
			g_VGM_Pointer += 2;

			if (reg == 7)
				val = (val & 0b00111111) | 0b10000000; // Validate register values

			if (ShouldExport(chip, 0, reg, val))
			{
				switch (reg)
				{
				case 7:
				case 0:
				case 2:
				case 4:
				case 11:
				case 12:
					psgValFreq[val]++; // increment value counter
					break;
				}

				chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, chip, 0, reg, val));
			}
		}
		//-----------------------------------------------------------------------------
		else if (*g_VGM_Pointer == VGM_CMD_YM2413) // MSX-MUSIC/YM2413/OPLL, write value dd to register aa
		{
			u8 reg = g_VGM_Pointer[1];
			u8 val = g_VGM_Pointer[2];
			g_VGM_Pointer += 2;

			if (ShouldExport(LVGM_CHIP_MSXMUSIC, 0, reg, val))
			{
				chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_MSXMUSIC, 0, reg, val));
			}
		}
		//-----------------------------------------------------------------------------
		else if (*g_VGM_Pointer == VGM_CMD_Y8950) // MSX-AUDIO/Y8950/OPL1, write value dd to register aa
		{
			u8 reg = g_VGM_Pointer[1];
			u8 val = g_VGM_Pointer[2];
			g_VGM_Pointer += 2;

			if (reg == 0x08)
				val &= 0b11000100;

			if (ShouldExport(LVGM_CHIP_MSXAUDIO, 0, reg, val))
			{
				chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_MSXAUDIO, 0, reg, val));
			}
		}
		//-----------------------------------------------------------------------------
		else if (*g_VGM_Pointer == VGM_CMD_SCC) // SCC/K051649 or SCC+/K052539, port pp, write value dd to register aa
		{
			u8 val  = g_VGM_Pointer[3];

			// Generate register number
			u8 reg = 0;
			switch (g_VGM_Pointer[1])
			{
			case 0:	reg = 0x00;	break; // 0x00 - waveform
			case 1:	reg = 0x80;	break; // 0x01 - frequency
			case 2:	reg = 0x8A;	break; // 0x02 - volume
			case 3:	reg = 0x8F;	break; // 0x03 - key on/off
			case 4:	reg = 0xA0;	break; // 0x04 - waveform (0x00 used to do SCC access, 0x04 SCC+)
			case 5:	reg = 0xE0;	break; // 0x05 - test register
			}
			reg += g_VGM_Pointer[2];
			g_VGM_Pointer += 3;

			if (detect.Devices & LVGM_CHIP_SCCI) // SCC+
			{
				if (ShouldExport(LVGM_CHIP_SCCI, 0, reg, val))
				{
					// Check for mirrored register
					if ((reg >= 0xB0) && (reg <= 0xBF))
						reg -= 0x10;

					// Skip 'no function' and 'test register' access
					if (reg < 0xC0)
					{
						chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_SCCI, 0, reg, val));
					}
				}
			}
			else // SCC
			{
				if (ShouldExport(LVGM_CHIP_SCC, 0, reg, val))
				{
					// Check for mirrored register
					if ((reg >= 0x90) && (reg <= 0x9F))
						reg -= 0x10;

					// Skip 'no function' and 'test register' access
					if (reg < 0xA0)
					{
						chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_SCC, 0, reg, val));
					}
				}
			}
		}
		//-----------------------------------------------------------------------------
		else if (*g_VGM_Pointer == VGM_CMD_YMF278) // MOONSOUND/YMF278B/OPL4, port pp, write value dd to register aa
		{
			u8 port = g_VGM_Pointer[1];
			u8 reg  = g_VGM_Pointer[2];
			u8 val  = g_VGM_Pointer[3];
			g_VGM_Pointer += 3;

			// Skip second chip (port bit 7) and invalid ports
			if ((port <= VGM_PORT_OPL4_WAVE) && ShouldExport(LVGM_CHIP_OPL4, port, reg, val))
			{
				chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_OPL4, port, reg, val));
			}
		}
		else if (detect.OPLSource && ((*g_VGM_Pointer == VGM_CMD_YM3812) || (*g_VGM_Pointer == VGM_CMD_YMF262_0) || (*g_VGM_Pointer == VGM_CMD_YMF262_1))) // OPL2 or OPL3: Converted as OPL4 FM (OPL4 mode)
		{
			u8 port = (*g_VGM_Pointer != VGM_CMD_YMF262_1) ? VGM_PORT_OPL4_PART0 : VGM_PORT_OPL4_PART1;
			u8 reg = g_VGM_Pointer[1];
			u8 val = g_VGM_Pointer[2];
			g_VGM_Pointer += 2;

			if (detect.OPLSource == 2) // OPL2
			{
				if ((reg >= 0xC0) && (reg <= 0xC8)) // Output to both left and right channels (OPL2 has no CHA/CHB bits but they are needed when NEW = 1)
					val = (val & 0x0F) | 0x30;
				else if ((reg >= 0xE0) && (reg <= 0xF5)) // OPL2 has only 4 waveforms
					val &= 0x03;
			}
			else if ((port == VGM_PORT_OPL4_PART1) && (reg == 0x05)) // OPL3 mode: keep NEW2 set (OPL4 mode, wave part still accessible)
			{
				if (val & LVGM_OPL4_NEW)
					val = LVGM_OPL4_NEW | LVGM_OPL4_NEW2;
			}

			if (ShouldExport(LVGM_CHIP_OPL4, port, reg, val))
			{
				chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_OPL4, port, reg, val));
			}
		}
		//-----------------------------------------------------------------------------
		else if ((*g_VGM_Pointer == VGM_CMD_YM2608_0) || (*g_VGM_Pointer == VGM_CMD_YM2608_1)) // MAKOTO/YM2608/OPNA, write value dd to register aa
		{
			u8 port = (*g_VGM_Pointer == VGM_CMD_YM2608_0) ? 0 : 1;
			u8 reg = g_VGM_Pointer[1];
			u8 val = g_VGM_Pointer[2];
			g_VGM_Pointer += 2;

			if (port == 0)
			{
				if (reg == 0x27) // Timer control: keep only CH3 mode
					val &= 0xC0;
				else if (reg == 0x29) // IRQ enable: keep only SCH (6 FM channels)
					val &= 0x80;
			}

			if (ShouldExport(LVGM_CHIP_OPNA, port, reg, val))
			{
				chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_OPNA, port, reg, val));
			}
		}
		//-----------------------------------------------------------------------------
		else if (*g_VGM_Pointer == VGM_CMD_WAIT_16B) // Wait n samples, n can range from 0 to 65535 (approx 1.49 seconds). Longer pauses than this are represented by multiple wait commands.
		{
			u16 nn = *(u16*)(g_VGM_Pointer + 1);
			sampleCount += nn;
			g_VGM_Pointer += 2;
		}
		else if (*g_VGM_Pointer == VGM_CMD_WAIT_60HZ) // Wait 735 samples (60th of a second), a shortcut for 0x61 0xdf 0x02
		{
			sampleCount += VGM_WAIT_60HZ;
		}
		else if (*g_VGM_Pointer == VGM_CMD_WAIT_50HZ) // Wait 882 samples (50th of a second), a shortcut for 0x61 0x72 0x03
		{
			sampleCount += VGM_WAIT_50HZ;
		}
		else if ((*g_VGM_Pointer & 0xF0) == VGM_CMD_WAIT_4B) // wait n+1 samples, n can range from 0 to 15.
		{
			u8 n = 1 + (*g_VGM_Pointer & 0xF);
			sampleCount += n;
		}
		//-----------------------------------------------------------------------------
		else if (*g_VGM_Pointer == VGM_CMD_END) // End of sound data
		{
			chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_END));
			break;
		}
		else if (*g_VGM_Pointer == VGM_CMD_BLOCK) // Data block: 0x67 0x66 tt ss ss ss ss data[ss]
		{
			u8 type = g_VGM_Pointer[2];
			u32 size = *(u32*)(g_VGM_Pointer + 3) & 0x7FFFFFFF;
			switch (type)
			{
			case VGM_BLOCK_YM2608: // YM2608/OPNA DELTA-T ROM data
				if ((detect.Devices & LVGM_CHIP_OPNA) && (size > 8)) // YM2608 DELTA-T (ADPCM-B) memory image: rr rr rr rr (memory size) aa aa aa aa (start address) data[ss-8]
				{
					u32 start = *(u32*)(g_VGM_Pointer + 11);
					const u8* src = g_VGM_Pointer + 15;
					u32 len = size - 8;
					if (start & 3)
						printf("Warning: ADPCM data block start address not aligned on 4 bytes! %06x\n", start);
					if (len & 3)
						printf("Warning: ADPCM data block size not aligned on 4 bytes (padded with 0)! %06x\n", len);

					// Split the data block into chunks of 64 units max (4 bytes each)
					u32 addr = start >> 2;
					u32 units = (len + 3) / 4;
					for (u32 ofs = 0; ofs < units; ofs += LVGM_OPNA_DATA_MAX_UNIT)
					{
						u32 cnt = std::min<u32>(LVGM_OPNA_DATA_MAX_UNIT, units - ofs);
						lVGM_Chunk chunk(LVGM_CHUNK_DATA, LVGM_CHIP_OPNA, 1, 0, (u16)(addr + ofs));
						for (u32 i = ofs * 4; i < (ofs + cnt) * 4; i++)
							chunk.Data.push_back((i < len) ? src[i] : 0);
						chunkList.push_back(chunk);
					}
				}
				break;
			case VGM_BLOCK_YMF278B: // YMF278B/OPL4 RAM data
				if ((detect.Devices & LVGM_CHIP_OPL4) && (size > 8)) // YMF278B RAM image: rr rr rr rr (memory size) aa aa aa aa (start address) data[ss-8]
				{
					u32 start = (LVGM_OPL4_RAM_BASE + *(u32*)(g_VGM_Pointer + 11)) & 0x3FFFFF; // VGM start address is relative to the RAM start (22-bit address)
					const u8* src = g_VGM_Pointer + 15;
					lVGM_Chunk chunk(LVGM_CHUNK_DATA, LVGM_CHIP_OPL4, VGM_PORT_OPL4_WAVE, (u8)(start >> 16), (u16)start);
					chunk.Data.assign(src, src + size - 8);
					chunkList.push_back(chunk);
				}
				break;
			case VGM_BLOCK_Y8950: // Y8950/MSX-Audio RAM data
			default:
				printf("Warning: Unsupported data block type! %02x\n", type);
			}

			

			g_VGM_Pointer += 6 + size;
		}
		else if ((*g_VGM_Pointer & 0xF0) == VGM_CMD_DAC_WAIT) // YM2612 DAC write and wait n samples (DAC write is not supported)
		{
			sampleCount += *g_VGM_Pointer & 0x0F;
		}
		else // Unsupported commands (skip operands)
		{
			u8 cmd = *g_VGM_Pointer;
			printf("Warning: Invalid command! %02x\n", cmd);
			if ((cmd >= 0x30) && (cmd <= 0x3F))
				g_VGM_Pointer += 1;
			else if ((cmd == 0x4F) || (cmd == 0x50))
				g_VGM_Pointer += 1;
			else if ((cmd >= 0x40) && (cmd <= 0x5F))
				g_VGM_Pointer += 2;
			else if (cmd == VGM_CMD_PCM_WRITE) // 0x68 0x66 cc oo oo oo dd dd dd ss ss ss
				g_VGM_Pointer += 11;
			else if ((cmd == VGM_CMD_DAC_SETUP) || (cmd == VGM_CMD_DAC_DATA) || (cmd == VGM_CMD_DAC_START_FAST)) // DAC stream control
				g_VGM_Pointer += 4;
			else if (cmd == VGM_CMD_DAC_FREQ)
				g_VGM_Pointer += 5;
			else if (cmd == VGM_CMD_DAC_START)
				g_VGM_Pointer += 10;
			else if (cmd == VGM_CMD_DAC_STOP)
				g_VGM_Pointer += 1;
			else if ((cmd >= 0xA0) && (cmd <= 0xBF))
				g_VGM_Pointer += 2;
			else if ((cmd >= 0xC0) && (cmd <= 0xDF))
				g_VGM_Pointer += 3;
			else if (cmd >= 0xE0)
				g_VGM_Pointer += 4;
		}
		g_VGM_Pointer++;

		// Check for end of 50/60 Hz frame
		if (sampleCount >= sampleFrame)
		{
			u16 val = sampleCount / sampleFrame;
			chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_FRAME, LVGM_CHIP_NONE, 0, 0, val));
			sampleCount %= sampleFrame;
		}
	}

	//............................................................................
	// More common byte for PSG
	if ((detect.Devices & LVGM_CHIP_PSG) && !psgValFreq.empty())
	{
		using pair_type = decltype(psgValFreq)::value_type;
		auto pr = std::max_element(std::begin(psgValFreq), std::end(psgValFreq),
			[](const pair_type& p1, const pair_type& p2) { return p1.second < p2.second; });
		g_lVGM_Default = pr->first;
	}

	//............................................................................
	// Start export
	exp->StartSection(name);

	// Add header
	if (g_lVGM_AddHeader)
	{
		exp->AddComment("---- Header ----");

		// -- Ident --
		AddList(exp, std::vector<u8>{ 'l', 'V', 'G', 'M' }, "Ident \"lVGM\"");

		// -- Setting --
		u8 flag = 0;
		if (g_lVGM_Frequency == LVGM_FREQ_50HZ)
			flag |= LVGM_OPTION_50HZ;
		if (g_lVGM_Frequency == LVGM_FREQ_60HZ)
			flag |= LVGM_OPTION_60HZ;
		if (g_VGM_Header->Loop_offset)
			flag |= LVGM_OPTION_LOOP;
		if (detect.Devices != LVGM_CHIP_PSG)
			flag |= LVGM_OPTION_DEVICE;
		flag |= (LVGM_VERSION << 4);
		AddByte(exp, flag, MSX::Format("Options (freq:%s loop:%i dev:%i)", (flag & LVGM_OPTION_50HZ) ? "50Hz" : "60Hz", (flag & LVGM_OPTION_LOOP) ? 1 : 0, (flag & LVGM_OPTION_DEVICE) ? 1 : 0));

		// -- Devices --
		if (detect.Devices != LVGM_CHIP_PSG)
			AddByte(exp, detect.Devices, MSX::Format("Devices (chips:%s)", detect.ChipDesc.c_str()));

		if (detect.Devices & LVGM_CHIP_PSG)
			AddByte(exp, g_lVGM_Default, "PSG - More common byte");

		exp->AddComment("---- Data ----");
	}

	// // Remove dulicate and/or reorder register write
	// if (g_lVGM_Simplify != LVGM_SIMPLIFY_NONE)
	// {
	// 	Simplify(chunkList);
	// }

	// Group register writes by chip in each frame (to minimize chip section markers)
	if (detect.bMultiChip)
		GroupByChip(chunkList);

	// With replay, record lines without segment split (done at the second pass)
	ExporterRecord rec;
	MSX::ExporterDummy dummy;
	MSX::ExporterInterface* out = exp;
	u32 split = g_lVGM_Split;
	if (g_lVGM_Replay)
	{
		rec.StartSection(name);
		dummy.StartSection(name);
		out = &rec;
		g_lVGM_Split = 0;
	}

	// Write data
	for (auto chunk = chunkList.begin(); chunk != chunkList.end(); ++chunk)
	{
		switch (chunk->Type)
		{
		case LVGM_CHUNK_REG:
			switch (chunk->Chip)
			{
			case LVGM_CHIP_PSG:
			case LVGM_CHIP_PSG2:
				ExportPSG(out, chunk);
				break;
			case LVGM_CHIP_OPLL:
				ExportOPLL(out, chunk);
				break;
			case LVGM_CHIP_OPL1:
				ExportOPL1(out, chunk);
				break;
			case LVGM_CHIP_SCC:
				ExportSCC(out, chunk);
				break;
			case LVGM_CHIP_SCCI:
				ExportSCCI(out, chunk);
				break;
			case LVGM_CHIP_OPL4:
				ExportOPL4(out, chunk);
				break;
			case LVGM_CHIP_OPNA:
				ExportOPNA(out, chunk);
				break;
			default:
				break;
			}
			break;

		case LVGM_CHUNK_DATA:
		{
			MSX::ExporterInterface* dataOut = out;
			if (g_lVGM_Replay) // Deferred to the second pass (size depends on segment split)
			{
				lVGM_Line line;
				line.bDeferred = true;
				line.Chip = g_lVGM_CurChip;
				line.Chunk = chunk;
				rec.Lines.push_back(line);
				dataOut = &dummy;
			}
			switch (chunk->Chip)
			{
			case LVGM_CHIP_OPL4:
				ExportOPL4Data(dataOut, chunk);
				break;
			case LVGM_CHIP_OPNA:
				ExportOPNAData(dataOut, chunk);
				break;
			default:
				break;
			}
			break;
		}

		case LVGM_CHUNK_FRAME:
		{
			u32 frameCount = chunk->Value;
			// Merge consecutive frame wait chunks
			while ((std::next(chunk) != chunkList.end()) && (std::next(chunk)->Type == LVGM_CHUNK_FRAME))
			{
				++chunk;
				frameCount += chunk->Value;
			}

			// Long wait (FC nn: 1~256 frames)
			while (frameCount > 256)
			{
				AddList(out, std::vector<u8>{ LVGM_OP_WAIT, 255 }, "-------- Wait: FC nn");
				frameCount -= 256;
			}
			if (frameCount > 16)
			{
				AddList(out, std::vector<u8>{ LVGM_OP_WAIT, (u8)(frameCount - 1) }, "-------- Wait: FC nn");
				frameCount = 0;
			}
			// Short wait (En: 1~16 frames)
			if (frameCount)
				AddList(out, std::vector<u8>{ (u8)(0xE0 + (frameCount - 1)) }, "-------- Wait: Ex");
			break;
		}

		case LVGM_CHUNK_LOOP:
			AddByte(out, LVGM_OP_LOOP, "Loop marker");
			// With multiple chips, force the chip marker after the loop point
			if (detect.Devices & (detect.Devices - 1))
				g_lVGM_CurChip = LVGM_CHIP_NONE;
			g_lVGM_CurPort = 0xFF; // Force the port marker after the loop point
			break;

		case LVGM_CHUNK_END:
			AddByte(out, LVGM_OP_END, "End marker");
			break;

		default:
			break;
		}
	}

	if (g_lVGM_Replay)
	{
		g_lVGM_Split = split;
		ExportReplay(exp, rec.Lines);
	}

	// Close section
	exp->EndSection();

	return true;
}