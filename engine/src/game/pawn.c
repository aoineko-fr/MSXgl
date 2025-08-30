// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄           
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██   ▄▀██ ▄█▄█ ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄█ ▀▄██ ██ █ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Game pawn module
//─────────────────────────────────────────────────────────────────────────────
#include "game/pawn.h"
#include "vdp.h"
#include "memory.h"
// #if ((PAWN_TILEMAP_SRC == PAWN_TILEMAP_SRC_V9) || (PAWN_SPT_MODE == PAWN_SPT_MODE_V9_P1) || (PAWN_SPT_MODE == PAWN_SPT_MODE_V9_P2))
#include "v9990.h"
// #endif
// #if (PAWN_USE_RT_LOAD && PAWN_USE_SPRT_FX)
#include "sprite_fx.h"
// #endif

//=============================================================================
// DEFINES
//=============================================================================

// Sprite attribute index
#define PAWN_SPRT_POS_Y				0
#define PAWN_SPRT_POS_X				1
#define PAWN_SPRT_PATTERN			2
#define PAWN_SPRT_COLOR				3

// Number of sprite pattern
#if (PAWN_SPRITE_SIZE == 16)
	#define PAWN_PATTERN_NUM		4
	#define PAWN_FUNC_FLIP_X		SpriteFX_FlipHorizontal16
	#define PAWN_FUNC_FLIP_Y		SpriteFX_FlipVertical16
	#define PAWN_FUNC_ROTATE_90		SpriteFX_RotateRight16
	#define PAWN_FUNC_ROTATE_180	SpriteFX_RotateHalfTurn16
	#define PAWN_FUNC_ROTATE_270	SpriteFX_RotateLeft16
#else
	#define PAWN_PATTERN_NUM		1
	#define PAWN_FUNC_FLIP_X		SpriteFX_FlipHorizontal8
	#define PAWN_FUNC_FLIP_Y		SpriteFX_FlipVertical8
	#define PAWN_FUNC_ROTATE_90		SpriteFX_RotateRight8
	#define PAWN_FUNC_ROTATE_180	SpriteFX_RotateHalfTurn8
	#define PAWN_FUNC_ROTATE_270	SpriteFX_RotateLeft8
#endif

// Bounding box width
#if (PAWN_BOUND_X == PAWN_BOUND_CUSTOM)
	#define GET_BOUND_X()			g_Pawn->BoundX
#else
	#define GET_BOUND_X()			PAWN_BOUND_X
#endif

// Bounding box height
#if (PAWN_BOUND_Y == PAWN_BOUND_CUSTOM)
	#define GET_BOUND_Y()			g_Pawn->BoundY
#else
	#define GET_BOUND_Y()			PAWN_BOUND_Y
#endif

// Tile map getter macro
#if (PAWN_TILEMAP_SRC == PAWN_TILEMAP_SRC_RAM)
	#define PAWN_GET_TILE(X, Y)		g_Pawn_TileMap[((Y) * PAWN_TILEMAP_WIDTH) + (X)]
#elif (PAWN_TILEMAP_SRC == PAWN_TILEMAP_SRC_VRAM)
	#define PAWN_GET_TILE(X, Y)		VDP_Peek(g_ScreenLayoutLow + ((Y) * PAWN_TILEMAP_WIDTH) + (X), g_ScreenLayoutHigh)
#elif (PAWN_TILEMAP_SRC == PAWN_TILEMAP_SRC_V9)
	#define PAWN_GET_TILE(X, Y)		V9_Peek(V9_CellAddrP1A(X, Y))
#endif

// Macros to handle sprite index in both 1-id-per-layer and 1-id-per-pawn mode
#if (PAWN_ID_PER_LAYER)
	#define PAWN_SPRT_INIT()
	#define PAWN_SPRT_LOOP()		u8 sprtIdx = g_Pawn_Sprite->SpriteID;
	#define PAWN_SPRT_NEXT()

	#define PAWN_ADDR_INIT()
	#define PAWN_ADDR_LOOP()		u16 dest = g_SpriteAttributeLow + (g_Pawn_Sprite->SpriteID * 4);
	#define PAWN_ADDR_NEXT()
#else
	#define PAWN_SPRT_INIT()		u8 sprtIdx = g_Pawn->SpriteID;
	#define PAWN_SPRT_LOOP()
	#define PAWN_SPRT_NEXT()		sprtIdx++;

	#define PAWN_ADDR_INIT()		u16 dest = g_SpriteAttributeLow + (g_Pawn->SpriteID * 4);
	#define PAWN_ADDR_LOOP()
	#define PAWN_ADDR_NEXT()		dest += 4;
