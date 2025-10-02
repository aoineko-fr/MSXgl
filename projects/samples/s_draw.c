// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Draw functiosn sample
//─────────────────────────────────────────────────────────────────────────────

// #pragma sdcc_hash +

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL6 "\x01\x02\x03\x04\x05\x06"
#define MSX_GL8 "\x02\x03\x04\x05 "

// Screen setting
struct ScreenSetting
{
	// Setup
	const c8*  Name;
	u8         Mode;
	u16        Width;
	u8         BPC;
	// Colors
	u8         Text;
	u8         Background;
	u8         Red;
	u8         White;
	u8         Gray;
	u8         Black;
	// Data
	const u8*  Font;
	const u8*  Data;
	const u8*  DataLMMC;
	const u16* Palette;
	const c8*  Title;
};

// Screen setting
struct PaletteInfo
{
	const u16* Data;
	const c8*  Name;
};

//
struct DrawData
{
	u16 x1, y1, x2, y2;
	u8 color;
};

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"
#include "font/font_mgl_sample8.h"

// Screen mode settings
const struct ScreenSetting g_Settings[] =
{ //  Name              Mode              Width BPC Txt   BG    Red   White Gray  Black Font                Data  DataLMMC Palette 
	{ "Screen 5 (G4)",	VDP_MODE_SCREEN5, 256,	4,	0xFF, 0x44, 0x88, 0xFF, 0x11, 0x11, g_Font_MGL_Sample6, NULL, NULL, NULL, MSX_GL6 }, // 0
	{ "Screen 6 (G5)",	VDP_MODE_SCREEN6, 512,	2,	0xFF, 0xAA, 0x55, 0xFF, 0xAA, 0x55, g_Font_MGL_Sample8, NULL, NULL, NULL, MSX_GL8 }, // 1
	{ "Screen 7 (G6)",	VDP_MODE_SCREEN7, 512,	4,	0xFF, 0x44, 0x88, 0xFF, 0x11, 0x11, g_Font_MGL_Sample8, NULL, NULL, NULL, MSX_GL8 }, // 2
	{ "Screen 8 (G7)",	VDP_MODE_SCREEN8, 256,	8,	0xFF, 0x47, 0x1C, 0xFF, 0x6D, 0x00, g_Font_MGL_Sample6, NULL, NULL, NULL, MSX_GL6 }, // 3
};

// Palette
#include "palette/pal_msx2.h"
#include "palette/pal_msx1.h"
#include "palette/pal_db16.h"
#include "palette/pal_sweetie.h"
#include "palette/pal_wad.h"
#include "palette/pal_c64.h"
#include "palette/pal_ega.h"
#include "palette/pal_intellivision.h"
#include "palette/pal_mac2.h"
#include "palette/pal_mo5.h"
#include "palette/pal_os2.h"
#include "palette/pal_risc.h"
#include "palette/pal_scv.h"
#include "palette/pal_vic20.h"

// Palette data
const struct PaletteInfo g_Palettes[] =
{
	{ g_PaletteMSX2,  "V9998" },
	{ g_PaletteMSX1,  "TMS9918" },
	{ g_PaletteDB16,  "DB16" },
	{ g_PaletteSweetie, "Sweetie" },
	{ g_PaletteWad,   "Wad" },
	{ g_PaletteC64,   "C64" },
	{ g_PaletteEGA,   "EGA" },
	{ g_PaletteIV,    "Intellivision" },
	{ g_PaletteMac2,  "Mac II" },
	{ g_PaletteMO5,   "MO5" },
	{ g_PaletteOS2,   "OS/2" },
	{ g_PaletteRisc,  "RISC" },
	{ g_PaletteSCV,   "SCV" },
	{ g_PaletteVIC20, "VIC-20" },
};

// Character animation
const u8 chrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// MEMORY DATA
//=============================================================================

// Screen mode setting index
u8 g_SrcModeIndex = 0;
u8 g_PalIndex = 1;
u8 g_VBlank = 0;
u8 g_Frame = 0;

