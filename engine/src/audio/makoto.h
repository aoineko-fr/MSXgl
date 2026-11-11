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

//-----------------------------------------------------------------------------
// CHECK OPTIONS
//-----------------------------------------------------------------------------

// MAKOTO_USE_RESUME
#ifndef MAKOTO_USE_RESUME
	#warning MAKOTO_USE_RESUME is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define MAKOTO_USE_RESUME		TRUE
#endif

// MAKOTO_USE_ADPCM
#ifndef MAKOTO_USE_ADPCM
	#warning MAKOTO_USE_ADPCM is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define MAKOTO_USE_ADPCM		TRUE
#endif

//=============================================================================
// DEFINES
//=============================================================================

// Makoto (OPNA/YM2608) I/O ports
#define P_OPNA_ADDR0				0x14 // Array 0 address (FM CH1–3, SSG, rhythm, timers, 29h)
__sfr __at(P_OPNA_ADDR0)			io_OPNA_Reg0;
#define P_OPNA_DATA0				0x15 // Array 0 data
__sfr __at(P_OPNA_DATA0)			io_OPNA_Data0;
#define P_OPNA_ADDR1				0x16 // Array 1 address (ADPCM-B, FM CH4–6)
__sfr __at(P_OPNA_ADDR1)			io_OPNA_Reg1;
#define P_OPNA_DATA1				0x17 // Array 1 data
__sfr __at(P_OPNA_DATA1)			io_OPNA_Data1;
#define P_OPNA_STATUS				0x16 // Status (BUSY, timer flags and ADPCM flags)
const __sfr __at(P_OPNA_STATUS)		io_OPNA_Stat;

// SSG channels
#define MAKOTO_SSG_CHANNEL_A		0
#define MAKOTO_SSG_CHANNEL_B		1
#define MAKOTO_SSG_CHANNEL_C		2

// Rythm instrument ID
#define MAKOTO_RHY_BASS_DRUM		0
#define MAKOTO_RHY_SNARE_DRUM		1
#define MAKOTO_RHY_TOP_CYMBAL		2
#define MAKOTO_RHY_HI_HAT			3
#define MAKOTO_RHY_TOM_TOM			4
#define MAKOTO_RHY_RIM_SHOT			5

// Delta-N values for each frequency
#define MAKOTO_ADPCM_DELTAN_8KHZ	0x24DE
#define MAKOTO_ADPCM_DELTAN_11KHZ	0x32CE
#define MAKOTO_ADPCM_DELTAN_16KHZ	0x49BA
#define MAKOTO_ADPCM_DELTAN_22KHZ	0x659C
#define MAKOTO_ADPCM_DELTAN_32KHZ	0x9375
#define MAKOTO_ADPCM_DELTAN_44KHZ	0xCB37
#define MAKOTO_ADPCM_DELTAN_48KHZ	0xDD2F
#define MAKOTO_ADPCM_DELTAN_MAX		0xFFFF

//=============================================================================
// PROTOTYPE
//=============================================================================

//.............................................................................
// Group: Core

// Function: Makoto_Initialize
// Initialize Makoto (OPNA/YM2608) module
// Note: Must be called first before any other access to Makoto sound card
//
// Return:
//   FALSE if no Makoto cartridge found
bool Makoto_Initialize();

// Function: Makoto_Detect
// Search for Makoto (OPNA/YM2608) cartridge
//
// Return:
//   FALSE if no Makoto cartridge found
bool Makoto_Detect();

// Function: Makoto_GetStatus
// Set Makoto register value for part 0 (FM CH1–3, SSG, rhythm, timers, 29h)
//
// Return:
//   Status value
//> OPNA_STATUS_FLAG_A		Timer A overflow flag
//> OPNA_STATUS_FLAG_B		Timer B overflow flag
//> OPNA_STATUS_EOS			ADPCM end of sample/transfer
//> OPNA_STATUS_BRDY		ADPCM buffer ready
//> OPNA_STATUS_ZERO		ADPCM zero level detected
//> OPNA_STATUS_PCMBSY		ADPCM playing
//> OPNA_STATUS_BUSY		Busy
inline u8 Makoto_GetStatus() { return io_OPNA_Stat; }

// Function: Makoto_IsBusy
// Is Makoto busy (registers are not ready to be written)
//
// Return:
//   FALSE if Makoto is ready to be written
inline bool Makoto_IsBusy() { return Makoto_GetStatus() & OPNA_STATUS_BUSY; }

// Function: Makoto_WaitBusy
// Wait while Makoto is busy (256 tries max to avoid infinite loop when no chip is present)
inline void Makoto_WaitBusy() { u8 timeout = 0; while (Makoto_IsBusy() && --timeout) {} }

// Function: Makoto_SetRegister0
// Set Makoto register value for part 0 (FM CH1–3, SSG, rhythm, timers, 29h)
// Note: Writing R#16 (rhythm key) needs 576 φM cycles before next write; use <Makoto_RythmSetKey> or call <Makoto_WaitBusy> after
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

//.............................................................................
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

//.............................................................................
// Group: FM

/* ... */

//.............................................................................
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
inline void Makoto_RythmSetKey(u8 key) { Makoto_SetRegister0(OPNA_REG_RHY_KEY, key); Makoto_WaitBusy(); }

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
inline void Makoto_RythmSetInstVolume(u8 rtyhm, u8 vol, u8 pan) { Makoto_SetRegister0(OPNA_REG_RHY_BD + rtyhm, (vol & 0x1F) | pan); }

//.............................................................................
// Group: ADPCM

#if (MAKOTO_USE_ADPCM)

