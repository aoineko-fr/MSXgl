// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄  ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄█▀▀ ██▄▀ ▄█▀▄ ██  ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀█▄▄ ██   ▀█▄▀ ▀█▄ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#include "scroll.h"
#include "color.h"

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// READ-ONLY DATA
//=============================================================================

//=============================================================================
// MEMORY DATA
//=============================================================================

// Address of the source map data
u16 g_Scroll_Map;

#if (SCROLL_HORIZONTAL)
// Horizontal offset (in pixel)
u16 g_Scroll_OffsetX;
// Horizontal offset (in tiles)
u8 g_Scroll_TileX;
#endif

#if (SCROLL_VERTICAL)
// Vertical offset (in pixel)
u16 g_Scroll_OffsetY;
// Vertical offset (in tiles)
u8 g_Scroll_TileY;
#endif

#if (SCROLL_ADJUST)
u8 g_Scroll_Adjust;
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize scrolling module
u8 Scroll_Initialize(u16 map)
{	
	g_Scroll_Map = map;

	#if (SCROLL_HORIZONTAL)
	g_Scroll_OffsetX = 0;
	g_Scroll_TileX = 0xFF;
	#endif

	#if (SCROLL_VERTICAL)
	g_Scroll_OffsetY = 0;
	g_Scroll_TileY = 0xFF;
	#endif

	#if (SCROLL_ADJUST)
	g_Scroll_Adjust = 0;
	#endif

	u8 sprtId = 0;
	
	#if ((SCROLL_HORIZONTAL) && (SCROLL_MASK))
	// Initialize mask sprites
	VDP_EnableSprite(TRUE);
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16 | VDP_SPRITE_SCALE_2);
	VDP_FillVRAM(0xFF, g_SpritePatternLow + (SCROLL_MASK_PATTERN * 4), g_SpritePatternHigh, 8 * 4);
	sprtId = SCROLL_MASK_ID;
	u8 y = (SCROLL_DST_Y) * 8 - 1;
	for(u8 i = 0; i < (SCROLL_DST_H + 3) / 4; ++i)
	{
		VDP_SetSpriteExUniColor(sprtId++, 0, y, 0, (u8)(SCROLL_MASK_COLOR + VDP_SPRITE_EC));
		y += 32;
	}
	y = (SCROLL_DST_Y) * 8 - 1;
	for(u8 i = 0; i < (SCROLL_DST_H + 3) / 4; ++i)
	{
		VDP_SetSpriteExUniColor(sprtId++, 0, y, 0, SCROLL_MASK_COLOR);
		y += 32;
	}
	VDP_DisableSpritesFrom(sprtId);
	#endif
	
	return sprtId;
}

#if (SCROLL_HORIZONTAL)
//-----------------------------------------------------------------------------
// Set scrolling horizontal offset
void Scroll_SetOffsetH(i8 offset)
{
	if(offset > 0)
	{
	#if (SCROLL_WRAP)
		g_Scroll_OffsetX += offset;
		if(g_Scroll_OffsetX >= ((SCROLL_SRC_W) * 8))
			g_Scroll_OffsetX += offset - ((SCROLL_SRC_W) * 8);
	#else
		if(g_Scroll_OffsetX + offset < ((SCROLL_SRC_W) - (SCROLL_DST_W)) * 8)
			g_Scroll_OffsetX += offset;
		else
			g_Scroll_OffsetX = ((SCROLL_SRC_W) - (SCROLL_DST_W)) * 8;
	#endif
	}
	else // if(offset <= 0)
	{
	#if (SCROLL_WRAP)
		if(g_Scroll_OffsetX > -offset)
			g_Scroll_OffsetX += offset;
		else
			g_Scroll_OffsetX += offset + ((SCROLL_SRC_W) * 8);
	#else
		if(g_Scroll_OffsetX > -offset)
			g_Scroll_OffsetX += offset;
		else
			g_Scroll_OffsetX = 0;
	#endif
	}
}
#endif // (SCROLL_HORIZONTAL)

#if (SCROLL_VERTICAL)
//-----------------------------------------------------------------------------
// Set scrolling vertical offset
void Scroll_SetOffsetV(i8 offset)
{
	if(offset > 0)
	{
		if(g_Scroll_OffsetY + offset < ((SCROLL_SRC_H) - (SCROLL_DST_H)) * 8)
			g_Scroll_OffsetY += offset;
		else
			g_Scroll_OffsetY = ((SCROLL_SRC_H) - (SCROLL_DST_H)) * 8;
	}
	else // if(offset <= 0)
	{
		if(g_Scroll_OffsetY > -offset)
			g_Scroll_OffsetY += offset;
		else
			g_Scroll_OffsetY = 0;
	}
}
#endif // (SCROLL_VERTICAL)

