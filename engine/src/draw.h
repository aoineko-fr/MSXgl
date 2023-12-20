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

#if ((MSX_VERSION >= MSX_2) && (VDP_USE_COMMAND))

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

// Function: Draw_Line
// Draws a line (one pixel wide)
//
// Parameters:
//   x1 - Source position X
//   y1 - Source position Y
//   x2 - Destination position X
//   y2 - Destination position Y
//   color - Draw color (format depends on screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
void Draw_Line(UX x1, UY y1, UX x2, UY y2, u8 color, u8 op);

// Function: Draw_LineH
// Draws an horizontal line (one pixel wide)
//
// Parameters:
//   x1 - Source position X
//   x2 - Destination position X
//   y - Position Y
//   color - Draw color (format depends on screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
void Draw_LineH(UX x1, UX x2, UY y, u8 color, u8 op);

// Function: Draw_LineV
// Draws a vertical line (one pixel wide)
//
// Parameters:
//   x - Position X
//   y1 - Source position Y
//   y2 - Destination position Y
//   color - Draw color (format depends on screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
void Draw_LineV(UX x, UY y1, UY y2, u8 color, u8 op);

// Function: Draw_Box
// Draws a box (one pixel wide)
//
// Parameters:
//   x1 - Source position X
//   y1 - Source position Y
//   x2 - Destination position X
//   y2 - Destination position Y
//   color - Draw color (format depends on screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
void Draw_Box(UX x1, UY y1, UX x2, UY y2, u8 color, u8 op);

// Function: Draw_FillBox
// Draws a fully filled box
//
// Parameters:
//   x1 - Source position X
//   y1 - Source position Y
//   x2 - Destination position X
//   y2 - Destination position Y
//   color - Draw color (format depends on screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
void Draw_FillBox(UX x1, UY y1, UX x2, UY y2, u8 color, u8 op);

// Function: Draw_Circle
// Draws a circle (one pixel wide)
//
// Parameters:
//   x - Destination position
//   y - Destination position
//   radius - Circle radius
//   color - Draw color (format depends on screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
void Draw_Circle(UX x, UY y, u8 radius, u8 color, u8 op);

// Function: Draw_Point
// Draws a point
//
// Parameters:
//   x - Destination position
//   y - Destination position
//   color - Draw color (format depends on screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
inline void Draw_Point(UX x, UY y, u8 color, u8 op) { VDP_CommandPSET(x, y, color, op); }

#endif // ((MSX_VERSION >= MSX_2) && (VDP_USE_COMMAND))
