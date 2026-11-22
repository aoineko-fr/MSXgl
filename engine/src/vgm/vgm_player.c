// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄  ▄▄▄  ▄ ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██   ██▀█
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▀  ▀█▄█ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// VGM replayer
//  https://vgmrips.net/wiki/VGM_Specification
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "vgm_player.h"
#include "bios_mainrom.h"
#if (VGM_USE_SCC)
#include "scc.h"
#endif
#if (VGM_USE_MSXMUSIC)
#include "msx-music.h"
#endif
#if (VGM_USE_MSXAUDIO)
#include "msx-audio.h"
#endif
#if (VGM_USE_OPL4)
#include "audio/opl4.h"
#endif
#if (VGM_USE_OPNA)
#include "audio/opna.h"
#endif

//=============================================================================
// DEFINES
//=============================================================================

#define VGM_IDENT *(u32*)"Vgm "

//=============================================================================
// MEMORY DATA
//=============================================================================

const VGM_Header* g_VGM_Header;
const u8*         g_VGM_Pointer;
const u8*         g_VGM_Loop;
u16               g_VGM_WaitCount;
u16               g_VGM_WaitFrame;
u8                g_VGM_State;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize VGM replayer
void VGM_Initialize()
{
	#if (VGM_USE_PSG)
	// Initialize PSG module
	#endif
	#if (VGM_USE_MSXMUSIC)
	MSXMusic_Initialize();
	#endif
	#if (VGM_USE_MSXAUDIO)
	MSXAudio_Initialize();
	#endif
	#if (VGM_USE_SCC)
	SCC_Initialize();
	#endif
	#if (VGM_USE_OPL4)
	OPL4_Initialize(OPL4_MODE_OPL4);
	#endif
	#if (VGM_USE_OPNA)
	OPNA_Initialize();
	#endif
}

//-----------------------------------------------------------------------------
// Mute all sound chips (playback state is not changed)
void VGM_Mute()
{
	#if (VGM_USE_PSG)
	PSG_Mute();
	#endif
	#if (VGM_USE_MSXMUSIC)
	MSXMusic_Mute();
	#endif
	#if (VGM_USE_MSXAUDIO)
	MSXAudio_Mute();
	#endif
	#if (VGM_USE_SCC)
	SCC_Mute();
	#endif
	#if (VGM_USE_OPL4)
	OPL4_Mute();
	#endif
	#if (VGM_USE_OPNA)
	OPNA_Mute();
	#endif
}

//-----------------------------------------------------------------------------
// Play a VGM data
bool VGM_Play(const void* addr, bool loop)
{
	VGM_Mute(); // Mute all sound chip

	g_VGM_Header = (VGM_Header*)addr;

	if (g_VGM_Header->Ident != VGM_IDENT)
		return FALSE;
	
	g_VGM_State = VGM_STATE_PLAY;
	if (loop)
		g_VGM_State |= VGM_STATE_LOOP;

	g_VGM_Pointer = (u8*)((u16)&g_VGM_Header->Data_offset + (u16)g_VGM_Header->Data_offset);
	if (g_VGM_Header->Loop_offset)
		g_VGM_Loop = (u8*)((u16)&g_VGM_Header->Loop_offset + (u16)g_VGM_Header->Loop_offset);
	else
		g_VGM_Loop = g_VGM_Pointer; // No loop point: loop the whole song
	g_VGM_WaitCount = 0;

	if (Sys_Is50Hz())
		VGM_SetFrequency50Hz();
	else
		VGM_SetFrequency60Hz();

	#if (VGM_USE_OPL4)
	// Initialize OPL2/3 (played on OPL4 in OPL4 mode to keep wave part accessible)
	if (VGM_ContainsOPL2())
	{
		g_VGM_State |= VGM_STATE_OPL2;
		OPL4_SetPart1Register(OPL4_REG_EXPANSION, OPL4_F_NEW | OPL4_F_NEW2);
		loop(i, 9) // Enable left and right outputs of all FM channels (OPL2 songs don't write CHA/CHB bits)
			OPL4_SetPart0Register(OPL4_REG_FBCNT + i, OPL4_F_CHA | OPL4_F_CHB);
	}
	else if (VGM_ContainsOPL3())
	{
		OPL4_SetPart1Register(OPL4_REG_EXPANSION, OPL4_F_NEW | OPL4_F_NEW2);
	}
	#endif

	return TRUE;
}

