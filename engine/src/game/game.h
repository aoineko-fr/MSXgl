// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄           
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██   ▄▀██ ▄█▄█ ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄█ ▀▄██ ██ █ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Game state handler
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//-----------------------------------------------------------------------------
//   G A M E
//-----------------------------------------------------------------------------

// Group: Game Core

//-----------------------------------------------------------------------------
// FUNCTIONS

// Function: Game_Initialize
// Initialize game module
//
// Parameters:
//   screenMode - The screen mode to select at game start. See <VDP_MODE>
void Game_Initialize(u8 screenMode);

// Function: Game_Update
// Update game frame
void Game_Update();

// Function: Game_Release
// Release game module
void Game_Release();


//-----------------------------------------------------------------------------
//   G A M E   L O O P
//-----------------------------------------------------------------------------
#if (GAME_USE_LOOP)
// Group: Game Loop

//-----------------------------------------------------------------------------
// RAM DATA

extern bool g_GameExit;

//-----------------------------------------------------------------------------
// FUNCTIONS

// Function: Game_MainLoop
// Game main loop
//
// Parameters:
//   screenMode - The screen mode to select at game start. See <VDP_MODE>
void Game_MainLoop(u8 screenMode);

// Function: Game_Exit
// Game exit
inline void Game_Exit() { g_GameExit = TRUE; }

#endif


//-----------------------------------------------------------------------------
//   G A M E   S T A T E
//-----------------------------------------------------------------------------
#if (GAME_USE_STATE)
// Group: Game State

//-----------------------------------------------------------------------------
// DEFINES

// Functions
typedef bool (*State)(void);	// Callback default signature

//-----------------------------------------------------------------------------
// RAM DATA

extern State g_GameState;
extern State g_GamePrevState;

//-----------------------------------------------------------------------------
// FUNCTIONS

// Function: Game_SetState
// Set the next state (change will be effective at the next state update)
//
// Parameters:
//   newState - The new state to execute
void Game_SetState(State newState);

// Function: Game_RestoreState
// Restore the previous state
void Game_RestoreState();

// Function: Game_UpdateState
// Check state transition and update current state
void Game_UpdateState();

// Function: Game_GetCurrentState
// Get current state
//
// Return:
//   Address of the current state function
inline State Game_GetCurrentState() { return g_GameState; }

#endif // (GAME_USE_STATE)


//-----------------------------------------------------------------------------
//   G A M E   V - S Y N C
//-----------------------------------------------------------------------------
#if (GAME_USE_VSYNC)
// Group: Game V-Synch

//-----------------------------------------------------------------------------
// RAM DATA

extern bool	g_GameVSync;
extern u8 g_GameFrame;
extern callback g_GameVSyncCB;

#if (GAME_USE_SYNC_50HZ)
extern bool g_Game60Hz;
extern u8 g_GameCount;
#endif

//-----------------------------------------------------------------------------
// FUNCTIONS

// Function: Game_VSyncHook
// Vertical-synchronization hook handler
void Game_VSyncHook();

// Function: Game_SetVSyncCallback
// Set V-Sync callback
//
// Parameters:
//   cb - The v-sync function to set
inline void Game_SetVSyncCallback(callback cb) { g_GameVSyncCB = cb; }

// Function: Game_GetFrameCount
// Get frame counter
//
// Return:
//   Current frame counter (0-255).
inline u8 Game_GetFrameCount() { return g_GameFrame; }

// Function: Game_WaitVSync
// Wait for vertical-synchronization 
void Game_WaitVSync();

#endif // (GAME_USE_VSYNC)
