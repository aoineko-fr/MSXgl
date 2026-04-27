// MSXgl palette
// by Guillaume "Aoineko" Blanchard (2022) under CC BY-SA free license
// ─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "color.h"

// DawnBringer's 16 color palette (v1.0)
const u16 g_PaletteDB16[] =
{
#if (VDP_USE_PALETTE16)
	RGB16(1, 0, 1), // very_dark_violet
#endif
	RGB16(2, 1, 1), // shadowy_lavender
	RGB16(1, 1, 3), // flint
	RGB16(2, 2, 2), // regent_grey
	RGB16(4, 2, 1), // peppermint
	RGB16(1, 3, 1), // red_earth
	RGB16(6, 2, 2), // root_beer
	RGB16(3, 3, 3), // faded_red
	RGB16(2, 3, 6), // bronze
	RGB16(6, 3, 1), // birthday_suit
	RGB16(4, 4, 4), // banana
	RGB16(3, 5, 1), // deep_koamaru
	RGB16(6, 5, 4), // indigo
	RGB16(3, 5, 5), // cadet_blue
	RGB16(6, 6, 3), // hunter_green
	RGB16(6, 7, 6), // leaf
};