// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄           
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██   ▄▀██ ▄█▄█ ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄█ ▀▄██ ██ █ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Game pawn module
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// OPTIONS VALIDATION
//=============================================================================

// GAMEPAWN_ID_PER_LAYER
#ifndef GAMEPAWN_ID_PER_LAYER
	#warning GAMEPAWN_ID_PER_LAYER is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define GAMEPAWN_ID_PER_LAYER		FALSE
#endif

// GAMEPAWN_USE_PHYSICS
#ifndef GAMEPAWN_USE_PHYSICS
	#warning GAMEPAWN_USE_PHYSICS is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define GAMEPAWN_USE_PHYSICS		FALSE
#endif

// GAMEPAWN_BORDER_EVENT
#ifndef GAMEPAWN_BORDER_EVENT
	#warning GAMEPAWN_BORDER_EVENT is not defined in "msxgl_config.h"! Default value will be used: 0
	#define GAMEPAWN_BORDER_EVENT		0
#endif

// GAMEPAWN_BORDER_BLOCK
#ifndef GAMEPAWN_BORDER_BLOCK
	#warning GAMEPAWN_BORDER_BLOCK is not defined in "msxgl_config.h"! Default value will be used: 0
	#define GAMEPAWN_BORDER_BLOCK		0
#endif

// GAMEPAWN_BORDER_MIN_Y
#ifndef GAMEPAWN_BORDER_MIN_Y
	#warning GAMEPAWN_BORDER_MIN_Y is not defined in "msxgl_config.h"! Default value will be used: 0
	#define GAMEPAWN_BORDER_MIN_Y		0
#endif

// GAMEPAWN_BORDER_MAX_Y
#ifndef GAMEPAWN_BORDER_MAX_Y
	#warning GAMEPAWN_BORDER_MAX_Y is not defined in "msxgl_config.h"! Default value will be used: 192
	#define GAMEPAWN_BORDER_MAX_Y		192
#endif

// GAMEPAWN_COL_DOWN
#ifndef GAMEPAWN_COL_DOWN
	#warning GAMEPAWN_COL_DOWN is not defined in "msxgl_config.h"! Default value will be used: GAMEPAWN_COL_50
	#define GAMEPAWN_COL_DOWN			GAMEPAWN_COL_50
#endif

// GAMEPAWN_COL_UP
#ifndef GAMEPAWN_COL_UP
	#warning GAMEPAWN_COL_UP is not defined in "msxgl_config.h"! Default value will be used: GAMEPAWN_COL_50
	#define GAMEPAWN_COL_UP				GAMEPAWN_COL_50
#endif

// GAMEPAWN_COL_RIGHT
#ifndef GAMEPAWN_COL_RIGHT
	#warning GAMEPAWN_COL_RIGHT is not defined in "msxgl_config.h"! Default value will be used: GAMEPAWN_COL_50
	#define GAMEPAWN_COL_RIGHT			GAMEPAWN_COL_50
#endif

// GAMEPAWN_COL_LEFT
#ifndef GAMEPAWN_COL_LEFT
	#warning GAMEPAWN_COL_LEFT is not defined in "msxgl_config.h"! Default value will be used: GAMEPAWN_COL_50
	#define GAMEPAWN_COL_LEFT			GAMEPAWN_COL_50
#endif

// GAMEPAWN_BOUND_X
#ifndef GAMEPAWN_BOUND_X
	#warning GAMEPAWN_BOUND_X is not defined in "msxgl_config.h"! Default value will be used: GAMEPAWN_BOUND_CUSTOM
	#define GAMEPAWN_BOUND_X			GAMEPAWN_BOUND_CUSTOM
#endif

// GAMEPAWN_BOUND_Y
#ifndef GAMEPAWN_BOUND_Y
	#warning GAMEPAWN_BOUND_Y is not defined in "msxgl_config.h"! Default value will be used: GAMEPAWN_BOUND_CUSTOM
	#define GAMEPAWN_BOUND_Y			GAMEPAWN_BOUND_CUSTOM
#endif

// GAMEPAWN_FORCE_SM1
#ifndef GAMEPAWN_FORCE_SM1
	#warning GAMEPAWN_FORCE_SM1 is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define GAMEPAWN_FORCE_SM1			FALSE
#endif

// GAMEPAWN_USE_VRAM_COL
#ifndef GAMEPAWN_USE_VRAM_COL
	#warning GAMEPAWN_USE_VRAM_COL is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define GAMEPAWN_USE_VRAM_COL		TRUE
#endif