//-----------------------------------------------------------------------------
// Resume music playback
void VGM_Resume()
{
	// After a stop, restart from the muted state (like VGM_Play)
	if (g_VGM_State & VGM_STATE_STOP)
	{
		g_VGM_State &= ~VGM_STATE_STOP;
		g_VGM_State |= VGM_STATE_PLAY;
		return;
	}

	g_VGM_State |= VGM_STATE_PLAY;

	#if (VGM_USE_PAUSE)
	#if (VGM_USE_PSG && PSG_USE_RESUME)
	if (VGM_ContainsPSG())
		PSG_Resume();
	#endif
	#if (VGM_USE_MSXMUSIC && MSXMUSIC_USE_RESUME)
	if (VGM_ContainsMSXMusic())
		MSXMusic_Resume();
	#endif
	#if (VGM_USE_MSXAUDIO && MSXAUDIO_USE_RESUME)
	if (VGM_ContainsMSXAudio())
		MSXAudio_Resume();
	#endif
	#if (VGM_USE_SCC && SCC_USE_RESUME)
	if (VGM_ContainsSCC())
		SCC_Resume();
	#endif
	#if (VGM_USE_OPL4 && OPL4_USE_RESUME)
	if (VGM_ContainsOPL4())
		OPL4_Resume();
	#endif
	#if (VGM_USE_OPNA && OPNA_USE_RESUME)
	if (VGM_ContainsOPNA())
		OPNA_Resume();
	#endif
	#endif // (VGM_USE_PAUSE)
}

#if (VGM_USE_PAUSE)
//-----------------------------------------------------------------------------
// Pause music playback
void VGM_Pause()
{
	g_VGM_State &= ~VGM_STATE_PLAY;
	VGM_Mute();
}
#endif // (VGM_USE_PAUSE)

//-----------------------------------------------------------------------------
// Play a ayVGM data
void VGM_Stop()
{
	g_VGM_State &= ~VGM_STATE_PLAY;
	VGM_Mute();
	g_VGM_Pointer = (u8*)((u16)&g_VGM_Header->Data_offset + (u16)g_VGM_Header->Data_offset);
	g_VGM_WaitCount = 0;
	#if (VGM_USE_PAUSE)
	g_VGM_State |= VGM_STATE_STOP; // Chips state saved by the mute must not be restored
	#endif
}

