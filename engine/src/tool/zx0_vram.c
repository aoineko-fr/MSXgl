// -----------------------------------------------------------------------------
// ZX0 decoder by Einar Saukas
// "Standard" version (69 bytes only)
// Modified by Maxim (20210817) to support decompressing to SMS VRAM
// - Inlining the ldir replacements may save a few cycles
// -----------------------------------------------------------------------------
// Parameters:
//   HL: source address (compressed data)
//   DE: destination address (decompressing)
// -----------------------------------------------------------------------------
void ZX0_UnpackToVRAM(const void* source, void* dest) __naked
{
	source; // HL
	dest;   // DE

__asm
// -----------------------------------------------------------------------------
// ZX0 decoder by Einar Saukas & Urusergi
// "Standard" version enhanced for decompression to VRAM
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

	push    af
dzx0s_literal_to_VRAM:
	push    bc

	// Set destination address bits 0~7 to port #1

	ld      a, d
	rlca
	rlca
	and     a, #0x03
	di                              //~~~~~~~~~~~~~~~~~~~~~~~~~~
	out     (P_VDP_REG), a          // RegPort = (destHigh << 2) + (destLow >> 14);
	ld      a, #(0x80 | 0x0E)
	out     (P_VDP_REG), a          // RegPort = VDP_REG(14);

	ld      a, e
	out     (P_VDP_ADDR), a         // AddrPort = (srcLow & 0x00FF)
	// Set destination address bits 8~13 to port #1
	ld      a, d
	and     a, #0x3F
	add     a, #F_VDP_WRIT
	out     (P_VDP_ADDR), a         // 12cc   AddrPort = ((srcLow >> 8) & 0x3F) + F_VDP_WRIT
	// Write data
	ld      a, (hl)
	ei                              //~~~~~~~~~~~~~~~~~~~~~~ //  5cc
	out     (P_VDP_DATA), a         // 12cc   DataPort = value

	pop     bc
	inc     hl
	inc     de
	dec     bc
	ld      a,b
	or      c
	jr      nz,dzx0s_literal_to_VRAM
	pop     af
	add     a, a                    // copy from last offset or new offset?
	jr      c, dzx0s_new_offset
	call    dzx0s_elias             // obtain length
dzx0s_copy:
	ex      (sp), hl                // preserve source, restore offset
	push    hl                      // preserve offset
	add     hl, de                  // calculate destination - offset

	push    af
dzx0s_offset_to_VRAM:
	push    bc

	ld      a, h
	rlca
	rlca
	and     a, #0x03
	di                              //~~~~~~~~~~~~~~~~~~~~~~~~~~
	out     (P_VDP_REG), a          // RegPort = (destHigh << 2) + (destLow >> 14);
	ld      a, #(0x80 | 0x0E)
	out     (P_VDP_REG), a          // RegPort = VDP_REG(14);

	// Set destination address bits 0~7 to port #1
	ld      a, l
	out     (P_VDP_ADDR), a         // AddrPort = (srcLow & 0x00FF)
	// Set destination address bits 8~13 to port #1
	ld      a, h
	and     a, #0x3F                // F_VDP_READ
	out     (P_VDP_ADDR), a         // 12cc      AddrPort = ((srcLow >> 8) & 0x3F) + F_VDP_READ
	// Wait for VDP to be ready
	nop                             //  5cc      For 20cc wait
	// Read data
	ei                              //~~~~~~~~~~~~~~~~~~~~~~ //  5cc
	in      a, (P_VDP_DATA)         // 12cc     value = DataPort

	ld      b, a
	ld      a, d
	rlca
	rlca
	and     a, #0x03
	di                              //~~~~~~~~~~~~~~~~~~~~~~~~~~
	out     (P_VDP_REG), a          // RegPort = (destHigh << 2) + (destLow >> 14);
	ld      a, #(0x80 | 0x0E)
	out     (P_VDP_REG), a          // RegPort = VDP_REG(14);

	// Set destination address bits 0~7 to port #1
	ld      a, e
	out     (P_VDP_ADDR), a         // AddrPort = (srcLow & 0x00FF)
	// Set destination address bits 8~13 to port #1
	ld      a, d
	and     a, #0x3F
	add     a, #F_VDP_WRIT
	out     (P_VDP_ADDR), a         // 12cc   AddrPort = ((srcLow >> 8) & 0x3F) + F_VDP_WRIT
	// Write data
	ld      a, b
	ei                              //~~~~~~~~~~~~~~~~~~~~~~ //  5cc
	out     (P_VDP_DATA), a         // 12cc   DataPort = value

	pop     bc
	inc     hl
	inc     de
	dec     bc
	ld      a,b
	or      c
	jr      nz,dzx0s_offset_to_VRAM
	pop     af

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
