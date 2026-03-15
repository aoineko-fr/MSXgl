// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄  ▄▄  ▄▄▄▄ ▄       ▄▄    ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ▄  ██▀ ██▄  ▄  ▄███ ██   ▄██  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ██ ▀█▄ ██   ██ ▀█▄▄ ▀█▄ ▀▄██  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  Bit field module
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================
#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

// BITFIELD_MAX
#ifndef BITFIELD_MAX
	#warning BITFIELD_MAX is not defined in "msxgl_config.h"! Default value will be used: 64
	#define BITFIELD_MAX	64
#endif

#define BITFIELD_BYTES				((BITFIELD_MAX + 7) / 8)

//=============================================================================
// VARIABLES
//=============================================================================

extern u8 g_BitField[BITFIELD_BYTES];

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: BitField_Initialize
// Initialize quest variables
void BitField_Initialize();

// Function: BitField_Enable
// Set a quest variable
//
// Parameters:
//   var - Quest variable to set
void BitField_Enable(u8 var);

// Function: BitField_Disable
// Reset a quest variable
//
// Parameters:
//   var - Quest variable to set
void BitField_Disable(u8 var);

// Function: BitField_Set
// Set or reset a quest variable
//
// Parameters:
//   var    - Quest variable to set
//   enable - TRUE to enable, FALSE to disable
inline void BitField_Set(u8 var, bool enable) { if (enable) BitField_Enable(var); else BitField_Disable(var); }

// Function: BitField_Toggle
// Toggle a quest variable
//
// Parameters:
//   var - Quest variable to set
void BitField_Toggle(u8 var);

// Function: BitField_Get
// Get a quest variable
//
// Parameters:
//   var - Quest variable to set
//
// Return:
//   FALSE if variable is not set
bool BitField_Get(u8 var);

// Function: BitField_GetData
// Get bit field raw data
//
// Return:
//   Raw data of the bitfield
inline u8* BitField_GetData() { return g_BitField; }
