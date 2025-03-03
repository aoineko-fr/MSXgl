// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██   ▄▀██ ▄█▄█ ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄█ ▀▄██ ██ █ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Game menu handler
//─────────────────────────────────────────────────────────────────────────────
#include "game/menu.h"
#include "vdp.h"
#include "string.h"

// #if (MENU_USE_DEFAULT_CALLBACK)
#include "print.h"
#include "input.h"
// #endif

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
Menu_EventCB	g_MenuEventCB;
u8				g_MenuPageIdx;
u8				g_MenuFlag;

#if (MENU_USE_DEFAULT_CALLBACK)
u8				g_MenuInputPrev;
#endif

#if (MENU_SCREEN_WIDTH == MENU_VARIABLE)
u8				g_MenuScrWidth;
#define MENU_GET_SCREEN_WIDTH() g_MenuScrWidth
#else
#define MENU_GET_SCREEN_WIDTH() MENU_SCREEN_WIDTH
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
	x;
	y;
	type;
	text;
}

//-----------------------------------------------------------------------------
//
void Menu_DefaultEventCB(u8 event)
{
	event;
}

#endif

//.............................................................................
// CORE FEATURE
//.............................................................................

//-----------------------------------------------------------------------------
// Initialize a menu
void Menu_Initialize(const Menu* menus)
{
	g_MenuTable = menus;
	g_MenuFlag = 0;
	#if (MENU_USE_DEFAULT_CALLBACK)
	g_MenuInputPrev = 0xFF;
	g_MenuInputCB = Menu_DefaultInputCB;
	g_MenuDrawCB = Menu_DefaultDrawCB;
	g_MenuEventCB = Menu_DefaultEventCB;
	#else
	g_MenuInputCB = NULL;
	g_MenuDrawCB = NULL;
	g_MenuEventCB = NULL;
	#endif
}

