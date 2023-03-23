// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄▄       ▄▄  ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██  ▄███ ██▀ ██▀ ▄███ ██▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ▀█▄ ▀█▄▄ ▀█▄ ▀█▄ ▀█▄▄ ██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Pletter v0.5c - XL2S Entertainment 2008
// Copyright (c) 2002-2003 Team Bomba under MIT License
//─────────────────────────────────────────────────────────────────────────────
#include "pletter.h"
#include "asm.h"
#include "system_port.h"

//=============================================================================
// OPTIONS VALIDATION
//=============================================================================

// PLETTER_LENGTHINDATA
#ifndef PLETTER_LENGTHINDATA
	#warning PLETTER_LENGTHINDATA is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define PLETTER_LENGTHINDATA	FALSE
#endif

// PLETTER_DI_MODE
#ifndef PLETTER_DI_MODE
	#warning PLETTER_DI_MODE is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define PLETTER_DI_MODE			PLETTER_DI_LOOP
#endif

// PLETTER_WRITE_MODE
#ifndef PLETTER_WRITE_MODE
	#warning PLETTER_WRITE_MODE is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define PLETTER_WRITE_MODE		PLETTER_WRITE_SAFE
#endif

// Disable interruption mode
#if (PLETTER_DI_MODE == PLETTER_DI_NONE)
	#define DI_FULL
	#define EI_FULL
	#define DI_LOOP
	#define EI_LOOP
#elif (PLETTER_DI_MODE == PLETTER_DI_FULL)
	#define DI_FULL		di
	#define EI_FULL		ei
	#define DI_LOOP
	#define EI_LOOP
#elif (PLETTER_DI_MODE == PLETTER_DI_LOOP)
	#define DI_FULL
	#define EI_FULL
	#define DI_LOOP		di
	#define EI_LOOP		ei
#endif

// VRAM write timing mode
#if (PLETTER_WRITE_MODE == PLETTER_WRITE_SAFE)
	#define WRITE_NOP	nop
#elif (PLETTER_WRITE_MODE == PLETTER_WRITE_QUICK)
	#define WRITE_NOP
#endif


//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Unpack Pletter compressed data to a RAM buffer
// call unpack with hl pointing to some pletter5 data, and de pointing to the destination.
// changes all registers
// Set PLETTER_LENGTHINDATA to TRUE when the original size is written in the pletter data
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
#if (PLETTER_LENGTHINDATA)
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
	ld______ixl_e
	inc		hl
	ld		e, (hl)
	ld______ixh_e
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

//-----------------------------------------------------------------------------
// Unpack Pletter compressed data directly to VRAM
// Only first 16 KB of VRAM can be use as destination (0000-3FFF)
void Pletter_UnpackToVRAM(const void* source, u16 dest)
{
	source;	// HL
	dest;	// DE

__asm
//-----------------------------------------------------------
// Pletter v0.5b VRAM Depacker v1.1 - 16Kb version
//  by  Metalion (22-02-2008, 16:54)
//  https://msx.org/forum/development/msx-development/bitbuster-depack-vram?page=7
// HL = RAM/ROM source
// DE = VRAM destination
//-----------------------------------------------------------
	DI_FULL
	push	ix

// VRAM address setup
	ld		a, e
	out		(P_VDP_1), a
	ld		a, d
	or		#0x40
	out		(P_VDP_1), a

// Initialization
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
	ld		hl, #v_modes
	add		hl, de
	ld		e, (hl)
	ld______ixl_e
	inc		hl
	ld		e, (hl)
	ld______ixh_e
	ld		e, #1
	exx
	ld		iy, #v_loop

// Main depack loop
v_literal:
	ld		c, #P_VDP_0
	outi
	inc		de
v_loop:
	add		a, a
	call	z, getbit
	jr		nc, v_literal

// Compressed data
	exx
	ld		h, d
	ld		l, e
v_getlen:
	add		a, a
	call	z, getbitexx
	jr		nc, v_lenok
v_lus:
	add		a, a
	call	z, getbitexx
	adc		hl, hl
	ret		c
	add		a, a
	call	z, getbitexx
	jr		nc, v_lenok
	add		a, a
	call	z, getbitexx
	adc		hl, hl
	jp		c, v_Depack_out
	add		a, a
	call	z, getbitexx
	jp		c, v_lus
v_lenok:
	inc		hl
	exx
	ld		c, (hl)
	inc		hl
	ld		b, #0
	bit		7, c
	jp		z, v_offsok
	jp		(ix)

v_mode7:
	add		a, a
	call	z, getbit
	rl		b
v_mode6:
	add		a, a
	call	z, getbit
	rl		b
v_mode5:
	add		a, a
	call	z, getbit
	rl		b
v_mode4:
	add		a, a
	call	z, getbit
	rl		b
v_mode3:
	add		a, a
	call	z, getbit
	rl		b
v_mode2:
	add		a, a
	call	z, getbit
	rl		b
	add		a, a
	call	z, getbit
	jr		nc, v_offsok
	or		a
	inc		b
	res		7, c
v_offsok:
	inc		bc
	push	hl
	exx
	push	hl
	exx
	ld		l, e
	ld		h, d
	sbc		hl, bc
	pop		bc
	push	af
v_offsok_loop:
	ld		a, l
	DI_LOOP
	out		(P_VDP_1), a
	ld		a, h
	WRITE_NOP				// VDP timing
	out		(P_VDP_1),a
	WRITE_NOP				// VDP timing
	WRITE_NOP				// VDP timing
	in		a, (P_VDP_0)
	ex		af, af'			; '
	ld		a, e
	WRITE_NOP				// VDP timing
	out		(P_VDP_1), a
	ld		a, d
	or		#0x40
	out		(P_VDP_1), a
	ex		af, af'			;'
	WRITE_NOP				// VDP timing
	EI_LOOP
	out		(P_VDP_0), a
	inc		de
	cpi
	jp		pe, v_offsok_loop
	pop		af
	pop		hl
	jp		(iy)

// getbit:
	// ld		a, (hl)
	// inc		hl
	// rla
	// ret

// getbitexx:
	// exx
	// ld		a, (hl)
	// inc		hl
	// exx
	// rla
	// ret

// Depacker exit
v_Depack_out:
	pop		ix
	EI_LOOP
	ret

v_modes:
	.dw		v_offsok
	.dw		v_mode2
	.dw		v_mode3
	.dw		v_mode4
	.dw		v_mode5
	.dw		v_mode6
	.dw		v_mode7

__endasm;
}
