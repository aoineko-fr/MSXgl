// __________________________
// ██▀█▀██▀▀▀█▀▀█▀█  ▄▄▄ ▄▄  │   ▄▄▄                ▄▄      
// █  ▄ █▄ ▀██▄ ▀▄█ ██   ██  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █▀▀ ▄█  █ █ ▀█▄█ ██▄▄│  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀──────────┘                 ▀▀
//  PCM-Encoder sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "arkos\akg_player.h"
#include "arkos\akm_player.h"
#include "arkos\aky_player.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

// Music entry
struct MusicEntry
{
	const c8* Name;
	const u8* Data;
	u8        Segment;
};

// Init function callback
typedef void (*cbInit)(const void*, u16);

// Player entry
struct PlayerEntry
{
	const c8* Name;
	cbInit    Init;
	callback  Decode;
	callback  Stop;
	const struct MusicEntry* Musics;
};

// Segment data
extern const u8* g_AKG_jinj_med;
extern const u8* g_AKG_HocusPocus;
extern const u8* g_AKG_Justaddcream;
extern const u8* g_AKG_Sarkboteur;
extern const u8* g_AKM_jinj_med;
extern const u8* g_AKM_HocusPocus;
extern const u8* g_AKM_Justaddcream;
extern const u8* g_AKM_Sarkboteur;
extern const u8* g_AKY_jinj_med;
extern const u8* g_AKY_AHarmlessGrenade;
extern const u8* g_AKY_Justaddcream;
extern const u8* g_AKY_Sarkboteur;

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font\font_mgl_sample6.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Music list
const struct MusicEntry g_MusicEntryAKG[] =
{
	{ "Cancion Nueva                        ", g_AKG_jinj_med,     4 },
	{ "Hocus Pocus (by Targhan)             ", g_AKG_HocusPocus,   5 },
	{ "Just add cream (by Excellence in Art)", g_AKG_Justaddcream, 6 },
	{ "Sarkboteur (by Rob Hubbard)          ", g_AKG_Sarkboteur,   7 },
};
const struct MusicEntry g_MusicEntryAKY[] =
{
	{ "Cancion Nueva                        ", g_AKY_jinj_med,     12 },
	{ "A Harmless Grenade (by Targhan)      ", g_AKY_AHarmlessGrenade, 13 },
	{ "Just add cream (by Excellence in Art)", g_AKY_Justaddcream, 14 },
	{ "Sarkboteur (by Rob Hubbard)          ", g_AKY_Sarkboteur,   15 },
};
const struct MusicEntry g_MusicEntryAKM[] =
{
	{ "Cancion Nueva                        ", g_AKM_jinj_med,     8 },
	{ "Hocus Pocus (by Targhan)             ", g_AKM_HocusPocus,   9 },
	{ "Just add cream (by Excellence in Art)", g_AKM_Justaddcream, 10 },
	{ "Sarkboteur (by Rob Hubbard)          ", g_AKM_Sarkboteur,   11 },
};

const struct PlayerEntry g_PlayerEntry[] =
{
	{ "AKG (generic)",    AKG_Init, AKG_Decode, AKG_Stop, g_MusicEntryAKG },
	{ "AKY (fast)",       AKY_Init, AKY_Decode, null,     g_MusicEntryAKY },
	{ "AKM (minimalist)", AKM_Init, AKM_Decode, AKM_Stop, g_MusicEntryAKM },
};

//=============================================================================
// MEMORY DATA
//=============================================================================

const struct PlayerEntry* g_CurrentPlayer;
u8   g_PlayerIdx = 0;
u8   g_MusicIdx = 0;
bool g_Freq50Hz = false;

// V-blank synchronization flag
u8 g_VBlank = 0;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// H_TIMI interrupt hook
void VBlankHook()
{
	g_VBlank = 1;
}

//-----------------------------------------------------------------------------
// Wait for V-Blank period
void WaitVBlank()
{
	while(g_VBlank == 0) {}
	g_VBlank = 0;
}

//-----------------------------------------------------------------------------
//
void SetMusic(u8 idx)
{
	g_MusicIdx = idx;
	
	const struct MusicEntry* mus = &g_PlayerEntry[g_PlayerIdx].Musics[idx];

	Print_SetPosition(0, 10);
	Print_DrawFormat("Music [%i/%i]:\n\n  %s", idx + 1, numberof(g_MusicEntryAKG), mus->Name);

	SET_BANK_SEGMENT(3, mus->Segment);

	g_CurrentPlayer->Init(mus->Data, 0);
}

