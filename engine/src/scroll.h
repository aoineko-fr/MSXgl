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
// Initialize scrolling module.
//
// Parameters:
//   map - Address of the tilemap used to render the scrolling background. 
//
// Return:
//   First free sprite index (Scroll module reserve several sprites if mask option is activted).
u8 Scroll_Initialize(u16 map);

#if (SCROLL_HORIZONTAL)
// Function: Scroll_SetOffsetH
// Set scrolling horizontal offset
//
// Parameters:
//   offset - Horizontal scrolling offset (in pixels)
void Scroll_SetOffsetH(i8 offset);
#endif

#if (SCROLL_VERTICAL)
// Function: Scroll_SetOffsetV
// Set scrolling vertical offset
//
// Parameters:
//   offset - Vertical scrolling offset (in pixels)
void Scroll_SetOffsetV(i8 offset);
#endif

#if ((SCROLL_ADJUST) && (SCROLL_ADJUST_SPLIT))
// Function: Scroll_HBlankAdjust
// Adjust screen offset. Function to be called in the H-Blank handler
//
// Parameters:
//   adjust - Ajust step
//            0: Frame start
//            1: Scrolling adjust
//            2: No more adjust
void Scroll_HBlankAdjust(u8 adjust);
#endif

// Function: Scroll_Update
// Update scrolling module
void Scroll_Update();