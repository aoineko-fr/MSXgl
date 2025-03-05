 // ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Example game
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

#define LOGO_START					8
#define LOGO_OFFSET					33+16
#define LOGO_END					64+16

void Pletter_LoadGM2(void*, u16 dst);

bool Logo_Skip();

extern u8 g_FrameCount;
extern u16 g_StateTimer;

//=============================================================================
// READ-ONLY DATA
//=============================================================================

#include "content/logo_tile.h"
#include "content/logo_sprt.h"

const struct VDP_Sprite g_LogoSprt[] =
{
	// Set sprites data - Mask
	{ 95,  96, 0, COLOR_BLACK },
	{ 95, 112, 0, COLOR_BLACK },
	{ 95, 128, 0, COLOR_BLACK },
	{ 79, 104, 0, COLOR_BLACK },
	{ 79, 120, 0, COLOR_BLACK },
	{ 63, 104, 0, COLOR_BLACK },
	{ 63, 120, 0, COLOR_BLACK },
	// Logo
	{ 63, 104,  4, COLOR_LIGHT_RED },
	{ 63, 120,  8, COLOR_LIGHT_RED },
	{ 79, 104, 12, COLOR_LIGHT_RED },
	{ 79, 120, 16, COLOR_LIGHT_RED },
	{ 55, 104, 20, COLOR_DARK_RED },
	{ 55, 120, 24, COLOR_DARK_RED },
	{ 71, 104, 28, COLOR_WHITE },
	{ 71, 120, 32, COLOR_WHITE },
	{ 87, 104, 36, COLOR_LIGHT_YELLOW },
	{ 87, 120, 40, COLOR_LIGHT_YELLOW },
};

//=============================================================================
// MEMORY DATA
//=============================================================================

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void Logo_Initialize()
{
	// Initialize VDP
	VDP_SetMode(VDP_MODE_SCREEN2);
	VDP_EnableDisplay(FALSE);
	VDP_SetColor(COLOR_BLACK);
	VDP_ClearVRAM();

	// Load sprites data
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);
	Pletter_UnpackToVRAM(g_DataLogoSprt, g_SpritePatternLow + 4 * 8);
	VDP_FillVRAM_16K(0xFF, g_SpritePatternLow, 4 * 8);
	VDP_WriteVRAM_16K((void*)&g_LogoSprt, g_SpriteAttributeLow, 4 * numberof(g_LogoSprt));
	
	// Load tiles data
	Pletter_LoadGM2(g_DataLogoTile_Patterns, g_ScreenPatternLow);
	Pletter_LoadGM2(g_DataLogoTile_Colors, g_ScreenColorLow);
	// Draw tiles data
	VDP_FillScreen_GM2(0x00);
	VDP_WriteLayout_GM2(g_DataLogoTileL0_Names, 12, 12, 6, 2);
	VDP_WriteLayout_GM2(g_DataLogoTileL1_Names, 13, 8, 4, 4);
	
	g_StateTimer = 0;
	g_FrameCount = 0;
	
	VDP_EnableDisplay(TRUE);
}

//-----------------------------------------------------------------------------
//
bool Logo_Update()
{
	g_FrameCount++;

	if ((g_FrameCount & 0b00000011) != 0)
		return FALSE;

	g_StateTimer++;
	if ((g_StateTimer >= LOGO_START) && (g_StateTimer < LOGO_START + LOGO_OFFSET))
	{
		u8 offset = (95 + LOGO_START) - g_StateTimer;

		VDP_SetSpritePositionY(0, offset);
		VDP_SetSpritePositionY(1, offset);
		VDP_SetSpritePositionY(2, offset);
		offset -= 16;
		VDP_SetSpritePositionY(3, offset);
		VDP_SetSpritePositionY(4, offset);
		offset -= 16;
		VDP_SetSpritePositionY(5, offset);
		VDP_SetSpritePositionY(6, offset);
	}

	if ((g_StateTimer == LOGO_END) || Logo_Skip())
		return TRUE;

	return FALSE;
}
