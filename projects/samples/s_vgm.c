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

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x02\x03\x04\x05"

//
#define LOG_SIZE 16
//
#define PLAYER_Y 3

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

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Music list
const struct MusicEntry g_MusicEntry[] =
{
	{ "Dragon Slayer 4    ", 0x8000, 2 },
	{ "Gambare Goemon     ", 0x8000, 3 },
	{ "Metal Gear         ", 0x8000, 5 },
	{ "Hi no Tori         ", 0x8000, 6 },
	{ "F1 Spirit (SCC)    ", 0x8000, 7 },
	{ "Undeadline (M-M)   ", 0x8000, 4 },
	{ "Final Fantasy (M-M)", 0x8000, 8 },
	{ "Xevious (M-A)      ", 0x8000, 9 },
	{ "Pro Yakyu (M-A)    ", 0x8000, 10 },
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
const c8* GetChipName(u8 type)
{
	switch(type)
	{
	case 0x00: return "AY8910";
	case 0x01: return "AY8912";
	case 0x02: return "AY8913";
	case 0x03: return "AY8930";
	case 0x10: return "YM2149";
	case 0x11: return "YM3439";
	case 0x12: return "YMZ284";
	case 0x13: return "YMZ294";
	}
	return "Unknow";
}

//-----------------------------------------------------------------------------
//
void DrawVGM(const u8* ptr)
{
	Print_SetPosition(25, 2);
	Print_DrawFormat("%4x", ptr);
	Print_SetPosition(25, 3);
	Print_DrawText("------");
	for(u8 i = 0; i < 1; ++i)
	{
		Print_SetPosition(25, 4 + i);
		if(*ptr == 0xA0) // AY8910, write value dd to register aa
		{
			Print_DrawFormat("#%2x=%2x", ptr[1], ptr[2]);
			ptr += 2;
		}
		#if (VGM_USE_SCC)
		else if(*ptr == 0xD2) // SCC1, port pp, write value dd to register aa
		{
			u8 reg = 0;
			switch(ptr[1])
			{
			case 0:	reg = 0x00;	break; // 0x00 - waveform
			case 1:	reg = 0x80;	break; // 0x01 - frequency
			case 2:	reg = 0x8A;	break; // 0x02 - volume
			case 3:	reg = 0x8F;	break; // 0x03 - key on/off
			case 4:	reg = 0xA0;	break; // 0x04 - waveform (0x00 used to do SCC access, 0x04 SCC+)
			case 5:	reg = 0xE0;	break; // 0x05 - test register
			}
			Print_DrawFormat("#%2x=%2x", reg + ptr[2], ptr[3]);
			ptr += 3;
		}
		#endif
		#if (VGM_USE_MSXMUSIC)
		else if(*ptr == 0x51) // YM2413, write value dd to register aa
		{
			Print_DrawFormat("#%2x=%2x", ptr[1], ptr[2]);
			ptr += 2;
		}
		#endif
		#if (VGM_USE_MSXAUDIO)
		else if(*ptr == 0x5C) // Y8950, write value dd to register aa
		{
			Print_DrawFormat("#%2x=%2x", ptr[1], ptr[2]);
			ptr += 2;
		}
		#endif
		else if(*ptr == 0x61) // Wait n samples, n can range from 0 to 65535 (approx 1.49 seconds). Longer pauses than this are represented by multiple wait commands.
		{
			Print_DrawFormat("W=%4x", *(u16*)(ptr+1));
			ptr += 2;
		}
		else if(*ptr == 0x62) // Wait 735 samples (60th of a second), a shortcut for 0x61 0xdf 0x02
		{
			Print_DrawText("W=60Hz");
		}
		else if(*ptr == 0x63) // Wait 882 samples (50th of a second), a shortcut for 0x61 0x72 0x03
		{
			Print_DrawText("W=50Hz");
		}
		else if(*ptr == 0x66) // End of sound data
		{
			Print_DrawText("END!!!");
		}
		else if ((*ptr & 0xF0) == 0x70) // wait n+1 samples, n can range from 0 to 15.
		{
			Print_DrawFormat("W=%1x   ", 1 + *ptr & 0x0F);
		}
		else
		{
			Print_DrawFormat("~~%2x~~", *ptr);
			// Print_DrawHex8(*ptr);
		}
		Print_Return();
		ptr++;
	}
}

//-----------------------------------------------------------------------------
//
void UpdatePlayer()
{
	Print_SetPosition(0, 7);
	Print_DrawText("Player:\n");
	Print_DrawFormat("\x07" "Freq    %s\n", (g_VGM_State & VGM_STATE_50HZ) ? "50Hz" : "60Hz");
	Print_DrawFormat("\x07" "DoLoop  %c\n", (g_VGM_State & VGM_STATE_LOOP) ? '\x0C' : '\x0B');
	Print_DrawFormat("\x07" "DoPlay  %c\n", (g_VGM_State & VGM_STATE_PLAY) ? '\x0C' : '\x0B');
}

//-----------------------------------------------------------------------------
//
void SetMusic(u8 idx)
{
	g_CurrentMusic = idx;

	SET_BANK_SEGMENT(1, g_MusicEntry[idx].Segment);

	bool ok = VGM_Play(g_MusicEntry[idx].Data, TRUE);

	Print_SetPosition(0, 2);
	Print_DrawFormat("%i/%i %s", 1 + idx, numberof(g_MusicEntry), g_MusicEntry[idx].Name);

	Print_SetPosition(0, 12);
	Print_DrawFormat("Ident:   %s   (%x)\n", ok ? "OK" : "Invalide", (u16)&g_VGM_Header->Ident);
	Print_DrawFormat("Version: %1x.%1x%1x\n", (u8)(g_VGM_Header->Version >> 8) & 0xF, (u8)(g_VGM_Header->Version >> 4) & 0xF, (u8)(g_VGM_Header->Version) & 0xF);
	Print_DrawFormat("Loop:    %4X (%x)\n", g_VGM_Header->Loop_offset, (u16)&g_VGM_Header->Loop_offset + (u16)g_VGM_Header->Loop_offset);
	Print_DrawFormat("AY8910:  %c\n", VGM_ContainsPSG() ? '\x0C' : '\x0B');
	if(VGM_ContainsPSG())
	{
		Print_DrawFormat(" type:   %s\n", GetChipName(g_VGM_Header->AYT));
		Print_DrawFormat(" flag:   %2x,%2x,%2x\n", g_VGM_Header->AY_Flags[0], g_VGM_Header->AY_Flags[1], g_VGM_Header->AY_Flags[2]);
	}
	else
	{
		Print_DrawText(" type:         \n");
		Print_DrawText(" flag:           \n");
	}
	Print_DrawFormat("YM2413:  %c\n", VGM_ContainsMSXMusic() ? '\x0C' : '\x0B');
	Print_DrawFormat("Y8950:   %c\n", VGM_ContainsMSXAudio() ? '\x0C' : '\x0B');
	Print_DrawFormat("K051649: %c\n", VGM_ContainsSCC() ? '\x0C' : '\x0B');

	UpdatePlayer();
}

//-----------------------------------------------------------------------------
//
void ButtonPlay()
{
	VGM_Resume();
	UpdatePlayer();
}

//-----------------------------------------------------------------------------
//
void ButtonPause()
{
	VGM_Pause();
	UpdatePlayer();
}

//-----------------------------------------------------------------------------
//
void ButtonStop()
{
	VGM_Stop();	
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
	if(g_VGM_State & VGM_STATE_LOOP)
		g_VGM_State &= ~VGM_STATE_LOOP;
	else
		g_VGM_State |= VGM_STATE_LOOP;
	UpdatePlayer();
}

//-----------------------------------------------------------------------------
//
void SetCursor(u8 id)
{
	g_CurrentButton = id % 6;
	VDP_SetSpriteSM1(0, 8 + 16 * g_CurrentButton, (PLAYER_Y + 1) * 8 - 1, g_ButtonEntry[g_CurrentButton].Char, COLOR_LIGHT_RED);
}

//-----------------------------------------------------------------------------
//
void Print_DrawSlot(u8 slot)
{
	if(slot == 0xFF)
	{
		Print_DrawText("No!");
		return;
	}
	Print_DrawInt(Sys_SlotGetPrimary(slot));
	if(Sys_SlotIsExpended(slot))
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

	#if (VGM_USE_SCC)
		SCC_Initialize();
	#endif
	#if (VGM_USE_MSXMUSIC)
		MSXMusic_Initialize();
	#endif
	#if (VGM_USE_MSXAUDIO)
		MSXAudio_Initialize();
	#endif

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample8, 1); // Initialize font
	Print_DrawText(MSX_GL " VGM Sample");
	Print_DrawLineH(0, 1, 32);

	Print_SetPosition(20, 7);
	Print_DrawText("Main-ROM:");
	Print_SetPosition(20, 8);
	Print_DrawFormat("\x07" "Freq  %s", (g_ROMVersion.VSF) ? "50Hz" : "60Hz");

	Print_SetPosition(20, 17);
	Print_DrawText("Slots: ");
	Print_SetPosition(20, 18);
	Print_DrawText("\x07" "YM2413: ");
	Print_DrawSlot(g_MSXMusic_SlotId);
	Print_SetPosition(20, 19);
	Print_DrawText("\x07" "Y8950:  ");
	Print_DrawChar(MSXAudio_Detect() ? '\x0C' : '\x0B');
	// Print_DrawSlot(g_MSXAudio_SlotId);
	Print_SetPosition(20, 20);
	Print_DrawText("\x07" "SCC:    ");
	Print_DrawSlot(SCC_SLOT);

	// Decode VGM header
	SetMusic(0);
	Print_DrawBox(0, PLAYER_Y, numberof(g_ButtonEntry) * 2 + 1, 3);
	for(u8 i = 0; i < numberof(g_ButtonEntry); ++i)
	{
		Print_SetPosition(1 + 2 * i, PLAYER_Y + 1);
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
		// VDP_SetColor(0xFE);
		VGM_Decode();
		#if (PSG_ACCESS == PSG_INDIRECT)
		PSG_Apply();
		#endif
		// VDP_SetColor(0xF0);
		VDP_SetSpriteColorSM1(0, g_ColorBlink[(count >> 2) & 0x03]);
		
		DrawVGM(g_VGM_Pointer);


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
			if(g_VGM_State & VGM_STATE_50HZ)
				VGM_SetFrequency60Hz();
			else
				VGM_SetFrequency50Hz();
			UpdatePlayer();
		}
		
		prevRow8 = row8;
	}
}
