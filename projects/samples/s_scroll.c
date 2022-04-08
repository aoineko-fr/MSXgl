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
#define VRAM_LAYOUT_TABLE			0x3800
#define VRAM_COLOR_TABLE			0x2000
#define VRAM_PATTERN_TABLE			0x0000
#define VRAM_SPRITE_ATTRIBUTE		0x3E00
#define VRAM_SPRITE_PATTERN			0x1800
	VDP_SetLayoutTable(VRAM_LAYOUT_TABLE);
	VDP_SetColorTable(VRAM_COLOR_TABLE);
	VDP_SetPatternTable(VRAM_PATTERN_TABLE);
	VDP_SetSpritePatternTable(VRAM_SPRITE_PATTERN);
	VDP_SetSpriteAttributeTable(VRAM_SPRITE_ATTRIBUTE);

	VDP_SetColor(0xF0);
	VDP_ClearVRAM();
	VDP_EnableVBlank(true);

	// Initialize text
	Print_SetTextFont(g_Font_MGL_Sample8, 128);
	VDP_FillLayout_GM2(128, 0, 0, 32, 24);
	Print_DrawText(MSX_GL " SCROLL SAMPLE");
	Print_DrawLineH(0, 1, 32);

	// Load tiles pattern
	VDP_LoadPattern_GM2(g_DataMapGM2_Patterns, 94, 0);
	VDP_LoadColor_GM2(g_DataMapGM2_Colors, 94, 0);

	// Initialize scroll module
	Scroll_Initialize((u16)g_DataMapGM2_Names);
	
	while(1)
	{
		Halt();		
		Scroll_Update();

		u8 row8 = Keyboard_Read(8);
		if(IS_KEY_PRESSED(row8, KEY_RIGHT))
		{
			Scroll_SetOffset(1);
		}
		else if(IS_KEY_PRESSED(row8, KEY_LEFT))
		{
			Scroll_SetOffset(-1);
		}
	}
}