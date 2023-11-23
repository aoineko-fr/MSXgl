// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "localize.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Localization
#include "s_loc_data.h"

// Fonts
#include "font/font_mgl_sample6.h"

// Sign-of-life animation
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// MEMORY DATA
//=============================================================================

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize screen mode 0 (text)
	VDP_SetMode(VDP_MODE_SCREEN0);
	VDP_ClearVRAM();

	// Initialize font (use BIOS font)
	Print_SetTextFont(g_Font_MGL_Sample6, 1);
	Print_DrawText(MSX_GL " Localization sample");
	Print_DrawLineH(0, 1, 40);

	// Initialize the localization module
	Loc_Initialize(g_LocData, TEXT_MAX);
	loop(l, LANG_MAX)
	{
		Loc_SetLangage(l);
		loop(t, TEXT_MAX)
			Print_DrawTextAt(1 + l * 12, 5 + t, Loc_GetText(t) ? Loc_GetText(t) : "--");
	}

	// Main loop
	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();

		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);
	}
}