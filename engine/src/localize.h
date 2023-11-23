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

#include "core.h"

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

extern u8 g_Loc_TextNum;
extern u16 g_Loc_DataRoot;
extern const c8** g_Loc_CurLang;

//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

// Function: 
// Initialize the localization module
void Loc_Initialize(const void* data, u8 ids);

// Function: 
// Set the current langage
inline void Loc_SetLangage(u8 langId) { g_Loc_CurLang = (const c8**)(g_Loc_DataRoot + g_Loc_TextNum * 2 * langId); }

// Function: 
// Get a given text in the current langage
inline const c8* Loc_GetText(u8 textId) { return g_Loc_CurLang[textId]; }