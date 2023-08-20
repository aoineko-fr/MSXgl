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
#if (LVGM_USE_SCC)
#include "scc.h"
#endif
#if (LVGM_USE_MSXMUSIC)
#include "msx-music.h"
#endif
#if (LVGM_USE_MSXAUDIO)
#include "msx-audio.h"
#endif

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

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

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
//
void DrawVGM(const u8* ptr)
{
	Print_SetPosition(32-6, 4);
	Print_DrawFormat("%i %4x", g_SegIdx, ptr);
}

//-----------------------------------------------------------------------------
//
void UpdatePlayer()
{
	Print_SetPosition(0, 11);
	Print_DrawText("Player:\n");
	Print_DrawFormat("\x07" "Freq      %s\n", (g_LVGM_State & LVGM_STATE_50HZ) ? "50Hz" : "60Hz");
	Print_DrawFormat("\x07" "DoLoop    %c\n", (g_LVGM_State & LVGM_STATE_LOOP) ? '\x0C' : '\x0B');
	Print_DrawFormat("\x07" "DoPlay    %c\n", (g_LVGM_State & LVGM_STATE_PLAY) ? '\x0C' : '\x0B');
}

//-----------------------------------------------------------------------------
//
bool MusicNotification(u8 id)
{
	switch(id)
	{
	case 0: // End of data segment
		g_SegIdx++;
		DOSMapper_SetPage2(g_SegList[g_SegIdx].Number); // Select the next segment 
		LVGM_SetPointer((const u8*)0x8000); // Set playback address to the start of the next segment
		return FALSE;

	case 0xFE: // Reach loop marker
		g_SegLoop = g_SegIdx; // Backup the segment that contain the loop marker
		break;
	
	case 0xFF: // Jump to loop marker
		g_SegIdx = g_SegLoop; // Restore the segment that contain the loop marker
		DOSMapper_SetPage2(g_SegList[g_SegIdx].Number);
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
//
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
	g_SegIdx = 0;
	g_SegLoop = 0;
	DOSMapper_SetPage2(g_SegList[g_SegIdx].Number);

	//.........................................................................
	// Load file
	u8 err;
	Print_SetPosition(0, 2);
	Print_DrawCharX(' ', 32);
	Print_SetPosition(0, 2);
	Print_DrawFormat("Load \"%s\"...", g_MusicEntry[idx].File);
	// open
	u8 file = DOS_FOpen(g_MusicEntry[idx].File, O_RDONLY);
	if(file == 0xFF)
		return;
	// get size
	u32 size = DOS_FSeek(file, 0, SEEK_END);
	err = DOS_GetLastError();
	if(err != DOS_ERR_NONE)
		return;
	// rewind
	DOS_FSeek(file, 0, SEEK_SET);
	// load
	while(size > 16 * 1024)
	{
		DOS_FRead(file, (void*)0x8000, 16 * 1024);
		err = DOS_GetLastError();
		if(err != DOS_ERR_NONE)
			return;
		size -= 16 * 1024;
		DOSMapper_SetPage2(g_SegList[++g_SegIdx].Number);
	}
	DOS_FRead(file, (void*)0x8000, size);
	err = DOS_GetLastError();
	if(err != DOS_ERR_NONE)
		return;
	Print_DrawText(" OK!");

	//.........................................................................
	// Start playback
	g_SegIdx = 0;
	DOSMapper_SetPage2(g_SegList[g_SegIdx].Number);
	bool ok = LVGM_Play((const void*)0x8000, TRUE);
	
	//.........................................................................
	// Display music info
	Print_SetPosition(0, 2);
	Print_DrawCharX(' ', 32);
	Print_SetPosition(0, 2);
	Print_DrawFormat("%i/%i %s", 1 + idx, numberof(g_MusicEntry), g_MusicEntry[idx].Name);

	Print_SetPosition(0, 4);
	Print_DrawText("Data:\n");
	Print_DrawFormat("\x07" "Ident     %c\n", ok ? '\x0C' : '\x0B');
	Print_DrawFormat("\x07" "Freq      %i Hz\n", (g_LVGM_Header->Option & LVGM_OPTION_50HZ) ? 50 : 60);
	Print_DrawFormat("\x07" "Loop      %c\n", (g_LVGM_Header->Option & LVGM_OPTION_LOOP) ? '\x0C' : '\x0B');
	Print_DrawCharX(' ', 32);
	Print_SetPosition(0, 8);
	Print_DrawFormat("\x07" "Chips     %c", (g_LVGM_Header->Option & LVGM_OPTION_DEVICE) ? '\x0C' : '\x0B');
	if(LVGM_GetDevices() & LVGM_CHIP_PSG)
		Print_DrawText(" PSG");
	if(LVGM_GetDevices() & LVGM_CHIP_OPLL)
		Print_DrawText(" OPLL");
	if(LVGM_GetDevices() & LVGM_CHIP_OPL1)
		Print_DrawText(" OPL1");
	if(LVGM_GetDevices() & LVGM_CHIP_SCC)
		Print_DrawText(" SCC");
	Print_SetPosition(0, 9);
	Print_DrawFormat("\x07" "Default   %c %2xh", (LVGM_GetDevices() & LVGM_CHIP_PSG) ? '\x0C' : '\x0B', LVGM_GetDefaultPSGValue());

	UpdatePlayer();
}

//-----------------------------------------------------------------------------
//
void ButtonPlay()
{
	LVGM_Resume();
	UpdatePlayer();
}

//-----------------------------------------------------------------------------
//
void ButtonPause()
{
	LVGM_Pause();
	UpdatePlayer();
}

//-----------------------------------------------------------------------------
//
void ButtonStop()
{
	LVGM_Stop();	
	UpdatePlayer();
}

//-----------------------------------------------------------------------------
//
void ButtonPrev()
{
	if (g_CurrentMusic > 0)
		SetMusic(g_CurrentMusic - 1);
	else
		SetMusic(numberof(g_MusicEntry) - 1);
}

//-----------------------------------------------------------------------------
//
void ButtonNext()
{
	if (g_CurrentMusic < numberof(g_MusicEntry) - 1)
		SetMusic(g_CurrentMusic + 1);
	else
		SetMusic(0);
}

//-----------------------------------------------------------------------------
//
void ButtonLoop()
{
	if (g_LVGM_State & LVGM_STATE_LOOP)
		g_LVGM_State &= ~LVGM_STATE_LOOP;
	else
		g_LVGM_State |= LVGM_STATE_LOOP;
	UpdatePlayer();
}

//-----------------------------------------------------------------------------
//
void SetCursor(u8 id)
{
	g_CurrentButton = id % 6;
	VDP_SetSpriteSM1(0, 8 + 16*g_CurrentButton, 128+8-1, g_ButtonEntry[g_CurrentButton].Char, COLOR_LIGHT_RED);
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

//-----------------------------------------------------------------------------
//
void VDP_InterruptHandler()
{
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize and alloc memory segment
	if(!DOSMapper_Init())
	{
		DOS_StringOutput("Error: Unable to init mapper\n$");
		DOS_Exit(1);
	}
	loop(i, 4)
	{
		if(!DOSMapper_Alloc(DOS_ALLOC_USER, DOS_SEGSLOT_PRIM, &g_SegList[i]))
		{
			DOS_StringOutput("Error: Can't allocate 4 segments of 16 KB\n$");
			DOS_Exit(1);
		}
	}

	// Initialize screen
	VDP_SetMode(VDP_MODE_SCREEN1);
	VDP_ClearVRAM();
	VDP_EnableVBlank(TRUE);

	// Initialize audio chip
	LVGM_SetNotifyCallback(MusicNotification);

	#if (LVGM_USE_SCC)
	SCC_Initialize();
	#endif
	#if (LVGM_USE_MSXMUSIC)
	MSXMusic_Initialize();
	#endif
	#if (LVGM_USE_MSXAUDIO)
	MSXAudio_Initialize();
	#endif

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample8, 1); // Initialize font
	Print_DrawText(MSX_GL " LVGM Sample");
	Print_DrawLineH(0, 1, 32);

	Print_SetPosition(20, 11);
	Print_DrawText("Main-ROM:");
	Print_SetPosition(20, 12);
	Print_DrawFormat("\x07" "Freq  %s", (g_ROMVersion.VSF) ? "50Hz" : "60Hz");

	u8 Y = 17;
	Print_SetPosition(20, Y++);
	Print_DrawText("Slots: ");
	#if (LVGM_USE_MSXMUSIC)
	Print_SetPosition(20, Y++);
	Print_DrawText("\x07" "YM2413: ");
	Print_DrawSlot(g_MSXMusic_SlotId);
	#endif
	#if (LVGM_USE_MSXAUDIO)
	Print_SetPosition(20, Y++);
	Print_DrawText("\x07" "Y8950:  ");
	Print_DrawChar(MSXAudio_Detect() ? '\x0C' : '\x0B');
	#endif
	#if (LVGM_USE_SCC)
	Print_SetPosition(20, Y++);
	Print_DrawText("\x07" "SCC:    ");
	Print_DrawSlot(SCC_SLOT);
	#endif

	// Decode VGM header
	SetMusic(0);
	Print_DrawBox(0, 16, numberof(g_ButtonEntry) * 2 + 1, 3);
	for(u8 i = 0; i < numberof(g_ButtonEntry); ++i)
	{
		Print_SetPosition(1 + 2 * i, 17);
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
		LVGM_Decode();
VDP_SetColor(0xF6);
		#if (PSG_ACCESS == PSG_INDIRECT)
		PSG_Apply();
		#endif
VDP_SetColor(0xF0);
		VDP_SetSpriteColorSM1(0, g_ColorBlink[(count >> 2) & 0x03]);
		
		DrawVGM(g_LVGM_Pointer);

		Print_SetPosition(31, 0);
		u8 chr = count++ & 0x03;
		Print_DrawChar(g_ChrAnim[chr]);
		
		// Handle input
		u8 row8 = Keyboard_Read(8);

		// Change button
		if (IS_KEY_PRESSED(row8, KEY_RIGHT) && !IS_KEY_PRESSED(prevRow8, KEY_RIGHT))
		{
			SetCursor(g_CurrentButton + 1);
		}
		else if (IS_KEY_PRESSED(row8, KEY_LEFT) && !IS_KEY_PRESSED(prevRow8, KEY_LEFT))
		{
			SetCursor(g_CurrentButton - 1);
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