#endif

// Callback for sprite parsing
typedef void (*SpriteCallback)(u8);	

//=============================================================================
// MEMORY DATA
//=============================================================================

// RAM buffer to send data to VRAM
static u8 g_Pawn_Buffer[4];

// Static pointer for compilation optimization
static Pawn* g_Pawn;
static const Pawn_Sprite* g_Pawn_Sprite;

// Current cell coordinate
#if (PAWN_USE_PHYSICS)
u8 g_Pawn_CellX;
u8 g_Pawn_CellY;
#endif

// Tile map buffer in RAM
#if (PAWN_TILEMAP_SRC == PAWN_TILEMAP_SRC_RAM)
const u8* g_Pawn_TileMap;
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void PawnSprite_Enable(u8 sprtIdx)
{
#if (PAWN_SPT_MODE == PAWN_SPT_MODE_V9_P1)
	V9_SetSpriteDisableP1(sprtIdx, FALSE);
#elif (PAWN_SPT_MODE == PAWN_SPT_MODE_V9_P2)
	V9_SetSpriteDisableP2(sprtIdx, FALSE);
#else // if ((PAWN_SPT_MODE == PAWN_SPT_MODE_AUTO) || (PAWN_SPT_MODE == PAWN_SPT_MODE_MSX1) || (PAWN_SPT_MODE == PAWN_SPT_MODE_MSX2))
	sprtIdx;
#endif
}

//-----------------------------------------------------------------------------
//
void PawnSprite_Disable(u8 sprtIdx)
{
#if (PAWN_SPT_MODE == PAWN_SPT_MODE_V9_P1)
	V9_SetSpriteDisableP1(sprtIdx, TRUE);
#elif (PAWN_SPT_MODE == PAWN_SPT_MODE_V9_P2)
	V9_SetSpriteDisableP2(sprtIdx, TRUE);
#else // if ((PAWN_SPT_MODE == PAWN_SPT_MODE_AUTO) || (PAWN_SPT_MODE == PAWN_SPT_MODE_MSX1) || (PAWN_SPT_MODE == PAWN_SPT_MODE_MSX2))
	VDP_HideSprite(sprtIdx);
#endif
}

//-----------------------------------------------------------------------------
//
void PawnSprite_Initialize(u8 sprtIdx)
{
#if (PAWN_SPT_MODE == PAWN_SPT_MODE_V9_P1)
	V9_SetSpriteInfoP1(sprtIdx, V9_SPAT_INFO_PALETTE(g_Pawn_Sprite->Color));
#elif (PAWN_SPT_MODE == PAWN_SPT_MODE_V9_P2)
	V9_SetSpriteInfoP2(sprtIdx, V9_SPAT_INFO_PALETTE(g_Pawn_Sprite->Color));
#else // if ((PAWN_SPT_MODE == PAWN_SPT_MODE_AUTO) || (PAWN_SPT_MODE == PAWN_SPT_MODE_MSX1) || (PAWN_SPT_MODE == PAWN_SPT_MODE_MSX2))
	u8 color = g_Pawn_Sprite->Color;
	if ((g_Pawn_Sprite->Flag & PAWN_SPRITE_OR) != 0)
		color |= VDP_SPRITE_CC;

	#if (PAWN_SPT_MODE == PAWN_SPT_MODE_MSX12)
	if (VDP_GetMode() < VDP_MODE_MSX2)
		VDP_SetSpriteColorSM1(sprtIdx, color);
	else
		VDP_SetSpriteUniColor(sprtIdx, color);
	#elif (PAWN_SPT_MODE == PAWN_SPT_MODE_MSX1)
	VDP_SetSpriteColorSM1(sprtIdx, color);
	#else // (PAWN_SPT_MODE == PAWN_SPT_MODE_MSX2)
	VDP_SetSpriteUniColor(sprtIdx, color);
	#endif

	#if (PAWN_USE_RT_LOAD)
	VDP_SetSpritePattern(sprtIdx, sprtIdx * PAWN_PATTERN_NUM);
	#endif
#endif
}

//-----------------------------------------------------------------------------
// Parse pawn's sprite and execute callback for each one
void Pawn_ParseSprite(Pawn* pawn, SpriteCallback cb)
{
	g_Pawn = pawn;
	g_Pawn_Sprite = g_Pawn->SpriteList;
	PAWN_SPRT_INIT()
	loop(i, g_Pawn->SpriteNum)
	{
		PAWN_SPRT_LOOP()
		cb(sprtIdx);
		PAWN_SPRT_NEXT();
		g_Pawn_Sprite++;
	}
}

