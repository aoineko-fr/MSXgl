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

#define MENU_USE_DEFAULT_CALLBACK	TRUE

// Menu layout
#define MENU_SCREEN_WIDTH			32

#define MENU_CLEAR					0

#define MENU_CHAR_CURSOR			'@'
#define MENU_CHAR_TRUE				'O'
#define MENU_CHAR_FALSE				'X'
#define MENU_CHAR_LEFT				'<'
#define MENU_CHAR_RIGHT				'>'

#define MENU_POS_X					4
#define MENU_POS_Y					11
#define MENU_WIDTH					24
#define MENU_HEIGHT					8

#define MENU_CURSOR_MODE_NONE		0
#define MENU_CURSOR_MODE_CHAR		1
#define MENU_CURSOR_MODE_SPRT		2
#define MENU_CURSOR_MODE			MENU_CURSOR_MODE_CHAR
#define MENU_CURSOR_OFFSET			(-1)

#define MENU_ITEM_X					5
#define MENU_ITEM_X_GOTO			5
#define MENU_ITEM_ALIGN				MENU_ITEM_ALIGN_LEFT
#define MENU_ITEM_ALIGN_GOTO		MENU_ITEM_ALIGN_LEFT

#define MENU_VALUE_X				12

// Menu item types
enum MENU_ITEM_TYPE
{
	MENU_ITEM_ACTION =				0, // Execute callback function defined in Action
	MENU_ITEM_GOTO,					   // Change page to the one defined in Value
//------------------------------------
	MENU_ITEM_INT,					   // Handle pointer to 8-bits integer defined in Action
	MENU_ITEM_BOOL,					   // Handle pointer to boolean defined in Action
//------------------------------------
	MENU_ITEM_EMPTY,				   // Empty entry
	MENU_ITEM_TEXT,					   // Handle pointer to zero-terminated string
	MENU_ITEM_UPDATE,
//------------------------------------
	MENU_ITEM_MASK =				0x0F,
//------------------------------------
	MENU_ITEM_ALIGN_DEFAULT =		0x00,
	MENU_ITEM_ALIGN_LEFT =			0x10,
	MENU_ITEM_ALIGN_CENTER =		0x20,
	MENU_ITEM_ALIGN_RIGHT =			0x30,
	MENU_ITEM_ALIGN_MASK =			0x30,
//------------------------------------
	MENU_ITEM_DISABLE =				0x80, // Item is disabled
//------------------------------------
	MENU_ITEM_MAX,
};

// Menu item actions
enum MENU_ACTION
{
	MENU_ACTION_SET = 0,
	MENU_ACTION_GET,
	MENU_ACTION_INC,
	MENU_ACTION_DEC,
//------------------------------------
	MENU_ACTION_MAX,
};

// Menu item layut
enum MENU_LAYOUT
{
	MENU_LAYOUT_LEFT = 0,
	MENU_LAYOUT_RIGHT,
	MENU_LAYOUT_CENTER,
//------------------------------------
	MENU_LAYOUT_MAX,
};

// Menu input value
enum MENU_INPUT_TYPE
{
	MENU_INPUT_UP      = 1 << 0,
	MENU_INPUT_DOWN    = 1 << 1,
	MENU_INPUT_LEFT    = 1 << 2,
	MENU_INPUT_RIGHT   = 1 << 3,
	MENU_INPUT_TRIGGER = 1 << 4,
	MENU_INPUT_5       = 1 << 5,
	MENU_INPUT_6       = 1 << 6,
	MENU_INPUT_7       = 1 << 7,
};

// Menu draw callback
enum MENU_DRAW_CALLBACK
{
	MENU_DRAW_TEXT = 0,
	MENU_DRAW_INT,
	MENU_DRAW_BOOL,
	MENU_DRAW_CURSOR,
//------------------------------------
	MENU_DRAW_MAX,
};

// Action callback signature
typedef const c8* (*Menu_ActionCB)(u8, i8);

// Input callback signature
typedef u8 (*Menu_InputCB)();

// Draw callback signature
typedef void (*Menu_DrawCB)(u8, u8, u8, const void*);

// Menu item strcture
typedef struct
{
	const c8* Text;
	u8        Type;
	void*     Action;
	i8        Value;
} MenuItem;

// Menu strcture
typedef struct
{
	const c8* Title;
	MenuItem* Items;
	u8        ItemNum;
} Menu;

// Table of the menu pages.
extern const Menu*		g_MenuTable;
extern const Menu*		g_MenuPage;
extern u8				g_MenuItem;
extern Menu_InputCB		g_MenuInputCB;
extern Menu_DrawCB		g_MenuDrawCB;

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: Menu_Initialize
//
// Must be called first (will reset or set default callback)
void Menu_Initialize(const Menu* menus);

// Function: Menu_SetInputCallback
// Set the callback function to be called to handle menu input
//
// Parameters:
//   cb - Callback function address
inline void Menu_SetInputCallback(Menu_InputCB cb) { g_MenuInputCB = cb; }

// Function: Menu_SetDrawCallback
// Set the callback function to be called to handle menu item drawing
//
// Parameters:
//   cb - Callback function address
inline void Menu_SetDrawCallback(Menu_DrawCB cb) { g_MenuDrawCB = cb; }

// Function: Menu_DrawPage
//
void Menu_DrawPage(u8 page);

// Function: Menu_Update
//
void Menu_Update();
