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
#include "print.h"
#include "input.h"
#include "memory.h"

//=============================================================================
// DEFINE
//=============================================================================

#if (MENU_SCREEN_WIDTH == MENU_VARIABLE)
#define MENU_GET_SCREEN_WIDTH() g_MenuData.ScrWidth
#else
#define MENU_GET_SCREEN_WIDTH() MENU_SCREEN_WIDTH
#endif

//=============================================================================
// READ-ONLY DATA
//=============================================================================

//=============================================================================
// MEMORY DATA
//=============================================================================

MenuData g_MenuData;

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

	u8 ret = (new & g_MenuData.InputPrev) ^ new;
	g_MenuData.InputPrev = new;

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
	Mem_Set(0x00, (u8*)&g_MenuData, sizeof(MenuData));
	g_MenuData.Table = menus;
	#if (MENU_USE_DEFAULT_CALLBACK)
	g_MenuData.InputPrev = 0xFF;
	g_MenuData.InputCB = Menu_DefaultInputCB;
	g_MenuData.DrawCB = Menu_DefaultDrawCB;
	g_MenuData.EventCB = Menu_DefaultEventCB;
	#endif
}

//-----------------------------------------------------------------------------
// Display a item in the current page
void Menu_DisplayItem(u8 item)
{
	g_MenuData.CurItem = &g_MenuData.Page->Items[item];
	u8 type = g_MenuData.CurItem->Type & MENU_ITEM_MASK;
	if(type >= MENU_ITEM_EMPTY)
		return;

	g_MenuData.EventCB((g_MenuData.Item == item) ? MENU_EVENT_DRAW_SELECTED : MENU_EVENT_DRAW_ENTRY);

	// Get base Y coordinate
	u8 y = MENU_ITEM_Y + item;
	//  Clear line
	u16 dst = g_ScreenLayoutLow + MENU_FRAME_X + (y * MENU_GET_SCREEN_WIDTH());
	VDP_FillVRAM(MENU_CHAR_CLEAR, dst, g_ScreenLayoutHigh, MENU_FRAME_WIDTH);

	// Get base X coordinate
	u8 x;
	if((type == MENU_ITEM_GOTO) || (type == MENU_ITEM_BACK))
		x = MENU_ITEM_X_GOTO;
	else
		x = MENU_ITEM_X;
	if(type == MENU_ITEM_TEXT)
		x += g_MenuData.CurItem->Value;
	// Get align flag
	u8 align = g_MenuData.CurItem->Type & MENU_ITEM_ALIGN_MASK;
	if(align == MENU_ITEM_ALIGN_DEFAULT)
	{
		if((type == MENU_ITEM_GOTO) || (type == MENU_ITEM_BACK))
			align = MENU_ITEM_ALIGN_GOTO;
		else
			align = MENU_ITEM_ALIGN;
	}
	// Adjust X coordinate
	if(align != MENU_ITEM_ALIGN_LEFT)
	{
		u8 len = String_Length(g_MenuData.CurItem->Text);
		if(align == MENU_ITEM_ALIGN_RIGHT)
			x -= len;
		else // if(align == MENU_ITEM_ALIGN_CENTER)
			x -= len / 2;
	}

	// Draw cursor
	#if (MENU_CURSOR_MODE == MENU_CURSOR_MODE_CHAR)
	if(g_MenuData.Item == item)
	{
		Print_SetPosition(x + MENU_CURSOR_OFFSET, y);
		Print_DrawChar(MENU_CHAR_CURSOR);
	}
	#endif

	// Draw item label
	Print_DrawTextAt(x, y, g_MenuData.CurItem->Text);

	// Draw item value
	Print_SetPosition(MENU_VALUE_X, y);
	switch(type)
	{
		case MENU_ITEM_ACTION:
		{
			Menu_ActionCB cb = (Menu_ActionCB)g_MenuData.CurItem->Action;
			const c8* str = cb(MENU_ACTION_GET, g_MenuData.CurItem->Value);
			if(str)
			{
				if(g_MenuData.Item == item)
					Print_DrawChar(MENU_CHAR_LEFT);
				else
					Print_Space();
				Print_DrawText(str);
				if(g_MenuData.Item == item)
					Print_DrawChar(MENU_CHAR_RIGHT);
			}
			break;
		}
		case MENU_ITEM_INT:
		{
			if(g_MenuData.Item == item)
				Print_DrawChar(MENU_CHAR_LEFT);
			else
				Print_Space();
			i8* data = (i8*)g_MenuData.CurItem->Action;
			Print_DrawInt(*data);
			if(g_MenuData.Item == item)
				Print_DrawChar(MENU_CHAR_RIGHT);
			break;
		}
		case MENU_ITEM_BOOL:
		{
			u8* data = (u8*)g_MenuData.CurItem->Action;
			if(g_MenuData.Item == item)
				Print_DrawChar(MENU_CHAR_LEFT);
			else
				Print_Space();
			Print_DrawChar(*data ? MENU_CHAR_TRUE : MENU_CHAR_FALSE);
			if(g_MenuData.Item == item)
				Print_DrawChar(MENU_CHAR_RIGHT);
			break;
		}
	};
}

