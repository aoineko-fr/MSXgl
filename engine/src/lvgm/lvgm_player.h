// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "psg.h"

//=============================================================================
// DEFINES
//=============================================================================

#define USE_LVGM_SCC 0

// LVGM data flag
enum LVGM_FLAG
{
	LVGM_FLAG_50HZ = 0b00000001,
	LVGM_FLAG_60HZ = 0b00000010,
	LVGM_FLAG_LOOP = 0b00000100,

	LVGM_FLAG_SCC  = 0b00010000,
	LVGM_FLAG_SCCI = 0b00100000,
	LVGM_FLAG_ext3 = 0b00110000,
	LVGM_FLAG_ext4 = 0b01000000,
	LVGM_FLAG_ext5 = 0b01010000,
	LVGM_FLAG_ext6 = 0b01100000,
	LVGM_FLAG_ext7 = 0b01110000,
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
	u8	Ident[4]; // 'ayM '
	u8	Flag;     // see LVGM_FLAG
};

// Extern data
extern const struct LVGM_Header* g_LVGM_Header;
extern const u8* g_LVGM_Pointer;
extern u8        g_LVGM_Wait;
extern u8        g_LVGM_State;
extern const u8  g_LVGM_RegTable[13];
extern const u8  g_LVGM_Ident[4];

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
inline void LVGM_Pause() { g_LVGM_State &= ~LVGM_STATE_PLAY; PSG_Mute(); }

// Function: LVGM_Decode
// Decode a frame of music
void LVGM_Decode();