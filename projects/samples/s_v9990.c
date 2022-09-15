// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "v9990.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font\font_mgl_sample6.h"

// Sprite
#include "content\v9990\data_v9_chr.h"

// Tiles
#include "content\v9990\data_v9_bg.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// MEMORY DATA
//=============================================================================


//=============================================================================
// MAIN LOOP
//=============================================================================

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

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_EnableVBlank(TRUE);
	VDP_ClearVRAM();

	DisplayMSX();

	// V9_SetMode(V9_MODE_B1);
	// V9_SetBPP(V9_R06_BPP_4);

	V9_SetMode(V9_MODE_P1);
	V9_SetSpriteEnable(TRUE);
	V9_SetSpritePatternAddr(V9_P1_SGT_08000);

	V9_ClearVRAM();

	V9_SetPalette(1,  numberof(g_DataV9BG_palette), g_DataV9BG_palette);
	V9_SetPalette(17, numberof(g_DataV9Chr_palette), g_DataV9Chr_palette);

	// V9_P1_PGT_A		0x00000	// Pattern Generator Table (Layer A). 8160 patterns max
	// V9_P1_SGT		0x00000	// Sprite Generator Table  
	// V9_P1_SPAT		0x3FE00	// Sprite Attribute Table
	// V9_P1_PGT_B		0x40000	// Pattern Generator Table (Layer B). 7680 patterns max
	// V9_P1_PNT_A		0x7C000	// Pattern Name Table (Layer A)
	// V9_P1_PNT_B		0x7E000	// Pattern Name Table (Layer B)

	V9_WriteVRAM(V9_P1_PGT_A, g_DataV9BG, sizeof(g_DataV9BG));
	V9_WriteVRAM(V9_P1_PGT_B, g_DataV9BG, sizeof(g_DataV9BG));

	V9_WriteVRAM(0x08000, g_DataV9Chr, sizeof(g_DataV9Chr));

	for(u16 i = 0; i < 32*6; i++)
	{
		u16 addr = (((i / 32) * 64) + (i % 32)) * 2;
		V9_Poke(V9_P1_PNT_A + addr, (u8)(i & 0xFF));
		addr++;
		V9_Poke(V9_P1_PNT_A + addr, (u8)(i >> 8));
	}

	struct V9_Sprite attr;
	for(u16 i = 0; i < 125; i++)
	{
		attr.Y = (i / 13) * 20;
		attr.Pattern = 0;//i % 13;
		attr.X = (i % 13) * 20;
		attr.P = 0;
		attr.D = 0;
		attr.SC = 1;
		V9_SetSpriteP1(i, &attr);
	}

	V9_FillVRAM(V9_P1_PNT_B, 0, 64*64*2);

	u16 count = 0;
	u8 clr = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();
		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);

		V9_SetScrollingX(count);
		V9_SetScrollingBY(count);

		u8 frame = (count >> 1) % 6;
		for(u16 i = 0; i < 125; i++)
			V9_SetSpritePatternP1(i, frame);

		// V9_SetPort(6, 0x07);
		// 

		if(Keyboard_IsKeyPressed(KEY_R))
			DisplayMSX();
		if(Keyboard_IsKeyPressed(KEY_C))
			V9_SetRegister(15, clr++ & 0xF);
	}
}