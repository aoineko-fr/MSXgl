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

#define SCROLL_SRC_X			0
#define SCROLL_SRC_Y			0
#define SCROLL_SRC_W			128
#define SCROLL_SRC_H			24

#define SCROLL_DST_X			0
#define SCROLL_DST_Y			2
#define SCROLL_DST_W			32
#define SCROLL_DST_H			22

#define SCROLL_WRAP				1

#define SCROLL_SKIP_NONE		0
#define SCROLL_SKIP_1			0x1
#define SCROLL_SKIP_2			0x3
#define SCROLL_SKIP_4			0x7
#define SCROLL_SKIP_8			0xF

#define SCROLL_SKIP				SCROLL_SKIP_NONE

#if (MSX_VERSION >= MSX_2)
	#define SCROLL_ADJUST		1
	#define SCROLL_MASK			1
	#define SCROLL_MASK_ID		0
#endif

//=============================================================================
// READ-ONLY DATA
//=============================================================================

//=============================================================================
// MEMORY DATA
//=============================================================================
u8  g_Scroll_Count = 0;
u16 g_Scroll_OffsetX = 0;
u16 g_Scroll_OffsetY = 0;
u16 g_Scroll_Map;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// 
void Scroll_Initialize(u16 map)
{	
	g_Scroll_Map = map;

	#if ((MSX_VERSION >= MSX_2) && SCROLL_MASK)
	// Initialize mask sprites
	VDP_EnableSprite(true);
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16 | VDP_SPRITE_SCALE_2);
	VDP_FillVRAM(0xFF, g_SpritePatternLow, g_SpritePatternHigh, 8*4);
	VDP_SetSpriteExUniColor(SCROLL_MASK_ID + 0,  0, (u8)255+0,   0, (u8)COLOR_BLACK + VDP_SPRITE_EC);
	VDP_SetSpriteExUniColor(SCROLL_MASK_ID + 1,  0, (u8)255+32,  0, (u8)COLOR_BLACK + VDP_SPRITE_EC);
	VDP_SetSpriteExUniColor(SCROLL_MASK_ID + 2,  0, (u8)255+64,  0, (u8)COLOR_BLACK + VDP_SPRITE_EC);
	VDP_SetSpriteExUniColor(SCROLL_MASK_ID + 3,  0, (u8)255+96,  0, (u8)COLOR_BLACK + VDP_SPRITE_EC);
	VDP_SetSpriteExUniColor(SCROLL_MASK_ID + 4,  0, (u8)255+128, 0, (u8)COLOR_BLACK + VDP_SPRITE_EC);
	VDP_SetSpriteExUniColor(SCROLL_MASK_ID + 5,  0, (u8)255+160, 0, (u8)COLOR_BLACK + VDP_SPRITE_EC);
	VDP_SetSpriteExUniColor(SCROLL_MASK_ID + 6,  0, (u8)255+0,   0, COLOR_BLACK);
	VDP_SetSpriteExUniColor(SCROLL_MASK_ID + 7,  0, (u8)255+32,  0, COLOR_BLACK);
	VDP_SetSpriteExUniColor(SCROLL_MASK_ID + 8,  0, (u8)255+64,  0, COLOR_BLACK);
	VDP_SetSpriteExUniColor(SCROLL_MASK_ID + 9,  0, (u8)255+96,  0, COLOR_BLACK);
	VDP_SetSpriteExUniColor(SCROLL_MASK_ID + 10, 0, (u8)255+128, 0, COLOR_BLACK);
	VDP_SetSpriteExUniColor(SCROLL_MASK_ID + 11, 0, (u8)255+160, 0, COLOR_BLACK);
	VDP_DisableSpritesFrom(12);
	#endif
}

//-----------------------------------------------------------------------------
// 
void Scroll_SetOffset(i8 offset)
{
	if(offset > 0)
	{
	#if (SCROLL_WRAP)
		g_Scroll_OffsetX += offset;
		if(g_Scroll_OffsetX >= (SCROLL_SRC_W * 8))
			g_Scroll_OffsetX += offset - (SCROLL_SRC_W * 8);
	#else
		if(g_Scroll_OffsetX + offset < (SCROLL_SRC_W - SCROLL_DST_W) * 8)
			g_Scroll_OffsetX += offset;
		else
			g_Scroll_OffsetX = (SCROLL_SRC_W - SCROLL_DST_W) * 8;
	#endif
	}
	else // if(offset < 0)
	{
	#if (SCROLL_WRAP)
		if(g_Scroll_OffsetX > -offset)
			g_Scroll_OffsetX += offset;
		else
			g_Scroll_OffsetX += offset + (SCROLL_SRC_W * 8);
	#else
		if(g_Scroll_OffsetX > -offset)
			g_Scroll_OffsetX += offset;
		else
			g_Scroll_OffsetX = 0;
	#endif
	}
}

//-----------------------------------------------------------------------------
// 
void Scroll_Update()
{	
	#if (SCROLL_SKIP != SCROLL_SKIP_NONE)
	if(count & SCROLL_SKIP)
		continue;
	#endif

	#if (MSX_VERSION >= MSX_2)
	u8 offsetStep = g_Scroll_OffsetX & 0x7;
		#if (SCROLL_ADJUST)
		VDP_SetAdjustOffset(offsetStep);
		#endif
		#if (SCROLL_MASK)
		VDP_SetSpritePositionX(SCROLL_MASK_ID + 0, offsetStep); // Left mask
		VDP_SetSpritePositionX(SCROLL_MASK_ID + 1, offsetStep);
		VDP_SetSpritePositionX(SCROLL_MASK_ID + 2, offsetStep);
		VDP_SetSpritePositionX(SCROLL_MASK_ID + 3, offsetStep);
		VDP_SetSpritePositionX(SCROLL_MASK_ID + 4, offsetStep);
		VDP_SetSpritePositionX(SCROLL_MASK_ID + 5, offsetStep);
		offsetStep += 255 - 7;
		VDP_SetSpritePositionX(SCROLL_MASK_ID + 6,  offsetStep); // Right mask
		VDP_SetSpritePositionX(SCROLL_MASK_ID + 7,  offsetStep);
		VDP_SetSpritePositionX(SCROLL_MASK_ID + 8,  offsetStep);
		VDP_SetSpritePositionX(SCROLL_MASK_ID + 9,  offsetStep);
		VDP_SetSpritePositionX(SCROLL_MASK_ID + 10, offsetStep);
		VDP_SetSpritePositionX(SCROLL_MASK_ID + 11, offsetStep);
		#endif
	#endif

	u16 offsetTileX = g_Scroll_OffsetX >> 3;

	u16 src = (u16)g_Scroll_Map + offsetTileX + SCROLL_SRC_Y * SCROLL_SRC_W;
	u16 dst = g_ScreenLayoutLow + (SCROLL_DST_Y * 32) + SCROLL_DST_X;
	u16 sw = SCROLL_SRC_W - offsetTileX;
	for(u8 y = 0; y < SCROLL_DST_H; ++y)
	{
		#if(SCROLL_WRAP)
		if(offsetTileX + SCROLL_DST_W > SCROLL_SRC_W)
		{
			VDP_WriteVRAM_16K((const u8*)src, dst, sw);
			VDP_WriteVRAM_16K((const u8*)src - offsetTileX, dst + sw, SCROLL_DST_W - sw);
		}
		else
		#endif
		VDP_WriteVRAM_16K((const u8*)src, dst, SCROLL_DST_W);
		src += SCROLL_SRC_W;
		dst += 32;
	}
}


