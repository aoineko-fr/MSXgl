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
#include "print.h"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

//=============================================================================
// MEMORY DATA
//=============================================================================

const Menu*		g_MenuTable;
const Menu*		g_MenuPage;
u8				g_MenuItem;
Menu_InputCB	g_MenuCB[MENU_CALLBACK_MAX];

//=============================================================================
// FUNCTIONS
//=============================================================================

#pragma disable_warning	244		// remove "pointer types incompatible" warning

//-----------------------------------------------------------------------------
//
void Menu_DisplayItem(u8 item)
{
	// Clean buttom-third
	u16 dst = g_ScreenLayoutLow + ((MENU_ITEMS_Y + item) * MENU_SCREEN_WIDTH);
	VDP_FillVRAM_16K(0, dst, MENU_SCREEN_WIDTH);
	
	// Draw item
	if(item < g_MenuPage->itemNum)
	{
		MenuEntry* pCurEntry = &g_MenuPage->items[item];
		// if(g_MenuItem == item)
			// Print_SelectTextFont(g_DataFont, OFFSET_TITLE_FONT_ALT);
		// else
			// Print_SelectTextFont(g_DataFont, OFFSET_TITLE_FONT_DEF);
			
		u8 x;
		if((pCurEntry->type & MENU_ITEM_GOTO) != 0)
			x = MENU_ITEMS_X_GOTO;
		else
			x = MENU_ITEMS_X;
		 
		u8 y = MENU_ITEMS_Y;
		
		if(pCurEntry->type == MENU_ITEM_TEXT)
			x += pCurEntry->value;
			
		Print_SetPosition(x, y + item);
		Print_DrawText(pCurEntry->text);

		Print_SetPosition(x + 9, y + item);

		if(pCurEntry->type == MENU_ITEM_ACTION)
		{
			Menu_ActionCB cb = (Menu_ActionCB)pCurEntry->action;
			const c8* str = cb(MENU_ACTION_GET, pCurEntry->value);
			if(str)
			{
				if(g_MenuItem == item)
					Print_DrawChar('<');
				else
					Print_Space();
				Print_DrawText(str);
				if(g_MenuItem == item)
					Print_DrawChar('>');
			}
		}
		else if(pCurEntry->type == MENU_ITEM_INT)
		{
			i8* data = (u8*)pCurEntry->action;
			if(g_MenuItem == item)
				Print_DrawChar('<');
			else
				Print_Space();
			Print_DrawInt(*data);
			if(g_MenuItem == item)
				Print_DrawChar('>');
		}
		else if(pCurEntry->type == MENU_ITEM_BOOL)
		{
			u8* data = (u8*)pCurEntry->action;
			if(g_MenuItem == item)
				Print_DrawChar('<');
			else
				Print_Space();
			Print_DrawChar(*data ? '/' : '\\');
			if(g_MenuItem == item)
				Print_DrawChar('>');
		}
	}
}

//-----------------------------------------------------------------------------
//
void Menu_Display(u8 page)
{
	// Initialize menu
	g_MenuPage = &g_MenuTable[page];
	g_MenuItem = 0;
	while(g_MenuItem < g_MenuPage->itemNum)
	{
		if(g_MenuPage->items[g_MenuItem].type >= MENU_ITEM_ACTION)
			break;
		g_MenuItem++;
	}
	
	// Display menu items
	for(u8 item = 0; item < MENU_ITEMS_H; ++item)
	{
		Menu_DisplayItem(item);
	}
}

