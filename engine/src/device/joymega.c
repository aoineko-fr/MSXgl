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
#include "system_port.h"

//			MSX					MEGA
//-------------------------------------------------------
//	Up			(1) --------- (1)	Up
//	Down		(2) ~~~~~~~~~ (2)	Down
//	Left		(3) ~~~~~~~~~ (3)	Left
//	Right		(4) ~~~~~~~~~ (4)	Right
//	+5V			(5) ~~~~~~~~~ (5)	+5V
//	Trigger 1	(6) ~~~~~~~~~ (6)	TL (A/B)
//	Trigger 2	(7) ~~~~~~~~~ (9)	TR (Start/C)
//	OUT			(8) ~~~~~~~~~ (7)	TH (Select)
//	Ground		(9) ~~~~~~~~~ (8)	Ground

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
//
// 			TH out	TR in	TL in	D3 in	D2 in	D1 in	D0 in
// Cycle	Pin8	Pin7	Pin6	Pin4	Pin3	Pin2	Pin1
//----------------------------------------------------------------
// <1>		LO		C		B		Right	Left	Down	Up
// <2>		HI		Start	A		0		0		Down	Up
//
// INPUT_PORT1 = 0b00010011
// INPUT_PORT2 = 0b01101100
u8 JoyMega_Read3(enum INPUT_PORT port) __NAKED
{
	port; // A

__asm
	// Setup
	ld		h, a
	and		#0b11001111				// Set Pin 8 LOW
	ld		l, a

jm3_detect_start:
	INPUT_DI
	// Get: [..CBRLDU]
	ld		a, #15
	out		(P_PSG_REGS), a			// Select R#15
	ld		a, l
	out		(P_PSG_DATA), a			// Set Pin 8 LOW
	ld		a, #14
	out		(P_PSG_REGS), a			// Select R#14
	in		a, (P_PSG_STAT)			// Read R#14
	and		#0b00111111
	ld		b, a					// Backup: [00CBRLDU]

	// Get: [..SA....]
	ld		a, #15
	out		(P_PSG_REGS), a			// Select R#15
	ld		a, h
	out		(P_PSG_DATA), a			// Set Pin 8 HIGH
	ld		a, #14
	out		(P_PSG_REGS), a			// Select R#14
	in		a, (P_PSG_STAT)			// Read R#14

	// Merge result
	INPUT_EI
	and		#0b00110000
	add		a, a
	add		a, a					// Compute: [SA000000]
	or		a, b					// Merge: [SACBRLDU]
	ret								// Return A

__endasm;
}

//-----------------------------------------------------------------------------
// Read Megadrive 6-button joypad through JoyMega adapter
//
// 			TH out	TR in	TL in	D3 in	D2 in	D1 in	D0 in
// Cycle	Pin8	Pin7	Pin6	Pin4	Pin3	Pin2	Pin1
//----------------------------------------------------------------
// <1>		LO		C		B		Right	Left	Down	Up
// <2>		HI		Start	A		0		0		Down	Up
//  3		LO		C		B		Right	Left	Down	Up
//  4		HI		Start	A		0		0		Down	Up
//  5		LO		C		B		Right	Left	Down	Up
//  6		HI		Start	A		0		0		0		0
// <7>		LO		C		B		Mode	X		Y		Z
//  8		HI		Start	A		---		---		---		---
//
// INPUT_PORT1 = 0b00010011
// INPUT_PORT2 = 0b01101100
u16 JoyMega_Read6(enum INPUT_PORT port) __NAKED
{
	port; // A

__asm
	// Setup
	ld		h, a
	and		#0b11001111				// Set Pin 8 LOW
	ld		l, a

jm6_detect_start:
	INPUT_DI
	call	jm6_set_low				// Set: Phase 1
	call	jm6_get					// Get: [..CBRLDU]
	and		#0b00111111
	ld		b, a					// Backup: [00CBRLDU]

	call	jm6_set_high			// Set: Phase 2
	call	jm6_get					// Get: [..SA....]
	and		#0b00110000
	add		a, a
	add		a, a					// Compute: [SA000000]
	or		a, b					// Merge: [SACBRLDU]
	ld		e, a					// Store in E

	call	jm6_set_low				// Set: Phase 3
	call	jm6_set_high			// Set: Phase 4
	call	jm6_set_low				// Set: Phase 5
	call	jm6_set_high			// Set: Phase 6
	call	jm6_get					// Get: [....0000]
	and		#0x0F					// Keep low 4-bit; should be 0 for a 6-button joypad
	jp		nz, jm6_detect_failed

	call	jm6_set_low				// Set: Phase 6
	call	jm6_get					// Get: [....MXYZ]
	and		#0x0F					// Compute: [0000MXYZ]
	ld		d, a					// Store in D

	call	jm6_set_high			// Set: Phase 7 (reset the joystick internal counter)
	INPUT_EI

	ret								// return DE

jm6_set_high:
	ld		a, #15
	out		(P_PSG_REGS), a			// Select R#15
	ld		a, h
	out		(P_PSG_DATA), a			// Set Pin 6 LOW
	ret

jm6_set_low:
	ld		a, #15
	out		(P_PSG_REGS), a			// Select R#15
	ld		a, l
	out		(P_PSG_DATA), a			// Set Pin 6 LOW
	ret

jm6_get:
	ld		a, #14
	out		(P_PSG_REGS), a			// Select R#14
	in		a, (P_PSG_STAT)			// Read R#14
	ret

jm6_detect_failed:
	ld		d, #0					// Reset D
	ret								// Return DE (in 3-butons mode)

__endasm;
}
