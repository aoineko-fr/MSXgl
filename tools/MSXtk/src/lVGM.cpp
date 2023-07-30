//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄      ▄        
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀ ▀▀██ ▄  ██▀▄  
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █ ██▄▄ ██ ██▀   
//______________________________▀▀  ___________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/MSXbin)
// under CC-BY-AS free license (https://creativecommons.org/licenses/by-sa/2.0/)

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

// MSXtk
#include "MSXtk.h"
#include "VGM.h"
#include "lVGM.h"

//=============================================================================
// DEFINES
//=============================================================================

enum LVGM_CHUNK_TYPE
{
	LVGM_CHUNK_REG,
	LVGM_CHUNK_FRAME,
	LVGM_CHUNK_LOOP,
	LVGM_CHUNK_END,
};

//
struct lVGM_Chunk
{
	LVGM_CHUNK_TYPE Type;
	LVGM_CHIP       Chip;
	u8              Port;
	u8              Register;
	u16             Value;

	lVGM_Chunk() : Type(LVGM_CHUNK_REG), Chip(LVGM_CHIP_NONE), Port(0), Register(0), Value(0) {}
	lVGM_Chunk(LVGM_CHUNK_TYPE t, LVGM_CHIP c = LVGM_CHIP_NONE, u8 p = 0, u8 r = 0, u16 v = 0) : Type(t), Chip(c), Port(p), Register(r), Value(v) {}
};

//
struct lVGM_Seq
{
	u8 FirstReg;
	u8 Count;
	std::vector<u8> Values;
};

//
struct lVGM_Detect
{
	u8          ChipNum;
	std::string ChipDesc;
	u8          Devices;
	bool        bMultiChip;
};

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
u8                      g_lVGM_CurChip;
//std::vector<lVGM_Chunk> g_Chunk;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void AddDevice(lVGM_Detect& detect, std::string name, u8 flag)
{
	if(detect.ChipNum)
		detect.ChipDesc += ", ";
	detect.ChipDesc += name;
	detect.Devices |= flag;
	detect.ChipNum++;
}

//-----------------------------------------------------------------------------
//
bool DetectDevices(lVGM_Detect& detect)
{
	detect.ChipDesc = "";
	detect.ChipNum = 0;
	detect.Devices = 0;

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

	detect.bMultiChip = (detect.ChipNum > 1);

	return detect.ChipNum > 0;
}

