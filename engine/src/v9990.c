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
V9_Address g_V9_Address;

//-----------------------------------------------------------------------------
//
void V9_SetWriteAddress(const V9_Address* addr)
{
	addr; // HL
__asm
set_write_addr:						//-cc-
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
set_write_addr:						//-cc-
	ld		b, a					//  5
	xor		a						//  5
	out		(V9_P4), a				// 12	Select R#0 (with write increment)
	ld		c, #V9_P3				//  8
	out		(c), d					// 14	R#0 (lower address)
	out		(c), e					// 14	R#1 (center address)
	out		(c), b					// 14	R#2 (upper address)
__endasm;							//-total: 72 cc
}*/

//-----------------------------------------------------------------------------
//
void V9_FillVRAM(u8 value, const V9_Address* dest, u16 count)
{
	value;	// A
	dest;	// DE
	count;	// SP+2
	V9_SetWriteAddress(dest);
}

//-----------------------------------------------------------------------------
//
void V9_WriteVRAM(const void* src, const V9_Address* dest, u16 count)
{
	src;	// HL
	dest;	// DE
	count;	// SP+2
	V9_SetWriteAddress(dest);
}

//-----------------------------------------------------------------------------
//
void V9_SetReadAddress(const V9_Address* addr)
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
void V9_ReadVRAM(const V9_Address* src, u8* dest, u16 count)
{
	src;	// HL
	dest;	// DE
	count;	// SP+2
	V9_SetWriteAddress(dest);
}