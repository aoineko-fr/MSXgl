// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄ ▄▄ ▄  ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ▄█▀ ▀█▄▀ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▄ ██ █ ▀▄█▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// ZX0 decoder by Einar Saukas, Urusergi & introspec
//  "Standard" version (68 bytes only)
//  https://github.com/einar-saukas/ZX0/blob/main/z80/dzx0_standard.asm
//─────────────────────────────────────────────────────────────────────────────
#include "zx0.h"

#if (ZX0_MODE == ZX0_MODE_STANDARD)

//-----------------------------------------------------------------------------
// Unpack ZX0 compressed data to a RAM buffer
// "Standard" version (68 bytes only)
void ZX0_UnpackToRAM(const void* source, void* dest)
{
	source;	// HL
	dest;	// DE

__asm
// -----------------------------------------------------------------------------
// ZX0 decoder by Einar Saukas & Urusergi
// "Standard" version (68 bytes only)
// -----------------------------------------------------------------------------
// Parameters:
//   HL: source address (compressed data)
//   DE: destination address (decompressing)
// -----------------------------------------------------------------------------
dzx0_standard:
	ld      bc, #0xFFFF             // preserve default offset 1
	push    bc
	inc     bc
	ld      a, #0x80
dzx0s_literals:
	call    dzx0s_elias             // obtain length
	ldir                            // copy literals
	add     a, a                    // copy from last offset or new offset?
	jr      c, dzx0s_new_offset
	call    dzx0s_elias             // obtain length
dzx0s_copy:
	ex      (sp), hl                // preserve source, restore offset
	push    hl                      // preserve offset
	add     hl, de                  // calculate destination - offset
	ldir                            // copy from offset
	pop     hl                      // restore offset
	ex      (sp), hl                // preserve offset, restore source
	add     a, a                    // copy from literals or new offset?
	jr      nc, dzx0s_literals
dzx0s_new_offset:
	pop     bc                      // discard last offset
	ld      c, #0xFE                // prepare negative offset
	call    dzx0s_elias_loop        // obtain offset MSB
	inc     c
	ret     z                       // check end marker
	ld      b, c
	ld      c, (hl)                 // obtain offset LSB
	inc     hl
	rr      b                       // last offset bit becomes first length bit
	rr      c
	push    bc                      // preserve new offset
	ld      bc, #1                  // obtain length
	call    nc, dzx0s_elias_backtrack
	inc     bc
	jr      dzx0s_copy
dzx0s_elias:
	inc     c                       // interlaced Elias gamma coding
dzx0s_elias_loop:
	add     a, a
	jr      nz, dzx0s_elias_skip
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	rla
dzx0s_elias_skip:
	ret     c
dzx0s_elias_backtrack:
	add     a, a
	rl      c
	rl      b
	jr      dzx0s_elias_loop
// -----------------------------------------------------------------------------
__endasm;
}

#elif (ZX0_MODE == ZX0_MODE_TURBO)

