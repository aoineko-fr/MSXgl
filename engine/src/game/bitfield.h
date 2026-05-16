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
// Initialize bit field (set all bits to 0)
void BitField_Initialize();

// Function: BitField_Enable
// Set a given bit
//
// Parameters:
//   var - Bit number to set
void BitField_Enable(u8 var);

// Function: BitField_Disable
// Reset a given bit
//
// Parameters:
//   var - Bit number to reset
void BitField_Disable(u8 var);

// Function: BitField_Set
// Set or reset a given bit
//
// Parameters:
//   var    - Bit number to set
//   enable - TRUE to enable, FALSE to disable
inline void BitField_Set(u8 var, bool enable) { if (enable) BitField_Enable(var); else BitField_Disable(var); }

// Function: BitField_Toggle
// Toggle a given bit
//
// Parameters:
//   var - Bit number to toggle
void BitField_Toggle(u8 var);

// Function: BitField_Get
// Get a given bit
//
// Parameters:
//   var - Bit number to get
//
// Return:
//   FALSE if bit is not set
bool BitField_Get(u8 var);

// Function: BitField_GetBitNumber
// Get the number of bits that fit in the bit field
inline u16 BitField_GetBitNumber() { return BITFIELD_MAX; }

// Function: BitField_GetSize
// Get the size of the bit field in bytes
inline u16 BitField_GetSize() { return BITFIELD_BYTES; }

// Function: BitField_GetData
// Get bit field raw data
//
// Return:
//   Raw data of the bit field
inline u8* BitField_GetData() { return g_BitField; }