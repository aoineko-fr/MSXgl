// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄   ▄▄▄ ▄▄▄▄  ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██▄█ ▀█▄   ██  ██ ▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ██ █ ▄▄█▀ ▄██▄ ▀█▄▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// BASIC USR routines handler 
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================
#include "core.h"
#include "bios_var.h"

//=============================================================================
// DEFINES
//=============================================================================

enum BASIC_TYPE
{
	BASIC_TYPE_INT    = 2, // 2-byte integer type
	BASIC_TYPE_STRING = 3, // String type
	BASIC_TYPE_FLOAT  = 4, // Single precision real type
	BASIC_TYPE_DOUBLE = 8, // Double precision real type
};

#define BASIC_GET_TYPE()			(*(u8*)(M_VALTYP))

#define BASIC_GET_INT(type)			(*(type*)(M_DAC+2))

#define BASIC_GET_BYTE()			(*(i8*)(M_DAC+2))
#define BASIC_GET_WORD()			(*(i16*)(M_DAC+2))
#define BASIC_GET_FLOAT()			(*(f32*)(M_DAC))
#define BASIC_GET_STRING()			(*(const c8**)(M_DAC+1))
#define BASIC_GET_STRING_SIZE()		(*(u8*)(M_DAC))

#define BASIC_SET_BYTE(val)			(*(i16*)(M_DAC+2) = (i16)(val))
#define BASIC_SET_WORD(val)			(*(i16*)(M_DAC+2) = (i16)(val))
#define BASIC_SET_FLOAT(val)		(*(f32*)(M_DAC) = (f32)(val))
#define BASIC_SET_STRING(val, len)	(*(u8*)(M_DAC) = (u8)(len)); (*(const c8**)(M_DAC+1) = (const c8*)(val))

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Getter
// Get variable from Basic
//-----------------------------------------------------------------------------

// Function: Basic_GetType
// Get Basic variable type
// This is the value sent by Basic's USR() function.
inline u8 Basic_GetType() { return BASIC_GET_TYPE(); }

// Function: Basic_GetByte
// Get Basic variable as 8-bits interger
// This is the value sent by Basic's USR() function.
inline i8 Basic_GetByte() { return BASIC_GET_BYTE(); }

// Function: Basic_GetWord
// Get Basic variable as 16-bits interger
// This is the value sent by Basic's USR() function.
inline i16 Basic_GetWord() { return BASIC_GET_WORD(); }

// Function: Basic_GetFloat
// Get Basic variable as 32-bits float number
// This is the value sent by Basic's USR() function.
inline f32 Basic_GetFloat() { return BASIC_GET_FLOAT(); }

// Function: Basic_GetString
// Get Basic variable as string pointer
// This is the value sent by Basic's USR() function.
inline const c8* Basic_GetString() { return BASIC_GET_STRING(); }

// Function: Basic_GetStringLength
// Get Basic string variable length
// This is the value sent by Basic's USR() function.
inline u8 Basic_GetStringLength() { return BASIC_GET_STRING_SIZE(); }

//-----------------------------------------------------------------------------
// Group: Setter
// Set variable for Basic
//-----------------------------------------------------------------------------

// Function: Basic_SetByte
// Set Basic variable as 8-bits interger
// This is the value returned by Basic's USR() function.
//
// Parameters:
//   val - Value to be send to Basic
inline void Basic_SetByte(i8 val) { BASIC_SET_BYTE(val); }

// Function: Basic_SetWord
// Set Basic variable as 16-bits interger
// This is the value returned by Basic's USR() function.
//
// Parameters:
//   val - Value to be send to Basic
inline void Basic_SetWord(i16 val) { BASIC_SET_WORD(val); }

// Function: Basic_SetFloat
// Set Basic variable as 32-bits float number
// This is the value returned by Basic's USR() function.
//
// Parameters:
//   val - Value to be send to Basic
inline void Basic_SetFloat(f32 val) { BASIC_SET_FLOAT(val); }

// Function: Basic_SetString
// Set Basic variable as string pointer
// This is the value returned by Basic's USR() function.
//
// Parameters:
//   val - Value to be send to Basic
inline void Basic_SetString(const c8* val, u8 len) { BASIC_SET_STRING(val, len); }