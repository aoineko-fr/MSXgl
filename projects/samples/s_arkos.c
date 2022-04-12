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
	{ "Cancion Nueva  ", 0xA000, 4 },
	{ "Hocus Pocus    ", 0xA000, 5 },
	{ "Just add cream ", 0xA000, 6 },
	{ "Sarkboteur     ", 0xA000, 7 },
};
const struct MusicEntry g_MusicEntryAKY[] =
{
	{ "Cancion Nueva  ", 0xA000, 12 },
	{ "Hocus Pocus    ", 0xA000, 13 },
	{ "Just add cream ", 0xA000, 14 },
	{ "Sarkboteur     ", 0xA000, 15 },
};
const struct MusicEntry g_MusicEntryAKM[] =
{
	{ "Cancion Nueva  ", 0xA000, 8 },
	{ "Hocus Pocus    ", 0xA000, 9 },
	{ "Just add cream ", 0xA000, 10 },
	{ "Sarkboteur     ", 0xA000, 11 },
};

const struct PlayerEntry g_PlayerEntry[] =
{
	{ "AKG", AKG_Init, AKG_Decode, AKG_Stop, g_MusicEntryAKG },
	{ "AKY", AKY_Init, AKY_Decode, null,     g_MusicEntryAKY },
	{ "AKM", AKM_Init, AKM_Decode, AKM_Stop, g_MusicEntryAKM },
};

//=============================================================================
// MEMORY DATA
//=============================================================================

const struct PlayerEntry* g_CurrentPlayer;
u8   g_PlayerIdx;
u8   g_MusicIdx = 0;
bool g_Freq50Hz = false;


//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void SetMusic(u8 idx)
{
	g_MusicIdx = idx;
	
	const struct MusicEntry* mus = &g_PlayerEntry[g_PlayerIdx].Musics[idx];

	Print_SetPosition(0, 3);
	Print_DrawFormat("Music %i/%i: %s", idx + 1, numberof(g_MusicEntryAKG), mus->Name);

	SET_BANK_SEGMENT(3, mus->Segment);

	g_CurrentPlayer->Init(mus->Data, 0);
}


//-----------------------------------------------------------------------------
//
void SetPlayer(u8 idx)
{
	g_PlayerIdx = idx;
	g_CurrentPlayer = &g_PlayerEntry[idx];

	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL " ARKOS ");
	Print_DrawText(g_CurrentPlayer->Name);
	Print_DrawText(" SAMPLE");
	
	SetMusic(g_MusicIdx);
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
	Print_DrawLineH(0, 1, 40);
	
	SetPlayer(1);
	SetMusic(0);

	// Frequency
	g_Freq50Hz = g_ROMVersion.VSF ? 1 : 0;
	Print_SetPosition(0, 18);
	Print_DrawFormat("Freq: %s", (g_Freq50Hz) ? "50Hz" : "60Hz");
	Print_SetPosition(20, 18);
	Print_DrawFormat("BIOS: %s", (g_ROMVersion.VSF) ? "50Hz" : "60Hz");

	// Footer
	Print_DrawLineH(0, 22, 40);
	Print_SetPosition(0, 23);
	Print_DrawText("\x8D:Music \x83:Pause \x82:Player Home:Freq");

	u8 prevRow8 = 0xFF;
	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();
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
		if(IS_KEY_PRESSED(row8, KEY_UP) && !IS_KEY_PRESSED(prevRow8, KEY_UP))
		{
			if(g_PlayerIdx < numberof(g_PlayerEntry) - 1)
				SetPlayer(g_PlayerIdx + 1);
		}
		else if(IS_KEY_PRESSED(row8, KEY_DOWN) && !IS_KEY_PRESSED(prevRow8, KEY_DOWN))
		{
			if(g_PlayerIdx > 0)
				SetPlayer(g_PlayerIdx - 1);
		}
		if(IS_KEY_PRESSED(row8, KEY_SPACE) && !IS_KEY_PRESSED(prevRow8, KEY_SPACE))
		{
			if(g_CurrentPlayer->Stop != null)
				g_CurrentPlayer->Stop();
		}
		prevRow8 = row8;
	}
}