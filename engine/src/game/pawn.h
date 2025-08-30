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

// PAWN_ID_PER_LAYER
#ifndef PAWN_ID_PER_LAYER
	#warning PAWN_ID_PER_LAYER is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#ifdef GAMEPAWN_ID_PER_LAYER
		#define PAWN_ID_PER_LAYER		GAMEPAWN_ID_PER_LAYER
	#else
		#define PAWN_ID_PER_LAYER		FALSE
	#endif
#endif

// PAWN_USE_RT_LOAD
#ifndef PAWN_USE_RT_LOAD
	#warning PAWN_USE_RT_LOAD is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define PAWN_USE_RT_LOAD			FALSE
#endif

// PAWN_USE_SPRT_FX
#ifndef PAWN_USE_SPRT_FX
	#warning PAWN_USE_SPRT_FX is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define PAWN_USE_SPRT_FX			FALSE
#endif

// PAWN_USE_PHYSICS
#ifndef PAWN_USE_PHYSICS
	#warning PAWN_USE_PHYSICS is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#ifdef GAMEPAWN_USE_PHYSICS
		#define PAWN_USE_PHYSICS		GAMEPAWN_USE_PHYSICS
	#else
		#define PAWN_USE_PHYSICS		FALSE
	#endif
#endif

// PAWN_UNIT
#ifndef PAWN_UNIT
	#warning PAWN_UNIT is not defined in "msxgl_config.h"! Default value will be used: PAWN_UNIT_SCREEN
	#define PAWN_UNIT					PAWN_UNIT_SCREEN
#endif

// PAWN_SPRITE_SIZE
#ifndef PAWN_SPRITE_SIZE
	#warning PAWN_SPRITE_SIZE is not defined in "msxgl_config.h"! Default value will be used: 16
	#define PAWN_SPRITE_SIZE			16
#endif

// PAWN_BORDER_EVENT
#ifndef PAWN_BORDER_EVENT
	#warning PAWN_BORDER_EVENT is not defined in "msxgl_config.h"! Default value will be used: 0
	#ifdef GAMEPAWN_BORDER_EVENT
		#define PAWN_BORDER_EVENT		GAMEPAWN_BORDER_EVENT
	#else
		#define PAWN_BORDER_EVENT		0
	#endif
#endif

// PAWN_BORDER_BLOCK
#ifndef PAWN_BORDER_BLOCK
	#warning PAWN_BORDER_BLOCK is not defined in "msxgl_config.h"! Default value will be used: 0
	#ifdef GAMEPAWN_BORDER_BLOCK
		#define PAWN_BORDER_BLOCK		GAMEPAWN_BORDER_BLOCK
	#else
		#define PAWN_BORDER_BLOCK		0
	#endif
#endif

// PAWN_BORDER_MIN_Y
#ifndef PAWN_BORDER_MIN_Y
	#warning PAWN_BORDER_MIN_Y is not defined in "msxgl_config.h"! Default value will be used: 0
	#ifdef GAMEPAWN_BORDER_MIN_Y
		#define PAWN_BORDER_MIN_Y		GAMEPAWN_BORDER_MIN_Y
	#else
		#define PAWN_BORDER_MIN_Y		0
	#endif
#endif

// PAWN_BORDER_MAX_Y
#ifndef PAWN_BORDER_MAX_Y
	#warning PAWN_BORDER_MAX_Y is not defined in "msxgl_config.h"! Default value will be used: 192
	#ifdef GAMEPAWN_BORDER_MAX_Y
		#define PAWN_BORDER_MAX_Y		GAMEPAWN_BORDER_MAX_Y
	#else
		#define PAWN_BORDER_MAX_Y		192
	#endif
#endif

// PAWN_COL_DOWN
#ifndef PAWN_COL_DOWN
	#warning PAWN_COL_DOWN is not defined in "msxgl_config.h"! Default value will be used: PAWN_COL_50
	#ifdef GAMEPAWN_COL_DOWN
		#define PAWN_COL_DOWN			GAMEPAWN_COL_DOWN
	#else
		#define PAWN_COL_DOWN			PAWN_COL_50
	#endif
