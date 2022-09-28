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
#define MSX_GL "\x02\x03\x04\x05"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Font
#include "font\font_mgl_sample8.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 ntap;

//=============================================================================
// CODE
//=============================================================================

//-----------------------------------------------------------------------------
// 
void NTapScan()
{
	ntap = NTap_Check();
	Print_SetPosition(0, 3);
	Print_DrawFormat("Players:  %i\n", ntap);
	Print_DrawFormat("NinjaTap: %i\n", (ntap == 5) ? 1 : (ntap == 8) ? 2 : 0);

	Print_SetPosition(0, 9);
	for(u8 i = 0; i < 8; ++i)
	{
		Print_DrawFormat("P%i ", i + 1);
		if(i >= ntap)
			Print_DrawText("--------");
		Print_Return();
	}
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN1); // Initialize screen mode 0 (text)
	VDP_ClearVRAM();
	VDP_SetColor(0xF0);

	Print_SetTextFont(g_Font_MGL_Sample8, 1); // Initialize font
	Print_SetColor(0xF, 0x0);

	// Header
	Print_DrawText(MSX_GL " Ninja-Tap Sample");
	Print_DrawLineH(0, 1, 32);

	Print_SetPosition(3, 7);
	Print_DrawText("AB  \x88\x89\x8B\x8A");
	Print_DrawLineH(0, 8, 12);

	// Footer
	Print_DrawLineH(0, 22, 32);
	Print_SetPosition(0, 23);
	Print_DrawText("R:Rescan");

	NTapScan();

	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();

		Print_SetPosition(31, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);

		NTap_Update();
		for(u8 i = 0; i < ntap; ++i)
		{
			Print_SetPosition(3, 9 + i);
			Print_DrawBin8(NTap_GetData(i));
		}

		if(Keyboard_IsKeyPressed(KEY_R))
			NTapScan();
	}
}