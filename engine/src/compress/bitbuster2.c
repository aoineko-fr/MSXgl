// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄  ▄▄  ▄▄             ▄▄           
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ▄  ██▀ ██▄  ██ █  ██▀ ██▀ ▄███ ██▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ██ ▀█▄ ██▄▀ ▀█▄█ ▄██  ▀█▄ ▀█▄▄ ██  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
// by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Bitbuster 2
//	Copyright (c) 2003-2004, 2022 Arjan Bakker
//	
//	Permission is hereby granted, free of charge, to any person obtaining a copy of 
//	this software and associated documentation files (the "Software"), to deal in 
//	the Software without restriction, including without limitation the rights to 
//	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of 
//	the Software, and to permit persons to whom the Software is furnished to do so, 
//	subject to the following conditions:
//	
//	The above copyright notice and this permission notice shall be included in all 
//	copies or substantial portions of the Software.
//	
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
//	FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
//	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
//	IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
//	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//─────────────────────────────────────────────────────────────────────────────
#include "bitbuster2.h"

// File: bitbuster
//
//	The bitbuster module gives you depack support for data packed
//	using the bitbuster algorithm.
//
// FUNCTION NAMES:
//	The functions <depack> and <depack_block> are MODULE local, which means you
//	have to add the prefix "bitbuster." to its name. This is to prevent possible
//	name clashes with functions from other libraries you might be using. 
//	However, if you define <MAKE_BITBUSTER_GLOBAL> then these functions will be
//	available without the "bitbuster." prefix as well.
//
// COPYRIGHT & CREDITS:
//	This module has been released by Arjan Bakker under the MIT License;
//	please see the top of the source file(s) for the full copyright statement.
	
// #define MAKE_BITBUSTER_GLOBAL
//	Defining this will make all public functions that are normally only
//	available with the "bitbuster." prefix to also be available without
//	this prefix. See the introduction of <bitbuster> for more information.
 
#define BITBUSTER_OPTIMIZE_SPEED
//	Defining this will optimize the bitbuster depacker for speed, at the cost of
//	bigger code.

