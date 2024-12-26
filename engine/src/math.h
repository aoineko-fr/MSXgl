// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄      ▄▄  ▄▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▄▀██ ██▀ ██▄ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀▄██ ▀█▄ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘ 
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Quick math functions
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//-----------------------------------------------------------------------------
// Vector structure
//-----------------------------------------------------------------------------

// 8-bits unsigned vector structure
typedef struct
{
	u8			x;	
	u8			y;	
} VectorU8;

// 8-bits signed vector structure
typedef struct
{
	i8			x;	
	i8			y;	
} VectorI8;

// 16-bits unsigned vector structure
typedef struct
{
	u16			x;	
	u16			y;	
} VectorU16;

// 16-bits signed vector structure
typedef struct
{
	i16			x;	
	i16			y;	
} VectorI16;

#define VECTOR(type) Vector_##type
#define DEFVECTOR(type) typedef struct { type x; type y; } VECTOR(type)

//-----------------------------------------------------------------------------
// Group: Macros
// Helper macros
//-----------------------------------------------------------------------------

// Macro: Abs8
// Get absolute value of a signed 8-bits integer
#define ABS8(i)				(((u8)(i) & 0x80) ? ~((u8)(i) - 1) : (i))
// Macro: Abs16
// Get absolute value of a signed 16-bits integer
#define ABS16(i)			(((u16)(i) & 0x8000) ? ~((u16)(i) - 1) : (i))
// Macro: Abs32
// Get absolute value of a signed 32-bits integer
#define ABS32(i)			(((u32)(i) & 0x80000000) ? ~((u32)(i) - 1) : (i))

// Macro: Invert
// Invert sign of a signed integer
#define INVERT(a)			((^(a))++)

// Macro: Merge44
// Merge two 4 bits value into a 8 bits integer
#define MERGE44(a, b)		(u8)(((a) & 0x0F) << 4 | ((b) & 0x0F))
// Macro: Merge88
// Merge two 8 bits value into a 16 bits integer
#define MERGE88(a, b)		(u16)((u8)(a) << 8 | (u8)(b))
// Macro: ModuloPow2
// Get the power-of-2 modulo of a integer (ie. "MOD_POW2(100, 32)")
#define MOD_POW2(a, b)		((a) & ((b) - 1))

// Macro: Clamp8
// Clamp a 8-bits value into a interval 
#define CLAMP8(a, b, c)		((i8)(a) < (i8)(b)) ? (b) : ((i8)(a) > (i8)(c)) ? (c) : (a)
// Macro: Clamp16
// Clamp a 16-bits value into a interval 
#define CLAMP16(a, b, c)	((i16)(a) < (i16)(b)) ? (b) : ((i16)(a) > (i16)(c)) ? (c) : (a)

// Macro: Max
// Find highest value
#define MAX(a, b)			((a) > (b)) ? (a) : (b)
// Macro: Min
// Find lowest value
#define MIN(a, b)			((a) > (b)) ? (b) : (a)

//-----------------------------------------------------------------------------
// Group: Quick math
// Quick math routines
//-----------------------------------------------------------------------------

// Function: Math_Div10
// 8-bits fast 10 times division 
//
// Parameters:
//   val - Value to divide by 10
//
// Return:
//   val / 10
i8 Math_Div10(i8 val) __FASTCALL __PRESERVES(a, b, c, iyl, iyh);

// Function: Math_Div10_16b
// 16-bits fast 10 times division 
//
// Parameters:
//   val - Value to divide by 10
//
// Return:
//   val / 10
i16 Math_Div10_16b(i16 val) __FASTCALL __PRESERVES(b, d, e, iyl, iyh);

// Function: Math_Mod10
// 8-bits fast modulo-10 
//
// Parameters:
//   val - Value to module by 10
//
// Return:
//   val % 10
u8 Math_Mod10(u8 val) __PRESERVES(b, c, d, e, iyl, iyh);

// Function: Math_Mod10_16b
// 16-bits fast modulo-10 
//
// Parameters:
//   val - Value to module by 10
//
// Return:
//   val % 10
u8 Math_Mod10_16b(u16 val) __FASTCALL __PRESERVES(b, c, d, e, iyl, iyh);

