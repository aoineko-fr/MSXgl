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
#include "vdp.h"

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
#if (PLETTER_WRITE_MODE == PLETTER_WRITE_SAFE) // Safe VRAM write speed (29 t-states)
	#define PLETTER_WRITE	PLETTER_WRITE_30CC

#elif (PLETTER_WRITE_MODE == PLETTER_WRITE_NODISPLAY) // Safe VRAM write speed when screen display disable (20 t-states)
	#define PLETTER_WRITE	PLETTER_WRITE_22CC

#elif (PLETTER_WRITE_MODE == PLETTER_WRITE_MINIMAL) // No wait beetween write (17 t-states)
	#define PLETTER_WRITE	PLETTER_WRITE_17CC

#elif (PLETTER_WRITE_MODE == PLETTER_WRITE_QUICK) // No wait beetween write (12 t-states)
	#define PLETTER_WRITE	PLETTER_WRITE_12CC

#elif (PLETTER_WRITE_MODE == PLETTER_WRITE_AUTO) // Determine the worst case according to selected screen mode (12~29 t-states)

	#if (VDP_SAFE_ACCESS == VDP_ACCESS_29CC) // 29 CC => 30 CC
		#define PLETTER_WRITE	PLETTER_WRITE_30CC
	#elif (VDP_SAFE_ACCESS == VDP_ACCESS_20CC) // 20 CC => 22 CC
		#define PLETTER_WRITE	PLETTER_WRITE_22CC
	#elif (VDP_SAFE_ACCESS == VDP_ACCESS_15CC) // 15 CC => 17 CC
		#define PLETTER_WRITE	PLETTER_WRITE_17CC
	#elif (VDP_SAFE_ACCESS == VDP_ACCESS_12CC) // 12 CC => 12 CC
		#define PLETTER_WRITE	PLETTER_WRITE_12CC
	#endif

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

#if ((VDP_USE_VALIDATOR) && !(MSX_VERSION & MSX_1))
	// Reset VRAM address bit 14 to 16 (in R#14)
	xor		a
	out		(P_VDP_REG), a
	ld		a, #VDP_REG(14)
	out		(P_VDP_REG), a
#endif

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

.macro PLETTER_WRITE_12CC
	DI_LOOP
	out		(P_VDP_1), a	// 12 cc ~~~~~~~~~~
	ld		a, h			//  5 cc
	out		(P_VDP_1), a	// 12 cc ~~~~~~~~~~ 17
	in		a, (P_VDP_0)	// 12 cc ~~~~~~~~~~ 12
	ex		af, af' ;'		//  5 cc
	ld		a, e			//  5 cc
	out		(P_VDP_1), a	// 12 cc ~~~~~~~~~~ 22
	ld		a, d			//  5 cc
	or		#0x40			//  8 cc
	out		(P_VDP_1), a	// 12 cc ~~~~~~~~~~ 25
	ex		af, af' ;'		//  5 cc
	EI_LOOP
	out		(P_VDP_0), a	// 12 cc ~~~~~~~~~~ 17/22
.endm

.macro PLETTER_WRITE_17CC
	DI_LOOP
	out		(P_VDP_1), a	// 12 cc ~~~~~~~~~~
	ld		a, h			//  5 cc
	out		(P_VDP_1), a	// 12 cc ~~~~~~~~~~ 17
	nop						//  5 cc
	in		a, (P_VDP_0)	// 12 cc ~~~~~~~~~~ 17
	ex		af, af' ;'		//  5 cc
	ld		a, e			//  5 cc
	out		(P_VDP_1), a	// 12 cc ~~~~~~~~~~ 22
	ld		a, d			//  5 cc
	or		#0x40			//  8 cc
	out		(P_VDP_1), a	// 12 cc ~~~~~~~~~~ 25
	ex		af, af' ;'		//  5 cc
	EI_LOOP
	out		(P_VDP_0), a	// 12 cc ~~~~~~~~~~ 17/22
.endm

.macro PLETTER_WRITE_22CC
	DI_LOOP
	out		(P_VDP_1), a	// 12 cc ~~~~~~~~~~
	ld		a, h			//  5 cc
	nop						//  5 cc
	out		(P_VDP_1), a	// 12 cc ~~~~~~~~~~ 22
	nop						//  5 cc
	nop						//  5 cc
	in		a, (P_VDP_0)	// 12 cc ~~~~~~~~~~ 22
	ex		af, af' ;'		//  5 cc
	ld		a, e			//  5 cc
	out		(P_VDP_1), a	// 12 cc ~~~~~~~~~~ 22
	ld		a, d			//  5 cc
	or		#0x40			//  8 cc
	out		(P_VDP_1), a	// 12 cc ~~~~~~~~~~ 25
	ex		af, af' ;'		//  5 cc
	nop						//  5 cc
	EI_LOOP
	out		(P_VDP_0), a	// 12 cc ~~~~~~~~~~ 22/27
.endm

.macro PLETTER_WRITE_30CC
	DI_LOOP
	out		(P_VDP_1), a	// 12 cc ~~~~~~~~~~
	ld		a, h			//  5 cc
	nop						//  5 cc
	or		#0				//  8 cc
	out		(P_VDP_1), a	// 12 cc ~~~~~~~~~~ 30
	nop						//  5 cc
	nop						//  5 cc
	or		#0				//  8 cc
	in		a, (P_VDP_0)	// 12 cc ~~~~~~~~~~ 30
	ex		af, af' ;'		//  5 cc
	ld		a, e			//  5 cc
	or		#0				//  8 cc
	out		(P_VDP_1), a	// 12 cc ~~~~~~~~~~ 30
	ld		a, d			//  5 cc
	nop						//  5 cc
	or		#0x40			//  8 cc
	out		(P_VDP_1), a	// 12 cc ~~~~~~~~~~ 30
	ex		af, af' ;'		//  5 cc
	nop						//  5 cc
	or		#0				//  8 cc
	EI_LOOP
	out		(P_VDP_0), a	// 12 cc ~~~~~~~~~~ 30/35
.endm

	PLETTER_WRITE

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
	EI_FULL
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
