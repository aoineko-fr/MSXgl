// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Multi-directionnal scrolling sample
//
// Scrolling parameters (source data format, output window and used fetaures) 
// are defined in  msxgl_config.h for optimization purpose (the 'scroll' module 
// is optimized at compile time to useonly necessary features).
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "math.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x02\x03\x04\x05"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample8.h"
// Tiles
#include "content/tile/data_tile_gm2.h"
#include "content/tile/data_map_gm2.h"
// Sprites by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "content/data_sprt_layer.h"

const u8 g_Cloud1[] = { 1, 2, 3, 4, 5 };
const u8 g_Cloud2[] = { 6, 7 };

//=============================================================================
// MEMORY DATA
//=============================================================================

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	Bios_SetKeyClick(FALSE);

	// Initialize video
	VDP_SetMode(VDP_MODE_GRAPHIC3_MIRROR); // Screen mode 4 (G3) with mirrored pattern/color table
	VDP_EnableVBlank(TRUE);
	VDP_SetLineCount(VDP_LINE_212);
	VDP_SetColor(0xF5);
	VDP_ClearVRAM();

	// Generate tiles data
	//-- Bank 0
	u16 bankAddr = 0;
	VDP_WriteVRAM_16K(g_Font_MGL_Sample8 + 4 + 8 * 10,  g_ScreenPatternLow + 8 + bankAddr, 8);
	VDP_FillVRAM_16K(0x3C, g_ScreenColorLow + bankAddr, 256 * 8);
	//-- Bank 1
	bankAddr += 256 * 8;
	VDP_WriteVRAM_16K(g_Font_MGL_Sample8 + 4 + 8 * 185, g_ScreenPatternLow + 8 + bankAddr, 8);
	VDP_FillVRAM_16K(0x74, g_ScreenColorLow + bankAddr, 256 * 8);
	//-- Bank 2
	bankAddr += 256 * 8;
	VDP_WriteVRAM_16K(g_Font_MGL_Sample8 + 4 + 8 * 156, g_ScreenPatternLow + 8 + bankAddr, 8);
	VDP_FillVRAM_16K(0x96, g_ScreenColorLow + bankAddr, 256 * 8);

	// Generate layout
	u16 addr = g_ScreenLayoutLow;
	for(u16 i = 0; i < 1024; ++i)
	{
		if((i >= 32 * 3) && !(i & 0x20))
			VDP_Poke_16K(i & 0x01, addr);
		addr++;
	}

	// Initialize text
	g_PrintData.PatternOffset = 32;
	Print_SetFontEx(8, 8, 1, 1, 32, 127, g_Font_MGL_Sample8 + 32 * 8 + 4);
	Print_Initialize();
	Print_SetMode(PRINT_MODE_TEXT);
	VDP_WriteVRAM(g_PrintData.FontPatterns, g_ScreenPatternLow + (32 * 8), 0, g_PrintData.CharCount * 8); // Load data to VRAM

	// Print header
	Print_DrawTextAt(0, 0, "MSXgl - GM3 Sample");
	Print_DrawTextAt(0, 1, " [1]:Mirror 0    [2]:Mirror 01");
	Print_DrawTextAt(0, 2, " [3]:Mirror 02   [4]:No mirror");

	// Initialize sprite
	u8 X = 100, Y = 100;
	VDP_EnableSprite(TRUE);
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);
	VDP_LoadSpritePattern(g_DataSprtLayer, 32, 13*4*4);
	VDP_SetSpriteExUniColor(0, X, Y, 32, COLOR_BLACK);
	VDP_SetSpriteExUniColor(1, X, Y, 36, COLOR_WHITE);
	VDP_SetSpriteExUniColor(2, X, Y, 40, COLOR_LIGHT_RED);
	VDP_DisableSpritesFrom(3);

	u8 frame = 0;
	u8 shape = 6;
	u8 prevRow8 = 0xFF;
	while(1)
	{
		// Wait for v-synch
		Halt();
		VDP_SetVerticalOffset(frame);
		VDP_SetSprite(0, X, Y + frame, 32 + shape * 16);
		VDP_SetSprite(1, X, Y + frame, 36 + shape * 16);
		VDP_SetSprite(2, X, Y + frame, 40 + shape * 16);

		u8 row0 = Keyboard_Read(0);
		if(IS_KEY_PRESSED(row0, KEY_1))
			VDP_SetMode(VDP_MODE_GRAPHIC3_MIRROR_0);
		if(IS_KEY_PRESSED(row0, KEY_2))
			VDP_SetMode(VDP_MODE_GRAPHIC3_MIRROR_01);
		if(IS_KEY_PRESSED(row0, KEY_3))
			VDP_SetMode(VDP_MODE_GRAPHIC3_MIRROR_02);
		if(IS_KEY_PRESSED(row0, KEY_4))
			VDP_SetMode(VDP_MODE_GRAPHIC3);

		shape = 6;
		u8 row8 = Keyboard_Read(8);
		// Update horizontal scrolling offset
		if(IS_KEY_PRESSED(row8, KEY_RIGHT))
		{
			X++;
			shape = (frame >> 2) % 6;
		}
		else if(IS_KEY_PRESSED(row8, KEY_LEFT))
		{
			X--;
			shape = (frame >> 2) % 6;
		}
		if(IS_KEY_PRESSED(row8, KEY_DOWN))
		{
			Y++;
			shape = (frame >> 2) % 6;
		}
		else if(IS_KEY_PRESSED(row8, KEY_UP))
		{
			Y--;
			shape = (frame >> 2) % 6;
		}
		prevRow8 = row8;

		frame++;
	}
}