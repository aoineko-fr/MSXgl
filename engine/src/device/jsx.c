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
#include "input.h"
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

// INPUT_PORT1 = 0 0 01 00 11
// INPUT_PORT2 = 0 1 10 11 00

//-----------------------------------------------------------------------------
//
u8 JSX_Detect(enum INPUT_PORT port) __NAKED
{
	port; // A

__asm

	ld		h, a
	and		#0b11110000
	ld		l, a

jsx_detect_start:
	ld		a, #15
	out		(P_PSG_REGS), a			// Select R#15

	ld		a, #0b00000000
	out		(P_PSG_DATA), a			// Set Pin 6 and 8 LOW

	ld		a, l
	out		(P_PSG_DATA), a			// Set Pin 6 LOW and Pin 8 HIGH

	ld		a, h
	out		(P_PSG_DATA), a			// Set Pin 6 and Pin 8 HIGH

	ld		a, #14
	out		(P_PSG_REGS), a			// Select R#14
	in		a, (P_PSG_STAT)			// Read R#14
	and		#0x3F
	ret
__endasm;
}

//-----------------------------------------------------------------------------
//
u8 JSX_Read(enum INPUT_PORT port, u8* buffer) __NAKED
{
	port;   // A
	buffer; // DE

__asm

	ld		h, a
	and		#0b11110000
	ld		l, a

jsx_read_detect_start:
	ld		a, #15
	out		(P_PSG_REGS), a			// Select R#15

	ld		a, #0b00000000
	out		(P_PSG_DATA), a			// Set Pin 6 and 8 LOW

	ld		a, l
	out		(P_PSG_DATA), a			// Set Pin 6 LOW and Pin 8 HIGH

	ld		a, h
	out		(P_PSG_DATA), a			// Set Pin 6 and Pin 8 HIGH

	ld		a, #14
	out		(P_PSG_REGS), a			// Select R#14
	in		a, (P_PSG_STAT)			// Read R#14
	and		#0x3F

	//  7 6 5 4 3 2 1 0
	// –-–-–-–-–-–-–-–--
	//  0 0 A A A A B B
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

	ld		a, l
	out		(P_PSG_DATA), a			// Set Pin 6 LOW and Pin 8 HIGH

	ld		a, l
	out		(P_PSG_DATA), a			// Set Pin 6 and Pin 8 HIGH

	ld		a, #14
	out		(P_PSG_REGS), a			// Select R#14
	in		a, (P_PSG_STAT)			// Read R#14
	and		#0x3F

	ld		(de), a					// Store read byte in destination buffer 
	inc		de

	djnz	jsx_read_detect_loop

	ld		a, c
	ret
__endasm;
}

//-----------------------------------------------------------------------------
//
// INPUT_PORT1_HIGH			0b00010011
// INPUT_PORT2_HIGH			0b01101100
u8 JSXC_Detect(enum INPUT_PORT port) __NAKED
{
	port; // A

__asm
	ld		h, a
	and		#0b11001111
	ld		l, a

	INPUT_DI

jsxc_detect_start:
	// Set Pin 8 LOW
	ld		a, #15
	out		(P_PSG_REGS), a			// Select R#15
	ld		a, l
	out		(P_PSG_DATA), a			// Set Pin 8 LOW

	// Reset counter
	ld		b, #0					// 8 t-states
jsxc_detect_wait_1ms:
	djnz	jsxc_detect_wait_1ms	// 14/9 t-states
	// 8 + (14 * 255) + 9 = 3587 t-states
	// 3587 * 0,279 µs = 1000 µs = 1 ms

jsxc_detect_get_id:
	// Set Pin 8 HIGH
	ld		a, h
	out		(P_PSG_DATA), a			// Set Pin 8 HIGH

	ld		a, #14
	out		(P_PSG_REGS), a			// Select R#14
	nop
	in		a, (P_PSG_STAT)			// Read R#14
	and		#0x3F

	INPUT_EI
	ret
__endasm;
}

//-----------------------------------------------------------------------------
// Read data from a JSX-C compatible device
//
// Parameters:
//   port - General Purpose port to be read
//          Can be INPUT_PORT1_HIGH (0b00010011) or INPUT_PORT2_HIGH (0b01101100).
//   buffer - Memory pointer to receive read data
//
// Return:
//   Device ID
u8 JSXC_Read(enum INPUT_PORT port, u8* buffer) __NAKED
{
	port;   // A
	buffer; // DE

__asm

// Setup
	ld		h, a					// Retreive pins value for HIGH state
	and		#0b11001111				// Compute pins value for LOW state (reset pins 8)
	ld		l, a					

	INPUT_DI

// Send a pusle and pin 8, retreive Device ID and compute the number of bytes to read
jsxc_read_detect_start:
	// 
	ld		a, #15
	out		(P_PSG_REGS), a			// Select R#15
	ld		a, l
	out		(P_PSG_DATA), a			// Set pins to LOW state
	ld		a, h
	out		(P_PSG_DATA), a			// Set pins to LOW state -> Pulse!

	ld		a, #14
	out		(P_PSG_REGS), a			// Select R#14
	nop								// Delay to read R#14 37 T-States after the Pulse
	in		a, (P_PSG_STAT)			// Read R#14

	// Compute the number of bytes to read (number of axis + number of button rows) from Device ID:
	//  7 6 5 4 3 2 1 0
	// –-–-–-–-–-–-–-–--
	//  0 0 A A A A B B
	//      │ │ │ │ └─┴── Number of button rows (0-3)
	//      └─┴─┴─┴────── Number of axis (0-15)

	and		#0x3F					// Mask to keep only the lower 6 bits
	ld		c, a					// Backup device ID

	and		#0b00000011				// Compute button rows number (just masking the lower 2 bits)
	ld		b, a					

	ld		a, c					// Restore device ID
	rrca							// Compute axis number (right shift twice)
	rrca
	and		#0b00001111				// Masking the lower 6 bits shouldn't be necessary

	add		a, b					// Add number of axis and number of button rows to know the number of bytes to read

	ld		b, a					// Setup the loop counter

// Send pulse on pin 8 and read a byte
jsxc_read_detect_loop:
	ld		a, #15
	out		(P_PSG_REGS), a			// Select R#15
	ld		a, l
	out		(P_PSG_DATA), a			// Set pins to LOW state
	ld		a, h
	out		(P_PSG_DATA), a			// Set pins to LOW state -> Pulse!

	ld		a, #14
	out		(P_PSG_REGS), a			// Select R#14
	nop								// Delay to read R#14 37 T-States after the Pulse
	in		a, (P_PSG_STAT)			// Read R#14

	and		#0x3F					// Mask to keep only the lower 6 bits
	ld		(de), a					// Store read byte in destination buffer 
	inc		de

	djnz	jsxc_read_detect_loop

// Set pins LOW to reset the device internal counter before the next call to JSX-C driver
jsxc_read_detect_end:
	ld		a, #15
	out		(P_PSG_REGS), a			// Select R#15
	ld		a, l
	out		(P_PSG_DATA), a			// Set pins to LOW state for the Device internal

	INPUT_EI

	ld		a, c					// Return the Device ID
	ret
__endasm;
}