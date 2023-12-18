// MSXgl palette
// by Guillaume "Aoineko" Blanchard (2022) under CC BY-SA free license
// ─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "color.h"

// Mattel Electronics Intellivision palette
const u16 g_PaletteIV[] =
{
#if (VDP_USE_PALETTE16)
	RGB16_From32B(0x0C0005), // Black
#endif
	RGB16_From32B(0x002DFF), // Blue
	RGB16_From32B(0xFF3E00), // Red
	RGB16_From32B(0xC9D464), // Tan
	RGB16_From32B(0x00780F), // Grass Green
	RGB16_From32B(0x00A720), // Green
	RGB16_From32B(0xFAEA27), // Yellow
	RGB16_From32B(0xFFFCFF), // White
	RGB16_From32B(0xA7A8A8), // Gray
	RGB16_From32B(0x5ACBFF), // Cyan
	RGB16_From32B(0xFFA600), // Orange
	RGB16_From32B(0x3C5800), // Brown
	RGB16_From32B(0xFF3276), // Magenta
	RGB16_From32B(0xBD95FF), // Light Blue
	RGB16_From32B(0x6CCD30), // Yellow Green
	RGB16_From32B(0xC81A7D), // Purple
};