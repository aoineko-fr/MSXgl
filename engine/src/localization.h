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

extern u8 g_Loc_TextIDs;
extern const void* g_Loc_Data;
extern const c8** g_Loc_CurLang;

void Loc_SetLangage(u8 langId);

//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

// Function: 
// Initialize the localization module
inline void Loc_Initialize(const void* data, u8 ids) { g_Loc_Data = data; g_Loc_TextIDs = ids; Loc_SetLangage(0); }

// Function: 
// Set the current langage
inline void Loc_SetLangage(u8 langId) { g_Loc_CurLang = (const c8*)(g_Loc_Data) + (g_Loc_TextIDs * 2 * langId); }

// Function: 
// Get a given text in the current langage
inline const c8* Loc_GetText(u8 textId) { return g_Loc_CurLang[textId]; }