#endif

// PAWN_COL_UP
#ifndef PAWN_COL_UP
	#warning PAWN_COL_UP is not defined in "msxgl_config.h"! Default value will be used: PAWN_COL_50
	#ifdef GAMEPAWN_COL_UP
		#define PAWN_COL_UP				GAMEPAWN_COL_UP
	#else
		#define PAWN_COL_UP				PAWN_COL_50
	#endif
#endif

// PAWN_COL_RIGHT
#ifndef PAWN_COL_RIGHT
	#warning PAWN_COL_RIGHT is not defined in "msxgl_config.h"! Default value will be used: PAWN_COL_50
	#ifdef GAMEPAWN_COL_RIGHT
		#define PAWN_COL_RIGHT			GAMEPAWN_COL_RIGHT
	#else
		#define PAWN_COL_RIGHT			PAWN_COL_50
	#endif
#endif

// PAWN_COL_LEFT
#ifndef PAWN_COL_LEFT
	#warning PAWN_COL_LEFT is not defined in "msxgl_config.h"! Default value will be used: PAWN_COL_50
	#ifdef GAMEPAWN_COL_LEFT
		#define PAWN_COL_LEFT			GAMEPAWN_COL_LEFT
	#else
		#define PAWN_COL_LEFT			PAWN_COL_50
	#endif
#endif

// PAWN_BOUND_X
#ifndef PAWN_BOUND_X
	#warning PAWN_BOUND_X is not defined in "msxgl_config.h"! Default value will be used: PAWN_BOUND_CUSTOM
	#ifdef GAMEPAWN_BOUND_X
		#define PAWN_BOUND_X			GAMEPAWN_BOUND_X
	#else
		#define PAWN_BOUND_X			PAWN_BOUND_CUSTOM
	#endif
#endif

// PAWN_BOUND_Y
#ifndef PAWN_BOUND_Y
	#warning PAWN_BOUND_Y is not defined in "msxgl_config.h"! Default value will be used: PAWN_BOUND_CUSTOM
	#ifdef GAMEPAWN_BOUND_Y
		#define PAWN_BOUND_Y			GAMEPAWN_BOUND_Y
	#else
		#define PAWN_BOUND_Y			PAWN_BOUND_CUSTOM
	#endif
#endif

// PAWN_TILEMAP_WIDTH
#ifndef PAWN_TILEMAP_WIDTH
	#warning PAWN_TILEMAP_WIDTH is not defined in "msxgl_config.h"! Default value will be used: 32
	#ifdef GAMEPAWN_TILEMAP_WIDTH
		#define PAWN_TILEMAP_WIDTH		GAMEPAWN_TILEMAP_WIDTH
	#else
		#define PAWN_TILEMAP_WIDTH		32
	#endif
#endif

// PAWN_TILEMAP_HEIGHT
#ifndef PAWN_TILEMAP_HEIGHT
	#warning PAWN_TILEMAP_HEIGHT is not defined in "msxgl_config.h"! Default value will be used: 24
	#ifdef GAMEPAWN_TILEMAP_HEIGHT
		#define PAWN_TILEMAP_HEIGHT		GAMEPAWN_TILEMAP_HEIGHT
	#else
		#define PAWN_TILEMAP_HEIGHT		24
	#endif
#endif

// PAWN_TILEMAP_SRC
#ifndef PAWN_TILEMAP_SRC
	#warning PAWN_TILEMAP_SRC is not defined in "msxgl_config.h"! Default value will be used: PAWN_TILEMAP_SRC_AUTO
	#ifdef GAMEPAWN_TILEMAP_SRC
		#define PAWN_TILEMAP_SRC		GAMEPAWN_TILEMAP_SRC
	#else
		#define PAWN_TILEMAP_SRC		PAWN_TILEMAP_SRC_AUTO
	#endif
#endif

