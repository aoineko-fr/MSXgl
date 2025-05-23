// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄           ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ▄█▀▄ ▄█▀▄ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │   ██  ▀█▄▀ ▀█▄▀ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#include "reg.h"

//=============================================================================
// MEMORY DATA
//=============================================================================

// Data to store z80 registers
struct Registers g_Registers;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Set z80 register according to <Registers_Data> structure values
void Registers_Apply()
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

//-----------------------------------------------------------------------------
// Set <Registers_Data> structure data from z80 register values
void Registers_Store()
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

//-----------------------------------------------------------------------------
// Call BDOS function after setting registers from g_Registers data structure
void Registers_CallBDOS()
{
	__asm__("push ix");
	Registers_Apply();
	Call(BDOS);
	Registers_Store();
	__asm__("pop ix");
}