//-----------------------------------------------------------------------------
// Initialize game pawn
void Pawn_Initialize(Pawn* pawn, const Pawn_Sprite* sprtList, u8 sprtNum, u8 sprtID, const Pawn_Action* actList)
{
	sprtID;
	g_Pawn = pawn;
	// Initialize pawn structure
	Mem_Set(0x00, g_Pawn, sizeof(Pawn));
	g_Pawn->SpriteList = sprtList;
	g_Pawn->SpriteNum = sprtNum;
#if !(PAWN_ID_PER_LAYER)
	g_Pawn->SpriteID = sprtID;
#endif
	g_Pawn->ActionList = actList;

	// Initialize pawn action
	Pawn_ForceSetAction(g_Pawn, 0);

	// Initialize all sprites
	Pawn_ParseSprite(g_Pawn, PawnSprite_Initialize);
}

//-----------------------------------------------------------------------------
// Set game pawn position
void Pawn_SetPosition(Pawn* pawn, PAWN_POS x, PAWN_POS y)
{
	pawn->PositionX = x;
	pawn->PositionY = y;
	pawn->MoveX = 0;
	pawn->MoveY = 0;
	pawn->Update |= PAWN_UPDATE_POSITION;
}

//-----------------------------------------------------------------------------
// Force to set game pawn action id
void Pawn_ForceSetAction(Pawn* pawn, u8 id)
{
	pawn->ActionId = id;
	Pawn_RestartAction(pawn);
}

//-----------------------------------------------------------------------------
// Set game pawn action id
void Pawn_SetAction(Pawn* pawn, u8 id)
{
	if ((pawn->ActionList[pawn->ActionId].Interrupt == 1) && (pawn->ActionId != id))
		Pawn_ForceSetAction(pawn, id);
}

//-----------------------------------------------------------------------------
// Set if pawn is enable or disable. Disabled pawn will not be updated nor drawn.
void Pawn_SetEnable(Pawn* pawn, bool enable)
{
	g_Pawn = pawn;
	if (enable)
	{
		g_Pawn->Update &= ~PAWN_UPDATE_DISABLE;
		g_Pawn->Update |= (PAWN_UPDATE_PATTERN | PAWN_UPDATE_POSITION);

		Pawn_ParseSprite(g_Pawn, PawnSprite_Enable);
	}
	else
	{
		g_Pawn->Update |= PAWN_UPDATE_DISABLE;

		Pawn_ParseSprite(g_Pawn, PawnSprite_Disable);
	}
}

//-----------------------------------------------------------------------------
// Update animation of the game pawn
void Pawn_UpdateAnimation()
{

	const Pawn_Action* act = &g_Pawn->ActionList[g_Pawn->ActionId];

	// Finished current animation step
	if (g_Pawn->AnimTimer >= act->FrameList[g_Pawn->AnimStep].Duration)
	{
		g_Pawn->AnimTimer = 0;
		g_Pawn->AnimStep++;
		g_Pawn->Update |= PAWN_UPDATE_PATTERN;
	}

	// Finished last animation step
	if (g_Pawn->AnimStep >= act->FrameNum)
	{
		if (act->Loop) // restart action
		{
			g_Pawn->AnimTimer = 0;
			g_Pawn->AnimStep = 0;
			g_Pawn->Update |= PAWN_UPDATE_PATTERN;
		}
		else // stop action and transit to default action
		{
			Pawn_ForceSetAction(g_Pawn, 0);
			act = &g_Pawn->ActionList[0];
		}
	}

	// Execute event
	const Pawn_Frame* frame = &act->FrameList[g_Pawn->AnimStep];
	if (frame->Event != NULL)
		frame->Event();

	// Update animation
	g_Pawn->AnimFrame = frame->Id;
	g_Pawn->AnimTimer++;
	g_Pawn->Counter++;
}

