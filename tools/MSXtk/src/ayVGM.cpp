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

// MSXtk
#include "MSXtk.h"
#include "ayVGM.h"

//=============================================================================
// DEFINES
//=============================================================================

#define VGM_IDENT *(u32*)"Vgm "

#define VGM_WAIT_60HZ	735 // 60 Hz
#define VGM_WAIT_50HZ	882 // 50 Hz

extern std::string		g_TableName;

// VGM header file data structure
struct VGM_Header
{
	u32 Ident;				u32 EoF_offset;			u32 Version;			u32 SN76489_clock;
	u32 YM2413_clock;		u32 GD3_offset;			u32 Total_samples;		u32 Loop_offset;
	u32 Loop_samples;		u32 Rate;				u16 SN_FB;
	u8  SNW;
	u8  SF;					u32 YM2612_clock;
	u32 YM2151_clock;		u32 Data_offset;		u32 Sega_PCM_clock;		u32 SPCM_Interface;
	u32 RF5C68_clock;		u32 YM2203_clock;		u32 YM2608_clock;		u32 YM2610B_clock;
	u32 YM3812_clock;		u32 YM3526_clock;		u32 Y8950_clock;		u32 YMF262_clock;
	u32 YMF278B_clock;		u32 YMF271_clock;		u32 YMZ280B_clock;		u32 RF5C164_clock;
	u32 PWM_clock;			u32 AY8910_clock;		u8  AYT;
	u8  AY_Flags[3];		u8  VM;
	u8  dummy7D;
	u8  LB;
	u8  LM;
	u32 GB_DMG_clock;		u32 NES_APU_clock;		u32 MultiPCM_clock;		u32 uPD7759_clock;
	u32 OKIM6258_clock;		u8  OF;
	u8  KF;
	u8  CF;
	u8  dummy97;			u32 OKIM6295_clock;		u32 K051649_clock;
	u32 K054539_clock;		u32 HuC6280_clock;		u32 C140_clock;			u32 K053260_clock;
	u32 Pokey_clock;		u32 QSound_clock;		u32 SCSP_clock;			u32 Extra_Hdr_ofs;
	u32 WonderSwan_clock;	u32 VSU_clock;			u32 SAA1099_clock;		u32 ES5503_clock;
	u32 ES5506_clock;		u16 ES_chns;
	u8  CD;
	u8  dummyD7;			u32 X1_010_clock;		u32 C352_clock;
	u32 GA20_clock;			u32 dummyE4;			u32 dummyE8;			u32 dummyEC;
	u32 dummyF0;			u32 dummyF4;			u32 dummyF8;			u32 dummyFC;
};

// ayVGM options
extern AYVGM_FREQ g_ayVGM_Frequency;

//
struct VGM_Chunk {
	u8 Ident;
	u8 Register;
	u8 Value;
};

//=============================================================================
// VARIABLES
//=============================================================================

const VGM_Header* g_VGM_Header;
const u8* g_VGM_Pointer;
const u8* g_VGM_Loop;
u16       g_VGM_Wait;
std::vector<VGM_Chunk> g_Chunk;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void Flush(MSX::ExporterInterface* exp)
{
	if (g_Chunk.size() == 0)
		return;

	if (g_Chunk.size() == 1)
	{
		const VGM_Chunk& chunk = g_Chunk.back();
		exp->AddByteList(std::vector<u8>{ 0xF3, chunk.Register, chunk.Value}, "SCC 0x00+aa: nn");
	}
	else
	{
		const VGM_Chunk& chunk = g_Chunk.front();
		exp->AddByteList(std::vector<u8>{ 0xF5, chunk.Register, (u8)g_Chunk.size() }, "SCC nn[bb] -> (0x00 + aa)[bb]");
		exp->StartLine();
		for(u32 i = 0; i < g_Chunk.size(); i++)
			exp->AddByte(g_Chunk[i].Value);
		exp->EndLine();
	}

	g_Chunk.resize(0);
}


