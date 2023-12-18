// MSXgl palette
// by Guillaume "Aoineko" Blanchard (2022) under CC BY-SA free license
// ─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "color.h"

// EGA Palette
const u16 g_PaletteEGA[] =
{
#if (VDP_USE_PALETTE16)
	RGB16(0, 0, 0), // Black
#endif
	RGB16(0, 0, 5), // Blue
	RGB16(0, 5, 0), // Green
	RGB16(0, 5, 5), // Cyan
	RGB16(5, 0, 0), // Red
	RGB16(5, 0, 5), // Magenta
	RGB16(5, 2, 0), // Brown
	RGB16(5, 5, 5), // White / light gray
	RGB16(2, 2, 2), // Dark gray / bright black
	RGB16(2, 2, 7), // Bright Blue
	RGB16(2, 7, 2), // Bright green
	RGB16(2, 7, 7), // Bright cyan
	RGB16(7, 2, 2), // Bright red
	RGB16(7, 2, 7), // Bright magenta
	RGB16(7, 7, 2), // Bright yellow
	RGB16(7, 7, 7), // Bright white
};