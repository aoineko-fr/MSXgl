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
	MENU_MAX,      // Number of menu
};

// Prototype of functions used by the menu entries
const c8* MenuAction_Start(u8 op, i8 value);
const c8* MenuAction_Screen(u8 op, i8 value);

typedef struct
{
	const c8* Name;
	u8        Mode;  
} ModeInfo;

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Font
#include "font/font_mgl_sample6.h"
#include "font/font_mgl_sample8.h"

// Entries description for the Main menu
const MenuItem g_MenuMain[] =
{
	{ "Start",               MENU_ITEM_ACTION, MenuAction_Start, 0 },  // Entry to start a game
	{ "Continue",            MENU_ITEM_ACTION, MenuAction_Start, 1 },  // Entry to continue a game (same function is called but with '1' as value)
	{ "Options",             MENU_ITEM_GOTO, NULL, MENU_OPTION },      // Entry to go to another menu page
	{ NULL,                  MENU_ITEM_EMPTY, NULL, 0 },               // Blank entry to create a gap
	{ "Exit",                MENU_ITEM_ACTION, MenuAction_Start, -1 }, // Entry to exit the game
};

// Entries description for the Option menu
MenuItem g_MenuOption[] =
{
	{ "Screen",              MENU_ITEM_ACTION, MenuAction_Screen, 0 }, // Entry to change the screen mode
	{ NULL,                  MENU_ITEM_EMPTY, NULL, 0 },               // Blank entry to create a gap
	{ "Back",                MENU_ITEM_GOTO, NULL, MENU_MAIN },        // Entry to go back to the main menu
};

// List of all menus
const Menu g_Menus[MENU_MAX] =
{
	{ "Main",    g_MenuMain,    numberof(g_MenuMain),    NULL }, // MENU_MAIN
	{ "Options", g_MenuOption,  numberof(g_MenuOption),  NULL }, // MENU_OPTION
};

const ModeInfo g_ModeInfos[] =
{
	{ "T1", VDP_MODE_SCREEN0 }, 
	{ "G1", VDP_MODE_SCREEN1 }, 
	{ "G1", VDP_MODE_SCREEN2 }, 
};

//=============================================================================
// MEMORY DATA
//=============================================================================

bool g_Exit = FALSE;

bool g_ScreenId = 0;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
const c8* MenuAction_Start(u8 op, i8 value)
{
	if (op == MENU_ACTION_SET)
	{
		switch(value)
		{
		case 0: // Start
			break;
		case 1: // Continue
			break;
		case -1: // Exit
			g_Exit = TRUE;
			break;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Program entry point
const c8* MenuAction_Screen(u8 op, i8 value)
{
	value;
	if(op == MENU_ACTION_GET)
	{
		return g_ModeInfos[g_ScreenId].Name;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_ClearVRAM();

	Print_SetTextFont(g_Font_MGL_Sample6, 1); // Initialize font
	Print_SetColor(COLOR_WHITE, COLOR_DARK_BLUE);

	// Header
	Print_DrawTextAt(0, 0, MSX_GL6 " Menu Sample");
	Print_DrawLineH(0, 1, 32);


	Menu_Initialize(g_Menus);
	// Menu_SetInputCallback(Menu_HandleInput);
	// Menu_SetDrawCallback(Menu_HandleDraw);
	// Menu_SetEventCallback(Menu_HandleEvent);
	Menu_DrawPage(MENU_MAIN);

	while(!g_Exit)
	{
		// Wait for screen synchronization (50 or 60 Hz)
		Halt();

		// Update menu
		Menu_Update();
	}
}