#if (PAWN_USE_PHYSICS)
//-----------------------------------------------------------------------------
// Update pawn collision
void Pawn_UpdatePhysics()
{
#if (PAWN_UNIT == PAWN_UNIT_SCREEN)
	PAWN_POS targetX = g_Pawn->PositionX + g_Pawn->MoveX;
	PAWN_POS targetY = g_Pawn->PositionY + g_Pawn->MoveY;
#else // (PAWN_UNIT == PAWN_UNIT_QMN())
	PAWN_POS targetX = (g_Pawn->PositionX + g_Pawn->MoveX) / PAWN_UNIT;
	PAWN_POS targetY = (g_Pawn->PositionY + g_Pawn->MoveY) / PAWN_UNIT;
#endif

	//.....................................................................
	// Vertical movement - Go down
	if (g_Pawn->MoveY > 0)
	{
		#if ((PAWN_BORDER_EVENT & PAWN_BORDER_DOWN) || (PAWN_BORDER_BLOCK & PAWN_BORDER_DOWN))
		if (targetY + GET_BOUND_Y() >= PAWN_BORDER_MAX_Y)
		{
			#if (PAWN_BORDER_EVENT & PAWN_BORDER_DOWN)
				g_Pawn->PhysicsCB(PAWN_PHYSICS_BORDER_DOWN, 0);
			#endif
			#if (PAWN_BORDER_BLOCK & PAWN_BORDER_DOWN)
				targetY = (u8)(PAWN_BORDER_MAX_Y - GET_BOUND_Y());
			#endif
		}
		else
		#endif
		{
			g_Pawn_CellY = (targetY + GET_BOUND_Y()) / 8;
			g_Pawn_CellX = 0xFF;
			u8 lastCell = 0xFF;
			#if (PAWN_COL_DOWN & PAWN_COL_0)
				g_Pawn_CellX = (targetX) / 8;
				if (g_Pawn_CellX != lastCell)
				{
					lastCell = g_Pawn_CellX;
					u8 tile = PAWN_GET_TILE(g_Pawn_CellX, g_Pawn_CellY);
					if (g_Pawn->CollisionCB(tile))
					{
						g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_DOWN, tile);
						targetY = (g_Pawn_CellY * 8) - GET_BOUND_Y();
						goto skipVertival;
					}
				}
			#endif
			#if (PAWN_COL_DOWN & PAWN_COL_25)
				g_Pawn_CellX = (targetX + (GET_BOUND_X() / 4)) / 8;
				if (g_Pawn_CellX != lastCell)
				{
					lastCell = g_Pawn_CellX;
					u8 tile = PAWN_GET_TILE(g_Pawn_CellX, g_Pawn_CellY);
					if (g_Pawn->CollisionCB(tile))
					{
						g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_DOWN, tile);
						targetY = (g_Pawn_CellY * 8) - GET_BOUND_Y();
						goto skipVertival;
					}
				}
			#endif
			#if (PAWN_COL_DOWN & PAWN_COL_50)
				g_Pawn_CellX = (targetX + (GET_BOUND_X() / 2)) / 8;
				if (g_Pawn_CellX != lastCell)
				{
					lastCell = g_Pawn_CellX;
					u8 tile = PAWN_GET_TILE(g_Pawn_CellX, g_Pawn_CellY);
					if (g_Pawn->CollisionCB(tile))
					{
						g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_DOWN, tile);
						targetY = (g_Pawn_CellY * 8) - GET_BOUND_Y();
						goto skipVertival;
					}
				}
			#endif
			#if (PAWN_COL_DOWN & PAWN_COL_75)
				g_Pawn_CellX = (targetX + GET_BOUND_X() - (GET_BOUND_X() / 4)) / 8;
				if (g_Pawn_CellX != lastCell)
				{
					lastCell = g_Pawn_CellX;
					u8 tile = PAWN_GET_TILE(g_Pawn_CellX, g_Pawn_CellY);
					if (g_Pawn->CollisionCB(tile))
					{
						g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_DOWN, tile);
						targetY = (g_Pawn_CellY * 8) - GET_BOUND_Y();
						goto skipVertival;
					}
				}
			#endif
			#if (PAWN_COL_DOWN & PAWN_COL_100)
				g_Pawn_CellX = (targetX + GET_BOUND_X() - 1) / 8;
				if (g_Pawn_CellX != lastCell)
				{
					lastCell = g_Pawn_CellX;
					u8 tile = PAWN_GET_TILE(g_Pawn_CellX, g_Pawn_CellY);
					if (g_Pawn->CollisionCB(tile))
					{
						g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_DOWN, tile);
						targetY = (g_Pawn_CellY * 8) - GET_BOUND_Y();
						goto skipVertival;
					}
				}
			#endif
		}
	}
	//.....................................................................
	// Vertical movement - Go up
	else if (g_Pawn->MoveY < 0)
	{
		#if (PAWN_COL_UP != PAWN_COL_NONE)
		#if ((PAWN_BORDER_EVENT & PAWN_BORDER_UP) || (PAWN_BORDER_BLOCK & PAWN_BORDER_UP))
		#if (PAWN_BORDER_MIN_Y > 0)
		if ((targetY > g_Pawn->PositionY) || ((targetY < PAWN_BORDER_MIN_Y)))
		#else
		if (targetY > g_Pawn->PositionY)
		#endif
		{
			#if (PAWN_BORDER_EVENT & PAWN_BORDER_UP)
				g_Pawn->PhysicsCB(PAWN_PHYSICS_BORDER_UP, 0);
			#endif
			#if (PAWN_BORDER_BLOCK & PAWN_BORDER_UP)
				targetY = PAWN_BORDER_MIN_Y;
			#endif
		}
		else
		#endif // ((PAWN_BORDER_EVENT & PAWN_BORDER_UP) || (PAWN_BORDER_BLOCK & PAWN_BORDER_UP))
		{
			g_Pawn_CellX = (targetX + (GET_BOUND_X() / 2)) / 8;
			g_Pawn_CellY = (targetY) / 8;
			u8 tile = PAWN_GET_TILE(g_Pawn_CellX, g_Pawn_CellY);
			if (g_Pawn->CollisionCB(tile))
			{
				g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_UP, tile);
				targetY = (g_Pawn_CellY * 8) + 8;
			}
		}
		#endif // (PAWN_COL_UP != PAWN_COL_NONE)
	}
	//.....................................................................
	// No vertical movement - Check floor
	else // if (g_Pawn->MoveY == 0)
	{
		#if (PAWN_BORDER_BLOCK & PAWN_BORDER_DOWN)
		if (targetY + GET_BOUND_Y() < PAWN_BORDER_MAX_Y)
		#endif
		{
			u8 tile = 0;
			g_Pawn_CellY = (targetY + GET_BOUND_Y()) / 8;
			g_Pawn_CellX = 0xFF;
			u8 lastCell = 0xFF;
			#if (PAWN_COL_DOWN & PAWN_COL_0)
				g_Pawn_CellX = (targetX) / 8;
				if (g_Pawn_CellX != lastCell)
				{
					lastCell = g_Pawn_CellX;
					tile = PAWN_GET_TILE(g_Pawn_CellX, g_Pawn_CellY);
					if (g_Pawn->CollisionCB(tile))
						goto skipVertival;
				}
			#endif
			#if (PAWN_COL_DOWN & PAWN_COL_25)
				g_Pawn_CellX = (targetX + (GET_BOUND_X() / 4)) / 8;
				if (g_Pawn_CellX != lastCell)
				{
					lastCell = g_Pawn_CellX;
					tile = PAWN_GET_TILE(g_Pawn_CellX, g_Pawn_CellY);
					if (g_Pawn->CollisionCB(tile))
						goto skipVertival;
				}
			#endif
			#if (PAWN_COL_DOWN & PAWN_COL_50)
				g_Pawn_CellX = (targetX + (GET_BOUND_X() / 2)) / 8;
				if (g_Pawn_CellX != lastCell)
				{
					lastCell = g_Pawn_CellX;
					tile = PAWN_GET_TILE(g_Pawn_CellX, g_Pawn_CellY);
					if (g_Pawn->CollisionCB(tile))
						goto skipVertival;
				}
			#endif
			#if (PAWN_COL_DOWN & PAWN_COL_75)
				g_Pawn_CellX = (targetX + GET_BOUND_X() - (GET_BOUND_X() / 4)) / 8;
				if (g_Pawn_CellX != lastCell)
				{
					lastCell = g_Pawn_CellX;
					tile = PAWN_GET_TILE(g_Pawn_CellX, g_Pawn_CellY);
					if (g_Pawn->CollisionCB(tile))
						goto skipVertival;
				}
			#endif
			#if (PAWN_COL_DOWN & PAWN_COL_100)
				g_Pawn_CellX = (targetX + GET_BOUND_X() - 1) / 8;
				if (g_Pawn_CellX != lastCell)
				{
					lastCell = g_Pawn_CellX;
					tile = PAWN_GET_TILE(g_Pawn_CellX, g_Pawn_CellY);
					if (g_Pawn->CollisionCB(tile))
						goto skipVertival;
				}
			#endif
			g_Pawn->PhysicsCB(PAWN_PHYSICS_FALL, tile);
		}
	}
