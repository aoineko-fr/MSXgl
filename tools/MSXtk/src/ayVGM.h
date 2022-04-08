//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄      ▄        
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀ ▀▀██ ▄  ██▀▄  
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █ ██▄▄ ██ ██▀   
//______________________________▀▀  ___________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/MSXbin)
// under CC-BY-AS free license (https://creativecommons.org/licenses/by-sa/2.0/)

// MSXtk
#include "MSXtk.h"

//=============================================================================
// DEFINES
//=============================================================================

// ayVGM export frequency
enum AYVGM_FREQ
{
	AYVGM_FREQ_50HZ = 50,
	AYVGM_FREQ_60HZ = 60,
	AYVGM_FREQ_BOTH = 0xFF,
};

// ayVGM export frequency
enum AYVGM_FLAG
{
	AYVGM_FLAG_50HZ  = 0b00000001,
	AYVGM_FLAG_60HZ  = 0b00000010,
	AYVGM_FLAG_LOOP  = 0b00000100,

	AYVGM_FLAG_SCC  = 0b00010000,
	AYVGM_FLAG_SCCI = 0b00100000,
	AYVGM_FLAG_ext3 = 0b00110000,
	AYVGM_FLAG_ext4 = 0b01000000,
	AYVGM_FLAG_ext5 = 0b01010000,
	AYVGM_FLAG_ext6 = 0b01100000,
	AYVGM_FLAG_ext7 = 0b01110000,
};

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
bool ExportAyVGM(MSX::ExporterInterface* exp, const std::vector<u8>& data);