// Draw data
struct DrawData g_Data;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Generate random data into a given frame
void RandomizeData(u16 minX, u16 minY)
{
	u16 rnd = Math_GetRandom16();
	g_Data.x1 = rnd >> 8;
	g_Data.x2 = rnd & 0x00FF;
	if (g_Settings[g_SrcModeIndex].Width == 256)
	{
		g_Data.x1 >>= 1;
		g_Data.x2 >>= 1;
	}
	g_Data.x1 += minX;
	g_Data.x2 += minX;
	
	rnd = Math_GetRandom16();
	g_Data.y1 = rnd >> 8;
	g_Data.y2 = rnd & 0x00FF;
	while (g_Data.y1 > 98)
		g_Data.y1 -= 98;
	while (g_Data.y2 > 98)
		g_Data.y2 -= 98;
	g_Data.y1 += minY;
	g_Data.y2 += minY;
	
	rnd = Math_GetRandom16();
	g_Data.color = rnd & 0x00FF;
	if (g_Data.color == 0)
		g_Data.color++;
}

//-----------------------------------------------------------------------------
// Display screen mode data
void DisplayPage()
{
	const struct ScreenSetting* src = &g_Settings[g_SrcModeIndex];
	const struct PaletteInfo* pal = &g_Palettes[g_PalIndex];
	
	u8* buffer = Mem_HeapAlloc(256);
	u16 blockWidth = 16 * 256 / src->Width;
	u16 blockBytes = 16 / 8 * src->BPC;
	u16 lineBytes = src->Width / 8 * src->BPC;
	u8 scale = 8 / src->BPC;
	u8 palWidth = src->Width / 32;
	u16 colNum = (src->BPC == 2) ? 4 : (src->BPC == 4) ? 16 : 256;
	
	//-------------------------------------------------------------------------
	// Init
	
	VDP_SetMode(src->Mode);
	VDP_EnableTransparency(FALSE);
	VDP_SetColor(src->Background);
	VDP_CommandHMMV(0, 0, src->Width, 212, src->Background);
	
	if (src->BPC == 2)
	{
		VDP_SetPaletteEntry(0, RGB16(0, 0, 0));
		VDP_SetPaletteEntry(1, RGB16(1, 1, 2));
		VDP_SetPaletteEntry(2, RGB16(2, 2, 4));
		VDP_SetPaletteEntry(3, RGB16(3, 3, 7));
	}
	else if (src->BPC == 4)
		VDP_SetPalette(pal->Data);

	VDP_EnableSprite(FALSE);

	Print_SetBitmapFont(src->Font);
	Print_SetColor(src->Text, src->Background);
	
	Print_SetPosition(2, 2);
	Print_DrawFormat("%s DRAW SAMPLE \x8DMode:%s", src->Title, src->Name);
	Draw_LineH(0, src->Width - 1, 12, src->White, 0);
	Draw_LineH(0, src->Width - 1, 114, src->White, 0);
	Draw_LineV(src->Width / 2, 16, 212, src->White, 0);

	// Draw lines
	for (u8 i = 0; i < 16; ++i)
	{
		RandomizeData(0, 16);
		Draw_Line(g_Data.x1, g_Data.y1, g_Data.x2, g_Data.y2, g_Data.color, 0);
	}
	// Draw vertical/harizontal lines
	for (u8 i = 0; i < 8; ++i)
	{
		RandomizeData(src->Width / 2, 16);
		Draw_LineH(g_Data.x1, g_Data.x2, g_Data.y2, g_Data.color, 0);
		RandomizeData(src->Width / 2, 16);
		Draw_LineV(g_Data.x1, g_Data.y1, g_Data.y2, g_Data.color, 0);
	}
	for (u8 i = 0; i < 64; ++i)
	{
		RandomizeData(src->Width / 2, 16);
		Draw_Point(g_Data.x1, g_Data.y1, g_Data.color, 0);
	}
	// Draw boxes
	for (u8 i = 0; i < 16; ++i)
	{
		RandomizeData(0, 114);
		Draw_Box(g_Data.x1, g_Data.y1, g_Data.x2, g_Data.y2, g_Data.color, 0);
	}
	// Draw circles
	for (u8 i = 0; i < 16; ++i)
	{
		RandomizeData(src->Width / 2, 114);
		Draw_Circle(
			(g_Data.x1 + g_Data.x2) / 2, 
			(g_Data.y1 + g_Data.y2) / 2, 
			Min(Abs16(g_Data.y1 - g_Data.y2) / 2, Abs16(g_Data.x1 - g_Data.x2) / 2),
			g_Data.color, 0);
	}

	Print_SetPosition(8, 20);
	Print_DrawText("Lines");
	Print_SetPosition((src->Width / 2) + 8 + 8, 20);
	Print_DrawText("Hori./Verti. lines");	
	Print_SetPosition(8, 114 + 4);
	Print_DrawText("Boxes");
	Print_SetPosition((src->Width / 2) + 8, 114 + 4);
	Print_DrawText("Circles");

	#define PAL_POS_Y	(u8)(212 - 9)

	if (src->BPC == 4)
	{
		Print_SetPosition(2, PAL_POS_Y);
		Print_DrawFormat("\x82Pal:%s", pal->Name);
	}

	// Draw palette
	if (src->BPC == 8)
	{
		u16 startX = src->Width - (128 + 1);
		u16 x = startX;
		u16 y = PAL_POS_Y;
		Draw_FillBox(x - 1, (u8)(PAL_POS_Y - 1), (u8)(x + 128), (u8)(PAL_POS_Y + 8), 0xFF, 0);
		for (u16 j = 0; j < 4; ++j)
		{
			x = startX;
			for (u16 i = 0; i < 64; ++i)
			{
				Draw_FillBox(x, y, x + 1, y + 1, i + j * 64, 0);
				x += 2;
			}
			y += 2;
		}
	}
	else
	{
		u16 x = src->Width - (palWidth * colNum + 1);
		Draw_FillBox(x - 1, (u8)(PAL_POS_Y - 1), x + (palWidth * colNum), (u8)(PAL_POS_Y + 8), 0xFF, 0);
		loop(i, colNum)
		{
			u8 col = (src->BPC == 4) ? COLOR_MERGE2(i) : COLOR_MERGE4(i);
			Draw_FillBox(x, PAL_POS_Y, x + palWidth - 1, (u8)(PAL_POS_Y + 7), col, 0);
			x += palWidth;
		}
	}
}

