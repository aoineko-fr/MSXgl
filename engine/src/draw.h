// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██▄▀ ▄▀██ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ██   ▀▄██ █▀█▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#pragma once
#include "core.h"
#include "vdp.h"

//-----------------------------------------------------------------------------
// DFINES

//-----------------------------------------------------------------------------
// FUNCTIONS

/// Draw a line (pixel unit)
void Draw_Line(UX x1, UY y1, UX x2, UY y2, u8 color, u8 op);

/// Draw an horizontal line (pixel unit)
void Draw_LineH(UX x1, UX x2, UY y, u8 color, u8 op);

/// Draw a vertical line (pixel unit)
void Draw_LineV(UX x, UY y1, UY y2, u8 color, u8 op);

/// Draw a box (pixel unit)
void Draw_Box(UX x1, UY y1, UX x2, UY y2, u8 color, u8 op);

/// Draw a circle (pixel unit)
void Draw_Circle(UX x, UY y, u8 radius, u8 color, u8 op);