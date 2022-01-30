// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄           
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██   ▄▀██ ▄█▄█ ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄█ ▀▄██ ██ █ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// Game pawn module
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"


//=============================================================================
// DEFINES
//=============================================================================

// Animation frame structure (one pose of the pawn)
typedef struct
{
	u8			       Id;       // Animation frame data index
	u8			       Duration; // Frame duration (in frame number)
	callback	       Event;    // Event to execute at this frame
} Game_Frame;

// Animation action structure
typedef struct
{
	const Game_Frame*  FrameList;     // Animation frames data
	u8			       FrameNum;      // Animation frames data count
	u8			       Loop      : 1; // Is action looping?
	u8			       Interrupt : 1; // Is action interruptable?
} Game_Action;

// Pawn structure
typedef struct
{
	u8                 SpriteID;     // Sprite ID (0~31)
	i8                 OffsetX;      // Layer position offset...
	i8                 OffsetY;      // ...can be positive or negative
	u8                 DataMultiply; // Data index multiplier
	u8                 DataOffset;   // Data index offset
	u8                 Color;        // Sprite color
} Game_Sprite;

// Pawn update flags
enum PAWN_UPDATE_FLAG
{
	PAWN_UPDATE_POSITION = 0b00000001,
	PAWN_UPDATE_PATTERN  = 0b00000010,
};

// Pawn structure
typedef struct
{
	const Game_Sprite* SpriteList; // List of sprite layers
	u8                 SpriteNum;  // Number of sprite layers
	const Game_Action* ActionList; // List of actions
	u8                 PositionX;  // Pawn screen position
	u8                 PositionY;
	u8                 ActionId;   // Current action id
	u8                 AnimFrame;  // Current animation frame id
	u8			       AnimStep;   // Current step into the animation
	u8			       AnimTimer;  // Animation timer (into the current step)
	u8			       Update;     // Pawn update flags
} Game_Pawn;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

// Function: GamePawn_Initialize
// Initialize game pawn
void GamePawn_Initialize(Game_Pawn* pawn);

// Function: GamePawn_SetPosition
// Set game pawn position
void GamePawn_SetPosition(Game_Pawn* pawn, u8 x, u8 y);

// Function: GamePawn_SetAction
// Set game pawn action id
void GamePawn_SetAction(Game_Pawn* pawn, u8 id);

// Function: GamePawn_Update
// Update animation of the game pawn
void GamePawn_Update(Game_Pawn* pawn);

// Function: GamePawn_Draw
// Update rendering of the game pawn
void GamePawn_Draw(Game_Pawn* pawn);