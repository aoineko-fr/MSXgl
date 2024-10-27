// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄  ▄▄▄ ▄▄ ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ▀█▄  ▀█▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄█▀  ▄▄█▀ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// JSX driver module
//─────────────────────────────────────────────────────────────────────────────
#include "jsx.h"
#include "system_port.h"

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// READ-ONLY DATA
//=============================================================================

//=============================================================================
// MEMORY DATA
//=============================================================================

// Input buffer
// extern u8 g_JSX_InputBuffer[2][JSX_MAX_INPUT];

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// R#14   I/O Parallel Port A (ready)
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	CR	JIS	P7	P6	P4	P3	P2	P1 	
//  │	│	│	│	│	│	│	└── Pin 1 state of the selected general port (Up if joystick)
//	│	│	│	│	│   │   └────── Pin 2 state of the selected general port (Down if joystick)
//	│	│	│	│	│   └────────── Pin 3 state of the selected general port (Left if joystick)
//	│	│	│	│	└────────────── Pin 4 state of the selected general port (Right if joystick)
//	│	│	│	└────────────────── Pin 6 state of the selected general port (Trigger A if joystick)
//	│	│	└────────────────────── Pin 7 state of the selected general port (Trigger B if joystick)
//	│	└────────────────────────── 1 for JIS keyboard, 0 for JP50on (only valid for Japanese MSX)
//	└────────────────────────────── CASRD (Reading signal on cassette)

//-----------------------------------------------------------------------------
// R#15   I/O Parallel Port B (write)
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	LED	SEL	B8	A8	B7	B6	A7	A6 	
//  │	│	│	│	│	│	│	└── Pin control 6 of the general port 1
//	│	│	│	│	│   │   └────── Pin control 7 of the general port 1
//	│	│	│	│	│   └────────── Pin control 6 of the general port 2
//	│	│	│	│	└────────────── Pin control 7 of the general port 2
//	│	│	│	└────────────────── Pin control 8 of the general port 1 (0 for standard joystick mode)
//	│	│	└────────────────────── Pin control 8 of the general port 2 (0 for standard joystick mode)
//	│	└────────────────────────── Selection of the general port readable via register 14 (1 for port 2)
//	└────────────────────────────── LED control of the "Code" or "Kana" key. (1 to turn off)

//-----------------------------------------------------------------------------
//
u8 JSX_Detect(u8 port) __NAKED
{
	port; // A

__asm
	or		a
	jp		nz, jsx_setup_port2 
jsx_setup_port1:
	ld		hl, #jsx_data_port1
	jp		jsx_detect_start 
jsx_setup_port2:
	ld		hl, #jsx_data_port2

jsx_detect_start:
	ld		a, #15
	out		(P_PSG_REGS), a			// Select R#15

	ld		a, #0b00000000
	out		(P_PSG_DATA), a			// Set Pin 6 and 8 LOW

	ld		a, (hl)
	out		(P_PSG_DATA), a			// Set Pin 6 LOW and Pin 8 HIGH

	inc		hl
	ld		a, (hl)
	out		(P_PSG_DATA), a			// Set Pin 6 and Pin 8 HIGH

	ld		a, #14
	out		(P_PSG_REGS), a			// Select R#14
	in		a, (P_PSG_STAT)			// Read R#14

	ret								// Return value

jsx_data_port1:
	.db		0b00010000, 0b00010001

jsx_data_port2:
	.db		0b01100000, 0b01100100

__endasm;
}

//-----------------------------------------------------------------------------
//
u8 JSX_Read(u8 port, u8* buffer) __NAKED
{
	port;   // A
	buffer; // DE

__asm
	or		a
	jp		nz, jsx_read_setup_port2 
jsx_read_setup_port1:
	ld		hl, #jsx_data_port1
	jp		jsx_read_detect_start 
jsx_read_setup_port2:
	ld		hl, #jsx_data_port2

jsx_read_detect_start:
	ld		a, #15
	out		(P_PSG_REGS), a			// Select R#15

	ld		a, #0b00000000
	out		(P_PSG_DATA), a			// Set Pin 6 and 8 LOW

	ld		a, (hl)
	out		(P_PSG_DATA), a			// Set Pin 6 LOW and Pin 8 HIGH

	inc		hl
	ld		a, (hl)
	out		(P_PSG_DATA), a			// Set Pin 6 and Pin 8 HIGH

	ld		a, #14
	out		(P_PSG_REGS), a			// Select R#14

	in		a, (P_PSG_STAT)			// Read R#14

	//  7 6 5 4 3 2 1 0
	// –-–-–-–-–-–-–-–--
	//  x x A A A A B B
	//      │ │ │ │ └─┴── Number of button rows (0-3)
	//      └─┴─┴─┴────── Number of axis (0-15)

	ld		c, a					// Backup device ID

	and		#0b00000011				// Compute button rows number
	ld		b, a					

	ld		a, c					// Restore
	rrca							// Compute button rows number
	rrca
	and		#0b00001111

	add		a, b					// Total number of bytes to read

	ld		b, a					// Setup loop counter

jsx_read_detect_loop:

	ld		a, #15
	out		(P_PSG_REGS), a			// Select R#15

	dec		hl
	ld		a, (hl)
	out		(P_PSG_DATA), a			// Set Pin 6 LOW and Pin 8 HIGH

	inc		hl
	ld		a, (hl)
	out		(P_PSG_DATA), a			// Set Pin 6 and Pin 8 HIGH

	ld		a, #14
	out		(P_PSG_REGS), a			// Select R#14

	in		a, (P_PSG_STAT)			// Read R#14

	ld		(de), a					// Store read byte in destination buffer 
	inc		de

	djnz	jsx_read_detect_loop

	ld		a, c
	ret								// Return value

// jsx_data_port1:
// 	.db		0b00010000, 0b00010001

// jsx_data_port2:
// 	.db		0b01100000, 0b01100100

__endasm;
}