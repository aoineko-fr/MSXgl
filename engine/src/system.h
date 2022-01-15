// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄           ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  █ ██  ██▀ ██▀ ▄███ ▄█▄█
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀  ▀██ ▄██  ▀█▄ ▀█▄▄ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘        ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"
#include "bios.h"
#include "system_port.h"
#include "asm.h"

//=============================================================================
// DEFINES
//=============================================================================

//-----------------------------------------------------------------------------
// Slot ID
// ExxxSSPP
// │   ││└┴─ Primary slot number (00-11)
// │   └┴─── Secondary slot number (00-11)
// └──────── Expanded slot (0 = no, 1 = yes)
#define SLOT_0 (0x00)
#define SLOT_1 (0x01)
#define SLOT_2 (0x02)
#define SLOT_3 (0x03)

#define SLOT_EXP (1 << 7)

#define SLOT_0_0 (SLOT_0 | (0x00 << 2) | SLOT_EXP)
#define SLOT_0_1 (SLOT_0 | (0x01 << 2) | SLOT_EXP)
#define SLOT_0_2 (SLOT_0 | (0x02 << 2) | SLOT_EXP)
#define SLOT_0_3 (SLOT_0 | (0x03 << 2) | SLOT_EXP)

#define SLOT_1_0 (SLOT_1 | (0x00 << 2) | SLOT_EXP)
#define SLOT_1_1 (SLOT_1 | (0x01 << 2) | SLOT_EXP)
#define SLOT_1_2 (SLOT_1 | (0x02 << 2) | SLOT_EXP)
#define SLOT_1_3 (SLOT_1 | (0x03 << 2) | SLOT_EXP)

#define SLOT_2_0 (SLOT_2 | (0x00 << 2) | SLOT_EXP)
#define SLOT_2_1 (SLOT_2 | (0x01 << 2) | SLOT_EXP)
#define SLOT_2_2 (SLOT_2 | (0x02 << 2) | SLOT_EXP)
#define SLOT_2_3 (SLOT_2 | (0x03 << 2) | SLOT_EXP)

#define SLOT_3_0 (SLOT_3 | (0x00 << 2) | SLOT_EXP)
#define SLOT_3_1 (SLOT_3 | (0x01 << 2) | SLOT_EXP)
#define SLOT_3_2 (SLOT_3 | (0x02 << 2) | SLOT_EXP)
#define SLOT_3_3 (SLOT_3 | (0x03 << 2) | SLOT_EXP)

#define SLOT(p)			(0x03 & p)
#define SLOTEX(p, s)	((0x03 & p) | ((0x03 & s) << 2) | SLOT_EXP)

#define IS_SLOT_EXP(s)	(s & SLOT_EXP)
#define SLOT_PRIM(s)	(s & 0x03)
#define SLOT_SEC(s)		((s >> 2 ) & 0x03)

//-----------------------------------------------------------------------------
extern u16 g_FirstAddr;
extern u16 g_HeaderAddr;
extern u16 g_LastAddr;


//=============================================================================
// FONCTION
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Helper
// Helper inline functions

// Function: Call
// Direct call a routine at a given address (generate ASM code: "call XXXX")
//
// Parameters:
//   addr - Address to call
inline void Call(u16 addr) { ((void(*)(void))(addr))(); }

// Function: EnableInterrupt
// Enable interruption
inline void EnableInterrupt() { __asm__("ei"); }

// Function: DisableInterrupt
// Disable interruption
inline void DisableInterrupt() { __asm__("di"); }

// Function: Halt
// Disable interruption
inline void Halt() { __asm__("halt"); }

//-----------------------------------------------------------------------------
// Group: Slot
// Slot handling functions

// Function: Sys_GetPageSlot
// Get the slot ID of a given page
//
// Parameters:
//   page - The page to ckeck
u8 Sys_GetPageSlot(u8 page);

// Function: Sys_SetPageSlot
// Select a slot in a given page
//
// Parameters:
//   page - The page to change
//   slotId - The slot ID to select
void Sys_SetPageSlot(u8 page, u8 slotId);

// Function: Sys_IsSlotExpanded
// Check if slot is expanded
//
// Parameters:
//   slot - The slot to check
inline bool Sys_IsSlotExpanded(u8 slot) { return g_EXPTBL[slot] & 0x80; }

// Function: Sys_SetPage0Slot
// Select a given slot in page 0
//
// Parameters:
//   slotId - The slot to select
void Sys_SetPage0Slot(u8 slotId);

// Function: Sys_SlotIsExpended
// Check expended flag from slot ID
//
// Parameters:
//   slotId - The slot ID
inline bool Sys_SlotIsExpended(u8 slotId) { return slotId & SLOT_EXP; }

// Function: Sys_SlotGetPrimary
// Get primary slot from slot ID
//
// Parameters:
//   slotId - The slot ID
inline u8 Sys_SlotGetPrimary(u8 slotId) { return slotId & 0x03; }

// Function: Sys_SlotGetSecondary
// Get secondary slot from slot ID
//
// Parameters:
//   slotId - The slot ID
inline u8 Sys_SlotGetSecondary(u8 slotId) { return (slotId >> 2) & 0x03; }

//-----------------------------------------------------------------------------
// Group: Address
// Binary address functions

// Function: Sys_GetFirstAddr
// Get first address of program binary
inline u16 Sys_GetFirstAddr() { return (u16)&g_FirstAddr; }

// Function: Sys_GetHeardAddr
// Get address of program header (if any)
inline u16 Sys_GetHeardAddr() { return (u16)&g_HeaderAddr; }

// Function: Sys_GetLastAddr
// Get last address of program binary
inline u16 Sys_GetLastAddr()  { return (u16)&g_LastAddr; }

