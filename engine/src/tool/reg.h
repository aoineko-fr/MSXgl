// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄           ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ▄█▀▄ ▄█▀▄ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │   ██  ▀█▄▀ ▀█▄▀ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"
#include "dos.h"
#include "bios.h"

//=============================================================================
// DEFINES
//=============================================================================

// z80 registers starage structure
struct Registers
{
	u16 IX;
	u16 IY;
	u16 HL;
	u16 DE;
	u16 BC;
	u8  A;
};

// Data to store z80 registers
extern struct Registers g_Registers;

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: Registers_Apply
// Set z80 register according to <Registers_Data> structure values
// You can call Registers_ApplyBase() if you don't need to set IX and IY registers
inline void Registers_Apply()
{
	__asm
		ld		ix, (_g_Registers+ 0);
		ld		iy, (_g_Registers+ 2);
	_Registers_ApplyBase::
		ld		hl, (_g_Registers+ 4);
		ld		de, (_g_Registers+ 6);
		ld		bc, (_g_Registers+ 8);
		ld		a,  (_g_Registers+10);
	__endasm;
}

// Function: Registers_Store
// Set <Registers_Data> structure data from z80 register values
// You can call Registers_StoreBase() if you don't need to get IX and IY registers
inline void Registers_Store()
{
	__asm
		ld		(_g_Registers+ 0), ix;
		ld		(_g_Registers+ 2), iy;
	_Registers_StoreBase::
		ld		(_g_Registers+ 4), hl;
		ld		(_g_Registers+ 6), de;
		ld		(_g_Registers+ 8), bc;
		ld		(_g_Registers+10), a;
	__endasm;
}

// Function: Registers_Data
// Get z80 register data structure
inline struct Registers* Registers_Data() { return &g_Registers; }

// Function: Registers_CallBDOS
// Call BDOS function after setting registers from <Registers_Data> data structure. Return values are then store in <Registers_Data>.
void Registers_CallBDOS();

// Macro: REGISTERS_CALL
// Set registers from <Registers_Data> data structure, call the given address, then get register back to the <Registers_Data>
#define REGISTERS_CALL(addr)	\
	__asm__("push ix");			\
	Registers_Apply();			\
	Call(addr);					\
	Registers_Store();			\
	__asm__("pop ix");
