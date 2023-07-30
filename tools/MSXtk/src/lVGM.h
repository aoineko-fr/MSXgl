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
	LVGM_OPTION_60HZ   = 0b00000000, // Frequency (0: 60 Hz)
	LVGM_OPTION_50HZ   = 0b00000001, // Frequency (1: 50 Hz)
	LVGM_OPTION_LOOP   = 0b00000010, // Loop data included
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

// lVGM options
extern LVGM_FREQ   g_lVGM_Frequency;
extern bool        g_lVGM_AddHeader;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
bool ExportlVGM(std::string name, MSX::ExporterInterface* exp, const std::vector<u8>& data);