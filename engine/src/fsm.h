// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄  ▄▄▄  ▄ ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄  ▀█▄  ██▀█
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ▄▄█▀ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Finite State Machine
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

// 
typedef struct
{
	u8       Flag;					// State behavior flag (reserved)
	callback Begin;					// Function to be called when state begin (can be NULL)
	callback Update;				// Function to be called at each update tick
	callback End;					// Function to be called when state end (can be NULL)
} FSM_State;

//
extern FSM_State* g_CurrentState;
extern FSM_State* g_PrevState;

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: FSM_SetState
// Set current state and handle transition
// Should not be called from Begin/End callbacks
//
// Parameters:
//   state - The new state to be set
void FSM_SetState(FSM_State* state);

// Function: FSM_GetState
// Get current state
//
// Return:
//   The current state
inline FSM_State* FSM_GetState() { return g_CurrentState; }

// Function: FSM_SetPrevious
// Reset previous state
// Should not be called from Begin/End callbacks
inline void FSM_SetPrevious() { FSM_SetState(g_PrevState); g_PrevState = NULL; }

// Function: FSM_GetPrevious
// Get previous state
//
// Return:
//   The previous state
inline FSM_State* FSM_GetPrevious() { return g_PrevState; }

// Function: FSM_Update
// Update the current state
inline void FSM_Update() { g_CurrentState->Update(); }
