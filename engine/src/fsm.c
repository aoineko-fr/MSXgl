// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄  ▄▄▄  ▄ ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄  ▀█▄  ██▀█
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ▄▄█▀ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Finite State Machine
//─────────────────────────────────────────────────────────────────────────────
#include "fsm.h"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

//=============================================================================
// MEMORY DATA
//=============================================================================

FSM_State* g_CurrentState = NULL;
FSM_State* g_PrevState = NULL;
FSM_State* g_NextState = NULL;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Set current state and handle transition
void FSM_SetState(FSM_State* state)
{
	g_NextState = state;
}

//-----------------------------------------------------------------------------
// Update the current state
void FSM_Update()
{
	if(g_NextState)
	{
		// End previous state
		if(g_CurrentState && g_CurrentState->End)
			g_CurrentState->End();

		// Switch state
		g_PrevState = g_CurrentState;
		g_CurrentState = g_NextState;
		g_NextState = NULL;

		// Start next state
		if(g_CurrentState->Begin);
			g_CurrentState->Begin();
	}

	g_CurrentState->Update();
}
