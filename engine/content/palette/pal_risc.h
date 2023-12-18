// MSXgl palette
// by Guillaume "Aoineko" Blanchard (2022) under CC BY-SA free license
// ─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "color.h"

// RISC OS default palette
const u16 g_PaletteRisc[] =
{
#if (VDP_USE_PALETTE16)
	RGB16_From32B(0xFFFFFF), // White
#endif
	RGB16_From32B(0xDDDDDD), // Grey #1
	RGB16_From32B(0xBBBBBB), // Grey #2
	RGB16_From32B(0x999999), // Grey #3
	RGB16_From32B(0x777777), // Grey #4
	RGB16_From32B(0x555555), // Grey #5
	RGB16_From32B(0x333333), // Grey #6
	RGB16_From32B(0x000000), // Black
	RGB16_From32B(0x004499), // Dark blue
	RGB16_From32B(0xEEEE00), // Yellow
	RGB16_From32B(0x00CC00), // Green
	RGB16_From32B(0xDD0000), // Red
	RGB16_From32B(0xEEEEBB), // Beige
	RGB16_From32B(0x558800), // Dark green
	RGB16_From32B(0xFFBB00), // Gold/orange
	RGB16_From32B(0x00BBFF), // Light blue
};