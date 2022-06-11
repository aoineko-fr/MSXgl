// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄  ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄█▀▀ ██▄▀ ▄█▀▄ ██  ██  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀█▄▄ ██   ▀█▄▀ ▀█▄ ▀█▄ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once
#include "core.h"
#include "vdp.h"

//=============================================================================
// DEFINES
//=============================================================================

// Address of the source map data
extern u16 g_Scroll_Map;

#if (SCROLL_HORIZONTAL)
// Horizontal offset (in pixel)
extern u16 g_Scroll_OffsetX;
// Horizontal offset (in tiles)
extern u8 g_Scroll_TileX;
#endif

#if (SCROLL_VERTICAL)
// Vertical offset (in pixel)
extern u16 g_Scroll_OffsetY;
// Vertical offset (in tiles)
extern u8 g_Scroll_TileY;
#endif

#if (SCROLL_ADJUST)
extern u8 g_Scroll_Adjust;
#endif

//=============================================================================
// FUNCTIONS PROTOTYPE
//=============================================================================

// Function: Scroll_Initialize
// Initialize scrolling module
u8 Scroll_Initialize(u16 map);

#if (SCROLL_HORIZONTAL)
// Function: Scroll_SetOffsetH
// Set scrolling horizontal offset
void Scroll_SetOffsetH(i8 offset);
#endif

#if (SCROLL_VERTICAL)
// Function: Scroll_SetOffsetV
// Set scrolling vertical offset
void Scroll_SetOffsetV(i8 offset);
#endif

#if ((SCROLL_ADJUST) && (SCROLL_ADJUST_SPLIT))
// Function: Scroll_HBlankAdjust
// Adjust screen offset
void Scroll_HBlankAdjust(u8 adjust);
#endif

// Function: Scroll_Update
// Update scrolling
void Scroll_Update();