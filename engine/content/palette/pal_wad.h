// MSXgl palette
// by Guillaume "Aoineko" Blanchard (2022) under CC BY-SA free license
// ─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "color.h"

// Wad's palette - https://alumni.media.mit.edu/~wad/color/palette.html
const u16 g_PaletteWad[] =
{
#if (VDP_USE_PALETTE16)
	RGB16_From8B(0, 0, 0),       // Black
#endif
	RGB16_From8B(87, 87, 87),    // Dark gray
	RGB16_From8B(173, 35, 35),   // Red
	RGB16_From8B(42, 75, 215),   // Blue
	RGB16_From8B(29, 105, 20),   // Green
	RGB16_From8B(129, 74, 25),   // Brown
	RGB16_From8B(129, 38, 192),  // Purple
	RGB16_From8B(160, 160, 160), // Light gray
	RGB16_From8B(129, 197, 122), // Light green
	RGB16_From8B(157, 175, 255), // Light blue
	RGB16_From8B(41, 208, 208),  // Cyan
	RGB16_From8B(255, 146, 51),  // Orange
	RGB16_From8B(255, 238, 51),  // Yellow
	RGB16_From8B(233, 222, 187), // Tan
	RGB16_From8B(255, 205, 243), // Pink
	RGB16_From8B(255, 255, 255), // White
};