skipVertival:
	//.....................................................................
	// Horizontal movement - Go right
	if (g_Pawn->MoveX > 0)
	{
		#if ((PAWN_BORDER_EVENT & PAWN_BORDER_RIGHT) || (PAWN_BORDER_BLOCK & PAWN_BORDER_RIGHT))
		if ((u8)(targetX + GET_BOUND_X()) < g_Pawn->PositionX)
		{
			#if (PAWN_BORDER_EVENT & PAWN_BORDER_RIGHT)
				g_Pawn->PhysicsCB(PAWN_PHYSICS_BORDER_RIGHT, 0);
			#endif
			#if (PAWN_BORDER_BLOCK & PAWN_BORDER_RIGHT)
				targetX = (u8)(0 - GET_BOUND_X());
			#endif
		}
		else
		#endif
		{
			g_Pawn_CellX = (targetX + GET_BOUND_X()) / 8;
			g_Pawn_CellY = (targetY + (GET_BOUND_Y() / 2)) / 8;
			u8 tile = PAWN_GET_TILE(g_Pawn_CellX, g_Pawn_CellY);
			if (g_Pawn->CollisionCB(tile))
			{
				g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_RIGHT, tile);
				targetX = (g_Pawn_CellX * 8) - GET_BOUND_X();
			}
		}
	}
	//.....................................................................
	// Horizontal movement - Go left
	else if (g_Pawn->MoveX < 0)
	{
		#if ((PAWN_BORDER_EVENT & PAWN_BORDER_LEFT) || (PAWN_BORDER_BLOCK & PAWN_BORDER_LEFT))
		if (targetX > g_Pawn->PositionX)
		{
			#if (PAWN_BORDER_EVENT & PAWN_BORDER_LEFT)
				g_Pawn->PhysicsCB(PAWN_PHYSICS_BORDER_LEFT, 0);
			#endif
			#if (PAWN_BORDER_BLOCK & PAWN_BORDER_LEFT)
				targetX = 0;
			#endif
		}
		else
		#endif
		{
			g_Pawn_CellX = (targetX) / 8;
			g_Pawn_CellY = (targetY + (GET_BOUND_Y() / 2)) / 8;
			u8 tile = PAWN_GET_TILE(g_Pawn_CellX, g_Pawn_CellY);
			if (g_Pawn->CollisionCB(tile))
			{
				g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_LEFT, tile);
				targetX = (g_Pawn_CellX * 8) + 8;
			}
		}
	}