// Function: Makoto_ADPCMLoad
// Write ADPCM data into the cartridge sample RAM
// Addresses are in memory units of 4 bytes (x1-bit DRAM), so 16-bit covers the 256 KB of the Makoto
// Note: MAKOTO_USE_ADPCM need to be set to TRUE to use this function
// Note: ADPCM playback is stopped, and no other Makoto access (e.g. music replayer in interrupt) must occur during the load
//
// Parameters:
//   data - Pointer to the ADPCM data to load
//   addr - Start address in RAM (in 4 bytes unit)
//   size - Data size (in 4 bytes unit)
void Makoto_ADPCMLoad(const u8* data, u16 addr, u16 size);

// Function: Makoto_ADPCMPlay
// Start the playback of a sample loaded in RAM (see <Makoto_ADPCMLoad>)
// Note: MAKOTO_USE_ADPCM need to be set to TRUE to use this function
//
// Parameters:
//   addr - Start address in RAM (in 4 bytes unit)
//   size - Sample size (in 4 bytes unit)
//   loop - Restart from the start address when reaching the end of the sample
void Makoto_ADPCMPlay(u16 addr, u16 size, bool loop);

// Function: Makoto_ADPCMStop
// Stop the sample playback
// Note: MAKOTO_USE_ADPCM need to be set to TRUE to use this function
inline void Makoto_ADPCMStop() { Makoto_SetRegister1(OPNA_REG_ADPCM_CTRL1, OPNA_F_ADPCM_RESET); Makoto_SetRegister1(OPNA_REG_ADPCM_CTRL1, 0); }

// Function: Makoto_ADPCMIsPlaying
// Check if an ADPCM synthesis is running
// Note: MAKOTO_USE_ADPCM need to be set to TRUE to use this function
//
// Return:
//   TRUE if a sample is playing
inline bool Makoto_ADPCMIsPlaying() { return Makoto_GetStatus() & OPNA_STATUS_PCMBSY; }

// Function: Makoto_ADPCMSetDeltaN
// Set the playback rate/pitch of the sample (Delta-N)
// Note: MAKOTO_USE_ADPCM need to be set to TRUE to use this function
//
// Parameters:
//   delta - Delta-N value
//> MAKOTO_ADPCM_DELTAN_8KHZ    8 kHz         
//> MAKOTO_ADPCM_DELTAN_11KHZ)  11.025 kHz    
//> MAKOTO_ADPCM_DELTAN_16KHZ)  16 kHz        
//> MAKOTO_ADPCM_DELTAN_22KHZ)  22.05 kHz     
//> MAKOTO_ADPCM_DELTAN_32KHZ)  32 kHz        
//> MAKOTO_ADPCM_DELTAN_44KHZ)  44.1 kHz      
//> MAKOTO_ADPCM_DELTAN_48KHZ)  48 kHz        
//> MAKOTO_ADPCM_DELTAN_MAX)    55.5 kHz (max)
inline void Makoto_ADPCMSetDeltaN(u16 delta) { Makoto_SetRegister1(OPNA_REG_ADPCM_DELTA_L, (u8)delta); Makoto_SetRegister1(OPNA_REG_ADPCM_DELTA_H, (u8)(delta >> 8)); }

// Function: Makoto_ADPCMSetFrequency
// Set the playback rate of the sample from a frequency index
// Note: MAKOTO_USE_ADPCM need to be set to TRUE to use this function
//
// Parameters:
//   freq - Playback frequency
//> 8		8 kHz
//> 11		11.025 kHz
//> 16		16 kHz
//> 22		22.05 kHz
//> 32		32 kHz
//> 44		44.1 kHz
//> 48		48 kHz
//> 55		55.5 kHz (max)
inline void Makoto_ADPCMSetFrequency(u8 freq)
{
	switch (freq)
	{
	case 8:  Makoto_ADPCMSetDeltaN(MAKOTO_ADPCM_DELTAN_8KHZ);  break;
	case 11: Makoto_ADPCMSetDeltaN(MAKOTO_ADPCM_DELTAN_11KHZ); break;
	case 16: Makoto_ADPCMSetDeltaN(MAKOTO_ADPCM_DELTAN_16KHZ); break;
	case 22: Makoto_ADPCMSetDeltaN(MAKOTO_ADPCM_DELTAN_22KHZ); break;
	case 32: Makoto_ADPCMSetDeltaN(MAKOTO_ADPCM_DELTAN_32KHZ); break;
	case 44: Makoto_ADPCMSetDeltaN(MAKOTO_ADPCM_DELTAN_44KHZ); break;
	case 48: Makoto_ADPCMSetDeltaN(MAKOTO_ADPCM_DELTAN_48KHZ); break;
	case 55: Makoto_ADPCMSetDeltaN(MAKOTO_ADPCM_DELTAN_MAX);   break;
	}
}

// Function: Makoto_ADPCMSetVolume
// Set output level of ADPCM synthesis
// Note: MAKOTO_USE_ADPCM need to be set to TRUE to use this function
//
// Parameters:
//   vol - Output volume (0 = silent, 255 = max)
inline void Makoto_ADPCMSetVolume(u8 vol) { Makoto_SetRegister1(OPNA_REG_ADPCM_LEVEL, vol); }

// Function: Makoto_ADPCMSetPan
// Set the output channels of the ADPCM synthesis (also set memory type to x1-bit DRAM)
// Note: MAKOTO_USE_ADPCM need to be set to TRUE to use this function
//
// Parameters:
//   pan - Pan to set
//> OPNA_F_R				Right output enable
//> OPNA_F_L				Left output enable
//> OPNA_F_LR				Both outputs enable
inline void Makoto_ADPCMSetPan(u8 pan) { Makoto_SetRegister1(OPNA_REG_ADPCM_CTRL2, pan); }

#endif // (MAKOTO_USE_ADPCM)