// Function: Math_Flip
// Bits flip routine
//
// Parameters:
//   val - Value to flip
//
// Return:
//   Bits flipped value
u8 Math_Flip(u8 val) __PRESERVES(c, d, e, h, l, iyl, iyh);

// Function: Math_Flip_16b
// Bits flip routine
//
// Parameters:
//   val - Value to flip
//
// Return:
//   Bits flipped value
u16 Math_Flip_16b(u16 val) __PRESERVES(c, iyl, iyh);

// Function: Math_Negative
// Get the negative (additive inverse) of a 8-bit register
//
// Parameters:
//   val - Signed 8-bit value to negate
//
// Return:
//   Negative value of the input
inline i8 Math_Negative(i8 val) { return -val; }

// Function: Math_Negative16
// Get the negative (additive inverse) of a 16-bit register
//
// Parameters:
//   val - Signed 16-bit value to negate
//
// Return:
//   Negative value of the input
i16 Math_Negative16(i16 val) __FASTCALL __PRESERVES(b, c, d, e, iyl, iyh);

// Function: Math_Swap
// Swap MSB and LSB bytes
//
// Parameters:
//   val - 16-bits value to swap
//
// Return:
//   Swapped value
u16 Math_Swap(u16 val) __PRESERVES(a, b, c, iyl, iyh);

// Function: Math_SignedDiv2
// Divide a signed 8-bits integer by 2 using shift
//
// Parameters:
//   val - Value to divide by 2
//
// Return:
//   val / 2
i8 Math_SignedDiv2(i8 val) __NAKED __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: Math_SignedDiv4
// Divide a signed 8-bits integer by 4 using shift
//
// Parameters:
//   val - Value to divide by 4
//
// Return:
//   val / 4
i8 Math_SignedDiv4(i8 val) __NAKED __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: Math_SignedDiv8
// Divide a signed 8-bits integer by 8 using shift
//
// Parameters:
//   val - Value to divide by 8
//
// Return:
//   val / 8
i8 Math_SignedDiv8(i8 val) __NAKED __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: Math_SignedDiv16
// Divide a signed 8-bits integer by 16 using shift
//
// Parameters:
//   val - Value to divide by 16
//
// Return:
//   val / 16
i8 Math_SignedDiv16(i8 val) __NAKED __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: Math_SignedDiv32
// Divide a signed 8-bits integer by 32 using shift
//
// Parameters:
//   val - Value to divide by 32
//
// Return:
//   val / 32
i8 Math_SignedDiv32(i8 val) __NAKED __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: Math_Abs
// Get absolute value of a signed 8-bits integer
//
// Parameters:
//   val - Signed 8-bits value
//
// Return:
//   Absolute value of the input
inline u8 Math_Abs(i8 val) { return (((u8)(val) & 0x80) ? ~((u8)(val) - 1) : (val)); }

// Function: Math_Abs_16b
// Get absolute value of a signed 16-bits integer
//
// Parameters:
//   val - Signed 16-bits value
//
// Return:
//   Absolute value of the input
inline u16 Math_Abs_16b(i16 val) { return (((u16)(val) & 0x8000) ? ~((u16)(val) - 1) : (val)); }

// Function: Math_Abs_32b
// Get absolute value of a signed 32-bits integer
//
// Parameters:
//   val - Signed 32-bits value
//
// Return:
//   Absolute value of the input
inline u32 Math_Abs_32b(i32 val) { return (((u32)(val) & 0x80000000) ? ~((u32)(val) - 1) : (val)); }

//-----------------------------------------------------------------------------
// Group: Random
// Random routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 8-bits Random Number Generators

// No 8-bits random
#if (RANDOM_8_METHOD == RANDOM_8_NONE)
	#define RANDOM_8_NAME		"None"
	#define Math_SetRandomSeed8(seed)
	#define Math_GetRandom8() 0
// R Register value (7-bits)
#elif (RANDOM_8_METHOD == RANDOM_8_REGISTER)
	#define RANDOM_8_NAME		"R Register Value"
// R Register accumulation (7-bits)
#elif (RANDOM_8_METHOD == RANDOM_8_RACC)
	#define RANDOM_8_NAME		"R Register Acc"
