// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄      ▄▄        ▄▄       
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▄▀██ ██▄▀ ▄█▀▄ ██▀ ▄█▀▄ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀▄██ ██ █ ▀█▄▀ ▀█▄ ▀█▄▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                           
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Makoto (YM2608 - OPNA) handler module
//
// References:
//  - YM2608 (OPNA) Application Manual
//     https://theretroweb.com/chip/documentation/ym2608-applicatin-manual-jp-67b9ef528ddcb605215687.pdf
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"
#include "system_port.h"
#include "audio/makoto_reg.h"

//=============================================================================
// DEFINES
//=============================================================================

// SSG channels
#define MAKOTO_SSG_CHANNEL_A		0
#define MAKOTO_SSG_CHANNEL_B		1
#define MAKOTO_SSG_CHANNEL_C		2

// Rythm instruments
#define MAKOTO_RHY_BASS_DRUM		0
#define MAKOTO_RHY_SNARE_DRUM		1
#define MAKOTO_RHY_TOP_CYMBAL		2
#define MAKOTO_RHY_HI_HAT			3
#define MAKOTO_RHY_TOM_TOM			4
#define MAKOTO_RHY_RIM_SHOT			5

//=============================================================================
// PROTOTYPE
//=============================================================================

// Group: Core

// Function: Makoto_Initialize
// Initialize Makoto module
//
// Return:
//   FALSE if no Makoto (YM2608) chip found
bool Makoto_Initialize();

// Function: Makoto_Detect
// Search for Makoto (YM2608) chip
//
// Return:
//   FALSE if no Makoto (YM2608) chip found
bool Makoto_Detect();

// Function: Makoto_GetStatus
// Set Makoto register value for part 0 (FM CH1–3, SSG, rhythm, timers, 29h)
//
// Return:
//   Status value
//> OPNA_STATUS_FLAG_A			0x01	// Timer A overflow flag
//> OPNA_STATUS_FLAG_B			0x02	// Timer B overflow flag
//> OPNA_STATUS_EOS				0x04	// ADPCM end of sample/transfer
//> OPNA_STATUS_BRDY			0x08	// ADPCM buffer ready
//> OPNA_STATUS_ZERO			0x10	// ADPCM zero level detected
//> OPNA_STATUS_PCMBSY			0x20	// ADPCM playing
//> OPNA_STATUS_BUSY			0x80	// Busy
inline u8 Makoto_GetStatus() { return g_OPNA_StatusPort; }

// Function: Makoto_IsBusy
// Is Makoto busy (registers are not ready to be written)
//
// Return:
//   FALSE if Makoto is ready to be written
inline bool Makoto_IsBusy() { return Makoto_GetStatus() & OPNA_STATUS_BUSY; }

// Function: Makoto_SetRegister0
// Set Makoto register value for part 0 (FM CH1–3, SSG, rhythm, timers, 29h)
//
// Parameters:
//   reg   - Register to set
//   value - Value to set
void Makoto_SetRegister0(u8 reg, u8 value);

// Function: Makoto_GetRegister0
// Get Makoto register value for part 0 (FM CH1–3, SSG, rhythm, timers, 29h)
//
// Parameters:
//   reg - Register to get
//
// Return:
//   Value of the register
u8 Makoto_GetRegister0(u8 reg);

// Function: Makoto_SetRegister1
// Set Makoto register value for part 1 (ADPCM-B, FM CH4–6)
//
// Parameters:
//   reg   - Register to set
//   value - Value to set
void Makoto_SetRegister1(u8 reg, u8 value);

// Function: Makoto_GetRegister1
// Get Makoto register value for part 1 (ADPCM-B, FM CH4–6)
//
// Parameters:
//   reg - Register to get
//
// Return:
//   Value of the register
u8 Makoto_GetRegister1(u8 reg);

// Function: Makoto_SetRegister
// Set Makoto register value
//
// Parameters:
//   reg   - Register to set
//   value - Value to set
inline void Makoto_SetRegister(u16 reg, u8 value) { if (reg < 256) Makoto_SetRegister0((u8)reg, value); else Makoto_SetRegister1((u8)(reg - 256), value); }

// Function: Makoto_GetRegister
// Get Makoto register value
//
// Parameters:
//   reg - Register to get
//
// Return:
//   Value of the register
inline u8 Makoto_GetRegister(u16 reg) { return (reg < 256) ? Makoto_GetRegister0((u8)reg) : Makoto_GetRegister1((u8)(reg - 256)); }

// Function: Makoto_Mute
// Mute Makoto sound
void Makoto_Mute();

#if (MAKOTO_USE_RESUME)
// Function: Makoto_Resume
// Resume Makoto sound
void Makoto_Resume();
#endif

// Group: SSG

