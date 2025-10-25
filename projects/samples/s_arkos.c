// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Arkos Tracker II sample
//─────────────────────────────────────────────────────────────────────────────
// This sample program showcases the use of the 3 available Arkos Tracker 2 (AK2)'s replayers.
//
// In the context of a game, you will probably only need one of them.
// Check the AK2 documentation to choose the replayer that best matches your needs.
// https://www.julien-nevo.com/arkostracker/index.php/the-players/
// 
// Note that AK2 music data has to be prepared to be replayed at a given address.
// This address has to be defined when exporting data from AK2 and data must be 
// visible to the CPU at this address when calling AT2 functions in your code.
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// DEFINES
//=============================================================================

// Includes
#include "msxgl.h"
#include "arkos/akg_player.h"
#include "arkos/akm_player.h"
#include "arkos/aky_player.h"

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

// Music entry structure
struct MusicEntry
{
	const c8* Name;    // Music display name
	const u8* Data;    // Address of the data in the segment
	u8        Segment; // Segment number where the data is located
};

// Init function callback
typedef void (*cbPlay)(u8, const void*);
// Play function callback
typedef bool (*cbUpdate)();
// Init SFX function callback
typedef u8 (*cbPlaySFX)(const void*);
// Play SFX function callback
typedef void (*cbUpdateSFX)(u8, u8, u8);
// Stop SFX function callback
typedef void (*cbStopSFX)(u8);

// Player entry structure
struct PlayerEntry
{
	const c8* Name;    // Player display name
	cbUpdate    Decode;  // Pointer to player's update function
	cbPlay      Init;    // Pointer to player's music initialization and playback function
	callback    Stop;    // Pointer to player's music stop function
	cbPlaySFX   InitSFX; // Pointer to player's SFX initialization function
	cbUpdateSFX PlaySFX; // Pointer to player's SFX playback function
	cbStopSFX   StopSFX; // Pointer to player's SFX stop function
	const struct MusicEntry* Musics; // List of musics for this player
};

// Symbols for music data in mapper segments (see segment/s_arkos*.c)
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

// Font data
#include "font/font_mgl_sample6.h"

// SFX data (generated using Arkos Tracker 2's AKX exporter and converted to header file using MSXbin)
#include "content/arkos/akx_effects.h"

// Animation characters (used as sign-of-life)
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Musics list for AKG replayer
const struct MusicEntry g_MusicEntryAKG[] =
{
	{ "Cancion Nueva (by ?)                 ", g_AKG_jinj_med,     4 },
	{ "Hocus Pocus (by Targhan)             ", g_AKG_HocusPocus,   5 },
	{ "Just add cream (by Excellence in Art)", g_AKG_Justaddcream, 6 },
	{ "Sarkboteur (by Rob Hubbard)          ", g_AKG_Sarkboteur,   7 },
};

// Musics list for AKY replayer
const struct MusicEntry g_MusicEntryAKY[] =
{
	{ "Cancion Nueva (by ?)                 ", g_AKY_jinj_med,     12 },
	{ "A Harmless Grenade (by Targhan)      ", g_AKY_AHarmlessGrenade, 13 },
	{ "Just add cream (by Excellence in Art)", g_AKY_Justaddcream, 14 },
	{ "Sarkboteur (by Rob Hubbard)          ", g_AKY_Sarkboteur,   15 },
};

// Musics list for AKM replayer
const struct MusicEntry g_MusicEntryAKM[] =
{
	{ "Cancion Nueva (by ?)                 ", g_AKM_jinj_med,     8 },
	{ "Hocus Pocus (by Targhan)             ", g_AKM_HocusPocus,   9 },
	{ "Just add cream (by Excellence in Art)", g_AKM_Justaddcream, 10 },
	{ "Sarkboteur (by Rob Hubbard)          ", g_AKM_Sarkboteur,   11 },
};

// Replayers data
const struct PlayerEntry g_PlayerEntry[] =
{
	{ "AKG (generic)",    AKG_Decode, AKG_Play, AKG_Stop, AKG_InitSFX, AKG_PlaySFX, AKG_StopSFX, g_MusicEntryAKG },
	{ "AKY (fast)",       AKY_Decode, AKY_Play, NULL,     NULL,        NULL,        NULL,        g_MusicEntryAKY },
	{ "AKM (minimalist)", AKM_Decode, AKM_Play, AKM_Stop, AKM_InitSFX, AKM_PlaySFX, AKM_StopSFX, g_MusicEntryAKM },
};

