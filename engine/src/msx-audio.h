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

// Function: MSXAudio_Mute
// Mute MSX-Audio sound
void MSXAudio_Mute();

#if (MSXAUDIO_USE_RESUME)
// Function: MSXAudio_Resume
// Resume MSX-Audio sound
void MSXAudio_Resume();
#endif
