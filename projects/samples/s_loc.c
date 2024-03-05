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

// Screen character size
#define SRCSIZE 32

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Localization
#include "content/loc_data.h"

// Fonts
#include "font/font_bios_latin.h"
#include "font/font_bios_jap2.h"
#include "font/font_bios_hangul.h"
#include "font/font_bios_cyrillic.h"
#include "font/font_bios_arabic1.h"
#include "font/font_mgl_kana8.h"
#include "font/font_mgl_makoto.h"
#include "font/font_1re1_1.h"

// Font data
const Font g_Fonts[] = 
{
	{ "BIOS",         NULL,               1, 0x17 }, // 0
	{ "European MSX", g_Font_BIOS_EU,     0, 0x17 }, // 1
	{ "Japanese MSX", g_Font_BIOS_JP2,    0, 0x17 }, // 2
	{ "Korean MSX",   g_Font_BIOS_Hangul, 0, 0x17 }, // 3
	{ "Cyrillic MSX", g_Font_BIOS_Cyril,  0, 0x17 }, // 4
	{ "Arabic MSX",   g_Font_BIOS_ARB1,   0, 0x17 }, // 5
	{ "Makoto",       g_Font_MGL_Makoto,  0, 0x17 }, // 6
	{ "1re1",         g_Font_1re1_1,      0, 0x17 }, // 7
	{ "Kana8",        g_Font_MGL_Kana8,   0, 0x17 }, // 8
};

// 
const u8 g_DefaultFont[LANG_MAX] = 
{
	/* LANG_EN */ 1,
	/* LANG_FR */ 1,
	/* LANG_JA */ 2,
	/* LANG_DA */ 1,
	/* LANG_NL */ 1,
	/* LANG_FI */ 1,
	/* LANG_DE */ 1,
	/* LANG_IS */ 1,
	/* LANG_GA */ 1,
	/* LANG_IT */ 1,
	/* LANG_PL */ 1,
	/* LANG_PT */ 1,
	/* LANG_ES */ 1,
	/* LANG_UK */ 4,
	/* LANG_SK */ 1,
	/* LANG_KO */ 3,
	/* LANG_AR */ 5,
	/* LANG_RU */ 4,
	/* LANG_SV */ 1,
	/* LANG_TR */ 1,
};

// Sign-of-life animation
const const c8* g_ChrAnim = "-/|\\";

//=============================================================================
// MEMORY DATA
//=============================================================================

// Current language
u8 g_CurLang = LANG_JA;

// Current character font
u8 g_CurFont = 1;

// Callback
callback g_Display;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// 
void SetLanguage(u8 lang)
{
	g_CurLang = lang;
	g_CurFont = g_DefaultFont[lang];
	Loc_SetLanguage(lang);
}

//-----------------------------------------------------------------------------
// 
void VDP_SetTileColorGM1(u8 group, u8 color)
{
	VDP_Poke_16K(color, VDP_GetColorTable() + group);
}

//-----------------------------------------------------------------------------
// 
void DisplayHeader()
{
	const Font* font = &g_Fonts[g_CurFont];

	// Initialize screen mode 1
	VDP_SetMode(VDP_MODE_GRAPHIC1);
	VDP_SetColor(COLOR_BLACK);
	VDP_ClearVRAM();

	// Initialize language and font
	Print_SetTextFont(font->Data, font->Offset);
	Print_SetColor(COLOR_WHITE, COLOR_TRANSPARENT);
	VDP_SetTileColorGM1(2, (u8)COLOR_GRAY << 4);
	VDP_SetTileColorGM1(3, (u8)COLOR_GRAY << 4);
	VDP_SetTileColorGM1(4, (u8)COLOR_LIGHT_YELLOW << 4);
	VDP_SetTileColorGM1(5, (u8)COLOR_LIGHT_YELLOW << 4);
	VDP_SetTileColorGM1(6, (u8)COLOR_LIGHT_YELLOW << 4);
	VDP_SetTileColorGM1(7, (u8)COLOR_LIGHT_YELLOW << 4);

	// Display header
	Print_DrawTextAt(0, 0, Loc_GetText(TEXT_TITLE));
	Print_DrawCharXAt(0, 1, font->Line, SRCSIZE);
}

//-----------------------------------------------------------------------------
// 
void DisplayFooter()
{
	const Font* font = &g_Fonts[g_CurFont];

	// Display footer
	Print_DrawCharXAt(0, 22, font->Line, SRCSIZE);
	Print_SetPosition(0, 23);
	Print_DrawFormat("<>:%s ^v:%s", Loc_GetText(TEXT_LANG_LABEL), Loc_GetText(TEXT_FONT_LABEL));
}

