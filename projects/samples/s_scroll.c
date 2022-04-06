// __________________________
// ██▀█▀██▀▀▀█▀▀█▀█  ▄▄▄ ▄▄  │   ▄▄▄                ▄▄      
// █  ▄ █▄ ▀██▄ ▀▄█ ██   ██  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █▀▀ ▄█  █ █ ▀█▄█ ██▄▄│  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀──────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "scroll.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x02\x03\x04\x05"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font\font_mgl_sample8.h"
// Tiles
#include "content\tile\data_tile_gm2.h"
#include "content\tile\data_map_gm2.h"

//=============================================================================
// MAIN LOOP
//=============================================================================

i8 g_ScrollSpeed = 1;

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize video
	#if (MSX_VERSION >= MSX_2)
	VDP_SetMode(VDP_MODE_GRAPHIC3);
	#else
	VDP_SetMode(VDP_MODE_GRAPHIC2);
	#endif
	// VRAM Tables Address
	VDP_SetLayoutTable(0x3800);
	VDP_SetColorTable(0x2000);
	VDP_SetPatternTable(0x0000);
	VDP_SetSpritePatternTable(0x1800);
	VDP_SetSpriteAttributeTable(0x3E00);

	VDP_SetColor(0xF0);
	VDP_ClearVRAM();
	VDP_EnableVBlank(true);

	// Initialize text
	Print_SetTextFont(g_Font_MGL_Sample8, 128);
	VDP_FillLayout_GM2(128, 0, 0, 32, 24);
	Print_DrawText(MSX_GL " SCROLL SAMPLE");
	Print_SetPosition(26, 0);
	Print_DrawFormat("Spd:%i", g_ScrollSpeed);
	Print_DrawLineH(0, 1, 32);

	// Load tiles pattern
	VDP_LoadPattern_GM2(g_DataMapGM2_Patterns, 94, 0);
	VDP_LoadColor_GM2(g_DataMapGM2_Colors, 94, 0);

	// Initialize scroll module
	Scroll_Initialize((u16)g_DataMapGM2_Names);
	
	u8 prevRow8 = 0xFF;
	while(1)
	{
		Halt();
		Scroll_Update();

		u8 row8 = Keyboard_Read(8);
		if(IS_KEY_PRESSED(row8, KEY_UP) && !IS_KEY_PRESSED(prevRow8, KEY_UP))
		{
			if(g_ScrollSpeed < 8)
			{
				g_ScrollSpeed++;
				Print_SetPosition(30, 0);
				Print_DrawInt(g_ScrollSpeed);
			}
		}
		else if(IS_KEY_PRESSED(row8, KEY_DOWN) && !IS_KEY_PRESSED(prevRow8, KEY_DOWN))
		{
			if(g_ScrollSpeed > 1)
			{
				g_ScrollSpeed--;				
				Print_SetPosition(30, 0);
				Print_DrawInt(g_ScrollSpeed);
			}
		}
		if(IS_KEY_PRESSED(row8, KEY_RIGHT))
		{
			Scroll_SetOffsetH(g_ScrollSpeed);
		}
		else if(IS_KEY_PRESSED(row8, KEY_LEFT))
		{
			Scroll_SetOffsetH(-g_ScrollSpeed);
		}
		if(IS_KEY_PRESSED(row8, KEY_DOWN))
		{
			Scroll_SetOffsetV(g_ScrollSpeed);
		}
		else if(IS_KEY_PRESSED(row8, KEY_UP))
		{
			Scroll_SetOffsetV(-g_ScrollSpeed);
		}
		prevRow8 = row8;
	}
}