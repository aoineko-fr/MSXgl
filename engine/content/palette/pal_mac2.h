// MSXgl palette
// by Guillaume "Aoineko" Blanchard (2022) under CC BY-SA free license
// ─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "color.h"

// Macintosh II palette
const u16 g_PaletteMac2[] =
{
#if (VDP_USE_PALETTE16)
	RGB16_From32B(0xFFFFFF), // White
#endif
	RGB16_From32B(0xFBF305), // Yellow
	RGB16_From32B(0xFF6403), // Orange
	RGB16_From32B(0xDD0907), // Red
	RGB16_From32B(0xF20884), // Magenta
	RGB16_From32B(0x4700A5), // Purple
	RGB16_From32B(0x0000D3), // Blue
	RGB16_From32B(0x02ABEA), // Cyan
	RGB16_From32B(0x1FB714), // Green
	RGB16_From32B(0x006412), // Dark green
	RGB16_From32B(0x562C05), // Brown
	RGB16_From32B(0x90713A), // Tan
	RGB16_From32B(0xC0C0C0), // Light grey
	RGB16_From32B(0x818181), // Medium grey
	RGB16_From32B(0x404040), // Dark grey
	RGB16_From32B(0x000000), // Black
};