// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄ ▄▄ ▄ ▄▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ▀█▄▀  ▄█▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▀█  ██  ██▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// WYZ player module
//
// From SDCC WYZ player for MSX by mvac7
// Version: 1.3 (14/06/2021)
// Author:  MSX PSG proPLAYER v0.3 (09.03.2016) by WYZ/Iggy Rock
//          Adapted to SDCC: mvac7/303bcn > <mvac7303b@gmail.com>
// Web:     https://sites.google.com/site/wyzplayer/home
//          http://www.cpcwiki.eu/index.php/WYZTracker
// Desc:    Adaptation of the WYZ music player for programming in C with the SDCC compiler                                        
//─────────────────────────────────────────────────────────────────────────────

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------
#include "core.h"

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

// WYZ player status
// 7 6 5 4 3 2 1 0
// │     │ │ │ │ └── Load song ON/OFF
// │     │ │ │ └──── Player ON/OFF
// │     │ │ └────── EFECTOS ON/OFF
// │     │ └──────── SFX ON/OFF
// │     └────────── Is looping?
// └──────────────── Is ended?
extern u8 g_WYZ_State;

extern u8 g_WYZ_Song; //number of song playing
extern u8 g_WYZ_Tempo; //TEMPO

extern u8 AYREGS[16]; //PSG registers buffer

//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

// Function: WYZ_Initialize
// Initialize the Player
//
// Parameters:
//   song - Songs data index memory address
//   inst - Instruments data index memory address
//   fx   - FXs data index memory address
//   freq - Notes Table memory address
void WYZ_Initialize(u16 song, u16 inst, u16 fx, u16 freq) __sdcccall(0);

// Function: WYZ_Pause
// Pause song playback
void WYZ_Pause() __naked;  

// Function: WYZ_Resume
// Resume song playback
void WYZ_Resume(); 

// Function: WYZ_SetLoop
// Change loop mode
//
// Parameters:
//   loop - false = 0, true = 1
void WYZ_SetLoop(u8 loop) __naked __sdcccall(0);

// Function: WYZ_IsFinished
// Indicates whether the song has finished playing
//
// Returns:
//   true if finished
bool WYZ_IsFinished();

// Function: WYZ_PlayFX
// Play Sound Effect
//
// Parameters:
//   FX number
void WYZ_PlayFX(u8 numSound) __naked __sdcccall(0); 

// Function: WYZ_PlayAY
// Send data from AYREGS buffer to AY registers.
// Execute on each interruption of VBLANK
void WYZ_PlayAY() __naked;

// Function: WYZ_PlaySong
// Start song playback
//
// Parameters:
//   numSong - Song number
//   loop    - Loop status
void WYZ_PlaySong(u8 numSong, bool loop) __sdcccall(0); 

// Function: WYZ_Decode
// Process the next step in the song sequence 
void WYZ_Decode() __naked; 
