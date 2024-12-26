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

// Slot macros
#define SLOT(p)			(0x03 & (p))
#define SLOTEX(p, s)	((0x03 & (p)) | ((0x03 & (s)) << 2) | SLOT_EXP)

#define IS_SLOT_EXP(s)	((s) & SLOT_EXP)
#define SLOT_PRIM(s)	((s) & 0x03)
#define SLOT_SEC(s)		(((s) >> 2 ) & 0x03)

// Special slot ID values
#define SLOT_NOTFOUND 	0xFF
#define SLOT_INVALID 	0xFF

// Function callback to make iterations on slots
typedef bool (*CheckSlotCallback)(u8);

//-----------------------------------------------------------------------------
// CRT0 defines

extern u16 g_FirstAddr; // First address of the program
extern u16 g_HeaderAddr; // Address of the header
extern u16 g_LastAddr; // Last address of the program

#if ((TARGET_TYPE == TYPE_ROM) && GET_TARGET_ISR(TARGET))
	extern u8 g_VersionROM; // ROM version
	extern u8 g_VersionMSX; // MSX version
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Peek & poke
// Functions to read and write memory

// Function: Poke
// Set a 8-bit value at the given address
//
// Parameters:
//   addr - Address to set
//   val  - 8-bit value to set
inline void Poke(u16 addr, u8 val) { *(u8*)addr = val; }

// Function: Poke16
// Set a 16-bit value at the given address
//
// Parameters:
//   addr - Address to set
//   val  - 16-bit value to set
inline void Poke16(u16 addr, u16 val) { *(u16*)addr = val; }

// Function: Peek
// Get the 8-bit value at the given address
//
// Parameters:
//   addr - Address to get
//
// Return:
//   8-bit value stored at the given address
inline u8 Peek(u16 addr) { return *(u8*)addr; }

// Function: Peek16
// Get the 16-bit value at the given address
//
// Parameters:
//   addr - Address to get
//
// Return:
//   16-bit value stored at the given address
inline u16 Peek16(u16 addr) { return *(u16*)addr; }

//-----------------------------------------------------------------------------
// Group: Low level
// Functions to access low level functions

// Function: EnableInterrupt
// Enable interruption
inline void EnableInterrupt() { __asm__("ei"); }

// Function: DisableInterrupt
// Disable interruption
inline void DisableInterrupt() { __asm__("di"); }

// Function: Halt
// Pause the CPU until a new interruption occured
inline void Halt() { __asm__("halt"); }

//-----------------------------------------------------------------------------
// Group: Call
// Functions to call routines

// Function: Call
// Direct call a routine at a given address (generate ASM code: "call XXXX")
// No extra cost due to calling a C function.
//
// Parameters:
//   addr - Address to call
//
// Return:
//   Value stored in register A
inline void Call(u16 addr) { ((void(*)(void))addr)(); }

// Function: CallA
// Direct call a routine at a given address with a 8-bits parameter in register A (generate ASM code: "call XXXX")
// No extra cost due to calling a C function.
//
// Parameters:
//   addr - Address to call
//   a    - Function parameter to put in register A
inline void CallA(u16 addr, u8 a) { ((void(*)(u8))addr)(a); }

typedef void (*calll_t)(u8) __FASTCALL;

// Function: CallL
// Direct call a routine at a given address with a 8-bits parameter in register L (generate ASM code: "call XXXX")
// No extra cost due to calling a C function.
//
// Parameters:
//   addr - Address to call
//   l    - Function parameter to put in register L
inline void CallL(u16 addr, u8 l) { ((calll_t)addr)(l); }

// Function: CallHL
// Direct call a routine at a given address with a 16-bits parameter in register HL (generate ASM code: "call XXXX")
// No extra cost due to calling a C function.
//
// Parameters:
//   addr - Address to call
//   val  - Function parameter to put in register HL
inline void CallHL(u16 addr, u16 hl) { ((void(*)(u16))addr)(hl); }

// Function: CallDriver
// Direct call a routine at a given address with a 8-bits parameter in register A (generate ASM code: "call XXXX")
// No extra cost due to calling a C function.
//
// Parameters:
//   addr - Address of the driver
//   val  - 8-bit value to transfer to driver's main function
//
// Return:
//   Value returned by the driver
inline u8 CallDriver(u16 addr, u8 val) { return ((u8(*)(u8))addr)(val); }

//-----------------------------------------------------------------------------
// Group: Slot
// Slot handling functions

// Function: Sys_GetPageSlot
// Get the slot ID of a given page
//
// Parameters:
//   page - The page to ckeck
//
// Return:
//   The slot ID
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
//
// Return:
//   FALSE if the slot is not expanded
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
//
// Return:
//   FALSE if the slot is not expended
inline bool Sys_SlotIsExpended(u8 slotId) { return IS_SLOT_EXP(slotId); }

// Function: Sys_SlotGetPrimary
// Get primary slot from slot ID
//
// Parameters:
//   slotId - The slot ID
//
// Return:
//   The primary slot of the slot ID
inline u8 Sys_SlotGetPrimary(u8 slotId) { return SLOT_PRIM(slotId); }

// Function: Sys_SlotGetSecondary
// Get secondary slot from slot ID
//
// Parameters:
//   slotId - The slot ID
//
// Return:
//   The secondary slot of the slot ID
inline u8 Sys_SlotGetSecondary(u8 slotId) { return SLOT_SEC(slotId); }

// Function: Sys_CheckSlot
// Check all slots with a given callback function
//
// Parameters:
//   cb - Callback function to call for each slot
//
// Return:
//   The slot ID that match the callback function
u8 Sys_CheckSlot(CheckSlotCallback cb);

//-----------------------------------------------------------------------------
// Group: Address
// Binary address functions

// Function: Sys_GetFirstAddr
// Get first address of program binary
//
// Return:
//   The first address of the program binary
inline u16 Sys_GetFirstAddr() { return (u16)&g_FirstAddr; }

// Function: Sys_GetHeaderAddr
// Get address of program header (if any)
//
// Return:
//   The address of the program header
inline u16 Sys_GetHeaderAddr() { return (u16)&g_HeaderAddr; }

// Function: Sys_GetLastAddr
// Get last address of program binary
//
// Return:
//   The last address of the program binary
inline u16 Sys_GetLastAddr()  { return (u16)&g_LastAddr; }

//-----------------------------------------------------------------------------
// Group: Misc
// Miscellaneous helper functions

// Function: Sys_PlayClickSound
// Play the click sound
inline void Sys_PlayClickSound() { g_PortAccessKeyboard |= 0x80; }

// Function: Sys_StopClickSound
// Stop the click sound
inline void Sys_StopClickSound() { g_PortAccessKeyboard &= 0x7F; }

//=============================================================================
// INCLUDES
//=============================================================================

#include "rom_mapper.h"
