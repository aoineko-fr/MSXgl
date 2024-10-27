// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄ ▄▄ ▄  ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ▄█▀ ▀█▄▀ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▄ ██ █ ▀▄█▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// LZ48 decoder by Roudoudou
//  https://www.cpcwiki.eu/forum/programming/lz48-cruncherdecruncher/
//─────────────────────────────────────────────────────────────────────────────
#include "lz48.h"
#include "asm.h"
#include "system_port.h"
#include "vdp.h"

#if (LZ48_MODE == LZ48_MODE_STANDARD)

//-----------------------------------------------------------------------------
// Unpack LZ48 compressed data to a RAM buffer
// "Standard" version
void LZ48_UnpackToRAM(const void* source, void* dest)
{
	source;	// HL
	dest;	// DE
	__asm
//-----------------------------------------------------------------------------
// LZ48 decrunch
//
// hl  compressed data adress
// de  output adress of data
ram_decrunch:
	ldi
	ld		b, #0

ram_nextsequence:
	ld		a, (hl)
	inc		hl
	// ld		ixl, a
	ld______iyl_a
	and		#0xF0
	jr		z, ram_lzunpack // no litteral bytes
	rrca
	rrca
	rrca
	rrca

	ld		c, a
	cp		#15 // more bytes for length?
	jr		nz, ram_copyliteral

ram_getadditionallength:
	ld		a, (hl)
	inc		hl
	inc		a
	jr		nz, ram_lengthnext
	inc		b
	dec		bc
	jr		ram_getadditionallength
ram_lengthnext:
	dec		a
	add		a, c
	ld		c, a
	ld		a, b
	adc		a, #0
	ld		b, a // bc=length

ram_copyliteral:
	ldir

ram_lzunpack:
	// ld		a, lx
	ld______a_iyl
	and		#0x0F
	add		#3
	ld		c, a
	cp		#18 // more bytes for length?
	jr		nz, ram_readoffset

ram_getadditionallengthbis:
	ld		a, (hl)
	inc		hl
	inc		a
	jr		nz, ram_lengthnextbis
	inc		b
	dec		bc
	jr		ram_getadditionallengthbis
ram_lengthnextbis:
	dec		a
	add		a, c
	ld		c, a
	ld		a, b
	adc		a, #0
	ld		b, a // bc=length

ram_readoffset:
	// read encoded offset
	ld		a,(hl)
	inc		a
	ret		z // LZ48 end with zero offset
	inc		hl
	push	hl
	ld		l, a
	ld		a, e
	sub		l
	ld		l, a
	ld		a, d
	sbc		a, #0
	ld		h, a
	// source=dest-copyoffset

ram_copykey:
	ldir

	pop		hl
	jr		ram_nextsequence
//-----------------------------------------------------------------------------
	__endasm;
}

#elif (LZ48_MODE == LZ48_MODE_SPEED)

//-----------------------------------------------------------------------------
// Unpack LZ48 compressed data to a RAM buffer
// "Standard" version
void LZ48_UnpackToRAM(const void* source, void* dest)
{
	source;	// HL
	dest;	// DE
	__asm
//-----------------------------------------------------------------------------
// LZ48 decrunch
//
// In:	HL = compressed data address
//		DE = output data address
// Out:	HL    last address of compressed data read (you must inc once for LZ48 stream)
//		DE    last address of decrunched data write +1
//		BC    always 1
//		A     always zero
//		flags (Z,NC,P,PE)
//		HL'   address of lzunpack(if OPTIMIZE_JUMP=2)
//		E'    undetermined
//		BC'   always 0f10h
// Modif: AF, BC, DE, HL, BC', E', HL'

// DEFINE OPTIMIZE_JUMP 2

LZ48_decrunch:
	ldi
	ld		b, #0

	exx
	ld		bc, #0x0F10
//   IFNDEF OPTIMIZE_JUMP
//   ELSE
//     IF OPTIMIZE_JUMP=2
// 	ld hl,lzunpack
//     ENDIF
//   ENDIF
	exx
	jr		nextsequence

loop:
	ld		e, a
	rrca
	rrca
	rrca
	rrca

	and		b
	cp		b // more bytes for literal length?
	exx

	call	nc, getadditionallength

copyliteral:
	ld		c, a
	ldir

	exx
	ld		a, e
	and		b
lzunpack:
	cp		b // more bytes for match length?
	inc		a
	exx

	call	nc, getadditionallength

readoffset:
	ld		c, a
// read encoded offset
	sbc		a, (hl)
	sub		c
	ret		z // LZ48 end with zero offset
	inc		hl
	push	hl
// source=dest-copyoffset
	// A != 0 here
	ld		l, a
	ld		h, #0xFF
	add		hl, de
	ldir
	inc		b
	ldi
	ldi

	pop		hl

nextsequence:
	ld		a, (hl)
	inc		hl

	exx
	cp		c
	jr		nc, loop
//   IFNDEF OPTIMIZE_JUMP
	jr lzunpack // no literal bytes
//   ELSE
//     IF OPTIMIZE_JUMP=2
// 	jp (hl)
//     ELSE
//       IF OPTIMIZE_JUMP=1
// 	jp lzunpack // no literal bytes
//       ELSE
// 	jr lzunpack // no literal bytes
//       ENDIF
//     ENDIF
//   ENDIF

lengthNC:
	scf
lengthC:
	inc		c
	ret		nz

getadditionallength:
	ld		c, (hl) // get additional literal length byte
	inc		hl
	add		a, c // compute literal length total
	jr		nc, lengthNC
	inc		b
	jr		lengthC
//-----------------------------------------------------------------------------
	__endasm;
}

#elif (LZ48_MODE == LZ48_MODE_SIZE)

//-----------------------------------------------------------------------------
// Unpack LZ48 compressed data to a RAM buffer
// "Standard" version
void LZ48_UnpackToRAM(const void* source, void* dest)
{
	source;	// HL
	dest;	// DE
	__asm
//-----------------------------------------------------------------------------
// LZ48 decrunch
//
// In:	HL = compressed data address
// 		DE = output data address
// Out:	HL    last address of compressed data read (you must inc once for LZ48 stream)
//		DE    last address of decrunched data write +1
//		BC    always 1
//		A     always zero
//		flags (Z,NC,P,PE)
// Modif: AF, BC, DE, HL

LZ48_decrunch:
	ldi
	ld		b, #0

	jr		nextsequence

loop:
	push	af

	rrca
	rrca
	rrca
	rrca

	and		#0x0F
	cp		#0x0F // more bytes for literal length?

	call	nc, getadditionallength

copyliteral:
	ld		c, a
	ldir

	pop		af
	and		#0x0F
lzunpack:
	cp		#0x0F // more bytes for match length?
	inc		a

	call	nc, getadditionallength

readoffset:
	ld		c, a
// read encoded offset
	sbc		a, (hl)
	sub		c
	ret		z // LZ48 end with zero offset
	inc		hl
	push	hl
// source=dest-copyoffset
	// A != 0 here

	ld		l, a
	ld		h, #0xFF
	add		hl, de
	inc		bc
	inc		bc
	ldir

	pop		hl

nextsequence:
	ld		a, (hl)
	inc		hl

	cp		#0x10
	jr		nc, loop
	jr		lzunpack

lengthNC:
	scf
lengthC:
	inc		c
	ret		nz

getadditionallength:
	ld		c, (hl) // get additional literal length byte
	inc		hl
	add		a, c // compute literal length total
	jr		nc, lengthNC
	inc		b
	jr		lengthC
//-----------------------------------------------------------------------------
	__endasm;
}

#endif