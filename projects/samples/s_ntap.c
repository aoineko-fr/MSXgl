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
#define MSX_GL		"\x02\x03\x04\x05"

#define TAB_Y		8

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
	ntap = NTap_Check() & 0x0F;
	Print_SetPosition(0, 3);
	u8 info = NTap_GetInfo();
	Print_DrawFormat("Info:     %2xh\n", info);
	Print_DrawFormat("Players:  %i\n", ntap);
	Print_DrawFormat("NinjaTap: %i\n", (ntap == 5) ? 1 : (ntap == 8) ? 2 : 0);

	Print_SetPosition(0, TAB_Y + 2);
	for(u8 i = 0; i < 8; ++i)
	{
		Print_DrawFormat("P%i ", i + 1);
		Print_DrawText("--------  ");
		Print_Return();
	}

	if(info & NTAP_PORT1)
	{
		Print_DrawTextAtV(11, TAB_Y + 2, "\x19\x16\x16\x1B");
	}

	if(info & NTAP_PORT2)
	{
		u8 y = TAB_Y + 3;
		if(info & NTAP_PORT1)
			y += 3;
		Print_DrawTextAtV(11, y, "\x19\x16\x16\x1B");
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

	Print_SetPosition(3, TAB_Y);
	Print_DrawText("AB\x07\x07\x88\x89\x8B\x8A");
	Print_DrawLineH(0, TAB_Y+1, 13);

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
			Print_SetPosition(3, TAB_Y+2 + i);
			Print_DrawBin8(NTap_GetData(i));
		}

		if(Keyboard_IsKeyPressed(KEY_R))
			NTapScan();
	}
}