#if (PAWN_UNIT == PAWN_UNIT_SCREEN)
	g_Pawn->PositionX = targetX;
	g_Pawn->PositionY = targetY;
#else // (PAWN_UNIT == PAWN_UNIT_QMN())
	g_Pawn->PositionX = targetX * PAWN_UNIT;
	g_Pawn->PositionY = targetY * PAWN_UNIT;
#endif

	g_Pawn->Update |= PAWN_UPDATE_POSITION;
	g_Pawn->Update &= ~PAWN_UPDATE_COLLISION;
}
#endif

//-----------------------------------------------------------------------------
// Update the game pawn
void Pawn_Update(Pawn* pawn)
{
	g_Pawn = pawn;

	if (g_Pawn->Update & PAWN_UPDATE_DISABLE)
		return;

	// Update animation
	Pawn_UpdateAnimation();

#if (PAWN_USE_PHYSICS)
	// Update physics
	if (g_Pawn->Update & PAWN_UPDATE_COLLISION)
		Pawn_UpdatePhysics();
#endif
}

#if ((PAWN_SPT_MODE == PAWN_SPT_MODE_V9_P1) || (PAWN_SPT_MODE == PAWN_SPT_MODE_V9_P2))
//-----------------------------------------------------------------------------
// Update rendering of the game pawn
inline void Pawn_Draw_V9()
{
	g_Pawn_Sprite = g_Pawn->SpriteList;

	PAWN_SPRT_INIT()
	loop(i, g_Pawn->SpriteNum)
	{
		PAWN_SPRT_LOOP()
		g_Pawn_Buffer[0] = g_Pawn->PositionY + g_Pawn_Sprite->OffsetY - 1; // Decrement Y to fit screen coordinate;
		g_Pawn_Buffer[1] = g_Pawn->AnimFrame + g_Pawn_Sprite->DataOffset;
		g_Pawn_Buffer[2] = g_Pawn->PositionX + g_Pawn_Sprite->OffsetX;
	#if (PAWN_SPT_MODE == PAWN_SPT_MODE_V9_P1)
		V9_WriteVRAM(V9_P1_SPAT + (sprtIdx * PAWN_PATTERN_NUM), (const u8*)g_Pawn_Buffer, 3);
	#else
		V9_WriteVRAM(V9_P2_SPAT + (sprtIdx * PAWN_PATTERN_NUM), (const u8*)g_Pawn_Buffer, 3);
	#endif
		PAWN_SPRT_NEXT()
		g_Pawn_Sprite++;
	}
}
#endif // ((PAWN_SPT_MODE == PAWN_SPT_MODE_V9_P1) || (PAWN_SPT_MODE == PAWN_SPT_MODE_V9_P2))

