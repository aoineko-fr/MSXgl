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

//=============================================================================
// DEFINES
//=============================================================================

#define CUSTOM_ISR					0

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

#define V9_GROUND_Y		20
#define V9_HORIZON_Y	14

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

// Function prototype
void InitP1();  void TickP1();  void HblankP1();
void InitP2();  void TickP2();  void HblankP2();
void InitBmp(); void TickBmp(); void HblankBmp();

// Bits per pixel
#define BPP_2						0x02
#define BPP_4						0x04
#define BPP_8						0x08
#define BPP_16						0x10

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

// Animation characters
const u8 g_ChrAnim[4] = { '|', '\\', '-', '/' };

// Black color
const u8 g_ColorBlack[] = { 0, 0, 0 };

// Animation characters
const u8 g_BppModes[4] = { BPP_2, BPP_4, BPP_8, BPP_16 };

// Screen mode configuration
const struct ScreenMode g_ScreenMode[] =
{
#if (V9_USE_MODE_P1)
	{ "P1", V9_MODE_P1, InitP1,  TickP1,  HblankP1,  BPP_4, 256, 212 },
#endif
#if (V9_USE_MODE_P2)
	{ "P2", V9_MODE_P2, InitP2,  TickP2,  HblankP2,  BPP_4, 512, 212 },
#endif
#if (V9_USE_MODE_B0)
	{ "B0", V9_MODE_B0, InitBmp, TickBmp, HblankBmp, BPP_2|BPP_4|BPP_8|BPP_16, 192, 240 },
#endif
#if (V9_USE_MODE_B1)
	{ "B1", V9_MODE_B1, InitBmp, TickBmp, HblankBmp, BPP_2|BPP_4|BPP_8|BPP_16, 256, 212 },
#endif
#if (V9_USE_MODE_B2)
	{ "B2", V9_MODE_B2, InitBmp, TickBmp, HblankBmp, BPP_2|BPP_4|BPP_8|BPP_16, 384, 240 },
#endif
#if (V9_USE_MODE_B3)
	{ "B3", V9_MODE_B3, InitBmp, TickBmp, HblankBmp, BPP_2|BPP_4|BPP_8|BPP_16, 512, 212 },
#endif
#if (V9_USE_MODE_B4)
	{ "B4", V9_MODE_B4, InitBmp, TickBmp, HblankBmp, BPP_2|BPP_4, 768, 240 },
#endif
#if (V9_USE_MODE_B5)
	{ "B5", V9_MODE_B5, InitBmp, TickBmp, HblankBmp, BPP_2|BPP_4, 640, 400 },
#endif
#if (V9_USE_MODE_B6)
	{ "B6", V9_MODE_B6, InitBmp, TickBmp, HblankBmp, BPP_2|BPP_4, 650, 480 },
#endif
#if (V9_USE_MODE_B7)
	{ "B7", V9_MODE_B7, InitBmp, TickBmp, HblankBmp, BPP_2|BPP_4, 1024, 212 },
#endif
};

//=============================================================================
// MEMORY DATA
//=============================================================================

u8   g_CurrentMode = 0;
u8   g_CurrentBPP = BPP_4;
u16  g_Frame = 0;
bool g_Interlaced = false;
u16  g_TextX;
u16  g_TextY;

//=============================================================================
// CODE
//=============================================================================

