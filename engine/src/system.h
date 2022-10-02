// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄           ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  █ ██  ██▀ ██▀ ▄███ ▄█▄█
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀  ▀██ ▄██  ▀█▄ ▀█▄▄ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘        ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"
// #include "bios.h"
#include "system_port.h"
#include "asm.h"
#include "bios_var.h"

//=============================================================================
// DEFINES
//=============================================================================

//-----------------------------------------------------------------------------
// Slot ID
// ExxxSSPP
// │   ││└┴─ Primary slot number (00-11)
// │   └┴─── Secondary slot number (00-11)
// └──────── Expanded slot (0 = no, 1 = yes)

// Primary slots
#define SLOT_0			(0x00)
#define SLOT_1			(0x01)
#define SLOT_2			(0x02)
#define SLOT_3			(0x03)

#define SLOT_EXP		(1 << 7)
// Expanded slots		Primary   Secondary       Expand
#define SLOT_0_0		(SLOT_0 | (SLOT_0 << 2) | SLOT_EXP)
#define SLOT_0_1		(SLOT_0 | (SLOT_1 << 2) | SLOT_EXP)
#define SLOT_0_2		(SLOT_0 | (SLOT_2 << 2) | SLOT_EXP)
#define SLOT_0_3		(SLOT_0 | (SLOT_3 << 2) | SLOT_EXP)
#define SLOT_1_0		(SLOT_1 | (SLOT_0 << 2) | SLOT_EXP)
#define SLOT_1_1		(SLOT_1 | (SLOT_1 << 2) | SLOT_EXP)
#define SLOT_1_2		(SLOT_1 | (SLOT_2 << 2) | SLOT_EXP)
#define SLOT_1_3		(SLOT_1 | (SLOT_3 << 2) | SLOT_EXP)
#define SLOT_2_0		(SLOT_2 | (SLOT_0 << 2) | SLOT_EXP)
#define SLOT_2_1		(SLOT_2 | (SLOT_1 << 2) | SLOT_EXP)
#define SLOT_2_2		(SLOT_2 | (SLOT_2 << 2) | SLOT_EXP)
#define SLOT_2_3		(SLOT_2 | (SLOT_3 << 2) | SLOT_EXP)
#define SLOT_3_0		(SLOT_3 | (SLOT_0 << 2) | SLOT_EXP)
#define SLOT_3_1		(SLOT_3 | (SLOT_1 << 2) | SLOT_EXP)
#define SLOT_3_2		(SLOT_3 | (SLOT_2 << 2) | SLOT_EXP)
#define SLOT_3_3		(SLOT_3 | (SLOT_3 << 2) | SLOT_EXP)

#define SLOT(p)			(0x03 & (p))
#define SLOTEX(p, s)	((0x03 & (p)) | ((0x03 & (s)) << 2) | SLOT_EXP)

#define IS_SLOT_EXP(s)	((s) & SLOT_EXP)
#define SLOT_PRIM(s)	((s) & 0x03)
#define SLOT_SEC(s)		(((s) >> 2 ) & 0x03)

#define SLOT_NOTFOUND 	0xFF

typedef bool (*CheckSlotCallback)(u8);

//-----------------------------------------------------------------------------
extern u16 g_FirstAddr;
extern u16 g_HeaderAddr;
extern u16 g_LastAddr;

//-----------------------------------------------------------------------------
// Macro to change a given bank's segment
#if (ROM_MAPPER > ROM_PLAIN)
	extern u8 g_Bank0Segment;
	extern u8 g_Bank1Segment;
	extern u8 g_Bank2Segment;
	extern u8 g_Bank3Segment;
	#define SET_BANK_SEGMENT(b, s)	{ (*(u8*)(ADDR_BANK_##b) = (s)); (g_Bank##b##Segment = (s)); }
	#define GET_BANK_SEGMENT(b)	    (g_Bank##b##Segment)
#endif


//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Helper
// Helper inline functions

// Function: EnableInterrupt
// Enable interruption
inline void EnableInterrupt() { __asm__("ei"); }

// Function: DisableInterrupt
// Disable interruption
inline void DisableInterrupt() { __asm__("di"); }

// Function: Halt
// Pause the CPU until a new interruption occured
inline void Halt() { __asm__("halt"); }

// Function: Call
// Direct call a routine at a given address (generate ASM code: "call XXXX")
//
// Parameters:
//   addr - Address to call
inline u8 Call(u16 addr) { ((void(*)(void))addr)(); }

// Function: CallA
// Direct call a routine at a given address with a 8-bits parameter in register A (generate ASM code: "call XXXX")
//
// Parameters:
//   addr - Address to call
//   a    - Function parameter to put in register A
inline void CallA(u16 addr, u8 a) { ((void(*)(u8))addr)(a); }

typedef void (*calll_t)(u8) __FASTCALL;
// Function: CallL
// Direct call a routine at a given address with a 8-bits parameter in register L (generate ASM code: "call XXXX")
//
// Parameters:
//   addr - Address to call
//   l    - Function parameter to put in register L
inline void CallL(u16 addr, u8 l) { ((calll_t)addr)(l); }

// Function: CallHL
// Direct call a routine at a given address with a 16-bits parameter in register HL (generate ASM code: "call XXXX")
//
// Parameters:
//   addr - Address to call
//   val  - Function parameter to put in register HL
inline void CallHL(u16 addr, u16 hl) { ((void(*)(u16))addr)(hl); }

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
inline bool Sys_IsSlotExpanded(u8 slot) { return g_EXPTBL[slot] & SLOT_EXP; }

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
inline bool Sys_SlotIsExpended(u8 slotId) { return IS_SLOT_EXP(slotId); }

// Function: Sys_SlotGetPrimary
// Get primary slot from slot ID
//
// Parameters:
//   slotId - The slot ID
inline u8 Sys_SlotGetPrimary(u8 slotId) { return SLOT_PRIM(slotId); }

// Function: Sys_SlotGetSecondary
// Get secondary slot from slot ID
//
// Parameters:
//   slotId - The slot ID
inline u8 Sys_SlotGetSecondary(u8 slotId) { return SLOT_SEC(slotId); }

// Function: Sys_CheckSlot
// Check all slots with a given callback function
u8 Sys_CheckSlot(CheckSlotCallback cb);

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