//-----------------------------------------------------------------------------
// Draw a given page by its page number
void Menu_Display(u8 page)
{
	// Initialize menu
	g_MenuData.PrevPage = g_MenuData.PageIdx;
	g_MenuData.PageIdx = page;
	g_MenuData.Page = &g_MenuData.Table[page];

	if(g_MenuData.Page->Callback)
		g_MenuData.Page->Callback();

	g_MenuData.Item = 0;
	while(g_MenuData.Item < g_MenuData.Page->ItemNum)
	{
		if(g_MenuData.Page->Items[g_MenuData.Item].Type < MENU_ITEM_TEXT)
			break;
		g_MenuData.Item++;
	}

	// Clear
	u16 dst = g_ScreenLayoutLow + MENU_FRAME_X + (MENU_FRAME_Y * MENU_GET_SCREEN_WIDTH());
	for(u8 i = MENU_FRAME_Y; i < MENU_FRAME_Y + MENU_FRAME_HEIGHT; ++i)
	{
		VDP_FillVRAM(MENU_CHAR_CLEAR, dst, g_ScreenLayoutHigh, MENU_FRAME_WIDTH);
		dst += MENU_GET_SCREEN_WIDTH();
	}

	// Title
	if(g_MenuData.Page->Title)
	{
		g_MenuData.EventCB(MENU_EVENT_DRAW_TITLE);
		Print_DrawTextAt(MENU_TITLE_X, MENU_TITLE_Y, g_MenuData.Page->Title);
	}

	// Display menu items
	for(u8 item = 0; item < g_MenuData.Page->ItemNum; ++item)
	{
		Menu_DisplayItem(item);
	}
}

