// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄  ▄▄▄ ▄▄ ▄     ▄▄         ▄▄ ▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▀█▄  ▀█▄▀ ▄▄ ██▄█ ██ █  ▄██ ▄  ▄█▀▄
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▄▄█▀ ██ █    ██ █ ▀█▄█ ▀▄██ ██ ▀█▄▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// MSX-Audio (Y8950) handler module
//
// References:
//  - 
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"
#include "msx-audio_reg.h"
#include "audio/audio.h"

//-----------------------------------------------------------------------------
// CHECK OPTIONS
//-----------------------------------------------------------------------------

// MSXAUDIO_USE_CHANNEL
#ifndef MSXAUDIO_USE_CHANNEL
	#warning MSXAUDIO_USE_CHANNEL is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define MSXAUDIO_USE_CHANNEL	TRUE
#endif

//=============================================================================
// DEFINES
//=============================================================================

// Primary MSX-Audio I/O ports
#define P_MSXAUDIO_INDEX			0xC0 // Register Index
__sfr __at(P_MSXAUDIO_INDEX)		io_MSXAudio_Reg;
#define P_MSXAUDIO_DATA				0xC1 // Register data
__sfr __at(P_MSXAUDIO_DATA)			io_MSXAudio_Data;

// Secondary MSX-Audio I/O ports
#define P_MSXAUDIO_INDEX2			0xC2 // Register Index
__sfr __at(P_MSXAUDIO_INDEX2)		io_MSXAudio_Reg2;
#define P_MSXAUDIO_DATA2			0xC3 // Register data
__sfr __at(P_MSXAUDIO_DATA2)		io_MSXAudio_Data2;

//=============================================================================
// PROTOTYPE
//=============================================================================

// Group: Core

// Function: MSXAudio_Initialize
// Initialize MSX-Audio module
//
// Return:
//   FALSE if no MSX-Audio chip found
bool MSXAudio_Initialize();

// Function: MSXAudio_Detect
// Search for MSX-Audio (Y8950) chip
//
// Return:
//   FALSE if no MSX-Audio chip found
bool MSXAudio_Detect();

// Function: MSXAudio_SetRegister
// Set MSX-Audio register value
//
// Parameters:
//   reg   - Register to set
//   value - Value to set
void MSXAudio_SetRegister(u8 reg, u8 value);

// Function: MSXAudio_GetRegister
// Get MSX-Audio register value
//
// Parameters:
//   reg - Register to get
//
// Return:
//   Value of the register
u8 MSXAudio_GetRegister(u8 reg);

// Group: Player

// Function: MSXAudio_Mute
// Mute MSX-Audio sound
void MSXAudio_Mute();

#if (MSXAUDIO_USE_RESUME)
// Function: MSXAudio_Resume
// Resume MSX-Audio sound
void MSXAudio_Resume();
#endif

#if (MSXAUDIO_USE_CHANNEL)
// Group: Channel Info
// MSXAUDIO_USE_CHANNEL need to be set to TRUE to use functions from this group

// Function: MSXAudio_GetChannelCount
// Get the number of MSX-Audio channels (9 FM channels, 5 rhythm instruments and ADPCM)
//
// Return:
//   Number of channels
inline u8 MSXAudio_GetChannelCount() { return 15; }

// Function: MSXAudio_GetChannelVolume
// Get the current volume of a given channel (based on the values written with <MSXAudio_SetRegister>)
//
// Parameters:
//   chan - Channel index (0 to 8 for FM channels, 9 to 13 for rhythm instruments, 14 for ADPCM)
//
// Return:
//   0 (off) or 15 (keyed on or playing)
u8 MSXAudio_GetChannelVolume(u8 chan);

// Function: MSXAudio_GetChannelType
// Get the type of a given channel
//
// Parameters:
//   chan - Channel index (0 to 14)
//
// Return:
//   Channel type
inline u8 MSXAudio_GetChannelType(u8 chan) { return (chan < 9) ? CHANNEL_TYPE_FM_INST : (chan < 14) ? CHANNEL_TYPE_FM_RHYTHM : CHANNEL_TYPE_SAMPLE; }
#endif