//-----------------------------------------------------------------------------
// Unpack ZX0 compressed data to a RAM buffer
// "Turbo" version (126 bytes, 21% faster)
void ZX0_UnpackToRAM(const void* source, void* dest)
{
	source;	// HL
	dest;	// DE

__asm
// -----------------------------------------------------------------------------
// ZX0 decoder by Einar Saukas & introspec
// "Turbo" version (126 bytes, 21% faster)
// -----------------------------------------------------------------------------
// Parameters:
//   HL: source address (compressed data)
//   DE: destination address (decompressing)
// -----------------------------------------------------------------------------
dzx0_turbo:
	ld      bc, #0xFFFF             // preserve default offset 1
	ld      (dzx0t_last_offset+1), bc
	inc     bc
	ld      a, #0x80
	jr      dzx0t_literals
dzx0t_new_offset:
	ld      c, #0xFE                // prepare negative offset
	add     a, a
	jp      nz, dzx0t_new_offset_skip
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	rla
dzx0t_new_offset_skip:
	call    nc, dzx0t_elias         // obtain offset MSB
	inc     c
	ret     z                       // check end marker
	ld      b, c
	ld      c, (hl)                 // obtain offset LSB
	inc     hl
	rr      b                       // last offset bit becomes first length bit
	rr      c
	ld      (dzx0t_last_offset+1), bc // preserve new offset
	ld      bc, #1                  // obtain length
	call    nc, dzx0t_elias
	inc     bc
dzx0t_copy:
	push    hl                      // preserve source
dzx0t_last_offset:
	ld      hl, #0                  // restore offset
	add     hl, de                  // calculate destination - offset
	ldir                            // copy from offset
	pop     hl                      // restore source
	add     a, a                    // copy from literals or new offset?
	jr      c, dzx0t_new_offset
dzx0t_literals:
	inc     c                       // obtain length
	add     a, a
	jp      nz, dzx0t_literals_skip
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	rla
dzx0t_literals_skip:
	call    nc, dzx0t_elias
	ldir                            // copy literals
	add     a, a                    // copy from last offset or new offset?
	jr      c, dzx0t_new_offset
	inc     c                       // obtain length
	add     a, a
	jp      nz, dzx0t_last_offset_skip
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	rla
dzx0t_last_offset_skip:
	call    nc, dzx0t_elias
	jp      dzx0t_copy
dzx0t_elias:
	add     a, a                    // interlaced Elias gamma coding
	rl      c
	add     a, a
	jr      nc, dzx0t_elias
	ret     nz
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	rla
	ret     c
	add     a, a
	rl      c
	add     a, a
	ret     c
	add     a, a
	rl      c
	add     a, a
	ret     c
	add     a, a
	rl      c
	add     a, a
	ret     c
dzx0t_elias_loop:
	add     a, a
	rl      c
	rl      b
	add     a, a
	jr      nc, dzx0t_elias_loop
	ret     nz
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	rla
	jr      nc, dzx0t_elias_loop
	ret
// -----------------------------------------------------------------------------
__endasm;
}

#elif (ZX0_MODE == ZX0_MODE_FAST)

