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
#include "sprite_helper.h"

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
const u8 g_CharAnim[] = { '|', '\\', '-', '/' };

// 
const u8 g_Mask1[32] = 
{
	0b00000111, 
	0b00011111, 
	0b00111111, 
	0b01111111, 
	0b01111111, 
	0b11111111, 
	0b11111111, 
	0b11111111, 
	0b11111111, 
	0b11111111, 
	0b11111111, 
	0b01111111, 
	0b01111111, 
	0b00111111, 
	0b00011111, 
	0b00000111, 

	0b11100000,
	0b11111000,
	0b11111100,
	0b11111110,
	0b11111110,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111110,
	0b11111110,
	0b11111100,
	0b11111000,
	0b11100000,
};

// 
const u8 g_Mask2[32] = 
{
	0b00000000,
	0b00000000,
	0b00000011,
	0b00001111,
	0b00011111,
	0b00011111,
	0b00111111,
	0b00111111,
	0b00111111,
	0b00111111,
	0b00011111,
	0b00011111,
	0b00001111,
	0b00000011,
	0b00000000,
	0b00000000,

	0b00000000,
	0b00000000,
	0b11000000,
	0b11110000,
	0b11111000,
	0b11111000,
	0b11111100,
	0b11111100,
	0b11111100,
	0b11111100,
	0b11111000,
	0b11111000,
	0b11110000,
	0b11000000,
	0b00000000,
	0b00000000,
};

// 
const u8 g_Mask3[32] = 
{
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000011,
	0b00000111,
	0b00001111,
	0b00001111,
	0b00001111,
	0b00001111,
	0b00000111,
	0b00000011,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,

	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b11000000,
	0b11100000,
	0b11110000,
	0b11110000,
	0b11110000,
	0b11110000,
	0b11100000,
	0b11000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
};

// 
const u8 g_Mask4[32] = 
{
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000001,
	0b00000011,
	0b00000011,
	0b00000001,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,

	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b10000000,
	0b11000000,
	0b11000000,
	0b10000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
};
const u8* g_MaskAnim[] = { NULL, g_Mask1, g_Mask2, g_Mask3, g_Mask4, g_Mask3, g_Mask2, g_Mask1 };

//=============================================================================
// MEMORY DATA
//=============================================================================

// Screen mode setting index
u8 g_VBlank = 0;
u8 g_Frame = 0;

// Sprite data
u8 g_PatternBuffer[PATTERN_16OR_NUM * 8];

// Sprite mask
u8 g_Mask[32*2];