// Handle tilemap source backward compatibility
#if (PAWN_TILEMAP_SRC == PAWN_TILEMAP_SRC_AUTO)
	#undef PAWN_TILEMAP_SRC
	#if (defined(PAWN_USE_VRAM_COL) && PAWN_USE_VRAM_COL)
		#define PAWN_TILEMAP_SRC	PAWN_TILEMAP_SRC_VRAM
	#else
		#define PAWN_TILEMAP_SRC	PAWN_TILEMAP_SRC_RAM
	#endif
#endif

// PAWN_SPT_MODE
#ifndef PAWN_SPT_MODE
	#warning PAWN_SPT_MODE is not defined in "msxgl_config.h"! Default value will be used: PAWN_SPT_MODE_AUTO
	#ifdef GAMEPAWN_SPT_MODE
		#define PAWN_SPT_MODE			GAMEPAWN_SPT_MODE
	#else
		#define PAWN_SPT_MODE			PAWN_SPT_MODE_AUTO
	#endif
#endif

// Handle sprite mode backward compatibility
#if (PAWN_SPT_MODE == PAWN_SPT_MODE_AUTO)
	#undef PAWN_SPT_MODE
	#if (defined(PAWN_FORCE_SM1) && PAWN_FORCE_SM1)
		#define PAWN_SPT_MODE		PAWN_SPT_MODE_MSX1
	#elif (MSX_VERSION >= MSX_2)
		#define PAWN_SPT_MODE		PAWN_SPT_MODE_MSX2
	#else
		#define PAWN_SPT_MODE		PAWN_SPT_MODE_MSX1
	#endif
#endif


#if (PAWN_UNIT == PAWN_UNIT_SCREEN)
	// Pawn coordinate unit
	#define PAWN_POS				u8
#else // (PAWN_UNIT == PAWN_UNIT_QMN())
	// Pawn coordinate unit
	#define PAWN_POS				u16
#endif

//=============================================================================
// DEFINES
//=============================================================================

// Animation frame structure (one pose of the pawn)
typedef struct Pawn_Frame
{
	u8					Id;       // Animation frame data index (0-255)
	u8					Duration; // Frame duration (in frame number)
	callback			Event;    // Event to execute at this frame
} Pawn_Frame;

// Animation action structure
typedef struct Pawn_Action
{
	const Pawn_Frame*	FrameList;     // Animation frames data
	u8					FrameNum;      // Animation frames data count
	u8					Loop; // Is action looping?
	u8					Interrupt; // Is action interruptable?
	// u8					Loop      : 1; // Is action looping?
	// u8					Interrupt : 1; // Is action interruptable?
} Pawn_Action;

// Pawn sprite flags
enum PAWN_SPRITE_FLAG
{
	PAWN_SPRITE_BLEND = 0b00000001,
	PAWN_SPRITE_OR    = 0b00000100,
};

// Pawn structure
typedef struct Pawn_Sprite
{
#if (PAWN_ID_PER_LAYER)
	u8					SpriteID;     // Sprite ID
#endif
	i8					OffsetX;      // Layer position offset...
	i8					OffsetY;      // ...can be positive or negative
	u8					DataOffset;   // Data index offset
	u8					Color;        // Sprite color
	u8					Flag;         // Sprite flag
} Pawn_Sprite;

// Pawn update flags
enum PAWN_UPDATE_FLAG
{
	PAWN_UPDATE_POSITION  = 0b00000001,
	PAWN_UPDATE_PATTERN   = 0b00000010,
	PAWN_UPDATE_2         = 0b00000100,
	PAWN_UPDATE_3         = 0b00001000,

	PAWN_UPDATE_MASK	  = 0b00001111,

	#if (PAWN_USE_PHYSICS)
	PAWN_UPDATE_COLLISION = 0b00010000,
	PAWN_UPDATE_PHYSICS   = 0b00100000,
	#endif
	PAWN_UPDATE_BLEND     = 0b01000000,
	PAWN_UPDATE_DISABLE   = 0b10000000,
};

#if (PAWN_USE_PHYSICS)
// Physics callback
typedef void (*Pawn_PhysicsCB)(u8 event, u8 tile);

// Collision callback
typedef bool (*Pawn_CollisionCB)(u8 tile);

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
	#if (PAWN_BORDER_EVENT || PAWN_BORDER_BLOCK)
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

