// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄  ▄▄▄ ▄▄ ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ▀█▄  ▀█▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄█▀  ▄▄█▀ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// JSX driver module
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// OPTIONS VALIDATION
//=============================================================================

//=============================================================================
// DEFINES
//=============================================================================

#define JSX_SIGNATURE				0x0F
#define JSX_MAX_INPUT				(15 + 3)

// Input buffer
// extern u8 g_JSX_InputBuffer[2][JSX_MAX_INPUT];

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: JSX_Detect
//  7 6 5 4 3 2 1 0
// –-–-–-–-–-–-–-–--
//  x x A A A A B B
//      │ │ │ │ └─┴── Number of button rows (0-3)
//      └─┴─┴─┴────── Number of axis (0-15)
u8 JSX_Detect(u8 port) __NAKED;

// Function: JSX_Read
u8 JSX_Read(u8 port, u8* buffer) __NAKED;

// // Function: JSX_GetInput
// inline u8 JSX_GetInput(u8 port, u8 idx) { return g_JSX_InputBuffer[port][idx]; }

// // Function: JSX_GetInputBuffer
// inline u8* JSX_GetInputBuffer(u8 port) { return g_JSX_InputBuffer[port]; }
