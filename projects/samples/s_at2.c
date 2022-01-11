// __________________________
// ██▀█▀██▀▀▀█▀▀█▀█  ▄▄▄ ▄▄  │   ▄▄▄                ▄▄      
// █  ▄ █▄ ▀██▄ ▀▄█ ██   ██  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █▀▀ ▄█  █ █ ▀█▄█ ██▄▄│  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀──────────┘                 ▀▀
//  PCM-Encoder sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "arkos\akg_player.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

// Music entry
struct MusicEntry
{
	const c8* Name;
	const u8* Data;
	u8        Segment;
};

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font\font_mgl_sample6.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Music list
const struct MusicEntry g_MusicEntry[] =
{
	{ "A Harmless Grenade", 0xA000, 4 },
	{ "Hocus Pocus       ", 0xA000, 5 },
	{ "Just add cream    ", 0xA000, 6 },
	{ "Sarkboteur        ", 0xA000, 7 },
	{ "XXXX              ", 0xA000, 8 },
};


//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_CurrentMusic = 2;


//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void SetMusic(u8 idx)
{
	g_CurrentMusic = idx;

	SET_BANK_SEGMENT(3, g_MusicEntry[idx].Segment);

	// AKG_Init(g_MusicEntry[g_CurrentMusic].Data, 0);

	Print_SetPosition(0, 2);
	Print_DrawFormat("\x8D Music #%i: %s", idx, g_MusicEntry[idx].Name);
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_ClearVRAM();
	VDP_EnableVBlank(true);

	// Header
	Print_SetTextFont(g_Font_MGL_Sample6, 1); // Initialize font
	Print_DrawText(MSX_GL "  Arkos AKG Sample");
	Print_DrawLineH(0, 1, 40);
	
	SetMusic(0);

	// Footer
	Print_DrawLineH(0, 22, 40);
	Print_SetPosition(0, 23);
	Print_DrawText("\x81\x80:Music");

	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();
		// AKG_Decode();

		Print_SetPosition(39, 0);
		u8 chr = count++ & 0x03;
		Print_DrawChar(g_ChrAnim[chr]);

		// Handle input
		u8 row8 = Keyboard_Read(8);
		if(IS_KEY_PRESSED(row8, KEY_RIGHT))
		{
			if(g_CurrentMusic < numberof(g_MusicEntry) - 1)
				SetMusic(g_CurrentMusic + 1);
		}
		else if(IS_KEY_PRESSED(row8, KEY_LEFT))
		{
			if(g_CurrentMusic > 0)
				SetMusic(g_CurrentMusic - 1);
		}
	}
}