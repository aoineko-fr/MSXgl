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
u8 JoyMega_Read3(u8 port) __NAKED
{
	port; // A

__asm
	// Setup
	or		a
	jp		nz, jm3_setup_port2 
jm3_setup_port1:
	ld		hl, (jm_data_port1)
	jp		jm3_detect_start 
jm3_setup_port2:
	ld		hl, (jm_data_port2)

jm3_detect_start:
	INPUT_DI
	call	jm_get_high				// Get: xxCBRLDU
	ld		b, a

	call	jm_get_low				// Get: xxSAxxxx
	INPUT_EI
	and		#0b00110000
	add		a, a
	add		a, a					// Compute: SAxxxxxx

	or		a, b					// Merge: SACBRLDU

	ret								// Return A

jm_get_high:
	ld		a, #15
	out		(P_PSG_REGS), a			// Select R#15
	ld		a, h
	out		(P_PSG_DATA), a			// Set Pin 6 LOW

	ld		a, #14
	out		(P_PSG_REGS), a			// Select R#14
	in		a, (P_PSG_STAT)			// Read R#14

	ret

jm_get_low:
	ld		a, #15
	out		(P_PSG_REGS), a			// Select R#15
	ld		a, l
	out		(P_PSG_DATA), a			// Set Pin 6 LOW

	ld		a, #14
	out		(P_PSG_REGS), a			// Select R#14
	in		a, (P_PSG_STAT)			// Read R#14

	ret

jm_data_port1:
			// S 8  76
	.db		0b00000011 // Pin 8 LOW
	.db		0b00010011 // Pin 8 HIGH

jm_data_port2:
			// S8 76
	.db		0b01001100 // Pin 8 LOW
	.db		0b01101100 // Pin 8 HIGH

__endasm;
}

//-----------------------------------------------------------------------------
// Read Megadrive 6-button joypad through JoyMega adapter
u16 JoyMega_Read6(u8 port) __NAKED
{
	port; // A

__asm
	// Setup
	or		a
	jp		nz, jm3_setup_port2 
jm3_setup_port1:
	ld		hl, (jm_data_port1)
	jp		jm3_detect_start 
jm3_setup_port2:
	ld		hl, (jm_data_port2)

jm3_detect_start:
	INPUT_DI
	call	jm_get_high				// Get: xxCBRLDU
	ld		b, a

	call	jm_get_low				// Get: xxSAxxxx
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
