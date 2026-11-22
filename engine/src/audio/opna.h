// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄      ▄▄        ▄▄       
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▄▀██ ██▄▀ ▄█▀▄ ██▀ ▄█▀▄ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀▄██ ██ █ ▀█▄▀ ▀█▄ ▀█▄▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                           
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// OPNA (YM2608 - OPNA) handler module
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
#include "audio/opna_reg.h"
#include "audio/audio.h"

//-----------------------------------------------------------------------------
// CHECK OPTIONS
//-----------------------------------------------------------------------------

// OPNA_USE_RESUME
#ifndef OPNA_USE_RESUME
	#warning OPNA_USE_RESUME is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define OPNA_USE_RESUME		TRUE
#endif

// OPNA_USE_ADPCM
#ifndef OPNA_USE_ADPCM
	#warning OPNA_USE_ADPCM is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define OPNA_USE_ADPCM		TRUE
#endif

// OPNA_USE_CHANNEL
#ifndef OPNA_USE_CHANNEL
	#warning OPNA_USE_CHANNEL is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define OPNA_USE_CHANNEL	TRUE
#endif

//=============================================================================
// DEFINES
//=============================================================================

// OPNA (OPNA/YM2608) I/O ports
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
#define OPNA_SSG_CHANNEL_A		0
#define OPNA_SSG_CHANNEL_B		1
#define OPNA_SSG_CHANNEL_C		2

// Rhythm instrument ID
#define OPNA_RHY_BASS_DRUM		0
#define OPNA_RHY_SNARE_DRUM		1
#define OPNA_RHY_TOP_CYMBAL		2
#define OPNA_RHY_HI_HAT			3
#define OPNA_RHY_TOM_TOM			4
#define OPNA_RHY_RIM_SHOT			5

// Delta-N values for each frequency
#define OPNA_ADPCM_DELTAN_8KHZ	0x24DE
#define OPNA_ADPCM_DELTAN_11KHZ	0x32CE
#define OPNA_ADPCM_DELTAN_16KHZ	0x49BA
#define OPNA_ADPCM_DELTAN_22KHZ	0x659C
#define OPNA_ADPCM_DELTAN_32KHZ	0x9375
#define OPNA_ADPCM_DELTAN_44KHZ	0xCB37
#define OPNA_ADPCM_DELTAN_48KHZ	0xDD2F
#define OPNA_ADPCM_DELTAN_MAX		0xFFFF

//=============================================================================
// PROTOTYPE
//=============================================================================

//.............................................................................
// Group: Core

// Function: OPNA_Initialize
// Initialize OPNA (OPNA/YM2608) module
// Note: Must be called first before any other access to OPNA sound card
//
// Return:
//   FALSE if no OPNA cartridge found
bool OPNA_Initialize();

// Function: OPNA_Detect
// Search for OPNA (OPNA/YM2608) cartridge
//
// Return:
//   FALSE if no OPNA cartridge found
bool OPNA_Detect();

// Function: OPNA_GetStatus
// Get status register value
//
// Return:
//   Status value (combination of following flags)
//> OPNA_STATUS_FLAG_A		Timer A overflow flag
//> OPNA_STATUS_FLAG_B		Timer B overflow flag
//> OPNA_STATUS_EOS			ADPCM end of sample/transfer
//> OPNA_STATUS_BRDY		ADPCM buffer ready
//> OPNA_STATUS_ZERO		ADPCM zero level detected
//> OPNA_STATUS_PCMBSY		ADPCM playing
//> OPNA_STATUS_BUSY		Busy
inline u8 OPNA_GetStatus() { return io_OPNA_Stat; }

// Function: OPNA_IsBusy
// Is OPNA busy (registers are not ready to be written)
//
// Return:
//   FALSE if OPNA is ready to be written
inline bool OPNA_IsBusy() { return OPNA_GetStatus() & OPNA_STATUS_BUSY; }

// Function: OPNA_WaitBusy
// Wait while OPNA is busy (256 tries max to avoid infinite loop when no chip is present)
inline void OPNA_WaitBusy() { u8 timeout = 0; while (OPNA_IsBusy() && --timeout) {} }

// Function: OPNA_SetPart0Register
// Set OPNA register value for part 0 (FM CH1–3, SSG, rhythm, timers, 29h)
// Note: Writing R#16 (rhythm key) needs 576 φM cycles before next write; use <OPNA_RhythmSetKey> or call <OPNA_WaitBusy> after
//
// Parameters:
//   reg   - Register to set
//   value - Value to set
void OPNA_SetPart0Register(u8 reg, u8 value);

