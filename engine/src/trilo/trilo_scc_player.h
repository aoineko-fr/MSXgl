// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄      ▄  ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ██▄▀ ▄  ██  ▄█▀▄
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │   ██  ██   ██ ▀█▄ ▀█▄▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Trilo Tracker SCC replayer
//
// Trilo Tracker and the Z80 players are coded and designed by Cornelisser.
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

extern const void* g_TriloSCC_ToneTable;
extern const void* g_TriloSFX_Bank;
extern const void* g_TriloSFX_WaveTable;
extern u8 g_TriloSCC_Freq;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Group: SCC player
//-----------------------------------------------------------------------------

// Function: TriloSCC_Initialize
// Initialize replayer data. Only call this on start-up.
void TriloSCC_Initialize();

// Function: TriloSCC_Pause
// Stop/Restart music playback.
void TriloSCC_Pause();

// Function: TriloSCC_Resume
// Stop/Restart music playback.
inline void TriloSCC_Resume() { TriloSCC_Pause(); }

// Function: TriloSCC_FadeOut
// Fade out the music. Once the sound is silence the replayer is paused.
//
// Parameters:
//   speed - Fade speed (1 - 255)
void TriloSCC_FadeOut(u8 speed);

// Function: TriloSCC_SetBalanceSCC
// Set the main volume for the SCC chip.
// This enables for setting the balance between SCC and PSG as some MSX models default balance differs.
//
// Parameters:
//   vol - Master volume (0-7) 0=halve volume, 8=full volume. 
void TriloSCC_SetBalanceSCC(u8 vol);

// Function: TriloSCC_SetBalancePSG
// Set the main volume for the PSG chip.
// This enables for setting the balance between SCC and PSG as some MSX models default balance differs.
//
// Parameters:
//   vol - Master volume (0-7) 0=halve volume, 8=full volume. 
void TriloSCC_SetBalancePSG(u8 vol);

#define TRILO_50HZ 0x00
#define TRILO_60HZ 0xFF

// Function: TriloSCC_SetFrequency
// Set playback frequency.
//
// Parameters:
//   freq - Can be TRILO_50HZ (0) or TRILO_60HZ (not 0).
inline void TriloSCC_SetFrequency(u8 freq) { g_TriloSCC_Freq = freq; }

// Function: TriloSCC_LoadMusic
// Initialize a music for playback.
//
// Parameters:
//   addr - Address of the music data
void TriloSCC_LoadMusic(const void* addr);

// Function: TriloSCC_SetToneTable
// Set a custom tone table.
//
// Parameters:
//   addr - Address of the tone table
inline void TriloSCC_SetToneTable(const void* addr) { g_TriloSCC_ToneTable = addr; }

// Function: TriloSCC_Silent
// Set mixer values to silent.
void TriloSCC_Silent();

// Function: TriloSCC_Update
// Decode music data and process instruments and effects.
void TriloSCC_Update();

// Function: TriloSCC_Apply
// Output the data to the CHIP registers.
void TriloSCC_Apply();

//-----------------------------------------------------------------------------
// Group: SFX player
//-----------------------------------------------------------------------------

// Function: TriloSFX_Initialize
// Initialise the sfx player. Sets initial priorities, volume balancce and initial SCC waveform.
// Note : TriloTracker SFX replayer is ayFX bank format but use 2 bank entry for each Trilo SFX.
// The first one describe the PSG part of the SFX and the second one the SCC part of the SFX.
void TriloSFX_Initialize();

// Function: TriloSFX_SetBank
// Set the current SFX bank data
//
// Parameters:
//   bank - Address of the ayFX bank data
//   wave - Address of the bank wave table
inline void TriloSFX_SetBank(const void* bank, const void* wave) { g_TriloSFX_Bank = bank; g_TriloSFX_WaveTable = wave; }

// Function: TriloSFX_GetNumber
// Get the number of SFX in the bank.
//
// Return:
//   Number of SFX in the bank.
//   Note: TriloTracker SFX replayer use 2 bank entry for each SFX so return size is half the ayFX bank size.
inline u8 TriloSFX_GetNumber() { return *((u8*)g_TriloSFX_Bank) / 2; }

// Function: TriloSFX_SetBalanceSCC
// Set the main SFX volume for the SCC chip.
//
// Parameters:
//   vol - Master volume (0-15) 0=silenced, 15=full volume.
void TriloSFX_SetBalanceSCC(u8 vol);

// Function: TriloSFX_SetBalancePSG
// Set the main SFXvolume for the PSG chip.
//
// Parameters:
//   vol - Master volume (0-15) 0=silenced, 15=full volume.
void TriloSFX_SetBalancePSG(u8 vol);

// Function: TriloSFX_Play
// Start a new SFX.
//
// Parameters:
//   sfx - SFX index in the bank.
//   prio - Sound priority (0 is lowest, 255 is highest).
void TriloSFX_Play(u8 sfx, u8 prio);

// Function: TriloSFX_Update
// Decode SFX streams.
void TriloSFX_Update();