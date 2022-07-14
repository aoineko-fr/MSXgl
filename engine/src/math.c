// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄      ▄▄  ▄▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▄▀██ ██▀ ██▄ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀▄██ ▀█▄ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘ 
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Quick math functions
//
// References:
// - http://map.grauw.nl/sources/external/z80bits.html#4
// - http://z80-heaven.wikidot.com/math
// - http://z80-heaven.wikidot.com/advanced-math
// - https://wikiti.brandonw.net/index.php?title=Category:Z80_Routines:Math
// - https://www.chilliant.com/z80shift.html
//─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "math.h"

//-----------------------------------------------------------------------------
// DIVISION
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// 8-bits fast 10 times division 
i8 Math_Div10(i8 val) __FASTCALL
{
	val;
	__asm
//		ld		l, val		// fast call
		ld		d, #0
		ld		h, d
		add		hl, hl
		add		hl, de
		add		hl, hl
		add		hl, hl
		add		hl, de
		add		hl, hl
		ld		l, h
	__endasm;
}

//-----------------------------------------------------------------------------
/// 16-bits fast 10 times division 
i16 Math_Div10_16b(i16 val) __FASTCALL
{
	val;
	__asm
//		ld		hl, val		// fast call
		ld		bc, #0x0D0A
		xor		a
		add		hl, hl
		rla
		add		hl, hl
		rla
		add		hl, hl
		rla
		add		hl, hl
		rla
		cp		c
	label_0:
		jr		c, label_0 + #4
		sub		c
		inc		l
	label_1:
		djnz	label_1 - #7
	__endasm;
}

//-----------------------------------------------------------------------------
// MODULO
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// 18-bits fast modulo-10 
// Inputs:		A	8-bit unsigned integer
// Outputs:		A	HL mod 10
//				Z	Flag is set if divisible by 10
// 20 bytes, 83 cycles
u8 Math_Mod10(u8 val)
{
	val; // A
	__asm
		ld		h, a		// add nibbles
		rrca
		rrca
		rrca
		rrca
		add		a, h
		adc		a, #0		// n mod 15 (+1) in both nibbles
		daa
		ld		l, a
		sub		h			// test if quotient is even or odd
		rra
		sbc		a, a
		and		#5
		add		a, l
		daa
		and		#0x0F
	__endasm;
}

//-----------------------------------------------------------------------------
/// 16-bits fast modulo-10 
// Inputs:		HL	16-bit unsigned integer
// Outputs:		A	HL mod 10
//				Z	Flag is set if divisible by 10
// 24 bytes,  98cc
u8 Math_Mod10_16b(u16 val) __FASTCALL
{
	val;
	__asm
//		ld		hl, val		// fast call
		ld		a, h		// add bytes
		add		a, l
		adc		a, #0		// n mod 255 (+1)
		ld		h, a		// add nibbles
		rrca
		rrca
		rrca
		rrca
		add		a, h
		adc		a, #0		// n mod 15 (+1) in both nibbles
		daa
		ld		h, a
		sub		l			// test if quotient is even or odd
		rra
		sbc		a, a
		and		#5
		add		a, h
		daa
		and		#0x0F
		ld		l, a
	__endasm;
}

//-----------------------------------------------------------------------------
// BITS OPERATION
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// 8-bits fast bits flip 
// Inputs:		A		8-bits value
// Outputs:		A		Bits flipped value
// Author:		John Metcalf (http://www.retroprogramming.com/2014/01/fast-z80-bit-reversal.html)
u8 Math_Flip(u8 val)
{
	val; // A
	__asm
		// reverse bits in A
		// 17 bytes / 66 cycles
		ld		b, a		// a = 76543210
		rlca
		rlca				// a = 54321076
		xor		b
		and		#0xAA
		xor		b			// a = 56341270
		ld		b, a
		rlca
		rlca
		rlca				// a = 41270563
		rrc 	b			// b = 05634127
		xor 	b
		and 	#0x66
		xor 	b			// a = 01234567
	__endasm;
}