//-----------------------------------------------------------------------------
#if ((SCROLL_ADJUST) && (SCROLL_ADJUST_SPLIT))
void Scroll_HBlankAdjust(u8 adjust)
{
	switch(adjust)
	{
	case 0:
		VDP_SetHBlankLine((SCROLL_DST_Y) * 8 - 7);
		break;
	case 1:
		VDP_SetAdjustOffset(g_Scroll_Adjust);
		VDP_SetHBlankLine((u8)((SCROLL_DST_Y + SCROLL_DST_H) * 8 - 4));
		break;
	default:
		VDP_SetAdjustOffset(0);
		break;
	}
}
#endif

//-----------------------------------------------------------------------------
// Update scrolling
void Scroll_Update()
{	
	// Compute horizontal scrolling step
	#if (SCROLL_HORIZONTAL)
	u8  offsetStepX = g_Scroll_OffsetX & 0x7;
	u16 offsetTileX = g_Scroll_OffsetX >> 3;
	#endif
	
	// Compute vertical scrolling step
	#if (SCROLL_VERTICAL)
	u8  offsetStepY = g_Scroll_OffsetY & 0x7;
	u16 offsetTileY = g_Scroll_OffsetY >> 3;
	#endif
	
	// Set screen adjustment register
	#if (SCROLL_ADJUST)
	g_Scroll_Adjust = 0;
	#if (SCROLL_HORIZONTAL)
	g_Scroll_Adjust |= offsetStepX;
	#endif
	#if (SCROLL_VERTICAL)
	g_Scroll_Adjust |= offsetStepY << 4;
	#endif
	#if (!SCROLL_ADJUST_SPLIT)
	VDP_SetAdjustOffset(g_Scroll_Adjust);
	#endif
	#endif

	// Update sprite mask position
	#if ((SCROLL_HORIZONTAL) && (SCROLL_MASK))
	u8 sprtId = SCROLL_MASK_ID;
	offsetStepX += ((SCROLL_DST_X) * 8);
	for(u8 i = 0; i < (SCROLL_DST_H + 3) / 4; ++i)
		VDP_SetSpritePositionX(sprtId++, offsetStepX);
	offsetStepX += (((SCROLL_DST_W) - 1) * 8);
	for(u8 i = 0; i < (SCROLL_DST_H + 3) / 4; ++i)
		VDP_SetSpritePositionX(sprtId++, offsetStepX);
	#endif

	// Check for tile movement
	#if ((SCROLL_HORIZONTAL) && (SCROLL_VERTICAL))
	if((offsetTileX == g_Scroll_TileX) && (offsetTileY == g_Scroll_TileY))
		return;
	#elif (SCROLL_HORIZONTAL)
	if(offsetTileX == g_Scroll_TileX)
		return;
	#elif (SCROLL_VERTICAL)
	if(offsetTileY == g_Scroll_TileY)
		return;
	#endif

	#if (SCROLL_HORIZONTAL)
	g_Scroll_TileX = offsetTileX;
	#define TILE_X	offsetTileX
	#else
	#define TILE_X	0
	#endif

	#if (SCROLL_VERTICAL)
	g_Scroll_TileY = offsetTileY;
	#define TILE_Y	offsetTileY
	#else
	#define TILE_Y	0
	#endif

	// Do scrolling -- Update tiles table in VRAM
	u16 src = (u16)g_Scroll_Map + ((SCROLL_SRC_Y) * (SCROLL_SRC_W)) + (TILE_Y * (SCROLL_SRC_W)) + TILE_X;
	u16 dst = g_ScreenLayoutLow + ((SCROLL_DST_Y) * SCROLL_SCREEN_W) + (SCROLL_DST_X);
	u16 sw = (SCROLL_SRC_W) - TILE_X;
	for(u8 y = 0; y < (SCROLL_DST_H); ++y)
	{
		#if(SCROLL_WRAP)
		if(TILE_X + (SCROLL_DST_W) > (SCROLL_SRC_W))
		{
			VDP_WriteVRAM_16K((const u8*)src, dst, sw);
			VDP_WriteVRAM_16K((const u8*)src - TILE_X, dst + sw, (SCROLL_DST_W) - sw);
		}
		else
		#endif
		VDP_WriteVRAM_16K((const u8*)src, dst, (SCROLL_DST_W));
		src += (SCROLL_SRC_W);
		dst += SCROLL_SCREEN_W;
	}
}
