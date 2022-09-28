// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// PCCM Ninja-Tap Driver v2.0 for MSX (Primary & C-lib routine)
// Copyright 1994 1995 Knight's Chamber, all rights reserved.
// 	Written by Firm Knight.
//─────────────────────────────────────────────────────────────────────────────
#include "ninjatap.h"

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// INCLUDE_ASM(NTap_Dummy, "ninjatap.asm");

void NTap_Dummy()
{
	__asm
		#include "ninjatap.asm"
	__endasm;
}

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_NTap_Info[3];
u8 g_NTap_Data[8];
#if (NTAP_USE_PREVIOUS)
u8 g_NTap_Prev[8];
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Check the presence of Ninja Tap in the joystick ports.
u8 NTap_Check()
{
	__asm
		call		CKNTAP
	__endasm;
	return g_NTap_Info[1];
}

//-----------------------------------------------------------------------------
// Update the status of all joysticks whether they are connected to a Ninja Tap or directly to the port.
void NTap_Update()
{
	#if (NTAP_USE_PREVIOUS)
	loop(i, 8)
		g_NTap_Prev[i] = g_NTap_Data[i];
	#endif

	__asm
		ld			hl, #_g_NTap_Data
		call		GTNTAP
	__endasm;
}