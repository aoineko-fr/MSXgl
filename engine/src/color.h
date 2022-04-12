// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

/// Bits-per-color enum
enum COLOR_BPC
{
	BPC_1 = 0,	///< 1-bit		2 colors		[c1|c2|c3|c4|c5|c6|c7|c8]
	BPC_2,		///< 2-bits		4 colors		[c1:2|c2:2|c3:2|c4:2]
	BPC_4,		///< 4-bits		16 colors		[c1:4|c2:4]
	BPC_8,		///< 8-bits		256 colors		[c:8]
};

//-----------------------------------------------------------------------------
// DEFAULT 4-BITS MSX PALETTE
//-----------------------------------------------------------------------------

// 16 colors index
#define COLOR_TRANSPARENT	0
#define COLOR_BLACK			1
#define COLOR_MEDIUM_GREEN	2
#define COLOR_LIGHT_GREEN	3
#define COLOR_DARK_BLUE		4
#define COLOR_LIGHT_BLUE	5
#define COLOR_DARK_RED		6
#define COLOR_CYAN			7
#define COLOR_MEDIUM_RED	8
#define COLOR_LIGHT_RED		9
#define COLOR_DARK_YELLOW	10
#define COLOR_LIGHT_YELLOW	11
#define COLOR_DARK_GREEN	12
#define COLOR_MAGENTA		13
#define COLOR_GRAY			14
#define COLOR_WHITE			15

// Merge 2 x 4-bits colors on 1 byte
#define COLOR_MERGE(a, b)		(u8)((a & 0x0F) << 4 | (b & 0x0F))
#define COLOR_MERGE_4B			COLOR_MERGE
#define COLOR_MERGE2(a)			(u8)((a & 0x0F) << 4 | (a & 0x0F))

// Sprite GM7 palette                   G R B
#define COLOR_S7_BLACK			0	// (0,0,0)
#define COLOR_S7_DARK_BLUE		1	// (0,0,2)
#define COLOR_S7_DARK_RED		2	// (0,3,0)
#define COLOR_S7_DARK_MAGENTA	3	// (0,3,2)
#define COLOR_S7_DARK_GREEN		4	// (3,0,0)
#define COLOR_S7_DARK_CYAN		5	// (3,0,2)
#define COLOR_S7_DARK_YELLOW	6	// (3,3,0)
#define COLOR_S7_GRAY			7	// (3,3,2)
#define COLOR_S7_ORANGE			8	// (4,7,2)
#define COLOR_S7_BLUE			9	// (0,0,7)
#define COLOR_S7_RED			10	// (0,7,0)
#define COLOR_S7_MAGENTA		11	// (0,7,7)
#define COLOR_S7_GREEN			12	// (7,0,0)
#define COLOR_S7_CYAN			13	// (7,0,7)
#define COLOR_S7_YELLOW			14	// (7,7,0)
#define COLOR_S7_WHITE			15	// (7,7,7)

// Merge 4 x 2-bits colors on 1 byte (for GM5)
#define COLOR_MERGE_2B(a, b, c, d)	(u8)((a & 0x03) << 6 | (b & 0x03) << 4 | (c & 0x03) << 2 | (d & 0x03))
#define COLOR_MERGE4(a)				(u8)((a & 0x03) << 6 | (a & 0x03) << 4 | (a & 0x03) << 2 | (a & 0x03))

//-----------------------------------------------------------------------------
// 8-BITS RGB COLORS
//-----------------------------------------------------------------------------
// Format : [G:3|R:3|B:2]

// Helper macros
#define RGB8(r, g, b)		(u8)(((g & 0x07) << 5) + ((r & 0x07) << 2) + (b & 0x03))
#define GRB8(g, r, b)		(u8)(((g & 0x07) << 5) + ((r & 0x07) << 2) + (b & 0x03))

// Some colors
#define COLOR8_KHAKI		RGB8(5,6,0)
#define COLOR8_DARKKAKHI	RGB8(3,4,0)
#define COLOR8_SKIN			RGB8(6,5,2)
#define COLOR8_DARKSKIN		RGB8(4,3,1)
#define COLOR8_PINK			RGB8(7,0,2)
#define COLOR8_DARKPINK		RGB8(5,0,1)
#define COLOR8_SAND			RGB8(6,6,2)
#define COLOR8_LIGHTMAUVE	RGB8(5,5,3)
#define COLOR8_GRAY			RGB8(5,5,2)
#define COLOR8_DARKGRAY		RGB8(3,3,1)
#define COLOR8_BROWN		RGB8(4,2,0)
#define COLOR8_DARKBROWN	RGB8(2,1,0)
#define COLOR8_YELLOW		RGB8(6,6,1)
#define COLOR8_DARKYELLOW	RGB8(4,4,0)
#define COLOR8_LIGHTGREEN	RGB8(2,7,1)
#define COLOR8_GREEN		RGB8(0,5,0)
#define COLOR8_DARKGREEN	RGB8(0,3,0)
#define COLOR8_WHITE		RGB8(7,7,3)
#define COLOR8_LIGHTGRAY	RGB8(6,6,3)
#define COLOR8_CYAN			RGB8(6,7,3)
#define COLOR8_LIGHTBLUE	RGB8(5,6,3)
#define COLOR8_BLUE			RGB8(2,4,3)
#define COLOR8_DARKBLUE		RGB8(1,2,3)
#define COLOR8_NAVYBLUE		RGB8(0,0,2)
#define COLOR8_DARKNAVYBLUE	RGB8(0,0,1)
#define COLOR8_MAUVE		RGB8(6,3,2)
#define COLOR8_DARKMAUVE	RGB8(4,2,1)
#define COLOR8_ORANGE		RGB8(7,4,0)
#define COLOR8_DARKORANGE	RGB8(6,2,0)
#define COLOR8_RED			RGB8(7,1,0)
#define COLOR8_DARKRED		RGB8(5,1,0)
#define COLOR8_BLACK		RGB8(0,0,0)
#define COLOR8_LIME			RGB8(3,7,0)
#define COLOR8_DARKLIME		RGB8(2,5,0)

