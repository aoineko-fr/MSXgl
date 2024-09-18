// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄           
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██   ▄▀██ ▄█▄█ ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄█ ▀▄██ ██ █ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Game pawn module
//─────────────────────────────────────────────────────────────────────────────
#include "game_pawn.h"
#include "vdp.h"
#include "memory.h"

//=============================================================================
// MEMORY DATA
//=============================================================================

// RAM buffer to send data to VRAM
static u8 g_Game_DrawY;
static u8 g_Game_DrawX;
static u8 g_Game_DrawPattern;

// Static pointer for compilation optimization
static Game_Pawn* g_Pawn;
static const Game_Sprite* g_Sprite;

#if (GAMEPAWN_USE_PHYSICS)
// Current cell coordinate
u8 g_Game_CellX;
u8 g_Game_CellY;
#endif

// Tile map buffer in RAM
#if (!GAMEPAWN_USE_VRAM_COL)
const u8* g_GamePawn_TileMap;
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize game pawn
void GamePawn_Initialize(Game_Pawn* pawn, const Game_Sprite* sprtList, u8 sprtNum, u8 sprtID, const Game_Action* actList)
{
	sprtID;
	g_Pawn = pawn;
	// Initialize pawn structure
	Mem_Set(0x00, g_Pawn, sizeof(Game_Pawn));
	g_Pawn->SpriteList = sprtList;
	g_Pawn->SpriteNum = sprtNum;
	#if !(GAMEPAWN_ID_PER_LAYER)
	g_Pawn->SpriteID = sprtID;
	#endif
	g_Pawn->ActionList = actList;

	// Initialize pawn action
	GamePawn_SetAction(g_Pawn, 0);

	// Initialize pawn sprite color
	#if !(GAMEPAWN_ID_PER_LAYER)
	u8 sprtIdx = sprtID;
	#endif
	g_Sprite = sprtList;
	loop(i, g_Pawn->SpriteNum)
	{
		#if (GAMEPAWN_ID_PER_LAYER)
		u8 sprtIdx = g_Sprite->SpriteID;
		#endif

		u8 color = g_Sprite->Color;
		if((g_Sprite->Flag & PAWN_SPRITE_OR) != 0)
			color |= VDP_SPRITE_CC;

		#if ((MSX_VERSION & MSX_1) || GAMEPAWN_FORCE_SM1)
		VDP_SetSpriteColorSM1(sprtIdx, color);
		#else
		VDP_SetSpriteUniColor(sprtIdx, color);
		#endif

		#if !(GAMEPAWN_ID_PER_LAYER)
		if((g_Sprite->Flag & PAWN_SPRITE_ODD) == 0)
			sprtIdx++;
		#endif

		g_Sprite++;
	}

}

//-----------------------------------------------------------------------------
// Set game pawn position
void GamePawn_SetPosition(Game_Pawn* pawn, u8 x, u8 y)
{
	pawn->PositionX = x;
	pawn->PositionY = y;
	pawn->MoveX = 0;
	pawn->MoveY = 0;
	pawn->Update |= PAWN_UPDATE_POSITION;
}


//-----------------------------------------------------------------------------
// Set game pawn action id
void GamePawn_SetAction(Game_Pawn* pawn, u8 id)
{
	if(pawn->ActionId != id)
	{
		pawn->ActionId = id;
		pawn->AnimTimer = 0;
		pawn->AnimStep = 0;
		pawn->Update |= PAWN_UPDATE_PATTERN;
	}
}