// GAMEPAWN_TILEMAP_WIDTH
#ifndef GAMEPAWN_TILEMAP_WIDTH
	#warning GAMEPAWN_TILEMAP_WIDTH is not defined in "msxgl_config.h"! Default value will be used: 32
	#define GAMEPAWN_TILEMAP_WIDTH		32
#endif

// GAMEPAWN_TILEMAP_HEIGHT
#ifndef GAMEPAWN_TILEMAP_HEIGHT
	#warning GAMEPAWN_TILEMAP_HEIGHT is not defined in "msxgl_config.h"! Default value will be used: 24
	#define GAMEPAWN_TILEMAP_HEIGHT		24
#endif

//=============================================================================
// DEFINES
//=============================================================================

// Animation frame structure (one pose of the pawn)
typedef struct
{
	u8					Id;       // Animation frame data index (0-255)
	u8					Duration; // Frame duration (in frame number)
	callback			Event;    // Event to execute at this frame
} Game_Frame;

// Animation action structure
typedef struct
{
	const Game_Frame*	FrameList;     // Animation frames data
	u8					FrameNum;      // Animation frames data count
	u8					Loop      : 1; // Is action looping?
	u8					Interrupt : 1; // Is action interruptable?
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
#if (GAMEPAWN_ID_PER_LAYER)
	u8					SpriteID;     // Sprite ID
#endif
	i8					OffsetX;      // Layer position offset...
	i8					OffsetY;      // ...can be positive or negative
	u8					DataOffset;   // Data index offset
	u8					Color;        // Sprite color
	u8					Flag;         // Sprite flag
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
	// Border
	#if (GAMEPAWN_BORDER_EVENT || GAMEPAWN_BORDER_BLOCK)
		PAWN_PHYSICS_BORDER_DOWN,
		PAWN_PHYSICS_BORDER_UP,
		PAWN_PHYSICS_BORDER_RIGHT,
		PAWN_PHYSICS_BORDER_LEFT,
	#endif
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
	const Game_Sprite*	SpriteList;		// List of sprite layers
	u8					SpriteNum;		// Number of sprite layers
#if !(GAMEPAWN_ID_PER_LAYER)
	u8					SpriteID;		// Sprite ID of the first layer (0~31)
#endif
	const Game_Action*	ActionList;		// List of actions
	u8					PositionX;		// Pawn screen position
	u8					PositionY;
	u8					ActionId;		// Current action id
	u8					AnimFrame;		// Current animation frame id
	u8					AnimStep;		// Current step into the animation
	u8					AnimTimer;		// Animation timer (into the current step)
	u8					Update;			// Pawn update flags
	u8					Counter;		// Global update counter
#if (GAMEPAWN_USE_PHYSICS)
	i8					MoveX;			// Pawn movement offset
	i8					MoveY;
#if (GAMEPAWN_BOUND_X == GAMEPAWN_BOUND_CUSTOM)
	u8					BoundX;			// Pawn collision bound
#endif
#if (GAMEPAWN_BOUND_Y == GAMEPAWN_BOUND_CUSTOM)
	u8					BoundY;
#endif
	u8					PhysicsState;
	Game_PhysicsCB		PhysicsCB;
	Game_CollisionCB	CollisionCB;
#endif
} Game_Pawn;

#if (GAMEPAWN_BOUND_X == GAMEPAWN_BOUND_CUSTOM)
	#define GET_BOUND_X		g_Pawn->BoundX
#else
	#define GET_BOUND_X		GAMEPAWN_BOUND_X
#endif

#if (GAMEPAWN_BOUND_Y == GAMEPAWN_BOUND_CUSTOM)
	#define GET_BOUND_Y		g_Pawn->BoundY
#else
	#define GET_BOUND_Y		GAMEPAWN_BOUND_Y
#endif

// Tile map getter macro
#if (GAMEPAWN_USE_VRAM_COL)
	#define GAMEPAWN_GET_TILE(X, Y)	VDP_Peek(g_ScreenLayoutLow + (Y * GAMEPAWN_TILEMAP_WIDTH) + X, g_ScreenLayoutHigh)
#else
	#define GAMEPAWN_GET_TILE(X, Y)	g_GamePawn_TileMap[(Y * GAMEPAWN_TILEMAP_WIDTH) + X]
	// Tile map buffer in RAM
	extern const u8* g_GamePawn_TileMap;
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================
// Group: Core

