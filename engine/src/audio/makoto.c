// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄      ▄▄        ▄▄       
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▄▀██ ██▄▀ ▄█▀▄ ██▀ ▄█▀▄ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀▄██ ██ █ ▀█▄▀ ▀█▄ ▀█▄▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                           
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Makoto (YM2608 - OPNA) handler module
//
// References:
//  - YM2608 (OPNA) Application Manual
//     https://theretroweb.com/chip/documentation/ym2608-applicatin-manual-jp-67b9ef528ddcb605215687.pdf
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================

#include "audio/makoto.h"

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Mute registers               |----- SSG -------|--- FM CH1-3 ---|
const u8 g_Makoto_MuteReg0[] = { 0x08, 0x09, 0x0A, 0xB4, 0xB5, 0xB6 };
// Mute registers               |ADPCM|--- FM CH4-6 ---|
const u8 g_Makoto_MuteReg1[] = { 0x0B, 0xB4, 0xB5, 0xB6 };

//=============================================================================
// MEMORY DATA
//=============================================================================

#if (MAKOTO_USE_RESUME)
	// Backup registers
	u8 g_Makoto_BakReg0[sizeof(g_Makoto_MuteReg0)] = { 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0 };
	u8 g_Makoto_BakReg1[sizeof(g_Makoto_MuteReg1)] = { 0x00, 0xC0, 0xC0, 0xC0 };
	bool g_Makoto_DoBackup = TRUE;
	#define MAKOTO_DOBACKUP(do)	g_Makoto_DoBackup = do
#else
	#define MAKOTO_DOBACKUP(do)
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize Makoto module
bool Makoto_Initialize()
{
	bool ret = Makoto_Detect();
	return ret;
}

//-----------------------------------------------------------------------------
// Search for Makoto (YM2608) chip
bool Makoto_Detect()
{
	if (g_OPNA_StatusPort == 0xFF) // Is port connected?
		return FALSE;
	return Makoto_GetRegister0(OPNA_REG_ID) == OPNA_ID_YM2608;
}

//-----------------------------------------------------------------------------
// Set Makoto register value
void Makoto_SetRegister0(u8 reg, u8 value)
{
	g_OPNA_Addr0Port = reg;
	g_OPNA_Data0Port = value;

	if (reg == OPNA_REG_RHY_KEY)
		while (Makoto_IsBusy()) {}

#if (MAKOTO_USE_RESUME)
	if (g_Makoto_DoBackup)
	{
		switch (reg)
		{
		case 0x08: g_Makoto_BakReg0[0] = value; break;
		case 0x09: g_Makoto_BakReg0[1] = value; break;
		case 0x0A: g_Makoto_BakReg0[2] = value; break;
		case 0xB4: g_Makoto_BakReg0[3] = value; break;
		case 0xB5: g_Makoto_BakReg0[4] = value; break;
		case 0xB6: g_Makoto_BakReg0[5] = value; break;
		}
	}
#endif
}

//-----------------------------------------------------------------------------
// Get Makoto register value (most of the registers are write-only)
u8 Makoto_GetRegister0(u8 reg)
{
	g_OPNA_Addr0Port = reg;
	return g_OPNA_Data0Port;
}

//-----------------------------------------------------------------------------
// Set Makoto register value
void Makoto_SetRegister1(u8 reg, u8 value)
{
	g_OPNA_Addr1Port = reg;
	g_OPNA_Data1Port = value;

#if (MAKOTO_USE_RESUME)
	if (g_Makoto_DoBackup)
	{
		switch (reg)
		{
		case 0x0B: g_Makoto_BakReg1[0] = value; break;
		case 0xB4: g_Makoto_BakReg1[1] = value; break;
		case 0xB5: g_Makoto_BakReg1[2] = value; break;
		case 0xB6: g_Makoto_BakReg1[3] = value; break;
		}
	}
#endif
}

//-----------------------------------------------------------------------------
// Get Makoto register value (most of the registers are write-only)
u8 Makoto_GetRegister1(u8 reg)
{
	g_OPNA_Addr1Port = reg;
	return g_OPNA_Data1Port;
}

//-----------------------------------------------------------------------------
// Mute Makoto sound
void Makoto_Mute()
{
	MAKOTO_DOBACKUP(FALSE);
	// Stop SGG, FM and ADPCM-B sound
	loop(i, sizeof(g_Makoto_MuteReg0)) 
		Makoto_SetRegister0(g_Makoto_MuteReg0[i], 0);
	loop(i, sizeof(g_Makoto_MuteReg1))
		Makoto_SetRegister1(g_Makoto_MuteReg1[i], 0);
	// Stop Rythm
	Makoto_SetRegister0(OPNA_REG_RHY_KEY, 0xBF);
	MAKOTO_DOBACKUP(TRUE);
}

#if (MAKOTO_USE_RESUME)
//-----------------------------------------------------------------------------
// Resume Makoto sound
void Makoto_Resume()
{
	// Restore SGG, FM and ADPCM-B sound
	loop(i, sizeof(g_Makoto_MuteReg0))
		Makoto_SetRegister0(g_Makoto_MuteReg0[i], g_Makoto_BakReg0[i]);
	loop(i, sizeof(g_Makoto_MuteReg1))
		Makoto_SetRegister1(g_Makoto_MuteReg1[i], g_Makoto_BakReg1[i]);
}
#endif
