// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  VDP command sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL6		"\x01\x02\x03\x04\x05\x06"
#define MSX_GL8		"\x02\x03\x04\x05"
// #define GET_OP(a)	(g_Operator == 0xFF) ? a : g_Operator
#define HEIGHT		212

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
};

//=============================================================================
// MEMORY DATA
//=============================================================================

// Screen mode setting index
u8   g_SrcModeIndex;
u8   g_VBlank = 0;
u8   g_Frame = 0;
u16  SX0, SY0;
u16  SX, SY;
u8   g_Operator = 0xFF; // default
bool g_bMoving;

// Memory buffer to unpack compressed data
u8 g_LMMC2b_2[32*16*6];
u8 g_LMMC4b[16*16*6];
u8 g_LMMC4b_2[32*16*6];

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"
#include "font/font_mgl_sample8.h"

// Bitmaps by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "content/data_bmp_2b.h"
#include "content/data_bmp_4b.h"
#include "content/data_bmp_8b.h"

// Screen mode settings
const struct ScreenSetting g_Settings[] =
{ //  Name                                       Mode             Width BPC Txt   BG    Red   White Gray  Black Font                Data         DataLMMC     Palette 
	{ MSX_GL6 " SOFTWARE SPRITE SAMPLE (S5/G4)", VDP_MODE_SCREEN5, 256,	4,	0xFF, 0xCC, 0x22, 0x33, 0x11, 0x11, g_Font_MGL_Sample6, g_DataBmp4b, g_LMMC4b,    NULL }, // 0
	{ MSX_GL8 " SOFTWARE SPRITE SAMPLE (S6/G5)", VDP_MODE_SCREEN6, 512,	2,	0xFF, 0x00, 0x55, 0xAA, 0xAA, 0x55, g_Font_MGL_Sample8, g_DataBmp2b, g_LMMC2b_2,  NULL }, // 1
	{ MSX_GL8 " SOFTWARE SPRITE SAMPLE (S7/G6)", VDP_MODE_SCREEN7, 512,	4,	0xFF, 0xCC, 0x22, 0x33, 0x11, 0x11, g_Font_MGL_Sample8, g_DataBmp4b, g_LMMC4b_2,  NULL }, // 2
	{ MSX_GL6 " SOFTWARE SPRITE SAMPLE (S8/G7)", VDP_MODE_SCREEN8, 256,	8,	0xFF, COLOR8_DARKGREEN, COLOR8_GREEN, COLOR8_LIGHTGREEN, 0x6D, 0x00, g_Font_MGL_Sample6, g_DataBmp8b, g_DataBmp8b, NULL }, // 3
};

// Sign-of-life animation
const u8 chrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void BackupBackground()
{
	const struct ScreenSetting* src = &g_Settings[g_SrcModeIndex];
	u8 pixelWidth = src->Width / 256;
	u8 sprtWidth = pixelWidth * 16;

	// Backup
	VDP_CommandHMMM(SX * pixelWidth - 2, SY, 0, HEIGHT + 16, sprtWidth + 4, 16);

	SX0 = SX;
	SY0 = SY;
}

//-----------------------------------------------------------------------------
//
void InitScreen()
{
	const struct ScreenSetting* src = &g_Settings[g_SrcModeIndex];

	//-------------------------------------------------------------------------
	// Setup variables
	u8* buffer = Mem_HeapAlloc(256);
	u16 blockWidth = src->Width / 16;
	u16 blockBytes = 16 / 8 * src->BPC;
	u16 lineBytes = src->Width / 8 * src->BPC;
	u8 scale = 8 / src->BPC;
	u8 pixelWidth = src->Width / 256;
	u8 sprtWidth = pixelWidth * 16;

	// Initialize VDP	
	VDP_SetMode(src->Mode);
	VDP_SetColor(src->Background);
	VDP_EnableSprite(FALSE);

	// VDP_EnableHBlank(TRUE);
	// VDP_SetHBlankLine(212/2);

	//-------------------------------------------------------------------------
	// Initialize background
	if(src->BPC == 2)
	{
		VDP_SetPaletteEntry(0, RGB16(1, 3, 1));
		VDP_SetPaletteEntry(1, RGB16(0, 0, 0));
		VDP_SetPaletteEntry(2, RGB16(3, 5, 3));
		VDP_SetPaletteEntry(3, RGB16(7, 7, 7));
	}
	else if(src->BPC == 4)
	{
		VDP_SetPaletteEntry(0, COLOR16_DEFAULT_0);
		VDP_SetPaletteEntry(1, COLOR16_DEFAULT_1);
		VDP_SetPaletteEntry(2, COLOR16_DEFAULT_2);
		VDP_SetPaletteEntry(3, COLOR16_DEFAULT_3);
		VDP_SetPaletteEntry(13, RGB16(4, 4, 4));
	}
	VDP_CommandHMMV(0, 0, src->Width, HEIGHT, src->Background);
	for(u8 i = 0; i < 255; ++i)
	{
		u16 x = Math_GetRandom16() % src->Width;
		u16 y = Math_GetRandom16() % (HEIGHT - 24) + 16;
		VDP_CommandLMMV(x, y, pixelWidth, 1, src->Red, VDP_OP_IMP);
		x = Math_GetRandom16() % src->Width;
		y = Math_GetRandom16() % (HEIGHT - 24) + 16;
		VDP_CommandLMMV(x, y, pixelWidth, 1, src->White, VDP_OP_IMP);
	}

	// Initialize sprite
	VDP_CommandHMMV(0, HEIGHT, sprtWidth * 6, 16, 0);
	for(u8 i = 0; i < 6; ++i)
	{
		VDP_CommandLMMC(src->DataLMMC + i * (16 * 16 * pixelWidth), i * sprtWidth, HEIGHT, sprtWidth, 16, VDP_OP_TIMP);
	}

	//-------------------------------------------------------------------------
	// Initialize font
	Print_SetBitmapFont(src->Font);
	Print_SetColor(src->Text, src->Background);

	// Display header
	Print_SetPosition(0, 2);
	Print_DrawText(src->Name);
	Draw_LineH(0, src->Width - 1, 12, src->Text, 0);

	// Display footer
	Print_SetPosition(0, 204);
	Print_DrawText("\x81\x82\x80:Move \x83:Change mode");

	BackupBackground();
}