//-----------------------------------------------------------------------------
//
void SetPlayer(u8 idx)
{
	g_PlayerIdx = idx;
	g_CurrentPlayer = &g_PlayerEntry[idx];

	for(u8 i = 0; i < numberof(g_PlayerEntry) ; i++)
	{
		Print_SetPosition(0, i + 5);
		Print_DrawChar((i == idx) ? '\x8A' : ' ');		
	}
	
	SetMusic(g_MusicIdx);
}

//-----------------------------------------------------------------------------
//
void DisplayFreq()
{
	Print_SetPosition(0, 20);
	Print_DrawFormat("Freq: %s", (g_Freq50Hz) ? "50Hz" : "60Hz");
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_ClearVRAM();
	VDP_EnableVBlank(true);
	VDP_SetColor(0xF4);

	// Header
	Print_SetTextFont(g_Font_MGL_Sample6, 1); // Initialize font
	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL " ARKOS SAMPLE");
	Print_DrawLineH(0, 1, 40);
	
	Print_SetPosition(0, 3);
	Print_DrawText("Player: ");
	for(u8 i = 0; i < numberof(g_PlayerEntry) ; i++)
	{
		Print_SetPosition(2, i + 5);
		Print_DrawText(g_PlayerEntry[i].Name);		
	}

	SetPlayer(0);
	SetMusic(0);

	// Frequency
	g_Freq50Hz = g_ROMVersion.VSF ? 1 : 0;
	DisplayFreq();
	Print_SetPosition(20, 20);
	Print_DrawFormat("BIOS: %s", (g_ROMVersion.VSF) ? "50Hz" : "60Hz");

	// Footer
	Print_DrawLineH(0, 22, 40);
	Print_SetPosition(0, 23);
	Print_DrawText("\x8D:Music  \x83:Pause  \x82:Player  Home:Freq");

	VDP_EnableVBlank(true);
	Bios_SetHookCallback(H_TIMI, VBlankHook);

	u8 prevRow8 = 0xFF;
	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		WaitVBlank();
		if(g_Freq50Hz || (count % 6) != 0)
		{
			VDP_SetColor(0xF5);
			g_CurrentPlayer->Decode();
			VDP_SetColor(0xF4);
		}
		
		Print_SetPosition(39, 0);
		u8 chr = count++ & 0x03;
		Print_DrawChar(g_ChrAnim[chr]);

		// Handle input
		u8 row8 = Keyboard_Read(8);
		// Change played music
		if(IS_KEY_PRESSED(row8, KEY_RIGHT) && !IS_KEY_PRESSED(prevRow8, KEY_RIGHT))
		{
			if(g_MusicIdx < numberof(g_MusicEntryAKG) - 1)
				SetMusic(g_MusicIdx + 1);
		}
		else if(IS_KEY_PRESSED(row8, KEY_LEFT) && !IS_KEY_PRESSED(prevRow8, KEY_LEFT))
		{
			if(g_MusicIdx > 0)
				SetMusic(g_MusicIdx - 1);
		}
		// Change Arkos replayer
		if(IS_KEY_PRESSED(row8, KEY_UP) && !IS_KEY_PRESSED(prevRow8, KEY_UP))
		{
			if(g_PlayerIdx > 0)
				SetPlayer(g_PlayerIdx - 1);
		}
		else if(IS_KEY_PRESSED(row8, KEY_DOWN) && !IS_KEY_PRESSED(prevRow8, KEY_DOWN))
		{
			if(g_PlayerIdx < numberof(g_PlayerEntry) - 1)
				SetPlayer(g_PlayerIdx + 1);
		}
		// Stop music playback
		if(IS_KEY_PRESSED(row8, KEY_SPACE) && !IS_KEY_PRESSED(prevRow8, KEY_SPACE))
		{
			if(g_CurrentPlayer->Stop != null)
				g_CurrentPlayer->Stop();
		}
		// Change frequency
		if(IS_KEY_PRESSED(row8, KEY_HOME) && !IS_KEY_PRESSED(prevRow8, KEY_HOME))
		{
			g_Freq50Hz = 1 - g_Freq50Hz;
			DisplayFreq();
		}
		prevRow8 = row8;
	}
}