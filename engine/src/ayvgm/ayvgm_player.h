// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#include "psg.h"

//=============================================================================
// DEFINES
//=============================================================================

/// VGM header file data structure
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
extern u16       g_VGM_Wait;

//=============================================================================
// FUNCTIONS
//=============================================================================

/// Play a VGM data
bool VGM_Play(const void* addr, u8 loop);

/// Decode a frame of VGM data
void VGM_Decode();