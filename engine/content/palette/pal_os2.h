// MSXgl palette
// by Guillaume "Aoineko" Blanchard (2022) under CC BY-SA free license
// ─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "color.h"

// Microsoft Windows and IBM OS/2 default 16-color palette
const u16 g_PaletteOS2[] =
{
#if (VDP_USE_PALETTE16)
	RGB16_From32B(0x000000), // Black
#endif
	RGB16_From32B(0x800000), // Maroon
	RGB16_From32B(0x008000), // Green
	RGB16_From32B(0x808000), // Olive
	RGB16_From32B(0x000080), // Navy
	RGB16_From32B(0x800080), // Purple
	RGB16_From32B(0x008080), // Teal
	RGB16_From32B(0xC0C0C0), // Silver
	RGB16_From32B(0x808080), // Gray
	RGB16_From32B(0xFF0000), // Red
	RGB16_From32B(0x00FF00), // Lime
	RGB16_From32B(0xFFFF00), // Yellow
	RGB16_From32B(0x0000FF), // Blue
	RGB16_From32B(0xFF00FF), // Fuchsia
	RGB16_From32B(0x00FFFF), // Aqua
	RGB16_From32B(0xFFFFFF), // White
};