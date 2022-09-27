// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Ninja Tap sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "device\ninjatap.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Font
#include "font\font_mgl_sample6.h"

//=============================================================================
// MEMORY DATA
//=============================================================================

//=============================================================================
// CODE
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_ClearVRAM();

	Print_SetTextFont(g_Font_MGL_Sample6, 1); // Initialize font
	Print_SetColor(0xF, 0x0);
	Print_DrawText(MSX_GL " Ninja-Tap Sample");
	Print_DrawLineH(0, 1, 40);

	u8 ntap = NTap_Check();
	Print_SetPosition(0, 3);
	Print_DrawText("Players: ");
	Print_DrawInt(ntap);

	Print_SetPosition(0, 5);
	for(u8 i = 0; i < 8; ++i)
	{
		Print_DrawFormat("P%i: ", i + 1);
		if(i >= ntap)
			Print_DrawText("--");
		Print_Return();
	}

	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		NTap_Update();
		for(u8 i = 0; i < ntap; ++i)
		{
			Print_SetPosition(4, 5 + i);
			Print_DrawBin8(NTap_Get(i));
		}

	}
}