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

// MENU_CHAR_CLEAR
#ifndef MENU_CHAR_CLEAR
	#warning MENU_CHAR_CLEAR is not defined in "msxgl_config.h"! Default value will be used: 0
	#define MENU_CHAR_CLEAR			0
#endif

// MENU_FRAME_X
#ifndef MENU_FRAME_X
	#warning MENU_FRAME_X is not defined in "msxgl_config.h"! Default value will be used: 4
	#define MENU_FRAME_X			4
#endif

// MENU_FRAME_Y
#ifndef MENU_FRAME_Y
	#warning MENU_FRAME_Y is not defined in "msxgl_config.h"! Default value will be used: 11
	#define MENU_FRAME_Y			11
#endif

// MENU_FRAME_WIDTH
#ifndef MENU_FRAME_WIDTH
	#warning MENU_FRAME_WIDTH is not defined in "msxgl_config.h"! Default value will be used: 24
	#define MENU_FRAME_WIDTH		24
#endif

// MENU_FRAME_HEIGHT
#ifndef MENU_FRAME_HEIGHT
	#warning MENU_FRAME_HEIGHT is not defined in "msxgl_config.h"! Default value will be used: 8
	#define MENU_FRAME_HEIGHT		8
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

// MENU_TITLE_X
#ifndef MENU_TITLE_X
	#warning MENU_TITLE_X is not defined in "msxgl_config.h"! Default value will be used: MENU_FRAME_X
	#define MENU_TITLE_X			MENU_FRAME_X
#endif

// MENU_TITLE_Y
#ifndef MENU_TITLE_Y
	#warning MENU_TITLE_Y is not defined in "msxgl_config.h"! Default value will be used: MENU_FRAME_Y
	#define MENU_TITLE_Y			MENU_FRAME_Y
#endif

// MENU_ITEM_X
#ifndef MENU_ITEM_X
	#warning MENU_ITEM_X is not defined in "msxgl_config.h"! Default value will be used: MENU_FRAME_X
	#define MENU_ITEM_X				MENU_FRAME_X
#endif

// MENU_ITEM_Y
#ifndef MENU_ITEM_Y
	#warning MENU_ITEM_Y is not defined in "msxgl_config.h"! Default value will be used: MENU_FRAME_Y
	#define MENU_ITEM_Y				MENU_FRAME_Y
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

//=============================================================================
// DEFINES
//=============================================================================

