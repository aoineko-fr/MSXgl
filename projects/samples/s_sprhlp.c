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
#include "memory.h"
#include "tool/sprite_helper.h"

//=============================================================================
// DEFINES
//=============================================================================

// Pattern enum
#define PATTERN_16OR_NUM		(u8)(6*2*4)
#define SPRITE_16OR_NUM			(u8)4

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample8.h"

// Sprite by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "content/data_sprt_16or.h"

// Character animation
const u8 chrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// MEMORY DATA
//=============================================================================

// Screen mode setting index
u8 g_VBlank = 0;
u8 g_Frame = 0;

// Sprite data
u8 g_PatternBuffer[PATTERN_16OR_NUM * 8];

// Sprite X position
u8 g_PosX0;
u8 g_PosX1;

u8 g_Offset;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

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
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Setup system
	Bios_SetKeyClick(FALSE);
	Bios_SetHookCallback(H_TIMI, VBlankHook);

	// Setup screen
	VDP_SetMode(VDP_MODE_SCREEN4);
	VDP_SetColor(COLOR_DARK_BLUE);
	VDP_ClearVRAM();
	
	// Setup sprite
	VDP_EnableSprite(TRUE);
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16 + VDP_SPRITE_SCALE_2);

	// Load 16x16 sprites (Pattern 96~143)
	u8* ptr = g_PatternBuffer;
	for(u8 i5 = 0; i5 < 6; i5++)
	{
		Mem_Copy((u8*)g_DataSprt16or + (i5 * 2 +  0) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i5 * 2 + 12) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i5 * 2 +  1) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i5 * 2 + 13) * 8, ptr, 8); ptr += 8;
	}
	for(u8 i6 = 0; i6 < 6; i6++)
	{
		Mem_Copy((u8*)g_DataSprt16or + (i6 * 2 + 24) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i6 * 2 + 36) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i6 * 2 + 25) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i6 * 2 + 37) * 8, ptr, 8); ptr += 8;
	}

	// Initialize 16x16 OR sprites
	VDP_SetPaletteEntry(2, RGB16(7, 7, 7));
	VDP_SetPaletteEntry(3, RGB16(6, 4, 1));
	VDP_SetSpriteExUniColor(0, 0, 32, 0 * 4, 0x02);
	VDP_SetSpriteExUniColor(1, 0, 32, 1 * 4, VDP_SPRITE_CC + 0x01);
	VDP_SetSpriteExUniColor(2, 0, 64, 2 * 4, 0x02);
	VDP_SetSpriteExUniColor(3, 0, 64, 3 * 4, VDP_SPRITE_CC + 0x01);
	VDP_HideSpriteFrom(4);
	g_PosX0 = 0;
	g_PosX1 = 0;

	// Setup print
	Print_SetTextFont(g_Font_MGL_Sample8, 0);
	Print_SetColor(0xF, 0x4);
	Print_DrawTextAt(0, 0, "\x2\x3\x4\x5 Sprite helper sample");
	Print_DrawTextAt(1, 4, "Crop H");
	Print_DrawTextAt(15, 4, "##");
	Print_DrawTextAt(15, 5, "##");

	bool bContinue = TRUE;
	while(bContinue)
	{
		VDP_SetColor(COLOR_DARK_BLUE);
		WaitVBlank();
		VDP_SetColor(COLOR_LIGHT_BLUE);
	
		Print_DrawCharAt(31, 0, chrAnim[g_Frame & 0x03]);

		u8 frame = (g_Frame >> 2) % 6;
		u8 pat = (frame * 8 * 4);

		g_PosX0++;
		VDP_SetSpritePositionX(0, g_PosX0);
		VDP_SetSpritePositionX(1, g_PosX0);
		if((g_PosX0 > 104) && (g_PosX0 <= 120))
		{
			u8 offset = g_PosX0 - 105;
			Sprite_CropRight16(g_PatternBuffer + pat, 0xF000, offset);
			VDP_LoadSpritePattern(0xF000, 0, 4);
			Sprite_CropRight16(g_PatternBuffer + 24 * 8 + pat, 0xF000, offset);
			VDP_LoadSpritePattern(0xF000, 4, 4);
		}
		else if((g_PosX0 > 120) && (g_PosX0 <= 136))
		{
			u8 offset = 15 - (g_PosX0 - 121);
			Sprite_CropLeft16(g_PatternBuffer + pat, 0xF000, offset);
			VDP_LoadSpritePattern(0xF000, 0, 4);
			Sprite_CropLeft16(g_PatternBuffer + 24 * 8 + pat, 0xF000, offset);
			VDP_LoadSpritePattern(0xF000, 4, 4);
		}
		else
		{
			VDP_LoadSpritePattern(g_PatternBuffer + pat, 0, 4);
			VDP_LoadSpritePattern(g_PatternBuffer + 24 * 8 + pat, 4, 4);
		}





		// g_PosX0 = 128;
		// if(Keyboard_IsKeyPressed(KEY_RIGHT))
		// 	g_Offset++;
		// if(Keyboard_IsKeyPressed(KEY_LEFT))
		// 	g_Offset--;
		// g_Offset %= 16;


		g_PosX1 += 2;
		VDP_SetSpritePositionX(2, g_PosX1);
		VDP_SetSpritePositionX(3, g_PosX1);
		VDP_LoadSpritePattern(g_PatternBuffer + pat, 8, 4);
		VDP_LoadSpritePattern(g_PatternBuffer + 24 * 8 + pat, 12, 4);

		if(Keyboard_IsKeyPressed(KEY_ESC))
			bContinue = FALSE;
	}

	Bios_ClearHook(H_TIMI);
	Bios_Exit(0);
}


