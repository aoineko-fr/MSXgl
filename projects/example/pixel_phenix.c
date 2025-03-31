// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  Pixel Phenix Logo
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"
#include "vdp.h"
#include "compress/pletter.h"

//=============================================================================
// DEFINES
//=============================================================================

// Logo animation timers
#define LOGO_START					(8)
#define LOGO_OFFSET					(33 + 16)
#define LOGO_END					(64 + 16)

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Logo data
#include "content/logo_tile.h"
#include "content/logo_sprt.h"

// Logo sprites data
const struct VDP_Sprite g_LogoSprt[] =
{
	// Set sprites data - Mask
	{ 95,  96+8, 0, COLOR_BLACK },
	{ 95, 112+8, 0, COLOR_BLACK },
	{ 95, 128+8, 0, COLOR_BLACK },
	{ 79, 104+8, 0, COLOR_BLACK },
	{ 79, 120+8, 0, COLOR_BLACK },
	{ 63, 104+8, 0, COLOR_BLACK },
	{ 63, 120+8, 0, COLOR_BLACK },
	// Logo
	{ 63, 104+8,  4, COLOR_LIGHT_RED },
	{ 63, 120+8,  8, COLOR_LIGHT_RED },
	{ 79, 104+8, 12, COLOR_LIGHT_RED },
	{ 79, 120+8, 16, COLOR_LIGHT_RED },
	{ 55, 104+8, 20, COLOR_DARK_RED },
	{ 55, 120+8, 24, COLOR_DARK_RED },
	{ 71, 104+8, 28, COLOR_WHITE },
	{ 71, 120+8, 32, COLOR_WHITE },
	{ 87, 104+8, 36, COLOR_LIGHT_YELLOW },
	{ 87, 120+8, 40, COLOR_LIGHT_YELLOW },
};

const i8 g_LogoSprtOffset[] = { 0, 0, 0, -16, -16, -32, -32 };

//=============================================================================
// MEMORY DATA
//=============================================================================

u16 g_LogoTimer;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Set up the logo and load necessary data
void Logo_Initialize()
{
	// Initialize VDP
	VDP_SetMode(VDP_MODE_SCREEN2);
	VDP_EnableDisplay(FALSE);
	VDP_SetColor(COLOR_BLACK);
	VDP_ClearVRAM();
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);

	// Load sprites pattern data
	VDP_FillVRAM_16K(0xFF, VDP_GetSpritePatternTable(), 4 * 8);
	Pletter_UnpackToVRAM(g_DataLogoSprt, VDP_GetSpritePatternTable() + 4 * 8);
	// Initialize sprite attribute table
	VDP_WriteVRAM_16K((void*)&g_LogoSprt, VDP_GetSpriteAttributeTable(), sizeof(g_LogoSprt));
	
	// Load tiles data
	u16 offset = 0;
	loop(i, 3)
	{
		Pletter_UnpackToVRAM(g_DataLogoTile_Patterns, VDP_GetPatternTable() + offset);
		Pletter_UnpackToVRAM(g_DataLogoTile_Colors, VDP_GetColorTable() + offset);
		offset += 0x800;
	}

	// Draw tiles data
	VDP_FillScreen_GM2(0x00);
	VDP_WriteLayout_GM2(g_DataLogoTileL0_Names, 13, 12, 6, 2);
	VDP_WriteLayout_GM2(g_DataLogoTileL1_Names, 14, 8, 4, 4);
	
	g_LogoTimer = 0;

	VDP_EnableDisplay(TRUE);
}

//-----------------------------------------------------------------------------
// Update the logo animation
bool Logo_Update()
{
	g_LogoTimer++;
	if ((g_LogoTimer & 0b00000011) != 0)
		return FALSE;

	u8 logoTimer = g_LogoTimer >> 2;
	if ((logoTimer >= LOGO_START) && (logoTimer < LOGO_START + LOGO_OFFSET))
	{
		u8 offset = (95 + LOGO_START) - logoTimer;
		loop(i, numberof(g_LogoSprtOffset))
			VDP_SetSpritePositionY(i, offset + g_LogoSprtOffset[i]);
	}

	return (logoTimer == LOGO_END);
}