#if (PAWN_USE_RT_LOAD && PAWN_USE_SPRT_FX)
enum PAWN_SPRITE_FX
{
	PAWN_SPRITE_FX_NONE = 0,
	PAWN_SPRITE_FX_FLIP_X,
	PAWN_SPRITE_FX_FLIP_Y,
	PAWN_SPRITE_FX_ROTATE_90,
	PAWN_SPRITE_FX_ROTATE_180,
	PAWN_SPRITE_FX_ROTATE_270,
};
#endif

// Pawn structure
typedef struct Pawn
{
	const Pawn_Sprite*	SpriteList;		// List of sprite layers
	u8					SpriteNum;		// Number of sprite layers
#if !(PAWN_ID_PER_LAYER)
	u8					SpriteID;		// Sprite ID of the first layer (0~31)
#endif
	const Pawn_Action*	ActionList;		// List of actions
	PAWN_POS			PositionX;		// Pawn screen position
	PAWN_POS			PositionY;
	u8					ActionId;		// Current action id
	u8					AnimFrame;		// Current animation frame id
	u8					AnimStep;		// Current step into the animation
	u8					AnimTimer;		// Animation timer (into the current step)
	u8					Update;			// Pawn update flags
	u8					Counter;		// Global update counter
#if (PAWN_USE_RT_LOAD)
	const void*			PatternAddr;	// Pattern base address
#if (PAWN_USE_SPRT_FX)
	u8					SpriteFX;		// Sprite effect
#endif
#endif
#if (PAWN_USE_PHYSICS)
	i8					MoveX;			// Pawn movement offset
	i8					MoveY;
#if (PAWN_BOUND_X == PAWN_BOUND_CUSTOM)
	u8					BoundX;			// Pawn collision bound
#endif
#if (PAWN_BOUND_Y == PAWN_BOUND_CUSTOM)
	u8					BoundY;
#endif
	u8					PhysicsState;
	Pawn_PhysicsCB		PhysicsCB;
	Pawn_CollisionCB	CollisionCB;
#endif
} Pawn;

// Tile map buffer in RAM
#if (PAWN_TILEMAP_SRC == PAWN_TILEMAP_SRC_RAM)
extern const u8* g_Pawn_TileMap;
#endif

// Current cell coordinate
#if (PAWN_USE_PHYSICS)
extern u8 g_Pawn_CellX;
extern u8 g_Pawn_CellY;
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================
// Group: Core

// Function: Pawn_Initialize
// Initialize a game pawn.
//
// Parameters:
//   pawn     - Address of game pawn structure to initialize. This is he structure to be used for all other functions.
//   sprtList - Array of all sprite layers used to display the pawn. See <Pawn_Sprite> for details.
//   sprtNum  - Size of the 'sprtList' table (in entries number).  
//   sprtID   - First srite index to use for the pawn. The pawn will use all sprite indexes from 'sprtID' to 'sprtID + sprtNum - 1'.
//              If <PAWN_ID_PER_LAYER> compile option is activated, this parmeter is not used as each sprit layer define it's own sprite index.
//   actList  - Array of pawn action. Needed to use <Pawn_SetAction> function.
void Pawn_Initialize(Pawn* pawn, const Pawn_Sprite* sprtList, u8 sprtNum, u8 sprtID, const Pawn_Action* actList);

// Function: Pawn_SetPosition
// Set game pawn position. Force movement even if collision is activated.
//
// Parameters:
//   pawn - Address of game pawn structure to move.
//   x    - New X coordinate (in pixels).
//   y    - New Y coordinate (in pixels).
void Pawn_SetPosition(Pawn* pawn, PAWN_POS x, PAWN_POS y);

// Function: Pawn_GetPositionX
// Get the X coordinate of the pawn's position.
//
// Parameters:
//   pawn - Pointer to pawn structure.
//
// Return:
//   X coordinate of the pawn's position.
inline PAWN_POS Pawn_GetPositionX(Pawn* pawn) { return pawn->PositionX; }

