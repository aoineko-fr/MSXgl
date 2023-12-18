// MSXgl palette
// by Guillaume "Aoineko" Blanchard (2022) under CC BY-SA free license
// ─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "color.h"

// Epoch Super Cassette Vision palette
const u16 g_PaletteSCV[] =
{
#if (VDP_USE_PALETTE16)
	RGB16_From32B(0x000000), //
#endif
	RGB16_From32B(0xFF0000), //
	RGB16_From32B(0xFFA100), //
	RGB16_From32B(0xFFA09F), //
	RGB16_From32B(0xFFFF00), //
	RGB16_From32B(0xA3A000), //
	RGB16_From32B(0x00A100), //
	RGB16_From32B(0x00FF00), //
	RGB16_From32B(0xA0FF9D), //
	RGB16_From32B(0x00009B), //
	RGB16_From32B(0x0000FF), //
	RGB16_From32B(0xA200FF), //
	RGB16_From32B(0xFF00FF), //
	RGB16_From32B(0x00FFFF), //
	RGB16_From32B(0xA2A19F), //
	RGB16_From32B(0xFFFFFF), //
};