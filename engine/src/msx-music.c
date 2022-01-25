// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄  ▄▄▄ ▄▄ ▄     ▄ ▄           ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▀█▄  ▀█▄▀ ▄▄ ██▀█ ██ █  ██▀ ▄  ▄█▀▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▄▄█▀ ██ █    ██ █ ▀█▄█ ▄██  ██ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// MSX-Music (YM2413) handler module
//
// References:
//  - YM2413 FM Operator Type-LL (OPLL) Application Manual
//     https://www.smspower.org/maxim/Documents/YM2413ApplicationManual
//  - MSX-MUSIC programming
//     https://www.msx.org/wiki/MSX-MUSIC_programming
//─────────────────────────────────────────────────────────────────────────────
#include "msx-music.h"
#include "system_port.h"

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// READ-ONLY DATA
//=============================================================================

//=============================================================================
// MEMORY DATA
//=============================================================================
u8 g_MSXMusic_SlotId = 0xFF;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize MSX-Music module
void MSXMusic_Initialize()
{
	u8 detect = MSXMusic_Detect();
	if(detect == MSXMUS_EXTERNAL)
	{
		// activate
	}
}

//-----------------------------------------------------------------------------
// Search for MSX-Music (YM2413) chip
u8 MSXMusic_Detect()
{
	return MSXMUS_NOTFOUND;
}

//-----------------------------------------------------------------------------
// Set MSX-Music register value
void MSXMusic_SetRegister(u8 reg, u8 value)
{
	g_MSXMusic_Index = reg;
	g_MSXMusic_Data = value;
}

//-----------------------------------------------------------------------------
// Get MSX-Music register value
u8 MSXMusic_GetRegister(u8 reg)
{
	return 0;
}

//-----------------------------------------------------------------------------
// Mute MSX-Music sound
void MSXMusic_Mute()
{
	loop(i, 9)
	{
		MSXMusic_SetRegister(MSXMUS_REG_FERQ_0 + i, 0);
		MSXMusic_SetRegister(MSXMUS_REG_CTRL_0 + i, 0);
		MSXMusic_SetRegister(MSXMUS_REG_INST_0 + i, 0);
	}
}