// Function: OPNA_GetPart0Register
// Get OPNA register value for part 0 (FM CH1–3, SSG, rhythm, timers, 29h)
//
// Parameters:
//   reg - Register to get
//
// Return:
//   Value of the register
u8 OPNA_GetPart0Register(u8 reg);

// Function: OPNA_SetPart1Register
// Set OPNA register value for part 1 (ADPCM-B, FM CH4–6)
//
// Parameters:
//   reg   - Register to set
//   value - Value to set
void OPNA_SetPart1Register(u8 reg, u8 value);

// Function: OPNA_GetPart1Register
// Get OPNA register value for part 1 (ADPCM-B, FM CH4–6)
//
// Parameters:
//   reg - Register to get
//
// Return:
//   Value of the register
u8 OPNA_GetPart1Register(u8 reg);

// Function: OPNA_SetRegister
// Set OPNA register value
//
// Parameters:
//   reg   - Register to set
//   value - Value to set
inline void OPNA_SetRegister(u16 reg, u8 value) { if (reg < 256) OPNA_SetPart0Register((u8)reg, value); else OPNA_SetPart1Register((u8)(reg - 256), value); }

// Function: OPNA_GetRegister
// Get OPNA register value
//
// Parameters:
//   reg - Register to get
//
// Return:
//   Value of the register
inline u8 OPNA_GetRegister(u16 reg) { return (reg < 256) ? OPNA_GetPart0Register((u8)reg) : OPNA_GetPart1Register((u8)(reg - 256)); }

// Function: OPNA_Mute
// Mute OPNA sound
void OPNA_Mute();

#if (OPNA_USE_RESUME)
// Function: OPNA_Resume
// Resume OPNA sound
void OPNA_Resume();
#endif

//.............................................................................
// Group: SSG

// Function: OPNA_SSGSetTone
// Set the tone period of a given channel (tone generator control register)
//
// Parameters:
//   chan   - Channel to set (0 to 2; or OPNA_SSG_CHANNEL_A, OPNA_SSG_CHANNEL_B, OPNA_SSG_CHANNEL_C)
//   period - Period to set (12-bits value)
inline void OPNA_SSGSetTone(u8 chan, u16 period) { OPNA_SetPart0Register(OPNA_REG_SSG_TONE_A_L + (chan * 2), (u8)period); OPNA_SetPart0Register(OPNA_REG_SSG_TONE_A_H + (chan * 2), (u8)(period >> 8)); }

// Function: OPNA_SSGSetNoise
// Set the noise period (noise generator control register)
//
// Parameters:
//   period - Period to set (5-bits value)
inline void OPNA_SSGSetNoise(u8 period) { OPNA_SetPart0Register(OPNA_REG_SSG_NOISE, period); }

// Function: OPNA_SSGSetMixer
// Setup mixer by enabling tune and noise generators for each channel (mixer control enable register)
//
// Parameters:
//   mix - Mixer value to set
inline void OPNA_SSGSetMixer(u8 mix) { OPNA_SetPart0Register(OPNA_REG_SSG_MIXER, mix); }

// Function: OPNA_SSGSetVolume
// Set the volume of a given channel (Amplitude control register)
//
// Parameters:
//   chan - Channel to set (0 to 2; or OPNA_SSG_CHANNEL_A, OPNA_SSG_CHANNEL_B, OPNA_SSG_CHANNEL_C)
//   vol  - Volume to set (4-bits value)
inline void OPNA_SSGSetVolume(u8 chan, u8 vol) { OPNA_SetPart0Register(OPNA_REG_SSG_AMP_A + chan, vol); }

// Function: OPNA_SSGSetEnvelopePeriod
// Set the envelope period (Envelope priod control register)
//
// Parameters:
//   period - Period to set (16-bits value)
inline void OPNA_SSGSetEnvelopePeriod(u16 period) { OPNA_SetPart0Register(OPNA_REG_SSG_ENV_L, (u8)period); OPNA_SetPart0Register(OPNA_REG_SSG_ENV_H, (u8)(period >> 8)); }

// Function: OPNA_SSGSetEnvelopeShape
// Set the envelope shape (Envelope shape control register)
//
// Parameters:
//   shape - Shape to set (4-bits value)
inline void OPNA_SSGSetEnvelopeShape(u8 shape) { OPNA_SetPart0Register(OPNA_REG_SSG_SHAPE, shape); }