//-----------------------------------------------------------------------------
// Unpack ZX0 compressed data to a RAM buffer
// Speed-optimized ZX0 decompressor by spke (187 bytes)
void ZX0_UnpackToRAM(const void* source, void* dest)
{
	source;	// HL
	dest;	// DE

__asm
//
//  Speed-optimized ZX0 decompressor by spke (187 bytes)
//
//  ver.00 by spke (27/01-23/03/2021, 191 bytes)
//  ver.01 by spke (24/03/2021, 193(+2) bytes - fixed a bug in the initialization)
//  ver.01patch2 by uniabis (25/03/2021, 191(-2) bytes - fixed a bug with elias over 8bits)
//  ver.01patch9 by uniabis (10/09/2021, 187(-4) bytes - support for new v2 format)
//
//  Original ZX0 decompressors were written by Einar Saukas
//
//  This decompressor was written on the basis of "Standard" decompressor by
//  Einar Saukas and optimized for speed by spke. This decompressor is
//  about 5% faster than the "Turbo" decompressor, which is 128 bytes long.
//  It has about the same speed as the 412 bytes version of the "Mega" decompressor.
//
//  The decompressor uses AF, BC, DE, HL and IX and relies upon self-modified code.
//
//  The decompression is done in the standard way:
//
//  ld hl,FirstByteOfCompressedData
//  ld de,FirstByteOfMemoryForDecompressedData
//  call DecompressZX0
//
//  Of course, ZX0 compression algorithms are (c) 2021 Einar Saukas,
//  see https://github.com/einar-saukas/ZX0 for more information
//
//  Drop me an email if you have any comments/ideas/suggestions: zxintrospec@gmail.com
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.

DecompressZX0:

	ld      iy, #CopyMatch1
	ld      bc, #0xFFFF
	ld      (PrevOffset+1), bc      // default offset is -1
	inc     bc
	ld      a, #0x80
	jr      RunOfLiterals           // BC is assumed to contains 0 most of the time

ShorterOffsets:
	ld      b, #0xFF                // the top byte of the offset is always 0xFF
	ld      c, (hl)
	inc     hl
	rr      c
	ld      (PrevOffset+1), bc
	jr      nc, LongerMatch

CopyMatch2:                             // the case of matches with len=2
	ld      bc, #2

	// the faster match copying code
CopyMatch1:
	push    hl                      // preserve source

PrevOffset:
	ld      hl, #0xFFFF             // restore offset (default offset is -1)
	add     hl, de                  // HL = dest - offset
	ldir
	pop     hl                      // restore source

	// after a match you can have either
	// 0 + <elias length> = run of literals, or
	// 1 + <elias offset msb> + [7-bits of offset lsb + 1-bit of length] + <elias length> = another match
AfterMatch1:
	add     a, a
	jr      nc, RunOfLiterals

UsualMatch:                             // this is the case of usual match+offset
	add     a, a
	jr      nc, LongerOffets
	jr      nz, ShorterOffsets      // NZ after NC == "confirmed C"
	
	ld      a, (hl)                 // reload bits
	inc     hl
	rla

	jr      c, ShorterOffsets

LongerOffets:
	ld      c, #0xFE

	add     a, a                    // inline read gamma
	rl      c
	add     a, a
	jr      nc, #.-4

	call    z, ReloadReadGamma

ProcessOffset:

	inc     c
	ret     z                       // end-of-data marker (only checked for longer offsets)
	rr      c
	ld      b, c
	ld      c, (hl)
	inc     hl
	rr      c
	ld      (PrevOffset+1), bc

	// lowest bit is the first bit of the gamma code for length
	jr      c, CopyMatch2

LongerMatch:
	ld      bc, #1

	add     a, a                    // inline read gamma
	rl      c
	add     a, a
	jr      nc, #.-4

	call    z, ReloadReadGamma

CopyMatch3:
	push    hl                      // preserve source
	ld      hl, (PrevOffset+1)      // restore offset
	add     hl, de                  // HL = dest - offset

	// because BC>=3-1, we can do 2 x LDI safely
	ldi
	ldir
	inc     c
	ldi
	pop     hl                      // restore source

	// after a match you can have either
	// 0 + <elias length> = run of literals, or
	// 1 + <elias offset msb> + [7-bits of offset lsb + 1-bit of length] + <elias length> = another match
AfterMatch3:
	add     a, a
	jr      c, UsualMatch

RunOfLiterals:
	inc     c
	add     a, a
	jr      nc, LongerRun
	jr      nz, CopyLiteral         // NZ after NC == "confirmed C"
	
	ld      a, (hl)                 // reload bits
	inc     hl
	rla

	jr      c, CopyLiteral

LongerRun:
	add     a, a                    // inline read gamma
	rl      c
	add     a, a
	jr      nc, #.-4

	jr      nz, CopyLiterals
	
	ld      a, (hl)                 // reload bits
	inc     hl
	rla

	call    nc, ReadGammaAligned

CopyLiterals:
	ldi

CopyLiteral:
	ldir

	// after a literal run you can have either
	// 0 + <elias length> = match using a repeated offset, or
	// 1 + <elias offset msb> + [7-bits of offset lsb + 1-bit of length] + <elias length> = another match
	add     a, a
	jr      c, UsualMatch

RepMatch:
	inc     c
	add     a, a
	jr      nc, LongerRepMatch
	jr      nz, CopyMatch1          // NZ after NC == "confirmed C"

	ld      a, (hl)                 // reload bits
	inc     hl
	rla

	jr      c, CopyMatch1

LongerRepMatch:
	add     a, a                    // inline read gamma
	rl      c
	add     a, a
	jr      nc, #.-4

	jp      nz, CopyMatch1

	// this is a crafty equivalent of CALL ReloadReadGamma : JP CopyMatch1
	push    iy

	//  the subroutine for reading the remainder of the partly read Elias gamma code.
	//  it has two entry points: ReloadReadGamma first refills the bit reservoir in A,
	//  while ReadGammaAligned assumes that the bit reservoir has just been refilled.
ReloadReadGamma:
	ld      a, (hl)                 // reload bits
	inc     hl
	rla

	ret     c
ReadGammaAligned:
	add     a, a
	rl      c
	add     a, a
	ret     c
	add     a, a
	rl      c
	add     a, a
ReadingLongGamma:                       // this loop does not need unrolling, as it does not get much use anyway
	ret     c
	add     a, a
	rl      c
	rl      b
	add     a, a
	jr      nz, ReadingLongGamma

	ld      a, (hl)                 // reload bits
	inc     hl
	rla
	jr      ReadingLongGamma
__endasm;
}

