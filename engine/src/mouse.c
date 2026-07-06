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
#include "mouse.h"
#include "system_port.h"
#include "psg_reg.h"

//=============================================================================
// Direct access to mouse
//=============================================================================

i8 g_Mouse_OffsetX;
i8 g_Mouse_OffsetY;
u8 g_Mouse_Buttons;

//-----------------------------------------------------------------------------
// 
void Mouse_Read(u8 port, Mouse_State* data) __NAKED __PRESERVES(iyl, iyh)
{
	port; // A
	data; // DE

__asm
	WAIT1 = 10	// Short delay value
	WAIT2 = 30	// Long delay value

// Routine to read the mouse by direct accesses (works on MSX1/2/2+/turbo R)
// Original function by FRS and/or MRC users (https://www.msx.org/wiki/Mouse/Trackball)
//
// Input:  HL = 09310h for mouse in port 1 (H = 10010011b, L = 00010000b)
//         HL = 0EC20h for mouse in port 2 (H = 11101100b, L = 00100000b)
// Output: D = X-offset, E = Y-offset (D = E = 255 if no mouse)
GTMOUS:
	ld		l, a					// Backup input port value 

	ld		a, (de)					// Get previous mouse button value
	push	af						// Backup A for later use

	// Get current Code/Kana LED state
	ld		a, #PSG_REG_IO_PORT_B	// Select PSG R#15
	INPUT_DI
	out		(P_PSG_REGS), a
	INPUT_EI
	in		a, (P_PSG_STAT)			// Read from PSG R#15
	and		#0x80					// Keep current Code/Kana LED state
	or		l
	ld		h, a
	ld		l, a

	// Read first X-offset nibble (bits 4-7)
	ld		b, #WAIT2				// Long delay for first read
	call	GTOFS2					// Get R#14 content
	ld		(de), a					// Store mouse button value
	and		#0x0F
	rlca
	rlca
	rlca
	rlca
	ld		c, a					// Backup bits 4-7 of the X-offset
	// Read second X-offset nibble (bits 0-3)
	ld		a, h
	and		#0b11001111
	ld		l, a
	call	GTOFST					// Read bits 0-3 of the X-offset
	and		#0x0F
	or		c
	inc		de
	ld		(de), a					// Store combined X-offset

	// Read first Y-offset nibble (bits 7-4)
	ld		l, h
	call	GTOFST					// Read bits 7-4 of the y-offset
	and		#0x0F
	rlca
	rlca
	rlca
	rlca
	ld		c, a
	// Read second Y-offset nibble (bits 0-3)
	ld		a, h
	and		#0b11001111
	ld		l, a
	call	GTOFST					// Read bits 3-0 of the y-offset
	and		#0x0F
	or		c
	inc		de
	ld		(de), a					// Store combined Y-offset

	pop		af						// Restore A
	inc		de
	ld		(de), a					// Store previous mouse button value

	ret

GTOFST:
	ld		b, #WAIT1
GTOFS2:
	ld		a, #PSG_REG_IO_PORT_B	// Select PSG R#15 for mouse
	INPUT_DI
	out		(P_PSG_REGS), a
	ld		a, l					// Port 1=10010011b, 2=11101100b
	out		(P_PSG_DATA), a			// Write to PSG R#15

	call	WAITMS					// Extra delay because the mouse is slow

	ld		a, #PSG_REG_IO_PORT_A	// Select PSG R#14 for mouse
	out		(P_PSG_REGS), a
	INPUT_EI
	in		a, (P_PSG_STAT)			// Read to PSG R#14
	ret

WAITMS:
	ld		a, b
WTTR:
	djnz	WTTR
	.db		0xED, 0x55				// Back if Z80 (RETN on Z80, NOP on R800)
	rlca
	rlca
	ld		b, a
WTTR2:
	djnz	WTTR2
	ld		b, a
WTTR3:
	djnz	WTTR3

	ret
__endasm;
}