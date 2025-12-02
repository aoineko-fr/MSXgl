// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄            
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄█ ██▄▀ ██▄▀ ▄█▀▄  ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ██   ██ █ ▀█▄▀ ▄██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Arkos AKY replayer (Fastest)
//
// Arkos Tracker and the Z80 players are coded and designed by Julien Névo a.k.a Targhan/Arkos.
//  https://www.julien-nevo.com/arkostracker/index.php/credits/
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "arkos.h"

//=============================================================================
// VALIDATE
//=============================================================================

// AKY_BUFFER_ADDR
#if !defined(AKY_BUFFER_ADDR)
	#warning AKY_BUFFER_ADDR is not defined in "msxgl_config.h"! Default value will be used: 0xF000
	#define AKY_BUFFER_ADDR				0xF000
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: AKY_Play
// Initialize music and start playback
//
// Paramaters:
//   num	- Sub-music number if the AKG contain several musics (otherwise set to 0)
//   data	- Pointer to the music data (data must be export to be replayed at this exact location)
//            Check Arkos Tracker documentation for more details: https://www.julien-nevo.com/arkostracker
void AKY_Play(u8 num, const void* data);

// Function: AKY_Stop
// Stop music playback
// void AKY_Stop();

// Function: AKY_Update
// Decode a music frame and update the PSG
bool AKY_Update();