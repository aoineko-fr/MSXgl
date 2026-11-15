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

//=============================================================================
// INCLUDES
//=============================================================================

#include "audio/opna.h"

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Mute registers               |----- SSG -------|--- FM CH1-3 ---|
const u8 g_OPNA_MuteReg0[] = { 0x08, 0x09, 0x0A, 0xB4, 0xB5, 0xB6 };
// Mute registers               |ADPCM|--- FM CH4-6 ---|
const u8 g_OPNA_MuteReg1[] = { 0x0B, 0xB4, 0xB5, 0xB6 };

//=============================================================================
// MEMORY DATA
//=============================================================================

#if (OPNA_USE_RESUME)
	// Backup registers
	u8 g_OPNA_BakReg0[sizeof(g_OPNA_MuteReg0)] = { 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0 };
	u8 g_OPNA_BakReg1[sizeof(g_OPNA_MuteReg1)] = { 0x00, 0xC0, 0xC0, 0xC0 };
	bool g_OPNA_DoBackup = TRUE;
	#define OPNA_DOBACKUP(do)	g_OPNA_DoBackup = do
#else
	#define OPNA_DOBACKUP(do)
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize OPNA module
bool OPNA_Initialize()
{
	if (!OPNA_Detect())
		return FALSE;

#if (OPNA_USE_ADPCM)
	// Disable all interrupts
	OPNA_SetPart0Register(OPNA_REG_MODE, OPNA_F_SCH);
#endif

	return TRUE;
}

//-----------------------------------------------------------------------------
// Search for OPNA (OPNA/YM2608) chip
bool OPNA_Detect()
{
	if (io_OPNA_Stat == 0xFF) // Is port connected?
		return FALSE;
	return OPNA_GetPart0Register(OPNA_REG_ID) == OPNA_ID_YM2608;
}

//-----------------------------------------------------------------------------
// Set OPNA register value
void OPNA_SetPart0Register(u8 reg, u8 value)
{
	io_OPNA_Reg0 = reg;
	io_OPNA_Data0 = value;

#if (OPNA_USE_RESUME)
	if (g_OPNA_DoBackup)
	{
		switch (reg)
		{
		case 0x08: g_OPNA_BakReg0[0] = value; break;
		case 0x09: g_OPNA_BakReg0[1] = value; break;
		case 0x0A: g_OPNA_BakReg0[2] = value; break;
		case 0xB4: g_OPNA_BakReg0[3] = value; break;
		case 0xB5: g_OPNA_BakReg0[4] = value; break;
		case 0xB6: g_OPNA_BakReg0[5] = value; break;
		}
	}
#endif
}

//-----------------------------------------------------------------------------
// Get OPNA register value (most of the registers are write-only)
u8 OPNA_GetPart0Register(u8 reg)
{
	io_OPNA_Reg0 = reg;
	return io_OPNA_Data0;
}

//-----------------------------------------------------------------------------
// Set OPNA register value
void OPNA_SetPart1Register(u8 reg, u8 value)
{
	io_OPNA_Reg1 = reg;
	io_OPNA_Data1 = value;

#if (OPNA_USE_RESUME)
	if (g_OPNA_DoBackup)
	{
		switch (reg)
		{
		case 0x0B: g_OPNA_BakReg1[0] = value; break;
		case 0xB4: g_OPNA_BakReg1[1] = value; break;
		case 0xB5: g_OPNA_BakReg1[2] = value; break;
		case 0xB6: g_OPNA_BakReg1[3] = value; break;
		}
	}
#endif
}

//-----------------------------------------------------------------------------
// Get OPNA register value (most of the registers are write-only)
u8 OPNA_GetPart1Register(u8 reg)
{
	io_OPNA_Reg1 = reg;
	return io_OPNA_Data1;
}

//-----------------------------------------------------------------------------
// Mute OPNA sound
void OPNA_Mute()
{
	OPNA_DOBACKUP(FALSE);
	// Stop SGG, FM and ADPCM-B sound
	loop(i, sizeof(g_OPNA_MuteReg0)) 
		OPNA_SetPart0Register(g_OPNA_MuteReg0[i], 0);
	loop(i, sizeof(g_OPNA_MuteReg1))
		OPNA_SetPart1Register(g_OPNA_MuteReg1[i], 0);
	// Stop Rythm
	OPNA_RythmSetKey(0xBF);
	OPNA_DOBACKUP(TRUE);
}

