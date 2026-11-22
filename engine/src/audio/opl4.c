// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄  ▄▄▄  ▄▄   ▄    
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██▄▀ ██   █▄██ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ██   ██▄▄   ██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// OPL4 (YMF278B) handler module
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
#define OPL4_WAVE_MUTE				0xFF	// Wave total level value to mute a channel (TL = 7Fh with level direct)

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
	// Backup registers
	u8 g_OPL4_BakFM0[OPL4_FM_CH_NUM] = { 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30 }; // R#C0-C8 (part 0)
	u8 g_OPL4_BakFM1[OPL4_FM_CH_NUM] = { 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30 }; // R#C0-C8 (part 1)
	u8 g_OPL4_BakWave[OPL4_WAVE_CH_NUM];                                                         // W#50-67
	bool g_OPL4_DoBackup = TRUE;
	#define OPL4_DOBACKUP(do)		g_OPL4_DoBackup = do
#else
	#define OPL4_DOBACKUP(do)
#endif

#if (OPL4_USE_CHANNEL)
	u8 g_OPL4_ChanKey[2 * OPL4_FM_CH_NUM];  // Copy of R#B0-B8 (part 0 then part 1)
	u8 g_OPL4_ChanTrig[2 * OPL4_FM_CH_NUM]; // R#B0-B8 values ORed since the last read
	u8 g_OPL4_ChanRhythm;                   // Copy of R#BD (part 0)
	u8 g_OPL4_ChanRhyTrig;                  // R#BD values ORed since the last read
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

	#if (OPL4_USE_CHANNEL)
	// All channels are keyed off
	g_OPL4_ChanRhythm = 0;
	g_OPL4_ChanRhyTrig = 0;
	loop(i, 2 * OPL4_FM_CH_NUM)
	{
		g_OPL4_ChanKey[i] = 0;
		g_OPL4_ChanTrig[i] = 0;
	}
	#endif

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

#if (OPL4_USE_REGCOPY)

//-----------------------------------------------------------------------------
// Set OPL4 FM register value for part 0
void OPL4_SetPart0Register(u8 reg, u8 value)
{
	io_OPL4_Reg0 = reg;
	io_OPL4_Data0 = value;
	#if (OPL4_USE_RESUME)
	if (g_OPL4_DoBackup && (reg >= OPL4_REG_FBCNT) && (reg < OPL4_REG_FBCNT + OPL4_FM_CH_NUM))
		g_OPL4_BakFM0[reg - OPL4_REG_FBCNT] = value;
	#endif
	#if (OPL4_USE_CHANNEL)
	u8 i = reg - OPL4_REG_FNUM_H;
	if (i < OPL4_FM_CH_NUM) // Key on
	{
		g_OPL4_ChanKey[i] = value;
		g_OPL4_ChanTrig[i] |= value;
	}
	else if (reg == OPL4_REG_RHYTHM)
	{
		g_OPL4_ChanRhythm = value;
		g_OPL4_ChanRhyTrig |= value;
	}
	#endif
}

//-----------------------------------------------------------------------------
// Set OPL4 FM register value for part 1
void OPL4_SetPart1Register(u8 reg, u8 value)
{
	io_OPL4_Reg1 = reg;
	io_OPL4_Data1 = value;
	#if (OPL4_USE_RESUME)
	if (g_OPL4_DoBackup && (reg >= OPL4_REG_FBCNT) && (reg < OPL4_REG_FBCNT + OPL4_FM_CH_NUM))
		g_OPL4_BakFM1[reg - OPL4_REG_FBCNT] = value;
	#endif
	#if (OPL4_USE_CHANNEL)
	u8 i = reg - OPL4_REG_FNUM_H;
	if (i < OPL4_FM_CH_NUM) // Key on
	{
		g_OPL4_ChanKey[OPL4_FM_CH_NUM + i] = value;
		g_OPL4_ChanTrig[OPL4_FM_CH_NUM + i] |= value;
	}
	#endif
}

#endif // (OPL4_USE_REGCOPY)

#if (OPL4_USE_RESUME)

