// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "localize.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

// Font data description
typedef struct
{
	const c8* Name;
	const u8* Data;
	u8        Offset;
	u8        Line;
} Font;

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Localization
#include "content/loc_data.h"

// Fonts
#include "font/font_mgl_kana8.h"
#include "font/font_mgl_makoto.h"
#include "font/font_ibm.h"

// Sign-of-life animation
const const c8* g_ChrAnim = "-/|\\";

// Font data
const Font g_Fonts[] = 
{
	{ "BIOS",   NULL,              1, 0x17 },
	{ "Kana8",  g_Font_MGL_Kana8,  0, 0x17 },
	{ "Makoto", g_Font_MGL_Makoto, 0, 0x17 },
	{ "IBM",    g_Font_IBM,        1, 0xC4 },
};

//=============================================================================
// MEMORY DATA
//=============================================================================

// Current language
u8 g_CurLang = LANG_JA;

// Current character font
u8 g_CurFont = 1;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// 
void DisplayPage()
{
	const Font* font = &g_Fonts[g_CurFont];

	// Initialize screen mode 1 (text)
	VDP_SetMode(VDP_MODE_SCREEN1);
	VDP_ClearVRAM();

	// Initialize language and font
	Loc_SetLanguage(g_CurLang);
	Print_SetTextFont(font->Data, font->Offset);

	// Display header
	Print_DrawTextAt(0, 0, Loc_GetText(TEXT_TITLE));
	Print_DrawCharXAt(0, 1, font->Line, 32);

	// Information
	Print_SetPosition(0, 3);
	Print_DrawFormat("%s:%s", Loc_GetText(TEXT_LANGUAGE), Loc_GetText(TEXT_CURRENTLANG));
	Print_SetPosition(0, 5);
	Print_DrawFormat("%s:%s", Loc_GetText(TEXT_FONT), font->Name);

	// Display font
	loop(i, 255)
		VDP_Poke_16K(i, VDP_GetLayoutTable() + (7 * 32) + i);

	Print_SetPosition(0, 16);
	Print_DrawFormat("%s:%s", Loc_GetText(TEXT_SAMPLE), Loc_GetText(TEXT_EXAMPLE));

	// Display footer
	Print_DrawCharXAt(0, 22, font->Line, 32);
	Print_SetPosition(0, 23);
	Print_DrawFormat("<>:%s ^v:%s", Loc_GetText(TEXT_LANGUAGE), Loc_GetText(TEXT_FONT));
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize the localization module
	Loc_Initialize(g_LocData, TEXT_MAX);

	// Render the page information
	DisplayPage();

	// Main loop
	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		// Screen synchronization
		Halt();

		// Display sign-of-life
		Print_SetPosition(31, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);

		// Change language
		if(Keyboard_IsKeyPressed(KEY_RIGHT))
		{
			g_CurLang = (g_CurLang + 1) % LANG_MAX;
			DisplayPage();
		}
		else if(Keyboard_IsKeyPressed(KEY_LEFT))
		{
			g_CurLang = (g_CurLang + LANG_MAX - 1) % LANG_MAX;
			DisplayPage();
		}

		// Change font
		if(Keyboard_IsKeyPressed(KEY_UP))
		{
			g_CurFont = (g_CurFont + 1) % numberof(g_Fonts);
			DisplayPage();
		}
		else if(Keyboard_IsKeyPressed(KEY_DOWN))
		{
			g_CurFont = (g_CurFont + numberof(g_Fonts) - 1) % numberof(g_Fonts);
			DisplayPage();
		}
	}
}