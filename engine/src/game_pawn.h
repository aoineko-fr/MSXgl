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


// Pawn sprite flags
enum PAWN_SPRITE_FLAG
{
	PAWN_SPRITE_EVEN = 0b00000001,
	PAWN_SPRITE_ODD  = 0b00000010,
	PAWN_SPRITE_OR   = 0b00000100,
};

// Pawn structure
typedef struct
{
	u8                 SpriteID;     // Sprite ID (0~31)
	i8                 OffsetX;      // Layer position offset...
	i8                 OffsetY;      // ...can be positive or negative
	u8                 DataMultiply; // Data index multiplier
	u8                 DataOffset;   // Data index offset
	u8                 Color;        // Sprite color
	u8                 Flag;         // Sprite flag
} Game_Sprite;

// Pawn update flags
enum PAWN_UPDATE_FLAG
{
	PAWN_UPDATE_POSITION  = 0b00000001,
	PAWN_UPDATE_PATTERN   = 0b00000010,
	#if (GAMEPAWN_USE_PHYSICS)
	PAWN_UPDATE_COLLISION = 0b00010000,
	PAWN_UPDATE_PHYSICS   = 0b00100000,
	#endif
};

#if (GAMEPAWN_USE_PHYSICS)
// Physics callback
typedef void (*Game_PhysicsCB)(u8 event, u8 tile);

// Collision callback
typedef bool (*Game_CollisionCB)(u8 tile);

// Pawn physics events
enum PAWN_PHYSICS_EVENT
{
	// Collision
	PAWN_PHYSICS_COL_DOWN  = 0,
	PAWN_PHYSICS_COL_UP,
	PAWN_PHYSICS_COL_RIGHT,
	PAWN_PHYSICS_COL_LEFT,
	// Physics
	PAWN_PHYSICS_FALL,
};

// Pawn physics states
enum PAWN_PHYSICS_STATE
{
	PAWN_PHYSICS_INAIR     = 0b00000001, // Grounded overwise
};
#endif


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
	u8                 Counter;    // Global update counter
#if (GAMEPAWN_USE_PHYSICS)
	u8                 TargetX;    // Pawn target position
	u8                 TargetY;
	u8                 BoundX;     // Pawn collision bound
	u8                 BoundY;
	u8                 PhysicsState;
	Game_PhysicsCB     PhysicsCB;
	Game_CollisionCB   CollisionCB;
#endif
} Game_Pawn;

//=============================================================================
// FUNCTIONS
//=============================================================================
// Group: Core

// Function: GamePawn_Initialize
// Initialize game pawn
void GamePawn_Initialize(Game_Pawn* pawn, const Game_Sprite* sprtList, u8 sprtNum, const Game_Action* actList);

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

#if (GAMEPAWN_USE_PHYSICS)
// Group: Physics

// Function: GamePawn_SetTargetPosition
// Set pawn target position
void GamePawn_SetTargetPosition(Game_Pawn* pawn, u8 x, u8 y) ;

// Function: GamePawn_SetPhysicsCallback
// Set pawn physics callback
void GamePawn_InitializePhysics(Game_Pawn* pawn, Game_PhysicsCB pcb, Game_CollisionCB ccb, u8 boundX, u8 boundY);

// Function: GamePawn_GetPhysicsState
// Get pawn physics state
inline u8 GamePawn_GetPhysicsState(Game_Pawn* pawn) { return pawn->PhysicsState; }
#endif













