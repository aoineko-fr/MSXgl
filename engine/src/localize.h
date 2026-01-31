// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// 
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

// Number of localized test entries
extern u8 g_Loc_TextNum;

// Root address of the localization data
extern u16 g_Loc_DataRoot;

// Pointer to selected language pointer
extern const c8** g_Loc_CurLang;

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: Loc_Initialize
// Initialize the localization module
//
// Parameters:
//   data - Pointer to generated localization data
//   num  - Maximum number of entry for a language (should be TEXT_MAX).
void Loc_Initialize(const void* data, u8 num);

// Function: Loc_SetLanguage
// Set the current language
//
// Parameters:
//   lang - Index of the language to selection (must be between 0 and LANG_MAX-1).
inline void Loc_SetLanguage(u8 lang) { g_Loc_CurLang = (const c8**)(g_Loc_DataRoot + g_Loc_TextNum * 2 * lang); }

// Function: Loc_GetText
// Get a given text in the current language
//
// Parameters:
//   text - Index of the text to retreive in the current language (must be between 0 and TEXT_MAX-1).
//
// Return:
//   Pointer to the zero terminated string.
inline const c8* Loc_GetText(u16 text) { return g_Loc_CurLang[text]; }