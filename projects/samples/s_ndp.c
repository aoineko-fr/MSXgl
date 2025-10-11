// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  NDP player sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "ndp/ndp_player.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x02\x03\x04\x05"

// Player Y coordinate
#define COPY_Y						7
#define TITLE_X						0
#define TITLE_Y						7
#define PLAYER_Y					3
#define QR_X						1
#define QR_Y						13
#define QR_PATTERN					192

// Player button entry
typedef struct
{
	u8        Char;
	callback  Func;
} ButtonEntry;

// VGM music entry
typedef struct
{
	const c8* Name;
	void*     Song;
	void*     FX;
} MusicEntry;

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

// Song data
// DemoSongs by Naruto2413. All right reserved.
// https://naruto2413.bandcamp.com/album/ndp-demosongs
#include "content/ndp/NDP03.h"
#include "content/ndp/NDP04.h"
#include "content/ndp/NDP05.h"
#include "content/ndp/NDP09.h"
#include "content/ndp/NDP10.h"
#include "content/ndp/SE.h"
#include "content/img/naruto2413_qrcode.h"

// Music list
const MusicEntry g_MusicEntry[] =
{
	{ "NDP Demo 03", g_NDP_NDP03, NULL },
	{ "NDP Demo 04", g_NDP_NDP04, NULL },
	{ "NDP Demo 05", g_NDP_NDP05, NULL },
	{ "NDP Demo 09", g_NDP_NDP09, NULL },
	{ "NDP Demo 10", g_NDP_NDP10, NULL },
};

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Player button list
const ButtonEntry g_ButtonEntry[] =
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
u8   g_CurrentSFX = 0;
u8   g_CurrentButton;
bool g_Freq50Hz = FALSE;
bool g_DoLoop = TRUE;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void SetMusic(u8 idx)
{
	g_CurrentMusic = idx;
	const MusicEntry* entry = &g_MusicEntry[idx];

	NDP_Stop();
	NDP_SetMusicData(entry->Song); // Set the song data address
	NDP_Play(); // Start playing
	
	Print_SetPosition(TITLE_X, TITLE_Y);
	Print_DrawFormat("Music %i/%i: %s", 1 + idx, numberof(g_MusicEntry), entry->Name);
}

//-----------------------------------------------------------------------------
//
void SetSFX(u8 idx)
{
	g_CurrentSFX = idx;

	NDP_PlaySFX(g_CurrentSFX);
	
	Print_SetPosition(TITLE_X, TITLE_Y + 1);
	Print_DrawFormat("SFX %i/%i", 1 + idx, NDP_GetSFXNumber());
}

//-----------------------------------------------------------------------------
//
void ButtonPlay()
{
	NDP_Play();
}

//-----------------------------------------------------------------------------
//
void ButtonPause()
{
	NDP_Stop();
}

//-----------------------------------------------------------------------------
//
void ButtonStop()
{
	NDP_Stop();
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
	// g_DoLoop = 1 - g_DoLoop;
	// WYZ_SetLoop(g_DoLoop);
}

//-----------------------------------------------------------------------------
//
void SetCursor(u8 id)
{
	g_CurrentButton = id;
	VDP_SetSpriteSM1(0, 8 + 16*g_CurrentButton, ((PLAYER_Y+1)*8)-1, g_ButtonEntry[g_CurrentButton].Char, COLOR_LIGHT_RED);
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN1); // Initialize screen mode 1 (GM1)
	VDP_SetColor(COLOR_BLACK);
	VDP_ClearVRAM();

	Print_SetTextFont(g_Font_MGL_Sample8, 1); // Initialize font
	Print_SetColor(COLOR_WHITE, COLOR_BLACK);
	Print_DrawTextAt(0, 0, MSX_GL" NDP SAMPLE"); // Display title
	Print_DrawLineH(0, 1, 32);

	Print_SetPosition(0, QR_Y - 2);
	Print_DrawText("https://naruto2413.bandcamp.com");

	VDP_WriteVRAM(g_QRCode_Patterns, g_ScreenPatternLow + (QR_PATTERN * 8), g_ScreenPatternHigh, 64 * 8);
	loop(i, 64)
	{
		u8 x = QR_X + (i & 0x7);
		u8 y = QR_Y + (i >> 3);
		VDP_Poke(/*g_QRCode_Names[i] +*/ i + QR_PATTERN, g_ScreenLayoutLow + (y * 32) + x, g_ScreenLayoutHigh);
	}

	Print_SetPosition(0, COPY_Y);
	Print_DrawTextAt(QR_X + 9, QR_Y + 1, "DemoSongs");
	Print_DrawTextAt(QR_X + 9, QR_Y + 3, "Naruto2413 \x0D 2024");
	Print_DrawTextAt(QR_X + 9, QR_Y + 5, "All right reserved");

	NDP_Initialize(); // Initialize the NDP player
	NDP_SetSFXData(g_NDS_SE); // Set the SFX data address

	// Decode VGM header
	SetMusic(1);
	Print_DrawBox(0, PLAYER_Y, numberof(g_ButtonEntry) * 2 + 1, 3);
	for (u8 i = 0; i < numberof(g_ButtonEntry); ++i)
	{
		Print_SetPosition(1 + 2 * i, PLAYER_Y+1);
		Print_DrawChar(g_ButtonEntry[i].Char);		
	}

	// Footer
	Print_DrawLineH(0, 22, 32);
	Print_DrawTextAt(0, 23, "\x8D:Button \x83:Action \x82:SFX");

	// Sprite
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_8 + VDP_SPRITE_SCALE_1);
	VDP_LoadSpritePattern(g_Font_MGL_Sample8 + 4, 0, g_Font_MGL_Sample8[3] - g_Font_MGL_Sample8[2]);

	SetCursor(4);

	u8 prevRow8 = 0xFF;
	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();

// VDP_SetColor(COLOR_DARK_BLUE);
		NDP_Update();
// VDP_SetColor(COLOR_BLACK);

		Print_DrawCharAt(31, 0, g_ChrAnim[count++ & 0x03]);

		VDP_SetSpriteColorSM1(0, g_ColorBlink[(count >> 2) & 0x03]);

		// Handle input
		u8 row8 = Keyboard_Read(8);

		// Change button
		if (IS_KEY_PUSHED(row8, prevRow8, KEY_RIGHT))
		{
			SetCursor((g_CurrentButton < numberof(g_ButtonEntry) - 1) ? g_CurrentButton + 1 : 0);
		}
		else if (IS_KEY_PUSHED(row8, prevRow8, KEY_LEFT))
		{
			SetCursor((g_CurrentButton > 0) ? g_CurrentButton - 1 : numberof(g_ButtonEntry) - 1);
		}
		if (IS_KEY_PUSHED(row8, prevRow8, KEY_UP))
		{
			SetSFX(g_CurrentSFX < NDP_GetSFXNumber() - 1 ? g_CurrentSFX++ : 0);
		}
		else if (IS_KEY_PUSHED(row8, prevRow8, KEY_DOWN))
		{
			SetSFX(g_CurrentSFX > 0 ? g_CurrentSFX-- : NDP_GetSFXNumber() - 1);
		}
		// Activate button
		if (IS_KEY_PUSHED(row8, prevRow8, KEY_SPACE))
		{
			g_ButtonEntry[g_CurrentButton].Func();
		}

		prevRow8 = row8;
	}

	NDP_Release(); // Release the NDP player
}