//-----------------------------------------------------------------------------
// Update animation of the game pawn
void GamePawn_Update(Game_Pawn* pawn)
{
	g_Pawn = pawn;
	const Game_Action* act = &g_Pawn->ActionList[g_Pawn->ActionId];

	// Finished current animation step
	if(g_Pawn->AnimTimer >= act->FrameList[g_Pawn->AnimStep].Duration)
	{
		g_Pawn->AnimTimer = 0;
		g_Pawn->AnimStep++;
		g_Pawn->Update |= PAWN_UPDATE_PATTERN;
	}

	// Finished last animation step
	if(g_Pawn->AnimStep >= act->FrameNum)
	{
		if(act->Loop) // restart action
		{
			g_Pawn->AnimTimer = 0;
			g_Pawn->AnimStep = 0;
			g_Pawn->Update |= PAWN_UPDATE_PATTERN;
		}
		else // stop action and transit to default action
		{
			GamePawn_SetAction(g_Pawn, 0);
			return;
		}
	}

	// Execute event
	const Game_Frame* frame = &act->FrameList[g_Pawn->AnimStep];
	if(frame->Event != 0)
		frame->Event();

	// Update animation
	g_Pawn->AnimFrame = frame->Id;
	g_Pawn->AnimTimer++;
	g_Pawn->Counter++;

#if (GAMEPAWN_USE_PHYSICS)
	if(g_Pawn->Update & PAWN_UPDATE_COLLISION)
	{
		u8 targetX = g_Pawn->PositionX + g_Pawn->MoveX;
		u8 targetY = g_Pawn->PositionY + g_Pawn->MoveY;

		//.....................................................................
		// Vertical movement - Go down
		if(g_Pawn->MoveY > 0)
		{
			#if ((GAMEPAWN_BORDER_EVENT & GAMEPAWN_BORDER_DOWN) || (GAMEPAWN_BORDER_BLOCK & GAMEPAWN_BORDER_DOWN))
			if(targetY + GET_BOUND_Y >= GAMEPAWN_BORDER_MAX_Y)
			{
				#if (GAMEPAWN_BORDER_EVENT & GAMEPAWN_BORDER_DOWN)
					g_Pawn->PhysicsCB(PAWN_PHYSICS_BORDER_DOWN, 0);
				#endif
				#if (GAMEPAWN_BORDER_BLOCK & GAMEPAWN_BORDER_DOWN)
					targetY = (u8)(GAMEPAWN_BORDER_MAX_Y - GET_BOUND_Y);
				#endif
			}
			else
			#endif
			{
				g_Game_CellY = (targetY + GET_BOUND_Y) / 8;
				g_Game_CellX = 0xFF;
				u8 lastCell = 0xFF;
				#if (GAMEPAWN_COL_DOWN & GAMEPAWN_COL_0)
					g_Game_CellX = (targetX) / 8;
					if(g_Game_CellX != lastCell)
					{
						lastCell = g_Game_CellX;
						u8 tile = GAMEPAWN_GET_TILE(g_Game_CellX, g_Game_CellY);
						if(g_Pawn->CollisionCB(tile))
						{
							g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_DOWN, tile);
							targetY = (g_Game_CellY * 8) - GET_BOUND_Y;
							goto skipVertival;
						}
					}
				#endif
				#if (GAMEPAWN_COL_DOWN & GAMEPAWN_COL_25)
					g_Game_CellX = (targetX + (GET_BOUND_X / 4)) / 8;
					if(g_Game_CellX != lastCell)
					{
						lastCell = g_Game_CellX;
						u8 tile = GAMEPAWN_GET_TILE(g_Game_CellX, g_Game_CellY);
						if(g_Pawn->CollisionCB(tile))
						{
							g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_DOWN, tile);
							targetY = (g_Game_CellY * 8) - GET_BOUND_Y;
							goto skipVertival;
						}
					}
				#endif
				#if (GAMEPAWN_COL_DOWN & GAMEPAWN_COL_50)
					g_Game_CellX = (targetX + (GET_BOUND_X / 2)) / 8;
					if(g_Game_CellX != lastCell)
					{
						lastCell = g_Game_CellX;
						u8 tile = GAMEPAWN_GET_TILE(g_Game_CellX, g_Game_CellY);
						if(g_Pawn->CollisionCB(tile))
						{
							g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_DOWN, tile);
							targetY = (g_Game_CellY * 8) - GET_BOUND_Y;
							goto skipVertival;
						}
					}
				#endif
				#if (GAMEPAWN_COL_DOWN & GAMEPAWN_COL_75)
					g_Game_CellX = (targetX + GET_BOUND_X - (GET_BOUND_X / 4)) / 8;
					if(g_Game_CellX != lastCell)
					{
						lastCell = g_Game_CellX;
						u8 tile = GAMEPAWN_GET_TILE(g_Game_CellX, g_Game_CellY);
						if(g_Pawn->CollisionCB(tile))
						{
							g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_DOWN, tile);
							targetY = (g_Game_CellY * 8) - GET_BOUND_Y;
							goto skipVertival;
						}
					}
				#endif
				#if (GAMEPAWN_COL_DOWN & GAMEPAWN_COL_100)
					g_Game_CellX = (targetX + GET_BOUND_X - 1) / 8;
					if(g_Game_CellX != lastCell)
					{
						lastCell = g_Game_CellX;
						u8 tile = GAMEPAWN_GET_TILE(g_Game_CellX, g_Game_CellY);
						if(g_Pawn->CollisionCB(tile))
						{
							g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_DOWN, tile);
							targetY = (g_Game_CellY * 8) - GET_BOUND_Y;
							goto skipVertival;
						}
					}
				#endif
			}
		}
		//.....................................................................
		// Vertical movement - Go up
		else if(g_Pawn->MoveY < 0)
		{
			#if ((GAMEPAWN_BORDER_EVENT & GAMEPAWN_BORDER_UP) || (GAMEPAWN_BORDER_BLOCK & GAMEPAWN_BORDER_UP))
			#if (GAMEPAWN_BORDER_MIN_Y > 0)
			if((targetY > g_Pawn->PositionY) || ((targetY < GAMEPAWN_BORDER_MIN_Y)))
			#else
			if(targetY > g_Pawn->PositionY)
			#endif
			{
				#if (GAMEPAWN_BORDER_EVENT & GAMEPAWN_BORDER_UP)
					g_Pawn->PhysicsCB(PAWN_PHYSICS_BORDER_UP, 0);
				#endif
				#if (GAMEPAWN_BORDER_BLOCK & GAMEPAWN_BORDER_UP)
					targetY = GAMEPAWN_BORDER_MIN_Y;
				#endif
			}
			else
			#endif
			{
				g_Game_CellX = (targetX + (GET_BOUND_X / 2)) / 8;
				g_Game_CellY = (targetY) / 8;
				u8 tile = GAMEPAWN_GET_TILE(g_Game_CellX, g_Game_CellY);
				if(g_Pawn->CollisionCB(tile))
				{
					g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_UP, tile);
					targetY = (g_Game_CellY * 8) + 8;
				}
			}
		}
		//.....................................................................
		// No vertical movement - Check floor
		else // if(g_Pawn->MoveY == 0)
		{
			#if (GAMEPAWN_BORDER_BLOCK & GAMEPAWN_BORDER_DOWN)
			if(targetY + GET_BOUND_Y < GAMEPAWN_BORDER_MAX_Y)
			#endif
			{
				u8 tile = 0;
				g_Game_CellY = (targetY + GET_BOUND_Y) / 8;
				g_Game_CellX = 0xFF;
				u8 lastCell = 0xFF;
				#if (GAMEPAWN_COL_DOWN & GAMEPAWN_COL_0)
					g_Game_CellX = (targetX) / 8;
					if(g_Game_CellX != lastCell)
					{
						lastCell = g_Game_CellX;
						tile = GAMEPAWN_GET_TILE(g_Game_CellX, g_Game_CellY);
						if(g_Pawn->CollisionCB(tile))
							goto skipVertival;
					}
				#endif
				#if (GAMEPAWN_COL_DOWN & GAMEPAWN_COL_25)
					g_Game_CellX = (targetX + (GET_BOUND_X / 4)) / 8;
					if(g_Game_CellX != lastCell)
					{
						lastCell = g_Game_CellX;
						tile = GAMEPAWN_GET_TILE(g_Game_CellX, g_Game_CellY);
						if(g_Pawn->CollisionCB(tile))
							goto skipVertival;
					}
				#endif
				#if (GAMEPAWN_COL_DOWN & GAMEPAWN_COL_50)
					g_Game_CellX = (targetX + (GET_BOUND_X / 2)) / 8;
					if(g_Game_CellX != lastCell)
					{
						lastCell = g_Game_CellX;
						tile = GAMEPAWN_GET_TILE(g_Game_CellX, g_Game_CellY);
						if(g_Pawn->CollisionCB(tile))
							goto skipVertival;
					}
				#endif
				#if (GAMEPAWN_COL_DOWN & GAMEPAWN_COL_75)
					g_Game_CellX = (targetX + GET_BOUND_X - (GET_BOUND_X / 4)) / 8;
					if(g_Game_CellX != lastCell)
					{
						lastCell = g_Game_CellX;
						tile = GAMEPAWN_GET_TILE(g_Game_CellX, g_Game_CellY);
						if(g_Pawn->CollisionCB(tile))
							goto skipVertival;
					}
				#endif
				#if (GAMEPAWN_COL_DOWN & GAMEPAWN_COL_100)
					g_Game_CellX = (targetX + GET_BOUND_X - 1) / 8;
					if(g_Game_CellX != lastCell)
					{
						lastCell = g_Game_CellX;
						tile = GAMEPAWN_GET_TILE(g_Game_CellX, g_Game_CellY);
						if(g_Pawn->CollisionCB(tile))
							goto skipVertival;
					}
				#endif
				g_Pawn->PhysicsCB(PAWN_PHYSICS_FALL, tile);
			}
		}