//-----------------------------------------------------------------------------
//
u16 CountIncSequence(std::vector<lVGM_Chunk>::iterator& chunk, bool& bUnique)
{
	bUnique = true;
	u16 seqLen = 1;
	u8 regCnt = chunk->Register + 1;
	auto it = std::next(chunk);
	while ((it->Type == LVGM_CHUNK_REG) && (it->Chip == chunk->Chip) && (it->Register == regCnt))
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
//
void ExportPSG(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	assert(chunk->Type == LVGM_CHUNK_REG);
	assert(chunk->Chip == LVGM_CHIP_PSG);
	assert(chunk->Register < 16);

	// Check chip section
	if (g_lVGM_CurChip != LVGM_CHIP_PSG)
	{
		g_lVGM_CurChip = LVGM_CHIP_PSG;
		exp->AddByteLine(0xF0, "---- Start PSG section");
	}

	// Handle value 0 case
	u8 val = (u8)chunk->Value;
	if (val == 0)
	{
		exp->AddByteLine(0xD0 + chunk->Register, "R#n: Dn");
		return;
	}

	// Encode register access
	switch (chunk->Register)
	{
	case 0: // Register #0
		exp->AddByteList(std::vector<u8>{ 0x00, val }, "R#0: 00 nn");
		break;
	case 1: // Register #1
		exp->AddByteLine((u8)(0x10 + (val & 0x0F)), "R#1: 1n");
		break;
	case 2: // Register #2
		exp->AddByteList(std::vector<u8>{ 0x02, val }, "R#2: 02 nn");
		break;
	case 3: // Register #3
		exp->AddByteLine((u8)(0x20 + (val & 0x0F)), "R#3: 2n");
		break;
	case 4: // Register #4
		exp->AddByteList(std::vector<u8>{ 0x04, val }, "R#4: 04 nn");
		break;
	case 5: // Register #5
		exp->AddByteLine((u8)(0x30 + (val & 0x0F)), "R#5: 3n");
		break;
	case 6: // Register #6
		if (val <= 0x0F)
			exp->AddByteLine((u8)(0x40 + (val & 0x0F)), "R#6: 4n");
		else
			exp->AddByteLine((u8)(0x90 + (val & 0x0F)), "R#6: 9n | 0x10");
		break;
	case 7: // Register #7
		exp->AddByteList(std::vector<u8>{ 0x07, val }, "R#7: 07 nn");
		break;
	case 8: // Register #8
		if (val <= 0x0F)
			exp->AddByteLine((u8)(0x50 + (val & 0x0F)), "R#8: 5n");
		else
			exp->AddByteLine((u8)(0xA0 + (val & 0x0F)), "R#8: An | 0x10");
		break;
	case 9: // Register #9
		if (val <= 0x0F)
			exp->AddByteLine((u8)(0x60 + (val & 0x0F)), "R#9: 6n");
		else
			exp->AddByteLine((u8)(0xB0 + (val & 0x0F)), "R#9: Bn | 0x10");
		break;
	case 10: // Register #10
		if (val <= 0x0F)
			exp->AddByteLine((u8)(0x70 + (val & 0x0F)), "R#10: 7n");
		else
			exp->AddByteLine((u8)(0xC0 + (val & 0x0F)), "R#10: Cn | 0x10");
		break;
	case 11: // Register #11
		exp->AddByteList(std::vector<u8>{ 0x0B, val }, "R#11: 0B nn");
		break;
	case 12: // Register #12
		exp->AddByteList(std::vector<u8>{ 0x0C, val }, "R#12: 0C nn");
		break;
	case 13: // Register #13
		exp->AddByteLine((u8)(0x80 + (val & 0x0F)), "R#13: 8n");
		break;
	};
}

//-----------------------------------------------------------------------------
//
void AddSequence(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk, u16 count, u8 code, std::string comment)
{
	exp->StartLine();
	exp->AddByte(code);

	// hack for OPLL 7n code
	if ((chunk->Type == LVGM_CHUNK_REG) && (chunk->Chip == LVGM_CHIP_OPLL) && ((code & 0xF0) == 0x70))
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
//
void ExportOPLL(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	assert(chunk->Type == LVGM_CHUNK_REG);
	assert(chunk->Chip == LVGM_CHIP_OPLL);

	// Check chip section
	if (g_lVGM_CurChip != LVGM_CHIP_OPLL)
	{
		g_lVGM_CurChip = LVGM_CHIP_OPLL;
		exp->AddByteLine(0xF1, "---- Start OPLL section");
	}

	u8 reg = chunk->Register;
	u8 val = (u8)chunk->Value;

	// Check for sequence
	bool bUnique;
	u16 seqLen = CountIncSequence(chunk, bUnique);
	if ((reg == 0x00) && (seqLen == 8))
	{
		if (bUnique)
			exp->AddByteList(std::vector<u8>{ 0x50, val }, "50 nn => R#0~7");
		else
			AddSequence(exp, chunk, seqLen, 0x40, "40 nn[8] => R#0~7");
		chunk += seqLen - 1;
		return;
	}
	else if ((reg == 0x10) && (seqLen == 9))
	{
		if (bUnique)
			exp->AddByteList(std::vector<u8>{ 0x51, val }, "51 nn => R#10~18");
		else
			AddSequence(exp, chunk, seqLen, 0x41, "41 nn[9] => R#10~18");
		chunk += seqLen - 1;
		return;
	}
	else if ((reg == 0x20) && (seqLen == 9))
	{
		if (bUnique)
			exp->AddByteList(std::vector<u8>{ 0x52, val }, "52 nn => R#20~28");
		else
			AddSequence(exp, chunk, seqLen, 0x42, "42 nn[9] => R#20~28");
		chunk += seqLen - 1;
		return;
	}
	else if ((reg == 0x30) && (seqLen == 9))
	{
		if (bUnique)
			exp->AddByteList(std::vector<u8>{ 0x53, val }, "53 nn => R#30~38");
		else
			AddSequence(exp, chunk, seqLen, 0x43, "43 nn[9] => R#30~38");
		chunk += seqLen - 1;
		return;
	}
	else if ((seqLen >= 3) && (seqLen <= 7))
	{
		if (bUnique)
			exp->AddByteList(std::vector<u8>{ (u8)(0x60 + seqLen - 3), val }, "6n rr vv => R#rr~");
		else
			AddSequence(exp, chunk, seqLen, (u8)(0x70 + seqLen - 3), "7n rr vv[] => R#rr~");
		chunk += seqLen - 1;
		return;
	}

	if(val)
		exp->AddByteList(std::vector<u8>{ chunk->Register, val }, "rr: nn");
	else
		exp->AddByteLine(chunk->Register | 0x80, "rr: 0");
}
	
//-----------------------------------------------------------------------------
//
void ExportOPL1(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	if (g_lVGM_CurChip != LVGM_CHIP_OPL1)
	{
		g_lVGM_CurChip = LVGM_CHIP_OPL1;
		exp->AddByteLine(0xF2, "---- Start OPL1 section");
	}

	exp->AddByteList(std::vector<u8>{ chunk->Register, (u8)chunk->Value }, "");
}

//-----------------------------------------------------------------------------
//
void ExportSCC(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	if (g_lVGM_CurChip != LVGM_CHIP_SCC)
	{
		g_lVGM_CurChip = LVGM_CHIP_SCC;
		exp->AddByteLine(0xF3, "---- Start SCC section");
	}

	exp->AddByteList(std::vector<u8>{ chunk->Port, chunk->Register, (u8)chunk->Value }, "");
}

//-----------------------------------------------------------------------------
//
void ExportOPL4(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	if (g_lVGM_CurChip != LVGM_CHIP_OPL4)
	{
		g_lVGM_CurChip = LVGM_CHIP_OPL4;
		exp->AddByteLine(0xF7, "---- Start OPL4 section");
	}

	exp->AddByteList(std::vector<u8>{ chunk->Port, chunk->Register, (u8)chunk->Value }, "");
}


//-----------------------------------------------------------------------------
//
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
	exp->AddComment(MSX::Format("VGM version: %01x.%02x (chips:%s)", (g_VGM_Header->Version >> 8) & 0xF, g_VGM_Header->Version & 0x3, detect.ChipDesc.c_str()));

	//............................................................................
	// Parse data

	g_lVGM_CurChip = LVGM_CHIP_PSG;
	bool bLoop = false;
	u32 sampleFrame = (g_lVGM_Frequency == LVGM_FREQ_60HZ) ? VGM_WAIT_60HZ : VGM_WAIT_50HZ;
	u32 sampleCount = 0;
	std::vector<lVGM_Chunk> chunkList;

	while (1)
	{
		//-----------------------------------------------------------------------------
		if (*g_VGM_Pointer == VGM_CMD_AY8910) // PSG/AY8910, write value dd to register aa
		{
			u8 reg = g_VGM_Pointer[1];
			u8 val = g_VGM_Pointer[2];

			chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_PSG, 0, reg, val));

			g_VGM_Pointer += 2;
		}
		//-----------------------------------------------------------------------------
		else if (*g_VGM_Pointer == VGM_CMD_YM2413) // MSX-MUSIC/YM2413/OPLL, write value dd to register aa
		{
			u8 reg = g_VGM_Pointer[1];
			u8 val = g_VGM_Pointer[2];

			chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_OPLL, 0, reg, val));

			g_VGM_Pointer += 2;
		}
		//-----------------------------------------------------------------------------
		else if (*g_VGM_Pointer == VGM_CMD_Y8950) // MSX-AUDIO/Y8950/OPL1, write value dd to register aa
		{
			u8 reg = g_VGM_Pointer[1];
			u8 val = g_VGM_Pointer[2];

			chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_OPL1, 0, reg, val));

			g_VGM_Pointer += 2;
		}
		//-----------------------------------------------------------------------------
		else if (*g_VGM_Pointer == VGM_CMD_SCC) // SCC/SCC+/K051649/K052539, port pp, write value dd to register aa
		{
			u8 prt = g_VGM_Pointer[1];
			u8 reg = g_VGM_Pointer[2];
			u8 val = g_VGM_Pointer[3];

			chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_SCC, prt, reg, val));

			g_VGM_Pointer += 3;
		}
		//-----------------------------------------------------------------------------
		else if (*g_VGM_Pointer == VGM_CMD_YMF278) // MOONSOUND/YMF278B/OPL4, port pp, write value dd to register aa
		{
			u8 prt = g_VGM_Pointer[1];
			u8 reg = g_VGM_Pointer[2];
			u8 val = g_VGM_Pointer[3];

			chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_OPL4, prt, reg, val));

			g_VGM_Pointer += 3;
		}
		//-----------------------------------------------------------------------------
		else if (*g_VGM_Pointer == 0x61) // Wait n samples, n can range from 0 to 65535 (approx 1.49 seconds). Longer pauses than this are represented by multiple wait commands.
		{
			u16 nn = *(u16*)(g_VGM_Pointer + 1);
			sampleCount += nn;
			g_VGM_Pointer += 2;
		}
		else if (*g_VGM_Pointer == 0x62) // Wait 735 samples (60th of a second), a shortcut for 0x61 0xdf 0x02
		{
			sampleCount += VGM_WAIT_60HZ;
		}
		else if (*g_VGM_Pointer == 0x63) // Wait 882 samples (50th of a second), a shortcut for 0x61 0x72 0x03
		{
			sampleCount += VGM_WAIT_50HZ;
		}
		else if (*g_VGM_Pointer == 0x66) // End of sound data
		{
			chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_END));
			break;
		}
		else if ((*g_VGM_Pointer & 0xF0) == 0x70) // wait n+1 samples, n can range from 0 to 15.
		{
			u8 n = 1 + (*g_VGM_Pointer & 0xF);
			sampleCount += n;
		}
		else
		{
			printf("Warning: Invalid command! %02x\n", *g_VGM_Pointer);
		}
		g_VGM_Pointer++;

		if (!bLoop && (g_VGM_Pointer >= g_VGM_Loop))
		{
			bLoop = true;
			chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_LOOP));
		}

		// Check for end of 50/60 Hz frame
		if (sampleCount >= sampleFrame)
		{
			u16 val = sampleCount / sampleFrame;
			chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_FRAME, LVGM_CHIP_NONE, 0, 0, val));
			sampleCount %= sampleFrame;
		}
	}

	//............................................................................
	// Start export
	exp->StartSection(name);

	// Add header
	if (g_lVGM_AddHeader)
	{
		exp->AddComment("---- Header ----");

		// -- Ident --
		exp->AddByteList(std::vector<u8>{ 'l', 'V', 'G', 'M' }, "Ident \"lVGM\"");

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
		exp->AddByteLine(flag, MSX::Format("Options (freq:%s loop:%i dev:%i)", (flag & LVGM_OPTION_50HZ) ? "50Hz" : "60Hz", (flag & LVGM_OPTION_LOOP) ? 1 : 0, (flag & LVGM_OPTION_DEVICE) ? 1 : 0));

		// -- Devices --
		if (detect.Devices != LVGM_CHIP_PSG)
			exp->AddByteLine(detect.Devices, MSX::Format("Devices (chips:%s)", detect.ChipDesc.c_str()));

		exp->AddComment("---- Data ----");
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
				ExportPSG(exp, chunk);
				break;
			case LVGM_CHIP_OPLL:
				ExportOPLL(exp, chunk);
				break;
			case LVGM_CHIP_OPL1:
				ExportOPL1(exp, chunk);
				break;
			case LVGM_CHIP_SCC:
				ExportSCC(exp, chunk);
				break;
			case LVGM_CHIP_OPL4:
				ExportOPL4(exp, chunk);
				break;
			}
			break;
		case LVGM_CHUNK_FRAME:
		{
			u16 pack = chunk->Value / 16;
			for (u32 i = 0; i < pack; i++)
				exp->AddByteList(std::vector<u8>{ 0xEF }, "-------- Wait: Dn");
			exp->AddByteList(std::vector<u8>{ (u8)(0xE0 + ((chunk->Value - 1) & 0x0F)) }, "-------- Wait: Dn");
		}
			break;
		case LVGM_CHUNK_LOOP:
			exp->AddByteLine(0xFE, "Loop marker");
			break;
		case LVGM_CHUNK_END:
			exp->AddByteLine(0xFF, "End marker");
			break;
		}
	}

	// Close section
	exp->EndSection();

	return true;
}