//.............................................................................
// Group: FM

/* ... */

//.............................................................................
// Group: Rhythm

// Function: OPNA_RhythmSetKey
// Set the rhythm key (Rhythm key on/off register)
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
inline void OPNA_RhythmSetKey(u8 key) { OPNA_SetPart0Register(OPNA_REG_RHY_KEY, key); OPNA_WaitBusy(); }

// Function: OPNA_RhythmSetMainVolume
// Set the main volume of the rhythm instruments (Rhythm total level register)
//
// Parameters:
//   vol - Rhythm main level (0-63, 63 = loudest, 0.75 dB/step)
inline void OPNA_RhythmSetMainVolume(u8 vol) { OPNA_SetPart0Register(OPNA_REG_RHY_TL, vol); }

// Function: OPNA_RhythmSetInstVolume
// Set the volume of a given rhythm instrument (Rhythm output/level register)
//
// Parameters:
//   rtyhm - Rhythm instrument to set (0 to 5; or OPNA_RHY_BASS_DRUM, OPNA_RHY_SNARE_DRUM, OPNA_RHY_TOP_CYMBAL, OPNA_RHY_HI_HAT, OPNA_RHY_TOM_TOM, OPNA_RHY_RIM_SHOT)
//   vol   - Instrument level (0-31, 31 = loudest, 0.75 dB/step)
//   pan   - Pan to set
//> OPNA_F_R				Right output enable
//> OPNA_F_L				Left output enable
//> OPNA_F_LR				Both outputs enable
inline void OPNA_RhythmSetInstVolume(u8 rtyhm, u8 vol, u8 pan) { OPNA_SetPart0Register(OPNA_REG_RHY_BD + rtyhm, (vol & 0x1F) | pan); }

//.............................................................................
// Group: ADPCM

#if (OPNA_USE_ADPCM)

// Function: OPNA_ADPCMLoad
// Write ADPCM data into the cartridge sample RAM
// Addresses are in memory units of 4 bytes (x1-bit DRAM), so 16-bit covers the 256 KB of the OPNA
// Note: OPNA_USE_ADPCM need to be set to TRUE to use this function
// Note: ADPCM playback is stopped, and no other OPNA access (e.g. music replayer in interrupt) must occur during the load
//
// Parameters:
//   data - Pointer to the ADPCM data to load
//   addr - Start address in RAM (in 4 bytes unit)
//   size - Data size (in 4 bytes unit)
void OPNA_ADPCMLoad(const u8* data, u16 addr, u16 size);

// Function: OPNA_ADPCMPlay
// Start the playback of a sample loaded in RAM (see <OPNA_ADPCMLoad>)
// Note: OPNA_USE_ADPCM need to be set to TRUE to use this function
//
// Parameters:
//   addr - Start address in RAM (in 4 bytes unit)
//   size - Sample size (in 4 bytes unit)
//   loop - Restart from the start address when reaching the end of the sample
void OPNA_ADPCMPlay(u16 addr, u16 size, bool loop);

// Function: OPNA_ADPCMStop
// Stop the sample playback
// Note: OPNA_USE_ADPCM need to be set to TRUE to use this function
inline void OPNA_ADPCMStop() { OPNA_SetPart1Register(OPNA_REG_ADPCM_CTRL1, OPNA_F_ADPCM_RESET); OPNA_SetPart1Register(OPNA_REG_ADPCM_CTRL1, 0); }

// Function: OPNA_ADPCMIsPlaying
// Check if an ADPCM synthesis is running
// Note: OPNA_USE_ADPCM need to be set to TRUE to use this function
//
// Return:
//   TRUE if a sample is playing
inline bool OPNA_ADPCMIsPlaying() { return OPNA_GetStatus() & OPNA_STATUS_PCMBSY; }

// Function: OPNA_ADPCMSetDeltaN
// Set the playback rate/pitch of the sample (Delta-N)
// Note: OPNA_USE_ADPCM need to be set to TRUE to use this function
//
// Parameters:
//   delta - Delta-N value
//> OPNA_ADPCM_DELTAN_8KHZ    8 kHz         
//> OPNA_ADPCM_DELTAN_11KHZ)  11.025 kHz    
//> OPNA_ADPCM_DELTAN_16KHZ)  16 kHz        
//> OPNA_ADPCM_DELTAN_22KHZ)  22.05 kHz     
//> OPNA_ADPCM_DELTAN_32KHZ)  32 kHz        
//> OPNA_ADPCM_DELTAN_44KHZ)  44.1 kHz      
//> OPNA_ADPCM_DELTAN_48KHZ)  48 kHz        
//> OPNA_ADPCM_DELTAN_MAX)    55.5 kHz (max)
inline void OPNA_ADPCMSetDeltaN(u16 delta) { OPNA_SetPart1Register(OPNA_REG_ADPCM_DELTA_L, (u8)delta); OPNA_SetPart1Register(OPNA_REG_ADPCM_DELTA_H, (u8)(delta >> 8)); }

