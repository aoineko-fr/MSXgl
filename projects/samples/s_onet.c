// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "network/obsonet.h"

//=============================================================================
// DEFINES
//=============================================================================
#define SLOT_Y 3

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts data
#include "font\font_mgl_sample6.h"

// Character animation data
const c8 g_ChrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// MEMORY DATA
//=============================================================================

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
void DisplayInfo()
{
	Print_SetPosition(0, 4);
	Print_DrawText("Slot: ");
	u8 slot = ONET_Initialize();
	if(slot == 0xFF)
	{
		Print_DrawText("Not found!");
		return;
	}
	Print_DrawSlot(slot);
	Print_DrawFormat("\nBIOS: %s", ONET_HasBIOS() ? "Yes" : "No");
	Print_DrawFormat("\nVer:  %i.%i.%i", g_ONET_Version[0], g_ONET_Version[1], g_ONET_Version[2]);
	Print_DrawFormat("\nMAC:  %2x.%2x.%2x.%2x.%2x.%2x", g_ONET_MAC[0], g_ONET_MAC[1], g_ONET_MAC[2], g_ONET_MAC[3], g_ONET_MAC[4], g_ONET_MAC[5]);
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_ClearVRAM();

	VDP_SetMode(VDP_MODE_SCREEN0);
	VDP_FillVRAM_16K(0, 0x0000, 0x4000); // Clear VRAM

	Print_SetTextFont(g_Font_MGL_Sample6, 1);
	Print_SetColor(0xF, 0x0);

	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL " ObsoNET Sample");
	Print_DrawLineH(0, 1, 40);

	DisplayInfo();

	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();

		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);
	}
}