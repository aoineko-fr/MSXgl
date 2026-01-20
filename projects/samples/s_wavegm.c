// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  VGM format sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "device/wavegame.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL						"\x02\x03\x04\x05"

#define PLAYER_Y					4

// VGM music entry
struct MusicEntry
{
	const c8* Name;
	u16       Song;
};

// Player button entry
struct ButtonEntry
{
	u8        Char;
	callback  Func;
};

// Button callbacks
void ButtonPlay();
void ButtonPause();
void ButtonStop();
void ButtonPrev();
void ButtonNext();
void ButtonLoop();

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample8.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Music list
const struct MusicEntry g_MusicEntry[] =
{
	{ "Black Fly  ", 0 },
	{ "Funky Junky", 1 },
	{ "TechTalk   ", 2 },
};

// Player button list
const struct ButtonEntry g_ButtonEntry[] =
{
	{ 0x80, ButtonPlay },
	{ 0xB8, ButtonPause },
	{ 0xB9, ButtonStop },
	{ 0xBB, ButtonPrev },
	{ 0xBA, ButtonNext },
	{ 0xBE, ButtonLoop },
};

// Color shade
const u8 g_ColorBlink[4] = { COLOR_LIGHT_RED, COLOR_MEDIUM_RED, COLOR_DARK_RED, COLOR_MEDIUM_RED };

//=============================================================================
// MEMORY DATA
//=============================================================================

u8   g_CurrentMusic = 0;
u8   g_CurrentButton;
bool g_DoLoop = TRUE;


//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void SetMusic(u8 idx)
{
	g_CurrentMusic = idx;
	const struct MusicEntry* entry = &g_MusicEntry[idx];

	WaveGame_Play(entry->Song, g_DoLoop); 
	
	Print_SetPosition(0, 2);
	Print_DrawFormat("%i/%i %s", 1 + idx, numberof(g_MusicEntry), entry->Name);
}

//-----------------------------------------------------------------------------
//
void ButtonPlay()
{
	WaveGame_Continue();
}

//-----------------------------------------------------------------------------
//
void ButtonPause()
{
	WaveGame_Pause();
}

//-----------------------------------------------------------------------------
//
void ButtonStop()
{
	WaveGame_Stop();
}

//-----------------------------------------------------------------------------
//
void ButtonPrev()
{
	if (g_CurrentMusic > 0)
		SetMusic(g_CurrentMusic - 1);
}

//-----------------------------------------------------------------------------
//
void ButtonNext()
{
	if (g_CurrentMusic < numberof(g_MusicEntry) - 1)
		SetMusic(g_CurrentMusic + 1);
}

//-----------------------------------------------------------------------------
//
void ButtonLoop()
{
	g_DoLoop = 1 - g_DoLoop;
}

//-----------------------------------------------------------------------------
//
void SetCursor(u8 id)
{
	g_CurrentButton = id % 6;
	VDP_SetSpriteSM1(0, 8 + 16*g_CurrentButton, ((PLAYER_Y+1)*8)-1, g_ButtonEntry[g_CurrentButton].Char, COLOR_LIGHT_RED);
}


//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize screen
	VDP_SetMode(VDP_MODE_SCREEN1);
	VDP_SetColor2(COLOR_BLACK, COLOR_WHITE);
	VDP_ClearVRAM();
	VDP_EnableVBlank(TRUE);

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample8, 1); // Initialize font
	Print_DrawText(MSX_GL " WAVEGAME SAMPLE");
	Print_DrawLineH(0, 1, 32);

	// Decode VGM header
	SetMusic(0);
	Print_DrawBox(0, PLAYER_Y, numberof(g_ButtonEntry) * 2 + 1, 3);
	for (u8 i = 0; i < numberof(g_ButtonEntry); ++i)
	{
		Print_DrawCharAt(1 + 2 * i, PLAYER_Y+1, g_ButtonEntry[i].Char);
	}

	if (WaveGame_GetVersion() == 1)
		Print_DrawTextAt(0, 8, "Valid MSX Pico detected");
	else
		Print_DrawTextAt(0, 8, "No valid MSX Pico detected!\nYou need firmware 1.50+ or 2.07+");
	Print_DrawTextAt(0, 12, "Loop:\n");
	
	// Footer
	Print_DrawLineH(0, 22, 32);
	Print_DrawTextAt(0, 23, "\x8D:Button \x83:Action");

	//-------------------------------------------------------------------------
	// Sprite
	
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_8 + VDP_SPRITE_SCALE_1);
	VDP_LoadSpritePattern(g_Font_MGL_Sample8 + 4, 0, g_Font_MGL_Sample8[3] - g_Font_MGL_Sample8[2]);

	SetCursor(4);

	u8 prevRow8 = 0xFF;
	u8 count = 0;
	while (1)
	{
		Halt();

		VDP_SetSpriteColorSM1(0, g_ColorBlink[(count >> 2) & 0x03]);
		
		Print_DrawCharAt(8, 12, g_DoLoop ? 0xC : 0xB);
		Print_DrawCharAt(31, 0, g_ChrAnim[count++ & 0x03]);
		
		// Handle input
		u8 row8 = Keyboard_Read(8);

		// Change button
		if (IS_KEY_PRESSED(row8, KEY_RIGHT) && !IS_KEY_PRESSED(prevRow8, KEY_RIGHT))
		{
			SetCursor(g_CurrentButton < 5 ? g_CurrentButton + 1 : 0);
		}
		else if (IS_KEY_PRESSED(row8, KEY_LEFT) && !IS_KEY_PRESSED(prevRow8, KEY_LEFT))
		{
			SetCursor(g_CurrentButton > 0 ? g_CurrentButton - 1 : 5);
		}
		// Activate button
		if (IS_KEY_PRESSED(row8, KEY_SPACE) && !IS_KEY_PRESSED(prevRow8, KEY_SPACE))
		{
			g_ButtonEntry[g_CurrentButton].Func();
		}
		
		prevRow8 = row8;
	}
}