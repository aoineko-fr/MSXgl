// MSXgl palette
// by Guillaume "Aoineko" Blanchard (2022) under CC BY-SA free license
// ─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "color.h"

// Commodore C64 palette
const u16 g_PaletteC64[] =
{
#if (VDP_USE_PALETTE16)
	RGB16_From32B(0x000000), // Black
#endif
	RGB16_From32B(0xFFFFFF), // white
	RGB16_From32B(0x883932), // red	
	RGB16_From32B(0x67B6BD), // cyan
	RGB16_From32B(0x8B3F96), // purple
	RGB16_From32B(0x55A049), // green
	RGB16_From32B(0x40318D), // blue
	RGB16_From32B(0xBFCE72), // yellow
	RGB16_From32B(0x8B5429), // orange
	RGB16_From32B(0x574200), // brown
	RGB16_From32B(0xB86962), // light red
	RGB16_From32B(0x505050), // dark grey
	RGB16_From32B(0x787878), // grey
	RGB16_From32B(0x94E089), // light green
	RGB16_From32B(0x7869C4), // light blue
	RGB16_From32B(0x9F9F9F), // light grey
};