// Function: OPNA_ADPCMSetFrequency
// Set the playback rate of the sample from a frequency index
// Note: OPNA_USE_ADPCM need to be set to TRUE to use this function
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
inline void OPNA_ADPCMSetFrequency(u8 freq)
{
	switch (freq)
	{
	case 8:  OPNA_ADPCMSetDeltaN(OPNA_ADPCM_DELTAN_8KHZ);  break;
	case 11: OPNA_ADPCMSetDeltaN(OPNA_ADPCM_DELTAN_11KHZ); break;
	case 16: OPNA_ADPCMSetDeltaN(OPNA_ADPCM_DELTAN_16KHZ); break;
	case 22: OPNA_ADPCMSetDeltaN(OPNA_ADPCM_DELTAN_22KHZ); break;
	case 32: OPNA_ADPCMSetDeltaN(OPNA_ADPCM_DELTAN_32KHZ); break;
	case 44: OPNA_ADPCMSetDeltaN(OPNA_ADPCM_DELTAN_44KHZ); break;
	case 48: OPNA_ADPCMSetDeltaN(OPNA_ADPCM_DELTAN_48KHZ); break;
	case 55: OPNA_ADPCMSetDeltaN(OPNA_ADPCM_DELTAN_MAX);   break;
	}
}

// Function: OPNA_ADPCMSetVolume
// Set output level of ADPCM synthesis
// Note: OPNA_USE_ADPCM need to be set to TRUE to use this function
//
// Parameters:
//   vol - Output volume (0 = silent, 255 = max)
inline void OPNA_ADPCMSetVolume(u8 vol) { OPNA_SetPart1Register(OPNA_REG_ADPCM_LEVEL, vol); }

// Function: OPNA_ADPCMSetPan
// Set the output channels of the ADPCM synthesis (also set memory type to x1-bit DRAM)
// Note: OPNA_USE_ADPCM need to be set to TRUE to use this function
//
// Parameters:
//   pan - Pan to set
//> OPNA_F_R				Right output enable
//> OPNA_F_L				Left output enable
//> OPNA_F_LR				Both outputs enable
inline void OPNA_ADPCMSetPan(u8 pan) { OPNA_SetPart1Register(OPNA_REG_ADPCM_CTRL2, pan); }

#endif // (OPNA_USE_ADPCM)

#if (OPNA_USE_CHANNEL)
// Group: Channel Info

// Function: OPNA_GetChannelCount
// Get the number of OPNA channels: 3 SSG channels, 6 FM channels, 6 rhythm instruments and the ADPCM channel
// Note: OPNA_USE_CHANNEL need to be set to TRUE to use this function
//
// Return:
//   Number of channels
inline u8 OPNA_GetChannelCount() { return 16; }

// Function: OPNA_GetChannelVolume
// Get the current volume of a given channel (based on the values written with OPNA_Set*Register functions)
// Note: OPNA_USE_CHANNEL need to be set to TRUE to use this function
//
// Parameters:
//   chan - Channel index (0 to 2 for SSG, 3 to 8 for FM channels, 9 to 14 for rhythm instruments, 15 for ADPCM)
//
// Return:
//   Volume from 0 (mute) to 15 (max). FM channels, rhythm instruments and ADPCM are only 0 or 15
u8 OPNA_GetChannelVolume(u8 chan);

// Function: OPNA_GetChannelType
// Get the type of a given channel
// Note: OPNA_USE_CHANNEL need to be set to TRUE to use this function
//
// Parameters:
//   chan - Channel index (0 to 15)
//
// Return:
//   Channel type
inline u8 OPNA_GetChannelType(u8 chan) { return (chan < 3) ? CHANNEL_TYPE_TONE : (chan < 9) ? CHANNEL_TYPE_FM_INST : (chan < 15) ? CHANNEL_TYPE_FM_RHYTHM : CHANNEL_TYPE_SAMPLE; }
#endif
