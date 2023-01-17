// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄ ▄▄ ▄  ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ▄█▀ ▀█▄▀ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▄ ██ █ ▀▄█▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// ZX0 decoder by Einar Saukas & Urusergi
//  "Standard" version (68 bytes only)
//  https://github.com/einar-saukas/ZX0/blob/main/z80/dzx0_standard.asm
//─────────────────────────────────────────────────────────────────────────────
#include "zx0.h"

//-----------------------------------------------------------------------------
// Unpack ZX0 compressed data to a RAM buffer
void ZX0_UnpackToRAM(const void* source, void* dest)
{
	source;	// HL
	dest;	// DE

__asm
// Parameters:
//   HL: Source address (compressed data)
//   DE: Destination address (decompressing)
dzx0_standard:
	ld      bc, #0xFFFF             // Preserve default offset 1
	push    bc
	inc     bc
	ld      a, #0x80
dzx0s_literals:
	call    dzx0s_elias             // Obtain length
	ldir                            // Copy literals
	add     a, a                    // Copy from last offset or new offset?
	jr      c, dzx0s_new_offset
	call    dzx0s_elias             // Obtain length
dzx0s_copy:
	ex      (sp), hl                // Preserve source, restore offset
	push    hl                      // Preserve offset
	add     hl, de                  // Calculate destination - offset
	ldir                            // Copy from offset
	pop     hl                      // Restore offset
	ex      (sp), hl                // Preserve offset, restore source
	add     a, a                    // Copy from literals or new offset?
	jr      nc, dzx0s_literals
dzx0s_new_offset:
	pop     bc                      // Discard last offset
	ld      c, #0xFE                // Prepare negative offset
	call    dzx0s_elias_loop        // Obtain offset MSB
	inc     c
	ret     z                       // Check end marker
	ld      b, c
	ld      c, (hl)                 // Obtain offset LSB
	inc     hl
	rr      b                       // Last offset bit becomes first length bit
	rr      c
	push    bc                      // Preserve new offset
	ld      bc, #1                  // Obtain length
	call    nc, dzx0s_elias_backtrack
	inc     bc
	jr      dzx0s_copy
dzx0s_elias:
	inc     c                       // Interlaced Elias gamma coding
dzx0s_elias_loop:
	add     a, a
	jr      nz, dzx0s_elias_skip
	ld      a, (hl)                 // Load another group of 8 bits
	inc     hl
	rla
dzx0s_elias_skip:
	ret     c
dzx0s_elias_backtrack:
	add     a, a
	rl      c
	rl      b
	jr      dzx0s_elias_loop
__endasm;
}
