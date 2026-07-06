// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄             ▄  ▄   
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ▄▀██ ██▀▄   ▄▄ ▄   
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀▄██ ██ █ ▄ ██ ██  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                  ▀▀                                              
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Kanji ROM handling module
//─────────────────────────────────────────────────────────────────────────────
//  MRC wiki
//     https://www.msx.org/wiki/Category:Kanji-ROM
//     https://www.msx.org/wiki/Kanji_display
//     https://www.msx.org/wiki/KuTen_-_JIS_-_SJIS_Code_Conversion_Tables
//   MSX Datapack - 6.4 MSX Kanji Driver
//     https://ngs.no.coocan.jp/doc/wiki.cgi/datapack?page=6%BE%CF+%C6%E2%C9%F4%A5%EB%A1%BC%A5%C1%A5%F3
//─────────────────────────────────────────────────────────────────────────────
#include "kanji.h"
#include "system_port.h"

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// FUNCTIONS
//=============================================================================

#if (KANJI_USE_DETECT)

//-----------------------------------------------------------------------------
// Get Kanji-ROM level supported by the current machine
u8 Kanji_DetectLevel() __NAKED
{
	__asm

	kanji_check_l1:
		ld		h, #0x02			// Character '＼'
		ld		a, #0x00			// Character '＼'
		out		(P_KANJI_LEVEL1_LOW), a	// Number 6 low bits
		ld		c, #P_KANJI_LEVEL1_HIGH
		out		(c), h				// Number 6 high bits
		ld		hl, #kanji_detect_l1
		call	kanji_detect_check
		ld		a, #0				// Note: don't use 'xor' to don't change flags
		ret		nz					// Level 1 not found

	kanji_check_l2:
		ld		h, #0x35			// Character '龠'
		ld		a, #0x3E			// Character '龠'
		out		(P_KANJI_LEVEL2_LOW), a	// Number 6 low bits
		ld		c, #P_KANJI_LEVEL2_HIGH
		out		(c), h				// Number 6 high bits
		ld		hl, #kanji_detect_l2
		call	kanji_detect_check
		ld		a, #1
		ret		nz					// Level 1 found but 2 not found
		inc		a
		ret							// Level 1 and 2 found

	kanji_detect_check::
		ld		b, #8
	kanji_detect_loop:
		in		a, (c)
		cp		(hl)
		ret		nz
		inc		hl					// reset zero flag
		djnz	kanji_detect_loop
		ret

	kanji_detect_l1:
		// Data detecting level 1 (0x2140 => 0x0120 => Ku:1, Ten:32 => (1 * 96) + 32 = 128 => 0x0080 ==> 0x0200) 
		.db 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01

	kanji_detect_l2:
		// Data detecting level 2 (0x737E => 0x535E => Ku:83, Ten:94 => ((83 - 48) * 96) + 94 = 3454 => 0x0D7E ==> 0x353E) 
		.db 0x01, 0x02, 0x0C, 0x37, 0xC0, 0x3B, 0x2A, 0x2A
	__endasm;
}

#endif // (KANJI_USE_DETECT)

//-----------------------------------------------------------------------------
// Get bitmap of the given kanji from its index in the Kanji-ROM
void Kanji_GetFromIndex(u8* buf, u16 index)
{
	buf;   // -> HL
	index; // -> DE

	__asm
		// Select kanji
		ld		a, e
		// and		#0x3F
		out		(P_KANJI_LEVEL1_LOW), a		// Code's 6 low bits

		ld      a, d
		rl      e
		rla
		rl      e
		rla
		ld		c, #P_KANJI_LEVEL1_HIGH
		out		(c), a				// Code's 6 high bits

		// Get bitmap
		.rept 32
		ini							// in (hl++), (c)
		.endm
	__endasm;
}

#if (KANJI_USE_LEVEL2)

//-----------------------------------------------------------------------------
// Get bitmap of the given level 2 kanji from its index in the Kanji-ROM
void Kanji_GetFromIndexL2(u8* buf, u16 index)
{
	buf;   // -> HL
	index; // -> DE

	__asm
		// Adjust index for level 2 kanji
		ex		de, hl
		ld		bc, #-4608
		add		hl, bc
		ex		de, hl

		// Select kanji
		ld		a, e
		// and		#0x3F
		out		(P_KANJI_LEVEL2_LOW), a		// Code's 6 low bits

		ld      a, d
		rl      e
		rla
		rl      e
		rla
		ld		c, #P_KANJI_LEVEL2_HIGH
		out		(c), a				// Code's 6 high bits

		// Get bitmap
		.rept 32
		ini							// in (hl++), (c)
		.endm
	__endasm;
}

#endif