//-----------------------------------------------------------------------------
// Set OPL4 wave register value
void OPL4_SetWaveRegister(u8 reg, u8 value)
{
	io_OPL4_WaveReg = reg;
	io_OPL4_WaveData = value;
	if (g_OPL4_DoBackup && (reg >= OPL4_REG_WAV_TL) && (reg < OPL4_REG_WAV_TL + OPL4_WAVE_CH_NUM))
		g_OPL4_BakWave[reg - OPL4_REG_WAV_TL] = value;
}

#endif // (OPL4_USE_RESUME)

//-----------------------------------------------------------------------------
// Mute OPL4 sound
void OPL4_Mute()
{
	OPL4_DOBACKUP(FALSE);
	// Disable FM channels output
	loop(i, OPL4_FM_CH_NUM)
	{
		OPL4_SetPart0Register(OPL4_REG_FBCNT + i, 0);
		OPL4_SetPart1Register(OPL4_REG_FBCNT + i, 0);
	}
	// Set wave channels total level to minimum (writes are ignored while NEW2 = 0)
	loop(i, OPL4_WAVE_CH_NUM)
		OPL4_SetWaveRegister(OPL4_REG_WAV_TL + i, OPL4_WAVE_MUTE);
	OPL4_DOBACKUP(TRUE);

	#if (OPL4_USE_CHANNEL)
	// Release channels state
	loop(i, 2 * OPL4_FM_CH_NUM)
	{
		g_OPL4_ChanKey[i] = 0;
		g_OPL4_ChanTrig[i] = 0;
	}
	g_OPL4_ChanRhythm &= OPL4_F_RHY_EN;
	g_OPL4_ChanRhyTrig = 0;
	#endif
}

#if (OPL4_USE_RESUME)

//-----------------------------------------------------------------------------
// Resume OPL4 sound
void OPL4_Resume()
{
	// Restore FM channels output
	loop(i, OPL4_FM_CH_NUM)
	{
		OPL4_SetPart0Register(OPL4_REG_FBCNT + i, g_OPL4_BakFM0[i]);
		OPL4_SetPart1Register(OPL4_REG_FBCNT + i, g_OPL4_BakFM1[i]);
	}
	// Restore wave channels total level
	loop(i, OPL4_WAVE_CH_NUM)
		OPL4_SetWaveRegister(OPL4_REG_WAV_TL + i, g_OPL4_BakWave[i]);
}

#endif

#if (OPL4_USE_CHANNEL)

//-----------------------------------------------------------------------------
// Get the current volume of a given channel
u8 OPL4_GetChannelVolume(u8 chan)
{
	u8 on;
	if (chan < 2 * OPL4_FM_CH_NUM) // FM channel
	{
		if ((chan >= 6) && (chan < 9) && (g_OPL4_ChanRhythm & OPL4_F_RHY_EN)) // Used by rhythm
			return 0;
		on = g_OPL4_ChanKey[chan] | g_OPL4_ChanTrig[chan];
		g_OPL4_ChanTrig[chan] = 0;
		return (on & OPL4_F_KEY_ON) ? 15 : 0;
	}
	chan -= 2 * OPL4_FM_CH_NUM;
	if (chan < 5) // Rhythm instrument
	{
		if (!(g_OPL4_ChanRhythm & OPL4_F_RHY_EN))
			return 0;
		u8 bit = OPL4_F_RHY_BD >> chan; // BD, SD, TOM, TCY, HH
		on = (g_OPL4_ChanRhythm | g_OPL4_ChanRhyTrig) & bit;
		g_OPL4_ChanRhyTrig &= ~bit;
		return on ? 15 : 0;
	}
	// Wave channel (registers are readable)
	if (!(g_OPL4_Expansion & OPL4_F_NEW2))
		return 0;
	chan -= 5;
	if (!(OPL4_GetWaveRegister(OPL4_REG_WAV_KEY + chan) & OPL4_F_WAV_KEY_ON))
		return 0;
	return (u8)~OPL4_GetWaveRegister(OPL4_REG_WAV_TL + chan) >> 4; // Total level (bits 7-1) is an attenuation
}

#endif // (OPL4_USE_CHANNEL)


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