//-----------------------------------------------------------------------------
// Decode a frame of VGM data
void VGM_Decode()
{
	if (!(g_VGM_State & VGM_STATE_PLAY))
		return;
	
	while (g_VGM_WaitCount < g_VGM_WaitFrame)
	{
		#if (VGM_USE_PSG)
		if (*g_VGM_Pointer == VGM_CMD_AY8910) // AY8910, write value dd to register aa
		{
			u8 reg = g_VGM_Pointer[1];
			u8 val = g_VGM_Pointer[2];
			if (reg == 13)
			{
				val &= 0x3F;
				val |= 0x80;
			}
			PSG_SetRegister(reg, g_VGM_Pointer[2]);
			g_VGM_Pointer += 2;
		} else 
		#endif
		#if (VGM_USE_SCC)
		if (*g_VGM_Pointer == VGM_CMD_SCC) // SCC1, port pp, write value dd to register aa
		{
			u8 reg = 0;
			switch (g_VGM_Pointer[1])
			{
			// case 0:	reg = 0x00;	break; // 0x00 - waveform
			case 1:	reg = 0x80;	break; // 0x01 - frequency
			case 2:	reg = 0x8A;	break; // 0x02 - volume
			case 3:	reg = 0x8F;	break; // 0x03 - key on/off
			case 4:	reg = 0xA0;	break; // 0x04 - waveform (0x00 used to do SCC access, 0x04 SCC+)
			case 5:	reg = 0xE0;	break; // 0x05 - test register
			}
			SCC_SetRegister(reg + g_VGM_Pointer[2], g_VGM_Pointer[3]);
			g_VGM_Pointer += 3;
		} else
		#endif
		#if (VGM_USE_MSXMUSIC)
		if (*g_VGM_Pointer == VGM_CMD_YM2413) // YM2413, write value dd to register aa
		{
			MSXMusic_SetRegister(g_VGM_Pointer[1], g_VGM_Pointer[2]);
			g_VGM_Pointer += 2;
		} else
		#endif
		#if (VGM_USE_OPL4)
		if (*g_VGM_Pointer == VGM_CMD_YMF278) // MOONSOUND/YMF278B/OPL4, port pp, write value dd to register aa
		{
			u8 port = g_VGM_Pointer[1];
			u8 reg = g_VGM_Pointer[2];
			switch(port)
			{
			case VGM_PORT_OPL4_PART0:
				OPL4_SetPart0Register(reg, g_VGM_Pointer[3]);
				break;
			case VGM_PORT_OPL4_PART1:
				OPL4_SetPart1Register(reg, g_VGM_Pointer[3]);
				break;
			case VGM_PORT_OPL4_WAVE:
				OPL4_SetWaveRegister(reg, g_VGM_Pointer[3]);
				if ((reg >= 0x08) && (reg <= 0x1F))
					OPL4_WaitLoad();
				break;
			}
			g_VGM_Pointer += 3;
		} else
		if ((*g_VGM_Pointer == VGM_CMD_YM3812) || (*g_VGM_Pointer == VGM_CMD_YMF262_0) || (*g_VGM_Pointer == VGM_CMD_YMF262_1)) // OPL2/3
 		{
			u8 reg = g_VGM_Pointer[1];
			u8 value = g_VGM_Pointer[2];
			if (*g_VGM_Pointer == VGM_CMD_YMF262_1) // Part 1
			{
				if ((reg == OPL4_REG_EXPANSION) && (value & OPL4_F_NEW)) // OPL3 mode: keep NEW2 set (OPL4 mode)
					value = OPL4_F_NEW | OPL4_F_NEW2;
				OPL4_SetPart1Register(reg, value);
			}
			else if (reg != 0x01) // Part 0 (R#01 is the LSI test register on OPL4)
			{
				if (g_VGM_State & VGM_STATE_OPL2)
				{
					if ((reg >= OPL4_REG_FBCNT) && (reg <= OPL4_REG_FBCNT + 8)) // OPL2 has no CHA/CHB bits: output to both left and right channels
						value = (value & 0x0F) | OPL4_F_CHA | OPL4_F_CHB;
					else if ((reg >= 0xE0) && (reg <= 0xF5)) // OPL2 has only 4 waveforms
						value &= 0x03;
				}
				OPL4_SetPart0Register(reg, value);
			}
			g_VGM_Pointer += 2;
		} else
		#endif
		#if (VGM_USE_OPNA)
		if (*g_VGM_Pointer == VGM_CMD_YM2608_0) // YM2608 port 0, write value dd to register aa
		{
			u8 reg = g_VGM_Pointer[1];
			u8 value = g_VGM_Pointer[2];
			if (reg == 0x27) // Timer control: keep only CH3 mode
				value &= 0xC0;
			else if (reg == 0x29) // IRQ enable: keep only SCH (6 FM channels)
				value &= 0x80;
			OPNA_SetPart0Register(reg, value);
			if (reg == 0x10) // Rhythm key on/off: wait before next write
				OPNA_WaitBusy();
			g_VGM_Pointer += 2;
		}
		else if (*g_VGM_Pointer == VGM_CMD_YM2608_1) // YM2608 port 1, write value dd to register aa
		{
			OPNA_SetPart1Register(g_VGM_Pointer[1], g_VGM_Pointer[2]);
			g_VGM_Pointer += 2;
		} else
		#endif
		#if (VGM_USE_MSXAUDIO)
		if (*g_VGM_Pointer == VGM_CMD_Y8950) // Y8950, write value dd to register aa
		{
			u8 reg = g_VGM_Pointer[1];
			if ((reg != 0x04) && (reg != 0x18) && (reg != 0x19))
			{
				u8 value = g_VGM_Pointer[2];
				if (reg == 0x08)
					value &= 0b11000100;
				MSXAudio_SetRegister(reg, value);
			}
			g_VGM_Pointer += 2;
		} else
		#endif
		if (*g_VGM_Pointer == VGM_CMD_WAIT_16B) // Wait n samples, n can range from 0 to 65535 (approx 1.49 seconds). Longer pauses than this are represented by multiple wait commands.
		{
			g_VGM_WaitCount += *(u16*)(g_VGM_Pointer+1);
			g_VGM_Pointer += 2;
		}
		else if (*g_VGM_Pointer == VGM_CMD_WAIT_60HZ) // Wait 735 samples (60th of a second), a shortcut for 0x61 0xdf 0x02
		{
			g_VGM_WaitCount += VGM_WAIT_60HZ;
		}
		else if (*g_VGM_Pointer == VGM_CMD_WAIT_50HZ) // Wait 882 samples (50th of a second), a shortcut for 0x61 0x72 0x03
		{
			g_VGM_WaitCount += VGM_WAIT_50HZ;
		}
		else if (*g_VGM_Pointer == VGM_CMD_END) // End of sound data
		{
			if (g_VGM_State & VGM_STATE_LOOP)
			{
				g_VGM_Pointer = g_VGM_Loop;
				continue; // Don't skip the first command of the loop
			}
			else
			{
				VGM_Stop();
				return;
			}
		}
		else if (*g_VGM_Pointer == VGM_CMD_BLOCK) // Data block: 67 66 tt ss ss ss ss data[ss]
		{
			u16 size = *(u16*)(g_VGM_Pointer + 3); // Get stream size (only the 16-bit LSB is relevant on MSX)
			switch (g_VGM_Pointer[2]) // Switch on stream type
			{
			#if (VGM_USE_OPNA && OPNA_USE_ADPCM)
			case VGM_BLOCK_YM2608: // YM2608 DELTA-T (ADPCM-B) memory image: rr rr rr rr (memory size) aa aa aa aa (start address) data[size-8]
				if (size > 8) // Some VGM contain empty data block
				{
					u16 addr = (*(u16*)(g_VGM_Pointer + 11) >> 2) | ((u16)g_VGM_Pointer[13] << 14); // Byte address to 4 bytes unit
					OPNA_ADPCMLoad(g_VGM_Pointer + 15, addr, (size - 8 + 3) >> 2);
				}
				break;
			#endif
			#if (VGM_USE_OPL4 && OPL4_USE_WAVE)
			case VGM_BLOCK_YMF278B: // YMF278B RAM image: rr rr rr rr (memory size) aa aa aa aa (start address, relative to RAM start) data[size-8]
				if (size > 8) // Some VGM contain empty data block
					OPL4_WaveLoad(g_VGM_Pointer + 15, g_VGM_Pointer[13] + 0x20, *(u16*)(g_VGM_Pointer + 11), size - 8); // MoonSound RAM starts at 200000h
				break;
			#endif
			}
			g_VGM_Pointer += size + 6;
		}
		else if ((*g_VGM_Pointer & 0xF0) == VGM_CMD_WAIT_4B) // wait n+1 samples, n can range from 0 to 15.
		{
			g_VGM_WaitCount += 1 + (*g_VGM_Pointer & 0x0F);
		}
		else if ((*g_VGM_Pointer & 0xF0) == VGM_CMD_DAC_WAIT) // YM2612 DAC write then wait n samples (DAC is ignored)
		{
			g_VGM_WaitCount += *g_VGM_Pointer & 0x0F;
		}
		else
		{
			// Unsupported command: skip its operands
			u8 op = *g_VGM_Pointer;
			if ((op == 0x4F) || (op == 0x50) || ((op & 0xF0) == 0x30) || (op == 0x94)) // 1 operand
				g_VGM_Pointer += 1;
			else if (((op & 0xE0) == 0x40) || ((op & 0xE0) == 0xA0)) // 0x40~0x5F, 0xA0~0xBF: 2 operands
				g_VGM_Pointer += 2;
			else if ((op & 0xE0) == 0xC0) // 0xC0~0xDF: 3 operands
				g_VGM_Pointer += 3;
			else if ((op >= 0xE0) || (op == 0x90) || (op == 0x91) || (op == 0x95)) // 4 operands
				g_VGM_Pointer += 4;
			else if (op == 0x92) // DAC stream: set frequency
				g_VGM_Pointer += 5;
			else if (op == 0x93) // DAC stream: start
				g_VGM_Pointer += 10;
			else if (op == 0x68) // PCM RAM write
				g_VGM_Pointer += 11;
		}
		g_VGM_Pointer++;
	}
	g_VGM_WaitCount -= g_VGM_WaitFrame;
}