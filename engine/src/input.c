// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ██▀▄ ██▀▄ ██ █ ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄██▄ ██ █ ██▀  ▀█▄█ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘            ▀▀  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// User input handler using direct access to ports
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"
#include "input.h"
#include "system_port.h"
#include "psg_reg.h"

//=============================================================================
// FUNCTIONS
//=============================================================================


//=============================================================================
// General purpose port device detection
//=============================================================================
#if (INPUT_USE_DETECT)

//-----------------------------------------------------------------------------
// Detect device plugged in General purpose ports
u8 Input_Detect(enum INPUT_PORT port) __NAKED __PRESERVES(b, c, d, e, iyl, iyh)
{
	port; // A

__asm
	ld		h, a					// Pin 8 HIGH
	and		#0b11001111
	ld		l, a					// Pin 8 LOW

	ld		a, #PSG_REG_IO_PORT_B	// R#15
	INPUT_DI //-------- Disable interrupts --------
	out		(P_PSG_REGS), a			// Select port B (output)
	ld		a, l					// 
	out		(P_PSG_DATA), a			// Set Pin 8 LOW

	ld		a, #PSG_REG_IO_PORT_B	// R#15
	out		(P_PSG_REGS), a			// Select port B (output)
	ld		a, h					// 
	out		(P_PSG_DATA), a			// Set Pin 8 HIGH

	ld		a, #PSG_REG_IO_PORT_A	// R#14
	out		(P_PSG_REGS), a			// Select port A (input)
	INPUT_EI //-------- Enable interrupts --------
	in		a, (P_PSG_STAT)			//
	and		#0x3F

	ret
__endasm;
}

#endif

// Direct access to joystick
#if (INPUT_USE_JOYSTICK)
#include "joystick.c"
#endif

// Direct access to mouse
#if (INPUT_USE_MOUSE)
#include "mouse.c"
#endif

// Direct access to keyboard
#if (INPUT_USE_KEYBOARD)
#include "keyboard.c"
#endif