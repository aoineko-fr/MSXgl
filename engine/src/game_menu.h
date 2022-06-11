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

// Menu special entry
#define MENU_ITEM_DISABLE		(0x00)
#define MENU_ITEM_TEXT			(0x01)
//------------------------------------
#define MENU_ITEM_UPDATE		(0x02)
#define MENU_ITEM_ACTION		(0x03)
#define MENU_ITEM_INT			(0x04)
#define MENU_ITEM_BOOL			(0x05)
//------------------------------------
#define MENU_ITEM_GOTO			(0x80) 

// Menu actions
enum MENU_ACTION
{
	MENU_ACTION_SET = 0,
	MENU_ACTION_GET,
	MENU_ACTION_INC,
	MENU_ACTION_DEC,
};

// Functions
typedef const c8* (*menuCallback)(u8, i8); ///< Callback signature

// menu entry strcture
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

