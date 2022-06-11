// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄  ▄▄▄ ▄▄ ▄     ▄▄         ▄▄ ▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▀█▄  ▀█▄▀ ▄▄ ██▄█ ██ █  ▄██ ▄  ▄█▀▄
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▄▄█▀ ██ █    ██ █ ▀█▄█ ▀▄██ ██ ▀█▄▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// MSX-Audio (Y8950) handler module
//
// References:
//  - 
//─────────────────────────────────────────────────────────────────────────────
#include "msx-audio.h"
#include "system.h"
#include "system_port.h"

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// READ-ONLY DATA
//=============================================================================
const c8 g_MSXAudio_Ident[] = "AUDIO";

//=============================================================================
// MEMORY DATA
//=============================================================================

#if (MSXAUDIO_USE_RESUME)
u8 g_MSXAudio_RegBackup[16];
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize MSX-Audio module
void MSXAudio_Initialize()
{
	MSXAudio_Detect();
}

//-----------------------------------------------------------------------------
// Search for MSX-Audio (YM2413) chip
bool MSXAudio_Detect()
{
	return (g_MSXAudio_IndexPort | 0x06) == 0x06; // can be 0x00 for Moonsound
}

//-----------------------------------------------------------------------------
// Set MSX-Audio register value
void MSXAudio_SetRegister(u8 reg, u8 value)
{
	g_MSXAudio_IndexPort = reg;
	g_MSXAudio_DataPort = value;

	#if (MSXAUDIO_USE_RESUME)
	if((reg & 0xF0) == 0xB0) // MSXAUDIO_REG_CTRL_x
		g_MSXAudio_RegBackup[reg & 0x0F] = value;
	#endif
}

//-----------------------------------------------------------------------------
// Get MSX-Audio register value (most of the registers are write-only)
u8 MSXAudio_GetRegister(u8 reg)
{
	g_MSXAudio_IndexPort = reg;
	return g_MSXAudio_DataPort;
}

//-----------------------------------------------------------------------------
// Mute MSX-Audio sound
void MSXAudio_Mute()
{
	loop(i, 9)
	{
		MSXAudio_SetRegister(MSXAUDIO_REG_CTRL_1 + i, 0); // seem to be enough
	}
}

#if (MSXAUDIO_USE_RESUME)
//-----------------------------------------------------------------------------
// Resume MSX-Audio sound
void MSXAudio_Resume()
{
	loop(i, 9)
	{
		MSXAudio_SetRegister(MSXAUDIO_REG_CTRL_1 + i, g_MSXAudio_RegBackup[i]); // seem to be enough
	}
}
#endif