//-----------------------------------------------------------------------------
// Display a item in the current page
void Menu_DisplayItem(u8 item)
{
	MenuItem* pCurItem = &g_MenuPage->Items[item];
	u8 type = pCurItem->Type & MENU_ITEM_MASK;
	if(type >= MENU_ITEM_EMPTY)
		return;

	// Get base Y coordinate
	u8 y = MENU_ITEM_Y + item;
	//  Clear line
	u16 dst = g_ScreenLayoutLow + MENU_FRAME_X + (y * MENU_GET_SCREEN_WIDTH());
	VDP_FillVRAM(MENU_CHAR_CLEAR, dst, g_ScreenLayoutHigh, MENU_FRAME_WIDTH);

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
		if(align == MENU_ITEM_ALIGN_RIGHT)
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
	Print_DrawTextAt(x, y, pCurItem->Text);

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
			if(g_MenuItem == item)
				Print_DrawChar(MENU_CHAR_LEFT);
			else
				Print_Space();
			i8* data = (i8*)pCurItem->Action;
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
// Draw a given page by its page number
void Menu_DrawPage(u8 page)
{
	// Initialize menu
	g_MenuPageIdx = page;
	g_MenuPage = &g_MenuTable[page];

	if(g_MenuPage->Callback)
		g_MenuPage->Callback();

	g_MenuItem = 0;
	while(g_MenuItem < g_MenuPage->ItemNum)
	{
		if(g_MenuPage->Items[g_MenuItem].Type < MENU_ITEM_TEXT)
			break;
		g_MenuItem++;
	}

	// Clear
	u16 dst = g_ScreenLayoutLow + MENU_FRAME_X + (MENU_FRAME_Y * MENU_GET_SCREEN_WIDTH());
	for(u8 i = MENU_FRAME_Y; i < MENU_FRAME_Y + MENU_FRAME_HEIGHT; ++i)
	{
		VDP_FillVRAM(MENU_CHAR_CLEAR, dst, g_ScreenLayoutHigh, MENU_FRAME_WIDTH);
		dst += MENU_GET_SCREEN_WIDTH();
	}

	// Title
	if (g_MenuPage->Title)
	{
		Print_DrawTextAt(MENU_TITLE_X, MENU_TITLE_Y, g_MenuPage->Title);
	}

	// Display menu items
	for(u8 item = 0; item < g_MenuPage->ItemNum; ++item)
	{
		Menu_DisplayItem(item);
	}
}

//-----------------------------------------------------------------------------
// Update the menu handler
void Menu_Update()
{
	// Check dirty flag
	if(g_MenuFlag & MENU_FLAG_DIRTY)
	{
		Menu_DrawPage(g_MenuPageIdx);
		g_MenuFlag &= ~MENU_FLAG_DIRTY; // Remove 'dirty' flag
	}

	// Update menu items
	for(u8 item = 0; item < g_MenuPage->ItemNum; ++item)
	{
		MenuItem* pCurItem = &g_MenuPage->Items[item];
		if(pCurItem->Type == MENU_ITEM_UPDATE)
		{
			Menu_ActionCB cb = (Menu_ActionCB)pCurItem->Action;
			const c8* str = cb(MENU_ACTION_UPDATE, pCurItem->Value);
			if(str)
			{
				u8 y = MENU_FRAME_Y + item;
				// Clear line
				u16 dst = g_ScreenLayoutLow + MENU_FRAME_X + (y * MENU_GET_SCREEN_WIDTH());
				VDP_FillVRAM(MENU_CHAR_CLEAR, dst, g_ScreenLayoutHigh, MENU_FRAME_WIDTH);
				// Update draw
				Print_SetPosition(MENU_ITEM_X + pCurItem->Value, y);
				Print_DrawText(str);
			}
		}
	}

	// Handle input
	u8 input = g_MenuInputCB();
	MenuItem* pCurItem = &g_MenuPage->Items[g_MenuItem];
	u8 type = pCurItem->Type & MENU_ITEM_MASK;
	switch(type)
	{
		case MENU_ITEM_ACTION:
		{
			u8 act = MENU_ACTION_INVALID;
			u8 event = 0xFF;
			if(input & MENU_INPUT_TRIGGER)
			{
				act = MENU_ACTION_SET;
				event = MENU_EVENT_SET;
			}
			else if(input & MENU_INPUT_LEFT)
			{
				act = MENU_ACTION_DEC;
				event = MENU_EVENT_DEC;
			}
			else if(input & MENU_INPUT_RIGHT)
			{
				act = MENU_ACTION_INC;
				event = MENU_EVENT_INC;
			}
			if(act != MENU_ACTION_INVALID)
			{
				Menu_ActionCB cb = (Menu_ActionCB)pCurItem->Action;
				cb(act, pCurItem->Value);
				Menu_DisplayItem(g_MenuItem);
				g_MenuEventCB(event);
			}
			break;
		}
		case MENU_ITEM_GOTO:
		{
			if(input & (MENU_INPUT_TRIGGER | MENU_INPUT_RIGHT| MENU_INPUT_LEFT))
			{
				Menu_DrawPage(pCurItem->Value);
				g_MenuEventCB(MENU_EVENT_SET);
				return;
			}
			break;
		}
		case MENU_ITEM_INT:
		{
			if(input & (MENU_INPUT_TRIGGER | MENU_INPUT_RIGHT))
			{
				i8* data = (i8*)pCurItem->Action;
				if(pCurItem->Value != NULL)
				{
					MenuItemMinMax* param = (MenuItemMinMax*)pCurItem->Value;
					if((*data) + param->Step > param->Max)
						(*data) = param->Max;
					else
						(*data) += param->Step;
				}
				else
					(*data)++;
				Menu_DisplayItem(g_MenuItem);
				g_MenuEventCB(MENU_EVENT_INC);
			}
			else if(input & MENU_INPUT_LEFT)
			{
				i8* data = (i8*)pCurItem->Action;
				if(pCurItem->Value != NULL)
				{
					MenuItemMinMax* param = (MenuItemMinMax*)pCurItem->Value;
					if((*data) < param->Min + param->Step)
						(*data) = param->Min;
					else
						(*data) -= param->Step;
				}
				else
					(*data)--;
				Menu_DisplayItem(g_MenuItem);
				g_MenuEventCB(MENU_EVENT_DEC);
			}
			break;
		}
		case MENU_ITEM_BOOL:
		{
			if(input & (MENU_INPUT_TRIGGER | MENU_INPUT_RIGHT | MENU_INPUT_LEFT))
			{
				u8* data = (u8*)pCurItem->Action;
				*data = 1 - *data;
				Menu_DisplayItem(g_MenuItem);
				g_MenuEventCB(MENU_EVENT_SET);
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
			if((g_MenuPage->Items[i].Type & ~MENU_ITEM_ALIGN_MASK) < MENU_ITEM_TEXT)
			{
				u8 prev = g_MenuItem;
				g_MenuItem = i;
				Menu_DisplayItem(prev);
				Menu_DisplayItem(g_MenuItem);
				g_MenuEventCB(MENU_EVENT_UP);
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
			if((g_MenuPage->Items[i].Type & ~MENU_ITEM_ALIGN_MASK) < MENU_ITEM_TEXT)
			{
				u8 prev = g_MenuItem;
				g_MenuItem = i;
				Menu_DisplayItem(prev);
				Menu_DisplayItem(g_MenuItem);
				g_MenuEventCB(MENU_EVENT_DOWN);
				break;
			}
		}
	}
}