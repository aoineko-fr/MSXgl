// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄  ▄▄   ▄▄   ▄▄   ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ █▄██ █▄██ █▄██ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▀   ▄█▀  ▄█▀  ▄█▀ ▀▄█▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// References:
// - 
//─────────────────────────────────────────────────────────────────────────────
#include "v9990.h"

//
struct V9_Address g_V9_Address;

//-----------------------------------------------------------------------------
//
bool V9_Detect()
{
	__asm
	v9_detect:
		ld		a, #V9_REG(15) + V9_RII + V9_WII
		out		(V9_P4), a				// Select R#15

		in		a, (V9_P3)				// Read value
		ld		b, a
		cpl								// Invert value
		out		(V9_P3), a				// Write value
		in		a, (V9_P3)				// Read value again
		xor		b
		jr		z, v9_notfound
	v9_found:							// Return A=0xFF
		ld		c, #V9_P3
		out		(c), b					// Restore value
	v9_notfound:						// Return A=0x00
	__endasm;
}

//-----------------------------------------------------------------------------
//
void V9_SetRegister(u8 reg, u8 val)
{
	reg;	// A
	val;	// L
	__asm
	v9_set_reg:
		out		(V9_P4), a				// Select register
		ld		a, l
		out		(V9_P3), a				// Set value
	__endasm;
}

//-----------------------------------------------------------------------------
//
u8 V9_GetRegister(u8 reg)
{
	reg;	// A
	__asm
	v9_get_reg:
		out		(V9_P4), a				// Select register
		in		a, (V9_P3)				// Get value
	__endasm;
}


/*

//-----------------------------------------------------------------------------
//
void V9_SetWriteAddress(struct V9_Address* addr)
{
	addr; // HL
	__asm
	v9_set_write_addr:					//-cc-
		xor		a						//  5	
		out		(V9_P4), a				// 12	Select R#0 (with write increment)
		ld		c, #V9_P3				//  8	
		outi							// 18	R#0 (lower address)
		outi							// 18	R#1 (center address)
		outi							// 18	R#2 (upper address)
	__endasm;							//-total: 79 cc
}

/*void V9_SetWriteAddress(u8 up, u16 low)
{
	up;		// A
	low;	// DE
	__asm
	v9_set_write_addr:						//-cc-
		ld		b, a					//  5
		xor		a						//  5
		out		(V9_P4), a				// 12	Select R#0 (with write increment)
		ld		c, #V9_P3				//  8
		out		(c), d					// 14	R#0 (lower address)
		out		(c), e					// 14	R#1 (center address)
		out		(c), b					// 14	R#2 (upper address)
	__endasm;							//-total: 72 cc
}/

//-----------------------------------------------------------------------------
//
void V9_FillVRAM(u8 value, struct V9_Address* dest, u16 count) __naked
{
	value;	// A
	dest;	// DE
	count;	// SP+2
	__asm
	// Setup destination address
	v9_fill_addr:
		ld		h, d					// DE -> HL
		ld		l, e
		ld		b, a					// Backup value
		xor		a
		out		(V9_P4), a				// Select R#0 (with write increment)
		ld		c, #V9_P3
		outi							// Set R#0 (lower address)
		outi							// Set R#1 (center address)
		outi							// Set R#2 (upper address)
	// Setup fast 16-bits loop
	v9_fill_setup:
		ld		a, b					// Retreive value
		pop		iy
		pop		de						// Get counter
		ld		b, e					// Compute DB value... B is count LSB 
		dec		de
		inc		d						// D is count's MSB + 1 except when LSB is equal 0 (in this case D equal count's MSB)
	// Fast 16-bits loop
	v9_fill_loop:
		out		(V9_P0), a				// Fill VRAM
		djnz	v9_fill_loop			// Iner 8-bits loop
		dec		d
		jr		nz, v9_fill_loop		// Outer 8-bits loop
		
		jp		(iy)
__endasm;
}

//-----------------------------------------------------------------------------
//
void V9_WriteVRAM(u8* src, struct V9_Address* dest, u16 count) __naked
{
	src;	// HL -> DE
	dest;	// DE -> HL
	count;	// SP+2
	__asm
	// Setup destination address
	v9_write_addr:
		ex		de, hl
		xor		a
		out		(V9_P4), a				// Select R#0 (with write increment)
		ld		c, #V9_P3
		outi							// Set R#0 (lower address)
		outi							// Set R#1 (center address)
		outi							// Set R#2 (upper address)
	// Setup fast 16-bits loop
	v9_write_setup:
		ld		a, b					// Retreive value
		pop		iy
		pop		de						// Get counter
		ld		b, e					// Compute DB value... B is count LSB 
		dec		de
		inc		d						// D is count's MSB + 1 except when LSB is equal 0 (in this case D equal count's MSB)
		ld		c, #V9_P0
	// Fast 16-bits loop
	v9_write_loop:
		outi							// Write to VRAM
		jr		nz, v9_write_loop		// Iner 8-bits loop
		dec		d
		jr		nz, v9_write_loop		// Outer 8-bits loop
		
		jp		(iy)
__endasm;
}

//-----------------------------------------------------------------------------
//
void V9_SetReadAddress(struct V9_Address* addr)
{
	addr; // HL
__asm
set_read_addr:
	ld		#V9_REG(3)				// Select R#3 (with write increment)
	out		(V9_P4), a
	ld		c, #V9_P3
	outi							// R#0 (lower address)
	outi							// R#1 (center address)
	outi							// R#2 (upper address)
__endasm;
}


//-----------------------------------------------------------------------------
//
void V9_ReadVRAM(struct V9_Address* src, u8* dest, u16 count)
{
	src;	// HL
	dest;	// DE
	count;	// SP+2
	// V9_SetWriteAddress(dest);
}*/