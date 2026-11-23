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
// FUNCTIONS
//=============================================================================

// Function: AKY9ch_Play
// Initialize music and start playback
//
// Paramaters:
//   num	- Sub-music number if the AKG contain several musics (otherwise set to 0)
//   data	- Pointer to the music data (data must be export to be replayed at this exact location)
//            Check Arkos Tracker documentation for more details: https://www.julien-nevo.com/arkostracker
void AKY9ch_Play(const void* data);

// Function: AKY9ch_Update
// Decode a music frame and update the PSG
bool AKY9ch_Update();

// Function: AKY9ch_InitSFX
// Initialize sound effects. It MUST be called at any time before a first sound effect is triggered.
// It doesn't matter whether a song is playing or not, or even if AKY9ch_Play has been called.
// Note: Sound effects are only played while AKY9ch_Update is called (they are mixed into the music).
//
// Paramaters:
//   data	- Address to the sound effects data.
//
// Return:
//   Number of SFX in the package
u8 AKY9ch_InitSFX(const void* data);

// Function: AKY9ch_PlaySFX
// Plays a sound effect. If a previous one was already playing on the same channel, it is replaced.
// This does not actually plays the sound effect, but programs its playing.
//
// Paramaters:
//   sfx	- Sound effect number (>0!).
//   chan	- The channel where to play the sound effect (0, 1, 2; on the last PSG only).
//   vol	- Inverted volume (0 = full volume, 16 = no sound). Hardware sounds are also lowered.
void AKY9ch_PlaySFX(u8 sfx, u8 chan, u8 vol);

// Function: AKY9ch_StopSFX
// Stops a sound effect. Nothing happens if there was no sound effect.
//
// Paramaters:
//   chan	- The channel where to stop the sound effect (0, 1, 2).
void AKY9ch_StopSFX(u8 chan);