// Default palette colors
#define COLOR8_DEFAULT0		RGB8(0,0,0)		
#define COLOR8_DEFAULT1		RGB8(0,0,0)		
#define COLOR8_DEFAULT2		RGB8(1,6,0)		
#define COLOR8_DEFAULT3		RGB8(3,7,1)		
#define COLOR8_DEFAULT4		RGB8(1,1,3)		
#define COLOR8_DEFAULT5		RGB8(2,3,3)		
#define COLOR8_DEFAULT6		RGB8(5,1,0)		
#define COLOR8_DEFAULT7		RGB8(2,6,3)		
#define COLOR8_DEFAULT8		RGB8(7,1,0)		
#define COLOR8_DEFAULT9		RGB8(7,3,1)		
#define COLOR8_DEFAULT10	RGB8(6,6,0)		
#define COLOR8_DEFAULT11	RGB8(6,6,2)		
#define COLOR8_DEFAULT12	RGB8(1,4,0)
#define COLOR8_DEFAULT13	RGB8(6,2,2)
#define COLOR8_DEFAULT14	RGB8(5,5,2)
#define COLOR8_DEFAULT15	RGB8(7,7,3)

//-----------------------------------------------------------------------------
// 16-BITS RGB COLORS
//-----------------------------------------------------------------------------
// Format : [00000|G:3] [0|R:3|0|B:3]

// Helper macros
#define RGB16(r, g, b)		(u16)(((u16)(g & 0x07) << 8) + ((r & 0x07) << 4) + (b & 0x07))
#define GRB16(g, r, b)		(u16)(((u16)(g & 0x07) << 8) + ((r & 0x07) << 4) + (b & 0x07))

#define COLOR16_DEFAULT_0	RGB16(0, 0, 0)
#define COLOR16_DEFAULT_1	RGB16(0, 0, 0)
#define COLOR16_DEFAULT_2	RGB16(1, 6, 1)
#define COLOR16_DEFAULT_3	RGB16(3, 7, 3)
#define COLOR16_DEFAULT_4	RGB16(1, 1, 7)
#define COLOR16_DEFAULT_5	RGB16(2, 3, 7)
#define COLOR16_DEFAULT_6	RGB16(5, 1, 1)
#define COLOR16_DEFAULT_7	RGB16(2, 6, 7)
#define COLOR16_DEFAULT_8	RGB16(7, 1, 1)
#define COLOR16_DEFAULT_9	RGB16(7, 3, 3)
#define COLOR16_DEFAULT_10	RGB16(6, 6, 1)
#define COLOR16_DEFAULT_11	RGB16(6, 6, 4)
#define COLOR16_DEFAULT_12	RGB16(1, 4, 1)
#define COLOR16_DEFAULT_13	RGB16(6, 2, 5)
#define COLOR16_DEFAULT_14	RGB16(5, 5, 5)
#define COLOR16_DEFAULT_15	RGB16(7, 7, 7)

#define COLOR16_MSX1_0		RGB16(0, 0, 0)
#define COLOR16_MSX1_1		RGB16(0, 0, 0)
#define COLOR16_MSX1_2		RGB16(1, 5, 1)
#define COLOR16_MSX1_3		RGB16(3, 6, 3)
#define COLOR16_MSX1_4		RGB16(2, 2, 6)
#define COLOR16_MSX1_5		RGB16(3, 3, 7)
#define COLOR16_MSX1_6		RGB16(5, 2, 2)
#define COLOR16_MSX1_7		RGB16(2, 6, 7)
#define COLOR16_MSX1_8		RGB16(6, 2, 2)
#define COLOR16_MSX1_9		RGB16(6, 3, 3)
#define COLOR16_MSX1_10		RGB16(5, 5, 2)
#define COLOR16_MSX1_11		RGB16(6, 6, 3)
#define COLOR16_MSX1_12		RGB16(1, 4, 1)
#define COLOR16_MSX1_13		RGB16(5, 2, 5)
#define COLOR16_MSX1_14		RGB16(5, 5, 5)
#define COLOR16_MSX1_15		RGB16(7, 7, 7)