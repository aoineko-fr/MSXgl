// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄  ▄▄  ▄▄             ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ▄  ██▀ ██▄  ██ █  ██▀ ██▀ ▄███ ██▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ██ ▀█▄ ██▄▀ ▀█▄█ ▄██  ▀█▄ ▀█▄▄ ██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Bitbuster v1.2 - (c) 2002-2003  Team Bomba
//─────────────────────────────────────────────────────────────────────────────
#include "bitbuster.h"
#include "system_port.h"

//-----------------------------------------------------------------------------
// Unpack Bitbuster compressed data to a RAM buffer
void Bitbuster_UnpackToRAM(const void* source, void* dest)
{
	source;	// HL
	dest;	// DE

__asm
// Macro to get a bit from the bitstream
// Carry if bit is set, nocarry if bit is clear
// Must be entered with second registerset switched in!
.macro GET_BIT_FROM_BITSTREAM ?YM
	add		a, a					// Shift out new bit
	jp		nz, YM 					// If remaining value isn't zere, we're done

	ld		a, (hl)					// Get 8 bits from bitstream
	inc		hl						// Increase source data address

	rla								// (bit 0 will be set!!!!)
YM:
.endm

// In: HL: source
//     DE: destination
depack:
	inc		hl						// Skip original file length
	inc		hl						// Which is stored in 4 bytes
	inc		hl
	inc		hl

	ld		a, #128

	exx
	ld		de, #1
	exx
	
depack_loop:
	GET_BIT_FROM_BITSTREAM			// Get compression type bit
	jp		c, output_compressed	// If set, we got lz77 compression
	ldi								// Copy byte from compressed data to destination (literal byte)
// unrolled for extra speed
	GET_BIT_FROM_BITSTREAM			// Get compression type bit
	jp		c, output_compressed	// If set, we got lz77 compression
	ldi								// Copy byte from compressed data to destination (literal byte)
	GET_BIT_FROM_BITSTREAM			// Get compression type bit
	jp		c, output_compressed	// If set, we got lz77 compression
	ldi								// Copy byte from compressed data to destination (literal byte)
	jp		depack_loop
	

// handle compressed data
output_compressed:
	ld		c, (hl)					// Get lowest 7 bits of offset, plus offset extension bit
	inc		hl						// To next byte in compressed data

output_match:
	ld		b, #0
	bit		7, c
	jr		z, output_match1		// No need to get extra bits if carry not set

	GET_BIT_FROM_BITSTREAM			// Get offset bit 10 
	rl		b
	GET_BIT_FROM_BITSTREAM			// Get offset bit 9
	rl		b
	GET_BIT_FROM_BITSTREAM			// Get offset bit 8
	rl		b
	GET_BIT_FROM_BITSTREAM			// Get offset bit 7

	jp		c, output_match1		// Since extension mark already makes bit 7 set 
	res		7, c					// Only clear it if the bit should be cleared
output_match1:
	inc		bc

// return a gamma-encoded value
// length returned in HL
	exx								// To second register set!
	ld		h, d
	ld		l, e					// Initial length to 1
	ld		b, e					// Bitcount to 1
	
// determine number of bits used to encode value
get_gamma_value_size:
	exx
	GET_BIT_FROM_BITSTREAM			// Get more bits
	exx
	jr		nc, get_gamma_value_size_end // If bit not set, bitlength of remaining is known
	inc		b						// Increase bitcount
	jp		get_gamma_value_size	// Repeat...

get_gamma_value_bits:
	exx
	GET_BIT_FROM_BITSTREAM			// Get next bit of value from bitstream
	exx
	
	adc		hl, hl					// Insert new bit in HL
get_gamma_value_size_end:
	djnz	get_gamma_value_bits	// Repeat if more bits to go

get_gamma_value_end:
	inc		hl						// Length was stored as length-2 so correct this
	exx								// Back to normal register set
	
	ret	c
// HL' = length

	push	hl						// Address compressed data on stack

	exx
	push	hl						// Match length on stack
	exx

	ld		h, d
	ld		l, e					// Destination address in HL...
	sbc		hl, bc					// Calculate source address

	pop		bc						// Match length from stack

	ldir							// Transfer data

	pop		hl						// Address compressed data back from stack

	GET_BIT_FROM_BITSTREAM			// Get compression type bit
	jp		c, output_compressed	// If set, we got lz77 compression
	ldi								// Copy byte from compressed data to destination (literal byte)
	GET_BIT_FROM_BITSTREAM			// Get compression type bit
	jp		c, output_compressed	// If set, we got lz77 compression
	ldi								// Copy byte from compressed data to destination (literal byte)

	jp		depack_loop
__endasm;
}

