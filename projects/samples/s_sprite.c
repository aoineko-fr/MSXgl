// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Sprite mode 2 sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"
#include "bios.h"

//=============================================================================
// DEFINES
//=============================================================================

// Pattern enum
#define PATTERN_8_1ST			(u8)0
#define PATTERN_8_NUM			(u8)64
#define PATTERN_16_1ST			(u8)(PATTERN_8_1ST+PATTERN_8_NUM)
#define PATTERN_16_NUM			(u8)(6*2*4)
#define PATTERN_16OR_1ST		(u8)(PATTERN_16_1ST+PATTERN_16_NUM)
#define PATTERN_16OR_NUM		(u8)(6*2*4)

// Sprite enum
#define SPRITE_8_1ST			(u8)0
#define SPRITE_8_NUM			(u8)25
#define SPRITE_16_1ST			(u8)(SPRITE_8_1ST+SPRITE_8_NUM)
#define SPRITE_16_NUM			(u8)2
#define SPRITE_16OR_1ST			(u8)(SPRITE_16_1ST+SPRITE_16_NUM)
#define SPRITE_16OR_NUM			(u8)4

// Line interruption
#if (TARGET_TYPE == TYPE_BIN)
	#define SPRITE_2X_LINE		63-9
	#define SPRITE_8_LINE		113-9
#elif (TARGET_TYPE == TYPE_ROM)
	#define SPRITE_2X_LINE		63-4
	#define SPRITE_8_LINE		113-4
#elif (TARGET_TYPE == TYPE_DOS)
	#define SPRITE_2X_LINE		63-10
	#define SPRITE_8_LINE		113-10
#endif

// Vector structure
struct Vector
{
	i8 x, y;
};

/// Sprite data
struct SpriteData
{
	u8 Y;
	u8 X;
	u8 Shape;
};

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"
#include "font/font_mgl_symbol1.h"

// Sprite by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "content/data_sprt_16.h"
#include "content/data_sprt_16or.h"

// Character animation
const u8 chrAnim[] = { '|', '\\', '-', '/' };

// Sprite movement
const struct Vector DirMove[] = 
{
	{ (i8)1,  (i8)1 },
	{ (i8)1,  (i8)0 },
	{ (i8)1,  (i8)-1 },
	{ (i8)0,  (i8)-1 },
	{ (i8)-1, (i8)-1 },
	{ (i8)-1, (i8)0 },
	{ (i8)-1, (i8)1 },
	{ (i8)0,  (i8)1 },
};

// Color table
const u8 ColorTab[] = 
{
	15, 5, 6, 7, 1, 7, 6, 5,
	15, 5, 6, 7, 1, 7, 6, 5,
	15, 5, 6, 7, 1, 7, 6, 5,
};

//=============================================================================
// MEMORY DATA
//=============================================================================

// Screen mode setting index
u8 g_VBlank = 0;
u8 g_Frame = 0;
u8 g_Phase = 0;

// Sprite data
struct SpriteData g_Sprite[32];

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// H-Blank interrupt hook
void HBlankHook()
{
	// VDP_SetColor(COLOR_BLACK);
	if(g_Phase == 0)
	{
		VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16 + VDP_SPRITE_SCALE_2);
		VDP_SetHBlankLine(SPRITE_8_LINE);
		g_Phase++;
		// VDP_SetColor(COLOR_DARK_GREEN);
	}
	else
	{
		VDP_SetSpriteFlag(VDP_SPRITE_SIZE_8);
		// VDP_SetColor(COLOR_DARK_RED);
	}
}

