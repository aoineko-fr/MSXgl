// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▄███ ▄█▄█ ▄█▀▄ ██▄▀ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀█▄▄ ██ █ ▀█▄▀ ██    ▀██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                            ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Memory operations
//─────────────────────────────────────────────────────────────────────────────
#include "core.h"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Left-shift a memory buffer
void Mem_LShift(const u8* ptr, u16 size)
{
	ptr;	// HL
	size;	// DE -> BC

__asm
	ld		b, d
	ld		c, e
	dec		bc
	ld		a, (hl)				// A = ptr[0]
	ld		d, h
	ld		e, l
	inc		hl
	ldir
	ld		(de), a				// ptr[n-1] = A
__endasm;
}

//-----------------------------------------------------------------------------
// Right-shift a memory buffer
void Mem_RShift(const u8* ptr, u16 size)
{
	ptr;	// HL
	size;	// DE -> BC

__asm
	ld		b, d
	ld		c, e
	dec		bc
	add		hl, bc
	ld		a, (hl)				// A = ptr[n-1]
	ld		d, h
	ld		e, l
	dec		hl
	lddr
	ld		(de), a				// ptr[0] = A
__endasm;
}

//-----------------------------------------------------------------------------
// Increment a memory buffer
void Mem_Inc(const u8* ptr, u16 size)
{
	ptr;	// HL
	size;	// DE

__asm
	ld		b, e					// number of loops is in DE
	dec		de						// calculate DB value (destroys B, D and E)
	inc		d
memIncLoop:
	inc		(hl)
	inc		hl
	djnz	memIncLoop
	dec		d
	jp		nz, memIncLoop
__endasm;
}

//-----------------------------------------------------------------------------
// Decrement a memory buffer
void Mem_Dec(const u8* ptr, u16 size)
{
	ptr;	// HL
	size;	// DE

__asm
	ld		b, e					// number of loops is in DE
	dec		de						// calculate DB value (destroys B, D and E)
	inc		d
memDecLoop:
	dec		(hl)
	inc		hl
	djnz	memDecLoop
	dec		d
	jp		nz, memDecLoop
__endasm;
}