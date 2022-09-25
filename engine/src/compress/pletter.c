// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Pletter v0.5c - XL2S Entertainment 2008
// Copyright (c) 2002-2003 Team Bomba under MIT License
//─────────────────────────────────────────────────────────────────────────────
#include "pletter.h"

#define LENGTHINDATA 0

// Unpack Pletter compressed data to a RAM buffer
// call unpack with hl pointing to some pletter5 data, and de pointing to the destination.
// changes all registers
// define lengthindata when the original size is written in the pletter data
// define LENGTHINDATA
void Pletter_UnpackToRAM(const void* source, void* dest) __naked
{
	source;	// HL
	dest;	// DE

__asm
	push	ix
	call	unpack
	pop		ix
	ret

unpack:
#if (LENGTHINDATA)
	inc		hl
	inc		hl
#endif

	ld		a, (hl)
	inc		hl
	exx
	ld		de, #0
	add		a, a
	inc		a
	rl		e
	add		a, a
	rl		e
	add		a, a
	rl		e
	rl		e
	ld		hl, #modes
	add		hl, de
	ld		e, (hl)
	// ld		ixl, e
	.db 0xDD, 0x6B
	inc		hl
	ld		e, (hl)
	// ld		ixh, e
	.db 0xDD, 0x63
	ld		e, #1
	exx
	ld		iy, #loop
literal:
	ldi
loop:
	add		a, a
	call	z, getbit
	jr		nc, literal
	exx
	ld		h, d
	ld		l, e
getlen:
	add		a
	call	z, getbitexx
	jr		nc, getlen_lenok
getlen_lus:
	add		a
	call	z, getbitexx

	adc		hl, hl
	ret		c
	add		a
	call	z, getbitexx

	jr		nc, getlen_lenok
	add		a
	call	z, getbitexx

	adc 	hl, hl
	ret		c
	add		a
	call	z, getbitexx

	jp		c, getlen_lus
getlen_lenok:
	inc		hl
	exx
	ld		c, (hl)
	inc		hl
	ld		b, #0
	bit		7, c
	jp		z, offsok
	jp		(ix)

mode6:
	add		a
	call	z, getbit
	rl		b
mode5:
	add		a
	call	z, getbit
	rl		b
mode4:
	add		a
	call	z, getbit
	rl		b
mode3:
	add		a
	call	z, getbit
	rl		b
mode2:
	add		a
	call	z, getbit

	rl		b
	add		a
	call	z, getbit

	jr		nc, offsok
	or		a
	inc		b
	res		7, c
offsok:
	inc		bc
	push	hl
	exx
	push	hl
	exx
	ld		l, e
	ld		h, d
	sbc		hl, bc
	pop		bc
	ldir
	pop		hl
	jp		(iy)

getbit:
	ld		a, (hl)
	inc		hl
	rla
	ret

getbitexx:
	exx
	ld		a, (hl)
	inc		hl
	exx
	rla
	ret

modes:
	.dw		offsok
	.dw		mode2
	.dw		mode3
	.dw		mode4
	.dw		mode5
	.dw		mode6

__endasm;
}
