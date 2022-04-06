// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄  ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄█▀▀ ██▄▀ ▄█▀▄ ██  ██  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀█▄▄ ██   ▀█▄▀ ▀█▄ ▀█▄ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#pragma once
#include "core.h"
#include "vdp.h"

//=============================================================================
// DEFINES
//=============================================================================

extern u8  g_Scroll_Count;
extern u16 g_Scroll_OffsetX;
extern u16 g_Scroll_OffsetY;
extern u16 g_Scroll_Map;

//=============================================================================
// PROTOTYPE
//=============================================================================

// Initialize scrolling module
u8 Scroll_Initialize(u16 map);

// Set scrolling horizontal offset
void Scroll_SetOffsetH(i8 offset);

// Set scrolling vertical offset
void Scroll_SetOffsetV(i8 offset);

// Update scrolling
void Scroll_Update();