void InitPalette()
{
	switch (g_ScreenMode[g_CurrentMode].Mode)
	{
	case V9_MODE_P1:
	case V9_MODE_P2:
	case V9_MODE_B0:
	case V9_MODE_B1:
	case V9_MODE_B2:
	case V9_MODE_B3:
		V9_SetPaletteEntry(0, g_ColorBlack);
		V9_SetPalette(1,  15, g_DataV9BG_palette);
		V9_SetPaletteEntry(16, g_ColorBlack);
		V9_SetPalette(17, 15, g_DataV9Chr_palette);
		V9_SetPaletteEntry(32, g_ColorBlack);
		V9_SetPalette(33, 15, g_DataV9Font_palette);
		break;
	case V9_MODE_B4:
	case V9_MODE_B5:
	case V9_MODE_B6:
	case V9_MODE_B7:
		loop(i, 2)
		{
			V9_SetPaletteEntry(0 + i * 32, g_ColorBlack);
			V9_SetPalette(1 + i * 32,  15, g_DataV9BG_palette);
			V9_SetPaletteEntry(16 + i * 32, g_ColorBlack);
			V9_SetPalette(17 + i * 32, 15, g_DataV9Chr_palette);
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// P1
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
	V9_SetMode(V9_MODE_P1);
	V9_SetBackgroundColor(6);
	V9_SetDisplayEnable(FALSE);
	V9_SetSpriteEnable(TRUE);
	V9_SetInterrupt(V9_INT_VBLANK | V9_INT_HBLANK);
	V9_SetInterruptLine(71);

	InitPalette();

	// Pattern generator table
	V9_WriteVRAM(V9_P1_PGT_A, g_DataV9BG, sizeof(g_DataV9BG));
	V9_WriteVRAM(V9_P1_PGT_A + 0x04000, g_DataV9Font, sizeof(g_DataV9Font));
	V9_WriteVRAM(V9_P1_PGT_B, g_DataV9BG, sizeof(g_DataV9BG));

	// Pattern name table
	V9_FillVRAM16(V9_P1_PNT_A, 0, 64*64); // Init layer A
	V9_Print(V9_P1_PNT_A, "MSXgl V9990 Sample - P1 Mode");
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
// P2
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
	V9_SetMode(V9_MODE_P2);
	V9_SetBackgroundColor(6);
	V9_SetDisplayEnable(FALSE);
	V9_SetSpriteEnable(TRUE);
	V9_SetInterrupt(V9_INT_VBLANK | V9_INT_HBLANK);
	V9_SetInterruptLine(6);

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
	V9_Print(V9_P2_PNT, "MSXgl V9990 Sample - P2 Mode"); // Draw title
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
// Bitmap
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
	V9_CommandLMMM(idx % 32 * 8, 512 + 48 + (idx / 32) * 8, g_TextX, g_TextY, 8, 8, 0);
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
	V9_CommandLMMM((tile % 32) * 8, 512 + (tile / 32) * 8, x * 8, y * 8, 8, 8, 0);
}

//
void InitBmp()
{
	// Initialize screen mode
	V9_SetMode(g_ScreenMode[g_CurrentMode].Mode);
	V9_SetBackgroundColor(6);
	V9_SetDisplayEnable(FALSE);
	V9_SetBPP(V9_R06_BPP_4);
	V9_SetImageSpaceWidth(V9_R06_WIDH_1024);
	V9_SetInterrupt(V9_INT_VBLANK | V9_INT_HBLANK);
	V9_SetInterruptLine(8);

	InitPalette();

	V9_SelectPaletteBP4(0);
	V9_SetScrollingX(0);

	for(u8 i = 0; i < 48; ++i)
		V9_WriteVRAM((u32)(0 + (u32)(512 * (u32)(512 + 0 + i))), g_DataV9BG + (128 * i), 128);

	for(u8 i = 0; i < 24; ++i)
		V9_WriteVRAM((u32)(0 + (u32)(512 * (u32)(512 + 48 + i))), g_DataV9Font + (128 * i), 128);

	for(u8 i = 0; i < 16; ++i)
		V9_WriteVRAM((u32)(0 + (u32)(512 * (u32)(512 + 72 + i))), g_DataV9Chr + (128 * i), 128);

	V9_SetCommandLogicalOp(V9_R45_LOP_SET);//| V9_R45_TP);
	V9_SetCommandWriteMask(0xFFFF);
	V9_WaitCmdEnd();
	V9_CommandLMMV(0, 0, 1024, 256, 0, 0x6666);

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

	V9_PrintBmpStringAt(4, 1, "MSXgl V9990 Sample - ");
	V9_PrintBmpString(g_ScreenMode[g_CurrentMode].Name);

	// Cursor
	V9_SetCursorEnable(0, FALSE);
	V9_SetCursorEnable(1, FALSE);

	V9_SetDisplayEnable(TRUE);
}

//
void TickBmp()
{
	V9_SelectPaletteBP4(2);
	V9_SetScrollingX(0);

	if(Keyboard_IsKeyPressed(KEY_H))
	{
		V9_SelectPaletteBP4(0);
		V9_SetScrollingX(0);
		V9_SetScrollingY(512);
		V9_SetInterrupt(V9_INT_VBLANK);
	}
	else
	{
		V9_SetScrollingY(0);
		V9_SetInterrupt(V9_INT_VBLANK | V9_INT_HBLANK);
	}

	g_Frame++;
}

//
void HblankBmp()
{
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
	g_ScreenMode[g_CurrentMode].Hblank();
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
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_EnableVBlank(FALSE);
	VDP_ClearVRAM();

	DisplayMSX();

	#if (CUSTOM_ISR == 0)
		Bios_SetHookDirectCallback(H_KEYI, InterruptHook);
		Bios_ClearHook(H_TIMI);
	#endif
	g_ScreenMode[g_CurrentMode].Init();

	u16 count = 0;
	u8 clr = 0;
	while(1)
	{
		// V9_SetRegister(15, 2);
		while(g_VSynch == FALSE) {}
		// V9_SetRegister(15, 1);
		g_VSynch = FALSE;

		g_ScreenMode[g_CurrentMode].Tick();

		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);

		if(Keyboard_IsKeyPressed(KEY_R))
			DisplayMSX();

		u8 row8 = Keyboard_Read(8);
		// Update horizontal scrolling offset
		if(IS_KEY_PRESSED(row8, KEY_RIGHT))
		{
			if(g_CurrentMode < numberof(g_ScreenMode) - 1)
			{
				g_CurrentMode++;
				g_ScreenMode[g_CurrentMode].Init();
			}
		}
		else if(IS_KEY_PRESSED(row8, KEY_LEFT))
		{
			if(g_CurrentMode > 0)
			{
				g_CurrentMode--;
				g_ScreenMode[g_CurrentMode].Init();
			}
		}
		// Update vertical scrolling offset
		if(IS_KEY_PRESSED(row8, KEY_DOWN))
		{
			
		}
		else if(IS_KEY_PRESSED(row8, KEY_UP))
		{
			
		}
	}
}