// Enum: MENU_ITEM_TYPE
// Menu item types
enum MENU_ITEM_TYPE
{
	MENU_ITEM_ACTION =				0,	// Execute callback function defined in Action
	MENU_ITEM_GOTO,						// Change page to the one defined in Value
	MENU_ITEM_BACK,						// Return to the previous page
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

// Enum: MENU_ACTION
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

// Enum: MENU_INPUT_TYPE
// Menu input value
enum MENU_INPUT_TYPE
{
	MENU_INPUT_UP      = 1 << 0,
	MENU_INPUT_DOWN    = 1 << 1,
	MENU_INPUT_LEFT    = 1 << 2,
	MENU_INPUT_RIGHT   = 1 << 3,
	MENU_INPUT_TRIGGER = 1 << 4,
	MENU_INPUT_BACK    = 1 << 5,
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

// Enum: MENU_EVENT_CALLBACK
// Menu event callback
enum MENU_EVENT_CALLBACK
{
	MENU_EVENT_UP = 0,
	MENU_EVENT_DOWN,
	MENU_EVENT_SET,
	MENU_EVENT_INC,
	MENU_EVENT_DEC,
//....................................
	MENU_EVENT_DRAW_TITLE,			// Before drawing title
	MENU_EVENT_DRAW_ENTRY,			// Before drawing menu entry
	MENU_EVENT_DRAW_SELECTED,		// Before drawing selected menu entry
//....................................
	MENU_EVENT_MAX,
};

// Define: MENU_FLAG_DIRTY
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

// Menu item parameters
typedef struct MenuItem
{
	const c8* Text;		// Name of the item
	u8        Type;		// Type of the item (see <MENU_ITEM_TYPE>)
	void*     Action;	// Action associated to the item (depends on item type)
	i16       Value;	// Value associated to the item (depends on item type)
} MenuItem;

// Menu integer item min/max definition
typedef struct MenuItemMinMax
{
	i8        Min;		// Minimal value limit
	i8        Max;		// Maximal value limit
	i8        Step;		// Increment/decrement step value
} MenuItemMinMax;

// Menu parameters
typedef struct Menu
{
	const c8* Title;		// Title of the page (NULL means no title)
	MenuItem* Items;		// List of the page's menu entries
	u8        ItemNum;		// Number of the page's menu entries
	callback  Callback;		// Function to be called when page is opened
} Menu;

// Menu item parameters
typedef struct MenuData
{
	const Menu*		Table;
	const Menu*		Page;
	u8				Item;
	Menu_InputCB	InputCB;
	Menu_DrawCB		DrawCB;
	Menu_EventCB	EventCB;
	u8				PageIdx;
	u8				PrevPage;
	u8				Flag;
	MenuItem*       CurItem;
#if (MENU_USE_DEFAULT_CALLBACK)
	u8				InputPrev;
#endif
#if (MENU_SCREEN_WIDTH == MENU_VARIABLE)
	u8				ScrWidth;
#endif
} MenuData;

extern MenuData g_MenuData;

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: Menu_Initialize
// Initialize all the menu pages
// Must be called first (will reset or set default callback)
//
// Parameters:
//   menus - Table of <Menu> structures that discribe all the menu pages
void Menu_Initialize(const Menu* menus);

// Function: Menu_SetInputCallback
// Set the callback function to be called to handle menu input
//
// Parameters:
//   cb - Callback function address
inline void Menu_SetInputCallback(Menu_InputCB cb) { g_MenuData.InputCB = cb; }

// Function: Menu_SetDrawCallback
// Set the callback function to be called to handle menu item drawing
//
// Parameters:
//   cb - Callback function address
inline void Menu_SetDrawCallback(Menu_DrawCB cb) { g_MenuData.DrawCB = cb; }

// Function: Menu_SetEventCallback
// Set the callback function to be called when event occurs
//
// Parameters:
//   cb - Callback function address
inline void Menu_SetEventCallback(Menu_EventCB cb) { g_MenuData.EventCB = cb; }

// Function: Menu_SetDirty
// Set flag to force a whole menu redraw
inline void Menu_SetDirty() { g_MenuData.Flag |= MENU_FLAG_DIRTY; }

// Function: Menu_GetCurrentItem
// Get the current item in the menu
// This value is only available for the following events:
//   - MENU_EVENT_SET
//   - MENU_EVENT_INC
//   - MENU_EVENT_DEC
//   - MENU_EVENT_DRAW_ENTRY
//   - MENU_EVENT_DRAW_SELECTED
//
// Return:
//   Pointer to the current item in the menu
inline const MenuItem* Menu_GetCurrentItem() { return g_MenuData.CurItem; }

// Function: Menu_Display
// Draw a given page by its page number
//
// Parameters:
//   page - Page index to draw (index is relative to the ist passed to <Menu_Initialize> function)
void Menu_Display(u8 page);

// Function: Menu_Update
// Update the menu handler
void Menu_Update();

#if (MENU_SCREEN_WIDTH == MENU_VARIABLE)

// Function: Menu_SetScreenWidth
// Set menu screen with
//
// Parameters:
//   width - Screen width in screen mode's unit (tiles or pixels)
inline void Menu_SetScreenWidth(u8 width) { g_MenuData.ScrWidth = width; }

// Function: Menu_GetScreenWidth
// Get menu screen with
//
// Return: Screen width in screen mode's unit (tiles or pixels)
inline u8 Menu_GetScreenWidth() { return g_MenuData.ScrWidth; }

#endif
