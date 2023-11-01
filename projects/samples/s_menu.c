// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "game_menu.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL6		"\x01\x02\x03\x04\x05\x06"
#define MSX_GL8		"\x02\x03\x04\x05"

// Index of all menu pages
enum MENU_PAGES
{
	MENU_MAIN = 0, // Main page
	MENU_OPTION,   // Options page
	MENU_ALIGN,    // Align page
	MENU_MAX,      // Number of menu
};

// Prototype of functions used by the menu entries
const c8* MenuAction_Screen(u8 op, i8 value);
const c8* MenuAction_Start(u8 op, i8 value);

typedef struct
{
	const c8* Name;
	u8        Mode;  
	u8        Width;
	const u8* Font;
	const c8* Title;
	u8        Color;
} ModeInfo;

void SetupScreen();

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Font
#include "font/font_mgl_sample6.h"
#include "font/font_mgl_sample8.h"

// Entries description for the Main menu
const MenuItem g_MenuMain[] =
{
	{ "Start",               MENU_ITEM_ACTION, MenuAction_Start, 1 }, // Entry to start a game (will trigger MenuAction_Start with value equal to '1')
	{ "Options",             MENU_ITEM_GOTO, NULL, MENU_OPTION },     // Entry to go to Option menu page
	{ "Align",               MENU_ITEM_GOTO, NULL, MENU_ALIGN },      // Entry to go to Align menu page
	{ NULL,                  MENU_ITEM_EMPTY, NULL, 0 },              // Blank entry to create a gap
	{ "Exit",                MENU_ITEM_ACTION, MenuAction_Start, 0 }, // Entry to exit the game (will trigger MenuAction_Start with value equal to '0')
};

// Entries description for the Option menu
MenuItem g_MenuOption[] =
{
	{ "Mode",                MENU_ITEM_ACTION, MenuAction_Screen, 0 }, // Entry to change the screen mode (will trigger MenuAction_Screen)
	{ "Integer",             MENU_ITEM_INT, &g_Integer, 0 },           // Entry to edit an integer
	{ "Boolean",             MENU_ITEM_BOOL, &g_Boolean, 0 },          // Entry to edit a boolean
	{ NULL,                  MENU_ITEM_EMPTY, NULL, 0 },               // Blank entry to create a gap
	{ "Back",                MENU_ITEM_GOTO, NULL, MENU_MAIN },        // Entry to go back to the main menu
};

// Entries description for the Align menu
MenuItem g_MenuAlign[] =
{
	{ "Left",                MENU_ITEM_TEXT+MENU_ITEM_ALIGN_LEFT,   NULL, 0 }, // Entry display a text aligned to left
	{ "Center",              MENU_ITEM_TEXT+MENU_ITEM_ALIGN_CENTER, NULL, 0 }, // Entry display a text aligned to center
	{ "Right",               MENU_ITEM_TEXT+MENU_ITEM_ALIGN_RIGHT,  NULL, 0 }, // Entry display a text aligned to right
	{ NULL,                  MENU_ITEM_EMPTY, NULL, 0 },                       // Blank entry to create a gap
	{ "Back",                MENU_ITEM_GOTO, NULL, MENU_MAIN },                // Entry to go back to the main menu
};

// List of all menus
const Menu g_Menus[MENU_MAX] =
{
	{ "Main",    g_MenuMain,   numberof(g_MenuMain),   NULL }, // MENU_MAIN
	{ "Options", g_MenuOption, numberof(g_MenuOption), NULL }, // MENU_OPTION
	{ "Align",   g_MenuAlign,  numberof(g_MenuAlign),  NULL }, // MENU_ALIGN
};

const ModeInfo g_ModeInfos[] =
{
	{ "Screen 0 (T1)", VDP_MODE_SCREEN0, 40, g_Font_MGL_Sample6, MSX_GL6 " Menu Sample", COLOR_DARK_BLUE },
	{ "Screen 1 (G1)", VDP_MODE_SCREEN1, 32, g_Font_MGL_Sample8, MSX_GL8 " Menu Sample", COLOR_DARK_GREEN },
	{ "Screen 2 (G2)", VDP_MODE_SCREEN2, 32, g_Font_MGL_Sample8, MSX_GL8 " Menu Sample", COLOR_DARK_RED },
};

//=============================================================================
// MEMORY DATA
//=============================================================================

// Test to end the program
bool g_Exit = FALSE;

// Current screen mode index
u8 g_ScreenId = 0;

// A dummy integer value to be modified in menu
u8 g_Integer = 0;

// A dummy boolean value to be modified in menu
bool g_Boolean = TRUE;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
const c8* MenuAction_Screen(u8 op, i8 value)
{
	value;
	switch (op) // 'op' represents the operation performed on the menu entry when this function is called.
	{
		case MENU_ACTION_GET: // Recovers the value of the input to be displayed.
			return g_ModeInfos[g_ScreenId].Name;

		case MENU_ACTION_SET: // Manages trigger button pressing
		case MENU_ACTION_INC: // Manages right direction pressing
			g_ScreenId = (g_ScreenId + 1) % numberof(g_ModeInfos);
			SetupScreen();
			break;

		case MENU_ACTION_DEC: // Manages left direction pressing
			g_ScreenId = (g_ScreenId + numberof(g_ModeInfos) - 1) % numberof(g_ModeInfos);
			SetupScreen();
			break;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Program entry point
const c8* MenuAction_Start(u8 op, i8 value)
{
	if (op == MENU_ACTION_SET) // Manages trigger button pressing
	{
		switch(value) // 'value' is defined in the menu entries table (so the same callback function can be called several times with different values)
		{
		case 0: // Exit value
			g_Exit = TRUE;
			break;

		case 1: // Game start value 
			break;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Program entry point
void SetupScreen()
{
	const ModeInfo* info = &g_ModeInfos[g_ScreenId];

	// Initialize screen mode
	VDP_SetColor(info->Color); // Change background color
	VDP_SetMode(info->Mode); // Set screen mode
	VDP_ClearVRAM(); // Clean the whole VRAM

	// Initialize font
	Print_SetTextFont(info->Font, 1); // Set font characters
	Print_SetColor(COLOR_WHITE, info->Color); // Set font color

	// Header text
	Print_DrawTextAt(0, 0, info->Title);
	Print_DrawLineH(0, 1, info->Width);

	// Force menu redraw
	Menu_SetDirty(); // Force current page to be redrawn
	Menu_SetScreenWidth(info->Width); // Set the current screen width (need MENU_SCREEN_WIDTH to be set to MENU_VARIABLE)
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	SetupScreen();

	Menu_Initialize(g_Menus); // Initialize the menu
	// Menu_SetInputCallback(Menu_HandleInput);
	// Menu_SetDrawCallback(Menu_HandleDraw);
	// Menu_SetEventCallback(Menu_HandleEvent);
	Menu_DrawPage(MENU_MAIN); // Display the first page

	while(!g_Exit)
	{
		// Wait for screen synchronization (50 or 60 Hz)
		Halt();

		// Update menu
		Menu_Update();
	}
}