//-----------------------------------------------------------------------------
// 
void DisplayFont()
{
	const Font* font = &g_Fonts[g_CurFont];
	Loc_SetLanguage(g_CurLang);

	DisplayHeader();

	// Language information
	Print_SetPosition(0, 2);
	Print_DrawFormat("-%s:%s (%s)", Loc_GetText(TEXT_LANG_LABEL), Loc_GetText(TEXT_LANG_VALUE), Loc_GetText(TEXT_LANG_VALUE_EN));

	// Sample text
	Print_SetPosition(0, 3);
	Print_DrawFormat("-%s:%s\n", Loc_GetText(TEXT_PANGRAM_LABEL), Loc_GetText(TEXT_PANGRAM_VALUE));
	Print_DrawFormat("-%s:%s", Loc_GetText(TEXT_GLYPH_LABEL), Loc_GetText(TEXT_GLYPH_VALUE));

	// Display font
	Print_DrawCharXAt(0, 12, font->Line, SRCSIZE);
	Print_SetPosition(0, 13);
	Print_DrawFormat("-%s:%s\n", Loc_GetText(TEXT_FONT_LABEL), font->Name);
	u16 addr = VDP_GetLayoutTable() + 14 * SRCSIZE;
	loop(i, 255)
		VDP_Poke_16K(i, addr++);

	DisplayFooter();
}

//-----------------------------------------------------------------------------
// 
// void DisplayARB()
// {
// 	SetLanguage(LANG_AR);

// 	DisplayHeader();

// 	Print_DrawTextAlignAt(31,  4, Loc_GetText(TEXT_SAMPLE_1), PRINT_ALIGN_RIGHT);
// 	Print_DrawTextAlignAt(31,  5, Loc_GetText(TEXT_SAMPLE_2), PRINT_ALIGN_RIGHT);
// 	Print_DrawTextAlignAt(31,  6, Loc_GetText(TEXT_SAMPLE_3), PRINT_ALIGN_RIGHT);
// 	Print_DrawTextAlignAt(31,  7, Loc_GetText(TEXT_SAMPLE_4), PRINT_ALIGN_RIGHT);
// 	Print_DrawTextAlignAt(31,  8, Loc_GetText(TEXT_SAMPLE_5), PRINT_ALIGN_RIGHT);
// 	Print_DrawTextAlignAt(31,  9, Loc_GetText(TEXT_SAMPLE_6), PRINT_ALIGN_RIGHT);
// 	Print_DrawTextAlignAt(31, 10, Loc_GetText(TEXT_SAMPLE_7), PRINT_ALIGN_RIGHT);

// 	DisplayFooter();
// }

//-----------------------------------------------------------------------------
// 
void DisplayTrans()
{
	const Font* font = &g_Fonts[g_CurFont];
	Loc_SetLanguage(g_CurLang);

	DisplayHeader();
	DisplayFooter();

	// Information
	Print_SetPosition(0, 3);
	loop(i, 5)
	{
		Loc_SetLanguage(i);
		Print_DrawFormat("%s: %s\n\n", Loc_GetText(TEXT_LANG_VALUE_EN), Loc_GetText(TEXT_PANGRAM_VALUE));
	}
}

//-----------------------------------------------------------------------------
// 
void Update()
{
	// Change language
	if(Keyboard_IsKeyPressed(KEY_RIGHT))
	{
		SetLanguage((g_CurLang + 1) % LANG_MAX);
		g_Display();
	}
	else if(Keyboard_IsKeyPressed(KEY_LEFT))
	{
		SetLanguage((g_CurLang + LANG_MAX - 1) % LANG_MAX);
		g_Display();
	}

	// Change font
	if(Keyboard_IsKeyPressed(KEY_UP))
	{
		g_CurFont = (g_CurFont + 1) % numberof(g_Fonts);
		g_Display();
	}
	else if(Keyboard_IsKeyPressed(KEY_DOWN))
	{
		g_CurFont = (g_CurFont + numberof(g_Fonts) - 1) % numberof(g_Fonts);
		g_Display();
	}

	// Change font
	if(Keyboard_IsKeyPressed(KEY_F1))
	{
		g_Display = DisplayFont;
		g_Display();
	}
	else if(Keyboard_IsKeyPressed(KEY_F2))
	{
		g_Display = DisplayTrans;
		g_Display();
	}
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize the localization module
	Loc_Initialize(g_LocData, TEXT_MAX);
	SetLanguage(LANG_JA);

	// Render the page information
	// g_Display = DisplayARB;
	g_Display = DisplayFont;
	g_Display();

	// Main loop
	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		// Screen synchronization
		Halt();

		// Display sign-of-life
		Print_SetPosition(SRCSIZE - 1, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);

		Update();
	}
}