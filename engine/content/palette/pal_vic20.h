// MSXgl palette
// by Guillaume "Aoineko" Blanchard (2022) under CC BY-SA free license
// ─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "color.h"

// VIC-20 palette
const u16 g_PaletteVIC20[] =
{
#if (VDP_USE_PALETTE16)
	RGB16_From32B(0x000000), // Black
#endif
	RGB16_From32B(0xFFFFFF), // White
	RGB16_From32B(0x782922), // Red
	RGB16_From32B(0x87D6DD), // Cyan
	RGB16_From32B(0xAA5FB6), // Purple
	RGB16_From32B(0x55A049), // Green
	RGB16_From32B(0x40318D), // Blue
	RGB16_From32B(0xBFCE72), // Yellow
	RGB16_From32B(0xAA7449), // Orange
	RGB16_From32B(0xEAB489), // Light orange
	RGB16_From32B(0xB86962), // Light red
	RGB16_From32B(0xC7FFFF), // Light cyan
	RGB16_From32B(0xEA9FF6), // Light purple
	RGB16_From32B(0x94E089), // Light green
	RGB16_From32B(0x8071CC), // Light blue
	RGB16_From32B(0xFFFFB2), // Light yellow
};