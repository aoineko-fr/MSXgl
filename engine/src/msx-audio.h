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

#include "core.h"
#include "msx-audio_reg.h"

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// PROTOTYPE
//=============================================================================

// Function: MSXAudio_Initialize
// Initialize MSX-Audio module
void MSXAudio_Initialize();

// Function: MSXAudio_Detect
// Search for MSX-Audio (Y8950) chip
bool MSXAudio_Detect();

// Function: MSXAudio_SetRegister
// Set MSX-Audio register value
void MSXAudio_SetRegister(u8 reg, u8 value);

// Function: MSXAudio_GetRegister
// Get MSX-Audio register value
u8 MSXAudio_GetRegister(u8 reg);

// Function: MSXAudio_Mute
// Mute MSX-Audio sound
void MSXAudio_Mute();

#if (MSXAUDIO_USE_RESUME)
// Function: MSXAudio_Resume
// Resume MSX-Audio sound
void MSXAudio_Resume();
#endif
