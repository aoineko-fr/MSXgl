// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄  ▄▄▄  ▄▄   ▄    
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██▄▀ ██   █▄██ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ██   ██▄▄   ██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// OL4 (YMF278B) handler module
//
// References:
//  - OPL4 YMF278B Application Manual
//     https://map.grauw.nl/resources/sound/yamaha_opl4.pdf
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================

#include "audio/opl4.h"

//=============================================================================
// DEFINES
//=============================================================================

#define OPL4_MEM_BLOCK_MAX			32		// Number of 128 KB blocks in the 22-bit wave memory space (4 MB)
#define OPL4_MEM_MARKER				0xA5	// Base value of the RAM detection marker
#define OPL4_MIX_MUTE				0x3F	// Mix control value to mute both sides
#define OPL4_MIX_FM_DEFAULT			0x1B	// FM mix control value after reset (-9 dB on both sides)
#define OPL4_MIX_WAVE_DEFAULT		0x00	// Wave mix control value after reset (0 dB on both sides)

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Expansion register (R#261) value for each compatibility mode
const u8 g_OPL4_ModeFlags[OPL4_MODE_MAX] =
{
	0,								// OPL4_MODE_OPL2
	OPL4_F_NEW,						// OPL4_MODE_OPL3
	OPL4_F_NEW | OPL4_F_NEW2,		// OPL4_MODE_OPL4
};

//=============================================================================
// MEMORY DATA
//=============================================================================

// Current expansion register (R#261) value
u8 g_OPL4_Expansion = OPL4_F_NEW | OPL4_F_NEW2;

#if (OPL4_USE_RESUME)
	// Backup of mix control registers
	u8 g_OPL4_BakMixFM = OPL4_MIX_FM_DEFAULT;
	u8 g_OPL4_BakMixWave = OPL4_MIX_WAVE_DEFAULT;
	bool g_OPL4_Muted = FALSE;
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize OPL4 module
bool OPL4_Initialize(u8 mode)
{
	// Check for OPL4 device
	if (!OPL4_Detect())
		return FALSE;

	// Set compatibility mode
	g_OPL4_Expansion = g_OPL4_ModeFlags[mode];
	OPL4_SetPart1Register(OPL4_REG_EXPANSION, g_OPL4_Expansion);
	return TRUE;
}

//-----------------------------------------------------------------------------
// Detect OPL4 device
bool OPL4_Detect()
{
	// Is port connected?
	if (io_OPL4_Reg0 == 0xFF)
		return FALSE;

	// Enable wave part access (needed to read the device ID)
	OPL4_SetPart1Register(OPL4_REG_EXPANSION, OPL4_F_NEW | OPL4_F_NEW2);
	return (OPL4_GetWaveRegister(OPL4_REG_WAV_MEMORY) & OPL4_ID_MASK) == OPL4_ID_YMF278B;
}

//-----------------------------------------------------------------------------
// Set FM and wave mix control registers (whatever the compatibility mode)
void OPL4_SetMix(u8 fm, u8 wave)
{
	OPL4_SetPart1Register(OPL4_REG_EXPANSION, OPL4_F_NEW | OPL4_F_NEW2); // Wave registers are only accessible while NEW2 = 1
	OPL4_SetWaveRegister(OPL4_REG_MIX_FM, fm);
	OPL4_SetWaveRegister(OPL4_REG_MIX_WAVE, wave);
	OPL4_SetPart1Register(OPL4_REG_EXPANSION, g_OPL4_Expansion); // Restore compatibility mode
}

//-----------------------------------------------------------------------------
// Mute OPL4 sound
void OPL4_Mute()
{
#if (OPL4_USE_RESUME)
	if (!g_OPL4_Muted) // Don't overwrite the backup with mute values
	{
		OPL4_SetPart1Register(OPL4_REG_EXPANSION, OPL4_F_NEW | OPL4_F_NEW2);
		g_OPL4_BakMixFM = OPL4_GetWaveRegister(OPL4_REG_MIX_FM);
		g_OPL4_BakMixWave = OPL4_GetWaveRegister(OPL4_REG_MIX_WAVE);
		g_OPL4_Muted = TRUE;
	}
#endif
	OPL4_SetMix(OPL4_MIX_MUTE, OPL4_MIX_MUTE);
}

#if (OPL4_USE_RESUME)

//-----------------------------------------------------------------------------
// Resume OPL4 sound
void OPL4_Resume()
{
	if (g_OPL4_Muted)
	{
		OPL4_SetMix(g_OPL4_BakMixFM, g_OPL4_BakMixWave);
		g_OPL4_Muted = FALSE;
	}
}

#endif


#if (OPL4_USE_WAVE)

//-----------------------------------------------------------------------------
// Set wave memory address (and select the memory data register)
void OPL4_WaveSetMemAddr(u8 addr_h, u16 addr_l)
{
	OPL4_SetWaveRegister(OPL4_REG_WAV_ADDR_H, addr_h);
	OPL4_SetWaveRegister(OPL4_REG_WAV_ADDR_M, (u8)(addr_l >> 8));
	OPL4_SetWaveRegister(OPL4_REG_WAV_ADDR_L, (u8)addr_l); // Apply the full address
	io_OPL4_WaveReg = OPL4_REG_WAV_DATA;
}

//-----------------------------------------------------------------------------
// Write Wave data into the OPL4 memory
void OPL4_WaveLoad(const u8* data, u8 addr_h, u16 addr_l, u16 size)
{
	OPL4_WaveStartMemAccess(); // Enable memory access mode

	OPL4_WaveSetMemAddr(addr_h, addr_l);
	while (size--)
		io_OPL4_WaveData = *data++; // Address auto-increments

	OPL4_WaveEndMemAccess(); // End of memory access
}

//-----------------------------------------------------------------------------
// Get OPL4 total wave memory size (in 128 KB blocks)
u8 OPL4_WaveGetMemSize()
{
	OPL4_WaveStartMemAccess(); // Enable memory access mode

	// Write a 2-bytes marker at the start of each block, from the last to the first, so mirrored blocks read the lowest block marker
	// The second byte is the complement of the first one, so a ROM block can't match by chance
	for (u8 i = OPL4_MEM_BLOCK_MAX; i > 0; i--)
	{
		u8 block = i - 1;
		OPL4_WaveSetMemAddr(block << 1, 0x0000);
		io_OPL4_WaveData = OPL4_MEM_MARKER + block;
		io_OPL4_WaveData = ~(OPL4_MEM_MARKER + block); // Address auto-increments
	}

	// Count the blocks that read back their own marker (ROM blocks ignore writes)
	u8 count = 0;
	for (u8 i = 0; i < OPL4_MEM_BLOCK_MAX; i++)
	{
		OPL4_WaveSetMemAddr(i << 1, 0x0000);
		u8 m0 = io_OPL4_WaveData;
		u8 m1 = io_OPL4_WaveData; // Address auto-increments
		if ((m0 == (u8)(OPL4_MEM_MARKER + i)) && (m1 == (u8)~(OPL4_MEM_MARKER + i)))
			count++;
	}

	OPL4_WaveEndMemAccess(); // End of memory access
	return count;
}

#endif // (OPL4_USE_WAVE)
