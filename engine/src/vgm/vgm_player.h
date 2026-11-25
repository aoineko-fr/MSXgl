// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄  ▄▄▄  ▄ ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██   ██▀█
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▀  ▀█▄█ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// VGM replayer
//  https://vgmrips.net/wiki/VGM_Specification
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "vgm.h"
#include "psg.h"
#include "scc.h"

//-----------------------------------------------------------------------------
// CHECK OPTIONS
//-----------------------------------------------------------------------------

// VGM_USE_PAUSE
#ifndef VGM_USE_PAUSE
	#warning VGM_USE_PAUSE is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define VGM_USE_PAUSE			TRUE
#endif

//=============================================================================
// DEFINES
//=============================================================================

// VGM playback state flag
enum VGM_STATE
{
	VGM_STATE_50HZ = 0b00000001,
	VGM_STATE_LOOP = 0b00000010,
	VGM_STATE_OPL2 = 0b00001000, // OPL2 song played on OPL4 (register values need to be adapted)
	VGM_STATE_STOP = 0b01000000, // Playback stopped (resume restarts from a clean state)
	VGM_STATE_PLAY = 0b10000000,
};

#define VGM_WAIT_50HZ	882 // 50 Hz
#define VGM_WAIT_60HZ	735 // 60 Hz

// VGM chips flag (same values than LVGM_CHIP)
enum VGM_CHIP
{
	VGM_CHIP_PSG      = 0b00000001, // AY-3-8910
	VGM_CHIP_MSXMUSIC = 0b00000010, // YM2413/OPLL
	VGM_CHIP_MSXAUDIO = 0b00000100, // Y8950/OPL1
	VGM_CHIP_SCC      = 0b00001000, // K051649
	VGM_CHIP_SCCI     = 0b00010000, // K052539
	VGM_CHIP_PSG2     = 0b00100000, // 2nd AY-3-8910
	VGM_CHIP_OPL4     = 0b01000000, // YMF278B (also OPL2/OPL3 data)
	VGM_CHIP_OPNA     = 0b10000000, // YM2608 (also YM2203 data)
};

// VGM work variables
extern const VGM_Header* g_VGM_Header;
extern const u8* g_VGM_Pointer;
extern const u8* g_VGM_Loop;
extern u16       g_VGM_WaitCount;
extern u16       g_VGM_WaitFrame;
extern u8        g_VGM_State;

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: VGM_Initialize
// Initialize VGM replayer
void VGM_Initialize();

// Function: VGM_Play
// Start music playback
//
// Parameters:
//   addr - Address of source data
//   loop - If set to TRUE, playback restart when end is reached 
//
// Return:
//    TRUE if initialization succeed
bool VGM_Play(const void* addr, bool loop);

// Function: VGM_Stop
// Stop music playback
void VGM_Stop();

// Function: VGM_SetFrequency50Hz
// Change frequency to 50 Hz
inline void VGM_SetFrequency50Hz() { g_VGM_State |= VGM_STATE_50HZ; g_VGM_WaitFrame = VGM_WAIT_50HZ; }

// Function: VGM_SetFrequency60Hz
// Change frequency to 60 Hz
inline void VGM_SetFrequency60Hz() { g_VGM_State &= ~VGM_STATE_50HZ; g_VGM_WaitFrame = VGM_WAIT_60HZ; }

// Function: VGM_IsPlaying
// Check if music playing
//
// Return:
//   TRUE if music is playing
inline bool VGM_IsPlaying() { return g_VGM_State & VGM_STATE_PLAY; }

// Function: VGM_Mute
// Mute all sound chips (playback state is not changed)
void VGM_Mute();

#if (VGM_USE_PAUSE)
// Function: VGM_Pause
// Pause music playback
// Note: VGM_USE_PAUSE need to be set to TRUE to use this function
void VGM_Pause();
#endif

// Function: VGM_Resume
// Resume music playback (after a stop, playback restarts from the beginning without restoring the chips state)
void VGM_Resume();

// Function: VGM_Decode
// Decode a frame of music
void VGM_Decode();

// Function: VGM_ContainsPSG
// Check if initialized song contains PSG/AY-3-8910 data
//
// Return:
//   FALSE if music dont contains PSG/AY-3-8910 data
inline bool VGM_ContainsPSG() { return (g_VGM_Header->Version >= 0x0151) && g_VGM_Header->AY8910_clock; }

