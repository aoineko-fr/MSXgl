// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  PAC's SRAM sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "device/pac.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

//
#define PAGE_CURSOR 12

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Fake save data
const u8 g_DataWrite[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_Device = 0;

u8 g_Page = 0;

u8 g_Data[8];

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void Print_DrawSlot(u8 slot)
{
	Print_DrawInt(Sys_SlotGetPrimary(slot));
	if(Sys_SlotIsExpended(slot))
	{
		Print_DrawChar('-');
		Print_DrawInt(Sys_SlotGetSecondary(slot));
	}
}

//-----------------------------------------------------------------------------
// 
const c8* GetStatName(u8 stat)
{
	switch(stat)
	{
	case PAC_CHECK_EMPTY: return "Empty";
	#if (PAC_USE_SIGNATURE)
	case PAC_CHECK_APP:   return "Appli";
	#endif
	}
	return "Undef";
}

//-----------------------------------------------------------------------------
// 
void SetPage(u8 page)
{
	Print_SetPosition(0, PAGE_CURSOR + g_Page);
	Print_DrawChar(' ');

	Print_SetPosition(0, PAGE_CURSOR + page);
	Print_DrawChar(0x8A);
	g_Page = page;
}

//-----------------------------------------------------------------------------
// 
void DrawPage(u8 page)
{
	u8 stat = PAC_Check(page);
	PAC_Read(page, g_Data, 8);

	Print_SetPosition(2, PAGE_CURSOR + page);
	Print_DrawCharX(' ', 40);

	Print_SetPosition(2, PAGE_CURSOR + page);
	if(PAC_GetSlot(g_Device) != SLOT_NOTFOUND)
	{
		Print_DrawFormat("%i | %s | %2x %2x %2x %2x %2x %2x %2x %2x", page, GetStatName(stat),
			g_Data[0], g_Data[1], g_Data[2], g_Data[3], g_Data[4], g_Data[5], g_Data[6], g_Data[7]);
	}
	else
		Print_DrawText("  |       |");
}

//-----------------------------------------------------------------------------
// 
void SetDevice(u8 dev)
{
	g_Device = dev;
	PAC_Select(g_Device);
	PAC_Activate(TRUE);

	Print_SetPosition(20, 4);
	Print_DrawCharX(' ', 20);

	Print_SetPosition(20, 4);
	Print_DrawFormat(" (%i) ", g_Device);
	if(PAC_GetSlot(g_Device) == SLOT_NOTFOUND)
		Print_DrawText("Not found");
	else
		Print_DrawSlot(PAC_GetSlot(g_Device));

	SetPage(0);
	for(u8 i = 0; i < PAC_PAGE_MAX; ++i)
		DrawPage(i);

	Print_SetPosition(0, PAGE_CURSOR + PAC_PAGE_MAX);
	Print_DrawText("----+-------+-------------------------");
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	Bios_SetKeyClick(FALSE);

	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_ClearVRAM();

	Print_SetTextFont(g_Font_MGL_Sample6, 1); // Initialize font
	Print_DrawText(MSX_GL " PAC SRAM sample");
	Print_DrawLineH(0, 1, 40);

	bool bFound = PAC_Initialize();

	Print_SetPosition(0, 3);
	Print_DrawText("Devices:");
	Print_SetPosition(20, 3);
	Print_DrawFormat("Current:");

	for(u8 i = 0; i < PAC_DEVICE_MAX; ++i)
	{
		Print_SetPosition(0, 4 + i);
		Print_DrawFormat(" %i: ", i);
		if(PAC_GetSlot(i) == SLOT_NOTFOUND)
			Print_DrawText("Not found");
		else
			Print_DrawSlot(PAC_GetSlot(i));
	}

	Print_SetPosition(0, PAGE_CURSOR - 2);
	Print_DrawText("Page  Stat    Data");
	Print_SetPosition(0, PAGE_CURSOR - 1);
	Print_DrawText("----+-------+-------------------------");
	Print_SetPosition(0, PAGE_CURSOR + PAC_PAGE_MAX);
	Print_DrawText("----+-------+-------------------------");

	Print_DrawLineH(0, 22, 40);
	Print_SetPosition(0, 23);
	Print_DrawText("\x8D:Device \x82:Page W:Write E:Erase A:All");

	SetDevice(0);
	
	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();

		if(Keyboard_IsKeyPressed(KEY_UP))
		{
			SetPage((g_Page + PAC_PAGE_MAX - 1) % PAC_PAGE_MAX);
			while(Keyboard_IsKeyPressed(KEY_UP)) {}
		}
		if(Keyboard_IsKeyPressed(KEY_DOWN))
		{
			SetPage((g_Page + 1) % PAC_PAGE_MAX);
			while(Keyboard_IsKeyPressed(KEY_DOWN)) {}
		}

		if(Keyboard_IsKeyPressed(KEY_RIGHT))
		{
			g_Device = (g_Device + 1) % PAC_DEVICE_MAX;
			SetDevice(g_Device);
		}
		if(Keyboard_IsKeyPressed(KEY_LEFT))
		{
			g_Device = (g_Device + PAC_DEVICE_MAX - 1) % PAC_DEVICE_MAX;
			SetDevice(g_Device);
		}
		if(Keyboard_IsKeyPressed(KEY_W))
		{
			PAC_Write(g_Page, g_DataWrite, sizeof(g_DataWrite));
			DrawPage(g_Page);
		}
		if(Keyboard_IsKeyPressed(KEY_E))
		{
			PAC_Format(g_Page);
			DrawPage(g_Page);
		}
		if(Keyboard_IsKeyPressed(KEY_A))
		{
			PAC_FormatAll();
			SetDevice(g_Device);
		}

		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);
	}
}