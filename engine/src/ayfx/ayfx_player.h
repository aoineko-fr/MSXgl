// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// ayFX Replayer
//─────────────────────────────────────────────────────────────────────────────
// Based on ayFX Replayer v1.31 by SapphiRe
//  - v1.31		Fixed bug on previous version, only PSG channel C worked
//  - v1.3		Fixed volume and Relative volume versions on the same file, conditional compilation
//				Support for dynamic or fixed channel allocation
//  - v1.2f/r	ayFX bank support
//  - v1.11f/r	If a frame volume is zero then no AYREGS update
//  - v1.1f/r	Fixed volume for all ayFX streams
//  - v1.1		Explicit priority (as suggested by AR)
//  - v1.0f		Bug fixed (error when using noise)
//  - v1.0		Initial release
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

// MGL headers
#include "core.h"
#include "psg.h"

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

// Code include
#define AYFX_USE_RELATIVE 0
#define AYFX_USE_CALLBACK 1

// ayFX modes
enum AYFX_MODE
{
	AYFX_MODE_FIXED   = 0,	// All sounds are played in the same channel
	AYFX_MODE_FSWITCH = 1,	// Frame Switch: Channel is switched each frame during FX playback (should less affected the playing music)
	AYFX_MODE_PSWITCH = 2,	// Play Switch: Channel is switched each time a play occured
};

// Return values from ayFX_Play()
enum AYFX_ERROR
{
	AYFX_ERROR_NONE  = 0,	// No errors, the new sample has been correctly initialized
	AYFX_ERROR_PRIO  = 1,	// Priority error, there's a sample with higher priority being played
	AYFX_ERROR_INDEX = 2,	// Index error, the current bank don't have a sample with such index
};

extern u8  ayFX_Mode;		// ayFX mode (@see AYFX_MODE)
extern u16 ayFX_Bank;		// Current ayFX Bank
extern u8  ayFX_Priority;	// Current ayFX stream priotity
extern u16 ayFX_Pointer;	// Pointer to the current ayFX stream
extern u16 ayFX_Tone;		// Current tone of the ayFX stream
extern u8  ayFX_Noise;		// Current noise of the ayFX stream
extern u8  ayFX_Volume;		// Current volume of the ayFX stream
extern u8  ayFX_Channel;	// PSG channel to play the ayFX stream
#if (AYFX_USE_RELATIVE)
extern u16 ayFX_VT;			// ayFX relative volume table pointer
#endif
extern callback ayFX_Finish; // Finish callback

//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

// Function: ayFX_InitBank
// Setup the ayFX replayer
//
// Paramaters:
//   bank	- Pointer to the ayFX bank data
void ayFX_InitBank(void* bank);

// Function: ayFX_PlayBank
// Play a ayFX sound
//
// Paramaters:
//   id		- Sound index in the bank
//   prio	- Priority of the sound (0-15). 0 is the highest priority.
//
// Returns:
//   Error number (if any). See <AYFX_ERROR>.
u8 ayFX_PlayBank(u8 id, u8 prio);

// Function: ayFX_Play
// Play a SFX directly from data pointer
//
// Paramaters:
//   file	- Pointer to AFX file data
void ayFX_Play(void* data);

// Function: ayFX_Stop
// Stop the sound playback
void ayFX_Stop();

// Function: ayFX_Mute
// Stop the sound playback and mute the channel
void ayFX_Mute();

// Function: ayFX_Update
// Play a frame of an ayFX stream
void ayFX_Update();

// Function: ayFX_SetMode
// Set channel mode
//
// Paramaters:
//   mode	- Playback mode. See <AYFX_MODE>.
inline void ayFX_SetMode(u8 mode) { ayFX_Mode = mode; }

// Function: ayFX_SetChannel
// Set the channel to use for sound playback
//
// Paramaters:
//   chan	- Channel number for sound playback.
inline void ayFX_SetChannel(u8 chan) { ayFX_Channel = 3 - chan; }

// Function: ayFX_GetChannel
// Get the channel used for sound playback
//
// Return:
//   Channel numberused for sound playback.
inline u8 ayFX_GetChannel() { return 3 - ayFX_Channel; }

// Function: ayFX_GetBankNumber
// Get number of sound in the bank
//
// Return:
//   Number of sound in the bank
inline u8 ayFX_GetBankNumber() { return *(u8*)ayFX_Bank; }

// Function: ayFX_SetFinishCB
// Set the function to be call when a sound ended
//
// Paramaters:
//   cb	- Function pointer to be called when sound end
inline void ayFX_SetFinishCB(callback cb) { ayFX_Finish = cb; }
