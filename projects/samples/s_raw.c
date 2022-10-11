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

enum BASIC_TYPE
{
	BASIC_TYPE_INT    = 2, // 2-byte integer type
	BASIC_TYPE_STRING = 3, // String type
	BASIC_TYPE_FLOAT  = 4, // Single precision real type
	BASIC_TYPE_DOUBLE = 8, // Double precision real type
};

//=============================================================================
// READ-ONLY DATA
//=============================================================================

//=============================================================================
// MEMORY DATA
//=============================================================================

//=============================================================================
// CODE
//=============================================================================

//-----------------------------------------------------------------------------
//
void Bios_PrintText(const c8* str)
{
	while(*str)
		Bios_TextPrintChar(*str++);
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	Bios_PrintText("Hello world!");
	// VDP_SetMode(VDP_MODE_SCREEN1); // Initialize screen mode 0 (text)
	// VDP_SetColor(0xF0);
	// VDP_ClearVRAM();

	// Print_SetTextFont(g_Font_MGL_Sample8, 1); // Initialize font
	// Print_SetColor(0xF, 0x0);

	// NTapScan();
	// NTap_Check();

	// u8 count = 0;
	// while(!Keyboard_IsKeyPressed(KEY_ESC))
	// {
		// // VDP_SetColor(0xF4);
		// Halt();
		// // VDP_SetColor(0xF0);

		// Print_SetPosition(31, 0);
		// Print_DrawChar(g_ChrAnim[count++ & 0x03]);

		// NTap_Update();
		// for(u8 i = 0; i < g_JoyNum; ++i)
		// {
			// Print_SetPosition(3, TAB_Y+2 + i);
			// Print_DrawBin8(NTap_GetData(i));
		// }

		// if(Keyboard_IsKeyPressed(KEY_R))
			// NTapScan();

		// if(Keyboard_IsKeyPressed(KEY_D))
		// {
			// g_DriverIdx++;
			// if(g_DriverIdx >= numberof(g_Drivers))
				// g_DriverIdx = 0;
			// NTapScan();
		// }
	// }
}