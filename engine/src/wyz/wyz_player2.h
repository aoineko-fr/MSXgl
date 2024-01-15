// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄ ▄▄ ▄ ▄▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ▀█▄▀  ▄█▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▀█  ██  ██▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// WYZ player module
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------
#include "core.h"
#include "psg.h"

//-----------------------------------------------------------------------------
// VALIDATE
//-----------------------------------------------------------------------------

// WYZ_CHANNELS
#if !defined(WYZ_CHANNELS)
	#warning WYZ_CHANNELS is not defined in "msxgl_config.h"! Default value will be used: WYZ_3CH
	#define WYZ_CHANNELS				WYZ_3CH
#endif

// WYZ_USE_DIRECT_ACCESS
#if !defined(WYZ_USE_DIRECT_ACCESS)
	#warning WYZ_USE_DIRECT_ACCESS is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define WYZ_USE_DIRECT_ACCESS		FALSE
#endif

// WYZ_CHAN_BUFFER_SIZE
#if !defined(WYZ_CHAN_BUFFER_SIZE)
	#warning WYZ_CHAN_BUFFER_SIZE is not defined in "msxgl_config.h"! Default value will be used: 32
	#define WYZ_CHAN_BUFFER_SIZE		32
#endif

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

extern u16 g_WYZ_SongTable;
extern u16 g_WYZ_InstrumentTable;
extern u16 g_WYZ_SoundTable;
extern u16 g_WYZ_NoteTable;

extern u8  g_WYZ_State; // Player state
// 7 6 5 4 3 2 1 0
//       │ │ │ │ └── Load song
//       │ │ │ └──── Playing
//       │ │ └────── Effect
//       │ └──────── SFX
//       └────────── Loop
extern u8  g_WYZ_Music; // Current music number
extern u8  g_WYZ_Tempo; // Tempo
extern u8  g_WYZ_Header; // Music data header
// 7 6 5 4 3 2 1 0
//   │     │ │ │ └── Loop ON/OFF
//   │     │ │ └──── FX on channel A
//   │     │ └────── FX on channel B
//   │     └──────── FX on channel C
//   └────────────── Frequency (0:50 Hz, 1:60 Hz)


#if (WYZ_USE_DIRECT_ACCESS)
	extern u8 PSG_REG_INT[14];
	extern u8 PSG_REG_EXT[14];
	#define AYREGS  PSG_REG_INT
	#define AYREGS2 PSG_REG_EXT
#else
	#define AYREGS  g_PSG_Regs
	#define AYREGS2 g_PSG2_Regs
#endif

//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

void WYZ_InitPlayer();

// Function: WYZ_Initialize
// Initialize the Player
//
// Parameters:
//   song - Songs data index memory address
//   inst - Instruments data index memory address
//   fx   - FXs data index memory address
//   freq - Notes Table memory address
inline void WYZ_Initialize(u16 song, u16 inst, u16 fx, u16 freq)
{
	g_WYZ_SongTable       = song;
	g_WYZ_InstrumentTable = inst;
	g_WYZ_SoundTable      = fx;
	g_WYZ_NoteTable       = freq;
	
	WYZ_InitPlayer();
}

// Function: WYZ_Play
// Start music playback
//
// Parameters:
//   music - Music number
void WYZ_Play(u8 music) __naked; 

// Function: WYZ_Stop
// Stop song playback 
void WYZ_Stop(); 

// Function: WYZ_Decode
// Process the next step in the song sequence 
void WYZ_Decode() __naked; 
