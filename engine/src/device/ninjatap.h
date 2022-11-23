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
#pragma once

#include "core.h"

//=============================================================================
// OPTIONS VALIDATION
//=============================================================================

#define NTAP_DRIVER_MSXGL			(1 << 0) // MSXgl custom driver (based on DM-System2 one)
#define NTAP_DRIVER_GIGAMIX			(1 << 1) // Original Gigamix's DM-System2 driver
#define NTAP_DRIVER_SHINOBI			(1 << 2) // Shinobi Tap driver by Danjovic

// NTAP_DRIVER
#ifndef NTAP_DRIVER
	#warning NTAP_DRIVER is not defined in "msxgl_config.h"! Default value will be used: NTAP_DRIVER_MSXGL
	#define NTAP_DRIVER				NTAP_DRIVER_MSXGL
#endif
#if ((NTAP_DRIVER & 0x07) == 0)
	#warning Invalid value for NTAP_DRIVER! Default value will be used: NTAP_DRIVER_MSXGL
	#undef  NTAP_DRIVER
	#define NTAP_DRIVER				NTAP_DRIVER_MSXGL
#endif

// NTAP_USE_PREVIOUS
#ifndef NTAP_USE_PREVIOUS
	#warning NTAP_USE_PREVIOUS is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define NTAP_USE_PREVIOUS		TRUE
#endif

//=============================================================================
// DEFINES
//=============================================================================

extern u8 g_NTap_Info;
extern u8 g_NTap_Buffer[2];
extern u8 g_NTap_Data[8];
#if (NTAP_USE_PREVIOUS)
extern u8 g_NTap_Prev[8];
#endif

#define NTAP_RIGHT					0b00000001
#define NTAP_LEFT					0b00000010
#define NTAP_DOWN					0b00000100
#define NTAP_UP						0b00001000
#define NTAP_START					0b00010000
#define NTAP_SELECT					0b00100000
#define NTAP_A						0b01000000
#define NTAP_B						0b10000000
// FM-Town triggers
#define NTAP_FMT_RUN				(NTAP_RIGHT + NTAP_LEFT) 
#define NTAP_FMT_SELECT				(NTAP_UP + NTAP_DOWN)

#define NTAP_NUM_MASK				0b00001111
#define NTAP_PORT1_MASK				0b00110000
#define NTAP_PORT2_MASK				0b11000000

#define NTAP_PORT1					0b00010000
#define NTAP_PORT2					0b01000000

#define NTAP_TYPE_NONE				0
#define NTAP_TYPE_NINJA				1
#define NTAP_TYPE_SHINOBI			2
#define NTAP_TYPE_UNKNOW			3

//=============================================================================
// FUNCTIONS
//=============================================================================

u8 NTap_CheckMGL();		// Modified version of DM-System 2 driver
u8 NTap_CheckDM();		// Original DM-System 2 driver
u8 NTap_CheckST();		// Shinobi Tap driver

// Function: NTap_Check
// Check the presence of Ninja Tap in the joystick ports.
//
// Return:
//   Ninja Tap information. See <NTap_GetInfo>
inline u8 NTap_Check()
{
	#if (NTAP_DRIVER & NTAP_DRIVER_MSXGL)
		return NTap_CheckMGL();
	#elif (NTAP_DRIVER & NTAP_DRIVER_GIGAMIX)
		return NTap_CheckDM();
	#elif (NTAP_DRIVER & NTAP_DRIVER_SHINOBI)
		return NTap_CheckST();
	#endif
}

// Function: NTap_GetInfo
// Get Ninja Tap information.
// <NTap_Check> function must be called first.
//
// Return:
//   Number of usable joysticks and types. See <NTap_Check>.
//   > 7	6	5	4	3	2	1	0
//   > │	│	│	│	└───┴───┴───┴── Nomber of joystick port available (2, 5 or 8)
//   > │	│	└───┴────────────────── Tap type pluged in port 1 (0=No Tap, 1=Ninja Tap, 2=Shinobi Tap)
//   > └────┴────────────────────────── Tap type pluged in port 2 (0=No Tap, 1=Ninja Tap, 2=Shinobi Tap)
inline u8 NTap_GetInfo() { return g_NTap_Info; }

// Function: NTap_GetPortNum
// Get total number of joystick port.
// <NTap_Check> function must be called first.
//
// Return:
//   Number of usable joysticks. See <NTap_Check>.
inline u8 NTap_GetPortNum() { return NTap_GetInfo() & NTAP_NUM_MASK; }

// Function: NTap_Update
// Update the status of all joysticks whether they are connected to a Ninja Tap or directly to the port.
void NTap_Update();

// Function: NTap_GetData
// Get the data of a given joystick according to its index.
// <NTap_Update> function must be called first.
//
// Parameters:
//   idx - Joystick port index.
//         For example, if the Ninja Tap is connected to port B, the first entry (index 0)
//         will be the joystick connected directly to port A, while the next (index 1) will
//         be the joystick connected to the first port of the Ninja Tap.
//
// Return:
//   State of the joystick. Bit set to 0 are triggered.
//   > 7	6	5	4	3	2	1	0
//   > │	│	│	│	│	│	│	└── Right direction
//   > │	│	│	│	│	│	└────── Left direction
//   > │	│	│	│	│	└────────── Down direction
//   > │	│	│	│	└────────────── Up direction
//   > │	│	│	└────────────────── Start bouton (for NES pad)
//   > │	│	└────────────────────── Select bouton (for NES pad)
//   > │	└────────────────────────── A bouton
//   > └─────────────────────────────── B bouton
inline u8 NTap_GetData(u8 idx) { return g_NTap_Data[idx]; }

// Function: NTap_IsPressed
// Check if the given bouton is pressed on the given joystick index
// <NTap_Update> function must be called first.
//
// Parameters:
//   idx - Joystick port index.
//   btn - Bouton value.
//         Can be one of <NTAP_RIGHT>, <NTAP_LEFT>, <NTAP_DOWN>, <NTAP_UP>, <NTAP_START>, <NTAP_SELECT>, <NTAP_A> or <NTAP_B>.
//
// Return:
//   TRUE if button is currently pressed, FALSE otherwise.
inline u8 NTap_IsPressed(u8 idx, u8 btn) { return (g_NTap_Data[idx] & btn) == 0; }

#if (NTAP_USE_PREVIOUS)
// Function: NTap_IsPushed
// Check if the given bouton has just been pushed on the given joystick index
// <NTap_Update> function must be called first.
//
// Parameters:
//   idx - Joystick port index.
//   btn - Bouton value. See <NTap_IsPressed>.
//
// Return:
//   TRUE if button has just been pushed this frame, FALSE otherwise.
inline u8 NTap_IsPushed(u8 idx, u8 btn) { return ((g_NTap_Data[idx] & btn) == 0) && ((g_NTap_Prev[idx] & btn) != 0); }

// Function: NTap_IsReleased
// Check if the given bouton has just been released on the given joystick index
// <NTap_Update> function must be called first.
//
// Parameters:
//   idx - Joystick port index.
//   btn - Bouton value. See <NTap_IsPressed>.
//
// Return:
//   TRUE if button has just been released this frame, FALSE otherwise.
inline u8 NTap_IsReleased(u8 idx, u8 btn) { return ((g_NTap_Data[idx] & btn) != 0) && ((g_NTap_Prev[idx] & btn) == 0); }
#endif