//-----------------------------------------------------------------------------
/// 16-bits bits flip 
u16 Math_Flip_16b(u16 val) __FASTCALL
{
	// register u8 a = val >> 8;
	// register u8 b = val & 0x00FF;
	// a = Math_Flip(a);
	// b = Math_Flip(b);
	// return (b << 8) + a;
	
	val; // HL
	__asm
		ld		a, h
		call	_Math_Flip	// flip high bits
		ld		b, a
		ld		a, l
		call	_Math_Flip	// flip low bits
		ld		h, a		// flip high/low
		ld		l, b
	__endasm;
}

//=============================================================================
// RANDOM
//=============================================================================


//-----------------------------------------------------------------------------
// 7-bits R register value
//-----------------------------------------------------------------------------
#if (RANDOM_8_METHOD == RANDOM_8_REGISTER)

//-----------------------------------------------------------------------------
/// Initialize random generator seed
void Math_SetRandomSeed8(u8 seed) {}

//-----------------------------------------------------------------------------
/// Generates 8-bit pseudorandom numbers
u8 Math_GetRandom8()
{
	__asm
		ld		a, r
	__endasm;	
}


//-----------------------------------------------------------------------------
// 7-bits R register value
//-----------------------------------------------------------------------------
#elif (RANDOM_8_METHOD == RANDOM_8_RACC)

u8 g_RandomSeed8 = 0;

//-----------------------------------------------------------------------------
/// Initialize random generator seed
void Math_SetRandomSeed8(u8 seed) { g_RandomSeed8 = seed; }

//-----------------------------------------------------------------------------
/// Generates 8-bit pseudorandom numbers
u8 Math_GetRandom8()
{
	__asm
	#if(1)
		ld      a, (_g_RandomSeed8)
		ld		b, a
		ld		a, r
		xor		b
		ld      (_g_RandomSeed8), a
	#else // Alternative version from ARTRAG (https://www.msx.org/forum/development/msx-development/example-random-number-generator?page=1)
		ld		a, r
		ld		b, a
		ld		a, (_g_RandomSeed8)
		xor		b
		ld		(_g_RandomSeed8), a
	#endif
	__endasm;	
}


//-----------------------------------------------------------------------------
// 8-bits Ion Random
//-----------------------------------------------------------------------------
#elif (RANDOM_8_METHOD == RANDOM_8_ION)

u16 g_RandomSeed8 = 1;

//-----------------------------------------------------------------------------
/// Initialize random generator seed
void Math_SetRandomSeed8(u8 seed) { g_RandomSeed8 = (u16)seed; }

//-----------------------------------------------------------------------------
/// Generates 8-bit pseudorandom numbers
// Ion Random by Joe Wingbermuehle (https://wikiti.brandonw.net/index.php?title=Z80_Routines:Math:Random)
u8 Math_GetRandom8()
{
	__asm
	#if(0)
		ld      hl, (_g_RandomSeed8)
		ld      a, r
		ld      d, a
		ld      e, (hl)
		add     hl, de
		add     a, l
		xor     h
		ld      (_g_RandomSeed8), hl
	#else // Alternative version  (better distribution but a little bit slower)
		ld      hl, (_g_RandomSeed8)
		ld      a, r
		ld      d, a
		ld      e, a
		add     hl, de
		xor     l
		add     a
		xor     h
		ld      l, a
		ld      (_g_RandomSeed8), hl
	#endif

	__endasm;
}


//-----------------------------------------------------------------------------
// 8-bits Memory Peek
//-----------------------------------------------------------------------------
#elif (RANDOM_8_METHOD == RANDOM_8_MEMORY)

u8 g_RandomSeed8 = 0;

//-----------------------------------------------------------------------------
/// Initialize random generator seed
void Math_SetRandomSeed8(u8 seed) { g_RandomSeed8 = seed; }

//-----------------------------------------------------------------------------
/// Generates 8-bit pseudorandom numbers
// Peek value from address
u8 Math_GetRandom8()
{
	__asm
		ld      a, (_g_RandomSeed8)
		ld		l, a				// Put seed in L
		ld		a, r
		ld		h, a				// Put R register in H (R is 7-bits counter so upper address is 0x7Fxx
		ld      (_g_RandomSeed8), a
		ld		a, (hl)				// Get byte at HL random address

	__endasm;
}

#endif


