// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄           ▄▄               ▄▄▄▄ ▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  █ ██  ██▀ ██▀ ▄███ ▄█▄█     ██  ▄  ▄█▄█ ▄███ ██▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀  ▀██ ▄██  ▀█▄ ▀█▄▄ ██ █     ██  ██ ██ █ ▀█▄▄ ██  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘        ▀▀                                
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// MSX turbo R's system timer
//─────────────────────────────────────────────────────────────────────────────
// Source: MSX Datapack - 7.1 System Timer
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"
#include "system_port.h"

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// FUNCTIONS
//=============================================================================
#if (MSX_VERSION == MSX_TR)

// Function: Timer_Reset
// Reset the system timer
// Note: Never reset the timer from the interrupt routine if there is any chance that the timer is used by the background program
inline void Timer_Reset() { io_Timer_Low = 0; }

// Function: Timer_GetLow
// Get the low byte of the system timer
//
// Returns:
//   Low byte of the system timer (1 unit = 3.911 μs)
inline u8 Timer_GetLow() { return io_Timer_Low; }

// Function: Timer_GetHigh
// Get the high byte of the system timer
//
// Returns:
//   High byte of the system timer (1 unit = 1.001 ms)
inline u8 Timer_GetHigh() { return io_Timer_High; }

// Function: Timer_Get
// Get the full value of the system timer
//
// Returns:
//   Full value of the system timer (1 unit = 3.911 μs)
inline u16 Timer_Get() { return (io_Timer_High << 8) + io_Timer_Low; }

#endif // (MSX_VERSION == MSX_TR)