// Sprite X position
u8 g_PosX0;
u8 g_PosX1;

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
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);

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
	VDP_SetSpriteExUniColor(0, 0, 32-1, 0 * 4, 0x02);
	VDP_SetSpriteExUniColor(1, 0, 32-1, 1 * 4, VDP_SPRITE_CC + 0x01);
	VDP_SetSpriteExUniColor(2, 0, 80-1, 2 * 4, 0x02);
	VDP_SetSpriteExUniColor(3, 0, 80-1, 3 * 4, VDP_SPRITE_CC + 0x01);
	VDP_SetSpriteExUniColor(4, 0, 128-1, 4 * 4, 0x02);
	VDP_SetSpriteExUniColor(5, 0, 128-1, 5 * 4, VDP_SPRITE_CC + 0x01);
	VDP_HideSpriteFrom(6);
	g_PosX0 = 0;
	g_PosX1 = 0;

	// Setup print
	Print_SetTextFont(g_Font_MGL_Sample8, 0);
	Print_SetColor(0xF, 0x4);
	VDP_FillVRAM_16K(COLOR_LIGHT_BLUE << 4, g_ScreenColorLow + (32*4*8) + (0*256*8), 32*4*8);
	VDP_FillVRAM_16K(COLOR_LIGHT_BLUE << 4, g_ScreenColorLow + (32*4*8) + (1*256*8), 32*4*8);
	VDP_FillVRAM_16K(COLOR_LIGHT_BLUE << 4, g_ScreenColorLow + (32*4*8) + (2*256*8), 32*4*8);

	Print_DrawTextAt(0, 0, "\x2\x3\x4\x5 Sprite helper sample");
	Print_DrawCharXAt(0, 1, '\x17', 32);

	Print_DrawTextAt(1, 4, "Crop R/L");
	Print_DrawTextAt(15, 3, "\x9F\x9F");
	Print_DrawTextAt(15, 4, "\x9F\x9F");
	Print_DrawTextAt(15, 5, "\x9F\x9F");
	Print_DrawCharXAt(0, 6, '\x9F', 32);

	Print_DrawTextAt(1, 10, "Flip H");
	Print_DrawCharXAt(0, 12, '\x9F', 32);

	Print_DrawTextAt(1, 16, "Mask");
	Print_DrawCharXAt(0, 18, '\x9F', 32);

	Print_DrawTextAt(1, 22, "Rotate");
	Print_DrawCharXAt(0, 18, '\x9F', 32);

	bool bContinue = TRUE;
	while(bContinue)
	{
		// VDP_SetColor(COLOR_DARK_BLUE);
		WaitVBlank();
		// VDP_SetColor(COLOR_LIGHT_BLUE);
	
		Print_DrawCharAt(31, 0, g_CharAnim[g_Frame & 0x03]);

		u8 frame = (g_Frame >> 2) % 6;
		u8 pat = (frame * 8 * 4);

		// Crop right/left
		g_PosX0++;
		VDP_SetSpritePositionX(0, g_PosX0);
		VDP_SetSpritePositionX(1, g_PosX0);
		if((g_PosX0 > 104) && (g_PosX0 <= 120))
		{
			u8 offset = g_PosX0 - 105;
			Sprite_CropRight16((const u8*)(g_PatternBuffer + pat), (u8*)0xF000, offset);
			VDP_LoadSpritePattern((u8*)0xF000, 0, 4);
			Sprite_CropRight16((const u8*)(g_PatternBuffer + pat + (24 * 8)), (u8*)0xF000, offset);
			VDP_LoadSpritePattern((u8*)0xF000, 4, 4);
		}
		else if((g_PosX0 >= 120) && (g_PosX0 < 136))
		{
			u8 offset = 15 - (g_PosX0 - 120);
			Sprite_CropLeft16((const u8*)(g_PatternBuffer + pat), (u8*)0xF000, offset);
			VDP_LoadSpritePattern((u8*)0xF000, 0, 4);
			Sprite_CropLeft16((const u8*)(g_PatternBuffer + pat + (24 * 8)), (u8*)0xF000, offset);
			VDP_LoadSpritePattern((u8*)0xF000, 4, 4);
		}
		else
		{
			VDP_LoadSpritePattern(g_PatternBuffer + pat, 0, 4);
			VDP_LoadSpritePattern(g_PatternBuffer + 24 * 8 + pat, 4, 4);
		}

		// Flip vertical
		g_PosX1--;
		VDP_SetSpritePositionX(2, g_PosX1);
		VDP_SetSpritePositionX(3, g_PosX1);
		Sprite_FlipH16((const u8*)(g_PatternBuffer + pat), (u8*)0xF000);
		VDP_LoadSpritePattern((u8*)0xF000, 8, 4);
		Sprite_FlipH16((const u8*)(g_PatternBuffer + pat + (24 * 8)), (u8*)0xF000);
		VDP_LoadSpritePattern((u8*)0xF000, 12, 4);

		// Mask
		VDP_SetSpritePositionX(4, g_PosX0);
		VDP_SetSpritePositionX(5, g_PosX0);
		frame = (g_Frame >> 3) % 8;
		if(g_MaskAnim[frame])
		{
			Sprite_Mask16((const u8*)(g_PatternBuffer + pat), (u8*)0xF000, g_MaskAnim[frame]);
			VDP_LoadSpritePattern((u8*)0xF000, 16, 4);
			Sprite_Mask16((const u8*)(g_PatternBuffer + pat + (24 * 8)), (u8*)0xF000, g_MaskAnim[frame]);
			VDP_LoadSpritePattern((u8*)0xF000, 20, 4);
		}
		else
		{
			VDP_LoadSpritePattern(g_PatternBuffer + pat, 16, 4);
			VDP_LoadSpritePattern(g_PatternBuffer + 24 * 8 + pat, 20, 4);
		}

		if(Keyboard_IsKeyPressed(KEY_ESC))
			bContinue = FALSE;
	}

	Bios_ClearHook(H_TIMI);
	Bios_Exit(0);
}


