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

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize game pawn
void GamePawn_Initialize(Game_Pawn* pawn, const Game_Sprite* sprtList, u8 sprtNum, const Game_Action* actList)
{
	pawn->SpriteList = sprtList;
	pawn->SpriteNum = sprtNum;
	pawn->ActionList = actList;
	pawn->ActionId = 0xFF;
	pawn->AnimFrame = 0xFF;
	pawn->Update = 0xFF;
	pawn->Counter = 0;
	GamePawn_SetAction(pawn, 0);
	
	loop(i, pawn->SpriteNum)
	{
		const Game_Sprite* sprt = &pawn->SpriteList[i];
		VDP_SetSpriteColorSM1(sprt->SpriteID, sprt->Color);
	}

}

//-----------------------------------------------------------------------------
// Set game pawn position
void GamePawn_SetPosition(Game_Pawn* pawn, u8 x, u8 y)
{
	pawn->PositionX = x;
	pawn->PositionY = y;
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
	const Game_Action* act = &pawn->ActionList[pawn->ActionId];

	// Finished current animation step
	if(pawn->AnimTimer >= act->FrameList[pawn->AnimStep].Duration)
	{
		pawn->AnimTimer = 0;
		pawn->AnimStep++;
		pawn->Update |= PAWN_UPDATE_PATTERN;
	}

	// Finished last animation step
	if(pawn->AnimStep >= act->FrameNum)
	{
		if(act->Loop) // restart action
		{
			pawn->AnimTimer = 0;
			pawn->AnimStep = 0;
			pawn->Update |= PAWN_UPDATE_PATTERN;
		}
		else // stop action and transit to default action
		{
			GamePawn_SetAction(pawn, 0);
			return;
		}
	}
	
	// Execute event
	if(act->FrameList[pawn->AnimStep].Event != 0)
		act->FrameList[pawn->AnimStep].Event();
	
	// Update animation
	pawn->AnimFrame = act->FrameList[pawn->AnimStep].Id;
	pawn->AnimTimer++;
	pawn->Counter++;
}

//-----------------------------------------------------------------------------
// Update rendering of the game pawn
void GamePawn_Draw(Game_Pawn* pawn)
{
	if(pawn->Update == 0)
		return;

	loop(i, pawn->SpriteNum)
	{
		const Game_Sprite* sprt = &pawn->SpriteList[i];
		
		if(sprt->Flag & PAWN_SPRITE_EVEN)
		{
			if((pawn->Counter & 1) != 0)
				continue;
		}
		else if(sprt->Flag & PAWN_SPRITE_ODD)
		{
			if((pawn->Counter & 1) == 0)
				continue;
		}

		if(pawn->Update & PAWN_UPDATE_POSITION)
		{
			u8 x = pawn->PositionX + sprt->OffsetX;
			u8 y = pawn->PositionY + sprt->OffsetY;	
			VDP_SetSpritePosition(sprt->SpriteID, x, y);
		}
		
		if(sprt->Flag || (pawn->Update & PAWN_UPDATE_PATTERN))
		{
			u8 pattern = (pawn->AnimFrame * sprt->DataMultiply) + sprt->DataOffset;
			VDP_SetSpritePattern(sprt->SpriteID, pattern);
		}
	}
	pawn->Update = 0;
}

