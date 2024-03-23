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
#include <map>

// MSXtk
#include "MSXtk.h"
#include "VGM.h"
#include "lVGM.h"

//=============================================================================
// DEFINES
//=============================================================================

//
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
LVGM_SIMPLIFY           g_lVGM_Simplify;
u32						g_lVGM_Split;

// Global variables
u8                      g_lVGM_CurChip;
u8                      g_lVGM_Default;

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
void CheckSplit(MSX::ExporterInterface* exp, u8 size)
{
	if (g_lVGM_Split)
	{
		u32 curSize = exp->GetTotalSize() - 1;
		u32 targetSize = curSize + size + 2;
		if ((targetSize / g_lVGM_Split) > (curSize / g_lVGM_Split))
		{
			exp->AddByteList(std::vector<u8>{ 0xFD, 0x00 }, "======== End of data segment");
			u8 padding = g_lVGM_Split - (exp->GetTotalSize() % g_lVGM_Split);
			loopx(padding)
				exp->AddByteLine(0x00, "Padding");
		}
	}
}

//-----------------------------------------------------------------------------
//
void AddByte(MSX::ExporterInterface* exp, u8 value, std::string comment = "")
{
	CheckSplit(exp, 1);

	exp->AddByteLine(value, comment);
}

//-----------------------------------------------------------------------------
//
void AddList(MSX::ExporterInterface* exp, std::vector<u8> list, std::string comment = "")
{
	CheckSplit(exp, (u8)list.size());

	exp->AddByteList(list, comment);
}

