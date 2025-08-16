// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "v9990.h"
#include "math.h"
#include "memory.h"

//=============================================================================
// DEFINES
//=============================================================================

#define CUSTOM_ISR					0

// Library's logo
#define MSX_GL						"\x01\x02\x03\x04\x05\x06"

// Display constants
#define V9_GROUND_Y					20
#define V9_HORIZON_Y				14
#define BMP_TILES_Y					256

// Bits per pixel
#define BPP_2						0x02
#define BPP_4						0x04
#define BPP_8						0x08
#define BPP_16						0x10

// Screen mode index (match the g_ScreenMode table)
enum 
{
#if (V9_USE_MODE_P1)
	MODE_P1,
#endif
#if (V9_USE_MODE_P2)
	MODE_P2,
#endif
#if ((V9_USE_MODE_B0) || (V9_USE_MODE_B1) || (V9_USE_MODE_B2) || (V9_USE_MODE_B3) || (V9_USE_MODE_B4) || (V9_USE_MODE_B5) || (V9_USE_MODE_B6) || (V9_USE_MODE_B7))
	MODE_B0,
	MODE_B1,
	MODE_B2,
	MODE_B3,
	MODE_B4,
	MODE_B5,
	MODE_B6,
	MODE_B7,
#endif
	MODE_MAX,
};

// Screen mode structure
struct ScreenMode
{
	const c8* Name;
	u8        Mode;
	callback  Init;
	callback  Tick;
	callback  Hblank;
	u8        BPP;
	u16       Width;
	u16       Height;
};

// Color mode structure
struct ColorMode
{
	const c8* Name;
	u8        Mode;
	u8        BPP;
	u16       ScaneLine;
};

// Function prototypes
void InitP1();  void TickP1();  void HblankP1();
void InitP2();  void TickP2();  void HblankP2();
void InitBmp(); void TickBmp(); void HblankBmp();

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// 1-bit font (VDP)
#include "font/font_mgl_sample6.h"

// 4-bits font (V9990)
#include "content/v9990/data_v9_font.h"

// Sprite
#include "content/v9990/data_v9_chr.h"

// Tiles
#include "content/v9990/data_v9_bg.h"

// Trigo table
#include "mathtable/mt_sin_64.inc"

// Animation characters
const u8 g_ChrAnim[4] = { '|', '\\', '-', '/' };

// Black color
const u8 g_ColorBlack[] = { 0, 0, 0 };
const u8 g_ColorBG[] = { 0x07, 0x12, 0x1C };

// Screen mode configuration
const struct ScreenMode g_ScreenMode[MODE_MAX] =
{
#if (V9_USE_MODE_P1)
	{ "P1", V9_MODE_P1, InitP1,  TickP1,  HblankP1,  BPP_4, 256, 212 },
#endif
#if (V9_USE_MODE_P2)
	{ "P2", V9_MODE_P2, InitP2,  TickP2,  HblankP2,  BPP_4, 512, 212 },
#endif
#if ((V9_USE_MODE_B0) || (V9_USE_MODE_B1) || (V9_USE_MODE_B2) || (V9_USE_MODE_B3) || (V9_USE_MODE_B4) || (V9_USE_MODE_B5) || (V9_USE_MODE_B6) || (V9_USE_MODE_B7))
	{ "B0", V9_MODE_B0, InitBmp, TickBmp, HblankBmp, BPP_2|BPP_4|BPP_8|BPP_16, 192, 240 },
	{ "B1", V9_MODE_B1, InitBmp, TickBmp, HblankBmp, BPP_2|BPP_4|BPP_8|BPP_16, 256, 212 },
	{ "B2", V9_MODE_B2, InitBmp, TickBmp, HblankBmp, BPP_2|BPP_4|BPP_8|BPP_16, 384, 240 },
	{ "B3", V9_MODE_B3, InitBmp, TickBmp, HblankBmp, BPP_2|BPP_4|BPP_8|BPP_16, 512, 212 },
	{ "B4", V9_MODE_B4, InitBmp, TickBmp, HblankBmp, BPP_2|BPP_4, 768, 240 },
	{ "B5", V9_MODE_B5, InitBmp, TickBmp, HblankBmp, BPP_2|BPP_4, 640, 400 },
	{ "B6", V9_MODE_B6, InitBmp, TickBmp, HblankBmp, BPP_2|BPP_4, 640, 480 },
	{ "B7", V9_MODE_B7, InitBmp, TickBmp, HblankBmp, BPP_2|BPP_4, 1024, 212 },
#endif
};

