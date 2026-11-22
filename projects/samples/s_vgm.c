// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  VGM format sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "psg.h"
#include "vgm/vgm_player.h"
#if (VGM_USE_SCC)
	#include "scc.h"
#endif
#if (VGM_USE_MSXMUSIC)
	#include "msx-music.h"
#endif
#if (VGM_USE_MSXAUDIO)
	#include "msx-audio.h"
#endif
#if (VGM_USE_OPL4)
	#include "audio/opl4.h"
#endif
#if (VGM_USE_OPNA)
	#include "audio/opna.h"
#endif
#include "s_vgm_rawdef.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL						"\x02\x03\x04\x05"

#define MUSIC_BANK					1

// Layout
#define LAYOUT_FILE_X				0
#define LAYOUT_FILE_Y				2
#define LAYOUT_PLAYER_X				0
#define LAYOUT_PLAYER_Y				4
#define LAYOUT_INFO_X				0
#define LAYOUT_INFO_Y				8
#define LAYOUT_DATA_X				0
#define LAYOUT_DATA_Y				13
#define LAYOUT_DETECT_X				20
#define LAYOUT_DETECT_Y				8
#define LAYOUT_OPCODE_X				26
#define LAYOUT_OPCODE_Y				4

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

// Segment data
extern const c8* g_VGM_psg_ds4_03;
extern const c8* g_VGM_psg_goemon_07;
extern const c8* g_VGM_psg_metalgear_05;
extern const c8* g_VGM_psg_honotori_09;
extern const c8* g_VGM_scc_f1spirit_01;
extern const c8* g_VGM_mm_undeadline_03;
extern const c8* g_VGM_mm_ff_03;
extern const c8* g_VGM_ma_xevious_01;
extern const c8* g_VGM_ma_proyakyu_10;


//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample8.h"