#if ((PAWN_SPT_MODE == PAWN_SPT_MODE_MSX1) || (PAWN_SPT_MODE == PAWN_SPT_MODE_MSX2) || (PAWN_SPT_MODE == PAWN_SPT_MODE_MSX12))

u8 g_Pawn_FrameOffset;

#if 1//(PAWN_ID_PER_LAYER)
//-----------------------------------------------------------------------------
// Update rendering of the game pawn
inline void Pawn_Draw_Sprite()
{
	g_Pawn_Sprite = g_Pawn->SpriteList;

	loop(i, g_Pawn->SpriteNum)
	{
		g_Pawn_FrameOffset = 0;
		if ((g_Pawn->Update & PAWN_UPDATE_BLEND) && (g_Pawn_Sprite->Flag & PAWN_SPRITE_BLEND)) // Skip odd frames
		{
			if ((g_Pawn->Counter & 1) != 0)
				g_Pawn_FrameOffset = PAWN_PATTERN_NUM;
			g_Pawn->Update |= PAWN_UPDATE_PATTERN;
		}

		g_Pawn_Buffer[PAWN_SPRT_POS_Y] = g_Pawn->PositionY + g_Pawn_Sprite->OffsetY - 1; // Decrement Y to fit screen coordinate
		g_Pawn_Buffer[PAWN_SPRT_POS_X] = g_Pawn->PositionX + g_Pawn_Sprite->OffsetX;
		
		u8 size = 2;
		if (g_Pawn->Update & PAWN_UPDATE_PATTERN)
		{
			u8 frame = g_Pawn->AnimFrame + g_Pawn_Sprite->DataOffset + g_Pawn_FrameOffset;
			#if (PAWN_USE_RT_LOAD)
				const u8* src = (const u8*)g_Pawn->PatternAddr + frame * 8;
				#if (PAWN_USE_SPRT_FX)
					u8* buffer = (u8*)Mem_GetHeapAddress();
					switch (g_Pawn->SpriteFX)
					{
						case PAWN_SPRITE_FX_FLIP_X:
							PAWN_FUNC_FLIP_X(src, buffer);
							src = buffer;
							break;
						case PAWN_SPRITE_FX_FLIP_Y:
							PAWN_FUNC_FLIP_Y(src, buffer);
							src = buffer;
							break;
						case PAWN_SPRITE_FX_ROTATE_90:
							PAWN_FUNC_ROTATE_90(src, buffer);
							src = buffer;
							break;
						case PAWN_SPRITE_FX_ROTATE_180:
							PAWN_FUNC_ROTATE_180(src, buffer);
							src = buffer;
							break;
						case PAWN_SPRITE_FX_ROTATE_270:
							PAWN_FUNC_ROTATE_270(src, buffer);
							src = buffer;
							break;
						default:
							break;
					}
				#endif
				VDP_WriteVRAM_16K(src, VDP_GetSpritePatternTable() + g_Pawn_Sprite->SpriteID * PAWN_PATTERN_NUM * 8, PAWN_PATTERN_NUM * 8);
			#else
				g_Pawn_Buffer[PAWN_SPRT_PATTERN] = frame;
				size++;
			#endif
		}

		u16 dest = g_SpriteAttributeLow + (g_Pawn_Sprite->SpriteID * 4);		
		VDP_WriteVRAM(g_Pawn_Buffer, dest, g_SpriteAttributeHigh, size);
		
		g_Pawn_Sprite++;
	}
}
#else // if (PAWN_ID_PER_LAYER == FALSE)
//-----------------------------------------------------------------------------
// Update rendering of the game pawn
inline void Pawn_Draw_Sprite()
{
	g_Pawn_Sprite = g_Pawn->SpriteList;

	u8 sprtIdx = g_Pawn->SpriteID;
	u16 dest = g_SpriteAttributeLow + (g_Pawn->SpriteID * 4);
	loop(i, g_Pawn->SpriteNum)
	{
		g_Pawn_FrameOffset = 0;
		if ((g_Pawn->Update & PAWN_UPDATE_BLEND) && (g_Pawn_Sprite->Flag & PAWN_SPRITE_BLEND)) // Skip odd frames
		{
			if ((g_Pawn->Counter & 1) != 0)
				g_Pawn_FrameOffset = PAWN_PATTERN_NUM;
			g_Pawn->Update |= PAWN_UPDATE_PATTERN;
		}

		g_Pawn_Buffer[PAWN_SPRT_POS_Y] = g_Pawn->PositionY + g_Pawn_Sprite->OffsetY - 1; // Decrement Y to fit screen coordinate
		g_Pawn_Buffer[PAWN_SPRT_POS_X] = g_Pawn->PositionX + g_Pawn_Sprite->OffsetX;
		
		u8 size = 2;
		if (g_Pawn->Update & PAWN_UPDATE_PATTERN)
		{
			u8 frame = g_Pawn->AnimFrame + g_Pawn_Sprite->DataOffset + g_Pawn_FrameOffset;
			#if (PAWN_USE_RT_LOAD)
				const u8* src = (const u8*)g_Pawn->PatternAddr + frame * 8;
				#if (PAWN_USE_SPRT_FX)
					u8* buffer = (u8*)Mem_GetHeapAddress();
					switch (g_Pawn->SpriteFX)
					{
						case PAWN_SPRITE_FX_FLIP_X:
							PAWN_FUNC_FLIP_X(src, buffer);
							src = buffer;
							break;
						case PAWN_SPRITE_FX_FLIP_Y:
							PAWN_FUNC_FLIP_Y(src, buffer);
							src = buffer;
							break;
						case PAWN_SPRITE_FX_ROTATE_90:
							PAWN_FUNC_ROTATE_90(src, buffer);
							src = buffer;
							break;
						case PAWN_SPRITE_FX_ROTATE_180:
							PAWN_FUNC_ROTATE_180(src, buffer);
							src = buffer;
							break;
						case PAWN_SPRITE_FX_ROTATE_270:
							PAWN_FUNC_ROTATE_270(src, buffer);
							src = buffer;
							break;
						default:
							break;
					}
				#endif
				VDP_WriteVRAM_16K(src, VDP_GetSpritePatternTable() + sprtIdx * PAWN_PATTERN_NUM * 8, PAWN_PATTERN_NUM * 8);
			#else
				g_Pawn_Buffer[PAWN_SPRT_PATTERN] = frame;
				size++;
			#endif
		}

		VDP_WriteVRAM(g_Pawn_Buffer, dest, g_SpriteAttributeHigh, size);
		
		sprtIdx++;
		dest += 4;

		g_Pawn_Sprite++;
	}
}
#endif // (PAWN_ID_PER_LAYER)
#endif // ((PAWN_SPT_MODE == PAWN_SPT_MODE_MSX1) || (PAWN_SPT_MODE == PAWN_SPT_MODE_MSX2) || (PAWN_SPT_MODE == PAWN_SPT_MODE_MSX12))

