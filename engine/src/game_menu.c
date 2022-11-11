// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██   ▄▀██ ▄█▄█ ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄█ ▀▄██ ██ █ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Game menu handler
//─────────────────────────────────────────────────────────────────────────────
#include "game_menu.h"
#include "vdp.h"
#include "string.h"

#if (MENU_USE_DEFAULT_CALLBACK)
#include "print.h"
#include "input.h"
#endif

//=============================================================================
// READ-ONLY DATA
//=============================================================================

//=============================================================================
// MEMORY DATA
//=============================================================================

const Menu*		g_MenuTable;
const Menu*		g_MenuPage;
u8				g_MenuItem;
Menu_InputCB	g_MenuInputCB;
Menu_DrawCB		g_MenuDrawCB;

#if (MENU_USE_DEFAULT_CALLBACK)
u8				g_MenuInputPrev;
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

#pragma disable_warning	244		// remove "pointer types incompatible" warning


//.............................................................................
// DEFAULT CALLBACK
//.............................................................................
#if (MENU_USE_DEFAULT_CALLBACK)

//-----------------------------------------------------------------------------
//
u8 Menu_DefaultInputCB()
{
	u8 new = 0;
	if(Keyboard_IsKeyPressed(KEY_UP))    new |= MENU_INPUT_UP;
	if(Keyboard_IsKeyPressed(KEY_DOWN))  new |= MENU_INPUT_DOWN;
	if(Keyboard_IsKeyPressed(KEY_LEFT))  new |= MENU_INPUT_LEFT;
	if(Keyboard_IsKeyPressed(KEY_RIGHT)) new |= MENU_INPUT_RIGHT;
	if(Keyboard_IsKeyPressed(KEY_SPACE)) new |= MENU_INPUT_TRIGGER;

	u8 ret = (new & g_MenuInputPrev) ^ new;
	g_MenuInputPrev = new;

	return ret;
}

//-----------------------------------------------------------------------------
//
void Menu_DefaultDrawCB(u8 x, u8 y, u8 type, const void* text)
{
}

#endif

//.............................................................................
// CORE FEATURE
//.............................................................................

//-----------------------------------------------------------------------------
//
void Menu_Initialize(const Menu* menus)
{
	g_MenuTable = menus;

	#if (MENU_USE_DEFAULT_CALLBACK)
	g_MenuInputPrev = 0xFF;
	g_MenuInputCB = Menu_DefaultInputCB;
	g_MenuDrawCB = Menu_DefaultDrawCB;
	#else
	g_MenuInputCB = NULL;
	g_MenuDrawCB = NULL;
	#endif
}

//-----------------------------------------------------------------------------
//
void Menu_DisplayItem(u8 item)
{
	MenuItem* pCurItem = &g_MenuPage->Items[item];
	u8 type = pCurItem->Type & MENU_ITEM_MASK;
	if(type == MENU_ITEM_EMPTY)
		return;

	// Get base Y coordinate
	u8 y = MENU_POS_Y + item;
	//  Clear line
	u16 dst = g_ScreenLayoutLow + MENU_POS_X + (y * MENU_SCREEN_WIDTH);
	VDP_FillVRAM(MENU_CLEAR, dst, g_ScreenLayoutHigh, MENU_WIDTH);

	// Get base X coordinate
	u8 x;
	if(type == MENU_ITEM_GOTO)
		x = MENU_ITEM_X_GOTO;
	else
		x = MENU_ITEM_X;
	if(type == MENU_ITEM_TEXT)
		x += pCurItem->Value;
	// Get align flag
	u8 align = pCurItem->Type & MENU_ITEM_ALIGN_MASK;
	if(align == MENU_ITEM_ALIGN_DEFAULT)
	{
		if(type == MENU_ITEM_GOTO)
			align = MENU_ITEM_ALIGN_GOTO;
		else
			align = MENU_ITEM_ALIGN;
	}
	// Adjust X coordinate
	if(align != MENU_ITEM_ALIGN_LEFT)
	{
		u8 len = String_Length(pCurItem->Text);
		if(align == MENU_ITEM_ALIGN_LEFT)
			x -= len;
		else // if(align == MENU_ITEM_ALIGN_CENTER)
			x -= len / 2;
	}

	// Draw cursor
	#if (MENU_CURSOR_MODE == MENU_CURSOR_MODE_CHAR)
	if(g_MenuItem == item)
	{
		Print_SetPosition(x + MENU_CURSOR_OFFSET, y);
		Print_DrawChar(MENU_CHAR_CURSOR);
	}
	#endif

	// Draw item label
	Print_SetPosition(x, y);
	Print_DrawText(pCurItem->Text);

	// Draw item value
	Print_SetPosition(MENU_VALUE_X, y);
	switch(type)
	{
		case MENU_ITEM_ACTION:
		{
			Menu_ActionCB cb = (Menu_ActionCB)pCurItem->Action;
			const c8* str = cb(MENU_ACTION_GET, pCurItem->Value);
			if(str)
			{
				if(g_MenuItem == item)
					Print_DrawChar(MENU_CHAR_LEFT);
				else
					Print_Space();
				Print_DrawText(str);
				if(g_MenuItem == item)
					Print_DrawChar(MENU_CHAR_RIGHT);
			}
			break;
		}
		case MENU_ITEM_INT:
		{
			i8* data = (i8*)pCurItem->Action;
			if(g_MenuItem == item)
				Print_DrawChar(MENU_CHAR_LEFT);
			else
				Print_Space();
			Print_DrawInt(*data);
			if(g_MenuItem == item)
				Print_DrawChar(MENU_CHAR_RIGHT);
			break;
		}
		case MENU_ITEM_BOOL:
		{
			u8* data = (u8*)pCurItem->Action;
			if(g_MenuItem == item)
				Print_DrawChar(MENU_CHAR_LEFT);
			else
				Print_Space();
			Print_DrawChar(*data ? MENU_CHAR_TRUE : MENU_CHAR_FALSE);
			if(g_MenuItem == item)
				Print_DrawChar(MENU_CHAR_RIGHT);
			break;
		}
	};
}

