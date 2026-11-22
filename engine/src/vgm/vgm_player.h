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

// VGM commands
enum VGM_COMMAND
{
	// Chips
	VGM_CMD_YM2413         = 0x51, // aa dd	   | MSX-MUSIC/YM2413/OPLL, write value dd to register aa
	VGM_CMD_YM2608_0       = 0x56, // aa dd    | MAKOTO/YM2608/OPNA port 0, write value dd to register aa
	VGM_CMD_YM2608_1       = 0x57, // aa dd    | MAKOTO/YM2608/OPNA port 1, write value dd to register aa
	VGM_CMD_Y8950          = 0x5C, // aa dd	   | MSX-AUDIO/Y8950/OPL1, write value dd to register aa
	VGM_CMD_YM3812         = 0x5A, // aa dd	   | YM3812/OPL2, write value dd to register aa
	VGM_CMD_YMF262_0       = 0x5E, // aa dd    | YMF262/OPL3 port 0, write value dd to register aa
	VGM_CMD_YMF262_1       = 0x5F, // aa dd    | YMF262/OPL3 port 1, write value dd to register aa
	VGM_CMD_AY8910         = 0xA0, // aa dd	   | PSG/AY8910, write value dd to register aa
	VGM_CMD_YMF278         = 0xD0, // pp aa dd | MOONSOUND/YMF278B/OPL4, port pp, write value dd to register aa
	VGM_CMD_SCC            = 0xD2, // pp aa dd | SCC/SCC+/K051649/K052539, port pp, write value dd to register aa
	// Waits
	VGM_CMD_WAIT_16B       = 0x61, // nn nn    | Wait n samples, n can range from 0 to 65535 (approx 1.49 seconds).Longer pauses than this are represented by multiple wait commands.
	VGM_CMD_WAIT_60HZ      = 0x62, // wait 735 samples(60th of a second), a shortcut for 0x61 0xdf 0x02
	VGM_CMD_WAIT_50HZ      = 0x63, // wait 882 samples(50th of a second), a shortcut for 0x61 0x72 0x03
	VGM_CMD_WAIT_4B        = 0x70, // wait n+1 samples, n can range from 0 to 15.
	VGM_CMD_WAIT_1         = 0x70,
	VGM_CMD_WAIT_2         = 0x71,
	VGM_CMD_WAIT_3         = 0x72,
	VGM_CMD_WAIT_4         = 0x73,
	VGM_CMD_WAIT_5         = 0x74,
	VGM_CMD_WAIT_6         = 0x75,
	VGM_CMD_WAIT_7         = 0x76,
	VGM_CMD_WAIT_8         = 0x77,
	VGM_CMD_WAIT_9         = 0x78,
	VGM_CMD_WAIT_10        = 0x79,
	VGM_CMD_WAIT_11        = 0x7A,
	VGM_CMD_WAIT_12        = 0x7B,
	VGM_CMD_WAIT_13        = 0x7C,
	VGM_CMD_WAIT_14        = 0x7D,
	VGM_CMD_WAIT_15        = 0x7E,
	VGM_CMD_WAIT_16        = 0x7F,
	// Special
	VGM_CMD_END            = 0x66, // end of sound data
	VGM_CMD_BLOCK          = 0x67, // data block
	VGM_CMD_PCM_WRITE      = 0x68, // 0x66 cc oo oo oo dd dd dd ss ss ss | PCM RAM write
	VGM_CMD_DAC_WAIT       = 0x80, // YM2612 port 0 address 2A write from the data bank, then wait n samples (0x8n)
	// DAC stream control
	VGM_CMD_DAC_SETUP      = 0x90, // ss tt pp cc    | Setup stream control
	VGM_CMD_DAC_DATA       = 0x91, // ss dd ll bb    | Set stream data
	VGM_CMD_DAC_FREQ       = 0x92, // ss ff ff ff ff | Set stream frequency
	VGM_CMD_DAC_START      = 0x93, // ss aa aa aa aa mm ll ll ll ll | Start stream
	VGM_CMD_DAC_STOP       = 0x94, // ss             | Stop stream
	VGM_CMD_DAC_START_FAST = 0x95, // ss bb bb ff    | Start stream (fast call)
};

// VGM block value
enum VGM_BLOCK
{
	VGM_BLOCK_YM2608  = 0x81,
	VGM_BLOCK_YMF278B = 0x87,
	VGM_BLOCK_Y8950   = 0x88,
};

// VGM ports value
enum VGM_PORT
{
	VGM_PORT_OPL4_PART0 = 0,
	VGM_PORT_OPL4_PART1,
	VGM_PORT_OPL4_WAVE,

	VGM_PORT_OPNA_PART0 = 0,
	VGM_PORT_OPNA_PART1,
};