//-----------------------------------------------------------------------------
// Update rendering of the game pawn
void Pawn_Draw(Pawn* pawn)
{
	g_Pawn = pawn;

	if ((g_Pawn->Update & (PAWN_UPDATE_PATTERN | PAWN_UPDATE_POSITION)) == 0)
		return;
	
	if (g_Pawn->Update & PAWN_UPDATE_DISABLE)
		return;

#if ((PAWN_SPT_MODE == PAWN_SPT_MODE_V9_P1) || (PAWN_SPT_MODE == PAWN_SPT_MODE_V9_P2))
	Pawn_Draw_V9();
#elif ((PAWN_SPT_MODE == PAWN_SPT_MODE_MSX1) || (PAWN_SPT_MODE == PAWN_SPT_MODE_MSX2) || (PAWN_SPT_MODE == PAWN_SPT_MODE_MSX12))
	Pawn_Draw_Sprite();
#endif

	g_Pawn->Update &= ~(PAWN_UPDATE_PATTERN | PAWN_UPDATE_POSITION);
}

#if (PAWN_USE_PHYSICS)
//-----------------------------------------------------------------------------
// Set pawn movement vector
void Pawn_SetMovement(Pawn* pawn, i8 dx, i8 dy) 
{ 
	pawn->MoveX = dx;
	pawn->MoveY = dy;
	pawn->Update |= PAWN_UPDATE_COLLISION;
}

//-----------------------------------------------------------------------------
// Set pawn physics callback
void Pawn_InitializePhysics(Pawn* pawn, Pawn_PhysicsCB pcb, Pawn_CollisionCB ccb, u8 boundX, u8 boundY)
{
	boundX;
	boundY;
#if (PAWN_BOUND_X == PAWN_BOUND_CUSTOM)
	pawn->BoundX = boundX;
#endif
#if (PAWN_BOUND_Y == PAWN_BOUND_CUSTOM)
	pawn->BoundY = boundY;
#endif
	pawn->PhysicsCB = pcb; 
	pawn->CollisionCB = ccb;
}
#endif