// Function: Makoto_SSGSetTone
// Set the tone period of a given channel (tone generator control register)
//
// Parameters:
//   chan   - Channel to set (0 to 2; or MAKOTO_SSG_CHANNEL_A, MAKOTO_SSG_CHANNEL_B, MAKOTO_SSG_CHANNEL_C)
//   period - Period to set (12-bits value)
inline void Makoto_SSGSetTone(u8 chan, u16 period) { Makoto_SetRegister0(OPNA_REG_SSG_TONE_A_L + (chan * 2), (u8)period); Makoto_SetRegister0(OPNA_REG_SSG_TONE_A_H + (chan * 2), (u8)(period >> 8)); }

// Function: Makoto_SSGSetNoise
// Set the noise period (noise generator control register)
//
// Parameters:
//   period - Period to set (5-bits value)
inline void Makoto_SSGSetNoise(u8 period) { Makoto_SetRegister0(OPNA_REG_SSG_NOISE, period); }

// Function: Makoto_SSGSetMixer
// Setup mixer by enabling tune and noise generators for each channel (mixer control enable register)
//
// Parameters:
//   mix - Mixer value to set
inline void Makoto_SSGSetMixer(u8 mix) { Makoto_SetRegister0(OPNA_REG_SSG_MIXER, mix); }

// Function: Makoto_SSGSetVolume
// Set the volume of a given channel (Amplitude control register)
//
// Parameters:
//   chan - Channel to set (0 to 2; or MAKOTO_SSG_CHANNEL_A, MAKOTO_SSG_CHANNEL_B, MAKOTO_SSG_CHANNEL_C)
//   vol  - Volume to set (4-bits value)
inline void Makoto_SSGSetVolume(u8 chan, u8 vol) { Makoto_SetRegister0(OPNA_REG_SSG_AMP_A + chan, vol); }

// Function: Makoto_SSGSetEnvelopePeriod
// Set the envelope period (Envelope priod control register)
//
// Parameters:
//   period - Period to set (16-bits value)
inline void Makoto_SSGSetEnvelopePeriod(u16 period) { Makoto_SetRegister0(OPNA_REG_SSG_ENV_L, (u8)period); Makoto_SetRegister0(OPNA_REG_SSG_ENV_H, (u8)(period >> 8)); }

// Function: Makoto_SSGSetEnvelopeShape
// Set the envelope shape (Envelope shape control register)
//
// Parameters:
//   shape - Shape to set (4-bits value)
inline void Makoto_SSGSetEnvelopeShape(u8 shape) { Makoto_SetRegister0(OPNA_REG_SSG_SHAPE, shape); }

// Group: Rythm

// Function: Makoto_RythmSetKey
// Set the rythm key (Rythm key on/off register)
//
// Parameters:
//   key - Key to set (6-bits value)
//> OPNA_F_RHY_BD		Bass Drum
//> OPNA_F_RHY_SD		Snare Drum
//> OPNA_F_RHY_TOP		Top Cymbal
//> OPNA_F_RHY_HH		Hi-Hat
//> OPNA_F_RHY_TOM		Tom-tom
//> OPNA_F_RHY_RIM		Rim Shot
//> OPNA_F_RHY_ALL		All instruments
//> OPNA_F_RHY_DUMP		Dump (stop selected instruments)
inline void Makoto_RythmSetKey(u8 key) { Makoto_SetRegister0(OPNA_REG_RHY_KEY, key); }

// Function: Makoto_RythmSetMainVolume
// Set the main volume of the rythm instruments (Rythm total level register)
//
// Parameters:
//   vol - Rhythm main level (0-63, 63 = loudest, 0.75 dB/step)
inline void Makoto_RythmSetMainVolume(u8 vol) { Makoto_SetRegister0(OPNA_REG_RHY_TL, vol); }

// Function: Makoto_RythmSetInstVolume
// Set the volume of a given rythm instrument (Rythm output/level register)
//
// Parameters:
//   rtyhm - Rythm instrument to set (0 to 5; or MAKOTO_RHY_BASS_DRUM, MAKOTO_RHY_SNARE_DRUM, MAKOTO_RHY_TOP_CYMBAL, MAKOTO_RHY_HI_HAT, MAKOTO_RHY_TOM_TOM, MAKOTO_RHY_RIM_SHOT)
//   vol   - Instrument level (0-31, 31 = loudest, 0.75 dB/step)
//   pan   - Pan to set
//> OPNA_F_R				Right output enable
//> OPNA_F_L				Left output enable
//> OPNA_F_LR				Both outputs enable
inline void Makoto_RythmSetInstVolume(u8 rtyhm, u8 vol, u8 pan) { Makoto_SetRegister0(OPNA_REG_RHY_BD + rtyhm, (vol & 0x0F) | pan); }