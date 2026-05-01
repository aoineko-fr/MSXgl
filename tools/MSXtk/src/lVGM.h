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

#define LVGM_VERSION 0

// lVGM export frequency
enum LVGM_FREQ
{
	LVGM_FREQ_50HZ     = 50,
	LVGM_FREQ_60HZ     = 60,
};

// lVGM option
enum LVGM_OPTION
{
	LVGM_OPTION_FREQ = 0b00000001, // Frequency
	LVGM_OPTION_60HZ = 0b00000000, // Frequency (0: 60 Hz)
	LVGM_OPTION_50HZ = 0b00000001, // Frequency (1: 50 Hz)
	LVGM_OPTION_LOOP = 0b00000010, // Loop data included
	LVGM_OPTION_DEVICE = 0b00000100, // Devices list
	LVGM_OPTION_FORMAT = 0b11110000, // File format version (0)
};

// lVGM audio chips
enum LVGM_CHIP
{
	LVGM_CHIP_NONE      = 0,
	// Supported chips
	LVGM_CHIP_PSG       = 0b00000001, // Include PSG data (AY-3-8910)
	LVGM_CHIP_MSXMUSIC  = 0b00000010, // Include MSX-MUSIC data (YM2413/OPLL)
	LVGM_CHIP_MSXAUDIO  = 0b00000100, // Include MSX-AUDIO data (Y8950/OPL1 + ADPCM)
	LVGM_CHIP_SCC       = 0b00001000, // Include SCC data (K051649)
	LVGM_CHIP_SCCI      = 0b00010000, // Include SCC+ data (K052539)
	LVGM_CHIP_PSG2      = 0b00100000, // Include secondary PSG data
	LVGM_CHIP_7         = 0b01000000, // 
	LVGM_CHIP_MOONSOUND = 0b10000000, // Include Moonsound data (YMF278/OPL4)
	// Alias
	LVGM_CHIP_AY8910    = LVGM_CHIP_PSG,
	LVGM_CHIP_YM2413    = LVGM_CHIP_MSXMUSIC,
	LVGM_CHIP_OPLL      = LVGM_CHIP_MSXMUSIC,
	LVGM_CHIP_Y8950     = LVGM_CHIP_MSXAUDIO,
	LVGM_CHIP_OPL1      = LVGM_CHIP_MSXAUDIO,
	LVGM_CHIP_K051649   = LVGM_CHIP_SCC,
	LVGM_CHIP_K052539   = LVGM_CHIP_SCCI,
	LVGM_CHIP_YMF278    = LVGM_CHIP_MOONSOUND,
	LVGM_CHIP_OPL4      = LVGM_CHIP_MOONSOUND,
};

// LVGM playback state flag
enum LVGM_STATE
{
	LVGM_STATE_50HZ = 0b00000001,
	LVGM_STATE_LOOP = 0b00000010,
	LVGM_STATE_PLAY = 0b10000000,
};

// Special operator
enum LVGM_OP
{
	LVGM_OP_PSG    = 0xF0, // Start of PSG chunk (default when not defined)
	LVGM_OP_OPLL   = 0xF1, // Start of MSX-MUSIC chunk
	LVGM_OP_OPL1   = 0xF2, // Start of MSX-AUDIO chunk
	LVGM_OP_SCC    = 0xF3, // Start of SCC chunk
	LVGM_OP_SCCI   = 0xF4, // Start of SCC+ chunk
	LVGM_OP_PSG2   = 0xF5, // Start of secondary PSG chunk
	LVGM_OP_OPL4   = 0xF7, // Start of Moonsound chunk
	LVGM_OP_WAIT   = 0xFC, // Wait counter
	LVGM_OP_NOTIFY = 0xFD, // Optional markers
	LVGM_OP_LOOP   = 0xFE, // Loop position
	LVGM_OP_END    = 0xFF, // End of song
};

// Special operator
enum LVGM_NOTIFY
{
	LVGM_NOTIFY_SEG_END   = 0x00, // End of data segment
	LVGM_NOTIFY_LOOP_MARK = 0xFE, // Reach loop marker
	LVGM_NOTIFY_LOOP_JUMP = 0xFF, // Jump to loop marker
};

// 
enum LVGM_SIMPLIFY
{
	LVGM_SIMPLIFY_NONE,
	LVGM_SIMPLIFY_ORDER,     // Reorder register write
	LVGM_SIMPLIFY_DUPLICATE, // Reorder and clean duplicate register write in the same frame (keep only the last write)
};

// lVGM options
extern LVGM_FREQ     g_lVGM_Frequency;
extern bool          g_lVGM_AddHeader;
extern LVGM_SIMPLIFY g_lVGM_Simplify;
extern u32           g_lVGM_Split;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
bool ExportlVGM(std::string name, MSX::ExporterInterface* exp, const std::vector<u8>& data);