skipVertival:
		//.....................................................................
		// Horizontal movement - Go right
		if(g_Pawn->MoveX > 0)
		{
			#if ((GAMEPAWN_BORDER_EVENT & GAMEPAWN_BORDER_RIGHT) || (GAMEPAWN_BORDER_BLOCK & GAMEPAWN_BORDER_RIGHT))
			if((u8)(targetX + GET_BOUND_X) < g_Pawn->PositionX)
			{
				#if (GAMEPAWN_BORDER_EVENT & GAMEPAWN_BORDER_RIGHT)
					g_Pawn->PhysicsCB(PAWN_PHYSICS_BORDER_RIGHT, 0);
				#endif
				#if (GAMEPAWN_BORDER_BLOCK & GAMEPAWN_BORDER_RIGHT)
					targetX = (u8)(0 - GET_BOUND_X);
				#endif
			}
			else
			#endif
			{
				g_Game_CellX = (targetX + GET_BOUND_X) / 8;
				g_Game_CellY = (targetY + (GET_BOUND_Y / 2)) / 8;
				u8 tile = GAMEPAWN_GET_TILE(g_Game_CellX, g_Game_CellY);
				if(g_Pawn->CollisionCB(tile))
				{
					g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_RIGHT, tile);
					targetX = (g_Game_CellX * 8) - GET_BOUND_X;
				}
			}
		}
		//.....................................................................
		// Horizontal movement - Go left
		else if(g_Pawn->MoveX < 0)
		{
			#if ((GAMEPAWN_BORDER_EVENT & GAMEPAWN_BORDER_LEFT) || (GAMEPAWN_BORDER_BLOCK & GAMEPAWN_BORDER_LEFT))
			if(targetX > g_Pawn->PositionX)
			{
				#if (GAMEPAWN_BORDER_EVENT & GAMEPAWN_BORDER_LEFT)
					g_Pawn->PhysicsCB(PAWN_PHYSICS_BORDER_LEFT, 0);
				#endif
				#if (GAMEPAWN_BORDER_BLOCK & GAMEPAWN_BORDER_LEFT)
					targetX = 0;
				#endif
			}
			else
			#endif
			{
				g_Game_CellX = (targetX) / 8;
				g_Game_CellY = (targetY + (GET_BOUND_Y / 2)) / 8;
				u8 tile = GAMEPAWN_GET_TILE(g_Game_CellX, g_Game_CellY);
				if(g_Pawn->CollisionCB(tile))
				{
					g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_LEFT, tile);
					targetX = (g_Game_CellX * 8) + 8;
				}
			}
		}

		g_Pawn->PositionX = targetX;
		g_Pawn->PositionY = targetY;
		g_Pawn->Update |= PAWN_UPDATE_POSITION;
		g_Pawn->Update &= ~PAWN_UPDATE_COLLISION;
	}
