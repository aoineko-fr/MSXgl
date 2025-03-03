// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄           
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██   ▄▀██ ▄█▄█ ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄█ ▀▄██ ██ █ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Game state handler
//─────────────────────────────────────────────────────────────────────────────
#include "game/game.h"
#include "vdp.h"
#include "bios_hook.h"
#include "bios_mainrom.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#if (GAME_USE_VSYNC)

// Function prototype
void Game_DefaultVSyncCB();

#endif

//-----------------------------------------------------------------------------
// Memory data
//-----------------------------------------------------------------------------

#if (GAME_USE_LOOP)
bool			g_GameExit = FALSE;
#endif

#if (GAME_USE_STATE)
State			g_GameState = NULL;
State			g_GamePrevState = NULL;
#endif

#if (GAME_USE_VSYNC)
volatile bool	g_GameVSync = FALSE;
u8				g_GameFrame = 0;
callback		g_GameVSyncCB = Game_DefaultVSyncCB;
#endif

#if ((GAME_USE_VSYNC) && (GAME_USE_SYNC_50HZ))
bool			g_Game60Hz = 0;
u8				g_GameCount = 0;
#endif

//=============================================================================
//
//   G A M E
//
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize game module
void Game_Initialize(u8 screenMode)
{
	VDP_SetMode(screenMode);
	#if ((GAME_USE_VSYNC) && ((TARGET_TYPE != TYPE_ROM) || !(TARGET & ROM_ISR)) && (TARGET != TARGET_DOS0))
		VDP_EnableVBlank(TRUE);
		Bios_SetHookCallback(H_TIMI, Game_VSyncHook);
	#endif

	#if ((GAME_USE_VSYNC) && (GAME_USE_SYNC_50HZ))
	// Initialize frequency
	g_Game60Hz = (g_ROMVersion.VSF == 0);
	#endif
}

//-----------------------------------------------------------------------------
// Update game frame
void Game_Update()
{
	#if (GAME_USE_VSYNC)
		Game_WaitVSync();
	#endif
	#if (GAME_USE_STATE)
		Game_UpdateState();
	#endif	
}

//-----------------------------------------------------------------------------
// Release game module
void Game_Release()
{
	#if ((GAME_USE_VSYNC) && ((TARGET_TYPE != TYPE_ROM) || !(TARGET & ROM_ISR)) && (TARGET != TARGET_DOS0))
		Bios_ClearHook(H_TIMI);
	#endif
}


//=============================================================================
//
//   G A M E   L O O P
//
//=============================================================================
#if (GAME_USE_LOOP)

//-----------------------------------------------------------------------------
// Game main loop
void Game_MainLoop(u8 screenMode)
{
	Game_Initialize(screenMode);
	while(!g_GameExit)
		Game_Update();
	Game_Release();
}

//-----------------------------------------------------------------------------
// Game exit
void Game_Exit()
{
	g_GameExit = TRUE;
}

#endif // (GAME_USE_LOOP)


//=============================================================================
//
//   G A M E   S T A T E
//
//=============================================================================
#if (GAME_USE_STATE)

//-----------------------------------------------------------------------------
// Set the next state (change will be effective at the next state update)
// @param		newState	The new state to start (can be NULL to desactivate state-machine)
void Game_SetState(State newState)
{
	g_GamePrevState = g_GameState;
	g_GameState = newState;
}

//-----------------------------------------------------------------------------
// Restore the previous state
void Game_RestoreState()
{
	State prev = g_GamePrevState;
	g_GamePrevState = g_GameState;
	g_GameState = prev;
}

//-----------------------------------------------------------------------------
// Check state transition and update current state
void Game_UpdateState()
{
	bool bFrameFinish = FALSE;
	while((g_GameState != NULL) && !bFrameFinish)
	{
		bFrameFinish = g_GameState();
	}	
}

#endif // (GAME_USE_STATE)


//=============================================================================
//
//   G A M E   V - S Y N C H
//
//=============================================================================
#if (GAME_USE_VSYNC)

// Default V-Sync callback
void Game_DefaultVSyncCB() {}

//-----------------------------------------------------------------------------
// Vertical-synchronization hook handler
#if (((TARGET_TYPE == TYPE_ROM) && (TARGET & ROM_ISR)) || (TARGET == TARGET_DOS0))
void VDP_InterruptHandler()
#else
void Game_VSyncHook()
#endif
{
	g_GameFrame++;
	g_GameVSyncCB();
	g_GameVSync = TRUE;
}

//-----------------------------------------------------------------------------
// Wait for vertical-synchronization 
void Game_WaitVSync()
{
	while(g_GameVSync == FALSE) {}

	#if (GAME_USE_SYNC_50HZ)
	if(g_Game60Hz)
	{
		g_GameCount++;
		if(g_GameCount == 6)
		{
			g_GameCount = 0;
			while(g_GameVSync == FALSE) {} // Wait an additional frame
		}
	}
	#endif

	g_GameVSync = FALSE;
}

#endif // (GAME_USE_VSYNC)