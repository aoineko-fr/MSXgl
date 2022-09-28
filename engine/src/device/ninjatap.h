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
#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

#define NTAP_USE_PREVIOUS			0

extern u8 g_NTap_Info[3];
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

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: NTap_Check
// Check the presence of Ninja Tap in the joystick ports.
//
// Return:
//   Number of usable joysticks
//   - 2: No Ninja Tap detected (only 1 joystick in each port)
//   - 5: 1 Ninja Tap detected (4 joysticks on Ninja Tap and 1 in the other port)
//   - 8: 2 Ninja Tap detected (4 joysticks on each Ninja Tap)
u8 NTap_Check();

// Function: NTap_GetInfo
// Get Ninja Tap information.
// <NTap_Check> function must be called first.
//
// Return:
//   Number of usable joysticks. See <NTap_Check>.
inline u8 NTap_GetInfo() { return g_NTap_Info[1]; }

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