#if (OPNA_USE_RESUME)
//-----------------------------------------------------------------------------
// Resume OPNA sound
void OPNA_Resume()
{
	// Restore SGG, FM and ADPCM-B sound
	loop(i, sizeof(g_OPNA_MuteReg0))
		OPNA_SetPart0Register(g_OPNA_MuteReg0[i], g_OPNA_BakReg0[i]);
	loop(i, sizeof(g_OPNA_MuteReg1))
		OPNA_SetPart1Register(g_OPNA_MuteReg1[i], g_OPNA_BakReg1[i]);
}
#endif

#if (OPNA_USE_ADPCM)

//-----------------------------------------------------------------------------
// Write ADPCM data into the cartridge sample RAM
void OPNA_ADPCMLoad(const u8* data, u16 addr, u16 size)
{
	OPNA_SetPart1Register(OPNA_REG_ADPCM_FLAG, OPNA_F_MSK_ZERO | OPNA_F_MSK_TB | OPNA_F_MSK_TA); // Enable BRDY and EOS flags (13h)
	OPNA_SetPart1Register(OPNA_REG_ADPCM_FLAG, OPNA_F_IRQ_RESET); // Reset all flags
	OPNA_SetPart1Register(OPNA_REG_ADPCM_CTRL1, OPNA_F_ADPCM_REC | OPNA_F_ADPCM_MEM); // Memory write mode (60h)
	OPNA_SetPart1Register(OPNA_REG_ADPCM_CTRL2, 0); // Memory type: x1-bit DRAM
	OPNA_SetPart1Register(OPNA_REG_ADPCM_START_L, (u8)addr);
	OPNA_SetPart1Register(OPNA_REG_ADPCM_START_H, (u8)(addr >> 8));
	u16 stop = addr + size - 1;
	OPNA_SetPart1Register(OPNA_REG_ADPCM_STOP_L, (u8)stop);
	OPNA_SetPart1Register(OPNA_REG_ADPCM_STOP_H, (u8)(stop >> 8));
	OPNA_SetPart1Register(OPNA_REG_ADPCM_LIMIT_L, 0xFF); // Limit must always be set (no address wrap)
	OPNA_SetPart1Register(OPNA_REG_ADPCM_LIMIT_H, 0xFF);
	u16 count = size << 2; // 16384 units (64 KB) gives 0, handled as 65536 by the do-while
	do
	{
		io_OPNA_Reg1 = OPNA_REG_ADPCM_DATA;
		io_OPNA_Data1 = *data++;
		io_OPNA_Reg1 = OPNA_REG_ADPCM_FLAG;
		io_OPNA_Data1 = OPNA_F_MSK_ZERO | OPNA_F_MSK_BRDY | OPNA_F_MSK_TB | OPNA_F_MSK_TA; // Reset BRDY flag (1Bh)
		io_OPNA_Data1 = OPNA_F_MSK_ZERO | OPNA_F_MSK_TB | OPNA_F_MSK_TA; // Enable BRDY and EOS flags (13h)
		while (!(OPNA_GetStatus() & OPNA_STATUS_BRDY)) {} // Wait for the data to be written (status is FFh when no chip, so no infinite loop)
	} while (--count);
	OPNA_SetPart1Register(OPNA_REG_ADPCM_CTRL1, 0); // End of memory access
	OPNA_SetPart1Register(OPNA_REG_ADPCM_FLAG, OPNA_F_IRQ_RESET); // Reset all flags
}

//-----------------------------------------------------------------------------
// Start the playback of a sample loaded in RAM (see <OPNA_ADPCMLoad>)
void OPNA_ADPCMPlay(u16 addr, u16 size, bool loop)
{
	OPNA_SetPart1Register(OPNA_REG_ADPCM_START_L, (u8)addr);
	OPNA_SetPart1Register(OPNA_REG_ADPCM_START_H, (u8)(addr >> 8));
	u16 stop = addr + size - 1;
	OPNA_SetPart1Register(OPNA_REG_ADPCM_STOP_L, (u8)stop);
	OPNA_SetPart1Register(OPNA_REG_ADPCM_STOP_H, (u8)(stop >> 8));
	OPNA_SetPart1Register(OPNA_REG_ADPCM_CTRL1, loop ? OPNA_F_ADPCM_START | OPNA_F_ADPCM_MEM | OPNA_F_ADPCM_REPEAT : OPNA_F_ADPCM_START | OPNA_F_ADPCM_MEM);
}

#endif // (OPNA_USE_ADPCM)
