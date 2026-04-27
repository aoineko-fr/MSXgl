// MSXgl palette
// by Guillaume "Aoineko" Blanchard (2022) under CC BY-SA free license
// ─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "color.h"

// Sweetie 16 palette by GrafxKid - https://lospec.com/palette-list/sweetie-16
const u16 g_PaletteSweetie[] =
{
#if (VDP_USE_PALETTE16)
	RGB16_From32B(0x1A1C2C), //
#endif
	RGB16_From32B(0x5D275D), //
	RGB16_From32B(0xB13E53), //
	RGB16_From32B(0xEF7D57), //
	RGB16_From32B(0xFFCD75), //
	RGB16_From32B(0xA7F070), //
	RGB16_From32B(0x38B764), //
	RGB16_From32B(0x257179), //
	RGB16_From32B(0x29366F), //
	RGB16_From32B(0x3B5DC9), //
	RGB16_From32B(0x41A6F6), //
	RGB16_From32B(0x73EFF7), //
	RGB16_From32B(0xF4F4F4), //
	RGB16_From32B(0x94B0C2), //
	RGB16_From32B(0x566C86), //
	RGB16_From32B(0x333C57), //
};
