// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄  ▄▄▄ ▄▄ ▄     ▄ ▄           ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▀█▄  ▀█▄▀ ▄▄ ██▀█ ██ █  ██▀ ▄  ▄█▀▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▄▄█▀ ██ █    ██ █ ▀█▄█ ▄██  ██ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// MSX-Music (YM2413) handler module
//
// References:
//  - YM2413 FM Operator Type-LL (OPLL) Application Manual
//     https://www.smspower.org/maxim/Documents/YM2413ApplicationManual
//  - MSX-MUSIC programming
//     https://www.msx.org/wiki/MSX-MUSIC_programming
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================

#include "msx-music.h"
#include "bios.h"
#include "system.h"
#include "system_port.h"
#include "memory.h"

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// READ-ONLY DATA
//=============================================================================
const c8 g_MSXMusic_Ident[] = "APRLOPLL";

//=============================================================================
// MEMORY DATA
//=============================================================================
u8 g_MSXMusic_SlotId = SLOT_NOTFOUND;

#if (MSXMUSIC_USE_PAUSE)
bool g_MSXMusic_DoBackup = TRUE;
u8 g_MSXMusic_RegBackup[16];
#define MSXMUSIC_DOBACKUP(do)	g_MSXMusic_DoBackup = do
#else
#define MSXMUSIC_DOBACKUP(do)
#endif

#if (MSXMUSIC_USE_CHANNEL)
u8 g_MSXMusic_ChanKey[9];   // Copy of R#20-28
u8 g_MSXMusic_ChanTrig[9];  // R#20-28 values ORed since the last read
u8 g_MSXMusic_ChanRhythm;   // Copy of R#0E
u8 g_MSXMusic_ChanRhyTrig;  // R#0E values ORed since the last read
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize MSX-Music module
bool MSXMusic_Initialize()
{
	u8 ret = MSXMusic_Detect();
	#if (MSXMUSIC_USE_PAUSE)
	Mem_Set(0, g_MSXMusic_RegBackup, 16);
	#endif
	#if (MSXMUSIC_USE_CHANNEL)
	g_MSXMusic_ChanRhythm = 0;
	g_MSXMusic_ChanRhyTrig = 0;
	Mem_Set(0, g_MSXMusic_ChanKey, 9);
	Mem_Set(0, g_MSXMusic_ChanTrig, 9);
	#endif
	return ret != MSXMUSIC_NOTFOUND;
}

//-----------------------------------------------------------------------------
// 
bool MSXMusic_CheckInternal(u8 slotId)
{
	const c8* ptr = g_MSXMusic_Ident;
	u16 dest = 0x4018;
	while (*ptr != 0)
	{
		if (*ptr != BIOS_InterSlotRead(slotId, dest++))
			return FALSE;
		ptr++;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// 
bool MSXMusic_CheckExternal(u8 slotId)
{
	const c8* ptr = g_MSXMusic_Ident + 4;
	u16 dest = 0x401C;
	while (*ptr != 0)
	{
		if (*ptr != BIOS_InterSlotRead(slotId, dest++))
			return FALSE;
		ptr++;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// Search for MSX-Music (YM2413) chip
u8 MSXMusic_Detect()
{
	// Check internal MSX-Music chip
	g_MSXMusic_SlotId = Sys_CheckSlot(MSXMusic_CheckInternal);
	if (g_MSXMusic_SlotId != SLOT_NOTFOUND)
		return MSXMUSIC_INTERNAL;

	// Check external MSX-Music chip
	g_MSXMusic_SlotId = Sys_CheckSlot(MSXMusic_CheckExternal);
	if (g_MSXMusic_SlotId != SLOT_NOTFOUND)
	{
		// Activate external FM-PAC
		u8 val = BIOS_InterSlotRead(g_MSXMusic_SlotId, 0x7FF6);
		BIOS_InterSlotWrite(g_MSXMusic_SlotId, 0x7FF6, val | 0x01);
		return MSXMUSIC_EXTERNAL;
	}
	
	return MSXMUSIC_NOTFOUND;
}

//-----------------------------------------------------------------------------
// Set MSX-Music register value
void MSXMusic_SetRegister(u8 reg, u8 value)
{
	io_MSXMusic_Reg = reg;
	io_MSXMusic_Data = value;

	#if (MSXMUSIC_USE_PAUSE)
	if ((g_MSXMusic_DoBackup) && ((reg & 0xF0) == 0x20)) // MSXMUSIC_REG_CTRL_x
		g_MSXMusic_RegBackup[reg & 0x0F] = value;
	#endif

	#if (MSXMUSIC_USE_CHANNEL)
	u8 i = reg - MSXMUSIC_REG_CTRL_1;
	if (i < 9) // Key on
	{
		g_MSXMusic_ChanKey[i] = value;
		g_MSXMusic_ChanTrig[i] |= value;
	}
	else if (reg == MSXMUSIC_REG_RHYTHM)
	{
		g_MSXMusic_ChanRhythm = value;
		g_MSXMusic_ChanRhyTrig |= value;
	}
	#endif
}

//-----------------------------------------------------------------------------
// Get MSX-Music register value (most of the registers are write-only)
u8 MSXMusic_GetRegister(u8 reg)
{
	io_MSXMusic_Reg = reg;
	return io_MSXMusic_Data;
}

//-----------------------------------------------------------------------------
// Mute MSX-Music sound
void MSXMusic_Mute()
{
	MSXMUSIC_DOBACKUP(FALSE);
	loop (i, 9)
	{
		MSXMusic_SetRegister(MSXMUSIC_REG_CTRL_1 + i, 0); // seem to be enough
	}
	MSXMUSIC_DOBACKUP(TRUE);
}

#if (MSXMUSIC_USE_PAUSE)
//-----------------------------------------------------------------------------
// Resume MSX-Music sound
void MSXMusic_Resume()
{
	loop (i, 9)
	{
		MSXMusic_SetRegister(MSXMUSIC_REG_CTRL_1 + i, g_MSXMusic_RegBackup[i]); // seem to be enough
	}
}
#endif

#if (MSXMUSIC_USE_CHANNEL)
//-----------------------------------------------------------------------------
// Get the current volume of a given channel
u8 MSXMusic_GetChannelVolume(u8 chan)
{
	u8 s;
	if (chan < 9) // Melodic channel
	{
		if ((chan >= 6) && (g_MSXMusic_ChanRhythm & 0x20)) // Used by rhythm
			return 0;
		s = g_MSXMusic_ChanKey[chan] | g_MSXMusic_ChanTrig[chan];
		g_MSXMusic_ChanTrig[chan] = 0;
		return (s & 0x10) ? 15 : 0; // R#2x bit 4
	}
	// Rhythm instrument
	if (!(g_MSXMusic_ChanRhythm & 0x20)) // Rhythm mode
		return 0;
	u8 bit = 0x10 >> (chan - 9); // BD, SD, TOM, TCY, HH
	s = (g_MSXMusic_ChanRhythm | g_MSXMusic_ChanRhyTrig) & bit;
	g_MSXMusic_ChanRhyTrig &= ~bit;
	return s ? 15 : 0;
}
#endif