//=============================================================================
// MEMORY DATA
//=============================================================================

// Pointer to current player data
const struct PlayerEntry* g_CurrentPlayer = NULL;

// Current player index
u8 g_PlayerIdx = 0;

// Current music index
u8 g_MusicIdx = 0;

// Current SFX index
u8 g_SFXIdx = 0;

// Number of SFX in the bank
u8 g_SFXNum = 0;

// Is current frequency 50 Hz?
bool g_Freq50Hz = FALSE;

// V-blank synchronization flag
bool g_VBlank = FALSE;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// H_TIMI interrupt hook (called at 50/60 Hz)
void VBlankHook()
{
	g_VBlank = TRUE;
}

//-----------------------------------------------------------------------------
// Wait for V-Blank period to synchronize main loop with display frequency
void WaitVBlank()
{
	while (!g_VBlank) {}
	g_VBlank = FALSE;
}

//-----------------------------------------------------------------------------
// Display the current frquency
void DisplayFreq()
{
	Print_SetPosition(0, 20);
	Print_DrawFormat("Freq: %s", (g_Freq50Hz) ? "50Hz" : "60Hz");
}

//-----------------------------------------------------------------------------
// Display information about SFX
void DisplaySFX()
{
	Print_SetPosition(0, 14);
	if (g_CurrentPlayer->InitSFX)
		Print_DrawFormat("SFX: %i/%i        ", g_SFXNum ? g_SFXIdx + 1 : 0, g_SFXNum);
	else
		Print_DrawText("SFX: Unsupported");
}

//-----------------------------------------------------------------------------
// Handle music event callback
void HandleEvent(u8 event)
{
	VDP_SetColor((event % 8) + 1);
}

//-----------------------------------------------------------------------------
// Set the new music to be played
void SetMusic(u8 idx)
{
	g_MusicIdx = idx;
	g_SFXIdx = 0;

	// Get current music data
	const struct MusicEntry* mus = &g_PlayerEntry[g_PlayerIdx].Musics[idx];

	// If current replayer supports SFX, initialize the sound bank
	// Note: SFX data have been exported to be replayed at address 0xE000
	if (g_CurrentPlayer->InitSFX) 
	{
		Mem_Copy(g_AKX_effects, (void*)0xE000, sizeof(g_AKX_effects));
		g_SFXNum = g_CurrentPlayer->InitSFX((const void*)0xE000); // can be AKG_InitSFX or AKM_InitSFX
	}

	// Update the current music information on screen
	Print_SetPosition(0, 10);
	Print_DrawFormat("Music [%i/%i]:\n\n  %s", idx + 1, numberof(g_MusicEntryAKG), mus->Name);

	// Update the SFX information on screen
	DisplaySFX();

	// Select music mapper segment in bank #3 (0xA000~0xBFFF)
	SET_BANK_SEGMENT(3, mus->Segment);

	// Call the current replayer's initialize function (also start the music playback)
	// Function can be AKG_Init, AKY_Init or AKM_Init
	g_CurrentPlayer->Init(0, mus->Data);
}

