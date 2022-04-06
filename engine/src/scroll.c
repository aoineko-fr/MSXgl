// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄  ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄█▀▀ ██▄▀ ▄█▀▄ ██  ██  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀█▄▄ ██   ▀█▄▀ ▀█▄ ▀█▄ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#include "scroll.h"
#include "color.h"

//=============================================================================
// DEFINES
//=============================================================================

// Allow horizontal and/or vertical scrolling
#define SCROLL_HORIZONTAL			1
#define SCROLL_VERTICAL				1

// Source data info
#define SCROLL_SRC_X				0
#define SCROLL_SRC_Y				0
#define SCROLL_SRC_W				128
#define SCROLL_SRC_H				24

// Destination data info
#define SCROLL_DST_X				0
#define SCROLL_DST_Y				2
#define SCROLL_DST_W				32
#define SCROLL_DST_H				22

// Allow scroll data looping
#define SCROLL_WRAP					1

// Skip frame
#define SCROLL_SKIP_NONE			0
#define SCROLL_SKIP_1				0x1
#define SCROLL_SKIP_2				0x3
#define SCROLL_SKIP_4				0x7
#define SCROLL_SKIP_8				0xF
#define SCROLL_SKIP					SCROLL_SKIP_NONE

// Use screen position adjust register (allow per-pixel scrolling) [MSX2]
#define SCROLL_ADJUST				1
	
// Use sprite mask (allow smooth per-pixel scrolling) [MSX2]
#define SCROLL_MASK					1
#define SCROLL_MASK_ID				0
#define SCROLL_MASK_COLOR			COLOR_BLACK

//=============================================================================
// READ-ONLY DATA
//=============================================================================

//=============================================================================
// MEMORY DATA
//=============================================================================
u8  g_Scroll_Count = 0;
u16 g_Scroll_OffsetX = 0;
u16 g_Scroll_OffsetY = 0;
u8  g_Scroll_TileX = 0;
u8  g_Scroll_TileY = 0;
u16 g_Scroll_Map;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize scrolling module
u8 Scroll_Initialize(u16 map)
{	
	g_Scroll_Map = map;
	g_Scroll_OffsetX = 0;
	g_Scroll_OffsetY = 0;
	g_Scroll_TileX = 0xFF;
	g_Scroll_TileY = 0xFF;

	u8 sprtId = 0;
	#if ((MSX_VERSION >= MSX_2) && SCROLL_MASK)
	// Initialize mask sprites
	VDP_EnableSprite(true);
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16 | VDP_SPRITE_SCALE_2);
	VDP_FillVRAM(0xFF, g_SpritePatternLow, g_SpritePatternHigh, 8*4);
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
	else // if(offset < 0)
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
	g_Scroll_OffsetY += offset;
}
#endif // (SCROLL_VERTICAL)

//-----------------------------------------------------------------------------
// Update scrolling
void Scroll_Update()
{	
	// Check frame skip
	#if (SCROLL_SKIP != SCROLL_SKIP_NONE)
	if(count & SCROLL_SKIP)
		continue;
	#endif

	// Update screen adjust register and sprite mask
	#if (MSX_VERSION >= MSX_2)
	u8 offsetStep = g_Scroll_OffsetX & 0x7;
		#if (SCROLL_ADJUST)
		VDP_SetAdjustOffset(offsetStep);
		#endif
		#if (SCROLL_MASK)
		u8 sprtId = SCROLL_MASK_ID;

		offsetStep += ((SCROLL_DST_X) * 8);
		for(u8 i = 0; i < (SCROLL_DST_H + 3) / 4; ++i)
			VDP_SetSpritePositionX(sprtId++, offsetStep);

		offsetStep += (((SCROLL_DST_W) - 1) * 8);
		for(u8 i = 0; i < (SCROLL_DST_H + 3) / 4; ++i)
			VDP_SetSpritePositionX(sprtId++, offsetStep);
		#endif
	#endif

	// Get tile offset and check for movement
	u16 offsetTileX = g_Scroll_OffsetX >> 3;
	// if(offsetTileX == g_Scroll_TileX)
		// return;
	g_Scroll_TileX = offsetTileX;

	u16 offsetTileY = g_Scroll_OffsetY >> 3;
	// if(offsetTileX == g_Scroll_TileX)
		// return;
	g_Scroll_TileY = offsetTileY;



	// 
	u16 src = (u16)g_Scroll_Map + ((SCROLL_SRC_Y) * (SCROLL_SRC_W)) + (offsetTileY * (SCROLL_SRC_W)) + offsetTileX;
	u16 dst = g_ScreenLayoutLow + ((SCROLL_DST_Y) * 32) + (SCROLL_DST_X);
	u16 sw = (SCROLL_SRC_W) - offsetTileX;
	for(u8 y = 0; y < (SCROLL_DST_H); ++y)
	{
		#if(SCROLL_WRAP)
		if(offsetTileX + (SCROLL_DST_W) > (SCROLL_SRC_W))
		{
			VDP_WriteVRAM_16K((const u8*)src, dst, sw);
			VDP_WriteVRAM_16K((const u8*)src - offsetTileX, dst + sw, (SCROLL_DST_W) - sw);
		}
		else
		#endif
		VDP_WriteVRAM_16K((const u8*)src, dst, (SCROLL_DST_W));
		src += (SCROLL_SRC_W);
		dst += 32;
	}
}
