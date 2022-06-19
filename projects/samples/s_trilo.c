// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  VGM format sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "trilo\trilo_scc_player.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x02\x03\x04\x05"

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

extern u16 tmu_kv2puzz;

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font\font_mgl_sample8.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Music list
const struct MusicEntry g_MusicEntry[] =
{
	{ "kv2-puzz", NULL, 1 },
	{ "nem3airb", NULL, 2 },
	{ "nem3-st1", NULL, 3 },
	{ "triplets", NULL, 4 },
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

void AsmData()
{
	__asm

// _tmu_kv2puzz::
	// .include "content/trilo/tmu_kv2puzz.asm"

	_tmu_kv2puzz:
		#include "content/trilo/tmu_nem3airb.asm"
		; .include "scc_player/music/tmu_kv2puzz.asm"
		; .include "scc_player/music/tmu_nem3airb.asm"
		; .include "scc_player/music/tmu_nem3sel.asm"
		; .include "scc_player/music/tmu_nem3st1.asm"
		; .include "scc_player/music/tmu_triplets.asm"
		; .include "scc_player/music/tmu_YYACTION.asm"		; crash
		; .include "scc_player/music/tmu_YYBOSS.asm"
		; .include "scc_player/music/tmu_YYCREDIT.asm"
		; .include "scc_player/music/tmu_YYGAME.asm"
		; .include "scc_player/music/tmu_YYGMOVR.asm"		; ???
		; .include "scc_player/music/tmu_YYINTRO.asm"
		; .include "scc_player/music/tmu_YYLVLCLR.asm"
		; .include "scc_player/music/tmu_YYMISION.asm"
		; .include "scc_player/music/tmu_YYMULTI.asm"
		; .include "scc_player/music/tmu_YYSCORE.asm"
		; .include "scc_player/music/tmu_YYTALK.asm"
		; .include "scc_player/music/tmu_YYTITLE.asm"
		; .include "scc_player/music/tmu_YYVICTRY.asm"

	__endasm;
}
//-----------------------------------------------------------------------------
//
void SetMusic(u8 idx)
{
	g_CurrentMusic = idx;

	// bool ok = ayVGM_Play(g_MusicEntry[idx].Data, TRUE);
	
	Print_SetPosition(0, 2);
	Print_DrawFormat("%i/%i %s", 1 + idx, numberof(g_MusicEntry), g_MusicEntry[idx].Name);
}

//-----------------------------------------------------------------------------
//
void ButtonPlay()
{
	// ayVGM_Resume();
}

//-----------------------------------------------------------------------------
//
void ButtonPause()
{
	// ayVGM_Pause();
}

//-----------------------------------------------------------------------------
//
void ButtonStop()
{
	// ayVGM_Stop();	
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
	// if(g_ayVGM_State & AYVGM_STATE_LOOP)
		// g_ayVGM_State &= ~AYVGM_STATE_LOOP;
	// else
		// g_ayVGM_State |= AYVGM_STATE_LOOP;
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
	VDP_EnableVBlank(TRUE);

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample8, 1); // Initialize font
	Print_DrawText("MSXgl - TRILO SAMPLE");
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
	
	// SET_BANK_SEGMENT(3, 4);
	TriloSCC_Initialize();
	TriloSCC_LoadSong((u16)&tmu_kv2puzz);

	// Footer
	Print_DrawLineH(0, 22, 32);
	Print_SetPosition(0, 23);
	Print_DrawText("<>:Button Space:Action");

	//-------------------------------------------------------------------------
	// Sprite
	
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_8 + VDP_SPRITE_SCALE_1);
	// VDP_LoadSpritePattern(g_Font_MGL_Sample8 + 4, 0, g_Font_MGL_Sample8[3] - g_Font_MGL_Sample8[2]);

	SetCursor(4);

	u8 prevRow8 = 0xFF;
	u8 count = 0;
	while(1)
	{
		Halt();
		TriloSCC_Apply();
		TriloSCC_Play();

		VDP_SetSpriteColorSM1(0, g_ColorBlink[(count >> 2) & 0x03]);
		
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
		
		prevRow8 = row8;
	}
}