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

//=============================================================================
// DEFINES
//=============================================================================

#define VGM_IDENT *(u32*)"Vgm "

//=============================================================================
// MEMORY DATA
//=============================================================================

const struct VGM_Header* g_VGM_Header;
const u8* g_VGM_Pointer;
const u8* g_VGM_Loop;
u16       g_VGM_WaitCount;
u16       g_VGM_WaitFrame;
u8        g_VGM_State;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
/// Play a VGM data
bool VGM_Play(const void* addr, bool loop)
{
	VGM_Pause(); // Mute all sound chip
	
	g_VGM_Header = (struct VGM_Header*)addr;

	if(g_VGM_Header->Ident != VGM_IDENT)
		return FALSE;
	
	g_VGM_State = VGM_STATE_PLAY;
	if(loop)
		g_VGM_State |= VGM_STATE_LOOP;

	g_VGM_Pointer = (u8*)((u16)&g_VGM_Header->Data_offset + (u16)g_VGM_Header->Data_offset);
	if(g_VGM_Header->Loop_offset)
		g_VGM_Loop = (u8*)((u16)&g_VGM_Header->Loop_offset + (u16)g_VGM_Header->Loop_offset);
	else
		g_VGM_Loop = 0;
	g_VGM_WaitCount = 0;

	if(g_ROMVersion.VSF)
		VGM_SetFrequency50Hz();
	else
		VGM_SetFrequency60Hz();

	return TRUE;
}

//-----------------------------------------------------------------------------
// Resume music playback
void VGM_Resume()
{
	g_VGM_State |= VGM_STATE_PLAY;

	#if (PSG_USE_RESUME)
	if(VGM_ContainsPSG())
		PSG_Resume();
	#endif

	#if (VGM_USE_MSXMUSIC && MSXMUSIC_USE_RESUME)
	if(VGM_ContainsMSXMusic())
		MSXMusic_Resume();
	#endif

	#if (VGM_USE_MSXAUDIO && MSXAUDIO_USE_RESUME)
	if(VGM_ContainsMSXAudio())
		MSXAudio_Resume();
	#endif

	#if (VGM_USE_SCC && SCC_USE_RESUME)
	if(VGM_ContainsSCC())
		SCC_Resume();
	#endif
}

//-----------------------------------------------------------------------------
// Pause music playback
void VGM_Pause()
{
	g_VGM_State &= ~VGM_STATE_PLAY;
	
	PSG_Mute();

	#if (VGM_USE_MSXMUSIC)
	MSXMusic_Mute();
	#endif
	
	#if (VGM_USE_MSXAUDIO)
	MSXAudio_Mute();
	#endif
	
	#if (VGM_USE_SCC)
	SCC_Mute();
	#endif
}

//-----------------------------------------------------------------------------
// Play a ayVGM data
void VGM_Stop()
{
	VGM_Pause();
	g_VGM_Pointer = (u8*)((u16)&g_VGM_Header->Data_offset + (u16)g_VGM_Header->Data_offset);
	g_VGM_WaitCount = 0;
}

//-----------------------------------------------------------------------------
/// Decode a frame of VGM data
void VGM_Decode()
{
	if(!(g_VGM_State & VGM_STATE_PLAY))
		return;
	
	while(g_VGM_WaitCount < g_VGM_WaitFrame)
	{
		if(*g_VGM_Pointer == 0xA0) // AY8910, write value dd to register aa
		{
			PSG_SetRegister(g_VGM_Pointer[1], g_VGM_Pointer[2]);
			g_VGM_Pointer += 2;
		}
		#if (VGM_USE_SCC)
		else if(*g_VGM_Pointer == 0xD2) // SCC1, port pp, write value dd to register aa
		{
			u8 reg = 0;
			switch(g_VGM_Pointer[1])
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
		}
		#endif
		#if (VGM_USE_MSXMUSIC)
		else if(*g_VGM_Pointer == 0x51) // YM2413, write value dd to register aa
		{
			MSXMusic_SetRegister(g_VGM_Pointer[1], g_VGM_Pointer[2]);
			g_VGM_Pointer += 2;
		}
		#endif
		#if (VGM_USE_MSXAUDIO)
		else if(*g_VGM_Pointer == 0x5C) // Y8950, write value dd to register aa
		{
			u8 reg = g_VGM_Pointer[1];
			if((reg != 0x04) && (reg != 0x18) && (reg != 0x19))
			{
				u8 value = g_VGM_Pointer[2];
				if(reg == 0x08)
					value &= 0b11000100;
				MSXAudio_SetRegister(reg, value);
			}
			g_VGM_Pointer += 2;
		}
		#endif
		else if(*g_VGM_Pointer == 0x61) // Wait n samples, n can range from 0 to 65535 (approx 1.49 seconds). Longer pauses than this are represented by multiple wait commands.
		{
			g_VGM_WaitCount += *(u16*)(g_VGM_Pointer+1);
			g_VGM_Pointer += 2;
		}
		else if(*g_VGM_Pointer == 0x62) // Wait 735 samples (60th of a second), a shortcut for 0x61 0xdf 0x02
		{
			g_VGM_WaitCount += 735;
		}
		else if(*g_VGM_Pointer == 0x63) // Wait 882 samples (50th of a second), a shortcut for 0x61 0x72 0x03
		{
			g_VGM_WaitCount += 882;
		}
		else if(*g_VGM_Pointer == 0x66) // End of sound data
		{
			if((g_VGM_Loop != 0) && (g_VGM_State & VGM_STATE_LOOP))
			{
				g_VGM_Pointer = g_VGM_Loop;
			}
			else
			{
				VGM_Stop();
				return;
			}
		}
		else if ((*g_VGM_Pointer & 0xF0) == 0x70) // wait n+1 samples, n can range from 0 to 15.
		{
			g_VGM_WaitCount += 1 + *g_VGM_Pointer & 0x0F;
		}
		else
		{
		}
		g_VGM_Pointer++;
	}
	g_VGM_WaitCount -= g_VGM_WaitFrame;
}