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

// Screen mode structure
struct ScreenMode
{
	const c8* Name;
	u8        Mode;
	callback  Init;
	callback  Tick;
	u8        BPP;
};

// Function prototype
void InitP1();  void TickP1();
void InitP2();  void TickP2();
void InitBmp(); void TickBmp();

// Bits per pixel
#define BPP_2						0x02
#define BPP_4						0x04
#define BPP_8						0x08
#define BPP_16						0x10

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// 1-bit font (VDP)
#include "font\font_mgl_sample6.h"

// 4-bits font (V9990)
#include "content\v9990\data_v9_font.h"

// Sprite
#include "content\v9990\data_v9_chr.h"

// Tiles
#include "content\v9990\data_v9_bg.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Screen mode configuration
const struct ScreenMode g_ScreenMode[] =
{
	{ "P1", V9_MODE_P1, InitP1,  TickP1,  BPP_4 },
	{ "P2", V9_MODE_P2, InitP2,  TickP2,  BPP_4 },
	{ "B0", V9_MODE_B0, InitBmp, TickBmp, BPP_2|BPP_4|BPP_8|BPP_16 },
	{ "B1", V9_MODE_B1, InitBmp, TickBmp, BPP_2|BPP_4|BPP_8|BPP_16 },
	{ "B2", V9_MODE_B2, InitBmp, TickBmp, BPP_2|BPP_4|BPP_8|BPP_16 },
	{ "B3", V9_MODE_B3, InitBmp, TickBmp, BPP_2|BPP_4|BPP_8|BPP_16 },
	{ "B4", V9_MODE_B4, InitBmp, TickBmp, BPP_2|BPP_4 },
	{ "B5", V9_MODE_B5, InitBmp, TickBmp, BPP_2|BPP_4 },
	{ "B6", V9_MODE_B6, InitBmp, TickBmp, BPP_2|BPP_4 },
	{ "B7", V9_MODE_B7, InitBmp, TickBmp, BPP_2|BPP_4 },
};

// Black color
const u8 g_ColorBlack[] = { 0, 0, 0 };

//=============================================================================
// MEMORY DATA
//=============================================================================

u8  g_CurrentMode = 3;
u8  g_CurrentBPP = BPP_4;
u16 g_Frame = 0;

//=============================================================================
// CODE
//=============================================================================

//-----------------------------------------------------------------------------
// P1
//-----------------------------------------------------------------------------

#define P1_GROUND_Y		20
#define P1_HORIZON_Y	14

void V9_Print(u32 addr, const c8* str)
{
	while(*str != 0)
		V9_Poke16(addr += 2, 512 + *(str++) - ' ');
}

//
void InitP1()
{
	// V9_ClearVRAM();
	V9_SetMode(V9_MODE_P1);
	V9_SetSpriteEnable(TRUE);
	V9_SetBackgroundColor(6);

	V9_SetInterrupt(V9_INT_VBLANK | V9_INT_HBLANK);
	V9_SetInterruptLine(71);

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
		for(u8 j = y; j < 20; j++)
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
		for(u8 j = 20; j < 27; j++)
		{
			u8 cell;
			if(j == 20)
				cell = 33;
			else
				cell = (j & 1) ? 65 : 97;
			V9_Poke16(V9_CellAddrP1A(0, j) + i*2, cell + i%2);
		}
	}

	V9_FillVRAM16(V9_P1_PNT_B, 160, 64*27); // Init layer B
	for(u16 i = 0; i < 64; ++i) // Draw horizon
		V9_Poke16(V9_CellAddrP1B(0, P1_HORIZON_Y) + i*2, 128 + i%4);
	V9_FillVRAM16(V9_P1_PNT_B, 135, 64*P1_HORIZON_Y); // Draw sky
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
}

//
void TickP1()
{
	V9_SetLayerPalette(2, 0);
	V9_SetScrollingX(0);
	V9_SetScrollingBX(g_Frame >> 1);

	u8 frame = (g_Frame >> 2) % 6;
	for(u16 i = 0; i < 125; ++i)
		V9_SetSpritePatternP1(i, frame);

	g_Frame++;
}