#elif (ZX0_MODE == ZX0_MODE_MEGA)

//-----------------------------------------------------------------------------
// Unpack ZX0 compressed data to a RAM buffer
void ZX0_UnpackToRAM(const void* source, void* dest)
{
	source;	// HL
	dest;	// DE

__asm
// -----------------------------------------------------------------------------
// ZX0 decoder by Einar Saukas
// "Mega" version (673 bytes, 28% faster)
// -----------------------------------------------------------------------------
// Parameters:
//   HL: source address (compressed data)
//   DE: destination address (decompressing)
// -----------------------------------------------------------------------------
dzx0_mega:
	ld      bc, #0xFFFF               // preserve default offset 1
	ld      (dzx0m_last_offset+1), bc
	inc     bc
	jr      dzx0m_literals0

dzx0m_new_offset6:
	ld      c, #0xFE                  // prepare negative offset
	add     a, a                    // obtain offset MSB
	jp      c, dzx0m_new_offset5
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_new_offset3
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_new_offset1
dzx0m_elias_offset1:
	add     a, a
	rl      c
	rl      b
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	add     a, a
	jp      nc, dzx0m_elias_offset7
dzx0m_new_offset7:
	inc     c
	ret     z                       // check end marker
	ld      b, c
	ld      c, (hl)                 // obtain offset LSB
	inc     hl
	rr      b                       // last offset bit becomes first length bit
	rr      c
	ld      (dzx0m_last_offset+1), bc // preserve new offset
	ld      bc, #1
	jp      c, dzx0m_length7        // obtain length
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_length5
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_length3
dzx0m_elias_length3:
	add     a, a
	rl      c
	rl      b
	add     a, a
	jp      nc, dzx0m_elias_length1
dzx0m_length1:
	push    hl                      // preserve source
	ld      hl, (dzx0m_last_offset+1)
	add     hl, de                  // calculate destination - offset
	ldir                            // copy from offset
	inc     c
	ldi                             // copy one more from offset
	pop     hl                      // restore source
	add     a, a                    // copy from literals or new offset?
	jr      c, dzx0m_new_offset0
dzx0m_literals0:
	inc     c
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	add     a, a                    // obtain length
	jp      c, dzx0m_literals7
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_literals5
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_literals3
dzx0m_elias_literals3:
	add     a, a
	rl      c
	rl      b
	add     a, a
	jp      nc, dzx0m_elias_literals1
dzx0m_literals1:
	ldir                            // copy literals
	add     a, a                    // copy from last offset or new offset?
	jr      c, dzx0m_new_offset0
	inc     c
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	add     a, a                    // obtain length
	jp      c, dzx0m_reuse7
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_reuse5
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_reuse3
dzx0m_elias_reuse3:
	add     a, a
	rl      c
	rl      b
	add     a, a
	jp      nc, dzx0m_elias_reuse1
dzx0m_reuse1:
	push    hl                      // preserve source
	ld      hl, (dzx0m_last_offset+1)
	add     hl, de                  // calculate destination - offset
	ldir                            // copy from offset
	pop     hl                      // restore source
	add     a, a                    // copy from literals or new offset?
	jr      nc, dzx0m_literals0

dzx0m_new_offset0:
	ld      c, #0xFE                  // prepare negative offset
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	add     a, a                    // obtain offset MSB
	jp      c, dzx0m_new_offset7
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_new_offset5
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_new_offset3
dzx0m_elias_offset3:
	add     a, a
	rl      c
	rl      b
	add     a, a
	jp      nc, dzx0m_elias_offset1
dzx0m_new_offset1:
	inc     c
	ret     z                       // check end marker
	ld      b, c
	ld      c, (hl)                 // obtain offset LSB
	inc     hl
	rr      b                       // last offset bit becomes first length bit
	rr      c
	ld      (dzx0m_last_offset+1), bc // preserve new offset
	ld      bc, #1
	jp      c, dzx0m_length1        // obtain length
	add     a, a
	rl      c
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	add     a, a
	jp      c, dzx0m_length7
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_length5
dzx0m_elias_length5:
	add     a, a
	rl      c
	rl      b
	add     a, a
	jp      nc, dzx0m_elias_length3
dzx0m_length3:
	push    hl                      // preserve source
	ld      hl, (dzx0m_last_offset+1)
	add     hl, de                  // calculate destination - offset
	ldir                            // copy from offset
	inc     c
	ldi                             // copy one more from offset
	pop     hl                      // restore source
	add     a, a                    // copy from literals or new offset?
	jr      c, dzx0m_new_offset2
dzx0m_literals2:
	inc     c
	add     a, a                    // obtain length
	jp      c, dzx0m_literals1
	add     a, a
	rl      c
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	add     a, a
	jp      c, dzx0m_literals7
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_literals5
dzx0m_elias_literals5:
	add     a, a
	rl      c
	rl      b
	add     a, a
	jp      nc, dzx0m_elias_literals3
dzx0m_literals3:
	ldir                            // copy literals
	add     a, a                    // copy from last offset or new offset?
	jr      c, dzx0m_new_offset2
	inc     c
	add     a, a                    // obtain length
	jp      c, dzx0m_reuse1
	add     a, a
	rl      c
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	add     a, a
	jp      c, dzx0m_reuse7
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_reuse5
dzx0m_elias_reuse5:
	add     a, a
	rl      c
	rl      b
	add     a, a
	jp      nc, dzx0m_elias_reuse3
dzx0m_reuse3:
	push    hl                      // preserve source
	ld      hl, (dzx0m_last_offset+1)
	add     hl, de                  // calculate destination - offset
	ldir                            // copy from offset
	pop     hl                      // restore source
	add     a, a                    // copy from literals or new offset?
	jr      nc, dzx0m_literals2

dzx0m_new_offset2:
	ld      c, #0xFE                  // prepare negative offset
	add     a, a                    // obtain offset MSB
	jp      c, dzx0m_new_offset1
	add     a, a
	rl      c
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	add     a, a
	jp      c, dzx0m_new_offset7
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_new_offset5
dzx0m_elias_offset5:
	add     a, a
	rl      c
	rl      b
	add     a, a
	jp      nc, dzx0m_elias_offset3
dzx0m_new_offset3:
	inc     c
	ret     z                       // check end marker
	ld      b, c
	ld      c, (hl)                 // obtain offset LSB
	inc     hl
	rr      b                       // last offset bit becomes first length bit
	rr      c
	ld      (dzx0m_last_offset+1), bc // preserve new offset
	ld      bc, #1
	jp      c, dzx0m_length3        // obtain length
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_length1
	add     a, a
	rl      c
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	add     a, a
	jp      c, dzx0m_length7
dzx0m_elias_length7:
	add     a, a
	rl      c
	rl      b
	add     a, a
	jp      nc, dzx0m_elias_length5
dzx0m_length5:
	push    hl                      // preserve source
	ld      hl, (dzx0m_last_offset+1)
	add     hl, de                  // calculate destination - offset
	ldir                            // copy from offset
	inc     c
	ldi                             // copy one more from offset
	pop     hl                      // restore source
	add     a, a                    // copy from literals or new offset?
	jr      c, dzx0m_new_offset4
dzx0m_literals4:
	inc     c
	add     a, a                    // obtain length
	jp      c, dzx0m_literals3
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_literals1
	add     a, a
	rl      c
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	add     a, a
	jp      c, dzx0m_literals7
dzx0m_elias_literals7:
	add     a, a
	rl      c
	rl      b
	add     a, a
	jp      nc, dzx0m_elias_literals5
dzx0m_literals5:
	ldir                            // copy literals
	add     a, a                    // copy from last offset or new offset?
	jr      c, dzx0m_new_offset4
	inc     c
	add     a, a                    // obtain length
	jp      c, dzx0m_reuse3
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_reuse1
	add     a, a
	rl      c
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	add     a, a
	jp      c, dzx0m_reuse7
dzx0m_elias_reuse7:
	add     a, a
	rl      c
	rl      b
	add     a, a
	jp      nc, dzx0m_elias_reuse5
dzx0m_reuse5:
	push    hl                      // preserve source
	ld      hl, (dzx0m_last_offset+1)
	add     hl, de                  // calculate destination - offset
	ldir                            // copy from offset
	pop     hl                      // restore source
	add     a, a                    // copy from literals or new offset?
	jr      nc, dzx0m_literals4

dzx0m_new_offset4:
	ld      c, #0xFE                  // prepare negative offset
	add     a, a                    // obtain offset MSB
	jp      c, dzx0m_new_offset3
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_new_offset1
	add     a, a
	rl      c
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	add     a, a
	jp      c, dzx0m_new_offset7
dzx0m_elias_offset7:
	add     a, a
	rl      c
	rl      b
	add     a, a
	jp      nc, dzx0m_elias_offset5
dzx0m_new_offset5:
	inc     c
	ret     z                       // check end marker
	ld      b, c
	ld      c, (hl)                 // obtain offset LSB
	inc     hl
	rr      b                       // last offset bit becomes first length bit
	rr      c
	ld      (dzx0m_last_offset+1), bc // preserve new offset
	ld      bc, #1
	jp      c, dzx0m_length5        // obtain length
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_length3
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_length1
dzx0m_elias_length1:
	add     a, a
	rl      c
	rl      b
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	add     a, a
	jp      nc, dzx0m_elias_length7
dzx0m_length7:
	push    hl                      // preserve source
	ld      hl, (dzx0m_last_offset+1)
	add     hl, de                  // calculate destination - offset
	ldir                            // copy from offset
	inc     c
	ldi                             // copy one more from offset
	pop     hl                      // restore source
	add     a, a                    // copy from literals or new offset?
	jp      c, dzx0m_new_offset6
dzx0m_literals6:
	inc     c
	add     a, a                    // obtain length
	jp      c, dzx0m_literals5
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_literals3
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_literals1
dzx0m_elias_literals1:
	add     a, a
	rl      c
	rl      b
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	add     a, a
	jp      nc, dzx0m_elias_literals7
dzx0m_literals7:
	ldir                            // copy literals
	add     a, a                    // copy from last offset or new offset?
	jp      c, dzx0m_new_offset6
	inc     c
	add     a, a                    // obtain length
	jp      c, dzx0m_reuse5
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_reuse3
	add     a, a
	rl      c
	add     a, a
	jp      c, dzx0m_reuse1
dzx0m_elias_reuse1:
	add     a, a
	rl      c
	rl      b
	ld      a, (hl)                 // load another group of 8 bits
	inc     hl
	add     a, a
	jp      nc, dzx0m_elias_reuse7
dzx0m_reuse7:
	push    hl                      // preserve source
dzx0m_last_offset:
	ld      hl, #0
	add     hl, de                  // calculate destination - offset
	ldir                            // copy from offset
	pop     hl                      // restore source
	add     a, a                    // copy from literals or new offset?
	jr      nc, dzx0m_literals6

	jp      dzx0m_new_offset6
// -----------------------------------------------------------------------------
__endasm;
}

#endif