//-----------------------------------------------------------------------------
//
void Menu_DrawPage(u8 page)
{
	// Initialize menu
	g_MenuPage = &g_MenuTable[page];
	g_MenuItem = 0;
	while(g_MenuItem < g_MenuPage->ItemNum)
	{
		if(g_MenuPage->Items[g_MenuItem].Type < MENU_ITEM_EMPTY)
			break;
		g_MenuItem++;
	}

	// Clear
	u16 dst = g_ScreenLayoutLow + MENU_POS_X + (MENU_POS_Y * MENU_SCREEN_WIDTH);
	for(u8 i = MENU_POS_Y; i < MENU_POS_Y + MENU_HEIGHT; ++i)
	{
		VDP_FillVRAM(MENU_CLEAR, dst, g_ScreenLayoutHigh, MENU_WIDTH);
		dst += MENU_SCREEN_WIDTH;
	}

	// Display menu items
	for(u8 item = 0; item < g_MenuPage->ItemNum; ++item)
	{
		Menu_DisplayItem(item);
	}
}

//-----------------------------------------------------------------------------
//
void Menu_Update()
{
	u8 input = Menu_DefaultInputCB();
	MenuItem* pCurItem = &g_MenuPage->Items[g_MenuItem];
	u8 type = pCurItem->Type & MENU_ITEM_MASK;

	switch(type)
	{
		case MENU_ITEM_ACTION:
		{
			if(input & MENU_INPUT_TRIGGER)
			{
				Menu_ActionCB cb = (Menu_ActionCB)pCurItem->Action;
				cb(MENU_ACTION_SET, pCurItem->Value);
				Menu_DisplayItem(g_MenuItem);
			}
			else if(input & MENU_INPUT_LEFT)
			{
				Menu_ActionCB cb = (Menu_ActionCB)pCurItem->Action;
				cb(MENU_ACTION_DEC, pCurItem->Value);
				Menu_DisplayItem(g_MenuItem);
			}
			else if(input & MENU_INPUT_RIGHT)
			{
				Menu_ActionCB cb = (Menu_ActionCB)pCurItem->Action;
				cb(MENU_ACTION_INC, pCurItem->Value);
				Menu_DisplayItem(g_MenuItem);
			}
			break;
		}
		case MENU_ITEM_GOTO:
		{
			if((input & MENU_INPUT_TRIGGER) || (input & MENU_INPUT_RIGHT) || (input & MENU_INPUT_LEFT))
			{
				Menu_DrawPage(pCurItem->Value);
				return;
			}
			break;
		}
		case MENU_ITEM_INT:
		{
			if((input & MENU_INPUT_TRIGGER) || (input & MENU_INPUT_RIGHT))
			{
				i8* data = (i8*)pCurItem->Action;
				(*data)++;
				Menu_DisplayItem(g_MenuItem);
			}
			else if(input & MENU_INPUT_LEFT)
			{
				i8* data = (i8*)pCurItem->Action;
				if((*data) > pCurItem->Value)
					(*data)--;
				Menu_DisplayItem(g_MenuItem);
			}
			break;
		}
		case MENU_ITEM_BOOL:
		{
			if((input & MENU_INPUT_TRIGGER) || (input & MENU_INPUT_RIGHT) || (input & MENU_INPUT_LEFT))
			{
				u8* data = (u8*)pCurItem->Action;
				*data = 1 - *data;
				Menu_DisplayItem(g_MenuItem);
			}
			break;
		}
	}

	// Handle navigation
	if(input & MENU_INPUT_UP)
	{
		u8 i = g_MenuItem;
		while(i > 0)
		{
			i--;
			if(g_MenuPage->Items[i].Type < MENU_ITEM_EMPTY)
			{
				u8 prev = g_MenuItem;
				g_MenuItem = i;
				Menu_DisplayItem(prev);
				Menu_DisplayItem(g_MenuItem);
				break;
			}
		}
	}
	else if(input & MENU_INPUT_DOWN)
	{
		u8 i = g_MenuItem;
		while(i < g_MenuPage->ItemNum-1)
		{
			i++;
			if(g_MenuPage->Items[i].Type < MENU_ITEM_EMPTY)
			{
				u8 prev = g_MenuItem;
				g_MenuItem = i;
				Menu_DisplayItem(prev);
				Menu_DisplayItem(g_MenuItem);
				break;
			}
		}
	}

	// Update menu items
	/*for(u8 item = 0; item < g_MenuPage->ItemNum; ++item)
	{
		MenuItem* pCurItem = &g_MenuPage->Items[item];
		if(type == MENU_ITEM_UPDATE)
		{
			Menu_ActionCB cb = (Menu_ActionCB)pCurItem->Action;
			const c8* str = cb(MENU_ACTION_GET, pCurItem->Value);
			u8 x = MENU_ITEM_POS_X + pCurItem->Value;
			u8 y = MENU_POS_Y + item;
			Print_SetPosition(x, y);
			Print_DrawText(str);
		}
	}*/
}