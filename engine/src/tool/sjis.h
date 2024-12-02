// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄           ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ▄█▀▄ ▄█▀▄ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │   ██  ▀█▄▀ ▀█▄▀ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// SJIS converter module
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: SJIS_Convert
// Convert a Shift-JIS text into MSX font format
//
// Parameters:
//   src - Source string in Shift-JIS format (Zero terminated)
//   dest - Destination string (Zero terminated). Buffer must be big enough to contain the resulting string
c8* SJIS_Convert(const c8* src, c8* dest);