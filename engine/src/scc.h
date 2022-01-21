// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄▄  ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ▀█▄  ██  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ▄▄█▀ ▀█▄█
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// SCC handler module
//
// References:
//  - Konami SCC Sound Chip
//     http://bifi.msxnet.org/msxnet/tech/scc.html
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

#define SCC_SLOT_PROGRAM	0	// Program on a SCC cartridge
#define SCC_SLOT_FIXED		1	// Fixed slot-id (non-expanded second cartridge slot)
#define SCC_SLOT_USER		2	// Defined by the user
#define SCC_SLOT_AUTO		3	// First auto-detected cartridge
//-------------------------------------
#define SCC_SLOT_MODE		SCC_SLOT_FIXED


#if (SCC_SLOT_MODE == SCC_SLOT_FIXED)
	#define SCC_SLOT SLOT_2 // Non-expanded second cartridge slot 
#else
	extern u8 g_SCC_SlotId;
	#define SCC_SLOT g_SCC_SlotId
#endif

#define SCC_SLOT_NOTFOUND 0x02

// Read / write - 32 bytes
#define SCC_WAVE_1	0x00 // Waveform channel 1
#define SCC_WAVE_2	0x20 // Waveform channel 2
#define SCC_WAVE_3	0x40 // Waveform channel 3
#define SCC_WAVE_4	0x60 // Waveform channel 4 (and channel 5 for SCC)
#define SCC_WAVE_5	0xA0 // Waveform channel 5 (SCC-I only)

// Write only - 16 bits
#define SCC_FREQ_1	0x80 // Frequency channel 1
#define SCC_FREQ_2	0x82 // Frequency channel 2
#define SCC_FREQ_3	0x84 // Frequency channel 3
#define SCC_FREQ_4	0x86 // Frequency channel 4
#define SCC_FREQ_5	0x88 // Frequency channel 5
//	7	6	5	4	3	2	1	0	
//	F7	F6	F5	F4	F3	F2	F1	F0
//	└───┴───┴───┴───┴───┴───┴───┴── Frequency, LSB (fine tune)
//	7	6	5	4	3	2	1	0	
//	x	x	x	x	FB	FA	F9	F8 	
//	                └───┴───┴───┴── Frequency, MSB (coarse tune)

// Write only - 8 bits
#define SCC_VOL_1	0x8A // Volume channel 1
#define SCC_VOL_2	0x8B // Volume channel 2
#define SCC_VOL_3	0x8C // Volume channel 3
#define SCC_VOL_4	0x8D // Volume channel 4
#define SCC_VOL_5	0x8E // Volume channel 5
//	7	6	5	4	3	2	1	0	
//	x	x	x	x	V3	V2	V1	V0
//	                └───┴───┴───┴── Amplitude

// Write only - 8 bits
#define SCC_MIXER	0x8F // On/off switch channel 1 to 5
//	7	6	5	4	3	2	1	0	
//	x	x	x	A5	A4	A3	A2	A1
//				│	│	│	│	└── Activate channel 1
//				│	│	│	└────── Activate channel 2
//				│	│	└────────── Activate channel 3
//				│	└────────────── Activate channel 4
//				└────────────────── Activate channel 5

// Write only - 8 bits
#define SCC_TEST	0xC0 // Test register

#define SCC_ADDR_WAVE_1		(0x9800 + SCC_WAVE_1)	 // Waveform channel 1
#define SCC_ADDR_WAVE_2		(0x9800 + SCC_WAVE_2)	 // Waveform channel 2
#define SCC_ADDR_WAVE_3		(0x9800 + SCC_WAVE_3)	 // Waveform channel 3
#define SCC_ADDR_WAVE_4		(0x9800 + SCC_WAVE_4)	 // Waveform channel 4 (and channel 5 for SCC)
#define SCC_ADDR_WAVE_5		(0x9800 + SCC_WAVE_5)	 // Waveform channel 5 (SCC-I only)
#define SCC_ADDR_FREQ_1		(0x9800 + SCC_FREQ_1)	 // Frequency channel 1
#define SCC_ADDR_FREQ_2		(0x9800 + SCC_FREQ_2)	 // Frequency channel 2
#define SCC_ADDR_FREQ_3		(0x9800 + SCC_FREQ_3)	 // Frequency channel 3
#define SCC_ADDR_FREQ_4		(0x9800 + SCC_FREQ_4)	 // Frequency channel 4
#define SCC_ADDR_FREQ_5		(0x9800 + SCC_FREQ_5)	 // Frequency channel 5
#define SCC_ADDR_VOL_1		(0x9800 + SCC_VOL_1)	 // Volume channel 1
#define SCC_ADDR_VOL_2		(0x9800 + SCC_VOL_2)	 // Volume channel 2
#define SCC_ADDR_VOL_3		(0x9800 + SCC_VOL_3)	 // Volume channel 3
#define SCC_ADDR_VOL_4		(0x9800 + SCC_VOL_4)	 // Volume channel 4
#define SCC_ADDR_VOL_5		(0x9800 + SCC_VOL_5)	 // Volume channel 5
#define SCC_ADDR_MIXER		(0x9800 + SCC_MIXER)	 // On/off switch channel 1 to 5
#define SCC_ADDR_TEST		(0x9800 + SCC_TEST)		 // Test register

//=============================================================================
// PROTOTYPE
//=============================================================================

// Function: SCC_Initialize
// Initialize SCC module
bool SCC_Initialize();

#if (SCC_SLOT_MODE == SCC_SLOT_USER)
// Function: SCC_SetSlot
// Set SCC slot ID
inline void SCC_SetSlot(u8 slotId) { g_SCC_SlotId = slotId; }
#endif

// Function: SCC_Select
// Select sound chip on SCC cartridge
void SCC_Select();

// Function: SCC_SetRegister
// Set the value of a given register
void SCC_SetRegister(u8 reg, u8 value);

// Function: SCC_GetRegister
// Get the value of a given register
u8 SCC_GetRegister(u8 reg);

// Function: SCC_Mute
// Mute the SCC sound chip
void SCC_Mute();
