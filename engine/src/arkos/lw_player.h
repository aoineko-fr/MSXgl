// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄            
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄█ ██▄▀ ██▄▀ ▄█▀▄  ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ██   ██ █ ▀█▄▀ ▄██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Arkos LW replayer (Lightweight)
//
// Arkos Tracker and the Z80 players are coded and designed by Julien Névo a.k.a Targhan/Arkos.
//  https://www.julien-nevo.com/arkostracker/index.php/credits/
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// VALIDATE
//=============================================================================

// AKL_SFX_STARTIDX
#if !defined(AKL_SFX_STARTIDX)
	#warning AKL_SFX_STARTIDX is not defined in "msxgl_config.h"! Default value will be used: 1
	#define AKL_SFX_STARTIDX				1
#endif

//=============================================================================
// DEFINES
//=============================================================================

extern bool g_AKL_Playing;

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: AKL_Play
// Initialize music and start playback
//
// Paramaters:
//   num	- Sub-music number if the AKG contain several musics (otherwise set to 0)
//   data	- Pointer to the music data (data must be export to be replayed at this exact location)
//            Check Arkos Tracker documentation for more details: https://www.julien-nevo.com/arkostracker
void AKL_Play(u8 sng, const void* data);

// Function: AKL_IsPlaying
// Check if a music is currently playing
//
// Return:
//   FALSE if no music is playing
inline bool AKL_IsPlaying() { return g_AKL_Playing; }

// Function: AKL_Stop
// Stop music playback
void AKL_Stop();

// Function: AKL_Update
// Decode a music frame and update the PSG
bool AKL_Update();

// Function: AKL_InitSFX
// Initializes the sound effects. It MUST be called at any times before a first sound effect is triggered.
// It doesn't matter whether the song is playing or not, or if it has been initialized or not.
//
// Paramaters:
//   data	- Address to the sound effects data.
//
// Return:
//   Number of SFX in the package
u8 AKL_InitSFX(const void* data);

// Function: AKL_PlaySFX
// Plays a sound effect. If a previous one was already playing on the same channel, it is replaced.
// This does not actually plays the sound effect, but programs its playing.
//
// Paramaters:
//   sfx	- Sound effect number (>0!).
//   chan	- The channel where to play the sound effect (0, 1, 2).
//   vol	- Inverted volume (0 = full volume, 16 = no sound). Hardware sounds are also lowered.
void AKL_PlaySFX(u8 sfx, u8 chan, u8 vol);

// Function: AKL_StopSFX
// Stops a sound effect. Nothing happens if there was no sound effect.
//
// Paramaters:
//   chan	- The channel where to stop the sound effect (0, 1, 2).
void AKL_StopSFX(u8 chan);