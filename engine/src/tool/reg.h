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
void Registers_Apply();

// Function: Registers_ApplyBase
// Set z80 register according to <Registers_Data> structure values
// You can call this function if you don't need to set IX and IY registers
void Registers_ApplyBase();

// Function: Registers_Store
// Set <Registers_Data> structure data from z80 register values
void Registers_Store();

// Function: Registers_StoreBase
// Set <Registers_Data> structure data from z80 register values
// You can call this function if you don't need to get IX and IY registers
void Registers_StoreBase();

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