//-----------------------------------------------------------------------------
// H_TIMI interrupt hook
void VBlankHook()
{
	g_VBlank = 1;
}

//-----------------------------------------------------------------------------
// Wait for V-Blank period
void WaitVBlank()
{
	while (g_VBlank == 0) {}
	g_VBlank = 0;
	g_Frame++;
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Init	
	Bios_SetHookCallback(H_TIMI, VBlankHook);
	DisplayPage();

	bool bContinue = TRUE;
	while (bContinue)
	{
		const struct ScreenSetting* src = &g_Settings[g_SrcModeIndex];

		Print_SetPosition(src->Width - 8, 2);
		Print_DrawChar(chrAnim[g_Frame & 0x03]);

		u8 row = Keyboard_Read(8);
		// Select screen mode
		if ((row & KEY_FLAG(KEY_LEFT)) == 0)
		{
			if (g_SrcModeIndex > 0)
				g_SrcModeIndex--;
			else
				g_SrcModeIndex = numberof(g_Settings) - 1;
			DisplayPage();
		}
		else if ((row & KEY_FLAG(KEY_RIGHT)) == 0)
		{
			if (g_SrcModeIndex < numberof(g_Settings) - 1)
				g_SrcModeIndex++;
			else
				g_SrcModeIndex = 0;
			DisplayPage();
		}
		// Select palette
		if (src->BPC == 4)
		{
			if ((row & KEY_FLAG(KEY_UP)) == 0)
			{
				if (g_PalIndex > 0)
					g_PalIndex--;
				else
					g_PalIndex = numberof(g_Palettes) - 1;
				DisplayPage();
			}
			else if ((row & KEY_FLAG(KEY_DOWN)) == 0)
			{
				if (g_PalIndex < numberof(g_Palettes) - 1)
					g_PalIndex++;
				else
					g_PalIndex = 0;
				DisplayPage();
			}
		}
		
		if (Keyboard_IsKeyPressed(KEY_ESC))
			bContinue = FALSE;

		WaitVBlank();
	}

	Bios_ClearHook(H_TIMI);
	Bios_Exit(0);
}