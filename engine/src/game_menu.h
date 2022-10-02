// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄           
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██   ▄▀██ ▄█▄█ ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄█ ▀▄██ ██ █ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Game menu handler
//─────────────────────────────────────────────────────────────────────────────
#pragma once
#include "core.h"

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

// Menu layout
#define MENU_ITEMS_X				8
#define MENU_ITEMS_X_GOTO			12
#define MENU_ITEMS_Y				14
#define MENU_ITEMS_H				(24-MENU_ITEMS_Y)
#define MENU_SCREEN_WIDTH			32


// Menu special entry
#define MENU_ITEM_DISABLE		(0x00)
#define MENU_ITEM_TEXT			(0x01)
//------------------------------------
#define MENU_ITEM_UPDATE		(0x02)
#define MENU_ITEM_ACTION		(0x03)
#define MENU_ITEM_INT			(0x04)
#define MENU_ITEM_BOOL			(0x05)
//------------------------------------
#define MENU_ITEM_GOTO			(0x80) // Bits 0~7 is menu page index

// Menu actions
enum MENU_ACTION
{
	MENU_ACTION_SET = 0,
	MENU_ACTION_GET,
	MENU_ACTION_INC,
	MENU_ACTION_DEC,
};

// Callback type
enum MENU_CALLBACK
{
	MENU_CALLBACK_UP = 0,
	MENU_CALLBACK_DOWN,
	MENU_CALLBACK_LEFT,
	MENU_CALLBACK_RIGHT,
	MENU_CALLBACK_TRIGGER,
//------------------------------------
	MENU_CALLBACK_MAX,
};

// Action callback signature
typedef const c8* (*Menu_ActionCB)(u8, i8);

// Input callback signature
typedef bool (*Menu_InputCB)();

// Menu entry strcture
typedef struct
{
	const c8* text;
	u8        type;
	void*     action;
	i8        value;
} MenuEntry;

// Menu strcture
typedef struct
{
	const c8*  title;
	MenuEntry* items;
	u8         itemNum;
} Menu;

// Table of the menu pages.
extern const Menu*		g_MenuTable;
extern const Menu*		g_MenuPage;
extern u8				g_MenuItem;
extern Menu_InputCB		g_MenuCB[MENU_CALLBACK_MAX];

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: Menu_SetCallback
// Set current 
//
// Parameters:
//   id - Callback ID (see <MENU_CALLBACK>)
//   cb - Callback function address
inline void Menu_SetCallback(u8 id, Menu_InputCB cb) { g_MenuCB[id] = cb; }

//
inline void Menu_Initialize(const Menu* menus) { g_MenuTable = menus; }

//
void Menu_Display(u8 page);

//
void Menu_Update();
