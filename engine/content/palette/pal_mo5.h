// MSXgl palette
// by Guillaume "Aoineko" Blanchard (2022) under CC BY-SA free license
// ─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "color.h"

// Thomson MO5 hardware palette
const u16 g_PaletteMO5[] =
{
#if (VDP_USE_PALETTE16)
	RGB16_From32B(0x000000), // Black
#endif
	RGB16_From32B(0xFF5555), // Red
	RGB16_From32B(0x00FF00), // Green
	RGB16_From32B(0xFFFF00), // Yellow
	RGB16_From32B(0x5555FF), // Blue
	RGB16_From32B(0xFF00FF), // Magenta
	RGB16_From32B(0x55FFFF), // Cyan
	RGB16_From32B(0xFFFFFF), // White
	RGB16_From32B(0xAAAAAA), // Gray
	RGB16_From32B(0xFFAAAA), // Rose
	RGB16_From32B(0xAAFFAA), // Light Green
	RGB16_From32B(0xFFFFAA), // Light Yellow
	RGB16_From32B(0x55AAFF), // Light Blue
	RGB16_From32B(0xFFAAFF), // Parma Pink
	RGB16_From32B(0xAAFFFF), // Light Cyan
	RGB16_From32B(0xFFAA55), // Orange
};