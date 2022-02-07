// __________________________
// ██▀█▀██▀▀▀█▀▀█▀█  ▄▄▄ ▄▄  │   ▄▄▄                ▄▄      
// █  ▄ █▄ ▀██▄ ▀▄█ ██   ██  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █▀▀ ▄█  █ █ ▀█▄█ ██▄▄│  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀──────────┘                 ▀▀
//  VGM format sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "psg.h"
#include "ayvgm\ayvgm_player.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x02\x03\x04\x05"

//
#define LOG_SIZE 16

// VGM music entry
struct MusicEntry
{
	const c8* Name;
	const u8* Data;
	u8        Segment;
};

// Player button entry
struct ButtonEntry
{
	u8        Char;
	callback  Func;
};

// Button callbacks
void ButtonPlay();
void ButtonPause();
void ButtonStop();
void ButtonPrev();
void ButtonNext();
void ButtonLoop();

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font\font_mgl_sample8.h"

// Music
#include "content\ayvgm\ayvgm_psg_ds4_03.h"
#include "content\ayvgm\ayvgm_psg_goemon07.h"
// #include "content\ayvgm\ayvgm_psg_penguin_12.h"
#include "content\ayvgm\ayvgm_psg_metalgear_05.h"
#include "content\ayvgm\ayvgm_psg_honotori_09.h"
// #include "content\ayvgm\ayvgm_scc_quarth_02.h"
// #include "content\ayvgm\ayvgm_scc_f1spirit_01.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Music list
const struct MusicEntry g_MusicEntry[] =
{
	{ "Dragon Slayer 4  ", g_ayVGM_psg_ds4_03, 0 },
	{ "Gambare Goemon   ", g_ayVGM_psg_goemon07, 0 },
	// { "Penguin Adventure", g_ayVGM_psg_penguin_12, 0 },
	{ "Metal Gear       ", g_ayVGM_psg_metalgear_05, 0 },
	{ "Hi no Tori       ", g_ayVGM_psg_honotori_09, 0 },
	// { "Quarth (SCC)     ", g_ayVGM_scc_quarth_02, 0 },
	// { "F1-Spirit (SCC)  ", g_ayVGM_scc_f1spirit_01, 0 },
};

// Player button list
const struct ButtonEntry g_ButtonEntry[] =
{
	{ 0x80, ButtonPlay },
	{ 0xB8, ButtonPause },
	{ 0xB9, ButtonStop },
	{ 0xBB, ButtonPrev },
	{ 0xBA, ButtonNext },
	{ 0xBE, ButtonLoop },
};

// Color shade
const u8 g_ColorBlink[4] = { COLOR_LIGHT_RED, COLOR_MEDIUM_RED, COLOR_DARK_RED, COLOR_MEDIUM_RED };

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_CurrentMusic = 0;
u8 g_CurrentButton;


//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void DrawVGM(const u8* ptr, u8 len)
{
	Print_SetPosition(25, 2);
	Print_DrawFormat("%4x", ptr);
	Print_SetPosition(25, 3);
	Print_DrawText("------");
	for(u8 i = 0; i < len; ++i)
	{
		Print_SetPosition(25, 4 + i);
		
		u8 op = *ptr >> 4;
		u8 val = *ptr & 0x0F;
		switch(op)
		{
		case 0x0:
			ptr++;
			Print_DrawFormat("R#%1x=%2x", val, *ptr);
			break;
		case 0x5:
		case 0x7:
		case 0x9:
		case 0xB:
			val |= 0x10;
		case 0x1:
		case 0x2:
		case 0x3:
		case 0x4:
		case 0x6:
		case 0x8:
		case 0xA:
		case 0xC:
			Print_DrawFormat("R#%1x=%2x", g_ayVGM_RegTable[--op], val);
			break;
		case 0xD:
		case 0xE:
			Print_DrawFormat("W=%1x...", 1 + val);
			break;
		case 0xF:
			Print_DrawText("END!!!");
		}
		ptr++;
	}
}

//-----------------------------------------------------------------------------
//
void UpdatePlayer()
{
	Print_SetPosition(0, 10);
	Print_DrawText("Player:\n");
	Print_DrawFormat("\x07" "Freq      %s\n", (g_ayVGM_State & AYVGM_STATE_50HZ) ? "50Hz" : "60Hz");
	Print_DrawFormat("\x07" "DoLoop    %c\n", (g_ayVGM_State & AYVGM_STATE_LOOP) ? '\x0C' : '\x0B');
	Print_DrawFormat("\x07" "DoPlay    %c\n", (g_ayVGM_State & AYVGM_STATE_PLAY) ? '\x0C' : '\x0B');
}

//-----------------------------------------------------------------------------
//
void SetMusic(u8 idx)
{
	g_CurrentMusic = idx;

	bool ok = ayVGM_Play(g_MusicEntry[idx].Data, true);
	
	Print_SetPosition(0, 2);
	Print_DrawFormat("%i/%i %s", 1 + idx, numberof(g_MusicEntry), g_MusicEntry[idx].Name);

	Print_SetPosition(0, 4);
	Print_DrawText("Data:\n");
	Print_DrawFormat("\x07" "Ident     %c\n", ok ? '\x0C' : '\x0B');
	Print_DrawFormat("\x07" "50Hz mark %c\n", (g_ayVGM_Header->Flag & AYVGM_FLAG_50HZ) ? '\x0C' : '\x0B');
	Print_DrawFormat("\x07" "60Hz mark %c\n", (g_ayVGM_Header->Flag & AYVGM_FLAG_60HZ) ? '\x0C' : '\x0B');
	Print_DrawFormat("\x07" "Loop      %c\n", (g_ayVGM_Header->Flag & AYVGM_FLAG_LOOP) ? '\x0C' : '\x0B');

	UpdatePlayer();
}

