// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Ninja Tap sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "device/ninjatap.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL		"\x02\x03\x04\x05"

//
#define TAB_Y		9

//
typedef u8 (*DetectFunc)(void);		// Callback default signature

//
struct NtapDriver
{
	const c8*  Name;
	DetectFunc Check;
};

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Font
#include "font/font_mgl_sample8.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

//
struct NtapDriver g_Drivers[] =
{
	{ "DM-System2", NTap_CheckDM },
	{ "MSXgl     ", NTap_CheckMGL },
	{ "Shinobi   ", NTap_CheckST },
};


//=============================================================================
// MEMORY DATA
//=============================================================================

//
u8 g_Info;

//
u8 g_JoyNum;

//
i8 g_DriverIdx = 2;

//=============================================================================
// CODE
//=============================================================================

//-----------------------------------------------------------------------------
//
void Print_DrawVersion(u16 ver)
{
	Print_DrawInt(ver >> 12);
	Print_DrawChar('.');
	Print_DrawInt((ver >> 6) & 0x3F);
	Print_DrawChar('.');
	Print_DrawInt(ver & 0x3F);
}

//-----------------------------------------------------------------------------
// 
const c8* NTapType(u8 type)
{
	switch(type)
	{
		case NTAP_TYPE_NINJA:   return "Ninja Tap";
		case NTAP_TYPE_SHINOBI: return "Shinobi Tap";
	}
	return "Unknow";
}

//-----------------------------------------------------------------------------
// 
void NTapScan()
{
	g_Info = g_Drivers[g_DriverIdx].Check();
	g_JoyNum = g_Info & NTAP_NUM_MASK;

	Print_Clear();

	// Header
	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL " Ninja-Tap Sample");
	Print_DrawLineH(0, 1, 32);

	Print_SetPosition(3, TAB_Y);
	Print_DrawText("ABSR\x88\x89\x8B\x8A");
	Print_DrawLineH(0, TAB_Y+1, 12);

	// Body
	Print_SetPosition(0, 3);
	Print_DrawFormat("Driver:   %s\n", g_Drivers[g_DriverIdx].Name);
	Print_DrawFormat("Info:     %2xh (%2xh,%2xh)\n", g_Info, g_NTap_Buffer[0], g_NTap_Buffer[1]);
	Print_DrawFormat("Players:  %i\n", g_JoyNum);
	Print_DrawFormat("NinjaTap: %i\n", (g_JoyNum == 5) ? 1 : (g_JoyNum == 8) ? 2 : 0);

	Print_SetPosition(0, TAB_Y + 2);
	for(u8 i = 0; i < 8; ++i)
	{
		Print_DrawFormat("P%i ", i + 1);
		Print_DrawText("--------  ");
		Print_Return();
	}

	if(g_Info & NTAP_PORT1_MASK)
	{
		Print_DrawTextAtV(11, TAB_Y + 2, "\x19\x16\x16\x1B");
		Print_DrawTextAt(12, TAB_Y + 3, NTapType(g_Info >> 4 & 0x3));
	}

	if(g_Info & NTAP_PORT2_MASK)
	{
		u8 y = TAB_Y + 3;
		if(g_Info & NTAP_PORT1_MASK)
			y += 3;
		Print_DrawTextAtV(11, y, "\x19\x16\x16\x1B");
		Print_DrawTextAt(12, y + 1, NTapType(g_Info >> 6 & 0x3));
	}

	// Footer
	Print_DrawLineH(0, 22, 32);
	Print_SetPosition(0, 23);
	Print_DrawText("R:Rescan D:Driver        ");
	Print_DrawVersion(VERSION_CURRENT);
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN1); // Initialize screen mode 0 (text)
	VDP_SetColor(0xF0);
	VDP_ClearVRAM();

	Print_SetTextFont(g_Font_MGL_Sample8, 1); // Initialize font
	Print_SetColor(0xF, 0x0);

	NTapScan();

	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		// VDP_SetColor(0xF4);
		Halt();
		// VDP_SetColor(0xF0);

		Print_SetPosition(31, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);

		NTap_Update();
		for(u8 i = 0; i < g_JoyNum; ++i)
		{
			Print_SetPosition(3, TAB_Y+2 + i);
			Print_DrawBin8(NTap_GetData(i));
		}

		if(Keyboard_IsKeyPressed(KEY_R))
			NTapScan();

		if(Keyboard_IsKeyPressed(KEY_D))
		{
			g_DriverIdx++;
			if(g_DriverIdx >= numberof(g_Drivers))
				g_DriverIdx = 0;
			NTapScan();
		}
	}
}