// VGM header file data structure
typedef struct VGM_Header
{
	// 1.00
	u32 Ident;
	u32 EoF_offset;
	u32 Version;
	u32 SN76489_clock;
	u32 YM2413_clock;				// MSX-Music (OPLL)
	u32 GD3_offset;
	u32 Total_samples;
	u32 Loop_offset;
	u32 Loop_samples;
	// 1.01
	u32 Rate;
	// 1.10
	u16 SN_FB;
	u8  SNW;
	// 1.51
	u8  SF;
	// 1.10
	u32 YM2612_clock;				// OPN2
	u32 YM2151_clock;				// OPM
	// 1.50
	u32 Data_offset;
	// 1.51
	u32 Sega_PCM_clock;
	u32 SPCM_Interface;
	u32 RF5C68_clock;
	u32 YM2203_clock;				// OPN
	u32 YM2608_clock;				// OPNA (Makoto)
	u32 YM2610B_clock;				// OPNB
	u32 YM3812_clock;				// OPL2
	u32 YM3526_clock;				// OPL/OPL1
	u32 Y8950_clock;				// MSX-Audio (OPL1)
	u32 YMF262_clock;				// OPL3
	u32 YMF278B_clock;				// OPL4
	u32 YMF271_clock;				// OPX
	u32 YMZ280B_clock;				// PCMD8
	u32 RF5C164_clock;
	u32 PWM_clock;
	u32 AY8910_clock;				// PSG
	u8  AYT;
	u8  AY_Flags[3];
	// 1.60
	u8  VM;
	u8  dummy7D;
	u8  LB;
	// 1.51
	u8  LM;
	// 1.61
	u32 GB_DMG_clock;
	u32 NES_APU_clock;
	u32 MultiPCM_clock;
	u32 uPD7759_clock;
	u32 OKIM6258_clock;
	u8  OF;
	u8  KF;
	u8  CF;
	u8  dummy97;
	u32 OKIM6295_clock;
	u32 K051649_clock;				// SCC
	u32 K054539_clock;
	u32 HuC6280_clock;
	u32 C140_clock;
	u32 K053260_clock;
	u32 Pokey_clock;
	u32 QSound_clock;
	// 1.71
	u32 SCSP_clock;
	// 1.70
	u32 Extra_Hdr_ofs;
	// 1.71
	u32 WonderSwan_clock;
	u32 VSU_clock;
	u32 SAA1099_clock;
	u32 ES5503_clock;
	u32 ES5506_clock;
	u16 ES_chns;
	u8  CD;
	u8  dummyD7;
	u32 X1_010_clock;
	u32 C352_clock;
	u32 GA20_clock;
	// 1.72
	u32 Mikey_clock;
	u32 dummyE8;
	u32 dummyEC;
	u32 dummyF0;
	u32 dummyF4;
	u32 dummyF8;
	u32 dummyFC;
} VGM_Header;

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
// Check if initialized song contains PSG data
//
// Return:
//   FALSE if music dont contains PSG data
inline bool VGM_ContainsPSG() { return (g_VGM_Header->Version >= 0x0151) && g_VGM_Header->AY8910_clock; }

// Function: VGM_ContainsMSXMusic
// Check if initialized song contains MSX-Music data
//
// Return:
//   FALSE if music dont contains MSX-Music data
inline bool VGM_ContainsMSXMusic() { return g_VGM_Header->YM2413_clock; }

// Function: VGM_ContainsMSXAudio
// Check if initialized song contains MSX-Audio data
//
// Return:
//   FALSE if music dont contains MSX-Audio data
inline bool VGM_ContainsMSXAudio() { return (g_VGM_Header->Version >= 0x0151) && g_VGM_Header->Y8950_clock; }

// Function: VGM_ContainsSCC
// Check if initialized song contains SCC data
//
// Return:
//   FALSE if music dont contains SCC data
inline bool VGM_ContainsSCC() { return (g_VGM_Header->Version >= 0x0161) && g_VGM_Header->K051649_clock; }

// Function: VGM_ContainsOPL2
// Check if initialized song contains OPL2 data
//
// Return:
//   FALSE if music dont contains OPL2 data
inline bool VGM_ContainsOPL2() { return (g_VGM_Header->Version >= 0x0151) && g_VGM_Header->YM3812_clock; }

// Function: VGM_ContainsOPL3
// Check if initialized song contains OPL3 data
//
// Return:
//   FALSE if music dont contains OPL3 data
inline bool VGM_ContainsOPL3() { return (g_VGM_Header->Version >= 0x0151) && g_VGM_Header->YMF262_clock; }

// Function: VGM_ContainsOPL4
// Check if initialized song contains OPL4 data
//
// Return:
//   FALSE if music dont contains OPL4 data
inline bool VGM_ContainsOPL4() { return (g_VGM_Header->Version >= 0x0151) && g_VGM_Header->YMF278B_clock; }

// Function: VGM_ContainsOPNA
// Check if initialized song contains OPNA data
//
// Return:
//   FALSE if music dont contains OPNA data
inline bool VGM_ContainsOPNA() { return (g_VGM_Header->Version >= 0x0151) && g_VGM_Header->YM2608_clock; }
