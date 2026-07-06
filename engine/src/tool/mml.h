// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄           ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ▄█▀▄ ▄█▀▄ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │   ██  ▀█▄▀ ▀█▄▀ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// MML (Music Macro Language) data and routines
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

// MML command format:
//  [BBB|TTTTT] [TTTTTTTT]				Frame data (number of bytes for the frame and frame duration in ticks)
//  [00|..PPPP] [PPPPPPPP]				Pitch Setting (0-4095)		"A"-"G", "N"
//  [1000|VVVV]							Audio volume (0-15)			"V"
//  [1001|SSSS]							Envelop shape (0-15) 		"S"
//  [01000000|] [PPPPPPPP] [PPPPPPPP]	Envelop period				"M"
//  [1101|SSSS] [PPPPPPPP] [PPPPPPPP]	Envelop shape + period	
//  [11111111|]							End of music queue

// Command ID
#define MML_CMD_PITCH				0x00
#define MML_CMD_VOL					0x80
#define MML_CMD_EVSHAPE				0x90
#define MML_CMD_EVPERIOD			0x40
#define MML_CMD_ENVELOP				0xD0
#define MML_CMD_END					0xFF

// Macro commands
#define MML_DUMMY()					0
#define MML_FRAME(b, t)				(((b) << 5) | (((t) >> 8) & 0x1F)), ((t) & 0xFF)
#define MML_PITCH(p)				(((p) >> 8) & 0x0F), ((p) & 0xFF)
#define MML_VOL(v)					(((v) & 0x0F) | MML_CMD_VOL)
#define MML_EVSHAPE(s)				(((s) & 0x0F) | MML_CMD_EVSHAPE)
#define MML_EVPERIOD(p)				MML_CMD_EVPERIOD, ((p) >> 8), ((p) & 0xFF)
#define MML_ENVELOP(s, p)			(((s) & 0x0F) | MML_CMD_ENVELOP), ((p) >> 8), ((p) & 0xFF)
#define MML_END()					MML_CMD_END


//=============================================================================
// FUNCTIONS
//=============================================================================
