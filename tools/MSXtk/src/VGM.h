//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄      ▄        
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀ ▀▀██ ▄  ██▀▄  
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █ ██▄▄ ██ ██▀   
//______________________________▀▀  ___________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/MSXbin)
// under CC-BY-AS free license (https://creativecommons.org/licenses/by-sa/2.0/)
//_____________________________________________________________________________
#pragma once

// MSXtk
#include "MSXtk.h"

//=============================================================================
// DEFINES
//=============================================================================

#define VGM_IDENT *(u32*)"Vgm "

#define VGM_WAIT_60HZ	735 // 60 Hz
#define VGM_WAIT_50HZ	882 // 50 Hz

// VGM commands
enum VGM_COMMAND
{
	// Chips
	VGM_CMD_AY8910    = 0xA0, // aa dd	  | PSG/AY8910, write value dd to register aa
	VGM_CMD_YM2413    = 0x51, // aa dd	  | MSX-MUSIC/YM2413/OPLL, write value dd to register aa
	VGM_CMD_Y8950     = 0x5C, // aa dd	  | MSX-AUDIO/Y8950/OPL1, write value dd to register aa
	VGM_CMD_SCC       = 0xD2, // pp aa dd | SCC/SCC+/K051649/K052539, port pp, write value dd to register aa
	VGM_CMD_YMF278    = 0xD0, // pp aa dd | MOONSOUND/YMF278B/OPL4, port pp, write value dd to register aa
	// Waits
	VGM_CMD_WAIT_16B  = 0x61, // nn nn	  | Wait n samples, n can range from 0 to 65535 (approx 1.49 seconds).Longer pauses than this are represented by multiple wait commands.
	VGM_CMD_WAIT_60HZ = 0x62, // wait 735 samples(60th of a second), a shortcut for 0x61 0xdf 0x02
	VGM_CMD_WAIT_50HZ = 0x63, // wait 882 samples(50th of a second), a shortcut for 0x61 0x72 0x03
	VGM_CMD_WAIT_4B   = 0x7F, // wait n+1 samples, n can range from 0 to 15.
	VGM_CMD_WAIT_1    = 0x70,
	VGM_CMD_WAIT_2    = 0x71,
	VGM_CMD_WAIT_3    = 0x72,
	VGM_CMD_WAIT_4    = 0x73,
	VGM_CMD_WAIT_5    = 0x74,
	VGM_CMD_WAIT_6    = 0x75,
	VGM_CMD_WAIT_7    = 0x76,
	VGM_CMD_WAIT_8    = 0x77,
	VGM_CMD_WAIT_9    = 0x78,
	VGM_CMD_WAIT_10   = 0x79,
	VGM_CMD_WAIT_11   = 0x7A,
	VGM_CMD_WAIT_12   = 0x7B,
	VGM_CMD_WAIT_13   = 0x7C,
	VGM_CMD_WAIT_14   = 0x7D,
	VGM_CMD_WAIT_15   = 0x7E,
	VGM_CMD_WAIT_16   = 0x7F,
	// Special
	VGM_CMD_END       = 0x66, // end of sound data
};

// VGM header file data structure
struct VGM_Header
{
	// 1.00
	u32 Ident;
	u32 EoF_offset;
	u32 Version;
	u32 SN76489_clock;
	u32 YM2413_clock;
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
	u32 YM2612_clock;
	u32 YM2151_clock;
	// 1.50
	u32 Data_offset;
	// 1.51
	u32 Sega_PCM_clock;
	u32 SPCM_Interface;
	u32 RF5C68_clock;
	u32 YM2203_clock;
	u32 YM2608_clock;
	u32 YM2610B_clock;
	u32 YM3812_clock;
	u32 YM3526_clock;
	u32 Y8950_clock;
	u32 YMF262_clock;
	u32 YMF278B_clock;
	u32 YMF271_clock;
	u32 YMZ280B_clock;
	u32 RF5C164_clock;
	u32 PWM_clock;
	u32 AY8910_clock;
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
	u32 K051649_clock;
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
	u32 dummyE4;
	u32 dummyE8;
	u32 dummyEC;
	u32 dummyF0;
	u32 dummyF4;
	u32 dummyF8;
	u32 dummyFC;
};