// Function: Pawn_GetPositionY
// Get the Y coordinate of the pawn's position.
//
// Parameters:
//   pawn - Pointer to pawn structure.
//
// Return:
//   Y coordinate of the pawn's position.
inline PAWN_POS Pawn_GetPositionY(Pawn* pawn) { return pawn->PositionY; }

// Function: Pawn_ForceSetAction
// Force to set game pawn action id.
//
// Parameters:
//   pawn - Address of game pawn structure to setup.
//   id   - New action index.
void Pawn_ForceSetAction(Pawn* pawn, u8 id);

// Function: Pawn_SetAction
// Set game pawn current action index.
//
// Parameters:
//   pawn - Address of game pawn structure to setup.
//   id   - New action index.
void Pawn_SetAction(Pawn* pawn, u8 id);

// Function: Pawn_RestartAction
// Restart the current action.
//
// Parameters:
//   pawn - Address of game pawn structure to setup.
inline void Pawn_RestartAction(Game_Pawn* pawn) { pawn->AnimTimer = 0; pawn->AnimStep = 0; pawn->Update |= PAWN_UPDATE_PATTERN; }

// Function: Pawn_GetAction
// Get the current action ID of the pawn.
//
// Parameters:
//   pawn - Pointer to pawn structure.
//
// Return:
//   Current action ID of the pawn.
inline u8 Pawn_GetAction(Pawn* pawn) { return pawn->ActionId; }

#if (PAWN_TILEMAP_SRC == PAWN_TILEMAP_SRC_RAM)
// Function: Pawn_SetTileMap
// Set the tile map to be used for collision.
// Available only when PAWN_TILEMAP_SRC is set to PAWN_TILEMAP_SRC_RAM.
//
// Parameters:
//   map - Pointer to RAM buffer with the tile map. Size must be PAWN_TILEMAP_WIDTH * PAWN_TILEMAP_HEIGHT.
inline void Pawn_SetTileMap(const u8* map) { g_Pawn_TileMap = map; }
#endif

// Function: Pawn_SetEnable
// Set if pawn is enable or disable. Disabled pawn will not be updated nor drawn.
//
// Parameters:
//   pawn - Address of game pawn structure to update (move, collision, etc.).
//   enable - TRUE to enable the pawn, FALSE to disable it.
void Pawn_SetEnable(Pawn* pawn, bool enable);

// Function: Pawn_Disable
// Disable a pawn. Disabled pawn will not be updated nor drawn.
//
// Parameters:
//   pawn - Address of game pawn structure to update (move, collision, etc.).
inline void Pawn_Disable(Pawn* pawn) { Pawn_SetEnable(pawn, FALSE); }

// Function: Pawn_Enable
// Enable a pawn. Enabled pawn will be updated and drawn.
//
// Parameters:
//   pawn - Address of game pawn structure to update (move, collision, etc.).
inline void Pawn_Enable(Pawn* pawn) { Pawn_SetEnable(pawn, TRUE); }

// Function: Pawn_Update
// Update animation of the game pawn. Must be called once a frame.
//
// Parameters:
//   pawn - Address of game pawn structure to update (move, collision, etc.).
void Pawn_Update(Pawn* pawn);

// Function: Pawn_SetDirty
// Mark the pawn as dirty. This will force the pawn to be redrawn.
//
// Parameters:
//   pawn - Address of game pawn structure to update (move, collision, etc.).
inline void Pawn_SetDirty(Pawn* pawn) { pawn->Update |= (PAWN_UPDATE_PATTERN | PAWN_UPDATE_POSITION); }

// Function: Pawn_SetColorBlend
// Enable or disable sprite color blending effect.
//
// Parameters:
//   pawn   - Address of game pawn structure to update (move, collision, etc.).
//   enable - TRUE to enable color blending, FALSE to disable it.
inline void Pawn_SetColorBlend(Pawn* pawn, bool enable) { if (enable) pawn->Update |= PAWN_UPDATE_BLEND; else pawn->Update &= ~PAWN_UPDATE_BLEND; }

