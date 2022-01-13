// __________________________
// ██▀█▀██▀▀▀█▀▀█▀█  ▄▄▄ ▄▄  │   ▄▄▄                ▄▄      
// █  ▄ █▄ ▀██▄ ▀▄█ ██   ██  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █▀▀ ▄█  █ █ ▀█▄█ ██▄▄│  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀──────────┘                 ▀▀
//  WYZ replayer sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "PT3player/PT3player.h"

//-----------------------------------------------------------------------------
// Data

// Fonts
#include "font\font_mgl_std0.h"
#include "font\font_mgl_symbol1.h"
// Note table
#include "PT3player\PT3player_NoteTable2.h"
// Music
#include "content\pt3\A_funny_day_with_my_MSX.h"
#include "content\pt3\After2.h"
// Character animation
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

//-----------------------------------------------------------------------------
// Static variables

u8 g_VBlank = 0;
u8 g_Frame = 0;

//-----------------------------------------------------------------------------
/// H_TIMI interrupt hook
void VBlankHook()
{
	g_VBlank = 1;
	g_Frame++;

    // PT3_PlayAY();
	PT3_PlayAY();
	PT3_Decode(); 
}

/// Wait for V-Blank period
void WaitVBlank()
{
	while(g_VBlank == 0) {}
	g_VBlank = 0;
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN5);
	VDP_SetFrequency(VDP_FREQ_50HZ);
	VDP_EnableSprite(false);
	VDP_SetColor(0x4);
	VDP_CommandHMMV(0, 0, 256, 212, 0x44);

	Print_Initialize(null);
	Print_SetFontVRAM(g_Font_MGL_Std0, 212);
	Print_SetPosition(3, 2);
	Print_DrawText("PT3 PLAYER SAMPLE");
	Draw_Box(0, 0, 255, 16, 0, 0x0F);
	
	Print_SetPosition(0, 24);
	Print_DrawText("- Song #0 -\n\n");
	Print_DrawText("Name:   ");
	Print_DrawText(SONG00_name);
	Print_Return();
	Print_DrawText("Author: ");
	Print_DrawText(SONG00_author);
	Print_Return();
	Print_DrawText("Size:   ");
	Print_DrawInt(sizeof(SONG00));
	Print_DrawText(" bytes");
	
	Print_SetColor(0x0F, 0x05);
	Print_SetFont(g_Font_MGL_Symbol1);
	Print_SetMode(PRINT_MODE_DEFAULT);
	Print_SetPosition(0, 128);
	Print_DrawChar(112);
	g_PrintData.CursorX += 2;
	Print_DrawChar(113);
	g_PrintData.CursorX += 2;
	Print_DrawChar(114);
	g_PrintData.CursorX += 2;
	Print_DrawChar(117);
	g_PrintData.CursorX += 2;
	Print_DrawChar(116);
	g_PrintData.CursorX += 2;
	Print_DrawChar(121);

	NoteTable = (unsigned int) NT;
	PT3_Init();
	PT3_InitSong((u16)SONG00, true);
	// PT3_InitSong((u16)g_After2, true);
	
	Bios_SetHookCallback(H_TIMI, VBlankHook);

	Print_SetFont(g_Font_MGL_Std0);
	Print_SetMode(PRINT_MODE_VRAM);

	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		WaitVBlank();

		Print_SetPosition(255-8, 0);
		u8 chr = count++ & 0x03;
		Print_DrawChar(g_ChrAnim[chr]);		
			
		// PT3_PlayAY();
		// PT3_Decode(); 
	}

	// Bios_ClearHook(H_TIMI);
	// Bios_Exit(0);
}