//-----------------------------------------------------------------------------
//
void DisplaySprite()
{
	const struct ScreenSetting* src = &g_Settings[g_SrcModeIndex];
	u8 pixelWidth = src->Width / 256;
	u8 sprtWidth = pixelWidth * 16;

	// Restore background at the previous place
	VDP_CommandHMMM(0, HEIGHT + 16, SX0 * pixelWidth - 2, SY0, sprtWidth + 4, 16);

	// Backup background at the new place
	VDP_CommandHMMM(SX * pixelWidth - 2, SY, 0, HEIGHT + 16, sprtWidth + 4, 16);

	// Draw the sprite
	u8 frame = g_bMoving ? ((g_Frame / 4) % 6) : 4;
	VDP_CommandLMMM(sprtWidth * frame, HEIGHT, SX * pixelWidth, SY, sprtWidth, 16, VDP_OP_TIMP);

	SX0 = SX;
	SY0 = SY;
}

//-----------------------------------------------------------------------------
// HBlank interrupt
void VDP_HBlankHandler()
{
	// VDP_SetColor(g_Frame);
}

//-----------------------------------------------------------------------------
// VBlank interrupt
void VDP_InterruptHandler()
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
	// VDP_SetColor(0);
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Precalc
	for(u16 i = 0; i < 16*16*6; ++i)
	{
		u8 c, b = g_DataBmp4b[i >> 1];
		if((i & 0x1) == 0)
			c = b >> 4;
		else // if((i & 0x1) == 1)
			c = b & 0x0F;
		if(c == 3)
			c = 13;
		g_LMMC4b[i] = c;
		g_LMMC4b_2[2*i+0] = c;
		g_LMMC4b_2[2*i+1] = c;
	}
	for(u16 i = 0; i < 16*16*6; ++i)
	{
		u8 c, b = g_DataBmp2b[i >> 2];
		if((i & 0x3) == 0)
			c = b >> 6;
		else if((i & 0x3) == 1)
			c = b >> 4;
		else if((i & 0x3) == 2)
			c = b >> 2;
		else // if((i & 0x3) == 3)
			c = b;
		g_LMMC2b_2[2*i+0] = c & 0x03;
		g_LMMC2b_2[2*i+1] = c & 0x03;
	}

	// Init
	g_SrcModeIndex = 3;

	SX = 64;
	SY = 64;
	InitScreen();

	bool bContinue = TRUE;
	while(bContinue)
	{
		WaitVBlank();
		DisplaySprite();

		const struct ScreenSetting* src = &g_Settings[g_SrcModeIndex];
		Print_SetPosition(src->Width - 8, 2);
		Print_DrawChar(chrAnim[g_Frame & 0x03]);

		u8 row = Keyboard_Read(KEY_ROW(KEY_DOWN));
		if((row & KEY_FLAG(KEY_SPACE)) == 0)
		{
			if(g_SrcModeIndex < numberof(g_Settings) - 1)
				g_SrcModeIndex++;
			else
				g_SrcModeIndex = 0;
			InitScreen();
		}
		g_bMoving = FALSE;
		if((row & KEY_FLAG(KEY_LEFT)) == 0)
		{
			if(SX > 2)
			{
				SX--;
				g_bMoving = TRUE;
			}
		}
		else if((row & KEY_FLAG(KEY_RIGHT)) == 0)
		{
			if(SX < 256-16-2)
			{
				SX++;
				g_bMoving = TRUE;
			}
		}
		if((row & KEY_FLAG(KEY_UP)) == 0)
		{
			if(SY > 2)
			{
				SY--;
				g_bMoving = TRUE;
			}
		}
		else if((row & KEY_FLAG(KEY_DOWN)) == 0)
		{
			if(SY < HEIGHT-16-2)
			{
				SY++;
				g_bMoving = TRUE;
			}
		}

		if(Keyboard_IsKeyPressed(KEY_ESC))
			bContinue = FALSE;
	}
}