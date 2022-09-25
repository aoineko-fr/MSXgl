// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_ClearVRAM();

	Print_SetTextFont(PRINT_DEFAULT_FONT, 1); // Initialize font (use BIOS font)
	Print_DrawText("Hello MSX world!");

	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
	}
}