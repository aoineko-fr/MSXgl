//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄                                                        
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀                                                        
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █                                                        
//_____________________________________________________________________________
// Application helper functions
//_____________________________________________________________________________
#include "game.h"
#include "vdp.h"
#include "bios_hook.h"

//=============================================================================
//
//   G A M E
//
//=============================================================================

//-----------------------------------------------------------------------------
/// Initialize game module
void Game_Initialize(u8 screenMode)
{
	VDP_SetMode(screenMode);
	#if (USE_GAME_VSYNC)
		VDP_EnableVBlank(true);
		Bios_SetHookCallback(H_TIMI, Game_VSyncHook);
	#endif
}

//-----------------------------------------------------------------------------
/// Update game frame
void Game_Update()
{
	#if (USE_GAME_VSYNC)
		Game_WaitVSync();
	#endif
	#if (USE_GAME_STATE)
		Game_UpdateState();
	#endif	
}

//-----------------------------------------------------------------------------
/// Release game module
void Game_Release()
{
	#if (USE_GAME_VSYNC)
		Bios_ClearHook(H_TIMI);
	#endif
}


//=============================================================================
//
//   G A M E   L O O P
//
//=============================================================================
#if (USE_GAME_LOOP)

//-----------------------------------------------------------------------------
// RAM DATA

bool g_GameExit = false;

//-----------------------------------------------------------------------------
// FUNCTIONS

//-----------------------------------------------------------------------------
/// Game main loop
void Game_MainLoop(u8 screenMode)
{
	Game_Initialize(screenMode);
	while(!g_GameExit)
		Game_Update();
	Game_Release();
}

//-----------------------------------------------------------------------------
/// Game exit
void Game_Exit()
{
	g_GameExit = true;
}

#endif // (USE_GAME_LOOP)

//=============================================================================
//
//   G A M E   S T A T E
//
//=============================================================================
#if (USE_GAME_STATE)

//-----------------------------------------------------------------------------
// RAM DATA

State g_GameState = null;
State g_GamePrevState = null;

//-----------------------------------------------------------------------------
// FUNCTIONS

//-----------------------------------------------------------------------------
/// Set the next state (change will be effective at the next state update)
/// @param		newState	The new state to start (can be NULL to desactivate state-machine)
void Game_SetState(State newState) __FASTCALL
{
	g_GamePrevState = g_GameState;
	g_GameState = newState;
}

//-----------------------------------------------------------------------------
/// Restore the previous state
void Game_RestoreState()
{
	State prev = g_GamePrevState;
	g_GamePrevState = g_GameState;
	g_GameState = prev;
}

//-----------------------------------------------------------------------------
/// Check state transition and update current state
void Game_UpdateState()
{
	bool bFrameFinish = false;
	while((g_GameState != null) && !bFrameFinish)
	{
		bFrameFinish = g_GameState();
	}	
}

#endif // (USE_GAME_STATE)


//=============================================================================
//
//   G A M E   V - S Y N C H
//
//=============================================================================
#if (USE_GAME_VSYNC)

void Game_DefaultVSyncCB();

//-----------------------------------------------------------------------------
// RAM DATA

bool     g_GameVSync = false;
u8       g_GameFrame = 0;
callback g_GameVSyncCB = Game_DefaultVSyncCB;

//-----------------------------------------------------------------------------
// FUNCTIONS

//-----------------------------------------------------------------------------
/// Default V-Sync callback
void Game_DefaultVSyncCB() {}

//-----------------------------------------------------------------------------
/// Set V-Sync callback
void Game_SetVSyncCallback(callback cb) __FASTCALL
{
	g_GameVSyncCB = cb;
}

//-----------------------------------------------------------------------------
/// Vertical-synchronization hook handler
void Game_VSyncHook()
{
	g_GameVSync = true;
	g_GameVSyncCB();
}

//-----------------------------------------------------------------------------
/// Wait for vertical-synchronization 
void Game_WaitVSync()
{
	while(g_GameVSync == false) {}
	g_GameVSync = false;
	g_GameFrame++;
}

#endif // (USE_GAME_VSYNC)