//-----------------------------------------------------------------------------
//
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
		AddByte(exp, 0xF0, "---- Start PSG section");
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
//
void ExportOPLL(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	assert(chunk->Type == LVGM_CHUNK_REG);
	assert(chunk->Chip == LVGM_CHIP_OPLL);

	// Check chip section
	if (g_lVGM_CurChip != LVGM_CHIP_OPLL)
	{
		g_lVGM_CurChip = LVGM_CHIP_OPLL;
		AddByte(exp, 0xF1, "---- Start OPLL section");
	}

	u8 reg = chunk->Register;
	u8 val = (u8)chunk->Value;

	// Check for sequence
	bool bUnique;
	u16 seqLen = CountIncSequence(chunk, bUnique);
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
	else if ((seqLen >= 3) && (seqLen <= 7))
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
//
void ExportOPL1(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	assert(chunk->Type == LVGM_CHUNK_REG);
	assert(chunk->Chip == LVGM_CHIP_OPL1);

	// Check chip section
	if (g_lVGM_CurChip != LVGM_CHIP_OPL1)
	{
		g_lVGM_CurChip = LVGM_CHIP_OPL1;
		AddByte(exp, 0xF2, "---- Start OPL1 section");
	}

	u8 reg = chunk->Register;
	u8 val = (u8)chunk->Value;

	// Check for sequence
	/*bool bUnique;
	u16 seqLen = CountIncSequence(chunk, bUnique);
	if ((seqLen >= 3) && (seqLen <= 18))
	{
		AddSequence(exp, chunk, seqLen, (u8)(0xD0 + seqLen - 3), "Dx yy zz[x] => R#yy~", true);
		chunk += seqLen - 1;
		return;
	}*/

	AddList(exp, std::vector<u8>{ reg, val }, "R#xx = yy");

	// Yakyu		6931	6926
	// Xevious		7712	7599
}

//-----------------------------------------------------------------------------
//
void ExportSCC(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	assert(chunk->Type == LVGM_CHUNK_REG);
	assert(chunk->Chip == LVGM_CHIP_SCC);

	// Check chip section
	if (g_lVGM_CurChip != LVGM_CHIP_SCC)
	{
		g_lVGM_CurChip = LVGM_CHIP_SCC;
		AddByte(exp, 0xF3, "---- Start SCC section");
	}

	u8 reg = chunk->Register;
	u8 val = (u8)chunk->Value;

	// Check for sequence
	bool bUnique;
	u16 seqLen = CountIncSequence(chunk, bUnique);
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
	else if ((seqLen >= 3) && (seqLen <= 18))
	{
		if (bUnique)
			AddList(exp, std::vector<u8>{ (u8)(0xC0 + seqLen - 3), reg, val }, "Cn rr vv => R#rr~");
		else
			AddSequence(exp, chunk, seqLen, (u8)(0xD0 + seqLen - 3), "Dn rr vv[] => R#rr~", true);
		chunk += seqLen - 1;
		return;
	}

	AddList(exp, std::vector<u8>{ reg, (u8)chunk->Value }, "#rr = nn");
}

//-----------------------------------------------------------------------------
//
void ExportSCCI(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	if (g_lVGM_CurChip != LVGM_CHIP_SCCI)
	{
		g_lVGM_CurChip = LVGM_CHIP_SCCI;
		AddByte(exp, 0xF7, "---- Start SCC+ section");
	}

	AddList(exp, std::vector<u8>{ chunk->Port, chunk->Register, (u8)chunk->Value }, "");
}


//-----------------------------------------------------------------------------
//
void ExportOPL4(MSX::ExporterInterface* exp, std::vector<lVGM_Chunk>::iterator& chunk)
{
	if (g_lVGM_CurChip != LVGM_CHIP_OPL4)
	{
		g_lVGM_CurChip = LVGM_CHIP_OPL4;
		AddByte(exp, 0xF7, "---- Start OPL4 section");
	}

	AddList(exp, std::vector<u8>{ chunk->Port, chunk->Register, (u8)chunk->Value }, "");
}


//-----------------------------------------------------------------------------
//
void Simplify(std::vector<lVGM_Chunk>& chunkList)
{
	std::vector<lVGM_Chunk> workList;
	std::map<u32, lVGM_Chunk> workMap;

	u32 count = 0;
	for (const auto& chunk : chunkList)
	{
		switch (chunk.Type)
		{
		case LVGM_CHUNK_REG:
			if (g_lVGM_Simplify == LVGM_SIMPLIFY_ORDER)
			{
				u32 id = (chunk.Port << 24) + (chunk.Register << 16) + (count++ & 0xFFFF);
				workMap[id] = chunk;
			}
			else //if (g_lVGM_Simplify == LVGM_SIMPLIFY_DUPLICATE)
			{
				u32 id = ((u8)chunk.Chip << 16) + (chunk.Port << 8) + chunk.Register;
				workMap.insert_or_assign(id, chunk);
			}
			//else
			//	workList.push_back(chunk);
			break;
		case LVGM_CHUNK_LOOP:
			workList.push_back(chunk);
			break;
		case LVGM_CHUNK_FRAME:
		case LVGM_CHUNK_END:
			if (workMap.size())
			{
				for (const auto& pair : workMap)
					workList.push_back(pair.second);
				workMap.clear();
				count = 0;
			}
			workList.push_back(chunk);
			break;
		}
	}

	chunkList = workList;
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
	g_lVGM_Default = 0;
	bool bLoop = false;
	u32 sampleFrame = (g_lVGM_Frequency == LVGM_FREQ_60HZ) ? VGM_WAIT_60HZ : VGM_WAIT_50HZ;
	u32 sampleCount = 0;
	std::vector<lVGM_Chunk> chunkList;
	std::map<u8, u16> psgValFreq;

	// Parse input data
	while (1)
	{
		//-----------------------------------------------------------------------------
		if (*g_VGM_Pointer == VGM_CMD_AY8910) // PSG/AY8910, write value dd to register aa
		{
			u8 reg = g_VGM_Pointer[1];
			u8 val = g_VGM_Pointer[2];
			g_VGM_Pointer += 2;

			switch (reg)
			{
			case 7:
				val = (val & 0b00111111) | 0b10000000; // Validate register values
			case 0:
			case 2:
			case 4:
			case 11:
			case 12:
				psgValFreq[val]++; // increment value counter
				break;
			}

			chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_PSG, 0, reg, val));
		}
		//-----------------------------------------------------------------------------
		else if (*g_VGM_Pointer == VGM_CMD_YM2413) // MSX-MUSIC/YM2413/OPLL, write value dd to register aa
		{
			u8 reg = g_VGM_Pointer[1];
			u8 val = g_VGM_Pointer[2];
			g_VGM_Pointer += 2;

			chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_OPLL, 0, reg, val));
		}
		//-----------------------------------------------------------------------------
		else if (*g_VGM_Pointer == VGM_CMD_Y8950) // MSX-AUDIO/Y8950/OPL1, write value dd to register aa
		{
			u8 reg = g_VGM_Pointer[1];
			u8 val = g_VGM_Pointer[2];
			g_VGM_Pointer += 2;

			if ((reg != 0x04) && (reg != 0x18) && (reg != 0x19))
			{
				if (reg == 0x08)
					val &= 0b11000100;

				chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_OPL1, 0, reg, val));
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
				// Check for mirrored register
				if ((reg >= 0xB0) && (reg <= 0xBF))
					reg -= 0x10;

				// Skip 'no function' and 'test register' access
				if (reg >= 0xC0)
					break;

				chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_SCCI, 0, reg, val));
			}
			else // SCC
			{
				// Check for mirrored register
				if ((reg >= 0x90) && (reg <= 0x9F))
					reg -= 0x10;

				// Skip 'no function' and 'test register' access
				if (reg >= 0xA0)
					break;

				chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_SCC, 0, reg, val));
			}
		}
		//-----------------------------------------------------------------------------
		else if (*g_VGM_Pointer == VGM_CMD_YMF278) // MOONSOUND/YMF278B/OPL4, port pp, write value dd to register aa
		{
			u8 port = g_VGM_Pointer[1];
			u8 reg  = g_VGM_Pointer[2];
			u8 val  = g_VGM_Pointer[3];
			g_VGM_Pointer += 3;

			chunkList.push_back(lVGM_Chunk(LVGM_CHUNK_REG, LVGM_CHIP_OPL4, port, reg, val));
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
	// More common byte for PSG
	if (detect.Devices & LVGM_CHIP_PSG)
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

	// Remove dulicate and/or reorder register write
	if (g_lVGM_Simplify != LVGM_SIMPLIFY_NONE)
	{
		Simplify(chunkList);
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
			case LVGM_CHIP_SCCI:
				ExportSCCI(exp, chunk);
				break;
			case LVGM_CHIP_OPL4:
				ExportOPL4(exp, chunk);
				break;
			default:
				break;
			}
			break;

		case LVGM_CHUNK_FRAME:
		{
			u16 pack = chunk->Value / 16;
			for (u32 i = 0; i < pack; i++)
				AddList(exp, std::vector<u8>{ 0xEF }, "-------- Wait: Ex");
			AddList(exp, std::vector<u8>{ (u8)(0xE0 + ((chunk->Value - 1) & 0x0F)) }, "-------- Wait: Ex");
			break;
		}

		case LVGM_CHUNK_LOOP:
			AddByte(exp, 0xFE, "Loop marker");
			break;

		case LVGM_CHUNK_END:
			AddByte(exp, 0xFF, "End marker");
			break;
		}
	}

	// Close section
	exp->EndSection();

	return true;
}