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
#include "memory.h"

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
bool g_MSXAudio_DoBackup = TRUE;
u8 g_MSXAudio_RegBackup[16];
#define MSXAUDIO_DOBACKUP(do)	g_MSXAudio_DoBackup = do
#else
#define MSXAUDIO_DOBACKUP(do)
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize MSX-Audio module
void MSXAudio_Initialize()
{
	MSXAudio_Detect();
	#if (MSXAUDIO_USE_RESUME)
	Mem_Set(0, g_MSXAudio_RegBackup, 16);
	#endif
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
	if(g_MSXAudio_DoBackup && ((reg & 0xF0)) == 0xB0) // MSXAUDIO_REG_CTRL_x
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
	MSXAUDIO_DOBACKUP(FALSE);
	loop(i, 9)
	{
		MSXAudio_SetRegister(MSXAUDIO_REG_CTRL_1 + i, 0); // seem to be enough
	}
	MSXAUDIO_DOBACKUP(TRUE);
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
