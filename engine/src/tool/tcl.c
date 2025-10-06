// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄           ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ▄█▀▄ ▄█▀▄ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │   ██  ▀█▄▀ ▀█▄▀ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// TCL module to communicate with the Programmable Device
//   Based on Pedro de Medeiros "pvmm" works
//   https://github.com/pvmm/openmsx-tcl-bridge/tree/main
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"
#include "tcl.h"

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Send command to TCL engine
void TCL_Send(const TCL_Data* data)
{
	data; // -> HL

__asm
	// Write data address to I/O port 0x06
	ld		c, #6
	out		(c), l
	out		(c), h
__endasm;
}