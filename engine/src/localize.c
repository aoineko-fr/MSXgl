// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// 
//─────────────────────────────────────────────────────────────────────────────
#include "localize.h"

//=============================================================================
// MEMORY DATA
//=============================================================================

// Number of localized test entries
u8         g_Loc_TextNum;

// Root address of the localization data
u16        g_Loc_DataRoot;

// Pointer to selected language pointer
const c8** g_Loc_CurLang;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize the localization module
void Loc_Initialize(const void* data, u8 ids)
{
	g_Loc_DataRoot = (u16)data;
	g_Loc_TextNum = ids;
	Loc_SetLanguage(0);
}
