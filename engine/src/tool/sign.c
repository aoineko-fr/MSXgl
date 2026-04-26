// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄           ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ▄█▀▄ ▄█▀▄ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │   ██  ▀█▄▀ ▀█▄▀ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Application (Konami style) signature detection
//   Search on all slot for a specific application signatures
//   https://aoineko.org/msxgl/index.php?title=AppSignature
//─────────────────────────────────────────────────────────────────────────────
#include "sign.h"
#include "system.h"
#include "bios.h"

//=============================================================================
// 
//=============================================================================

const u16 g_AppSign_Address[4] = { 0x4010, 0x4018, 0x8010, 0x8018 };

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// 
bool AppSign_CheckSlotAddr(u8 slot, u16 addr, const AppSign sign)
{
	const u8* ptr = (const u8*)sign;
	loop (i, 4)
	{
		if (BIOS_InterSlotRead(slot, addr++) != *ptr++)
			return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// 
bool AppSign_CheckSlot(u8 slot, const AppSign sign)
{
	const u16* addr = g_AppSign_Address;
	loop (i, 4)
	{
		if (AppSign_CheckSlotAddr(slot, *addr, sign))
			return TRUE;
		addr++;
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// Check if the application signature is present in any slot
bool AppSign_Check(const AppSign sign)
{
	loop (slot, 4)
	{
		if (g_EXPTBL[slot] & SLOT_EXP)
		{
			loop (sub, 4)
			{
				u8 slotId = SLOTEX(slot, sub);
				if (AppSign_CheckSlot(slotId, sign))
					return TRUE;
			}
		}
		else if (AppSign_CheckSlot(slot, sign))
			return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// Check all the application signatures in the list and return the first found application's index
u8 AppSign_CheckList(u8 appNum, const AppSign* appList)
{
	loop (i, appNum)
	{
		if (AppSign_Check(*appList))
			return i;
		appList++;
	}
	return APPSIGN_NONE;
}