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
#include "system.h"
#include "system_port.h"

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

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize MSX-Music module
void MSXMusic_Initialize()
{
	MSXMusic_Detect();
}

//-----------------------------------------------------------------------------
// 
bool MSXMusic_CheckInternal(u8 slotId)
{
	const c8* ptr = g_MSXMusic_Ident;
	u16 dest = 0x4018;
	while(*ptr != 0)
	{
		if(*ptr != Bios_InterSlotRead(slotId, dest++))
			return false;
		ptr++;
	}
	return true;
}

//-----------------------------------------------------------------------------
// 
bool MSXMusic_CheckExternal(u8 slotId)
{
	const c8* ptr = g_MSXMusic_Ident + 4;
	u16 dest = 0x401C;
	while(*ptr != 0)
	{
		if(*ptr != Bios_InterSlotRead(slotId, dest++))
			return false;
		ptr++;
	}
	return true;
}

//-----------------------------------------------------------------------------
// Search for MSX-Music (YM2413) chip
u8 MSXMusic_Detect()
{
	// Check internal MSX-Music chip
	g_MSXMusic_SlotId = Sys_CheckSlot(MSXMusic_CheckInternal);
	if(g_MSXMusic_SlotId != SLOT_NOTFOUND)
		return MSXMUSIC_INTERNAL;

	// Check external MSX-Music chip
	g_MSXMusic_SlotId = Sys_CheckSlot(MSXMusic_CheckExternal);
	if(g_MSXMusic_SlotId != SLOT_NOTFOUND)
	{
		// Activate external FM-PAC
		u8 val = Bios_InterSlotRead(g_MSXMusic_SlotId, 0x7FF6);
		Bios_InterSlotWrite(g_MSXMusic_SlotId, 0x7FF6, val | 0x01);
		return MSXMUSIC_EXTERNAL;
	}
	
	return MSXMUSIC_NOTFOUND;
}

//-----------------------------------------------------------------------------
// Set MSX-Music register value
void MSXMusic_SetRegister(u8 reg, u8 value)
{
	g_MSXMusic_IndexPort = reg;
	g_MSXMusic_DataPort = value;
}

//-----------------------------------------------------------------------------
// Get MSX-Music register value (most of the registers are write-only)
u8 MSXMusic_GetRegister(u8 reg)
{
	g_MSXMusic_IndexPort = reg;
	return g_MSXMusic_DataPort;
}

//-----------------------------------------------------------------------------
// Mute MSX-Music sound
void MSXMusic_Mute()
{
	loop(i, 9)
	{
		MSXMusic_SetRegister(MSXMUSIC_REG_FERQ_0 + i, 0);
		MSXMusic_SetRegister(MSXMUSIC_REG_CTRL_0 + i, 0);
		MSXMusic_SetRegister(MSXMUSIC_REG_INST_0 + i, 0);
	}
}
