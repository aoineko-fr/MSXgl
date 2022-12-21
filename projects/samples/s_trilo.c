// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  VGM format sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "trilo/trilo_scc_player.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x02\x03\x04\x05"

// VGM music entry
struct MusicEntry
{
	u8          Segment;
	const void* Data;
	const c8*   Music;
	const c8*   Author;
};

// Segment 4 data
extern void sfx_wavetable;
extern u8* g_SFX_TEST;
extern void tmu_kv2puzz;
extern void tmu_nem3st1;

// Segment 5 data
extern void tmu_nem3airb;
extern void tmu_nem3sel;

// Segment 6 data
extern void tmu_triplets;
extern void tmu_ULTITEST;

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
// #include "font\font_mgl_sample8.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Music list
const struct MusicEntry g_MusicEntry[] =
{
	{ 4, &tmu_kv2puzz,  "KVII- Puzzle Stage",               "Konami (Arr by Gryzor87)" },
	{ 5, &tmu_nem3airb, "NEM3- Air Battle",                 "Konami (Arr by Gryzor87)" },
	{ 5, &tmu_nem3sel,  "NEM3- Weapon Select",              "Konami (Arr by Gryzor87)" },
	{ 4, &tmu_nem3st1,  "NEM3- Stage 1",                    "Konami (Arr by Gryzor87)" },
	{ 6, &tmu_triplets, "TT -TRIPLETS-B50-BASSDRUM",        "Gryzor87 (c)2021" },
	{ 6, &tmu_ULTITEST, "TT Vs Replayer ULTIMATE  TESTv12", "Gryzor87 (c)2021" },
};

//
u16 g_PSG_A445_Konami[] =
{
	0x0D36, 0x0C78, 0x0BC5, 0x0B1C, 0x0A7C, 0x09E6, 0x0957, 0x08D1, 0x0853, 0x07DB, 0x076A, 0x0700,
	0x069B, 0x063C, 0x05E3, 0x058E, 0x053E, 0x04F3, 0x04AC, 0x0469, 0x0429, 0x03ED, 0x03B5, 0x0380,
	0x034E, 0x031E, 0x02F1, 0x02C7, 0x029F, 0x0279, 0x0256, 0x0234, 0x0215, 0x01F7, 0x01DB, 0x01C0,
	0x01A7, 0x018F, 0x0179, 0x0163, 0x0150, 0x013D, 0x012B, 0x011A, 0x010A, 0x00FB, 0x00ED, 0x00E0,
	0x00D3, 0x00C8, 0x00BC, 0x00B2, 0x00A8, 0x009E, 0x0095, 0x008D, 0x0085, 0x007E, 0x0077, 0x0070,
	0x006A, 0x0064, 0x005E, 0x0059, 0x0054, 0x004F, 0x004B, 0x0047, 0x0043, 0x003F, 0x003B, 0x0038,
	0x0035, 0x0032, 0x002F, 0x002C, 0x002A, 0x0028, 0x0025, 0x0023, 0x0021, 0x001F, 0x001E, 0x001C,
	0x001A, 0x0019, 0x0018, 0x0016, 0x0015, 0x0014, 0x0013, 0x0012, 0x0011, 0x0010, 0x000F, 0x000E,
};

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_CurrentMusic = 0;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void SetMusic(u8 idx)
{
	g_CurrentMusic = idx;

	SET_BANK_SEGMENT(3, g_MusicEntry[idx].Segment);
	TriloSCC_LoadMusic(g_MusicEntry[idx].Data);

	Print_SetPosition(0, 3);
	Print_DrawCharX(' ', 80);
	Print_SetPosition(0, 3);
	Print_DrawFormat("%i/%i %s", 1 + idx, numberof(g_MusicEntry), g_MusicEntry[idx].Music);
	Print_SetPosition(5, 4);
	Print_DrawFormat("by %s", g_MusicEntry[idx].Author);
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize screen
	VDP_SetMode(VDP_MODE_SCREEN0);
	VDP_ClearVRAM();
	VDP_EnableVBlank(TRUE);
	VDP_SetColor(0xF4);

	// Initialize font
	Print_SetTextFont(NULL, 1); // Initialize font
	// Print_SetTextFont(g_Font_MGL_Sample8, 1); // Initialize font
	Print_DrawText("MSXgl - TRILO TRACKER SAMPLE");
	Print_DrawLineH(0, 1, 40);

	Print_SetPosition(0, 20);
	Print_DrawFormat("Main-ROM Freq: %s", (g_ROMVersion.VSF) ? "50Hz" : "60Hz");

	// Initialize TriloSCC
	TriloSCC_Initialize();
	TriloSCC_SetFrequency((g_ROMVersion.VSF) ? TRILO_50HZ : TRILO_60HZ);
	TriloSCC_SetToneTable(g_PSG_A445_Konami);
	SetMusic(0);

	// Initialize TriloSFX
	TriloSFX_Initialize();
	TriloSFX_SetBank(&g_SFX_TEST, &sfx_wavetable);

	// Footer
	Print_DrawLineH(0, 22, 40);
	Print_SetPosition(0, 23);
	Print_DrawText("<|>:Music  [Del]:Pause  [Space]:SFX");

	u8 prevRow8 = 0xFF;
	u8 count = 0;
	u8 sfx = 0;
	while(1)
	{
		Halt();
		VDP_SetColor(0xF5);
		TriloSCC_Apply();
		TriloSCC_Update();
		TriloSFX_Update();
		VDP_SetColor(0xF4);

		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);

		// Handle input
		u8 row8 = Keyboard_Read(8);

		// Change music
		if(IS_KEY_PRESSED(row8, KEY_RIGHT) && !IS_KEY_PRESSED(prevRow8, KEY_RIGHT))
		{
			if(g_CurrentMusic < numberof(g_MusicEntry) - 1)
				SetMusic(g_CurrentMusic + 1);
		}
		else if(IS_KEY_PRESSED(row8, KEY_LEFT) && !IS_KEY_PRESSED(prevRow8, KEY_LEFT))
		{
			if(g_CurrentMusic > 0)
				SetMusic(g_CurrentMusic - 1);
		}
		// Pause/resume music
		if(IS_KEY_PRESSED(row8, KEY_DEL) && !IS_KEY_PRESSED(prevRow8, KEY_DEL))
		{
			TriloSCC_Pause();
		}
		// Play SFX
		if(IS_KEY_PRESSED(row8, KEY_SPACE) && !IS_KEY_PRESSED(prevRow8, KEY_SPACE))
		{
			TriloSFX_Play(sfx++, 0);
			if(sfx >= 2/*TriloSFX_GetNumber()*/)
				sfx = 0;
		}

		prevRow8 = row8;
	}
}