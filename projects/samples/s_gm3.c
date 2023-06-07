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

#define SPLIT_LINE (106 - 5)

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

const u8 g_BallColor[8] = { COLOR_WHITE, COLOR_LIGHT_GREEN, COLOR_LIGHT_RED, COLOR_LIGHT_YELLOW, COLOR_LIGHT_BLUE, COLOR_MEDIUM_GREEN, COLOR_MEDIUM_RED, COLOR_MAGENTA };

const u8 g_DataSprtBall[] =
{
// ======== Frame[0]
// ---- Layer[0] (16x16 0,0 1,1 inc 2)
// Sprite[0] (offset:0)
	0x07, /* .....### */ 
	0x1F, /* ...##### */ 
	0x3F, /* ..###### */ 
	0x7F, /* .####### */ 
	0x7F, /* .####### */ 
	0xFF, /* ######## */ 
	0xFF, /* ######## */ 
	0xFF, /* ######## */ 
// Sprite[1] (offset:8)
	0xFF, /* ######## */ 
	0xFF, /* ######## */ 
	0xFF, /* ######## */ 
	0x7F, /* .####### */ 
	0x7F, /* .####### */ 
	0x3F, /* ..###### */ 
	0x1F, /* ...##### */ 
	0x07, /* .....### */ 
// Sprite[2] (offset:16)
	0xE0, /* ###..... */ 
	0xF8, /* #####... */ 
	0xFC, /* ######.. */ 
	0xFE, /* #######. */ 
	0xFE, /* #######. */ 
	0xFF, /* ######## */ 
	0xFF, /* ######## */ 
	0xFF, /* ######## */ 
// Sprite[3] (offset:24)
	0xFF, /* ######## */ 
	0xFF, /* ######## */ 
	0xFF, /* ######## */ 
	0xFE, /* #######. */ 
	0xFE, /* #######. */ 
	0xFC, /* ######.. */ 
	0xF8, /* #####... */ 
	0xE0, /* ###..... */ 
};

//
const u16 g_SATAddr[] = { 0x1E00, 0x4200 };

//=============================================================================
// MEMORY DATA
//=============================================================================

struct VDP_Sprite g_SpriteData[29*2];

// Screen mode setting index
u8 g_VBlank = 0;
u8 g_Frame = 0;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void VDP_InterruptHandler() // VBlank
{
	g_VBlank = 1;
}

//-----------------------------------------------------------------------------
// Wait for V-Blank period
void WaitVBlank()
{
	while(g_VBlank == 0) {}
	g_VBlank = 0;
	g_Frame++;
	VDP_SetSpriteAttributeTable(g_SATAddr[0]);
	VDP_SetColor(COLOR_DARK_BLUE);
}


//-----------------------------------------------------------------------------
//
void VDP_HBlankHandler()
{
	VDP_SetSpriteAttributeTable(g_SATAddr[1]);
	VDP_SetColor(COLOR_LIGHT_BLUE);
}

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
	VDP_LoadSpritePattern(g_DataSprtBall, 0, 4);
	VDP_LoadSpritePattern(g_DataSprtLayer, 8*4, 13*4*4);

	loop(s, 2)
	{
		g_SpriteAtributeLow = g_SATAddr[s];
		g_SpriteAtributeHigh = 0;
		g_SpriteColorLow = g_SpriteAtributeLow - 0x200;
		g_SpriteColorHigh = 0;

		VDP_SetSpriteExUniColor(0, X, Y, 32, COLOR_BLACK);
		VDP_SetSpriteExUniColor(1, X, Y, 36, COLOR_WHITE);
		VDP_SetSpriteExUniColor(2, X, Y, 40, COLOR_LIGHT_RED);

		for(u8 i = 3; i < 32; ++i)
		{
			u8 idx = i - 3 + (s * 29);
			struct VDP_Sprite* sprt = &g_SpriteData[idx];
			sprt->X = Math_GetRandom8();
			sprt->Y = (106 - 16) * (i - 3) / 29 /*Math_GetRandom8() % (106 - 16)*/ + (s * 106);
			sprt->Pattern = 0;
			sprt->Color = g_BallColor[Math_GetRandom8() % 8];
			VDP_SetSpriteExUniColor(i, sprt->X, sprt->Y, sprt->Pattern, sprt->Color);
		}
	}

	// Interruption
	VDP_EnableVBlank(TRUE);
	VDP_EnableHBlank(TRUE);

	u8 shape = 6;
	u8 prevRow8 = 0xFF;
	while(1)
	{
		// Wait for v-synch
		WaitVBlank();
		VDP_SetHBlankLine(SPLIT_LINE + g_Frame);
		VDP_SetVerticalOffset(g_Frame);

		loop(s, 2)
		{
			// Update SAT address
			g_SpriteAtributeLow = g_SATAddr[s];
			g_SpriteAtributeHigh = 0;
			g_SpriteColorLow = g_SpriteAtributeLow - 0x200;
			g_SpriteColorHigh = 0;

			// Update player sprites
			u8 y = Y + g_Frame;
			if(y == 216)
				y++;
			VDP_SetSprite(0, X, y, 32 + shape * 16);
			VDP_SetSprite(1, X, y, 36 + shape * 16);
			VDP_SetSprite(2, X, y, 40 + shape * 16);

			// Update balloon sprites
			for(u8 i = 3; i < 32; ++i)
			{
				u8 idx = i - 3;
				if(s)
					idx += 29;
				struct VDP_Sprite* sprt = &g_SpriteData[idx];
				u8 y = sprt->Y + g_Frame;
				if(y == 216)
					y++;
				VDP_SetSpritePositionY(i, y);
			}
		}

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
			shape = (g_Frame >> 2) % 6;
		}
		else if(IS_KEY_PRESSED(row8, KEY_LEFT))
		{
			X--;
			shape = (g_Frame >> 2) % 6;
		}
		if(IS_KEY_PRESSED(row8, KEY_DOWN))
		{
			Y++;
			shape = (g_Frame >> 2) % 6;
		}
		else if(IS_KEY_PRESSED(row8, KEY_UP))
		{
			Y--;
			shape = (g_Frame >> 2) % 6;
		}
		prevRow8 = row8;
	}
}