//-----------------------------------------------------------------------------
// H_KEYI interrupt hook
void InterruptHook()
{
	__asm
		// Get S#1
		ld		a, #1
		out		(P_VDP_ADDR), a
		ld		a, #(0x80 + 15)
		out		(P_VDP_ADDR), a
		in		a, (P_VDP_STAT)
		//  Call H-Blank if bit #0 of S#1 is set 
		rrca
		jp		nc, _no_hblank
		call	_HBlankHook // call to C function HBlankHook() 
	_no_hblank:
		// Reset R#15 to S#0
		xor		a           		
		out		(P_VDP_ADDR), a
		ld		a, #(0x80 + 15)
		out		(P_VDP_ADDR),a
	__endasm;

	// Call((u16)HookBackup_KEYI);
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
	while(g_VBlank == 0) {}
	g_VBlank = 0;
	g_Frame++;
	g_Phase = 0;
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);
	VDP_SetHBlankLine(SPRITE_2X_LINE);
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	Bios_SetKeyClick(FALSE);
	if(g_MSXVER == 0)
	{
		Bios_ClearScreen();
		Bios_TextPrintSting("This sample need MSX2 or above");
		Bios_GetCharacter();
		return;
	}

	// Setup screen
	VDP_SetMode(VDP_MODE_SCREEN5);
	VDP_SetColor(COLOR_DARK_BLUE);
	VDP_CommandHMMV(0, 0, 256, 1024, COLOR_MERGE(COLOR_DARK_BLUE, COLOR_DARK_BLUE)); // Clear VRAM
	VDP_CommandWait();
	
	// Setup sprite
	VDP_EnableSprite(TRUE);
	VDP_SetSpritePatternTable(0x17000);
	VDP_SetSpriteAttributeTable(0x17A00);
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16 /*+ VDP_SPRITE_SCALE_2*/);

	// Load 8x8 sprites (Pattern 0~95)
	VDP_LoadSpritePattern(g_Font_MGL_Symbol1 + 4 + (16 * 10 * 8), PATTERN_8_1ST, PATTERN_8_NUM);
	// Load 16x16 sprites (Pattern 96~143)
	u8 chrSprt = PATTERN_16_1ST;
	for(u8 i5 = 0; i5 < 6; i5++)
	{
		VDP_LoadSpritePattern(g_DataSprt16 + (i5 * 2 +  0) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16 + (i5 * 2 + 12) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16 + (i5 * 2 +  1) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16 + (i5 * 2 + 13) * 8, chrSprt++, 1);	
	}
	for(u8 i6 = 0; i6 < 6; i6++)
	{
		VDP_LoadSpritePattern(g_DataSprt16 + (i6 * 2 + 24) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16 + (i6 * 2 + 36) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16 + (i6 * 2 + 25) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16 + (i6 * 2 + 37) * 8, chrSprt++, 1);
	}
	chrSprt = PATTERN_16OR_1ST;
	for(u8 i5 = 0; i5 < 6; i5++)
	{
		VDP_LoadSpritePattern(g_DataSprt16or + (i5 * 2 +  0) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16or + (i5 * 2 + 12) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16or + (i5 * 2 +  1) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16or + (i5 * 2 + 13) * 8, chrSprt++, 1);	
	}
	for(u8 i6 = 0; i6 < 6; i6++)
	{
		VDP_LoadSpritePattern(g_DataSprt16or + (i6 * 2 + 24) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16or + (i6 * 2 + 36) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16or + (i6 * 2 + 25) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16or + (i6 * 2 + 37) * 8, chrSprt++, 1);
	}
	// Initialize 8x8 sprites
	for(u8 i = 0; i < SPRITE_8_NUM; i++)
	{
		u16 rnd = Math_GetRandom16();
		g_Sprite[i].X = rnd >> 8;
		g_Sprite[i].Y = (rnd & 0x007F) + 114;
		g_Sprite[i].Shape = PATTERN_8_1ST + ((rnd >> 4) & 0x3F);
		VDP_SetSpriteExUniColor(SPRITE_8_1ST + i, g_Sprite[i].X, g_Sprite[i].Y, g_Sprite[i].Shape, ((rnd >> 12) & 0x7) + 0x08);
	}
	// Initialize 16x16 sprites
	VDP_SetSpriteExUniColor(SPRITE_16_1ST + 0, 0, 32, PATTERN_16_1ST + 0 * 4, 0x0F);
	VDP_SetSpriteExUniColor(SPRITE_16_1ST + 1, 0, 32, PATTERN_16_1ST + 6 * 4, 0x01);
	// Initialize 16x16 OR sprites
	VDP_SetPaletteEntry(2, RGB16(7, 7, 7));
	VDP_SetPaletteEntry(3, RGB16(6, 4, 1));
	VDP_SetSpriteExUniColor(SPRITE_16OR_1ST + 0, (u8)128, 32, PATTERN_16OR_1ST + 0 * 4, 0x02);
	VDP_SetSpriteExUniColor(SPRITE_16OR_1ST + 1, (u8)128, 32, PATTERN_16OR_1ST + 6 * 4, VDP_SPRITE_CC + 0x01);
	VDP_SetSpriteExUniColor(SPRITE_16OR_1ST + 2, (u8)128, 64+8, PATTERN_16OR_1ST + 0 * 4, 0x02);
	VDP_SetSpriteExUniColor(SPRITE_16OR_1ST + 3, (u8)128, 64+8, PATTERN_16OR_1ST + 6 * 4, VDP_SPRITE_CC + 0x01);

	VDP_SetPaletteEntry(5, RGB16(7, 5, 5));
	VDP_SetPaletteEntry(6, RGB16(5, 3, 3));
	VDP_SetPaletteEntry(7, RGB16(3, 1, 1));
	VDP_SetSpriteExMultiColor(SPRITE_16OR_1ST + 4, (u8)(256-32), 64+8, PATTERN_8_1ST + 16, ColorTab);

	VDP_HideSpriteFrom(SPRITE_16_1ST + 7);

	// Setup print
	Print_SetBitmapFont(g_Font_MGL_Sample6);
	Print_SetColor(0xF, 0x4);
	Print_SetPosition(0, 2);
	Print_DrawText("\x1\x2\x3\x4\x5\x6 SPRITE MODE 2 (G4)");
	Draw_LineH(0, 255, 12, 0xFF, 0);
	Draw_LineH(0, 255, 64, 0xFF, 0);
	Draw_LineH(0, 255, 114, 0xFF, 0);
	Draw_LineV(128, 16, 60, 0xFF, 0);

	Print_SetPosition(8, 20);
	Print_DrawText("16x16");
	Print_SetPosition(8+128, 20);
	Print_DrawText("16x16 OR Color");
	Print_SetPosition(8, 64 + 8);
	Print_DrawText("16x16 OR x2");
	Print_SetPosition(8+128, 64 + 8);
	Print_DrawText("Multi-color");
	Print_SetPosition(8, 24 + 98);
	Print_DrawText("8x8");
	
	struct SpriteData* data1 = &g_Sprite[SPRITE_16_1ST];
	data1->X = 0;
	data1->Y = 32;
	struct SpriteData* data2 = &g_Sprite[SPRITE_16_1ST+1];
	data2->X = 128;
	data2->Y = 32;
	struct SpriteData* data3 = &g_Sprite[SPRITE_16_1ST+2];
	data3->X = 0;
	data3->Y = 64+8;

	g_Phase = 0;
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);
	VDP_SetHBlankLine(SPRITE_2X_LINE);
	VDP_EnableHBlank(TRUE);
	Bios_SetHookCallback(H_KEYI, InterruptHook);
	Bios_SetHookCallback(H_TIMI, VBlankHook);
	
	bool bContinue = TRUE;
	while(bContinue)
	{
		// VDP_SetColor(COLOR_LIGHT_BLUE);
		WaitVBlank();
		// VDP_SetColor(COLOR_DARK_BLUE);
	
		// Print_SetPosition(248, 2);
		// Print_DrawChar(chrAnim[g_Frame & 0x03]);

		struct SpriteData* sprt = &g_Sprite[0];
		u8 sprtIdx = SPRITE_8_1ST;
		for(u8 i = 0; i < SPRITE_8_NUM; i++)
		{
			const struct Vector* mov = &DirMove[i & 0x7];
			sprt->X += mov->x;
			sprt->Y += mov->y;
			if(sprt->Y > 212)
				sprt->Y -= 98;
			if(sprt->Y < 114)
				sprt->Y += 98;

			VDP_SetSpriteData(sprtIdx++, (u8*)sprt++);
		}

		u8 frame = (g_Frame >> 2) % 6;
		data1->X++;
		if(data1->X > 128-16)
			data1->X = 0;
		data1->Shape = PATTERN_16_1ST + (0 + frame) * 4;
		VDP_SetSpriteData(SPRITE_16_1ST + 0, (u8*)data1);
		data1->Shape = PATTERN_16_1ST + (6 + frame) * 4;
		VDP_SetSpriteData(SPRITE_16_1ST + 1, (u8*)data1);

		data2->X = data1->X + 128;
		data2->Shape = PATTERN_16OR_1ST + (0 + frame) * 4;
		VDP_SetSpriteData(SPRITE_16OR_1ST + 0, (u8*)data2);
		data2->Shape = PATTERN_16OR_1ST + (6 + frame) * 4;
		VDP_SetSpriteData(SPRITE_16OR_1ST + 1, (u8*)data2);

		data3->X += 2;
		data3->Shape = PATTERN_16OR_1ST + (0 + frame) * 4;
		VDP_SetSpriteData(SPRITE_16OR_1ST + 2, (u8*)data3);
		data3->Shape = PATTERN_16OR_1ST + (6 + frame) * 4;
		VDP_SetSpriteData(SPRITE_16OR_1ST + 3, (u8*)data3);

		VDP_SetSpriteMultiColor(SPRITE_16OR_1ST + 4, ColorTab + ((g_Frame >> 2) & 0x07));
		
		if(Keyboard_IsKeyPressed(KEY_ESC))
			bContinue = FALSE;
	}

	Bios_ClearHook(H_TIMI);
	Bios_ClearHook(H_KEYI);
	Bios_Exit(0);
}