//-----------------------------------------------------------------------------
// Unpack Bitbuster 2 compressed data to a RAM buffer
void Bitbuster2_UnpackToRAM(const void* source, void* dest)
{
	source;	// HL
	dest;	// DE

__asm
	xor		a // Start decompressing new data 

#ifdef BITBUSTER_OPTIMIZE_SPEED
// use macro's for getbit routines when optimizing bitbuster depacker for speed 
// (inlined code, no overhead by calls)

	// macro to get a bit from the bitstream
	// carry if bit is set, nocarry if bit is clear
	.macro	GET_BIT_FROM_BITSTREAM ?done
		add		a, a			// shift out new bit
		jp		nz, done		// if remaining value isn't zere, we're done

		ld		a, (hl)			// get 8 bits from bitstream
		inc		hl				// increase source data address

		rla						// (bit 0 will be set!!!!)
	done: 
	.endm

#endif // ifdef BITBUSTER_OPTIMIZE_SPEED


#ifndef	BITBUSTER_OPTIMIZE_SPEED
// use calls for getbit code when not optimizing	for speed


	// macro to get a bit from the bitstream
	// carry if bit is set, nocarry if bit is clear
	.macro	GET_BIT_FROM_BITSTREAM
		call	get_bit_from_bitstream
	.endm


	// get a bit from the bitstream
	// carry if bit is set, nocarry if bit is clear
	get_bit_from_bitstream:
		add		a, a		// shift out new bit
		ret		nz		// if remaining value isn't zere, we're done
		
		ld 		a, (hl)	// get 8 bits from bitstream
		inc 	hl		// increase source data address

		rla // (bit 0 will be set!!!!)
		ret

#endif // ifndef BITBUSTER_OPTIMIZE_SPEED

// FUNCTION:	depack
//	Depack a blob of data that was packed with Bitbuster.
//
// ENTRY:
//	HL - Address of packed data
//	DE - Address to depack to
// A - Start decompressing new data (0) or continue decompressing (<>0)
//
// EXIT:
//	HL - Size of depacked data
//	A - Number of blocks left to decompress
//	FZ - Last block has been decompressed
//	FNZ- Last block hasn't been decompressed
//
// MODIFIES:
//	#AF, BC, DE, HL#
//
depack::
	or		a
	jr		nz, depack_next_block	// continue depacking a block if more blocks left

	ld		a, (hl)					// get the number of blocks

	inc		hl						// move to size of first block

depack_next_block:
	inc		hl
	inc		hl						// move over block size

	push	af						// push number of blocks
	push	de						// push destination address to be able to calculate decompressed size later on

	call	depack_block			// decompress the block

	ex		de, hl					// DE -> pointer to next block, HL -> pointer to first byte after decompressed data
	pop		bc						// pop the original destination address
	or		a						// clear carry
	sbc		hl, bc					// calculate length of decompressed data
	ld		b, h
	ld		c, l					// put it in BC
	ex		de, hl					// HL -> pointer to next block

	pop		af						// pop number of blocks
	dec		a						// one less to decompress
	ret


// FUNCTION:	depack_block
//	Depack data that was packed with Bitbuster.
//	Decompresses the RAW data, i.e. the data that is stored after the block count and block size!
//
// ENTRY:
//	HL - Address of packed data
//	DE - Address to depack to
//
// EXIT:
//	HL - Address of first byte after compressed data
//	DE - Address of first byte after decompressed data
//
// MODIFIES:
//	#AF, BC, BC', DE, DE', HL, HL'#
//
depack_block::

	ld		a, #128

depack_loop:
	GET_BIT_FROM_BITSTREAM				// get compression type bit
	jp		c, output_compressed		// if set, we got lz77 compression
	ldi									// copy byte from compressed data to destination (literal byte)

#ifdef BITBUSTER_OPTIMIZE_SPEED
	GET_BIT_FROM_BITSTREAM			// get compression type bit
	jp		c, output_compressed	// if set, we got lz77 compression
	ldi								// copy byte from compressed data to destination (literal byte)
	GET_BIT_FROM_BITSTREAM			// get compression type bit
	jp		c, output_compressed	// if set, we got lz77 compression
	ldi								// copy byte from compressed data to destination (literal byte)
#endif

	jp		depack_loop


//handle compressed data
output_compressed:
// calculate length value
	ld		bc, #1
get_gamma_value:
	GET_BIT_FROM_BITSTREAM 			// get more bits
	jr 		nc, get_gamma_value_end
	GET_BIT_FROM_BITSTREAM 			// get next bit of value from bitstream
	rl		c
	rl		b
	jp 		nc, get_gamma_value 	// repeat unless overflow occurred (=end of block marker)
	ret

get_gamma_value_end:
	// ld 		iyh, b
	.db 0xFD, 0x60
	// ld 		iyl, c
	.db 0xFD, 0x69

	ld		c, (hl)					// get lowest 7 bits of offset, plus offset extension bit
	inc		hl						// to next byte in compressed data
	
	ld		b, #0
	bit		7, c
	jr		z, output_match1		// no need to get extra bits if carry not set
	
	GET_BIT_FROM_BITSTREAM			// get offset bit 10 
	rl		b
	GET_BIT_FROM_BITSTREAM			// get offset bit 9
	rl		b
	GET_BIT_FROM_BITSTREAM			// get offset bit 8
	rl		b
	GET_BIT_FROM_BITSTREAM			// get offset bit 7

	jp		c, output_match2		// since extension mark already makes bit 7 set
	res		7, c					// only clear it if the bit should be cleared
output_match1:
	scf
output_match2:
	push	hl						// address compressed data on stack

	ld		h, d
	ld		l, e					// destination address in HL...
	sbc		hl, bc					// calculate source address, keep the required CF in mind! (length was stored length-2)

	// ld 		b, iyh
	.db 0xFD, 0x44
	// ld 		c, iyl
	.db 0xFD, 0x4D 

	ldir							// transfer data
	ldi								// transfer last byte (length was stored length-2)

	pop		hl						// address compressed data back from stack

#ifdef BITBUSTER_OPTIMIZE_SPEED
	GET_BIT_FROM_BITSTREAM			// get compression type bit
	jp		c, output_compressed	// if set, we got lz77 compression
	ldi								// copy byte from compressed data to destination (literal byte)
	GET_BIT_FROM_BITSTREAM			// get compression type bit
	jp		c, output_compressed	// if set, we got lz77 compression
	ldi								// copy byte from compressed data to destination (literal byte)
#endif

	jp	depack_loop
__endasm;
}