//-----------------------------------------------------------------------------
// Unpack Bitbuster compressed data directly to VRAM
// Only first 16 KB of VRAM can be use as destination (0000-3FFF)
void Bitbuster_UnpackToVRAM(const void* source, u16 dest)
{
	source;	// HL
	dest;	// DE

__asm
//-----------------------------------------------------------
// BitBuster v1.2 VRAM Depacker v1.1 - 16Kb version
//  by  Metalion (22-02-2008, 16:54)
//  https://msx.org/forum/development/msx-development/bitbuster-depack-vram?page=7
// HL = RAM/ROM source
// DE = VRAM destination
//-----------------------------------------------------------
	di

// VRAM address setup
	ld		a, e
	out		(P_VDP_1), a
	ld		a, d
	or		#0x40
	out		(P_VDP_1), a

// Skips 4 bytes data header
	inc		hl
	inc		hl
	inc		hl
	inc		hl

// Initialization
	ld		a, #128
	exx
	ld		de, #1
	exx

// Main depack loop
v_Depack_loop:
	add		a, a
	jp		nz, nxt0
	ld		a, (hl)
	inc		hl
	rla
nxt0:
	jp		c, v_Compressed
	ld		c, #P_VDP_0
	outi
	inc		de
	jp		v_Depack_loop

// Compressed data
v_Compressed:
	ld		c, (hl)
	inc		hl
v_Match:
	ld		b, #0
	bit		7, c
	jr		z, v_Match1
	add		a, a
	jp		nz, nxt1
	ld		a, (hl)
	inc		hl
	rla
nxt1:
	rl		b
	add		a, a
	jp		nz, nxt2
	ld		a, (hl)
	inc		hl
	rla
nxt2:
	rl		b
	add		a, a
	jp		nz, nxt3
	ld		a, (hl)
	inc		hl
	rla
nxt3:
	rl		b
	add		a, a
	jp		nz, nxt4
	ld		a, (hl)
	inc		hl
	rla
nxt4:
	jp		c, v_Match1
	res		7, c
v_Match1:
	inc		bc
	exx
	ld		h, d
	ld		l, e
	ld		b, e
v_Gamma_size:
	exx
	add		a, a
	jp		nz, nxt5
	ld		a, (hl)
	inc		hl
	rla
nxt5:
	exx
	jp		nc, v_Gamma_size_end
	inc		b
	jp		v_Gamma_size
v_Gamma_bits:
	exx
	add		a, a
	jp		nz, nxt6
	ld		a, (hl)
	inc		hl
	rla
nxt6:
	exx
	adc		hl, hl
v_Gamma_size_end:
	djnz	v_Gamma_bits
v_Gamma_end:
	inc		hl
	exx
	jp		c, v_Depack_out
	push	hl
	exx
	push	hl
	exx
	ld		h, d
	ld		l, e
	sbc		hl, bc
	pop		bc
	push	af
loop:
	ld		a, l
	out		(P_VDP_1), a
	ld		a, h
	nop								// VDP timing
	out		(P_VDP_1), a
	nop								// VDP timing
	in		a, (P_VDP_0)
	ex		af, af'					;'
	ld		a, e
	nop								// VDP timing
	out		(P_VDP_1), a
	ld		a, d
	or		#0x40
	out		(P_VDP_1), a
	ex		af, af'					;'
	nop 							// VDP timing
	out		(P_VDP_0), a
	inc		de
	cpi
	jp		pe, loop
	pop		af
	pop		hl
	jp		v_Depack_loop

// Depacker exit
v_Depack_out:
	ei
	ret
__endasm;

}