#if 0
//-----------------------------------------------------------------------------
//
void Flush(MSX::ExporterInterface* exp)
{
	if (g_Chunk.size() == 0)
		return;

	if (g_Chunk.size() == 1)
	{
		const lVGM_Chunk& chunk = g_Chunk.back();
		exp->AddByteList(std::vector<u8>{ 0xF3, chunk.Register, chunk.Value}, "SCC 0x00+aa: nn");
	}
	else
	{
		const lVGM_Chunk& chunk = g_Chunk.front();
		exp->AddByteList(std::vector<u8>{ 0xF5, chunk.Register, (u8)g_Chunk.size() }, "SCC nn[bb] -> (0x00 + aa)[bb]");
		exp->StartLine();
		for (u32 i = 0; i < g_Chunk.size(); i++)
			exp->AddByte(g_Chunk[i].Value);
		exp->EndLine();
	}

	g_Chunk.resize(0);
}

//-----------------------------------------------------------------------------
//
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
	if (!DetectDevices(detect))
	{
		printf("Error: No supported sound chip detected!\n");
		return false;
	}

	// File info
	exp->AddComment(MSX::Format("VGM version: %01x.%02x (chips:%s)", (g_VGM_Header->Version >> 8) & 0xF, g_VGM_Header->Version & 0x3, detect.ChipDesc.c_str()));

	//............................................................................
	// Start export
	exp->StartSection(name);

	//............................................................................
	// Add header
	if (g_lVGM_AddHeader)
	{
		// -- Ident --
		exp->AddByteList(std::vector<u8>{ 'l', 'V', 'G', 'M' }, "Ident");

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
		exp->AddByteLine(flag, "Options");

		// -- Devices --
		if (detect.Devices != LVGM_CHIP_PSG)
		{
			exp->AddByteLine(detect.Devices, "Devices");
		}
	}

	u8 curChip = LVGM_CHIP_PSG;
	u16 lastAddr = 0;
	u16 loopAddr = 0;
	u16 count50 = 0;
	u16 count60 = 0;

	//............................................................................
	// Parse data
	while (1)
	{
		//-----------------------------------------------------------------------------
		if (*g_VGM_Pointer == 0xA0) // AY8910, write value dd to register aa
		{
			if (curChip != LVGM_CHIP_PSG)
			{
				curChip = LVGM_CHIP_PSG;
				exp->AddByteLine(0xF0, "Devices");
			}

			lVGM_Seq seq;
			u8 count = CountSequence(*g_VGM_Pointer, g_VGM_Pointer, seq);

			lastAddr = (u16)exp->GetTotalSize();
			u8 reg = g_VGM_Pointer[1];
			u8 val = g_VGM_Pointer[2];
			switch (reg)
			{
			case 0: // Register #0
				Flush(exp);
				exp->AddByteList(std::vector<u8>{ 0x00, val }, "R#0: 00 nn");
				break;
			case 1: // Register #1
				Flush(exp);
				exp->AddByteLine((u8)(0x10 + (val & 0x0F)), "R#1: 1n");
				break;
			case 2: // Register #2
				Flush(exp);
				exp->AddByteList(std::vector<u8>{ 0x02, val }, "R#2: 02 nn");
				break;
			case 3: // Register #3
				Flush(exp);
				exp->AddByteLine((u8)(0x20 + (val & 0x0F)), "R#3: 2n");
				break;
			case 4: // Register #4
				Flush(exp);
				exp->AddByteList(std::vector<u8>{ 0x04, val }, "R#4: 04 nn");
				break;
			case 5: // Register #5
				Flush(exp);
				exp->AddByteLine((u8)(0x30 + (val & 0x0F)), "R#5: 3n");
				break;
			case 6: // Register #6
				if (val <= 0x0F)
				{
					Flush(exp);
					exp->AddByteLine((u8)(0x40 + (val & 0x0F)), "R#6: 4n");
				}
				else
				{
					Flush(exp);
					exp->AddByteLine((u8)(0x50 + (val & 0x0F)), "R#6: 5n | 0x10");
				}
				break;
			case 7: // Register #7
				Flush(exp);
				exp->AddByteList(std::vector<u8>{ 0x07, val }, "R#7: 07 nn");
				break;
			case 8: // Register #8
				if (val <= 0x0F)
				{
					Flush(exp);
					exp->AddByteLine((u8)(0x60 + (val & 0x0F)), "R#8: 6n");
				}
				else
				{
					Flush(exp);
					exp->AddByteLine((u8)(0x70 + (val & 0x0F)), "R#8: 7n | 0x10");
				}
				break;
			case 9: // Register #9
				if (val <= 0x0F)
				{
					Flush(exp);
					exp->AddByteLine((u8)(0x80 + (val & 0x0F)), "R#9: 8n");
				}
				else
				{
					Flush(exp);
					exp->AddByteLine((u8)(0x90 + (val & 0x0F)), "R#9: 9n | 0x10");
				}
				break;
			case 10: // Register #10
				if (val <= 0x0F)
				{
					Flush(exp);
					exp->AddByteLine((u8)(0xA0 + (val & 0x0F)), "R#10: An");
				}
				else
				{
					Flush(exp);
					exp->AddByteLine((u8)(0xB0 + (val & 0x0F)), "R#10: Bn | 0x10");
				}
				break;
			case 11: // Register #11
				Flush(exp);
				exp->AddByteList(std::vector<u8>{ 0x0B, val }, "R#11: 0B nn");
				break;
			case 12: // Register #12
				Flush(exp);
				exp->AddByteList(std::vector<u8>{ 0x0C, val }, "R#12: 0C nn");
				break;
			case 13: // Register #13
				Flush(exp);
				exp->AddBytLine((u8)(0xC0 + (val & 0x0F)), "R#13: Cn");
				break;
			};
			g_VGM_Pointer += 2;
		}
		//-----------------------------------------------------------------------------
		else if (*g_VGM_Pointer == 0xD2) // SCC1, port pp, write value dd to register aa
		{
			u8 val = g_VGM_Pointer[3];
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
			switch (reg)
			{
			case 0x80: // Frequency channel 1 - LSB
			case 0x82: // Frequency channel 2 - LSB
			case 0x84: // Frequency channel 3 - LSB
			case 0x86: // Frequency channel 4 - LSB
			case 0x88: // Frequency channel 5 - LSB
				Flush(exp);
				exp->AddByteList(std::vector<u8>{ (u8)(0xF0 + (reg & 0x0F)), val }, MSX::Format("SCC 0x%02X: nn", reg));
				break;
			case 0x81: // Frequency channel 1 - MSB
			case 0x83: // Frequency channel 2 - MSB
			case 0x85: // Frequency channel 3 - MSB
			case 0x87: // Frequency channel 4 - MSB
			case 0x89: // Frequency channel 5 - MSB
			case 0x8A: // Volume channel 1
			case 0x8B: // Volume channel 2
			case 0x8C: // Volume channel 3
			case 0x8D: // Volume channel 4
			case 0x8E: // Volume channel 5
				Flush(exp);
				exp->AddByteList(std::vector<u8>{ 0xF1, (u8)(((reg & 0x0F) << 4) | (val & 0x0F)) }, MSX::Format("SCC 0x%02X: n", reg));
				break;
			case 0x8F: // On/off switch channel 1 to 5
				if (val & 0x10)
				{
					Flush(exp);
					exp->AddByteList(std::vector<u8>{ 0xF1, (u8)(0xF0 | (val & 0x0F)) }, "SCC 0x8F: n | 0x10");
				}
				else
				{
					Flush(exp);
					exp->AddByteList(std::vector<u8>{ 0xF1, (u8)(0x00 | (val & 0x0F)) }, "SCC 0x8F: n");
				}
				break;
			default:

				if (g_Chunk.size() > 0)
				{
					const lVGM_Chunk& prevChunk = g_Chunk.back();
					if ((prevChunk.Chip != *g_VGM_Pointer) || (reg != prevChunk.Register + 1))
						Flush(exp);
				}

				lVGM_Chunk chunk;
				chunk.Chip = *g_VGM_Pointer;
				chunk.Register = reg;
				chunk.Value = val;
				g_Chunk.push_back(chunk);
				break;
			}

			g_VGM_Pointer += 3;
		}
		else if (*g_VGM_Pointer == 0x61) // Wait n samples, n can range from 0 to 65535 (approx 1.49 seconds). Longer pauses than this are represented by multiple wait commands.
		{
			u16 n = *(u16*)(g_VGM_Pointer + 1);
			count50 += n;
			count60 += n;
			g_VGM_Pointer += 2;
		}
		else if (*g_VGM_Pointer == 0x62) // Wait 735 samples (60th of a second), a shortcut for 0x61 0xdf 0x02
		{
			count50 += VGM_WAIT_60HZ;
			count60 += VGM_WAIT_60HZ;
		}
		else if (*g_VGM_Pointer == 0x63) // Wait 882 samples (50th of a second), a shortcut for 0x61 0x72 0x03
		{
			count50 += VGM_WAIT_50HZ;
			count60 += VGM_WAIT_50HZ;
		}
		else if (*g_VGM_Pointer == 0x66) // End of sound data
		{
			if (g_VGM_Loop)
			{
				Flush(exp);
				exp->AddByteList(std::vector<u8>{ 0xFE, (u8)loopAddr, (u8)(loopAddr >> 8) }, "End of music with loop");
			}
			else
			{
				Flush(exp);
				exp->AddByteList(std::vector<u8>{ 0xFF }, "End of music");
			}
			break;
		}
		else if ((*g_VGM_Pointer & 0xF0) == 0x70) // wait n+1 samples, n can range from 0 to 15.
		{
			count50 += 1 + (*g_VGM_Pointer & 0x0F);
			count60 += 1 + (*g_VGM_Pointer & 0x0F);
		}
		else
		{
		}
		g_VGM_Pointer++;

		if ((loopAddr == 0) && (g_VGM_Pointer >= g_VGM_Loop))
		{
			loopAddr = lastAddr;
		}

		// Check for end of 50 Hz frame
		if (g_lVGM_Frequency == LVGM_FREQ_50HZ)
		{
			if (count50 >= VGM_WAIT_50HZ)
			{
				Flush(exp);
				lastAddr = (u16)exp->GetTotalSize();
				u16 val = count50 / VGM_WAIT_50HZ;
				u16 pack = val / 16;
				for (u32 i = 0; i < pack; i++)
					exp->AddByteList(std::vector<u8>{ 0xDF }, "Wait: Dn");
				exp->AddByteList(std::vector<u8>{ (u8)(0xD0 + ((val - 1) & 0x0F)) }, "Wait: Dn");
				count50 %= VGM_WAIT_50HZ;
			}
		}

		// Check for end of 60 Hz frame
		if (g_lVGM_Frequency == LVGM_FREQ_60HZ)
		{
			if (count60 >= VGM_WAIT_60HZ)
			{
				Flush(exp);
				lastAddr = (u16)exp->GetTotalSize();
				u16 val = count60 / VGM_WAIT_60HZ;
				u16 pack = val / 16;
				for (u32 i = 0; i < pack; i++)
					exp->AddByteList(std::vector<u8>{ 0xDF }, "Wait: Dn");
				exp->AddByteList(std::vector<u8>{ (u8)(0xD0 + ((val - 1) & 0x0F)) }, "Wait: Dn");
				count60 %= VGM_WAIT_60HZ;
			}
		}
	}

	exp->EndSection();

	return true;
}
#endif