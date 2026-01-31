// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄                 ▄▄    ▄▄ ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██ █ ▄███  ██▀ ██▀   ██ █ ▄▀██ ██▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄█ ▀█▄█ ▀█▄▄ ▄██  ▀█▄   ▀█▀  ▀▄██ ██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  Quest variables module
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

// QUESTVAR_MAX
#ifndef QUESTVAR_MAX
	#warning QUESTVAR_MAX is not defined in "msxgl_config.h"! Default value will be used: 64
	#define QUESTVAR_MAX	64
#endif

#define QUESTVAR_BYTES				((QUESTVAR_MAX + 7) / 8)

//=============================================================================
// VARIABLES
//=============================================================================

extern u8 g_QuestVar[QUESTVAR_BYTES];

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: QuestVar_Initialize
// Initialize quest variables
void QuestVar_Initialize();

// Function: QuestVar_Set
// Set a quest variable
//
// Parameters:
//   var - Quest variable to set
void QuestVar_Set(u8 var);

// Function: QuestVar_Reset
// Reset a quest variable
//
// Parameters:
//   var - Quest variable to set
void QuestVar_Reset(u8 var);

// Function: QuestVar_Toggle
// Toggle a quest variable
//
// Parameters:
//   var - Quest variable to set
void QuestVar_Toggle(u8 var);

// Function: QuestVar_Get
// Get a quest variable
//
// Parameters:
//   var - Quest variable to set
bool QuestVar_Get(u8 var);