//-----------------------------------------------------------------------------
//
void ButtonPlay()
{
	ayVGM_Resume();
	UpdatePlayer();
}

//-----------------------------------------------------------------------------
//
void ButtonPause()
{
	ayVGM_Pause();
	UpdatePlayer();
}

//-----------------------------------------------------------------------------
//
void ButtonStop()
{
	ayVGM_Stop();	
	UpdatePlayer();
}

//-----------------------------------------------------------------------------
//
void ButtonPrev()
{
	if(g_CurrentMusic > 0)
		SetMusic(g_CurrentMusic - 1);
}

//-----------------------------------------------------------------------------
//
void ButtonNext()
{
	if(g_CurrentMusic < numberof(g_MusicEntry) - 1)
		SetMusic(g_CurrentMusic + 1);
}

//-----------------------------------------------------------------------------
//
void ButtonLoop()
{
	if(g_ayVGM_State & AYVGM_STATE_LOOP)
		g_ayVGM_State &= ~AYVGM_STATE_LOOP;
	else
		g_ayVGM_State |= AYVGM_STATE_LOOP;
	UpdatePlayer();
}

//-----------------------------------------------------------------------------
//
void SetCursor(u8 id)
{
	g_CurrentButton = id % 6;
	VDP_SetSpriteSM1(0, 8 + 16*g_CurrentButton, 128-1, g_ButtonEntry[g_CurrentButton].Char, COLOR_LIGHT_RED);
}


//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize screen
	VDP_SetMode(VDP_MODE_SCREEN1);
	VDP_ClearVRAM();
	VDP_EnableVBlank(true);

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample8, 1); // Initialize font
	Print_DrawText(MSX_GL " ayVGM Sample");
	Print_DrawLineH(0, 1, 32);

	Print_SetPosition(20, 10);
	Print_DrawText("Main-ROM:");
	Print_SetPosition(20, 11);
	Print_DrawFormat("\x07" "Freq  %s", (g_ROMVersion.VSF) ? "50Hz" : "60Hz");

	// Decode VGM header
	SetMusic(0);
	Print_DrawBox(0, 15, numberof(g_ButtonEntry) * 2 + 1, 3);
	for(u8 i = 0; i < numberof(g_ButtonEntry); ++i)
	{
		Print_SetPosition(1 + 2 * i, 16);
		Print_DrawChar(g_ButtonEntry[i].Char);
		
	}

	// Footer
	Print_DrawLineH(0, 22, 32);
	Print_SetPosition(0, 23);
	Print_DrawText("\x8D:Button \x83:Action \x82:Freq");

	//-------------------------------------------------------------------------
	// Sprite
	
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_8 + VDP_SPRITE_SCALE_1);
	VDP_LoadSpritePattern(g_Font_MGL_Sample8 + 4, 0, g_Font_MGL_Sample8[3] - g_Font_MGL_Sample8[2]);

	SetCursor(4);

	u8 prevRow8 = 0xFF;
	u8 count = 0;
	while(1)
	{
		Halt();
VDP_SetColor(0xF4);
		ayVGM_Decode();
VDP_SetColor(0xF6);
		#if (PSG_ACCESS == PSG_INDIRECT)
		PSG_Apply();
		#endif
VDP_SetColor(0xF0);
		VDP_SetSpriteColorSM1(0, g_ColorBlink[(count >> 2) & 0x03]);
		
		DrawVGM(g_ayVGM_Pointer, 1);


		Print_SetPosition(31, 0);
		u8 chr = count++ & 0x03;
		Print_DrawChar(g_ChrAnim[chr]);
		
		// Handle input
		u8 row8 = Keyboard_Read(8);

		// Change button
		if(IS_KEY_PRESSED(row8, KEY_RIGHT) && !IS_KEY_PRESSED(prevRow8, KEY_RIGHT))
		{
			SetCursor(g_CurrentButton + 1);
		}
		else if(IS_KEY_PRESSED(row8, KEY_LEFT) && !IS_KEY_PRESSED(prevRow8, KEY_LEFT))
		{
			SetCursor(g_CurrentButton - 1);
		}
		// Activate button
		if(IS_KEY_PRESSED(row8, KEY_SPACE) && !IS_KEY_PRESSED(prevRow8, KEY_SPACE))
		{
			g_ButtonEntry[g_CurrentButton].Func();
		}
		// Change frequency
		if((IS_KEY_PRESSED(row8, KEY_UP) && !IS_KEY_PRESSED(prevRow8, KEY_UP))
		 || (IS_KEY_PRESSED(row8, KEY_DOWN) && !IS_KEY_PRESSED(prevRow8, KEY_DOWN)))
		{
			if(g_ayVGM_State & AYVGM_STATE_50HZ)
				g_ayVGM_State &= ~AYVGM_STATE_50HZ;
			else
				g_ayVGM_State |= AYVGM_STATE_50HZ;
			UpdatePlayer();
		}
		
		prevRow8 = row8;
	}
}