//-----------------------------------------------------------------------------
//
void Menu_Update()
{
	// Handle activation
	MenuEntry* pCurEntry = &g_MenuPage->items[g_MenuItem];
	if(g_MenuCB[MENU_CALLBACK_TRIGGER] && g_MenuCB[MENU_CALLBACK_TRIGGER]())
	{
		// ayFX_PlayBank(14, 0);
		if(pCurEntry->type == MENU_ITEM_ACTION)
		{
			Menu_ActionCB cb = (Menu_ActionCB)pCurEntry->action;
			cb(MENU_ACTION_SET, pCurEntry->value);
		}
		else if(pCurEntry->type == MENU_ITEM_INT)
		{
			i8* data = (u8*)pCurEntry->action;
			*data++;
		}
		else if(pCurEntry->type == MENU_ITEM_BOOL)
		{
			u8* data = (u8*)pCurEntry->action;
			*data = 1 - *data;
		}
		Menu_DisplayItem(g_MenuItem);
		if((pCurEntry->type & MENU_ITEM_GOTO) != 0)
		{
			Menu_Display(pCurEntry->type & 0x7F);
		}
		return;
	}

	// Handle change value
	if(g_MenuCB[MENU_CALLBACK_RIGHT] && g_MenuCB[MENU_CALLBACK_RIGHT]())
	{
		if(pCurEntry->type == MENU_ITEM_ACTION)
		{
			Menu_ActionCB cb = (Menu_ActionCB)pCurEntry->action;
			cb(MENU_ACTION_INC, pCurEntry->value);
		}
		else if(pCurEntry->type == MENU_ITEM_INT)
		{
			i8* data = (u8*)pCurEntry->action;
			*data++;
		}
		else if(pCurEntry->type == MENU_ITEM_BOOL)
		{
			u8* data = (u8*)pCurEntry->action;
			*data = 1 - *data;
		}
		Menu_DisplayItem(g_MenuItem);
		// ayFX_PlayBank(14, 0);
	}
	else if(g_MenuCB[MENU_CALLBACK_LEFT] && g_MenuCB[MENU_CALLBACK_LEFT]())
	{
		if(pCurEntry->type == MENU_ITEM_ACTION)
		{
			Menu_ActionCB cb = (Menu_ActionCB)pCurEntry->action;
			cb(MENU_ACTION_DEC, pCurEntry->value);
		}
		else if(pCurEntry->type == MENU_ITEM_INT)
		{
			i8* data = (u8*)pCurEntry->action;
			*data--;
		}
		else if(pCurEntry->type == MENU_ITEM_BOOL)
		{
			u8* data = (u8*)pCurEntry->action;
			*data = 1 - *data;
		}
		Menu_DisplayItem(g_MenuItem);
		// ayFX_PlayBank(14, 0);
	}
	
	// Handle navigation
	if(g_MenuCB[MENU_CALLBACK_UP] && g_MenuCB[MENU_CALLBACK_UP]())
	{
		u8 i = g_MenuItem;
		while(i > 0)
		{
			i--;
			if(g_MenuPage->items[i].type >= MENU_ITEM_ACTION)
			{
				u8 prev = g_MenuItem;
				g_MenuItem = i;
				Menu_DisplayItem(prev);
				Menu_DisplayItem(g_MenuItem);
				break;
			}
		}			
	}
	else if((g_MenuCB[MENU_CALLBACK_DOWN] && g_MenuCB[MENU_CALLBACK_DOWN]()))
	{
		u8 i = g_MenuItem;
		while(i < g_MenuPage->itemNum-1)
		{
			i++;
			if(g_MenuPage->items[i].type >= MENU_ITEM_ACTION)
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
	/*if((g_GameFrame & 0x07) == 0) // 8th frame
	{
		Print_SelectTextFont(g_DataFont, OFFSET_TITLE_FONT_DEF);
		for(u8 item = 0; item < g_MenuPage->itemNum; ++item)
		{
			MenuEntry* pCurEntry = &g_MenuPage->items[item];
			if(pCurEntry->type == MENU_ITEM_UPDATE)
			{
				Menu_ActionCB cb = (Menu_ActionCB)pCurEntry->action;
				const c8* str = cb(MENU_ACTION_GET, pCurEntry->value);
				u8 x = MENU_ITEMS_X;
				x += pCurEntry->value;			
				Print_SetPosition(x, MENU_ITEMS_Y + item);
				Print_DrawText(str);
			}
		}
	}*/
}