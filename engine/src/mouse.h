// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ██▀▄ ██▀▄ ██ █ ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄██▄ ██ █ ██▀  ▀█▄█ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘            ▀▀  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// User input handler using direct access to ports
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"

//=============================================================================
// Group: Mouse
// Direct access to mouse
//=============================================================================

#define MOUSE_PORT_1				INPUT_PORT1_HIGH
#define MOUSE_PORT_2				INPUT_PORT2_HIGH

#define MOUSE_NOTFOUND				0xFF

#define MOUSE_BOUTON_1				0b00010000
#define MOUSE_BOUTON_2				0b00100000
#define MOUSE_BOUTON_LEFT			MOUSE_BOUTON_1
#define MOUSE_BOUTON_RIGHT			MOUSE_BOUTON_2

// Mouse sensitivity enumeration
enum MOUSE_SPEED
{
	MOUSE_SPEED_LOWEST  = 16, // [-8:7]
	MOUSE_SPEED_LOW     = 8,  // [-16:15]
	MOUSE_SPEED_MEDIUM  = 4,  // [-32:31]
	MOUSE_SPEED_HIGH    = 2,  // [-64:63]
	MOUSE_SPEED_HIGHEST = 1,  // [-127:127]
	MOUSE_SPEED_DEFAULT = MOUSE_SPEED_MEDIUM,
};

// Mouse state structure (don't change parameter order)
typedef struct Mouse_State
{
	u8			Buttons;
	i8			dX;
	i8			dY;
	u8			PrevButtons;
} Mouse_State;

// Function: Mouse_Read
// Gets current mouse information
//
// Parameters:
//   port - Port to check (MOUSE_PORT_1 or MOUSE_PORT_2)
//   data - Output mouse state
void Mouse_Read(u8 port, Mouse_State* data);

// Function: Mouse_GetOffsetX
// Gets current mouse X offset
//
// Parameters:
//   data - Current mouse state (from Mouse_Read)
//
// Return:
//   Mouse X offset
inline i8 Mouse_GetOffsetX(Mouse_State* data) { return -data->dX; }

// Function: Mouse_GetOffsetY
// Gets current mouse Y offset
//
// Parameters:
//   data - Current mouse state (from Mouse_Read)
//
// Return:
//   Mouse Y offset
inline i8 Mouse_GetOffsetY(Mouse_State* data) { return -data->dY; }

// Function: Mouse_GetAdjustedOffsetX
// Gets current mouse X offset adjusted by speed
//
// Parameters:
//   data - Current mouse state (from Mouse_Read)
//   spd - Mouse speed (MOUSE_SPEED_LOWEST, MOUSE_SPEED_LOW, MOUSE_SPEED_MEDIUM, MOUSE_SPEED_HIGH, MOUSE_SPEED_HIGHEST)
//
// Return:
//   Mouse X offset
inline i8 Mouse_GetAdjustedOffsetX(Mouse_State* data, u8 spd) { return -data->dX / spd; }

// Function: Mouse_GetAdjustedOffsetY
// Gets current mouse Y offset adjusted by speed
//
// Parameters:
//   data - Current mouse state (from Mouse_Read)
//   spd - Mouse speed (MOUSE_SPEED_LOWEST, MOUSE_SPEED_LOW, MOUSE_SPEED_MEDIUM, MOUSE_SPEED_HIGH, MOUSE_SPEED_HIGHEST)
//
// Return:
//   Mouse Y offset
inline i8 Mouse_GetAdjustedOffsetY(Mouse_State* data, u8 spd) { return -data->dY / spd; }

// Function: Mouse_IsButtonPress
// Check if mouse button is pressed
//
// Parameters:
//   data - Current mouse state (from Mouse_Read)
//   btn - Mouse button to check (MOUSE_BOUTON_LEFT, MOUSE_BOUTON_RIGHT)
//
// Return:
//   FALSE is not pressed
inline bool Mouse_IsButtonPress(Mouse_State* data, u8 btn) { return (data->Buttons & btn) == 0; }

// Function: Mouse_IsButtonClick
// Check if mouse button is clicked
//
// Parameters:
//   data - Current mouse state (from Mouse_Read)
//   btn - Mouse button to check (MOUSE_BOUTON_LEFT, MOUSE_BOUTON_RIGHT)
//
// Return:
//   FALSE is not clicked
inline bool Mouse_IsButtonClick(Mouse_State* data, u8 btn) { return ((data->Buttons & btn) == 0) && ((data->PrevButtons & btn) != 0); }