//-----------------------------------------------------------------------------
// Set the curren Arkos Tracker 2's replayer
void SetPlayer(u8 idx)
{
	// Stop previous playing song (using the previous replayer)
	if (g_CurrentPlayer)
	{
		if (g_CurrentPlayer->Stop)
			g_CurrentPlayer->Stop();
	}

	// Select the new replayer
	g_PlayerIdx = idx;
	g_CurrentPlayer = &g_PlayerEntry[idx];

	// Display cursor on front of the selected replayer
	for (u8 i = 0; i < numberof(g_PlayerEntry) ; i++)
		Print_DrawCharAt(0, i + 5, (i == idx) ? '\x8A' : ' ');

	// Play the current music with the newly selected replayer
	SetMusic(g_MusicIdx);
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize screen mode 0 (text)
	VDP_SetMode(VDP_MODE_SCREEN0);
	VDP_ClearVRAM();
	VDP_EnableVBlank(TRUE);
	VDP_SetColor(0xF4);

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample6, 1);

	// Dislay static information on screen
	Print_DrawTextAt(0, 0, MSX_GL " ARKOS TRACKER SAMPLE");
	Print_DrawLineH(0, 1, 40);

	// Display the list of the replayers
	Print_DrawTextAt(0, 3, "Player: ");
	for (u8 i = 0; i < numberof(g_PlayerEntry) ; i++)
		Print_DrawTextAt(2, i + 5, g_PlayerEntry[i].Name);		

	// Select first replayer and fist music
	AKG_SetEventCallback(HandleEvent);
	SetPlayer(0);
	SetMusic(0);

	// Detect current machine VDP frequency and display this information
	g_Freq50Hz = Sys_Is50Hz() ? 1 : 0;
	DisplayFreq();
	Print_SetPosition(20, 20);
	Print_DrawFormat("BIOS: %s", Sys_Is50Hz() ? "50Hz" : "60Hz");

	// Display footer
	Print_DrawLineH(0, 22, 40);
	Print_DrawTextAt(0, 23, "\x82:Player \x8D:Music \x83:Stop \x84:Freq \x85:SFX");

	// Enable interruption for screen frequency synchronization
	VDP_EnableVBlank(TRUE);
	Bios_SetHookCallback(H_TIMI, VBlankHook); // VBlankHook() will be call at each interruption

	u8 prevRow7 = 0xFF;
	u8 prevRow8 = 0xFF;
	u8 frameCount = 0;
	while (!Keyboard_IsKeyPressed(KEY_ESC))
	{
		// Wait for synchronizaion signal
		WaitVBlank();

		if (g_Freq50Hz || (frameCount % 6) != 0)
		{
			VDP_SetColor(0xF5);
			g_CurrentPlayer->Decode();
			VDP_SetColor(0xF4);
		}

		// Increment frame counter and dispaly the sign-of-life
		u8 chr = frameCount++ & 0x03;
		Print_DrawCharAt(39, 0, g_ChrAnim[chr]);

		// Handle input
		u8 row7 = Keyboard_Read(7);
		u8 row8 = Keyboard_Read(8);
		// Change played music
		if (IS_KEY_PUSHED(row8, prevRow8, KEY_RIGHT))
		{
			if (g_MusicIdx < numberof(g_MusicEntryAKG) - 1)
				SetMusic(g_MusicIdx + 1);
		}
		else if (IS_KEY_PUSHED(row8, prevRow8, KEY_LEFT))
		{
			if (g_MusicIdx > 0)
				SetMusic(g_MusicIdx - 1);
		}
		// Change Arkos replayer
		if (IS_KEY_PUSHED(row8, prevRow8, KEY_UP))
		{
			if (g_PlayerIdx > 0)
				SetPlayer(g_PlayerIdx - 1);
		}
		else if (IS_KEY_PUSHED(row8, prevRow8, KEY_DOWN))
		{
			if (g_PlayerIdx < numberof(g_PlayerEntry) - 1)
				SetPlayer(g_PlayerIdx + 1);
		}
		// Stop music playback
		if (IS_KEY_PUSHED(row8, prevRow8, KEY_SPACE))
		{
			if (g_CurrentPlayer->Stop)
				g_CurrentPlayer->Stop();
		}
		// SFX playback
		if (IS_KEY_PUSHED(row7, prevRow7, KEY_BACK))
		{
			if (g_CurrentPlayer->PlaySFX)
			{
				DisplaySFX();
				g_CurrentPlayer->PlaySFX(g_SFXIdx, ARKOS_CHANNEL_C, 0);
				g_SFXIdx++;
				if (g_SFXIdx >= g_SFXNum)
					g_SFXIdx = 0;
			}
		}
		// Change frequency
		if (IS_KEY_PUSHED(row7, prevRow7, KEY_RETURN))
		{
			g_Freq50Hz = 1 - g_Freq50Hz;
			DisplayFreq();
		}
		prevRow7 = row7;
		prevRow8 = row8;
	}
}