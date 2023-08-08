// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄  ▄▄ ▄  ▄▄▄  ▄ ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██  ██ █ ██   ██▀█
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄ ▀█▀  ▀█▄█ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "msxgl.h"

#define USE_LVGM_PSG			TRUE
#define USE_LVGM_MSXMUSIC		TRUE
#define USE_LVGM_MSXAUDIO		FALSE
#define USE_LVGM_SCC			TRUE
#define USE_LVGM_SCCI			FALSE
#define USE_LVGM_PSG2			FALSE
#define USE_LVGM_OPL4			FALSE

#if (USE_LVGM_PSG)
#include "psg.h"
#endif
#if (USE_LVGM_MSXMUSIC)
#include "msx-music.h"
#endif
#if (USE_LVGM_MSXAUDIO)
#include "msx-audio.h"
#endif
#if (USE_LVGM_SCC)
#include "scc.h"
#endif

//=============================================================================
// DEFINES
//=============================================================================

// lVGM option
enum LVGM_OPTION
{
	LVGM_OPTION_FREQ   = 0b00000001, // Frequency
	LVGM_OPTION_60HZ   = 0b00000000, // Frequency (0: 60 Hz)
	LVGM_OPTION_50HZ   = 0b00000001, // Frequency (1: 50 Hz)
	LVGM_OPTION_LOOP   = 0b00000010, // Loop data included
	LVGM_OPTION_DEVICE = 0b00000100, // Devices list
	LVGM_OPTION_FORMAT = 0b11110000, // File format version (0)
};

// Get frequency from option
#define LVGM_GET_FREQ(opt) (opt & LVGM_OPTION_FREQ)

// Get format version from option
#define LVGM_GET_VER(opt) (opt >> 4)

// lVGM audio chips
enum LVGM_CHIP
{
	LVGM_CHIP_NONE      = 0,
	// Supported chips
	LVGM_CHIP_PSG       = 0b00000001, // Include PSG data (AY-3-8910)
	LVGM_CHIP_MSXMUSIC  = 0b00000010, // Include MSX-MUSIC data (YM2413/OPLL)
	LVGM_CHIP_MSXAUDIO  = 0b00000100, // Include MSX-AUDIO data (Y8950/OPL1 + ADPCM)
	LVGM_CHIP_SCC       = 0b00001000, // Include SCC data (K051649)
	LVGM_CHIP_SCCI      = 0b00010000, // Include SCC+ data (K052539)
	LVGM_CHIP_PSG2      = 0b00100000, // Include secondary PSG data
	LVGM_CHIP_7         = 0b01000000, // 
	LVGM_CHIP_MOONSOUND = 0b10000000, // Include Moonsound data (YMF278/OPL4)
	// Alias
	LVGM_CHIP_AY8910    = LVGM_CHIP_PSG,
	LVGM_CHIP_YM2413    = LVGM_CHIP_MSXMUSIC,
	LVGM_CHIP_OPLL      = LVGM_CHIP_MSXMUSIC,
	LVGM_CHIP_Y8950     = LVGM_CHIP_MSXAUDIO,
	LVGM_CHIP_OPL1      = LVGM_CHIP_MSXAUDIO,
	LVGM_CHIP_K051649   = LVGM_CHIP_SCC,
	LVGM_CHIP_K052539   = LVGM_CHIP_SCCI,
	LVGM_CHIP_YMF278    = LVGM_CHIP_MOONSOUND,
	LVGM_CHIP_OPL4      = LVGM_CHIP_MOONSOUND,
};

// LVGM playback state flag
enum LVGM_STATE
{
	LVGM_STATE_50HZ = 0b00000001,
	LVGM_STATE_LOOP = 0b00000010,
	LVGM_STATE_PLAY = 0b10000000,
};

// LVGM header structure
struct LVGM_Header
{
	u8	Ident[4]; // 'lVGM'
	u8	Option;     // see <LVGM_OPTION>
	u8	Device;     // see <LVGM_CHIP>
};

//=============================================================================
// VARIABLES
//=============================================================================

// Extern data
extern const struct LVGM_Header* g_LVGM_Header;
extern const u8* g_LVGM_Pointer;
extern u8        g_LVGM_Wait;
extern u8        g_LVGM_State;
extern const u8  g_LVGM_RegTable[13];
extern const u8  g_LVGM_Ident[4];
extern u8        g_LVGM_CurChip;

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: LVGM_Play
// Start music playback
//
// Paramaters:
//   addr	- Address of source data
//   loop	- Is music looping?
bool LVGM_Play(const void* addr, bool loop);

// Function: LVGM_Stop
// Stop music playback
void LVGM_Stop();

// Function: LVGM_SetFrequency50Hz
// Change frequency to 50 Hz
//
// Return:
//   FALSE if music frequency is not 50 Hz
inline bool LVGM_SetFrequency50Hz() { g_LVGM_State |= LVGM_STATE_50HZ; }

// Function: LVGM_SetFrequency60Hz
// Change frequency to 60 Hz
//
// Return:
//   FALSE if music frequency is not 60 Hz
inline bool LVGM_SetFrequency60Hz() { g_LVGM_State &= ~LVGM_STATE_50HZ; }

// Function: LVGM_IsPlaying
// Check if music playing
//
// Return:
//   FALSE if music is not playing
inline bool LVGM_IsPlaying() { return g_LVGM_State & LVGM_STATE_PLAY; }

// Function: LVGM_Resume
// Resume music playback
inline void LVGM_Resume() { g_LVGM_State |= LVGM_STATE_PLAY; }

// Function: LVGM_Pause
// Pause music playback
void LVGM_Pause();

// Function: LVGM_Decode
// Decode a frame of music
void LVGM_Decode();