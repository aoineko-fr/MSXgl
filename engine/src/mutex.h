// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

extern u8 g_Mutex; //< @note Must be declared somewhere in the application code

#define MUTEX_DATA() u8 g_Mutex

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: Mutex_Init
// Initialize mutex
inline void Mutex_Init() { g_Mutex = 0; }

// Function: Mutex_Lock
// Lock the given mutex
//
// Parameters:
//   mutex - Mutex index (0-7)
inline void Mutex_Lock(u8 mutex) { g_Mutex |= (1 << mutex); }

// Function: Mutex_Release
// Release the given mutex
//
// Parameters:
//   mutex - Mutex index (0-7)
inline void Mutex_Release(u8 mutex) { g_Mutex &= ~(1 << mutex); }

// Function: Mutex_Wait
// Wait for mutex release
//
// Parameters:
//   mutex - Mutex index (0-7)
inline void Mutex_Wait(u8 mutex) { while ((g_Mutex & (1 << mutex)) != 0); }

// Function: Mutex_Gate
// Gate for mutex
//
// Parameters:
//   mutex - Mutex index (0-7)
inline bool Mutex_Gate(u8 mutex) { return ((g_Mutex & (1 << mutex)) == 0); }