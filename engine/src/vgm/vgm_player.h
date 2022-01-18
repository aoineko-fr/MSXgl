// __________________________
// ██▀█▀██▀▀▀█▀▀█▀█  ▄▄▄ ▄▄  │  ▄▄▄   ▄▄   ▄ ▄ ▄▄▄▄ ▄▄▄   ▄▄ 
// █  ▄ █▄ ▀██▄ ▀▄█ ██   ██  │  ██▄▀ ██ ▀ ██▀█ ██▄  ██▀█ ██ ▀
// █  █ █▀▀ ▄█  █ █ ▀█▄█ ██▄▄│  ██   ▀█▄▀ ██ █ ██▄▄ ██ █ ▀█▄▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀──────────┘  
//  PCM-Encoder replayer
//─────────────────────────────────────────────────────────────────────────────
#include "psg.h"

//=============================================================================
// DEFINES
//=============================================================================

// VGM playback state flag
enum VGM_STATE
{
	VGM_STATE_50HZ = 0b00000001,
	VGM_STATE_LOOP = 0b00000010,
	VGM_STATE_PLAY = 0b10000000,
};

#define VGM_WAIT_50HZ	882 // 50 Hz
#define VGM_WAIT_60HZ	735 // 60 Hz

// VGM header file data structure
struct VGM_Header
{
	u32 Ident;			u32 EoF_offset;		u32 Version;		u32 SN76489_clock;
	u32 YM2413_clock;	u32 GD3_offset;		u32 Total_samples;	u32 Loop_offset;
	u32 Loop_samples;	u32 Rate;			u16 SN_FB;
											u8 SNW;
											u8 SF;				u32 YM2612_clock;
	u32 YM2151_clock;	u32 Data_offset;	u32 Sega_PCM_clock;	u32 SPCM_Interface;
	u32 RF5C68_clock;	u32 YM2203_clock;	u32 YM2608_clock;	u32 YM2610B_clock;
	u32 YM3812_clock;	u32 YM3526_clock;	u32 Y8950_clock;	u32 YMF262_clock;
	u32 YMF278B_clock;	u32 YMF271_clock;	u32 YMZ280B_clock;	u32 RF5C164_clock;
	u32 PWM_clock;		u32 AY8910_clock;	u8 AYT;
											u8 AY_Flags[3];		u8 VM;
																u8 dummy7D;
																u8 LB;
																u8 LM;
	u32 GB_DMG_clock;	u32 NES_APU_clock;	u32 MultiPCM_clock;	u32 uPD7759_clock;
	u32 OKIM6258_clock;	u8 OF;
						u8 KF;
						u8 CF;
						u8 dummy97;			u32 OKIM6295_clock;	u32 K051649_clock;
	u32 K054539_clock;	u32 HuC6280_clock;	u32 C140_clock;		u32 K053260_clock;
	u32 Pokey_clock;	u32 QSound_clock;	u32 SCSP_clock;		u32 Extra_Hdr_ofs;
	u32 WonderSwan_clock; u32 VSU_clock;	u32 SAA1099_clock;	u32 ES5503_clock;
	u32 ES5506_clock;	u16 ES_chns;
						u8 CD;
						u8 dummyD7;			u32 X1_010_clock;	u32 C352_clock;
	u32 GA20_clock;		u32 dummyE4;		u32 dummyE8;		u32 dummyEC;
	u32 dummyF0;		u32 dummyF4;		u32 dummyF8;		u32 dummyFC;
};

// VGM work variables
extern const struct VGM_Header* g_VGM_Header;
extern const u8* g_VGM_Pointer;
extern const u8* g_VGM_Loop;
extern u16       g_VGM_WaitCount;
extern u16       g_VGM_WaitFrame;
extern u8        g_VGM_State;

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: VGM_Play
// Start music playback
bool VGM_Play(const void* addr, bool loop);

// Function: VGM_Stop
// Stop music playback
void VGM_Stop();

// Function: ayVGM_SetFrequency50Hz
// Change frequency to 50 Hz
inline bool VGM_SetFrequency50Hz() { g_VGM_State |= VGM_STATE_50HZ; g_VGM_WaitFrame = VGM_WAIT_50HZ; }

// Function: ayVGM_SetFrequency60Hz
// Change frequency to 60 Hz
inline bool VGM_SetFrequency60Hz() { g_VGM_State &= ~VGM_STATE_50HZ; g_VGM_WaitFrame = VGM_WAIT_60HZ; }

// Function: VGM_IsPlaying
// Check if music playing
inline bool VGM_IsPlaying() { return g_VGM_State & VGM_STATE_PLAY; }

// Function: VGM_Resume
// Resume music playback
inline void VGM_Resume() { g_VGM_State |= VGM_STATE_PLAY; }

// Function: VGM_Pause
// Pause music playback
inline void VGM_Pause() { g_VGM_State &= ~VGM_STATE_PLAY; PSG_Silent(); }

// Function: VGM_Decode
// Decode a frame of music
void VGM_Decode();