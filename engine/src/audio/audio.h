// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄         ▄▄ ▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄█ ██ █  ▄██ ▄  ▄█▀▄
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀█▄█ ▀▄██ ██ ▀█▄▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

enum CHANNEL_TYPE
{
	CHANNEL_TYPE_TONE = 0,  // Sound generator tone (PSG or SSG)
	CHANNEL_TYPE_WAVETABLE, // Wave table (SCC)
	CHANNEL_TYPE_FM_INST,   // FM instrument
	CHANNEL_TYPE_FM_RHYTHM, // FM rhythm
	CHANNEL_TYPE_SAMPLE,    // Sample (Wave, PCM, ADPCM)
//.....................................
	CHANNEL_TYPE_MAX,
};

// Channel state
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x	x	T	K	V3	V2	V1	V0
//	        │   │   └───┴───┴───┴── Volume (0: mute, 15: max)
//	 		│	└────────────────── Key on (or channel enabled)
//	 		└────────────────────── Keyed on since the last volume read
#define CHANNEL_STATE_VOL			0x0F
#define CHANNEL_STATE_KEY			0x10
#define CHANNEL_STATE_TRIG			0x20