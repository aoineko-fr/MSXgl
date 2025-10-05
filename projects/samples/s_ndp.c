// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "ndp/ndp_player.h"

// Song data
#include "content/ndp/RHYTHM_TRACK.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

extern void NDP_RAM0;
extern void NDP_RAM0END;
extern void NDP_RAMVAR;
extern void NDP_RAMVAREND;
extern void NDP_RAMVAL;
extern void NDP_RAMVALEND;
extern u16 NDP_RAM0SIZE;
extern u16 NDP_RAMVARSIZE;

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_ClearVRAM();

	Print_SetTextFont(PRINT_DEFAULT_FONT, 1); // Initialize font (use BIOS font)
	Print_DrawText("MSXgl - NDP Sample");

	Mem_Set(0x00, &NDP_RAM0, NDP_RAM0SIZE); // Clear NDP work area
	Mem_Copy(&NDP_RAMVAL, &NDP_RAMVAR, NDP_RAMVARSIZE);

	NDP_Initialize(); // Initialize the NDP player
	NDP_SetMusicAddress(g_NDP_RHYTHM_TRACK + 7); // Set the song data address
	NDP_Play(); // Start playing

	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);
	}

	NDP_Release(); // Release the NDP player
}