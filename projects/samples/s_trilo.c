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

extern u16 PSG_A445_Konami;
extern u16 SFX_TEST_WAVEBASE;

// Segment 4 data
extern u16 tmu_kv2puzz;
extern u16 tmu_triplets;
extern u16 sfx_wavetable;
extern u8* g_SFX_TEST;

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
// #include "font\font_mgl_sample8.h"

// SFX bank
// #include "content/trilo/SFX_TEST.h"

// g_SFX_TEST

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

// "scc_player/music/tmu_kv2puzz.asm"
// "scc_player/music/tmu_nem3airb.asm"
// "scc_player/music/tmu_nem3sel.asm"
// "scc_player/music/tmu_nem3st1.asm"
// "scc_player/music/tmu_triplets.asm"
// "scc_player/music/tmu_YYACTION.asm"		; crash
// "scc_player/music/tmu_YYBOSS.asm"
// "scc_player/music/tmu_YYCREDIT.asm"
// "scc_player/music/tmu_YYGAME.asm"
// "scc_player/music/tmu_YYGMOVR.asm"		; ???
// "scc_player/music/tmu_YYINTRO.asm"
// "scc_player/music/tmu_YYLVLCLR.asm"
// "scc_player/music/tmu_YYMISION.asm"
// "scc_player/music/tmu_YYMULTI.asm"
// "scc_player/music/tmu_YYSCORE.asm"
// "scc_player/music/tmu_YYTALK.asm"
// "scc_player/music/tmu_YYTITLE.asm"
// "scc_player/music/tmu_YYVICTRY.asm"
// INCLUDE_ASM(tmu_kv2puzz, "content/trilo/tmu_triplets.asm");

//-----------------------------------------------------------------------------
//
void Dummy()
{
	__asm

//--- PSG_A445_Konami:
	_PSG_A445_Konami::
		.dw 0x0D36, 0x0C78, 0x0BC5, 0x0B1C, 0x0A7C, 0x09E6, 0x0957, 0x08D1, 0x0853, 0x07DB, 0x076A, 0x0700
		.dw 0x069B, 0x063C, 0x05E3, 0x058E, 0x053E, 0x04F3, 0x04AC, 0x0469, 0x0429, 0x03ED, 0x03B5, 0x0380
		.dw 0x034E, 0x031E, 0x02F1, 0x02C7, 0x029F, 0x0279, 0x0256, 0x0234, 0x0215, 0x01F7, 0x01DB, 0x01C0
		.dw 0x01A7, 0x018F, 0x0179, 0x0163, 0x0150, 0x013D, 0x012B, 0x011A, 0x010A, 0x00FB, 0x00ED, 0x00E0
		.dw 0x00D3, 0x00C8, 0x00BC, 0x00B2, 0x00A8, 0x009E, 0x0095, 0x008D, 0x0085, 0x007E, 0x0077, 0x0070
		.dw 0x006A, 0x0064, 0x005E, 0x0059, 0x0054, 0x004F, 0x004B, 0x0047, 0x0043, 0x003F, 0x003B, 0x0038
		.dw 0x0035, 0x0032, 0x002F, 0x002C, 0x002A, 0x0028, 0x0025, 0x0023, 0x0021, 0x001F, 0x001E, 0x001C
		.dw 0x001A, 0x0019, 0x0018, 0x0016, 0x0015, 0x0014, 0x0013, 0x0012, 0x0011, 0x0010, 0x000F, 0x000E
	__endasm;
}

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
	Print_SetTextFont(NULL, 1); // Initialize font
	// Print_SetTextFont(g_Font_MGL_Sample8, 1); // Initialize font
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
	
	SET_BANK_SEGMENT(3, 4);
	TriloSCC_Initialize();
	TriloSCC_LoadSong((u16)&tmu_kv2puzz);
	TriloSCC_SetToneTable((u16)&PSG_A445_Konami);

	TriloSFX_Initialize();
	TriloSFX_Load((u16)g_SFX_TEST, (u16)&sfx_wavetable);

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
		TriloSCC_Update();
		TriloSFX_Update();

		VDP_SetSpriteColorSM1(0, g_ColorBlink[(count >> 2) & 0x03]);
		
		Print_SetPosition(31, 0);
		u8 chr = count++ & 0x03;
		Print_DrawChar(g_ChrAnim[chr]);
		
		// Handle input
		u8 row8 = Keyboard_Read(8);

		// Change button
		// if(IS_KEY_PRESSED(row8, KEY_RIGHT) && !IS_KEY_PRESSED(prevRow8, KEY_RIGHT))
		// {
			// SetCursor(g_CurrentButton + 1);
		// }
		// else if(IS_KEY_PRESSED(row8, KEY_LEFT) && !IS_KEY_PRESSED(prevRow8, KEY_LEFT))
		// {
			// SetCursor(g_CurrentButton - 1);
		// }
		// Activate button
		if(IS_KEY_PRESSED(row8, KEY_SPACE) && !IS_KEY_PRESSED(prevRow8, KEY_SPACE))
		{
			// g_ButtonEntry[g_CurrentButton].Func();
			TriloSFX_Play(0, 0);
		}
		
		prevRow8 = row8;
	}
}