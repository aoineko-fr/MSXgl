// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██▄▀ ▄▀██ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ██   ▀▄██ █▀█▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"
#include "vdp.h"

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

// Function: Draw_Line
// Draw a line (pixel unit)
//
// Parameters:
//   x1 - Source position X
//   y1 - Source position Y
//   x2 - Destination position X
//   y2 - Destination position Y
//   color - Draw color (format depends of the screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
void Draw_Line(UX x1, UY y1, UX x2, UY y2, u8 color, u8 op);

// Function: Draw_LineH
// Draw an horizontal line (pixel unit)
//
// Parameters:
//   x1 - Source position X
//   x2 - Destination position X
//   y - Position Y
//   color - Draw color (format depends of the screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
void Draw_LineH(UX x1, UX x2, UY y, u8 color, u8 op);

// Function: Draw_LineV
// Draw a vertical line (pixel unit)
//
// Parameters:
//   x - Position X
//   y1 - Source position Y
//   y2 - Destination position Y
//   color - Draw color (format depends of the screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
void Draw_LineV(UX x, UY y1, UY y2, u8 color, u8 op);

// Function: Draw_Box
// Draw a box (pixel unit)
//
// Parameters:
//   x1 - Source position X
//   y1 - Source position Y
//   x2 - Destination position X
//   y2 - Destination position Y
//   color - Draw color (format depends of the screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
void Draw_Box(UX x1, UY y1, UX x2, UY y2, u8 color, u8 op);

// Function: Draw_Circle
// Draw a circle (pixel unit)
//
// Parameters:
//   x - Source position
//   y - Source position
//   radius - Circle radius
//   color - Draw color (format depends of the screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
void Draw_Circle(UX x, UY y, u8 radius, u8 color, u8 op);

// Function: Draw_Point
// Draw a point
//
// Parameters:
//   x - Source position
//   y - Source position
//   color - Draw color (format depends of the screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
inline void Draw_Point(UX x, UY y, u8 color, u8 op) { VDP_CommandPSET(x, y, color, op); }