// Function: VGM_ContainsMSXMusic
// Check if initialized song contains MSX-Music/OPLL/YM2413 data
//
// Return:
//   FALSE if music dont contains MSX-Music/OPLL/YM2413 data
inline bool VGM_ContainsMSXMusic() { return g_VGM_Header->YM2413_clock; }

// Function: VGM_ContainsMSXAudio
// Check if initialized song contains MSX-Audio/Y8950 data
//
// Return:
//   FALSE if music dont contains MSX-Audio/Y8950 data
inline bool VGM_ContainsMSXAudio() { return (g_VGM_Header->Version >= 0x0151) && g_VGM_Header->Y8950_clock; }

// Function: VGM_ContainsSCC
// Check if initialized song contains SCC/K051649 data
//
// Return:
//   FALSE if music dont contains SCC/K051649 data
inline bool VGM_ContainsSCC() { return (g_VGM_Header->Version >= 0x0161) && g_VGM_Header->K051649_clock  && ((g_VGM_Header->K051649_clock & 0x80000000) == 0); }

// Function: VGM_ContainsSCCI
// Check if initialized song contains SCC+/K052539 data
//
// Return:
//   FALSE if music dont contains SCC+/K052539 data
inline bool VGM_ContainsSCCI() { return (g_VGM_Header->Version >= 0x0161) && g_VGM_Header->K051649_clock  && ((g_VGM_Header->K051649_clock & 0x80000000) != 0); }

// Function: VGM_ContainsOPL2
// Check if initialized song contains OPL2/YM3812 data
//
// Return:
//   FALSE if music dont contains OPL2/YM3812 data
inline bool VGM_ContainsOPL2() { return (g_VGM_Header->Version >= 0x0151) && g_VGM_Header->YM3812_clock; }

// Function: VGM_ContainsOPL3
// Check if initialized song contains OPL3/YMF262 data
//
// Return:
//   FALSE if music dont contains OPL3/YMF262 data
inline bool VGM_ContainsOPL3() { return (g_VGM_Header->Version >= 0x0151) && g_VGM_Header->YMF262_clock; }

// Function: VGM_ContainsOPL4
// Check if initialized song contains OPL4/YMF278B data
//
// Return:
//   FALSE if music dont contains OPL4/YMF278B data
inline bool VGM_ContainsOPL4() { return (g_VGM_Header->Version >= 0x0151) && g_VGM_Header->YMF278B_clock; }

// Function: VGM_ContainsOPN
// Check if initialized song contains OPN/YM2203 data
//
// Return:
//   FALSE if music dont contains OPN/YM2203 data
inline bool VGM_ContainsOPN() { return (g_VGM_Header->Version >= 0x0151) && g_VGM_Header->YM2203_clock; }

// Function: VGM_ContainsOPNA
// Check if initialized song contains OPNA/YM2608 data
//
// Return:
//   FALSE if music dont contains OPNA/YM2608 data
inline bool VGM_ContainsOPNA() { return (g_VGM_Header->Version >= 0x0151) && g_VGM_Header->YM2608_clock; }

// Function: VGM_GetFileChips
// Get chips list of a VGM data without playing it
//
// Parameters:
//   addr - Address of the VGM data
//
// Return:
//   Chip list (see <VGM_CHIP>)
inline u8 VGM_GetFileChips(const void* addr)
{
	const VGM_Header* head = (const VGM_Header*)addr;
	u8 chip = 0;
	if (head->YM2413_clock)
		chip |= VGM_CHIP_MSXMUSIC;
	if (head->Version >= 0x0151)
	{
		if (head->AY8910_clock)
			chip |= (head->AY8910_clock & 0x40000000) ? (VGM_CHIP_PSG | VGM_CHIP_PSG2) : VGM_CHIP_PSG; // Bit 30: dual chip
		if (head->Y8950_clock)
			chip |= VGM_CHIP_MSXAUDIO;
		if (head->YMF278B_clock || head->YM3812_clock || head->YMF262_clock)
			chip |= VGM_CHIP_OPL4;
		if (head->YM2608_clock || head->YM2203_clock)
			chip |= VGM_CHIP_OPNA;
	}
	if ((head->Version >= 0x0161) && head->K051649_clock)
		chip |= (head->K051649_clock & 0x80000000) ? VGM_CHIP_SCCI : VGM_CHIP_SCC; // Bit 31: K052539
	return chip;
}
