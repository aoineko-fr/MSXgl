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
#include "scroll.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x02\x03\x04\x05"

#if (MSX_VERSION & MSX_1)
	#define SET_SPRITE		VDP_SetSpriteSM1
#else
	#define SET_SPRITE		VDP_SetSpriteExUniColor
#endif

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

//=============================================================================
// MEMORY DATA
//=============================================================================

// Scrolling speed
i8 g_ScrollSpeed = 1;

// V-blank synchronization flag
u8 g_VBlank = 0;

// H-blank phase
u8 g_HBlank = 0;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

#if ((SCROLL_ADJUST) && (SCROLL_ADJUST_SPLIT))
//-----------------------------------------------------------------------------
// H-Blank interrupt hook
void HBlankHook()
{
	if(g_HBlank == 0)
	{
		Scroll_HBlankAdjust(1);
		g_HBlank++;
	}
	else
	{
		Scroll_HBlankAdjust(2);
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
#endif // ((SCROLL_ADJUST) && (SCROLL_ADJUST_SPLIT))

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
	#if ((SCROLL_ADJUST) && (SCROLL_ADJUST_SPLIT))
	g_HBlank = 0;
	Scroll_HBlankAdjust(0);
	#endif
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize video
	#if !(MSX_VERSION & MSX_1)
	VDP_SetMode(VDP_MODE_GRAPHIC3); // Screen mode 4 (G3)
	#else
	VDP_SetMode(VDP_MODE_GRAPHIC2); // Screen mode 2 (G2)
	#endif
	// VRAM Tables Address
	VDP_SetLayoutTable(0x3800);
	VDP_SetColorTable(0x2000);
	VDP_SetPatternTable(0x0000);
	VDP_SetSpritePatternTable(0x1800);
	VDP_SetSpriteAttributeTable(0x3E00);
	// Setup video
	VDP_SetColor(0xF0);
	VDP_ClearVRAM();
	VDP_EnableVBlank(TRUE);
	#if ((SCROLL_ADJUST) && (SCROLL_ADJUST_SPLIT))
	VDP_EnableHBlank(TRUE);
	Bios_SetHookCallback(H_KEYI, InterruptHook);
	#endif
	Bios_SetHookCallback(H_TIMI, VBlankHook);

	// Initialize text
	Print_SetTextFont(g_Font_MGL_Sample8, 96);
	VDP_FillLayout_GM2(96, 0, 0, 32, 24);
	// Header
	Print_DrawText(MSX_GL " SCROLL SAMPLE");
	Print_DrawLineH(0, 1, 32);
	// Footer
	Print_DrawLineH(0, 22, 32);
	Print_SetPosition(0, 23);
	Print_DrawFormat("\x81\x82\x80:Scroll \x83:Speed(%i)", g_ScrollSpeed);

	// Load tiles pattern
	VDP_LoadPattern_GM2(g_DataMapGM2_Patterns, 94, 0);
	VDP_LoadColor_GM2(g_DataMapGM2_Colors, 94, 0);
	
	// Initialize scroll module
	u8 sprt = Scroll_Initialize((u16)g_DataMapGM2_Names);
	Scroll_SetOffsetV(16);

	// Initialize sprite
	// VDP_EnableSprite(TRUE);
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16 | VDP_SPRITE_SCALE_2);
	VDP_LoadSpritePattern(g_DataSprtLayer, 32, 13*4*4);
	SET_SPRITE(sprt+0, 70, 130, 32, COLOR_BLACK);
	SET_SPRITE(sprt+1, 70, 130, 36, COLOR_WHITE);
	SET_SPRITE(sprt+2, 70, 130, 40, COLOR_LIGHT_RED);
	VDP_DisableSpritesFrom(sprt+3);

	u8 frame = 0;
	u8 prevRow8 = 0xFF;
	while(1)
	{
		// Wait for v-synch
		WaitVBlank();

		// Update scrolling rendering
		Scroll_Update();

		u8 sprtOffset = g_Scroll_OffsetX % 8;
		VDP_SetSpritePositionX(sprt+0, 70 + sprtOffset);
		VDP_SetSpritePositionX(sprt+1, 70 + sprtOffset);
		VDP_SetSpritePositionX(sprt+2, 70 + sprtOffset);

		u8 shape = 6;

		u8 row8 = Keyboard_Read(8);
		// Update scrolling speed
		if(IS_KEY_PRESSED(row8, KEY_SPACE) && !IS_KEY_PRESSED(prevRow8, KEY_SPACE))
		{
			if(g_ScrollSpeed < 8)
				g_ScrollSpeed++;
			else
				g_ScrollSpeed = 1;
			Print_SetPosition(19, 23);
			Print_DrawInt(g_ScrollSpeed);
		}
		else if(IS_KEY_PRESSED(row8, KEY_HOME) && !IS_KEY_PRESSED(prevRow8, KEY_HOME))
		{
			if(g_ScrollSpeed > 1)
				g_ScrollSpeed--;				
			else
				g_ScrollSpeed = 7;
			Print_SetPosition(19, 23);
			Print_DrawInt(g_ScrollSpeed);
		}
		// Update horizontal scrolling offset
		if(IS_KEY_PRESSED(row8, KEY_RIGHT))
		{
			Scroll_SetOffsetH(g_ScrollSpeed);
			shape = (frame >> 2) % 6;
		}
		else if(IS_KEY_PRESSED(row8, KEY_LEFT))
		{
			Scroll_SetOffsetH(-g_ScrollSpeed);
			shape = (frame >> 2) % 6;
		}
		// Update vertical scrolling offset
		if(IS_KEY_PRESSED(row8, KEY_DOWN))
		{
			Scroll_SetOffsetV(g_ScrollSpeed);
		}
		else if(IS_KEY_PRESSED(row8, KEY_UP))
		{
			Scroll_SetOffsetV(-g_ScrollSpeed);
		}
		prevRow8 = row8;

		VDP_SetSpritePattern(sprt+0, 32 + shape * 16);
		VDP_SetSpritePattern(sprt+1, 36 + shape * 16);
		VDP_SetSpritePattern(sprt+2, 40 + shape * 16);

		frame++;
	}
}