// Color mode configuration for bitmap modes
const struct ColorMode g_ColorMode[V9_COLOR_BMP_MAX] =
{
	{ "BP2",   V9_COLOR_BP2,   BPP_2,  256  }, // Color palette (4 colors out of 32768 colors)
	{ "BP4",   V9_COLOR_BP4,   BPP_4,  512  }, // Color palette (16 colors out of 32768 colors)
	{ "BP6",   V9_COLOR_BP6,   BPP_8,  1024 }, // Color palette (64 colors out of 32768 colors)
	{ "BD8",   V9_COLOR_BD8,   BPP_8,  1024 }, // Direct RGB [G:3|R:3|B:2] (256 colors)
	{ "BYJK",  V9_COLOR_BYJK,  BPP_8,  1024 }, // YJK Decoder (19268 colors)
	{ "BYJKP", V9_COLOR_BYJKP, BPP_8,  1024 }, // YJK Decoder + Color palette (12599 colors + 16 colors out of 32768 colors)
	{ "BYUV",  V9_COLOR_BYUV,  BPP_8,  1024 }, // YUV Decoder (19268 colors)
	{ "BYUVP", V9_COLOR_BYUVP, BPP_8,  1024 }, // YUV Decoder + Color palette (12599 colors + 16 colors out of 32768 colors)
	{ "BD16",  V9_COLOR_BD16,  BPP_16, 2048 }, // Direct RGB [YS|G:5|R:2][R:3|B:5] (32768 colors)
};

//=============================================================================
// MEMORY DATA
//=============================================================================

u8   g_CurrentScreen = MODE_P1; // Current screen mode index in the g_ScreenMode table
u8   g_CurrentColor = V9_COLOR_BP4; // Current color mode index in the g_ColorMode table

u16  g_Frame = 0;
bool g_Interlaced = false;
u16  g_TextX;
u16  g_TextY;
bool g_HideSprite = FALSE;
bool g_KeyPressed[256];

//=============================================================================
// CODE
//=============================================================================