//-----------------------------------------------------------------------------
//
bool ExportAyVGM(MSX::ExporterInterface* exp, const std::vector<u8>& data)
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

	u16 lastAddr = 0;
	u16 loopAddr = 0;

	exp->StartSection(g_TableName);

	// Add header
	// -- Ident --
	exp->AddByteList(std::vector<u8>{ 'a', 'y', 'M', ' ' }, "Ident");
	// -- Setting --
	u8 flag = 0;
	if ((g_ayVGM_Frequency == AYVGM_FREQ_50HZ) || (g_ayVGM_Frequency == AYVGM_FREQ_BOTH))
		flag |= AYVGM_FLAG_50HZ;
	if ((g_ayVGM_Frequency == AYVGM_FREQ_60HZ) || (g_ayVGM_Frequency == AYVGM_FREQ_BOTH))
		flag |= AYVGM_FLAG_60HZ;
	if (g_VGM_Header->Loop_offset)
		flag |= AYVGM_FLAG_LOOP;
	// Extension
	if (g_VGM_Header->K051649_clock && (g_VGM_Header->K051649_clock & (1 << 31)))
		flag |= AYVGM_FLAG_SCCI;
	else if (g_VGM_Header->K051649_clock)
		flag |= AYVGM_FLAG_SCC;

	exp->StartLine();
	exp->AddByte(flag);
	exp->EndLine("Flag");

	u16 count50 = 0;
	u16 count60 = 0;
	while (1)
	{
		//-----------------------------------------------------------------------------
		if (*g_VGM_Pointer == 0xA0) // AY8910, write value dd to register aa
		{
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
				exp->AddByteList(std::vector<u8>{ (u8)(0x10 + (val & 0x0F)) }, "R#1: 1n");
				break;
			case 2: // Register #2
				Flush(exp);
				exp->AddByteList(std::vector<u8>{ 0x02, val }, "R#2: 02 nn");
				break;
			case 3: // Register #3
				Flush(exp);
				exp->AddByteList(std::vector<u8>{ (u8)(0x20 + (val & 0x0F)) }, "R#3: 2n");
				break;
			case 4: // Register #4
				Flush(exp);
				exp->AddByteList(std::vector<u8>{ 0x04, val }, "R#4: 04 nn");
				break;
			case 5: // Register #5
				Flush(exp);
				exp->AddByteList(std::vector<u8>{ (u8)(0x30 + (val & 0x0F)) }, "R#5: 3n");
				break;
			case 6: // Register #6
				if(val <= 0x0F)
				{
					Flush(exp);
					exp->AddByteList(std::vector<u8>{ (u8)(0x40 + (val & 0x0F)) }, "R#6: 4n");
				}
				else
				{
					Flush(exp);
					exp->AddByteList(std::vector<u8>{ (u8)(0x50 + (val & 0x0F)) }, "R#6: 5n | 0x10");
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
					exp->AddByteList(std::vector<u8>{ (u8)(0x60 + (val & 0x0F)) }, "R#8: 6n");
				}
				else
				{
					Flush(exp);
					exp->AddByteList(std::vector<u8>{ (u8)(0x70 + (val & 0x0F)) }, "R#8: 7n | 0x10");
				}
				break;
			case 9: // Register #9
				if (val <= 0x0F)
				{
					Flush(exp);
					exp->AddByteList(std::vector<u8>{ (u8)(0x80 + (val & 0x0F)) }, "R#9: 8n");
				}
				else
				{
					Flush(exp);
					exp->AddByteList(std::vector<u8>{ (u8)(0x90 + (val & 0x0F)) }, "R#9: 9n | 0x10");
				}
				break;
			case 10: // Register #10
				if (val <= 0x0F)
				{
					Flush(exp);
					exp->AddByteList(std::vector<u8>{ (u8)(0xA0 + (val & 0x0F)) }, "R#10: An");
				}
				else
				{
					Flush(exp);
					exp->AddByteList(std::vector<u8>{ (u8)(0xB0 + (val & 0x0F)) }, "R#10: Bn | 0x10");
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
				exp->AddByteList(std::vector<u8>{ (u8)(0xC0 + (val & 0x0F)) }, "R#13: Cn");
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
					const VGM_Chunk& prevChunk = g_Chunk.back();
					if ((prevChunk.Ident != *g_VGM_Pointer) || (reg != prevChunk.Register + 1))
						Flush(exp);
				}

				VGM_Chunk chunk;
				chunk.Ident = *g_VGM_Pointer;
				chunk.Register = reg;
				chunk.Value = val;
				g_Chunk.push_back(chunk);
				break;
			}

			g_VGM_Pointer += 3;
		}
		else if (*g_VGM_Pointer == 0x61) // Wait n samples, n can range from 0 to 65535 (approx 1.49 seconds). Longer pauses than this are represented by multiple wait commands.
		{
			count50 += *(u16*)(g_VGM_Pointer + 1);
			count60 += *(u16*)(g_VGM_Pointer + 1);
			g_VGM_Pointer += 2;
		}
		else if (*g_VGM_Pointer == 0x62) // Wait 735 samples (60th of a second), a shortcut for 0x61 0xdf 0x02
		{
			count50 += 735;
			count60 += 735;
		}
		else if (*g_VGM_Pointer == 0x63) // Wait 882 samples (50th of a second), a shortcut for 0x61 0x72 0x03
		{
			count50 += 882;
			count60 += 882;
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
		if ((g_ayVGM_Frequency == AYVGM_FREQ_50HZ) || (g_ayVGM_Frequency == AYVGM_FREQ_BOTH))
		{
			if (count50 >= VGM_WAIT_50HZ)
			{
				Flush(exp);
				lastAddr = (u16)exp->GetTotalSize();
				u16 val = count50 / VGM_WAIT_50HZ;
				u16 pack = val / 16;
				for(u32 i = 0; i < pack; i++)
					exp->AddByteList(std::vector<u8>{ 0xDF }, "Wait50: Dn");
				exp->AddByteList(std::vector<u8>{ (u8)(0xD0 + ((val - 1) & 0x0F)) }, "Wait50: Dn");
				count50 %= VGM_WAIT_50HZ;
			}
		}

		// Check for end of 60 Hz frame
		if ((g_ayVGM_Frequency == AYVGM_FREQ_60HZ) || (g_ayVGM_Frequency == AYVGM_FREQ_BOTH))
		{
			if (count60 >= VGM_WAIT_60HZ)
			{
				Flush(exp);
				lastAddr = (u16)exp->GetTotalSize();
				u16 val = count60 / VGM_WAIT_60HZ;
				u16 pack = val / 16;
				for (u32 i = 0; i < pack; i++)
					exp->AddByteList(std::vector<u8>{ 0xEF }, "Wait60: En");
				exp->AddByteList(std::vector<u8>{ (u8)(0xE0 + ((val - 1) & 0x0F)) }, "Wait60: En");
				count60 %= VGM_WAIT_60HZ;
			}
		}
	}

	exp->EndSection();

	return true;
}