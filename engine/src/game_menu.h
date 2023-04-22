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

//=============================================================================
// OPTIONS VALIDATION
//=============================================================================

// 
#define MENU_CURSOR_MODE_NONE		0 // No cursor
#define MENU_CURSOR_MODE_CHAR		1 // Character cursor
#define MENU_CURSOR_MODE_SPRT		2 // Sprite cursor

// MENU_USE_DEFAULT_CALLBACK
#ifndef MENU_USE_DEFAULT_CALLBACK
	#warning MENU_USE_DEFAULT_CALLBACK is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define MENU_USE_DEFAULT_CALLBACK	TRUE
#endif

// MENU_SCREEN_WIDTH
#ifndef MENU_SCREEN_WIDTH
	#warning MENU_SCREEN_WIDTH is not defined in "msxgl_config.h"! Default value will be used: 32
	#define MENU_SCREEN_WIDTH		32
#endif

// MENU_CLEAR
#ifndef MENU_CLEAR
	#warning MENU_CLEAR is not defined in "msxgl_config.h"! Default value will be used: 0
	#define MENU_CLEAR				0
#endif

// MENU_POS_X
#ifndef MENU_POS_X
	#warning MENU_POS_X is not defined in "msxgl_config.h"! Default value will be used: 4
	#define MENU_POS_X				4
#endif

// MENU_POS_Y
#ifndef MENU_POS_Y
	#warning MENU_POS_Y is not defined in "msxgl_config.h"! Default value will be used: 11
	#define MENU_POS_Y				11
#endif

// MENU_WIDTH
#ifndef MENU_WIDTH
	#warning MENU_WIDTH is not defined in "msxgl_config.h"! Default value will be used: 24
	#define MENU_WIDTH				24
#endif

// MENU_HEIGHT
#ifndef MENU_HEIGHT
	#warning MENU_HEIGHT is not defined in "msxgl_config.h"! Default value will be used: 8
	#define MENU_HEIGHT				8
#endif

// MENU_CURSOR_MODE
#ifndef MENU_CURSOR_MODE
	#warning MENU_CURSOR_MODE is not defined in "msxgl_config.h"! Default value will be used: MENU_CURSOR_MODE_CHAR
	#define MENU_CURSOR_MODE		MENU_CURSOR_MODE_CHAR
#endif

// MENU_CURSOR_OFFSET
#ifndef MENU_CURSOR_OFFSET
	#warning MENU_CURSOR_OFFSET is not defined in "msxgl_config.h"! Default value will be used: -1
	#define MENU_CURSOR_OFFSET		(-1)
#endif

// MENU_ITEM_X
#ifndef MENU_ITEM_X
	#warning MENU_ITEM_X is not defined in "msxgl_config.h"! Default value will be used: 6
	#define MENU_ITEM_X				6
#endif

// MENU_ITEM_X_GOTO
#ifndef MENU_ITEM_X_GOTO
	#warning MENU_ITEM_X_GOTO is not defined in "msxgl_config.h"! Default value will be used: MENU_ITEM_X
	#define MENU_ITEM_X_GOTO		MENU_ITEM_X
#endif

// MENU_ITEM_ALIGN
#ifndef MENU_ITEM_ALIGN
	#warning MENU_ITEM_ALIGN is not defined in "msxgl_config.h"! Default value will be used: MENU_ITEM_ALIGN_LEFT
	#define MENU_ITEM_ALIGN			MENU_ITEM_ALIGN_LEFT
#endif

// MENU_ITEM_ALIGN_GOTO
#ifndef MENU_ITEM_ALIGN_GOTO
	#warning MENU_ITEM_ALIGN_GOTO is not defined in "msxgl_config.h"! Default value will be used: MENU_ITEM_ALIGN
	#define MENU_ITEM_ALIGN_GOTO	MENU_ITEM_ALIGN
#endif

// MENU_VALUE_X
#ifndef MENU_VALUE_X
	#warning MENU_VALUE_X is not defined in "msxgl_config.h"! Default value will be used: 13
	#define MENU_VALUE_X			13
#endif

#define MENU_MAX_ITEM 20

extern u8 g_MenuFlags[MENU_MAX_ITEM];

//=============================================================================
// DEFINES
//=============================================================================

// Menu item types
enum MENU_ITEM_TYPE
{
	MENU_ITEM_ACTION =				0,	// Execute callback function defined in Action
	MENU_ITEM_GOTO,						// Change page to the one defined in Value
//------------------------------------
	MENU_ITEM_INT,						// Handle pointer to 8-bits integer defined in Action
	MENU_ITEM_BOOL,						// Handle pointer to boolean defined in Action
//------------------------------------
	MENU_ITEM_TEXT,						// Handle pointer to zero-terminated string
	MENU_ITEM_EMPTY,					// Empty entry
	MENU_ITEM_UPDATE,
//------------------------------------
	MENU_ITEM_MAX,
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
};

// Menu item actions
enum MENU_ACTION
{
	MENU_ACTION_SET = 0,
	MENU_ACTION_GET,
	MENU_ACTION_INC,
	MENU_ACTION_DEC,
	MENU_ACTION_UPDATE,
//------------------------------------
	MENU_ACTION_MAX,
	MENU_ACTION_INVALID = 0xFF,
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

// Menu event callback
enum MENU_EVENT_CALLBACK
{
	MENU_EVENT_UP = 0,
	MENU_EVENT_DOWN,
	MENU_EVENT_SET,
	MENU_EVENT_INC,
	MENU_EVENT_DEC,
//------------------------------------
	MENU_EVENT_MAX,
};

// Menu item actions
#define MENU_FLAG_DIRTY				0x01

// Action callback signature
typedef const c8* (*Menu_ActionCB)(u8, i8);

// Input callback signature
typedef u8 (*Menu_InputCB)();

// Draw callback signature
typedef void (*Menu_DrawCB)(u8, u8, u8, const void*);

// Event callback signature
typedef void (*Menu_EventCB)(u8);

// Menu item strcture
typedef struct
{
	const c8* Text;
	u8        Type;
	void*     Action;
	i16       Value;
} MenuItem;

// Menu item strcture
typedef struct
{
	i8        Min;
	i8        Max;
	i8        Step;
} MenuItemMinMax;

// Menu strcture
typedef struct
{
	const c8* Title;
	MenuItem* Items;
	u8        ItemNum;
	callback  Callback;
} Menu;

// Table of the menu pages.
extern const Menu*		g_MenuTable;
extern const Menu*		g_MenuPage;
extern u8				g_MenuItem;
extern Menu_InputCB		g_MenuInputCB;
extern Menu_DrawCB		g_MenuDrawCB;
extern Menu_EventCB		g_MenuEventCB;
extern u8				g_MenuFlag;

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: Menu_Initialize
// Initialize a menu
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

// Function: Menu_SetEventCallback
// Set the callback function to be called when event occurs
//
// Parameters:
//   cb - Callback function address
inline void Menu_SetEventCallback(Menu_EventCB cb) { g_MenuEventCB = cb; }

// Function: Menu_SetDirty
// Set flag to force a whole menu redraw
inline void Menu_SetDirty() { g_MenuFlag |= MENU_FLAG_DIRTY; }

// Function: Menu_DrawPage
// Draw a given page by its page number
//
// Parameters:
//   cb - Callback function address
void Menu_DrawPage(u8 page);

// Function: Menu_Update
// Update the menu handler
void Menu_Update();