void InitPalette()
{
	switch(g_ScreenMode[g_CurrentScreen].Mode)
	{
	case V9_MODE_P1:
	case V9_MODE_P2:
		V9_SetPaletteEntry(0, g_ColorBlack);
		V9_SetPalette(1,  15, g_DataV9BG_palette);
		V9_SetPaletteEntry(16, g_ColorBlack);
		V9_SetPalette(17, 15, g_DataV9Chr_palette);
		V9_SetPaletteEntry(32, g_ColorBlack);
		V9_SetPalette(33, 15, g_DataV9Font_palette);
		break;
	case V9_MODE_B0:
	case V9_MODE_B1:
	case V9_MODE_B2:
	case V9_MODE_B3:
		V9_SetPaletteEntry(0, g_ColorBlack);
		V9_SetPalette(1,  15, g_DataV9BG_palette);
		V9_SetPaletteEntry(16, g_ColorBlack);
		V9_SetPalette(17, 15, g_DataV9Font_palette);
		V9_SetPaletteEntry(16 + 6, g_ColorBG);
		break;
	case V9_MODE_B4:
	case V9_MODE_B5:
	case V9_MODE_B6:
	case V9_MODE_B7:
		loop(i, 2)
		{
			V9_SetPaletteEntry(i * 32 + 0, g_ColorBlack);
			V9_SetPalette(i * 32 + 1,  15, g_DataV9BG_palette);
			V9_SetPaletteEntry(i * 32 + 16, g_ColorBlack);
			V9_SetPalette(i * 32 + 17, 15, g_DataV9Font_palette);
			V9_SetPaletteEntry(i * 32 + 16 + 6, g_ColorBG);
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// P1 MODE (256x212, 2 layers)
//-----------------------------------------------------------------------------

#if (V9_USE_MODE_P1)

void V9_Print(u32 addr, const c8* str)
{
	while(*str != 0)
		V9_Poke16(addr += 2, 512 + *(str++) - ' ');
}

//
void InitP1()
{
	// Initialize screen mode
	V9_SetScreenMode(V9_MODE_P1);
	V9_SetDisplayEnable(FALSE);
	V9_SetInterrupt(V9_INT_NONE);
	V9_SetBackgroundColor(6);
	V9_SetSpriteEnable(TRUE);

	InitPalette();

	// Pattern generator table
	V9_WriteVRAM(V9_P1_PGT_A, g_DataV9BG, sizeof(g_DataV9BG));
	V9_WriteVRAM(V9_P1_PGT_A + 0x04000, g_DataV9Font, sizeof(g_DataV9Font));
	V9_WriteVRAM(V9_P1_PGT_B, g_DataV9BG, sizeof(g_DataV9BG));

	// Pattern name table
	V9_FillVRAM16(V9_P1_PNT_A, 0x0000, 64*64); // Init layer A
	V9_Print(V9_P1_PNT_A, "MSXgl V9990 - P1 Mode (PP)"); // Draw title
	for(u8 i = 0; i < 8; ++i) // Draw plateform
	{
		u8 x = i * 8;
		u8 y = Math_GetRandom8() % 8 + 10;
		for(u8 j = y; j < V9_GROUND_Y; j++)
		{
			u8 cell;
			if(j == y)
				cell = 32;
			else if(((j - y) & 1))
				cell = 64;
			else
				cell = 96;

			u32 addr = V9_CellAddrP1A(x, j);
			V9_Poke16(addr += 2, cell++);
			V9_Poke16(addr += 2, cell++);
			V9_Poke16(addr += 2, cell++);
			V9_Poke16(addr += 2, cell++);
		}
	}
	for(u16 i = 0; i < 64; ++i) // Draw ground
	{
		for(u8 j = V9_GROUND_Y; j < 27; j++)
		{
			u8 cell;
			if(j == V9_GROUND_Y)
				cell = 33;
			else
				cell = (j & 1) ? 65 : 97;
			V9_Poke16(V9_CellAddrP1A(0, j) + i*2, cell + i%2);
		}
	}

	V9_FillVRAM16(V9_P1_PNT_B, 160, 64*27); // Init layer B
	for(u16 i = 0; i < 64; ++i) // Draw horizon
		V9_Poke16(V9_CellAddrP1B(0, V9_HORIZON_Y) + i*2, 128 + i%4);
	V9_FillVRAM16(V9_P1_PNT_B, 135, 64*V9_HORIZON_Y); // Draw sky
	for(u8 i = 0; i < 6; ++i) // Draw big cloud
	{
		u8 x = Math_GetRandom8() % 8 + (i * 10);
		u8 y = (i == 0) ? 0 : Math_GetRandom8() % 8;
		u32 addr = V9_CellAddrP1B(x, y);
		V9_Poke16(addr, 132); addr += 2;
		V9_Poke16(addr, 133); addr += 2;
		V9_Poke16(addr, 134); addr += 2;
		V9_Poke16(addr, 135);
		addr = V9_CellAddrP1B(x, y +1);
		V9_Poke16(addr, 164); addr += 2;
		V9_Poke16(addr, 165); addr += 2;
		V9_Poke16(addr, 166); addr += 2;
		V9_Poke16(addr, 167);
	}
	for(u8 i = 0; i < 6; ++i) // Draw small cloud
	{
		u8 x = Math_GetRandom8() % 8 + (i * 10);
		u8 y = Math_GetRandom8() % 4 + 9;
		u32 addr = V9_CellAddrP1B(x, y);
		V9_Poke16(addr, 136); addr += 2;
		V9_Poke16(addr, 137);
		addr = V9_CellAddrP1B(x, y +1);
		V9_Poke16(addr, 168); addr += 2;
		V9_Poke16(addr, 169);
	}

	// Sprite pattern generator table
	V9_SetSpritePatternAddr(V9_P1_SGT_08000);
	V9_WriteVRAM(0x08000, g_DataV9Chr, sizeof(g_DataV9Chr));

	// Sprite attribute table
	struct V9_Sprite attr;
	for(u16 i = 0; i < 125; ++i)
	{
		attr.Y = (i / 13) * 20 + 12;
		attr.Pattern = 0;
		attr.X = (i % 13) * 20;
		attr.P = 0;
		attr.D = 0;
		attr.SC = 1;
		V9_SetSpriteP1(i, &attr);
	}

	V9_SetInterrupt(V9_INT_VBLANK | V9_INT_HBLANK);
	V9_SetInterruptLine(71);
	V9_SetDisplayEnable(TRUE);
}

//
void TickP1()
{
	V9_SelectPaletteP1(2, 0);
	V9_SetScrollingX(0);
	V9_SetScrollingBX(g_Frame >> 1);

	u8 frame = (g_Frame >> 2) % 6;
	for(u16 i = 0; i < 125; ++i)
		V9_SetSpritePatternP1(i, frame);

	g_Frame++;
}

//
void HblankP1()
{
	V9_SelectPaletteP1(0, 0);
	V9_SetScrollingX(g_Frame >> 0);
	V9_SetScrollingBX(g_Frame >> 2);
}

#endif // (V9_USE_MODE_P1)

//-----------------------------------------------------------------------------
// P1 MODE (512x212, 1 layer)
//-----------------------------------------------------------------------------

#if (V9_USE_MODE_P2)

void V9_WriteVRAM_256to512(u32 addr, const u8* src, u8 line)
{
	for(u8 i = 0; i < line; ++i)
	{
		u8 line = i % 8;
		u8 row = i / 2 / 8 * 8;
		u8 odd = (i / 8) & 1;
		V9_WriteVRAM(addr + (256 * (row + line)) + (128 * odd),  src + (128 * i), 128);
	}
}

//
void InitP2()
{
	// Initialize screen mode
	V9_SetScreenMode(V9_MODE_P2);
	V9_SetDisplayEnable(FALSE);
	V9_SetInterrupt(V9_INT_NONE);
	V9_SetBackgroundColor(6);
	V9_SetSpriteEnable(TRUE);

	InitPalette();

	// Pattern generator table
	V9_WriteVRAM_256to512(V9_P2_PGT + 0x00000, g_DataV9BG, 48);
	V9_WriteVRAM_256to512(V9_P2_PGT + 0x04000, g_DataV9Font, 24);

	// Pattern name table
	// for(u16 i = 0; i < 64*27; ++i)
	// {
		// u16 addr = (((i / 64) * 128) + (i % 64)) * 2;
		// V9_Poke16(V9_P2_PNT + addr, i);
		// addr += 2;
	// }
	V9_FillVRAM16(V9_P2_PNT, 0, 128*2);
	V9_Print(V9_P2_PNT, "MSXgl V9990 - P2 Mode (PP)"); // Draw title
	V9_FillVRAM16(V9_P2_PNT + 512, 135, 128*V9_HORIZON_Y); // Draw sky
	for(u16 i = 0; i < 128; ++i) // Draw horizon
		V9_Poke16(V9_CellAddrP2(0, V9_HORIZON_Y) + i*2, 128 + i%4);
	V9_FillVRAM16(V9_CellAddrP2(0, V9_HORIZON_Y+1), 160, (27-V9_HORIZON_Y)*128); // Draw sea
	for(u8 i = 0; i < 12; ++i) // Draw big cloud
	{
		u8 x = Math_GetRandom8() % 8 + (i * 10);
		u8 y = Math_GetRandom8() % 4 + 4;
		u32 addr = V9_CellAddrP2(x, y);
		V9_Poke16(addr, 132); addr += 2;
		V9_Poke16(addr, 133); addr += 2;
		V9_Poke16(addr, 134); addr += 2;
		V9_Poke16(addr, 135);
		addr = V9_CellAddrP2(x, y +1);
		V9_Poke16(addr, 164); addr += 2;
		V9_Poke16(addr, 165); addr += 2;
		V9_Poke16(addr, 166); addr += 2;
		V9_Poke16(addr, 167);
	}
	for(u8 i = 0; i < 12; ++i) // Draw small cloud
	{
		u8 x = Math_GetRandom8() % 8 + (i * 10);
		u8 y = Math_GetRandom8() % 4 + 9;
		u32 addr = V9_CellAddrP2(x, y);
		V9_Poke16(addr, 136); addr += 2;
		V9_Poke16(addr, 137);
		addr = V9_CellAddrP2(x, y +1);
		V9_Poke16(addr, 168); addr += 2;
		V9_Poke16(addr, 169);
	}
	for(u8 i = 0; i < 8; ++i) // Draw plateform
	{
		u8 x = i * 16;
		u8 y = Math_GetRandom8() % 8 + 10;
		for(u8 j = y; j < V9_GROUND_Y; j++)
		{
			u8 cell;
			if(j == y)
				cell = 32;
			else if(((j - y) & 1))
				cell = 64;
			else
				cell = 96;

			u32 addr = V9_CellAddrP2(x, j);
			V9_Poke16(addr += 2, cell++);
			V9_Poke16(addr += 2, cell++);
			V9_Poke16(addr += 2, cell++); cell -= 2;
			V9_Poke16(addr += 2, cell++);
			V9_Poke16(addr += 2, cell++); cell -= 2;
			V9_Poke16(addr += 2, cell++);
			V9_Poke16(addr += 2, cell++);
			V9_Poke16(addr += 2, cell++);
		}
	}
	for(u16 i = 0; i < 128; ++i) // Draw ground
	{
		for(u8 j = V9_GROUND_Y; j < 27; j++)
		{
			u8 cell;
			if(j == V9_GROUND_Y)
				cell = 33;
			else
				cell = (j & 1) ? 65 : 97;
			V9_Poke16(V9_CellAddrP2(0, j) + i*2, cell + i%2);
		}
	}

	// Sprite pattern generator table
	V9_SetSpritePatternAddr(V9_P2_SGT_08000);
	for(u8 i = 0; i < 16; ++i)
		V9_WriteVRAM(V9_P2_PGT + 0x08000 + (256 * i), g_DataV9Chr + (128 * i), 128);

	// Sprite attribute table
	struct V9_Sprite attr;
	for(u8 i = 0; i < 125; ++i)
	{
		attr.Y = (i / 13) * 20 + 12;
		attr.Pattern = 0;
		attr.X = (i % 13) * 40 + 8;
		attr.P = 0;
		attr.D = 0;
		attr.SC = 1;
		V9_SetSpriteP2(i, &attr);
	}

	V9_SetInterrupt(V9_INT_VBLANK | V9_INT_HBLANK);
	V9_SetInterruptLine(6);
	V9_SetDisplayEnable(TRUE);
}

//
void TickP2()
{
	V9_SelectPaletteP2(2);
	V9_SetScrollingX(0);

	u8 frame = (g_Frame >> 2) % 6;
	for(u16 i = 0; i < 125; ++i)
		V9_SetSpritePatternP2(i, frame);

	g_Frame++;
}

//
void HblankP2()
{
	V9_SelectPaletteP2(0);
	V9_SetScrollingX(g_Frame >> 0);
	V9_SetScrollingBX(g_Frame >> 2);
}

#endif // (V9_USE_MODE_P1)

//-----------------------------------------------------------------------------
// BITMAP MODE
//-----------------------------------------------------------------------------

#if ((V9_USE_MODE_B0) || (V9_USE_MODE_B1) || (V9_USE_MODE_B2) || (V9_USE_MODE_B3) || (V9_USE_MODE_B4) || (V9_USE_MODE_B5) || (V9_USE_MODE_B6) || (V9_USE_MODE_B7))

//
void V9_SetPrintPositionBmp(u16 x, u16 y)
{
	g_TextX = x;
	g_TextY = y;
}

//
void V9_PrintBmpChar(c8 chr)
{
	u8 idx = chr - ' ';

	V9_WaitCmdEnd();
	V9_CommandLMMM(idx % 32 * 8, BMP_TILES_Y + 48 + (idx / 32) * 8, g_TextX, g_TextY, 8, 8, 0);
	g_TextX += 8;
}

//
void V9_PrintBmpString(const c8* str)
{
	while(*str)
		V9_PrintBmpChar(*str++);
}

// 
void V9_PrintBmpStringAt(u16 x, u16 y, const c8* str)
{
	V9_SetPrintPositionBmp(x, y);
	V9_PrintBmpString(str);
}

//
void V9_DrawTileBmp(u8 x, u8 y, u8 tile)
{
	V9_WaitCmdEnd();
	V9_CommandLMMM((tile % 32) * 8, BMP_TILES_Y + (tile / 32) * 8, x * 8, y * 8, 8, 8, 0);
}

const u8 g_BP6_Palette[] =
{
	0x00, 0x00, 0x00, // [ 1] Black
	0x0A, 0x0A, 0x0A, // [ 2] Dark gray
	0x16, 0x16, 0x16, // [ 3] Light gray
	0x1F, 0x1F, 0x1F, // [ 4] White
};

// 15-bit RGB => 8-bit GRB
const u8 g_BD8_PaletteBG[] =
{
	0b00000000, // [ 0] #000000
	0b00000000, // [ 1] #000000
	0b01000110, // [ 2] #3953AB : 010'001'10
	0b11001111, // [ 3] #71DCE9 : 110'011'11
	0b00110001, // [ 4] #903F43 : 001'100'01
	0b00000100, // [ 5] #2E142E : 000'001'00
	0b10000111, // [ 6] #3B95E2 : 100'001'11
	0b10111101, // [ 7] #EDB774 : 101'111'01
	0b10101110, // [ 8] #79A790 : 101'011'10
	0b11111111, // [ 9] #F6F3F3 : 111'111'11
	0b10010110, // [10] #A89E99 : 100'101'10
	0b00101001, // [11] #542F54 : 001'010'01
	0b01101111, // [12] #6D60E7 : 011'011'11
	0b01101001, // [13] #4E636B : 011'010'01
	0b11011011, // [14] #CEC9CC : 110'110'11
	0b01111001, // [15] #D86E61 : 011'110'01
};

//
void InitBmp()
{
	// Initialize screen mode
	V9_SetScreenMode(g_ScreenMode[g_CurrentScreen].Mode);
	V9_SetDisplayEnable(FALSE);
	V9_SetInterrupt(V9_INT_NONE);
	V9_SetImageSpaceWidth(V9_R06_WIDH_1024);
	V9_SetColorMode(g_ColorMode[g_CurrentColor].Mode);
	V9_SetBackgroundColor(6);
	V9_SetScrollingX(0);

	u16 bgColor = 0;

	if(g_CurrentColor == V9_COLOR_BP2)
	{
		bgColor = 0x5555;
		V9_SetPalette(0, 4, g_BP6_Palette);
		V9_SelectPaletteBP2(0);

		// Load graphics data to VRAM
		// for(u8 i = 0; i < 48; ++i)
		// 	V9_WriteVRAM((u32)(0 + (u32)(512 * (u32)(512 + 0 + i))), g_DataV9BG + (128 * i), 128);
		// for(u8 i = 0; i < 24; ++i)
		// 	V9_WriteVRAM((u32)(0 + (u32)(512 * (u32)(512 + 48 + i))), g_DataV9Font + (128 * i), 128);

		// loop(i, 48+24)
		// 	loop(j, 64)
		// 		V9_Poke((u32)(0 + (u32)(512 * (u32)(256 + 0 + i)) + j), (j & 1) ? 0b11100100 : 0b00011011);
		V9_FillVRAM(BMP_TILES_Y * 256, 0b00000000, 256 * 72);
	}
	else if(g_CurrentColor == V9_COLOR_BP4)
	{
		bgColor = 0x6666;
		InitPalette();
		V9_SelectPaletteBP4(0);

		// Load graphics data to VRAM
		for(u8 i = 0; i < 48; ++i)
			V9_WriteVRAM((u32)(0 + (u32)(512 * (u32)(BMP_TILES_Y + 0 + i))), g_DataV9BG + (128 * i), 128);
		for(u8 i = 0; i < 24; ++i)
		{
			u8* buf = (u8*)Mem_GetHeapAddress();
			loop(j, 128)
			{
				u8 v = *(const u8*)(g_DataV9Font + (128 * i) + j);
				if((v & 0xF0) == 0x00)
					v |= 0x60;
				else if((v & 0xF0) == 0x60)
					v &= 0x0F;
				if((v & 0x0F) == 0x00)
					v |= 0x06;
				else if((v & 0x0F) == 0x06)
					v &= 0xF0;
				buf[j] = v;
			}
			V9_WriteVRAM((u32)(0 + (u32)(512 * (u32)(BMP_TILES_Y + 48 + i))), buf, 128);
		}
	}
	else if(g_CurrentColor == V9_COLOR_BD8)
	{
		bgColor = 0x8787;
		// Load graphics data to VRAM
		const u8* data = g_DataV9BG;
		loop(i, 48)
		{
			u32 addr = BMP_TILES_Y * 1024 + i * 1024;
			loop(j, 128)
			{
				V9_Poke(addr++, g_BD8_PaletteBG[*data >> 4]);
				V9_Poke(addr++, g_BD8_PaletteBG[*data & 0x0F]);
				data++;
			}
		}
	}

	// Initialize background color
	V9_SetCommandLogicalOp(V9_R45_LOP_SET);//| V9_R45_TP);
	V9_SetCommandWriteMask(0xFFFF);
	V9_WaitCmdEnd();
	V9_CommandLMMV(0, 0, 1024, 256, 0, bgColor);

	for(u16 i = 0; i < 128; ++i) // Draw ground
	{
		for(u16 j = V9_HORIZON_Y; j < 32; j++)
		{
			u8 cell;
			if(j == V9_HORIZON_Y)
				cell = 128 + i % 4;
			else if(j < V9_GROUND_Y)
				cell = 160;
			else if(j == V9_GROUND_Y)
				cell = 33 + i % 2;
			else
			{
				cell = (j & 1) ? 65 : 97;
				cell += i % 2;
			}
			V9_DrawTileBmp(i, j, cell);
		}
	}

	for(u8 i = 0; i < 16; ++i) // Draw plateform
	{
		u16 x = i * 8;
		u8 y = Math_GetRandom8() % 8 + 10;
		for(u8 j = y; j < V9_GROUND_Y; j++)
		{
			u8 cell;
			if(j == y)
				cell = 32;
			else if(((j - y) & 1))
				cell = 64;
			else
				cell = 96;
			V9_DrawTileBmp(x,     j, cell++);
			V9_DrawTileBmp(x + 1, j, cell++);
			V9_DrawTileBmp(x + 2, j, cell++);
			V9_DrawTileBmp(x + 3, j, cell++);
		}
	}

	// Draw title
	V9_PrintBmpStringAt(g_CurrentScreen > MODE_B1 ? 16 : 8, 1, "MSXgl V9990 - ");
	V9_PrintBmpString(g_ScreenMode[g_CurrentScreen].Name);
	V9_PrintBmpString(" (");
	V9_PrintBmpString(g_ColorMode[g_CurrentColor].Name);
	V9_PrintBmpString(")");

	// Setup cursors
	V9_SetCursorEnable(true);
	V9_SetCursorPalette(2); // entry 8
	u8* buf = (u8*)Mem_GetHeapAddress();
	loop(i, 128)
	{
		buf[i] = i & 0x04 ? 0xAA : 0x55; // Initialize 'check' pattern
	}
	loop(i, 2)
		V9_SetCursorPattern(i, buf);

	V9_SetInterrupt(V9_INT_VBLANK | V9_INT_HBLANK);
	V9_SetInterruptLine(9);
	V9_SetDisplayEnable(TRUE);
}

//
void TickBmp()
{
	if(g_CurrentColor == V9_COLOR_BP4)
		V9_SelectPaletteBP4(1);

	V9_SetScrollingX(0);

	if(Keyboard_IsKeyPressed(KEY_H))
	{
		if(g_CurrentColor == V9_COLOR_BP4)
			V9_SelectPaletteBP4(0);

		V9_SetScrollingX(0);
		V9_SetScrollingY(BMP_TILES_Y);
		V9_SetInterrupt(V9_INT_VBLANK);
	}
	else
	{
		V9_SetScrollingY(0);
		V9_SetInterrupt(V9_INT_VBLANK | V9_INT_HBLANK);
	}

	loop(i, 2)
	{
		u8 idx = ((g_Frame >> 2) + (i * 32)) % 64;
		V9_SetCursorAttribute(i, 64 + i * 96, 64 + ((i16)g_Sinus64[idx] / 8), i + 1);
	}

	g_Frame++;
}

//
void HblankBmp()
{
	if(g_CurrentColor == V9_COLOR_BP4)
		V9_SelectPaletteBP4(0);

	V9_SetScrollingX(g_Frame >> 0);
}

#endif // ((V9_USE_MODE_B0) || (V9_USE_MODE_B1) || (V9_USE_MODE_B2) || (V9_USE_MODE_B3) || (V9_USE_MODE_B4) || (V9_USE_MODE_B5) || (V9_USE_MODE_B6) || (V9_USE_MODE_B7))

//-----------------------------------------------------------------------------
// Helper function
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
void DisplayMSX()
{
	Print_SetTextFont(g_Font_MGL_Sample6, 1); // Initialize font
	Print_SetColor(0xF, 0x0);
	Print_DrawText(MSX_GL " V9990 SAMPLE");
	Print_DrawLineH(0, 1, 40);

	const c8* str = "V9990 not found!";
	if(V9_Detect())
		str = "V9990 found!";
	Print_SetPosition(0, 3);
	Print_DrawText(str);

	Print_SetPosition(0, 5);
	Print_DrawText("Ports:\n\n 0-7\n 8-F");
	for(u8 i = 0; i < 16; ++i)
	{
		u8 val = V9_GetPort(V9_P00 + i);
		u8 x = 9 + (i % 8) * 3;
		u8 y = 7 + (i / 8);
		Print_SetPosition(x, y);
		Print_DrawHex8(val);
	}

	Print_SetPosition(0, 10);
	Print_DrawText("Registers:\n\n 00-07\n 08-0F\n 10-17\n 18-1F\n 20-27\n 28-2F\n 30-37\n 38-3F");
	for(u8 i = 0; i < 64; ++i)
	{
		u8 val = V9_GetRegister(i);
		u8 x = 9 + (i % 8) * 3;
		u8 y = 12 + (i / 8);
		Print_SetPosition(x, y);
		Print_DrawHex8(val);
	}

	Print_DrawLineH(0, 22, 40);
	Print_SetPosition(0, 23);
	Print_DrawText("R:Refresh C:Color");

}

bool g_VSynch = FALSE;

#if (CUSTOM_ISR == 0)
//-----------------------------------------------------------------------------
//
void InterruptHook()
{
__asm
	// Flush VDP interruption signal
	in		a, (P_VDP_STAT)

	// Call VDP interruption handler
	in		a, (V9_P06)
	out		(V9_P06), a
	// V-Blank interruption
	rra
	call	c, _V9_InterruptVBlank
	// H-Blank interruption
	rra
	call	c, _V9_InterruptHBlank
	// Command end interruption
	rra
	call	c, _V9_InterruptCommand
__endasm;
}
#endif

//-----------------------------------------------------------------------------
//
void V9_InterruptVBlank()
{
	g_VSynch = TRUE;
}

//-----------------------------------------------------------------------------
//
void V9_InterruptHBlank()
{
	g_ScreenMode[g_CurrentScreen].Hblank();
}

//-----------------------------------------------------------------------------
//
void V9_InterruptCommand()
{
}

//-----------------------------------------------------------------------------
//
void VDP_InterruptVBlank()
{
}

//-----------------------------------------------------------------------------
// Set current screen mode
void SetScreenMode(u8 mode)
{
	g_CurrentScreen = mode;
	g_ScreenMode[g_CurrentScreen].Init();
}

//-----------------------------------------------------------------------------
// Set current bitmap color mode
void SetColorMode(u8 mode)
{
#if (V9_USE_MODE_P1)
	if(g_CurrentScreen == MODE_P1)
		return;
#endif
#if (V9_USE_MODE_P2)
	if(g_CurrentScreen == MODE_P2)
		return;
#endif

	g_CurrentColor = mode;
	g_ScreenMode[g_CurrentScreen].Init();
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	V9_SetPort(V9_P15, 0); // Disactivate Video9000 if present (recommanded)

	// Initialize screen mode 0 (text)
	VDP_SetMode(VDP_MODE_SCREEN0);
	VDP_EnableVBlank(FALSE);
	VDP_ClearVRAM();

	DisplayMSX();

	#if (CUSTOM_ISR == 0)
		Bios_SetHookDirectCallback(H_KEYI, InterruptHook);
		Bios_ClearHook(H_TIMI);
	#endif

	//---- CURRENT SCREEN MODE INIT ----
	g_ScreenMode[g_CurrentScreen].Init();

	loop(i, 80)
		g_KeyPressed[i] = FALSE;

	u16 count = 0;
	u8 clr = 0;
	while(1)
	{
		// V9_SetRegister(15, 2);
		while(g_VSynch == FALSE) {}
		// V9_SetRegister(15, 1);
		g_VSynch = FALSE;

		//---- CURRENT SCREEN MODE UPDATE ----
		g_ScreenMode[g_CurrentScreen].Tick();

		// Sign of life
		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);

		// Handle inputs
		u8 row0 = Keyboard_Read(0);
		u8 row1 = Keyboard_Read(1);
		u8 row8 = Keyboard_Read(8);

		// Select next/previous screen mode
		if(IS_KEY_PRESSED(row8, KEY_RIGHT))
		{
			if(g_CurrentScreen < numberof(g_ScreenMode) - 1)
				SetScreenMode(g_CurrentScreen + 1);
		}
		else if(IS_KEY_PRESSED(row8, KEY_LEFT))
		{
			if(g_CurrentScreen > 0)
				SetScreenMode(g_CurrentScreen - 1);
		}

		// Direction selection of a screen mode
#if ((V9_USE_MODE_B0) || (V9_USE_MODE_B1) || (V9_USE_MODE_B2) || (V9_USE_MODE_B3) || (V9_USE_MODE_B4) || (V9_USE_MODE_B5) || (V9_USE_MODE_B6) || (V9_USE_MODE_B7))
		if(IS_KEY_PRESSED(row0, KEY_0))
			SetScreenMode(MODE_B0);
		else if(IS_KEY_PRESSED(row0, KEY_1))
			SetScreenMode(MODE_B1);
		else if(IS_KEY_PRESSED(row0, KEY_2))
			SetScreenMode(MODE_B2);
		else if(IS_KEY_PRESSED(row0, KEY_3))
			SetScreenMode(MODE_B3);
		else if(IS_KEY_PRESSED(row0, KEY_4))
			SetScreenMode(MODE_B4);
		else if(IS_KEY_PRESSED(row0, KEY_5))
			SetScreenMode(MODE_B5);
		else if(IS_KEY_PRESSED(row0, KEY_6))
			SetScreenMode(MODE_B6);
		else if(IS_KEY_PRESSED(row0, KEY_7))
			SetScreenMode(MODE_B7);
#endif
#if (V9_USE_MODE_P1)
		if(IS_KEY_PRESSED(row1, KEY_8))
			SetScreenMode(MODE_P1);
#endif
#if (V9_USE_MODE_P2)
		if(IS_KEY_PRESSED(row1, KEY_9))
			SetScreenMode(MODE_P2);
#endif

		// Select color mode
		if(IS_KEY_PRESSED(row8, KEY_DOWN))
		{
			if(g_CurrentColor < numberof(g_ColorMode) - 1)
				SetColorMode(g_CurrentColor + 1);
		}
		else if(IS_KEY_PRESSED(row8, KEY_UP))
		{
			if(g_CurrentColor > 0)
				SetColorMode(g_CurrentColor - 1);			
		}

		// Hide/display sprite/cursor
		if(Keyboard_IsKeyPressed(KEY_G))
		{
			if(!g_KeyPressed[KEY_G])
			{
				g_KeyPressed[KEY_G] = TRUE;
				g_HideSprite = !g_HideSprite;
				if(g_HideSprite)
					V9_SetFlag(8, V9_R08_SPD_OFF, V9_R08_SPD_OFF);
				else
					V9_SetFlag(8, V9_R08_SPD_OFF, 0);
			}
		}
		else
			g_KeyPressed[KEY_G] = FALSE;

		// Refresh MSX display
		if(Keyboard_IsKeyPressed(KEY_R))
			DisplayMSX();
	}

	V9_SetPort(V9_P15, 0x10); // Reactivate Video9000 if present (recommanded)
}