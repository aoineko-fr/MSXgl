// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "ndp/ndp_player.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"

// Song data
#include "content/ndp/RHYTHM_TRACK.h"
#include "content/ndp/LR.h"
#include "content/ndp/DDS2LABY.h"
#include "content/ndp/DRSP-AR1.h"
#include "content/ndp/DSEED-1.h"
#include "content/ndp/F1SP_3.h"

// Animation characters
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
	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_ClearVRAM();

	Print_SetTextFont(g_Font_MGL_Sample6, 1); // Initialize font
	Print_DrawText(MSX_GL " NDP SAMPLE"); // Display title
	Print_DrawLineH(0, 1, 40);

	NDP_Initialize(); // Initialize the NDP player
	NDP_SetMusicAddress(g_NDP_RHYTHM_TRACK); // Set the song data address
	NDP_Play(); // Start playing

	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();

		Print_DrawCharAt(39, 0, g_ChrAnim[count++ & 0x03]);
// VDP_SetColor(COLOR_DARK_BLUE);
		NDP_Update();
// VDP_SetColor(COLOR_BLACK);
	}

	NDP_Release(); // Release the NDP player
}