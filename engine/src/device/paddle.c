// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ██▀▄ ██▀▄ ██ █ ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄██▄ ██ █ ██▀  ▀█▄█ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘            ▀▀  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// User input handler using direct access to ports
//─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "system_port.h"
#include "paddle.h"
#include "bios_var.h"
#include "psg_reg.h"

//=============================================================================
// Variables
//=============================================================================

// Paddle state for both joystick port
u16 g_PaddleStates[2];

//=============================================================================
// Functions
//=============================================================================

//-----------------------------------------------------------------------------
// Read paddle state
void Paddle_Update() __NAKED
{
__asm
// Based on:
//    Program for reading the VAUS arkanoid paddle
//    danjovic@hotmail.com
//    http://hotbit.blogspot.com
//    License: GNU GPL 2.0

	// Read port A
	ld		iy, #_g_PaddleStates
	ld		de, #0x3E3F
	call	paddle_reads

	// Read port B
	ld		iy, #_g_PaddleStates + 2
	ld		de, #0x7B7F
	call	paddle_reads

	// Start counter for next frame (both port)
	ld		a, #15				// Select R#15
	out		(P_PSG_REGS), a
	ld		a, #0x0F
	out		(P_PSG_DATA), a		// Pin 8 LOW
	ld		a, #0x3F
	out		(P_PSG_DATA), a		// Pin 8 HIGH

	ret

paddle_reads:
	// Read counter 1st bit and button state
	ld		a, #15				// Select R#15
	out		(P_PSG_REGS), a
	ld		a, e
	out		(P_PSG_DATA), a		// Select given port for reading

	ld		a, #14				// Select R#14
	out		(P_PSG_REGS), a
	in		a, (P_PSG_STAT)		// Reads (port A)
	and		#0x03
	ld		h, a				// Store first bit and button state in H
	ld		l, #0				// Reset L
	ld		b, #8				// Setup 8 bits loop

paddle_nextbit:
	// Read remaining counter's 8 bits
	ld		a, #15				// Select R#15
	out		(P_PSG_REGS), a
	ld		a, d
	out		(P_PSG_DATA), a		// Clock LOW
	ld		a, e
	out		(P_PSG_DATA), a		// Clock HIGH

	ld		a, #14				// Select R#14
	out		(P_PSG_REGS), a
	in		a, (P_PSG_STAT)		// Reads (port A)
	srl		a					// Shift bit...
	rl		l					// ...and store it in L
	djnz	paddle_nextbit		// Reads next bit

	// Check for disconnection
	ld		a, h  
	rra							// H.0 to F.c 
	ld		a, l
	adc		a, #0				// If L + F.c == 0, then all counter bits are 1 (paddle is not conected) 
	jr		nz, paddle_end
	set		7, h				// Set disconnection flag
paddle_end:
	ld		0(iy), l
	ld		1(iy), h

	ret
__endasm;
}

#if (PADDLE_USE_CALIB)

// Paddle calibration offset
i16 g_PaddleOffset[2];

//-----------------------------------------------------------------------------
// Read paddle state
u8 Paddle_GetCalibratedAngle(u8 port)
{
	i16 val = (i16)Paddle_GetAngle(port) - g_PaddleOffset[port];
	if(val < 0)
		val = 0;
	else if(val > 255)
		val = 255; 
	return (u8)val;
}

#endif // (PADDLE_USE_CALIB)