//-----------------------------------------------------------------------------
// P2
//-----------------------------------------------------------------------------

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
	// V9_ClearVRAM();
	V9_SetMode(V9_MODE_P2);
	V9_SetSpriteEnable(TRUE);
	V9_SetBackgroundColor(6);

	V9_SetInterrupt(V9_INT_VBLANK | V9_INT_HBLANK);
	V9_SetInterruptLine(6);

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
	V9_FillVRAM16(V9_P2_PNT + 512, 135, 128*P1_HORIZON_Y); // Draw sky
	for(u16 i = 0; i < 128; ++i) // Draw horizon
		V9_Poke16(V9_CellAddrP2(0, P1_HORIZON_Y) + i*2, 128 + i%4);
	V9_FillVRAM16(V9_CellAddrP2(0, P1_HORIZON_Y+1), 160, (27-P1_HORIZON_Y)*128); // Draw sea
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
		for(u8 j = y; j < 20; j++)
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
		for(u8 j = 20; j < 27; j++)
		{
			u8 cell;
			if(j == 20)
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
}

//
void TickP2()
{
	V9_SetLayerPalette(2, 2);
	V9_SetScrollingX(0);

	u8 frame = (g_Frame >> 2) % 6;
	for(u16 i = 0; i < 125; ++i)
		V9_SetSpritePatternP2(i, frame);

	g_Frame++;
}

//-----------------------------------------------------------------------------
// Bitmap
//-----------------------------------------------------------------------------

//
void InitBmp()
{
	// V9_ClearVRAM();
	V9_SetMode(g_ScreenMode[g_CurrentMode].Mode);
	V9_SetBPP(V9_R06_BPP_4);
	V9_SetImageSpaceWidth(V9_R06_WIDH_1024);
	V9_SetBackgroundColor(6);

	V9_SetInterrupt(V9_INT_VBLANK);
	// V9_SetInterrupt(V9_INT_VBLANK | V9_INT_HBLANK);
	// V9_SetInterruptLine(8);

	V9_SetLayerPalette(0, 0);
	V9_SetScrollingX(0);

	for(u8 i = 0; i < 48; ++i)
		V9_WriteVRAM((u32)(0 + (u32)(512 * (u32)(512 + 0 + i))), g_DataV9BG + (128 * i), 128);

	for(u8 i = 0; i < 24; ++i)
		V9_WriteVRAM((u32)(0 + (u32)(512 * (u32)(512 + 48 + i))), g_DataV9Font + (128 * i), 128);

	for(u8 i = 0; i < 16; ++i)
		V9_WriteVRAM((u32)(0 + (u32)(512 * (u32)(512 + 72 + i))), g_DataV9Chr + (128 * i), 128);

	V9_SetCommandLogicalOp(V9_R45_LOP_SET);
	V9_SetCommandWriteMask(0xFFFF);
	V9_WaitCmdEnd();
	V9_CommandLMMV(0, 0, 1024, 256, 0, 0x0000);
	for(u8 i = 0; i < 64; ++i)
	{
		V9_WaitCmdEnd();
		V9_CommandLMMM(8, 512 + 8, 16 * i, 160, 16, 24, 0);
		for(u8 j = 0; j < 4; ++j)
		{
			V9_WaitCmdEnd();
			V9_CommandLMMM(8, 512 + 16, 16 * i, 160+24+16*j, 16, 16, 0);
		}
	}
}

//
void TickBmp()
{
	// V9_SetLayerPalette(2, 2);
	V9_SetScrollingX(0);

	if(Keyboard_IsKeyPressed(KEY_H))
		V9_SetScrollingY(512);
	else
		V9_SetScrollingY(0);

	g_Frame++;
}

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
	V9_SetLayerPalette(0, 0);
	V9_SetScrollingX(g_Frame >> 0);
	V9_SetScrollingBX(g_Frame >> 2);
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

	V9_SetPaletteEntry(0, g_ColorBlack);
	V9_SetPalette(1,  15, g_DataV9BG_palette);

	V9_SetPaletteEntry(16, g_ColorBlack);
	V9_SetPalette(17, 15, g_DataV9Chr_palette);

	V9_SetPaletteEntry(32, g_ColorBlack);
	V9_SetPalette(33, 15, g_DataV9Font_palette);

	#if (CUSTOM_ISR == 0)
		Bios_SetHookDirectCallback(H_KEYI, InterruptHook);
		Bios_ClearHook(H_TIMI);
	#endif
	g_ScreenMode[g_CurrentMode].Init();
	V9_SetRegister(15, 1);

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