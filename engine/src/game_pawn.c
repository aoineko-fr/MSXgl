// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄           
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██   ▄▀██ ▄█▄█ ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄█ ▀▄██ ██ █ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// Game pawn module
//─────────────────────────────────────────────────────────────────────────────
#include "game_pawn.h"
#include "vdp.h"
#include "memory.h"

//=============================================================================
// MEMORY DATA
//=============================================================================

static u8 g_Game_DrawY;
static u8 g_Game_DrawX;
static u8 g_Game_DrawPattern;
static Game_Pawn* g_Pawn;
static const Game_Sprite* g_Sprite;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize game pawn
void GamePawn_Initialize(Game_Pawn* pawn, const Game_Sprite* sprtList, u8 sprtNum, u8 sprtID, const Game_Action* actList)
{
	g_Pawn = pawn;
	// Initialize pawn structure
	Mem_Set(0x00, g_Pawn, sizeof(Game_Pawn));
	g_Pawn->SpriteList = sprtList;
	g_Pawn->SpriteNum = sprtNum;
	g_Pawn->SpriteID = sprtID;
	g_Pawn->ActionList = actList;

	// Initialize pawn action
	GamePawn_SetAction(g_Pawn, 0);
	
	// Initialize pawn sprite color
	u8 sprtIdx = sprtID;
	g_Sprite = sprtList;
	loop(i, g_Pawn->SpriteNum)
	{
		VDP_SetSpriteColorSM1(sprtIdx, g_Sprite->Color);

		if((g_Sprite->Flag & PAWN_SPRITE_ODD) == 0)
			sprtIdx++;
		
		g_Sprite++;
	}

}

//-----------------------------------------------------------------------------
// Set game pawn position
void GamePawn_SetPosition(Game_Pawn* pawn, u8 x, u8 y)
{
	pawn->PositionX = x;
	pawn->PositionY = y;
	pawn->TargetX = x;
	pawn->TargetY = y;
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
		// Vertical movement
		if(g_Pawn->TargetY > g_Pawn->PositionY) // Go down
		{
			u8 cellX = (g_Pawn->TargetX + (g_Pawn->BoundX / 2)) / 8;
			u8 cellY = (g_Pawn->TargetY + g_Pawn->BoundY) / 8;		
			u8 tile = VDP_Peek_16K(g_ScreenLayoutLow + (cellY * 32) + cellX);
			if(g_Pawn->CollisionCB(tile))
			{
				g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_DOWN, tile);
				g_Pawn->TargetY = (cellY * 8) - g_Pawn->BoundY;
			}
		}
		else if(g_Pawn->TargetY < g_Pawn->PositionY) // Go up
		{
			u8 cellX = (g_Pawn->TargetX + (g_Pawn->BoundX / 2)) / 8;
			u8 cellY = (g_Pawn->TargetY) / 8;		
			u8 tile = VDP_Peek_16K(g_ScreenLayoutLow + (cellY * 32) + cellX);
			if(g_Pawn->CollisionCB(tile))
			{
				g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_UP, tile);
				g_Pawn->TargetY = (cellY * 8) + 8;
			}
		}
		else // if(g_Pawn->TargetY == g_Pawn->PositionY)
		{
			u8 cellX = (g_Pawn->TargetX + (g_Pawn->BoundX / 2)) / 8;
			u8 cellY = (g_Pawn->TargetY + g_Pawn->BoundY) / 8;		
			u8 tile = VDP_Peek_16K(g_ScreenLayoutLow + (cellY * 32) + cellX);
			if(!g_Pawn->CollisionCB(tile))
			{
				g_Pawn->PhysicsCB(PAWN_PHYSICS_FALL, tile);
			}
		}

		// Horizontal movement
		if(g_Pawn->TargetX > g_Pawn->PositionX) // Go right
		{
			u8 cellX = (g_Pawn->TargetX + g_Pawn->BoundX) / 8;
			u8 cellY = (g_Pawn->TargetY + (g_Pawn->BoundY / 2)) / 8;		
			u8 tile = VDP_Peek_16K(g_ScreenLayoutLow + (cellY * 32) + cellX);
			if(g_Pawn->CollisionCB(tile))
			{
				g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_RIGHT, tile);
				g_Pawn->TargetX = (cellX * 8) - g_Pawn->BoundX;
			}
		}
		else if(g_Pawn->TargetX < g_Pawn->PositionX) // Go left
		{
			u8 cellX = (g_Pawn->TargetX) / 8;
			u8 cellY = (g_Pawn->TargetY + (g_Pawn->BoundY / 2)) / 8;		
			u8 tile = VDP_Peek_16K(g_ScreenLayoutLow + (cellY * 32) + cellX);
			if(g_Pawn->CollisionCB(tile))
			{
				g_Pawn->PhysicsCB(PAWN_PHYSICS_COL_LEFT, tile);
				g_Pawn->TargetX = (cellX * 8) + 8;
			}
		}
		
		g_Pawn->PositionX = g_Pawn->TargetX;
		g_Pawn->PositionY = g_Pawn->TargetY;
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
	u16 dest = g_SpriteAtributeLow + (g_Pawn->SpriteID * 4);
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

		g_Game_DrawY = g_Pawn->PositionY + g_Sprite->OffsetY;
		g_Game_DrawX = g_Pawn->PositionX + g_Sprite->OffsetX;
		u8 size = 2;
		
		if(g_Pawn->Update & PAWN_UPDATE_PATTERN)
		{
			g_Game_DrawPattern = g_Pawn->AnimFrame + g_Sprite->DataOffset;
			size++;
		}

		VDP_WriteVRAM((u8*)&g_Game_DrawY, dest, g_SpriteAtributeHigh, size);
		dest += 4;
		
	SkipDrawing:
		g_Sprite++;
	}
	g_Pawn->Update = 0;
}


#if (GAMEPAWN_USE_PHYSICS)
//-----------------------------------------------------------------------------
// Set pawn target position
void GamePawn_SetTargetPosition(Game_Pawn* pawn, u8 x, u8 y) 
{ 
	pawn->TargetX = x;
	pawn->TargetY = y;
	pawn->Update |= PAWN_UPDATE_COLLISION;
}

//-----------------------------------------------------------------------------
// Set pawn physics callback
void GamePawn_InitializePhysics(Game_Pawn* pawn, Game_PhysicsCB pcb, Game_CollisionCB ccb, u8 boundX, u8 boundY)
{ 
	pawn->PhysicsCB = pcb; 
	pawn->CollisionCB = ccb;
	pawn->BoundX = boundX;
	pawn->BoundY = boundY;
}

#endif