// Function: GamePawn_Initialize
// Initialize a game pawn.
//
// Parameters:
//   pawn     - Address of game pawn structure to initialize. This is he structure to be used for all other functions.
//   sprtList - Array of all sprite layers used to display the pawn. See <Game_Sprite> for details.
//   sprtNum  - Size of the 'sprtList' table (in entries number).  
//   sprtID   - First srite index to use for the pawn. The pawn will use all sprite indexes from 'sprtID' to 'sprtID + sprtNum - 1'.
//              If <GAMEPAWN_ID_PER_LAYER> compile option is activated, this parmeter is not used as each sprit layer define it's own sprite index.
//   actList  - Array of pawn action. Needed to use <GamePawn_SetAction> function.
void GamePawn_Initialize(Game_Pawn* pawn, const Game_Sprite* sprtList, u8 sprtNum, u8 sprtID, const Game_Action* actList);

// Function: GamePawn_SetPosition
// Set game pawn position. Force movement even if collision is activated.
//
// Parameters:
//   pawn - Address of game pawn structure to move.
//   x    - New X coordinate (in pixels).
//   y    - New Y coordinate (in pixels).
void GamePawn_SetPosition(Game_Pawn* pawn, u8 x, u8 y);

// Function: GamePawn_SetAction
// Set game pawn current action index.
//
// Parameters:
//   pawn - Address of game pawn structure to setup.
//   id   - New action index.
void GamePawn_SetAction(Game_Pawn* pawn, u8 id);

#if (!GAMEPAWN_USE_VRAM_COL)
// Function: GamePawn_SetTileMap
// Set the tile map to be used for collision.
// Available only when GAMEPAWN_USE_VRAM_COL is set to FALSE.
//
// Parameters:
//   map - Pointer to RAM buffer with the tile map. Size must be GAMEPAWN_TILEMAP_WIDTH * GAMEPAWN_TILEMAP_HEIGHT.
inline void GamePawn_SetTileMap(const u8* map) { g_GamePawn_TileMap = map; }
#endif

// Function: GamePawn_Update
// Update animation of the game pawn. Must be called once a frame.
//
// Parameters:
//   pawn - Address of game pawn structure to update (move, collision, etc.).
void GamePawn_Update(Game_Pawn* pawn);

// Function: GamePawn_Draw
// Update rendering of the game pawn. Must be called once a frame.
//
// Parameters:
//   pawn - Address of game pawn structure to draw.
void GamePawn_Draw(Game_Pawn* pawn);

#if (GAMEPAWN_USE_PHYSICS)
// Group: Physics

// Function: GamePawn_SetMovement
// Set pawn relative movement. Collision can prevent part of the movement. 
// Only available if GAMEPAWN_USE_PHYSICS compile option is set to TRUE.
//
// Parameters:
//   pawn - Address of game pawn structure to move.
//   x    - New X movement (in pixels).
//   y    - New Y movement (in pixels).
void GamePawn_SetMovement(Game_Pawn* pawn, i8 dx, i8 dy);

// Function: GamePawn_SetTargetPosition
// Set pawn absolute movement. Collision can prevent part of the movement.
// Only available if GAMEPAWN_USE_PHYSICS compile option is set to TRUE.
//
// Parameters:
//   pawn - Address of game pawn structure to move.
//   x    - New X coordinate (in pixels).
//   y    - New Y coordinate (in pixels).
inline void GamePawn_SetTargetPosition(Game_Pawn* pawn, u8 x, u8 y) { GamePawn_SetMovement(pawn, x - pawn->PositionX, y - pawn->PositionY); }

// Function: GamePawn_SetPhysicsCallback
// Set pawn physics callback
// Only available if GAMEPAWN_USE_PHYSICS compile option is set to TRUE.
//
// Parameters:
//   pawn   - Address of game pawn structure to initialize.
//   pcb    - Callback function to be called when physics events occurs.
//   ccb    - Callback function to be called when collision events occurs.
//   boundX - Bounding box X extend (in pixels, from the pawn's origin).
//   boundY - Bounding box Y extend (in pixels, from the pawn's origin).
void GamePawn_InitializePhysics(Game_Pawn* pawn, Game_PhysicsCB pcb, Game_CollisionCB ccb, u8 boundX, u8 boundY);

// Function: GamePawn_GetPhysicsState
// Get pawn physics state.
// Only available if GAMEPAWN_USE_PHYSICS compile option is set to TRUE.
//
// Parameters:
//   pawn - Pointer to pawn structure.
//
// Return:
//   Pawn's physics state.
inline u8 GamePawn_GetPhysicsState(Game_Pawn* pawn) { return pawn->PhysicsState; }

#endif