//-----------------------------------------------------------------------------
// Update the menu handler
void Menu_Update()
{
	// Check dirty flag
	if(g_MenuData.Flag & MENU_FLAG_DIRTY)
	{
		Menu_Display(g_MenuData.PageIdx);
		g_MenuData.Flag &= ~MENU_FLAG_DIRTY; // Remove 'dirty' flag
	}

	// Update menu items
	for(u8 item = 0; item < g_MenuData.Page->ItemNum; ++item)
	{
		g_MenuData.CurItem = &g_MenuData.Page->Items[item];
		if(g_MenuData.CurItem->Type == MENU_ITEM_UPDATE)
		{
			Menu_ActionCB cb = (Menu_ActionCB)g_MenuData.CurItem->Action;
			const c8* str = cb(MENU_ACTION_UPDATE, g_MenuData.CurItem->Value);
			if(str)
			{
				u8 y = MENU_FRAME_Y + item;
				// Clear line
				u16 dst = g_ScreenLayoutLow + MENU_FRAME_X + (y * MENU_GET_SCREEN_WIDTH());
				VDP_FillVRAM(MENU_CHAR_CLEAR, dst, g_ScreenLayoutHigh, MENU_FRAME_WIDTH);
				// Update draw
				Print_DrawTextAt(MENU_ITEM_X + g_MenuData.CurItem->Value, y, str);
			}
		}
	}

	// Handle input
	u8 input = g_MenuData.InputCB();
	g_MenuData.CurItem = &g_MenuData.Page->Items[g_MenuData.Item];
	u8 type = g_MenuData.CurItem->Type & MENU_ITEM_MASK;
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
				Menu_ActionCB cb = (Menu_ActionCB)g_MenuData.CurItem->Action;
				cb(act, g_MenuData.CurItem->Value);
				Menu_DisplayItem(g_MenuData.Item);
				g_MenuData.EventCB(event);
			}
			break;
		}
		case MENU_ITEM_GOTO:
		{
			if(input & (MENU_INPUT_TRIGGER | MENU_INPUT_RIGHT | MENU_INPUT_LEFT))
			{
				Menu_Display(g_MenuData.CurItem->Value);
				g_MenuData.EventCB(MENU_EVENT_SET);
				return;
			}
			break;
		}
		case MENU_ITEM_BACK:
		{
			if(input & (MENU_INPUT_TRIGGER | MENU_INPUT_RIGHT | MENU_INPUT_LEFT))
			{
				Menu_Display(g_MenuData.PrevPage);
				g_MenuData.EventCB(MENU_EVENT_SET);
				return;
			}
			break;
		}
		case MENU_ITEM_INT:
		{
			if(input & (MENU_INPUT_TRIGGER | MENU_INPUT_RIGHT))
			{
				i8* data = (i8*)g_MenuData.CurItem->Action;
				if(g_MenuData.CurItem->Value != NULL)
				{
					MenuItemMinMax* param = (MenuItemMinMax*)g_MenuData.CurItem->Value;
					if((*data) + param->Step > param->Max)
						(*data) = param->Max;
					else
						(*data) += param->Step;
				}
				else
					(*data)++;
				Menu_DisplayItem(g_MenuData.Item);
				g_MenuData.EventCB(MENU_EVENT_INC);
			}
			else if(input & MENU_INPUT_LEFT)
			{
				i8* data = (i8*)g_MenuData.CurItem->Action;
				if(g_MenuData.CurItem->Value != NULL)
				{
					MenuItemMinMax* param = (MenuItemMinMax*)g_MenuData.CurItem->Value;
					if((*data) < param->Min + param->Step)
						(*data) = param->Min;
					else
						(*data) -= param->Step;
				}
				else
					(*data)--;
				Menu_DisplayItem(g_MenuData.Item);
				g_MenuData.EventCB(MENU_EVENT_DEC);
			}
			break;
		}
		case MENU_ITEM_BOOL:
		{
			if(input & (MENU_INPUT_TRIGGER | MENU_INPUT_RIGHT | MENU_INPUT_LEFT))
			{
				u8* data = (u8*)g_MenuData.CurItem->Action;
				*data = 1 - *data;
				Menu_DisplayItem(g_MenuData.Item);
				g_MenuData.EventCB(MENU_EVENT_SET);
			}
			break;
		}
	}

	// Handle navigation
	if(input & MENU_INPUT_UP)
	{
		u8 i = g_MenuData.Item;
		while(i > 0)
		{
			i--;
			if((g_MenuData.Page->Items[i].Type & ~MENU_ITEM_ALIGN_MASK) < MENU_ITEM_TEXT)
			{
				u8 prev = g_MenuData.Item;
				g_MenuData.Item = i;
				Menu_DisplayItem(prev);
				Menu_DisplayItem(g_MenuData.Item);
				g_MenuData.EventCB(MENU_EVENT_UP);
				break;
			}
		}
	}
	else if(input & MENU_INPUT_DOWN)
	{
		u8 i = g_MenuData.Item;
		while(i < g_MenuData.Page->ItemNum-1)
		{
			i++;
			if((g_MenuData.Page->Items[i].Type & ~MENU_ITEM_ALIGN_MASK) < MENU_ITEM_TEXT)
			{
				u8 prev = g_MenuData.Item;
				g_MenuData.Item = i;
				Menu_DisplayItem(prev);
				Menu_DisplayItem(g_MenuData.Item);
				g_MenuData.EventCB(MENU_EVENT_DOWN);
				break;
			}
		}
	}
}