// Function: Pawn_Draw
// Update rendering of the game pawn. Must be called once a frame.
//
// Parameters:
//   pawn - Address of game pawn structure to draw.
void Pawn_Draw(Pawn* pawn);


#if (PAWN_USE_RT_LOAD)
// Function: Pawn_SetPatternAddress
// Set the base address of the sprite pattern data.
// Only available if PAWN_USE_RT_LOAD compile option is set to TRUE.
//
// Parameters:
//   pawn - Address of game pawn structure to initialize.
//   addr - Base address of the sprite pattern data.
inline void Pawn_SetPatternAddress(Pawn* pawn, const void* addr) { pawn->PatternAddr = addr; }

#if (PAWN_USE_SPRT_FX)
// Function: Pawn_SetSpriteFX
// Set sprite effect.
// Only available if PAWN_USE_RT_LOAD and PAWN_USE_SPRT_FX compile options are set to TRUE.
//
// Parameters:
//   pawn - Address of game pawn structure to initialize.
//   fx   - Sprite effect (crop, flip, mask, rotate).
inline void Pawn_SetSpriteFX(Pawn* pawn, u8 fx) { pawn->SpriteFX = fx; }
#endif

#endif


#if (PAWN_USE_PHYSICS)
// Group: Physics

// Function: Pawn_SetMovement
// Set pawn relative movement. Collision can prevent part of the movement. 
// Only available if PAWN_USE_PHYSICS compile option is set to TRUE.
//
// Parameters:
//   pawn - Address of game pawn structure to move.
//   x    - New X movement (in pixels).
//   y    - New Y movement (in pixels).
void Pawn_SetMovement(Pawn* pawn, i8 dx, i8 dy);

// Function: Pawn_SetTargetPosition
// Set pawn absolute movement. Collision can prevent part of the movement.
// Only available if PAWN_USE_PHYSICS compile option is set to TRUE.
//
// Parameters:
//   pawn - Address of game pawn structure to move.
//   x    - New X coordinate (in pixels).
//   y    - New Y coordinate (in pixels).
inline void Pawn_SetTargetPosition(Pawn* pawn, PAWN_POS x, PAWN_POS y) { Pawn_SetMovement(pawn, (i8)((i16)x - pawn->PositionX), (i8)((i16)y - pawn->PositionY)); }

// Function: Pawn_SetPhysicsCallback
// Set pawn physics callback
// Only available if PAWN_USE_PHYSICS compile option is set to TRUE.
//
// Parameters:
//   pawn   - Address of game pawn structure to initialize.
//   pcb    - Callback function to be called when physics events occurs.
//   ccb    - Callback function to be called when collision events occurs.
//   boundX - Bounding box X extend (in pixels, from the pawn's origin).
//   boundY - Bounding box Y extend (in pixels, from the pawn's origin).
void Pawn_InitializePhysics(Pawn* pawn, Pawn_PhysicsCB pcb, Pawn_CollisionCB ccb, u8 boundX, u8 boundY);

// Function: Pawn_GetPhysicsState
// Get pawn physics state.
// Only available if PAWN_USE_PHYSICS compile option is set to TRUE.
//
// Parameters:
//   pawn - Pointer to pawn structure.
//
// Return:
//   Pawn's physics state.
inline u8 Pawn_GetPhysicsState(Pawn* pawn) { return pawn->PhysicsState; }

// Function: Pawn_GetCallbackCellX
// Get the X coordinate of the cell that triggered a callback.
// The value is only valid within a callback.
// Only available if PAWN_USE_PHYSICS compile option is set to TRUE.
//
// Return:
//   X coordinate of the current cell.
inline const u8 Pawn_GetCallbackCellX() { return g_Pawn_CellX; }

// Function: Pawn_GetCallbackCellY
// Get the Y coordinate of the cell that triggered a callback.
// The value is only valid within a callback.
// Only available if PAWN_USE_PHYSICS compile option is set to TRUE.
//
// Return:
//   Y coordinate of the current cell.
inline const u8 Pawn_GetCallbackCellY() { return g_Pawn_CellY; }

#endif
