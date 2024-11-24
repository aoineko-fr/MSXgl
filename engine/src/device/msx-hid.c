// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄  ▄▄▄ ▄▄ ▄     ▄▄ ▄ ▄▄▄▄ ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▀█▄  ▀█▄▀ ▄▄▄ ██▄█  ██  ██ █
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▄▄█▀ ██ █     ██ █ ▄██▄ ██▄▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// User input handler using direct access to ports
//─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "system_port.h"
#include "psg_reg.h"

//=============================================================================
// Variables
//=============================================================================

//=============================================================================
// Functions
//=============================================================================

//-----------------------------------------------------------------------------
// Detect device plugged to joystick port
u16 HID_Detect(enum INPUT_PORT port)
{
	port; // A
__asm
	ld		h, a					// Pin 8 HIGH
	and		#0b11001111
	ld		l, a					// Pin 8 LOW

	ld		a, #PSG_REG_IO_PORT_B	// R#15
	out		(P_PSG_REGS), a			// Select port B (output)
	ld		a, l					// 
	out		(P_PSG_DATA), a			// Set Pin 8 LOW

	ld		a, #PSG_REG_IO_PORT_A	// R#14
	out		(P_PSG_REGS), a			// Select port A (input)
	in		a, (P_PSG_STAT)			//
	and		#0x3F
	ld		d, a					// Return value

	ld		a, #PSG_REG_IO_PORT_B	// R#15
	out		(P_PSG_REGS), a			// Select port B (output)
	ld		a, h					// 
	out		(P_PSG_DATA), a			// Set Pin 8 HIGH

	ld		a, #PSG_REG_IO_PORT_A	// R#14
	out		(P_PSG_REGS), a			// Select port A (input)
	in		a, (P_PSG_STAT)			//
	and		#0x3F
	ld		e, a					// Return value
__endasm;
}