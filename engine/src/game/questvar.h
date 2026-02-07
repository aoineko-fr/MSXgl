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

// Function: QuestVar_Enable
// Set a quest variable
//
// Parameters:
//   var - Quest variable to set
void QuestVar_Enable(u8 var);

// Function: QuestVar_Disable
// Reset a quest variable
//
// Parameters:
//   var - Quest variable to set
void QuestVar_Disable(u8 var);

// Function: QuestVar_Set
// Set or reset a quest variable
//
// Parameters:
//   var    - Quest variable to set
//   enable - TRUE to enable, FALSE to disable
inline void QuestVar_Set(u8 var, bool enable) { if (enable) QuestVar_Enable(var); else QuestVar_Disable(var); }

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