//-----------------------------------------------------------------------------
// 16-bits Linear congruential generator
//-----------------------------------------------------------------------------
#if (RANDOM_16_METHOD == RANDOM_16_LINEAR)

u16 g_RandomSeed16 = 0;

//-----------------------------------------------------------------------------
/// Initialize random generator seed
void Math_SetRandomSeed16(u16 seed) { g_RandomSeed16 = seed; }

//-----------------------------------------------------------------------------
/// Generates 16-bit pseudorandom numbers with a period of 65535
// Linear congruential generator
u16 Math_GetRandom16() __FASTCALL
{
	u16 ret = (g_RandomSeed16 * RANDOM_16_LINEAR_A) + RANDOM_16_LINEAR_C;
	g_RandomSeed16 = ret;
	return ret;
}


//-----------------------------------------------------------------------------
// 16-bit xorshift pseudorandom number generator
//-----------------------------------------------------------------------------
#elif (RANDOM_16_METHOD == RANDOM_16_XORSHIFT)

u16 g_RandomSeed16 = 1;

//-----------------------------------------------------------------------------
/// Initialize random generator seed
void Math_SetRandomSeed16(u16 seed) { g_RandomSeed16 = seed | 0x0001; }

//-----------------------------------------------------------------------------
/// Generates 16-bit pseudorandom numbers
// 16-bit xorshift pseudorandom number generator by John Metcalf (https://wikiti.brandonw.net/index.php?title=Z80_Routines:Math:Random)
// Outputs:		HL	Pseudorandom number
// Using the xor-shift method:
//	hl ^= hl << 7
//	hl ^= hl >> 9
//	hl ^= hl << 8
// Some alternative shift triplets which also perform well are:
//	{6, 7, 13}; {7, 9, 13}; {9, 7, 13}.
// 20 bytes, 86 cycles
u16 Math_GetRandom16() __FASTCALL
{
	__asm
	xrnd:
		ld		hl, (_g_RandomSeed16)
		ld		a, h
		rra
		ld		a, l
		rra
		xor		h
		ld		h, a
		ld		a, l
		rra
		ld		a, h
		rra
		xor		l
		ld		l, a
		xor		h
		ld		h, a
		ld		(_g_RandomSeed16), hl
	__endasm;
	
	/*g_RandomSeed16 ^= g_RandomSeed16 << 7;
	g_RandomSeed16 ^= g_RandomSeed16 >> 9;
	g_RandomSeed16 ^= g_RandomSeed16 << 8;
	return g_RandomSeed16;*/
}


//-----------------------------------------------------------------------------
// 16-bits Combined LFSR/LCG
//-----------------------------------------------------------------------------
#elif (RANDOM_16_METHOD == RANDOM_16_LFSR_LCG)

u16 g_RandomSeed16_1 = 1;
u16 g_RandomSeed16_2 = 2;

//-----------------------------------------------------------------------------
/// Initialize random generator seed
void Math_SetRandomSeed16(u16 seed)
{
	g_RandomSeed16_1 = seed;
	g_RandomSeed16_2 = seed | 0x0001;
}

//-----------------------------------------------------------------------------
/// Generates 16-bit pseudorandom numbers with a period of 65535
// Combined LFSR/LCG (16-bit seeds) (https://wikiti.brandonw.net/index.php?title=Z80_Routines:Math:Random)
// Inputs:		(seed1) contains a 16-bit seed value
//				(seed2) contains a NON-ZERO 16-bit seed value
// Outputs:		HL is the result
//				BC is the result of the LCG, so not that great of quality
//				DE is preserved
// cycle: 4,294,901,760 (almost 4.3 billion)
// 26 bytes, 160 cycles
u16 Math_GetRandom16() __FASTCALL
{
	__asm
		ld		hl, (_g_RandomSeed16_1)
		ld		b, h
		ld		c, l
		add		hl, hl
		add		hl, hl
		inc		l
		add		hl, bc
		ld		(_g_RandomSeed16_1), hl
		ld		hl, (_g_RandomSeed16_2)
		add		hl, hl
		sbc		a, a
		and		#0b00101101
		xor		l
		ld		l, a
		ld		(_g_RandomSeed16_2), hl
		add		hl, bc
	__endasm;
}

#endif