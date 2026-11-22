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

//=============================================================================
// INCLUDES
//=============================================================================

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

#if (MSXAUDIO_USE_CHANNEL)
u8 g_MSXAudio_ChanKey[9];   // Copy of R#B0-B8
u8 g_MSXAudio_ChanTrig[9];  // R#B0-B8 values ORed since the last read
u8 g_MSXAudio_ChanRhythm;   // Copy of R#BD
u8 g_MSXAudio_ChanRhyTrig;  // R#BD values ORed since the last read
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize MSX-Audio module
bool MSXAudio_Initialize()
{
	bool ret = MSXAudio_Detect();
	#if (MSXAUDIO_USE_RESUME)
	Mem_Set(0, g_MSXAudio_RegBackup, 16);
	#endif
	#if (MSXAUDIO_USE_CHANNEL)
	g_MSXAudio_ChanRhythm = 0;
	g_MSXAudio_ChanRhyTrig = 0;
	Mem_Set(0, g_MSXAudio_ChanKey, 9);
	Mem_Set(0, g_MSXAudio_ChanTrig, 9);
	#endif
	return ret;
}

//-----------------------------------------------------------------------------
// Search for MSX-Audio (YM2413) chip
bool MSXAudio_Detect()
{
	return (io_MSXAudio_Reg | 0x06) == 0x06; // can be 0x00 for Moonsound
}

//-----------------------------------------------------------------------------
// Set MSX-Audio register value
void MSXAudio_SetRegister(u8 reg, u8 value)
{
	io_MSXAudio_Reg = reg;
	io_MSXAudio_Data = value;

	#if (MSXAUDIO_USE_RESUME)
	if (g_MSXAudio_DoBackup && ((reg & 0xF0)) == 0xB0) // MSXAUDIO_REG_CTRL_x
		g_MSXAudio_RegBackup[reg & 0x0F] = value;
	#endif

	#if (MSXAUDIO_USE_CHANNEL)
	u8 i = reg - MSXAUDIO_REG_CTRL_1;
	if (i < 9) // Key on
	{
		g_MSXAudio_ChanKey[i] = value;
		g_MSXAudio_ChanTrig[i] |= value;
	}
	else if (reg == MSXAUDIO_REG_RHYTHM)
	{
		g_MSXAudio_ChanRhythm = value;
		g_MSXAudio_ChanRhyTrig |= value;
	}
	#endif
}

//-----------------------------------------------------------------------------
// Get MSX-Audio register value (most of the registers are write-only)
u8 MSXAudio_GetRegister(u8 reg)
{
	io_MSXAudio_Reg = reg;
	return io_MSXAudio_Data;
}

//-----------------------------------------------------------------------------
// Mute MSX-Audio sound
void MSXAudio_Mute()
{
	MSXAUDIO_DOBACKUP(FALSE);
	loop (i, 9)
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
	loop (i, 9)
	{
		MSXAudio_SetRegister(MSXAUDIO_REG_CTRL_1 + i, g_MSXAudio_RegBackup[i]); // seem to be enough
	}
}
#endif

#if (MSXAUDIO_USE_CHANNEL)
//-----------------------------------------------------------------------------
// Get the current volume of a given channel
u8 MSXAudio_GetChannelVolume(u8 chan)
{
	u8 s;
	if (chan < 9) // FM channel
	{
		if ((chan >= 6) && (g_MSXAudio_ChanRhythm & 0x20)) // Used by rhythm
			return 0;
		s = g_MSXAudio_ChanKey[chan] | g_MSXAudio_ChanTrig[chan];
		g_MSXAudio_ChanTrig[chan] = 0;
		return (s & 0x20) ? 15 : 0; // R#Bx bit 5
	}
	chan -= 9;
	if (chan < 5) // Rhythm instrument
	{
		if (!(g_MSXAudio_ChanRhythm & 0x20)) // Rhythm mode
			return 0;
		u8 bit = 0x10 >> chan; // BD, SD, TOM, TCY, HH
		s = (g_MSXAudio_ChanRhythm | g_MSXAudio_ChanRhyTrig) & bit;
		g_MSXAudio_ChanRhyTrig &= ~bit;
		return s ? 15 : 0;
	}
	return (io_MSXAudio_Reg & MSXAUDIO_STATUS_PCM) ? 15 : 0; // ADPCM
}
#endif
