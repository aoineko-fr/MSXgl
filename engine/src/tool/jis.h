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
//
// Return:
//   Pointer to the destination string
c8* SJIS_Convert(const c8* src, c8* dest);

// Function: SJIS_ToJIS
// Convert a Shift-JIS code into JIS code
//
// Parameters:
//   sjis - Shift-JIS code
//
// Return:
//   JIS code
u16 SJIS_ToJIS(u16 sjis);

// Function: JIS_ToSJIS
// Convert a JIS code into Shift-JIS code
//
// Parameters:
//   jis - JIS code
//
// Return:
//   Shift-JIS code
u16 JIS_ToSJIS(u16 jis);

// Function: JIS_ToKuTen
// Convert a JIS code into KuTen code
//
// Parameters:
//   jis - JIS code
//
// Return:
//   KuTen code
inline u16 JIS_ToKuTen(u16 jis) { return jis - 0x2020; }