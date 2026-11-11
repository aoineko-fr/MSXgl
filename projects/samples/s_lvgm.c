// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  VGM format sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "vgm/lvgm_player.h"
#include "dos.h"
#include "dos_mapper.h"
#include "memory.h"
#include "psg.h"
#if (LVGM_USE_MSXMUSIC)
#include "msx-music.h"
#endif
#if (LVGM_USE_MSXAUDIO)
#include "msx-audio.h"
#endif
#if (LVGM_USE_SCC)
#include "scc.h"
#endif
#if (LVGM_USE_MAKOTO)
#include "audio/makoto.h"
#endif

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL						"\x02\x03\x04\x05"

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

// VGM music entry
struct MusicEntry
{
	const c8* Name;
	const c8* File;
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
#include "font/font_mgl_sample8.h"

// Music list
const struct MusicEntry g_MusicEntry[] =
{
	{ "Dragon Slayer 4",      "ds4.lvm" },	
	{ "Maze of Galious",      "galious.lvm" },
	{ "Gambare Goemon",       "goemon.lvm" },
	{ "Metal Gear",           "mg.lvm" },
	{ "Hi no Tori",           "honotori.lvm" },
	{ "Penguin Adventure",    "penguin.lvm" },
	{ "Yurei Kun",            "yureikun.lvm" },
	{ "Final Fantasy (OPLL)", "ff.lvm" },
	{ "Laydock 2 (OPLL)",     "laydock2.lvm" },
	{ "Undeadline (OPLL)",    "undeadli.lvm" },
	{ "F1 Spirit (SCC)",      "f1spirit.lvm" },
	{ "Salamander (SCC)",     "salamand.lvm" },
	{ "Space Manbow (SCC)",   "manbow.lvm" },
	{ "Metal Gear 2 (SCC)",   "mg2.lvm" },
 	{ "Pro Yakyuu (OPL1)",    "proyakyu.lvm" },
 	{ "Xevious (OPL1)",       "xevious.lvm" },
 	{ "Beholder 3 (OPNA)",    "beholder.lvm" },
 	{ "Valis 2 (OPNA)",       "valis2.lvm" },
 	{ "Misty Blue (OPNA)",    "misty.lvm" },
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

// Last segment
DOS_Segment g_SegList[4];
u8 g_SegIdx;
u8 g_SegLoop;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Display lVGM raw data at the current playback position
void DrawRawData(const u8* ptr)
{
	Print_SetPosition(32 - 6, LAYOUT_PLAYER_Y + 1);
	Print_DrawFormat("%i %4x", g_SegIdx, ptr);
}

//-----------------------------------------------------------------------------
// Update player information
void UpdatePlayer()
{
	u8 y = LAYOUT_INFO_Y;
	Print_SetPosition(LAYOUT_INFO_X, y++);
	Print_DrawText("Player:");
	Print_SetPosition(LAYOUT_INFO_X, y++);
	Print_DrawFormat("\x07" "Freq      %s", (g_LVGM_State & LVGM_STATE_50HZ) ? "50Hz" : "60Hz");
	Print_SetPosition(LAYOUT_INFO_X, y++);
	Print_DrawFormat("\x07" "DoLoop    %c", (g_LVGM_State & LVGM_STATE_LOOP) ? '\x0C' : '\x0B');
	Print_SetPosition(LAYOUT_INFO_X, y++);
	Print_DrawFormat("\x07" "DoPlay    %c", (g_LVGM_State & LVGM_STATE_PLAY) ? '\x0C' : '\x0B');
}

//-----------------------------------------------------------------------------
// Handle lVGM notifications
bool MusicNotification(u8 id)
{
	switch (id)
	{
	case LVGM_NOTIFY_SEG_END: // End of data segment
		g_SegIdx++;
		DOSMapper_SetPage2(g_SegList[g_SegIdx].Number); // Select the next segment 
		LVGM_SetPointer((const u8*)0x8000); // Set playback address to the start of the next segment
		return FALSE;

	case LVGM_NOTIFY_LOOP_MARK: // Reach loop marker
		g_SegLoop = g_SegIdx; // Backup the segment that contain the loop marker
		break;
	
	case LVGM_NOTIFY_LOOP_JUMP: // Jump to loop marker
		g_SegIdx = g_SegLoop; // Restore the segment that contain the loop marker
		DOSMapper_SetPage2(g_SegList[g_SegIdx].Number);
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// Reset RAM mapper to default segment
void ResetMapper()
{
	g_SegIdx = 0;
	g_SegLoop = 0;
	DOSMapper_SetPage2(g_SegList[g_SegIdx].Number);
}

//-----------------------------------------------------------------------------
// Select a new music: Upload it to RAM mapper segment, display information and start playback
void SetMusic(u8 idx)
{
	g_CurrentMusic = idx;
	LVGM_Stop();
	LVGM_Decode();
	#if (PSG_ACCESS == PSG_INDIRECT)
	PSG_Apply();
	#endif

	//.........................................................................
	// Init mapper
	ResetMapper();

	//.........................................................................
	// Load file
	u8 err;
	Print_SetPosition(LAYOUT_FILE_X, LAYOUT_FILE_Y);
	Print_DrawCharX(' ', 32 - LAYOUT_FILE_X);
	Print_SetPosition(LAYOUT_FILE_X, LAYOUT_FILE_Y);
	Print_DrawFormat("Load \"%s\"...", g_MusicEntry[idx].File);
	// open
	u8 file = DOS_FOpen(g_MusicEntry[idx].File, O_RDONLY);
	if (file == 0xFF)
		return;
	// get size
	u32 size = DOS_FSeek(file, 0, SEEK_END);
	err = DOS_GetLastError();
	if (err != DOS_ERR_NONE)
		return;
	// rewind
	DOS_FSeek(file, 0, SEEK_SET);
	// load
	while (size > 16 * 1024)
	{
		DOS_FRead(file, (void*)0x8000, 16 * 1024);
		err = DOS_GetLastError();
		if (err != DOS_ERR_NONE)
			return;
		size -= 16 * 1024;
		DOSMapper_SetPage2(g_SegList[++g_SegIdx].Number);
	}
	DOS_FRead(file, (void*)0x8000, size);
	err = DOS_GetLastError();
	if (err != DOS_ERR_NONE)
		return;
	Print_DrawText(" OK!");

	//.........................................................................
	// Start playback
	g_SegIdx = 0;
	DOSMapper_SetPage2(g_SegList[g_SegIdx].Number);
	bool ok = LVGM_Play((const void*)0x8000, TRUE);
	
	//.........................................................................
	// Display music info
	Print_SetPosition(LAYOUT_FILE_X, LAYOUT_FILE_Y);
	Print_DrawCharX(' ', 32 - LAYOUT_FILE_X);
	Print_SetPosition(LAYOUT_FILE_X, LAYOUT_FILE_Y);
	Print_DrawFormat("%i/%i %s", 1 + idx, numberof(g_MusicEntry), g_MusicEntry[idx].Name);

	u8 y = LAYOUT_DATA_Y;
	Print_SetPosition(LAYOUT_DATA_X, y++);
	Print_DrawText("Data:");
	Print_SetPosition(LAYOUT_DATA_X, y++);
	Print_DrawFormat("\x07" "Ident     %c", ok ? '\x0C' : '\x0B');
	Print_SetPosition(LAYOUT_DATA_X, y++);
	Print_DrawFormat("\x07" "Freq      %iHz", (g_LVGM_Header->Option & LVGM_OPTION_50HZ) ? 50 : 60);
	Print_SetPosition(LAYOUT_DATA_X, y++);
	Print_DrawFormat("\x07" "Loop      %c", (g_LVGM_Header->Option & LVGM_OPTION_LOOP) ? '\x0C' : '\x0B');
	Print_SetPosition(LAYOUT_DATA_X, y);
	Print_DrawCharX(' ', 32 - LAYOUT_DATA_X);
	Print_SetPosition(LAYOUT_DATA_X, y++);
	Print_DrawFormat("\x07" "Chips     %c", (g_LVGM_Header->Option & LVGM_OPTION_DEVICE) ? '\x0C' : '\x0B');
	if (LVGM_GetDevices() & LVGM_CHIP_PSG)
		Print_DrawText(" PSG");
	if (LVGM_GetDevices() & LVGM_CHIP_OPLL)
		Print_DrawText(" OPLL");
	if (LVGM_GetDevices() & LVGM_CHIP_OPL1)
		Print_DrawText(" OPL1");
	if (LVGM_GetDevices() & LVGM_CHIP_SCC)
		Print_DrawText(" SCC");
	if (LVGM_GetDevices() & LVGM_CHIP_OPNA)
		Print_DrawText(" OPNA");
	Print_SetPosition(LAYOUT_DATA_X, y++);
	Print_DrawFormat("\x07" "Default   %c %2xh", (LVGM_GetDevices() & LVGM_CHIP_PSG) ? '\x0C' : '\x0B', LVGM_GetDefaultPSGValue());

	UpdatePlayer();
}

//-----------------------------------------------------------------------------
// Handle Play button action
void ButtonPlay()
{
	LVGM_Resume();
	UpdatePlayer();
}

//-----------------------------------------------------------------------------
// Handle Pause button action
void ButtonPause()
{
	LVGM_Pause();
	UpdatePlayer();
}

//-----------------------------------------------------------------------------
// Handle Stop button action
void ButtonStop()
{
	LVGM_Stop();
	ResetMapper();
	UpdatePlayer();
}

//-----------------------------------------------------------------------------
// Handle Previous button action
void ButtonPrev()
{
	if (g_CurrentMusic > 0)
		SetMusic(g_CurrentMusic - 1);
	else
		SetMusic(numberof(g_MusicEntry) - 1);
}

//-----------------------------------------------------------------------------
// Handle Next button action
void ButtonNext()
{
	if (g_CurrentMusic < numberof(g_MusicEntry) - 1)
		SetMusic(g_CurrentMusic + 1);
	else
		SetMusic(0);
}

//-----------------------------------------------------------------------------
// Handle Loop button action
void ButtonLoop()
{
	if (g_LVGM_State & LVGM_STATE_LOOP)
		g_LVGM_State &= ~LVGM_STATE_LOOP;
	else
		g_LVGM_State |= LVGM_STATE_LOOP;
	UpdatePlayer();
}

//-----------------------------------------------------------------------------
// Set player cursor position
void SetCursor(u8 id)
{
	g_CurrentButton = id % 6;
	VDP_SetSpriteSM1(0, (u8)((LAYOUT_PLAYER_X + 1) * 8) + 16 * g_CurrentButton, (u8)(((LAYOUT_PLAYER_Y + 1) * 8) - 1), g_ButtonEntry[g_CurrentButton].Char, COLOR_LIGHT_RED);
}

//-----------------------------------------------------------------------------
// Print a slot number
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

//-----------------------------------------------------------------------------
// Interrupt handle synchronized at 50/60 Hz
void VDP_InterruptHandler()
{
	VDP_SetColor(0xF4);
	LVGM_Decode();
	VDP_SetColor(0xF6);
	#if (PSG_ACCESS == PSG_INDIRECT)
	PSG_Apply();
	#endif
	VDP_SetColor(0xF0);
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize and alloc memory segment
	if (!DOSMapper_Init())
	{
		DOS_StringOutput("Error: Unable to init mapper\n$");
		DOS_Exit(1);
	}
	loop (i, 4)
	{
		if (!DOSMapper_Alloc(DOS_ALLOC_USER, DOS_SEGSLOT_PRIM, &g_SegList[i]))
		{
			DOS_StringOutput("Error: Can't allocate 4 segments of 16 KB\n$");
			DOS_Exit(1);
		}
	}

	// Initialize LVGM module (and all supported audio chip)
	LVGM_Initialize();
	LVGM_SetNotifyCallback(MusicNotification);

	// Initialize screen
	VDP_SetMode(VDP_MODE_SCREEN1);
	VDP_ClearVRAM();
	VDP_EnableVBlank(TRUE);

	// Initialize font and draw title
	Print_SetTextFont(g_Font_MGL_Sample8, 1); // Initialize font
	Print_DrawTextAt(0, 0, MSX_GL " LVGM SAMPLE");
	Print_DrawLineH(0, 1, 32);

	// Hardware detection
	u8 y = LAYOUT_DETECT_Y;
	Print_SetPosition(LAYOUT_DETECT_X, y++);
	Print_DrawText("Main-ROM:");
	Print_SetPosition(LAYOUT_DETECT_X, y);
	Print_DrawFormat("\x07" "Freq  %s", Sys_Is50Hz() ? "50Hz" : "60Hz");
	y += 2;
	Print_SetPosition(LAYOUT_DETECT_X, y++);
	Print_DrawText("Detect: ");
	#if (LVGM_USE_MSXMUSIC)
	Print_SetPosition(LAYOUT_DETECT_X, y++);
	Print_DrawText("\x07" "YM2413: ");
	Print_DrawSlot(MSXMusic_GetSlotId());
	#endif
	#if (LVGM_USE_MSXAUDIO)
	Print_SetPosition(LAYOUT_DETECT_X, y++);
	Print_DrawText("\x07" "Y8950:  ");
	Print_DrawChar(MSXAudio_Detect() ? '\x0C' : '\x0B');
	#endif
	#if (LVGM_USE_SCC)
	Print_SetPosition(LAYOUT_DETECT_X, y++);
	Print_DrawText("\x07" "SCC:    ");
	Print_DrawSlot(SCC_SLOT);
	#endif
	#if (LVGM_USE_MAKOTO)
	Print_SetPosition(LAYOUT_DETECT_X, y++);
	Print_DrawText("\x07" "YM2608: ");
	Print_DrawChar(Makoto_Detect() ? '\x0C' : '\x0B');
	#endif

	// Draw player
	Print_SetDrawStyle(PRINT_DRAW_STYLE_DOUBLE);
	Print_DrawBox(LAYOUT_PLAYER_X, LAYOUT_PLAYER_Y, numberof(g_ButtonEntry) * 2 + 1, 3);
	for (u8 i = 0; i < numberof(g_ButtonEntry); ++i)
	{
		Print_SetPosition(LAYOUT_PLAYER_X + 1 + 2 * i, LAYOUT_PLAYER_Y + 1);
		Print_DrawChar(g_ButtonEntry[i].Char);
		
	}

	// Display footer
	Print_DrawLineH(0, 22, 32);
	Print_DrawTextAt(0, 23, "\x8D:Button \x83:Action \x82:Freq");

	// Initialize player sprite	
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_8 + VDP_SPRITE_SCALE_1);
	VDP_LoadSpritePattern(g_Font_MGL_Sample8 + 4, 0, g_Font_MGL_Sample8[3] - g_Font_MGL_Sample8[2]);

	SetMusic(0);
	SetCursor(4);

	u8 prevRow8 = 0xFF;
	u8 count = 0;
	while (1)
	{
		Halt();
		VDP_InterruptHandler();

		VDP_SetSpriteColorSM1(0, g_ColorBlink[(count >> 2) & 0x03]);
		
		DrawRawData(g_LVGM_Pointer);

		Print_DrawCharAt(31, 0, 0xB0 + ((count >> 2) & 0x7));
		count++;
		
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
			if (g_LVGM_State & LVGM_STATE_50HZ)
				g_LVGM_State &= ~LVGM_STATE_50HZ;
			else
				g_LVGM_State |= LVGM_STATE_50HZ;
			UpdatePlayer();
		}
		// Exit
		if (IS_KEY_PRESSED(row8, KEY_ESC))
			DOS_Exit(0);
		
		prevRow8 = row8;
	}
}