// Music list
const struct MusicEntry g_MusicEntry[] =
{
	{ "Dragon Slayer 4    ", 0x8000 + PSG_DS4_03_VGM_REL,        PSG_DS4_03_VGM_SEG },
	{ "Gambare Goemon     ", 0x8000 + PSG_GOEMON_07_VGM_REL,     PSG_GOEMON_07_VGM_SEG },
	{ "Metal Gear         ", 0x8000 + PSG_METALGEAR_05_VGM_REL,  PSG_METALGEAR_05_VGM_SEG },
	{ "Hi no Tori         ", 0x8000 + PSG_HONOTORI_09_VGM_REL,   PSG_HONOTORI_09_VGM_SEG },
	{ "F1 Spirit (SCC)    ", 0x8000 + SCC_F1SPIRIT_01_VGM_REL,   SCC_F1SPIRIT_01_VGM_SEG },
	{ "Undeadline (M-M)   ", 0x8000 + MM_UNDEADLINE_03_VGM_REL,  MM_UNDEADLINE_03_VGM_SEG },
	{ "Final Fantasy (M-M)", 0x8000 + MM_FF_03_VGM_REL,          MM_FF_03_VGM_SEG },
	{ "Xevious (M-A)      ", 0x8000 + MA_XEVIOUS_01_VGM_REL,     MA_XEVIOUS_01_VGM_SEG },
	{ "Pro Yakyu (M-A)    ", 0x8000 + MA_PROYAKYU_10_VGM_REL,    MA_PROYAKYU_10_VGM_SEG },
	{ "Monkey Isl. (OPL2) ", 0x8000 + OPL2_MONKEYISLAND_11_VGM_REL, OPL2_MONKEYISLAND_11_VGM_SEG },
	{ "Heretic (OPL3)     ", 0x8000 + OPL3_HERETIC_01_VGM_REL,   OPL3_HERETIC_01_VGM_SEG },
	{ "Sonyc (OPL4)       ", 0x8000 + OPL4_SONYC_05_VGM_REL,     OPL4_SONYC_05_VGM_SEG },
	{ "Valis 2 (OPNA)     ", 0x8000 + OPNA_VALIS2_23_VGM_REL,    OPNA_VALIS2_23_VGM_SEG },
	{ "Misty Blue (OPNA)  ", 0x8000 + OPNA_MISTYBLUE_05_VGM_REL, OPNA_MISTYBLUE_05_VGM_SEG },
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

u8 g_Frame = 0;
u8 g_CurrentMusic = 0;
u8 g_CurrentButton;


//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Update player information
void UpdatePlayer()
{
	Print_SetPosition(LAYOUT_INFO_X, LAYOUT_INFO_Y);
	Print_DrawText("Player:\n");
	Print_DrawFormat("\x07" "Freq    %s\n", (g_VGM_State & VGM_STATE_50HZ) ? "50Hz" : "60Hz");
	Print_DrawFormat("\x07" "DoLoop  %c\n", (g_VGM_State & VGM_STATE_LOOP) ? '\x0C' : '\x0B');
	Print_DrawFormat("\x07" "DoPlay  %c\n", (g_VGM_State & VGM_STATE_PLAY) ? '\x0C' : '\x0B');
}

//-----------------------------------------------------------------------------
// Select a new music
void SetMusic(u8 idx)
{
	g_CurrentMusic = idx;

	SET_BANK_SEGMENT(MUSIC_BANK, g_MusicEntry[idx].Segment);

	bool ok = VGM_Play(g_MusicEntry[idx].Data, TRUE);

	Print_SetPosition(LAYOUT_FILE_X, LAYOUT_FILE_Y);
	Print_DrawFormat("%i/%i %s", 1 + idx, numberof(g_MusicEntry), g_MusicEntry[idx].Name);

	Print_SetPosition(LAYOUT_DATA_X, LAYOUT_DATA_Y);
	Print_DrawText("Data:\n");
	Print_DrawFormat("\x07" "Ident   %s\n", ok ? "OK" : "Wrong");
	Print_DrawFormat("\x07" "Version %1x.%1x%1x\n", (u8)(g_VGM_Header->Version >> 8) & 0xF, (u8)(g_VGM_Header->Version >> 4) & 0xF, (u8)(g_VGM_Header->Version) & 0xF);
	Print_DrawFormat("\x07" "Loop    %4X(%4X)\n", g_VGM_Header->Loop_offset, (u16)&g_VGM_Header->Loop_offset + (u16)g_VGM_Header->Loop_offset);
	if (g_VGM_Header->Rate)
		Print_DrawFormat("\x07" "Freq    %i    \n", g_VGM_Header->Rate);
	else
		Print_DrawText("\x07" "Freq    Undef\n");
	Print_DrawCharXAt(LAYOUT_DATA_X, LAYOUT_DATA_Y + 5, ' ', LAYOUT_DETECT_X - LAYOUT_DATA_X);
	Print_SetPosition(LAYOUT_DATA_X, LAYOUT_DATA_Y + 5);
	Print_DrawText(  "\x07" "Chip    ");
	if (VGM_ContainsPSG())
		Print_DrawText("PSG ");
	if (VGM_ContainsMSXMusic())
		Print_DrawText("OPLL ");
	if (VGM_ContainsMSXAudio())
		Print_DrawText("OPL1 ");
	if (VGM_ContainsSCC())
		Print_DrawText("SCC ");
	if (VGM_ContainsOPL2())
		Print_DrawText("OPL2 ");
	if (VGM_ContainsOPL3())
		Print_DrawText("OPL3 ");
	if (VGM_ContainsOPL4())
		Print_DrawText("OPL4 ");
	if (VGM_ContainsOPNA())
		Print_DrawText("OPNA ");
		
	Print_DrawCharXAt(0, 21, ' ', 32);

	UpdatePlayer();
}

//-----------------------------------------------------------------------------
// Draw VU
void DrawVU()
{
	Print_SetPosition(0, 21);
	#if (PSG_USE_CHANNEL)
	if (VGM_ContainsPSG())
	{
		loop (i, PSG_GetChannelCount())
			Print_DrawChar(0xA0 + ((PSG_GetChannelVolume(i) / 2 + 1) & 0x07));
		Print_Space();
	}
	#endif
	#if (MSXMUSIC_USE_CHANNEL)
	if (VGM_ContainsMSXMusic())
	{
		loop (i, MSXMusic_GetChannelCount())
			Print_DrawChar(0xA0 + ((MSXMusic_GetChannelVolume(i) / 2 + 1) & 0x07));
		Print_Space();
	}
	#endif
	#if (MSXAUDIO_USE_CHANNEL)
	if (VGM_ContainsMSXAudio())
	{
		loop (i, MSXAudio_GetChannelCount())
			Print_DrawChar(0xA0 + ((MSXAudio_GetChannelVolume(i) / 2 + 1) & 0x07));
		Print_Space();
	}
	#endif
	#if (SCC_USE_CHANNEL)
	if (VGM_ContainsSCC())
	{
		loop (i, SCC_GetChannelCount())
			Print_DrawChar(0xA0 + ((SCC_GetChannelVolume(i) / 2 + 1) & 0x07));
		Print_Space();
	}
	#endif
	#if (OPL4_USE_CHANNEL)
	if (VGM_ContainsOPL2() || VGM_ContainsOPL3() || VGM_ContainsOPL4())
	{
		loop (i, 32)//OPL4_GetChannelCount())
			Print_DrawChar(0xA0 + ((OPL4_GetChannelVolume(i) / 2 + 1) & 0x07));
	}
	#endif
	#if (OPNA_USE_CHANNEL)
	if (VGM_ContainsOPNA())
	{
		loop (i, OPNA_GetChannelCount())
			Print_DrawChar(0xA0 + ((OPNA_GetChannelVolume(i) / 2 + 1) & 0x07));
		Print_Space();
	}
	#endif
}

//-----------------------------------------------------------------------------
// Select Play button
void ButtonPlay()
{
	#if (VGM_USE_RESUME)
	VGM_Resume();
	UpdatePlayer();
	#endif
}

//-----------------------------------------------------------------------------
// Select Pause button
void ButtonPause()
{
	#if (VGM_USE_RESUME)
	VGM_Pause();
	UpdatePlayer();
	#endif
}

//-----------------------------------------------------------------------------
// Select Stop button
void ButtonStop()
{
	VGM_Stop();	
	UpdatePlayer();
}

//-----------------------------------------------------------------------------
// Select Previous button
void ButtonPrev()
{
	if (g_CurrentMusic > 0)
		SetMusic(g_CurrentMusic - 1);
}

//-----------------------------------------------------------------------------
// Select Next button
void ButtonNext()
{
	if (g_CurrentMusic < numberof(g_MusicEntry) - 1)
		SetMusic(g_CurrentMusic + 1);
}

//-----------------------------------------------------------------------------
// Select Loop button
void ButtonLoop()
{
	if (g_VGM_State & VGM_STATE_LOOP)
		g_VGM_State &= ~VGM_STATE_LOOP;
	else
		g_VGM_State |= VGM_STATE_LOOP;
	UpdatePlayer();
}

//-----------------------------------------------------------------------------
// Move player's cursor
void SetCursor(u8 id)
{
	g_CurrentButton = id % 6;
	VDP_SetSpriteSM1(0, (LAYOUT_PLAYER_X + 1) * 8 + 16 * g_CurrentButton, (LAYOUT_PLAYER_Y + 1) * 8 - 1, g_ButtonEntry[g_CurrentButton].Char, COLOR_LIGHT_RED);
}

//-----------------------------------------------------------------------------
// Draw a given slot
void Print_DrawSlot(u8 slot)
{
	if (slot == 0xFF)
	{
		Print_DrawText("No!");
		return;
	}
	Print_DrawInt(Sys_SlotGetPrimary(slot));
	if (Sys_SlotIsExpended(slot))
	{
		Print_DrawChar('-');
		Print_DrawInt(Sys_SlotGetSecondary(slot));
	}
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
	VDP_SetColor2(COLOR_BLACK, COLOR_WHITE);
	VDP_ClearVRAM();
	VDP_EnableVBlank(TRUE);

	// Initialize VGM module
	VGM_Initialize();

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample8, 1); // Initialize font
	Print_DrawText(MSX_GL " VGM SAMPLE");
	Print_DrawLineH(0, 1, 32);

	// Diplayr detected information
	u8 y = LAYOUT_DETECT_Y;
	Print_DrawTextAt(LAYOUT_DETECT_X, y++, "Main-ROM:");
	Print_SetPosition(LAYOUT_DETECT_X, y++);
	Print_DrawFormat("\x07" "Freq  %s", Sys_Is50Hz() ? "50Hz" : "60Hz");
	y++;
	Print_SetPosition(LAYOUT_DETECT_X, y++);
	Print_DrawText("Detect: ");
	#if (VGM_USE_MSXMUSIC)
	Print_SetPosition(LAYOUT_DETECT_X, y++);
	Print_DrawText("\x07" "YM2413  ");
	Print_DrawSlot(g_MSXMusic_SlotId);
	#endif
	#if (VGM_USE_MSXAUDIO)
	Print_SetPosition(LAYOUT_DETECT_X, y++);
	Print_DrawText("\x07" "Y8950   ");
	Print_DrawChar(MSXAudio_Detect() ? '\x0C' : '\x0B');
	#endif
	#if (VGM_USE_SCC)
	Print_SetPosition(LAYOUT_DETECT_X, y++);
	Print_DrawText("\x07" "SCC     ");
	Print_DrawSlot(SCC_SLOT);
	#endif
	#if (VGM_USE_OPL4)
	Print_SetPosition(LAYOUT_DETECT_X, y++);
	Print_DrawText("\x07" "YMF278B ");
	Print_DrawChar(OPL4_Detect() ? '\x0C' : '\x0B');
	Print_SetPosition(LAYOUT_DETECT_X, y++);
	Print_DrawFormat(" (%iKB)", OPL4_WaveGetMemSize() * 128);
	#endif
	#if (VGM_USE_OPNA)
	Print_SetPosition(LAYOUT_DETECT_X, y++);
	Print_DrawText("\x07" "YM2608  ");
	Print_DrawChar(OPNA_Detect() ? '\x0C' : '\x0B');
	#endif

	// Decode VGM header
	SetMusic(0);
	Print_SetDrawStyle(PRINT_DRAW_STYLE_DOUBLE);
	Print_DrawBox(LAYOUT_PLAYER_X, LAYOUT_PLAYER_Y, numberof(g_ButtonEntry) * 2 + 1, 3);
	for (u8 i = 0; i < numberof(g_ButtonEntry); ++i)
	{
		Print_SetPosition(LAYOUT_PLAYER_X + 1 + (2 * i), LAYOUT_PLAYER_Y + 1);
		Print_DrawChar(g_ButtonEntry[i].Char);
	}

	// Footer
	Print_SetDrawStyle(PRINT_DRAW_STYLE_LINE);
	Print_DrawLineH(0, 22, 32);
	Print_SetPosition(0, 23);
	Print_DrawText("\x8D:Button \x83:Action \x82:Freq");

	//-------------------------------------------------------------------------
	// Sprite
	
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_8 + VDP_SPRITE_SCALE_1);
	VDP_LoadSpritePattern(g_Font_MGL_Sample8 + 4, 0, g_Font_MGL_Sample8[3] - g_Font_MGL_Sample8[2]);

	SetCursor(4);

	u8 prevRow8 = 0xFF;
	while (1)
	{
		Halt();
		g_Frame++;

		// VDP_SetColor(0xFE);
		VGM_Decode();
		#if (PSG_ACCESS == PSG_INDIRECT)
		PSG_Apply();
		#endif
		// VDP_SetColor(0xF0);

		VDP_SetSpriteColorSM1(0, g_ColorBlink[(g_Frame >> 2) & 0x03]);
		Print_DrawCharAt(31, 0, 0xB0 + ((g_Frame >> 2) & 0x7));
		DrawVU();
		
		// Handle input
		u8 row8 = Keyboard_Read(8);

		// Change button
		if (IS_KEY_PRESSED(row8, KEY_RIGHT) && !IS_KEY_PRESSED(prevRow8, KEY_RIGHT))
		{
			SetCursor((g_CurrentButton < numberof(g_ButtonEntry) - 1) ? g_CurrentButton + 1 : 0);
		}
		else if (IS_KEY_PRESSED(row8, KEY_LEFT) && !IS_KEY_PRESSED(prevRow8, KEY_LEFT))
		{
			SetCursor((g_CurrentButton > 0) ? g_CurrentButton - 1 : numberof(g_ButtonEntry) - 1);
		}
		// Activate button
		if (IS_KEY_PRESSED(row8, KEY_SPACE) && !IS_KEY_PRESSED(prevRow8, KEY_SPACE))
		{
			g_ButtonEntry[g_CurrentButton].Func();
		}
		// Change frequency
		if ((IS_KEY_PRESSED(row8, KEY_UP) && !IS_KEY_PRESSED(prevRow8, KEY_UP))
		 || (IS_KEY_PRESSED(row8, KEY_DOWN) && !IS_KEY_PRESSED(prevRow8, KEY_DOWN)))
		{
			if (g_VGM_State & VGM_STATE_50HZ)
				VGM_SetFrequency60Hz();
			else
				VGM_SetFrequency50Hz();
			UpdatePlayer();
		}
		
		prevRow8 = row8;
	}
}
