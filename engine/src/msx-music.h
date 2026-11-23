// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄  ▄▄▄ ▄▄ ▄     ▄ ▄           ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▀█▄  ▀█▄▀ ▄▄ ██▀█ ██ █  ██▀ ▄  ▄█▀▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▄▄█▀ ██ █    ██ █ ▀█▄█ ▄██  ██ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// MSX-Music (YM2413) handler module
//
// References:
//  - YM2413 FM Operator Type-LL (OPLL) Application Manual
//     https://www.smspower.org/maxim/Documents/YM2413ApplicationManual
//  - MSX-MUSIC programming
//     https://www.msx.org/wiki/MSX-MUSIC_programming
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"
#include "msx-music_reg.h"
#include "audio/audio.h"

//-----------------------------------------------------------------------------
// CHECK OPTIONS
//-----------------------------------------------------------------------------

// MSXMUSIC_USE_CHANNEL
#ifndef MSXMUSIC_USE_CHANNEL
	#warning MSXMUSIC_USE_CHANNEL is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define MSXMUSIC_USE_CHANNEL	TRUE
#endif

//=============================================================================
// DEFINES
//=============================================================================

// Internal/external MSX-Music I/O ports
#define P_MSXMUSIC_INDEX			0x7C // Register Index
__sfr __at(P_MSXMUSIC_INDEX)		io_MSXMusic_Reg;
#define P_MSXMUSIC_DATA				0x7D // Register data
__sfr __at(P_MSXMUSIC_DATA)			io_MSXMusic_Data;

// Detection result
enum MSXMUSIC_DETECTION
{
	MSXMUSIC_NOTFOUND = 0, // No MSX-Music found
	MSXMUSIC_INTERNAL = 1, // Internal MSX-Music device found
	MSXMUSIC_EXTERNAL = 2, // External MSX-Music device found (FM-PAC)
};

// Slot of the MSX-Music chip
extern u8 g_MSXMusic_SlotId;

//=============================================================================
// PROTOTYPE
//=============================================================================

// Group: Core

// Function: MSXMusic_Initialize
// Initialize MSX-Music module
//
// Return:
//   FALSE if not found
bool MSXMusic_Initialize();

// Function: MSXMusic_Detect
// Search for MSX-Music (YM2413) chip
//
// Return:
//   MSXMUSIC_NOTFOUND if no MSX-Music chip found
//   MSXMUSIC_INTERNAL if an internal MSX-Music chip is found
//   MSXMUSIC_EXTERNAL if an external MSX-Music chip (FM-PAC)
u8 MSXMusic_Detect();

// Function: MSXMusic_GetSlotId
// Get the Slot ID of the first MSX-Music chip found
// <MSXMusic_Detect> or <MSXMusic_Initialize> must be called first
//
// Return:
//   Slot ID if found or SLOT_NOTFOUND otherwise
inline u8 MSXMusic_GetSlotId() { return g_MSXMusic_SlotId; }

// Function: MSXMusic_SetRegister
// Set MSX-Music register value
//
// Parameters:
//   reg   - Register to set
//   value - Value to set
void MSXMusic_SetRegister(u8 reg, u8 value);

// Function: MSXMusic_GetRegister
// Get MSX-Music register value
//
// Parameters:
//   reg - Register to get
//
// Return:
//   Value of the register
u8 MSXMusic_GetRegister(u8 reg);

// Group: Player

// Function: MSXMusic_Mute
// Mute MSX-Music sound
void MSXMusic_Mute();

#if (MSXMUSIC_USE_PAUSE)
// Function: MSXMusic_Resume
// Resume MSX-Music sound
void MSXMusic_Resume();
#endif

#if (MSXMUSIC_USE_CHANNEL)
// Group: Channel Info
// Note: MSXMUSIC_USE_CHANNEL need to be set to TRUE to use functions from this group

// Function: MSXMusic_GetChannelCount
// Get the number of MSX-Music channels (9 melodic channels and 5 rhythm instruments)
//
// Return:
//   Number of channels
inline u8 MSXMusic_GetChannelCount() { return 14; }

// Function: MSXMusic_GetChannelVolume
// Get the current volume of a given channel (based on the values written with <MSXMusic_SetRegister>)
//
// Parameters:
//   chan - Channel index (0 to 8 for melodic channels, 9 to 13 for rhythm instruments)
//
// Return:
//   0 (off) or 15 (keyed on)
u8 MSXMusic_GetChannelVolume(u8 chan);

// Function: MSXMusic_GetChannelType
// Get the type of a given channel
//
// Parameters:
//   chan - Channel index (0 to 13)
//
// Return:
//   Channel type
inline u8 MSXMusic_GetChannelType(u8 chan) { return (chan < 9) ? CHANNEL_TYPE_FM_INST : CHANNEL_TYPE_FM_RHYTHM; }
#endif
