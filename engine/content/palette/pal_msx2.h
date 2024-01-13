// MSXgl palette
// by Guillaume "Aoineko" Blanchard (2022) under CC BY-SA free license
// ─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "color.h"

// Default MSX2 (V9938) palette
const u16 g_PaletteMSX2[] =
{
#if (VDP_USE_PALETTE16)
	RGB16(0, 0, 0), // Transparent
#endif
	RGB16(0, 0, 0), // Black
	RGB16(1, 6, 1), // Medium green
	RGB16(3, 7, 3), // Light green
	RGB16(1, 1, 7), // Dark blue
	RGB16(2, 3, 7), // Light blue
	RGB16(5, 1, 1), // Dark red
	RGB16(2, 6, 7), // Cyan
	RGB16(7, 1, 1), // Medium red
	RGB16(7, 3, 3), // Light red
	RGB16(6, 6, 1), // Dark yellow
	RGB16(6, 6, 4), // Light yellow
	RGB16(1, 4, 1), // Dark green
	RGB16(6, 2, 5), // Magenta
	RGB16(5, 5, 5), // Gray
	RGB16(7, 7, 7)  // White
};