#endif
}

//-----------------------------------------------------------------------------
// Update rendering of the game pawn
void GamePawn_Draw(Game_Pawn* pawn)
{
	g_Pawn = pawn;
	if(g_Pawn->Update == 0)
		return;

	g_Sprite = g_Pawn->SpriteList;
	#if !(GAMEPAWN_ID_PER_LAYER)
	u16 dest = g_SpriteAtributeLow + (g_Pawn->SpriteID * 4);
	#endif
	loop(i, g_Pawn->SpriteNum)
	{
		if(g_Sprite->Flag & PAWN_SPRITE_EVEN) // Skip odd frames
		{
			if((g_Pawn->Counter & 1) != 0)
				goto SkipDrawing;
			else
				g_Pawn->Update |= PAWN_UPDATE_PATTERN;
		}
		else if(g_Sprite->Flag & PAWN_SPRITE_ODD) // Skip even frames
		{
			if((g_Pawn->Counter & 1) == 0)
				goto SkipDrawing;
			else
				g_Pawn->Update |= PAWN_UPDATE_PATTERN;
		}

		g_Game_DrawY = g_Pawn->PositionY + g_Sprite->OffsetY - 1; // Decrement Y to fit screen coordinate
		g_Game_DrawX = g_Pawn->PositionX + g_Sprite->OffsetX;
		u8 size = 2;

		if(g_Pawn->Update & PAWN_UPDATE_PATTERN)
		{
			g_Game_DrawPattern = g_Pawn->AnimFrame + g_Sprite->DataOffset;
			size++;
		}

		#if (GAMEPAWN_ID_PER_LAYER)
		u16 dest = g_SpriteAtributeLow + (g_Sprite->SpriteID * 4);
		#endif
		VDP_WriteVRAM((u8*)&g_Game_DrawY, dest, g_SpriteAtributeHigh, size);
		#if !(GAMEPAWN_ID_PER_LAYER)
		dest += 4;
		#endif

	SkipDrawing:
		//g_Sprite++;
		g_Sprite = (const Game_Sprite*)((u8*)g_Sprite + sizeof(Game_Sprite));
	}
	g_Pawn->Update = 0;
}


#if (GAMEPAWN_USE_PHYSICS)
//-----------------------------------------------------------------------------
// Set pawn movement vector
void GamePawn_SetMovement(Game_Pawn* pawn, i8 dx, i8 dy) 
{ 
	pawn->MoveX = dx;
	pawn->MoveY = dy;
	pawn->Update |= PAWN_UPDATE_COLLISION;
}

//-----------------------------------------------------------------------------
// Set pawn physics callback
void GamePawn_InitializePhysics(Game_Pawn* pawn, Game_PhysicsCB pcb, Game_CollisionCB ccb, u8 boundX, u8 boundY)
{
	boundX;
	boundY;
	pawn->PhysicsCB = pcb; 
	pawn->CollisionCB = ccb;
	#if (GAMEPAWN_BOUND_X == GAMEPAWN_BOUND_CUSTOM)
	pawn->BoundX = boundX;
	#endif
	#if (GAMEPAWN_BOUND_Y == GAMEPAWN_BOUND_CUSTOM)
	pawn->BoundY = boundY;
	#endif
}
#endif