// Ion Random
#elif (RANDOM_8_METHOD == RANDOM_8_ION)
	#define RANDOM_8_NAME		"Ion Random"
// Memory Peek from R
#elif (RANDOM_8_METHOD == RANDOM_8_MEMORY)
	#define RANDOM_8_NAME		"Memory Peek from R"
#endif

#if (RANDOM_8_METHOD != RANDOM_8_NONE)

// Function: Math_SetRandomSeed8
// Initialize random generator seed
//
// Parameters:
//   seed - Initial seed value for the generator
void Math_SetRandomSeed8(u8 seed);

// Function: Math_GetRandom8
// Generates 8-bit pseudorandom numbers
//
// Return:
//   A pseudorandom value between 0 and 255.
//   (Maximum value is 127 for RANDOM_8_REGISTER and RANDOM_8_RACC methods)
u8 Math_GetRandom8();

// Function: Math_GetRandomMax8
// Generates 8-bit pseudorandom numbers between 0 and max-1 value.
//
// Parameters:
//   max - Maximum value of the pseudorandom number (not included in the range).
//
// Return:
//   A pseudorandom value between 0 and max-1.
inline u8 Math_GetRandomMax8(u8 max) { return Math_GetRandom8() % max; }

// Function: Math_GetRandomRange8
// Generates 8-bit pseudorandom numbers between min and max-1 value.
//
// Parameters:
//   min - Minimum value of the pseudorandom number.
//   max - Maximum value of the pseudorandom number (not included in the range).
//
// Return:
//   A pseudorandom value between min and max-1.
inline u8 Math_GetRandomRange8(u8 min, u8 max) { return min + Math_GetRandom8() % (max - min); }
#endif


//-----------------------------------------------------------------------------
// 16-bits Random Number Generators

// No 8-bits random
#if (RANDOM_16_METHOD == RANDOM_16_NONE)
	#define RANDOM_16_NAME		"None"
	#define Math_SetRandomSeed16(seed)
	#define Math_GetRandom16() 0
// Linear congruential generator
#elif (RANDOM_16_METHOD == RANDOM_16_LINEAR)
	#define RANDOM_16_LINEAR_A	5			// 3		75		129
	#define RANDOM_16_LINEAR_C	1			// 0x8721	74		0x4321
	#define RANDOM_16_NAME		"Linear congruential"
// 16-bit xorshift pseudorandom number generator
#elif (RANDOM_16_METHOD == RANDOM_16_XORSHIFT)
	#define RANDOM_16_NAME 		"XOR Shift"
// Combination of a 16-bit Linear Feedback Shift Register (LFSR) and a 16-bit LCG
#elif (RANDOM_16_METHOD == RANDOM_16_LFSR_LCG)
	#define RANDOM_16_NAME 		"Combined LFSR/LCG"
#endif

#if (RANDOM_16_METHOD != RANDOM_16_NONE)

// Function: Math_SetRandomSeed16
// Initialize random generator seed
//
// Parameters:
//   seed - Initial seed value for the generator
void Math_SetRandomSeed16(u16 seed);

// Function: Math_GetRandom16
// Generates 16-bit pseudorandom numbers
//
// Return:
//   A pseudorandom value between 0 and 65535.
u16 Math_GetRandom16() __FASTCALL;

// Function: Math_GetRandomMax16
// Generates 16-bit pseudorandom numbers between 0 and max-1 value.
//
// Parameters:
//   max - Maximum value of the pseudorandom number (not included in the range).
//
// Return:
//   A pseudorandom value between 0 and max-1.
inline u16 Math_GetRandomMax16(u16 max) { return Math_GetRandom16() % max; }

// Function: Math_GetRandomRange16
// Generates 16-bit pseudorandom numbers between min and max-1 value.
//
// Parameters:
//   min - Minimum value of the pseudorandom number.
//   max - Maximum value of the pseudorandom number (not included in the range).
//
// Return:
//   A pseudorandom value between min and max-1.
inline u16 Math_GetRandomRange16(u16 min, u16 max) { return min + Math_GetRandom16() % (max - min); }

#endif
