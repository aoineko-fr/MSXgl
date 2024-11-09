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

#include "core.h"
#include "input.h"

// Paddle state for both joystick port
//
//: 5 4 3 2 1 0 9 8   7 6 5 4 3 2 1 0 
//:------------------------------------
//: D 0 0 0 0 0 B C   C C C C C C C C
//: │           │ └───┴─┴─┴─┴─┴─┴─┴─┴── 9-bit counter
//: │           └────────────────────── Button state (0: pressed, 1: released)
//: └────────────────────────────────── Disconnect state (0: connected, 1: disconnected)
extern u16 g_PaddleStates[2];

// Function: Paddle_Update
// Update both joystick port and store paddles information
void Paddle_Update();

// Function: Paddle_IsConnected
// Check if a paddle is connected to the given port
//
// Parameters:
//   port - Index of the port to check (0: Port A, 1: Port B)
//
// Returns:
//   TRUE if a paddle is connected to that port
inline bool Paddle_IsConnected(u8 port) { return (g_PaddleStates[port] & 0x8000) == 0; }

// Function: Paddle_GetAngle
// Get the rotation angle of a paddle on the given port
//
// Parameters:
//   port - Index of the port to check (0: Port A, 1: Port B)
//
// Returns:
//   Paddle angle (theorical value goes from 0 to 510, but valid range is around 110 to 380)
inline u16 Paddle_GetAngle(u8 port) { return g_PaddleStates[port] & 0x01FF; }

// Function: Paddle_IsButtonPressed
// Check if the given paddle's button is pressed
//
// Parameters:
//   port - Index of the port to check (0: Port A, 1: Port B)
//
// Returns:
//   TRUE if the given paddle's button is pressed
inline bool Paddle_IsButtonPressed(u8 port) { return (g_PaddleStates[port] & 0x0200) == 0; }

#if (PADDLE_USE_CALIB)

extern i16 g_PaddleOffset[2];

// Function: Paddle_SetCalibration
// Calibrate the given paddle
//
// Parameters:
//   port - Index of the port to check (0: Port A, 1: Port B)
//   min - Minimum recoreded value
//   max - Maximum recoreded value
inline void Paddle_SetCalibration(u8 port, u16 min, u16 max) { g_PaddleOffset[port] = min + (max - min) / 2 - 128; }

// Function: Paddle_GetCalibratedAngle
// Get the callibrated rotation angle of a paddle on the given port
//
// Parameters:
//   port - Index of the port to check (0: Port A, 1: Port B)
//
// Returns:
//   Paddle angle (from 0 to 255)
u8 Paddle_GetCalibratedAngle(u8 port);

#endif // (PADDLE_USE_CALIB)