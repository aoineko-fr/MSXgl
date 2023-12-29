// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄  ▄▄ ▄  ▄▄▄  ▄ ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██  ██ █ ██   ██▀█
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄ ▀█▀  ▀█▄█ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "msxgl.h"

//-----------------------------------------------------------------------------
// CHECK OPTIONS
//-----------------------------------------------------------------------------

// LVGM_USE_PSG
#ifndef LVGM_USE_PSG
	#warning LVGM_USE_PSG is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define LVGM_USE_PSG			TRUE
#endif

// LVGM_USE_MSXMUSIC
#ifndef LVGM_USE_MSXMUSIC
	#warning LVGM_USE_MSXMUSIC is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define LVGM_USE_MSXMUSIC		TRUE
#endif

// LVGM_USE_MSXAUDIO
#ifndef LVGM_USE_MSXAUDIO
	#warning LVGM_USE_MSXAUDIO is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define LVGM_USE_MSXAUDIO		TRUE
#endif

// LVGM_USE_SCC
#ifndef LVGM_USE_SCC
	#warning LVGM_USE_SCC is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define LVGM_USE_SCC			TRUE
#endif

// LVGM_USE_SCCI
#ifndef LVGM_USE_SCCI
	#warning LVGM_USE_SCCI is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define LVGM_USE_SCCI			FALSE
#endif

// LVGM_USE_PSG2
#ifndef LVGM_USE_PSG2
	#warning LVGM_USE_PSG2 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define LVGM_USE_PSG2			FALSE
#endif

// LVGM_USE_OPL4
#ifndef LVGM_USE_OPL4
	#warning LVGM_USE_OPL4 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define LVGM_USE_OPL4			FALSE
#endif

// LVGM_USE_NOTIFY
#ifndef LVGM_USE_NOTIFY
	#warning LVGM_USE_NOTIFY is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define LVGM_USE_NOTIFY			TRUE
#endif

#if (LVGM_USE_PSG)
#include "psg.h"
#endif
#if (LVGM_USE_MSXMUSIC)
#include "msx-music.h"
#endif
#if (LVGM_USE_MSXAUDIO)
#include "msx-audio.h"
#endif
#if (LVGM_USE_SCC)
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

// Macro: LVGM_GET_FREQ
// Get frequency from option
#define LVGM_GET_FREQ(opt) (opt & LVGM_OPTION_FREQ)

// Macro: LVGM_GET_VER
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

// Special operator
enum LVGM_OP
{
	LVGM_OP_PSG			= 0xF0, // Start of PSG chunk (default when not defined)
	LVGM_OP_OPLL		= 0xF1, // Start of MSX-MUSIC chunk
	LVGM_OP_OPL1		= 0xF2, // Start of MSX-AUDIO chunk
	LVGM_OP_SCC			= 0xF3, // Start of SCC chunk
	LVGM_OP_SCCI		= 0xF4, // Start of SCC+ chunk
	LVGM_OP_PSG2		= 0xF5, // Start of secondary PSG chunk
	LVGM_OP_OPL4		= 0xF7, // Start of Moonsound chunk
	LVGM_OP_NOTIFY		= 0xFD, // Optional markers
	LVGM_OP_LOOP		= 0xFE, // Loop position
	LVGM_OP_END			= 0xFF, // End of song
};

// Special operator
enum LVGM_NOTIFY
{
	LVGM_NOTIFY_SEG_END		= 0x00, // End of data segment
	LVGM_NOTIFY_LOOP_MARK	= 0xFE, // Reach loop marker
	LVGM_NOTIFY_LOOP_JUMP	= 0xFF, // Jump to loop marker
};

// LVGM header structure
struct LVGM_Header
{
	u8	Ident[4]; // 'lVGM'
	u8	Option;     // see <LVGM_OPTION>
	u8	Device;     // see <LVGM_CHIP>
};

// Notification callback signature
typedef bool (*LVGM_NotifyCB)(u8);

//=============================================================================
// VARIABLES
//=============================================================================

// Extern data
extern const struct LVGM_Header* g_LVGM_Header;
extern const u8* g_LVGM_Pointer;
extern u8        g_LVGM_Wait;
extern u8        g_LVGM_State;
extern const u8  g_LVGM_Ident[4];
extern u8        g_LVGM_CurChip;
extern u8        g_LVGM_Devices;
extern u8        g_LVGM_PSG_Default;

#if (LVGM_USE_NOTIFY)
extern LVGM_NotifyCB g_LVGM_Callback;
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: LVGM_Play
// Start music playback.
//
// Paramaters:
//   addr	- Address of source data
//   loop	- Is music looping?
//
// Return:
//   TRUE if playback started correctly.
bool LVGM_Play(const void* addr, bool loop);

// Function: LVGM_Stop
// Stop music playback.
void LVGM_Stop();

// Function: LVGM_SetFrequency50Hz
// Change frequency to 50 Hz.
inline void LVGM_SetFrequency50Hz() { g_LVGM_State |= LVGM_STATE_50HZ; }

// Function: LVGM_IsFrequency50Hz
// Check if frequency is 50 Hz.
//
// Return:
//   FALSE if frequency is not 50 Hz.
inline bool LVGM_IsFrequency50Hz() { return g_LVGM_State & LVGM_STATE_50HZ; }

// Function: LVGM_SetFrequency60Hz
// Change frequency to 60 Hz.
inline void LVGM_SetFrequency60Hz() { g_LVGM_State &= ~LVGM_STATE_50HZ; }

// Function: LVGM_SetFrequency60Hz
// Check if frequency is 60 Hz.
//
// Return:
//   TRUE if frequency is 60 Hz.
inline bool LVGM_IsFrequency60Hz() { return (g_LVGM_State & LVGM_STATE_50HZ) == 0; }

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

// Function: LVGM_SetPointer
// Decode a frame of music
inline void LVGM_SetPointer(const u8* ptr) { g_LVGM_Pointer = ptr; }

// Function: LVGM_GetDevices
// Get devices list
inline u8 LVGM_GetDevices() { return g_LVGM_Devices; }

// Function: LVGM_IncludePSG
// Is music including PSG data
inline bool LVGM_IncludePSG() { return g_LVGM_Devices & LVGM_CHIP_PSG; }

// Function: LVGM_IncludeOPLL
// Is music including OPLL/MSX-Music data
inline bool LVGM_IncludeOPLL() { return g_LVGM_Devices & LVGM_CHIP_MSXMUSIC; }

// Function: LVGM_IncludeOPL
// Is music including OPL/MSX-Audio data
inline bool LVGM_IncludeOPL() { return g_LVGM_Devices & LVGM_CHIP_MSXAUDIO; }

// Function: LVGM_IncludeSCC
// Is music including SCC data
inline bool LVGM_IncludeSCC() { return g_LVGM_Devices & LVGM_CHIP_SCC; }

// Function: LVGM_GetDefaultPSGValue
// Get default PSG value
inline u8 LVGM_GetDefaultPSGValue() { return g_LVGM_PSG_Default; }


#if (LVGM_USE_NOTIFY)
// Function: LVGM_SetNotifyCallback
// Set the function to be called when a notification is triggered
inline void LVGM_SetNotifyCallback(LVGM_NotifyCB cb) { g_LVGM_Callback = cb; }
#endif

