// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄            
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄█ ██▄▀ ██▄▀ ▄█▀▄  ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ██   ██ █ ▀█▄▀ ▄██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Arkos AKG replayer (Generic)
//
// Arkos Tracker and the Z80 players are coded and designed by Julien Névo a.k.a Targhan/Arkos.
//  https://www.julien-nevo.com/arkostracker/index.php/credits/
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "arkos.h"

//=============================================================================
// DEFINES
//=============================================================================

extern bool g_AKG_Playing;

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: AKG_Init
// Initialize a music and start playback
//
// Paramaters:
//   num	- Sub-music number if the AKG contain several musics (otherwise set to 0)
//   data	- Pointer to the music data (data must be export to be replayed at this exact location)
//            Check Arkos Tracker 2 documentation for more details: https://www.julien-nevo.com/arkostracker
void AKG_Play(u8 sng, const void* data);

// Function: AKG_IsPlaying
// Check if a music is currently playing
//
// Return:
//   FALSE if no music is playing
inline bool AKG_IsPlaying() { return g_AKG_Playing; }

// Function: AKG_Stop
// Stop current music playback
void AKG_Stop();

// Function: AKG_Decode
// Decode a music frame and update the PSG (must be called once each frame)
//
// Return:
//   TRUE if end of music have been reached
bool AKG_Decode();

// Function: AKG_InitSFX
// Initialize sound effects. It MUST be called at any time before a first sound effect is triggered.
// It doesn't matter whether a song is playing or not, or even if AKG_Init has been initialized.
//
// Paramaters:
//   data	- Address to the sound effects data.
//
// Return:
//   Number of SFX in the package
u8 AKG_InitSFX(const void* data);

// Function: AKG_PlaySFX
// Plays a sound effect. If a previous one was already playing on the same channel, it is replaced.
// This does not actually plays the sound effect, but programs its playing.
//
// Paramaters:
//   sfx	- Sound effect number (>0!).
//   chan	- The channel where to play the sound effect (0, 1, 2).
//   vol	- Inverted volume (0 = full volume, 16 = no sound). Hardware sounds are also lowered.
void AKG_PlaySFX(u8 sfx, u8 chan, u8 vol);

// Function: AKG_StopSFX
// Stops a sound effect. Nothing happens if there was no sound effect.
//
// Paramaters:
//   chan	- The channel where to stop the sound effect (0, 1, 2).
void AKG_StopSFX(u8 chan);
