// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄             ▄  ▄   
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ▄▀██ ██▀▄   ▄▄ ▄   
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀▄██ ██ █ ▄ ██ ██  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                  ▀▀                                              
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Kanji ROM handling module
//─────────────────────────────────────────────────────────────────────────────
//  MRC wiki
//     https://www.msx.org/wiki/Category:Kanji-ROM
//     https://www.msx.org/wiki/Kanji_display
//     https://www.msx.org/wiki/KuTen_-_JIS_-_SJIS_Code_Conversion_Tables
//   MSX Datapack - 6.4 MSX Kanji Driver
//     https://ngs.no.coocan.jp/doc/wiki.cgi/datapack?page=6%BE%CF+%C6%E2%C9%F4%A5%EB%A1%BC%A5%C1%A5%F3
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// VALIDATION
//=============================================================================

// KANJI_USE_DETECT
#ifndef KANJI_USE_DETECT
	#warning KANJI_USE_DETECT is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define KANJI_USE_DETECT		TRUE
#endif

// KANJI_USE_LEVEL2
#ifndef KANJI_USE_LEVEL2
	#warning KANJI_USE_LEVEL2 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define KANJI_USE_LEVEL2		TRUE
#endif

//=============================================================================
// DEFINES
//=============================================================================

enum KANJI_LEVEL
{
	KANJI_LEVEL_NONE = 0,
	KANJI_LEVEL_1,
	KANJI_LEVEL_2,
};

//=============================================================================
// FUNCTIONS
//=============================================================================

//.............................................................................
// Group: Kanji-ROM helper functions

// Function: Kanji_JIStoKuTen
// Convert a JIS to KuTen code
//
// Parameters:
//   jis - JIS code to convert
//
// Returns:
//   Corresponding KuTen code
inline u16 Kanji_JIStoKuTen(u16 jis) { return jis - 0x2020; }

// Function: Kanji_KuTenToIndex
// Convert a KuTen code to level 1 kanji index
//
// Parameters:
//   kuten - KuTen code to convert
//
// Returns:
//   Corresponding kanji index
inline u16 Kanji_KuTenToIndex(u16 kuten) { return ((kuten >> 8) < 16) ? (kuten >> 8) * 96 + (kuten & 0xFF) : (kuten >> 8) * 96 + (kuten & 0xFF) - 512; }

#if (KANJI_USE_LEVEL2)

// Function: Kanji_GetFromIndexL1
// See <Kanji_KuTenToIndex>
// Note: Need KANJI_USE_LEVEL2 option to be TRUE
inline u16 Kanji_KuTenToIndexL1(u16 kuten) { return Kanji_KuTenToIndex(kuten); }

// Function: Kanji_KuTenToIndexL2
// Convert a KuTen code to level 2 kanji index
// Note: Need KANJI_USE_LEVEL2 option to be TRUE
//
// Parameters:
//   kuten - KuTen code to convert
//
// Returns:
//   Corresponding kanji index
inline u16 Kanji_KuTenToIndexL2(u16 kuten) { return (kuten >> 8) * 96 + (kuten & 0xFF); }

#endif // (KANJI_USE_LEVEL2)

#if (KANJI_USE_DETECT)

// Function: Kanji_DetectLevel
// Get Kanji-ROM level supported by the current machine
//
// Returns:
//   Detected Kanji-ROM level (KANJI_LEVEL_NONE, KANJI_LEVEL_1 or KANJI_LEVEL_2)
u8 Kanji_DetectLevel();

#endif // (KANJI_USE_DETECT)


//.............................................................................
// Group: Kanji-ROM access for level 1 kanji

// Function: Kanji_GetFromIndex
// Get bitmap of the given kanji from its index in the Kanji-ROM
//
// Parameters:
//   buf   - Pointer to the buffer where the bitmap will be stored (32 bytes)
//   index - Kanji's index in the Kanji-ROM
void Kanji_GetFromIndex(u8* buf, u16 index);

// Function: Kanji_GetFromKuTen
// Get bitmap of the given kanji from its KuTen code
//
// Parameters:
//   buf   - Pointer to the buffer where the bitmap will be stored (32 bytes)
//   kuten - Kanji's KuTen code
inline void Kanji_GetFromKuTen(u8* buf, u16 kuten) { Kanji_GetFromIndex(buf, Kanji_KuTenToIndex(kuten)); }

// Function: Kanji_GetFromJIS
// Get bitmap of the given kanji from its JIS code
//
// Parameters:
//   buf - Pointer to the buffer where the bitmap will be stored (32 bytes)
//   JIS - Kanji's JIS code
inline void Kanji_GetFromJIS(u8* buf, u16 jis) { Kanji_GetFromKuTen(buf, Kanji_JIStoKuTen(jis)); }

#if (KANJI_USE_LEVEL2)

// Function: Kanji_GetFromIndexL1
// See <Kanji_GetFromIndex>
// Note: Need KANJI_USE_LEVEL2 option to be TRUE
inline void Kanji_GetFromIndexL1(u8* buf, u16 index) { Kanji_GetFromIndex(buf, index); }

// Function: Kanji_GetFromKuTenL1
// See <Kanji_GetFromKuTen>
// Note: Need KANJI_USE_LEVEL2 option to be TRUE
inline void Kanji_GetFromKuTenL1(u8* buf, u16 kuten) { Kanji_GetFromKuTen(buf, kuten); }

// Function: Kanji_GetFromJISL1
// See <Kanji_GetFromJIS>
// Note: Need KANJI_USE_LEVEL2 option to be TRUE
inline void Kanji_GetFromJISL1(u8* buf, u16 jis) { Kanji_GetFromJIS(buf, jis); }

#endif

//.............................................................................
// Group: Kanji-ROM access for level 2 kanji

#if (KANJI_USE_LEVEL2)

// Function: Kanji_GetFromIndexL2
// Get bitmap of the given level 2 kanji from its index in the Kanji-ROM
// Note: Need KANJI_USE_LEVEL2 option to be TRUE
//
// Parameters:
//   buf   - Pointer to the buffer where the bitmap will be stored (32 bytes)
//   index - The kanji index in KuTen format
void Kanji_GetFromIndexL2(u8* buf, u16 index);

// Function: Kanji_GetFromKuTenL2
// See <Kanji_GetFromKuTen>
// Note: Need KANJI_USE_LEVEL2 option to be TRUE
inline void Kanji_GetFromKuTenL2(u8* buf, u16 kuten) { Kanji_GetFromIndexL2(buf, Kanji_KuTenToIndexL2(kuten)); }

// Function: Kanji_GetFromJISL2
// See <Kanji_GetFromJIS>
// Note: Need KANJI_USE_LEVEL2 option to be TRUE
inline void Kanji_GetFromJISL2(u8* buf, u16 jis) { Kanji_GetFromKuTenL2(buf, Kanji_JIStoKuTen(jis)); }

#endif // (KANJI_USE_LEVEL2)

