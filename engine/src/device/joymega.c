// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄            ▄ ▄               
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ▄█▀▄ █ ██ ██▀█ ▄███ ▄▀██ ▄▀██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄█▀  ▀█▄▀  ▀██ ██ █ ▀█▄▄  ▀██ ▀▄██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘             ▀▀             ▀▀ 
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// JoyMega driver module
//
// References:
// - JoyMega (MRC): https://www.msx.org/wiki/JoyMega
// - JoyMega Adapter (FRS): https://frs.badcoffee.info/hardware/joymega-en.html
// - Six Button Control Pad (Sega Retro): https://segaretro.org/Six_Button_Control_Pad_(Mega_Drive)#Technical_information
//─────────────────────────────────────────────────────────────────────────────
#include "joymega.h"

//			MSX					MEGA
//-------------------------------------------------------
//	Up			(1) --------- (1)	Up
//	Down		(2) --------- (2)	Down
//	Left		(3) --------- (3)	Left
//	Right		(4) --------- (4)	Right
//	+5V			(5) --------- (5)	+5V
//	Trigger 1	(6) --------- (6)	TL (A/B)
//	Trigger 2	(7) --------- (9)	TR (Start/C)
//	OUT			(8) --------- (7)	TH (Select)
//	Ground		(9) --------- (8)	Ground

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// READ-ONLY DATA
//=============================================================================

//=============================================================================
// MEMORY DATA
//=============================================================================

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Read Megadrive 3-button joypad through JoyMega adapter
// INPUT_PORT1 = 0b00010011
// INPUT_PORT2 = 0b01101100
u8 JoyMega_Read3(u8 port) __NAKED
{
	port; // A

__asm
	// Setup
	ld		h, a
	and		#0b11001111
	ld		l, a

jm3_detect_start:
	INPUT_DI
	call	jm_set_high				// Get: xxCBRLDU
	call	jm_get
	ld		b, a

	call	jm_set_low				// Get: xxSAxxxx
	call	jm_get
	INPUT_EI
	and		#0b00110000
	add		a, a
	add		a, a					// Compute: SAxxxxxx

	or		a, b					// Merge: SACBRLDU
	ret								// Return A

jm_set_high:
	ld		a, #15
	out		(P_PSG_REGS), a			// Select R#15
	ld		a, h
	out		(P_PSG_DATA), a			// Set Pin 6 LOW
	ret

jm_set_low:
	ld		a, #15
	out		(P_PSG_REGS), a			// Select R#15
	ld		a, l
	out		(P_PSG_DATA), a			// Set Pin 6 LOW
	ret

jm_get:
	ld		a, #14
	out		(P_PSG_REGS), a			// Select R#14
	in		a, (P_PSG_STAT)			// Read R#14
	ret

__endasm;
}

//-----------------------------------------------------------------------------
// Read Megadrive 6-button joypad through JoyMega adapter
// INPUT_PORT1 = 0b00010011
// INPUT_PORT2 = 0b01101100
u16 JoyMega_Read6(u8 port) __NAKED
{
	port; // A

__asm
	// Setup
	ld		h, a
	and		#0b11001111
	ld		l, a

jm3_detect_start:
	INPUT_DI
	call	jm_set_high				// Get: xxCBRLDU
	call	jm_get
	ld		b, a

	call	jm_set_low				// Get: xxSAxxxx
	call	jm_get
	and		#0b00110000
	add		a, a
	add		a, a					// Compute: SAxxxxxx

	or		a, b					// Merge: SACBRLDU
	ld		e, a					// Store in E

	call	jm_get_high
	call	jm_get_low
	call	jm_get_high
	call	jm_get_low
	and		#0x0F					// Keep low 4-bit; should be 0 for a 6-button joypad
	jp		nz, jm3_detect_failed

	call	jm_get_high				// Get: xxCBMXYZ
	INPUT_EI
	and		#0x0F					// Compute: 0000MXYZ
	ld		d, a					// Store in D

	//call	jm_get_low				// Can be used to reset joypad internal counter

	ret								// return DE

jm3_detect_failed:
	ld		d, #0					// Reset D
	
	ret
__endasm;
}
