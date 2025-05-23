// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄           ▄  ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ██▀▄ ██▄▀ ▄  ██▀ ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ██▀  ██   ██ ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘       ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Sprite tool module
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "sprite_fx.h"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

#if (SPRITEFX_USE_CROP)

// Mask for left croping
const u8 g_SpriteMaskL[8] = 
{
	0b01111111,
	0b00111111,
	0b00011111,
	0b00001111,
	0b00000111,
	0b00000011,
	0b00000001,
	0b00000000,
};

// Mask for right croping
const u8 g_SpriteMaskR[8] = 
{
	0b11111110,
	0b11111100,
	0b11111000,
	0b11110000,
	0b11100000,
	0b11000000,
	0b10000000,
	0b00000000,
};

#endif // (SPRITEFX_USE_CROP)

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Cropping
//-----------------------------------------------------------------------------
#if (SPRITEFX_USE_CROP)

#if (SPRITEFX_USE_8x8)
//-----------------------------------------------------------------------------
// Crop 8x8 sprite left side
void SpriteFX_CropLeft8(const u8* src, u8* dest, u8 offset) __NAKED // Stack: 3 bytes
{
	src;	// HL
	dest;	// DE
	offset;	// SP[2]

__asm
	ld		iy, #2				// Get 'offset' from the stack
	add		iy, sp
	ld		c, 0(iy)			// C = offset

// u8 mask = g_SpriteMaskL[offset];
	ld		b, #0
	ld		iy, #_g_SpriteMaskL
	add		iy, bc
	ld		c, 0(iy)			// C = g_SpriteMaskL[offset]

// loop(i, 8)
// 	*dest++ = *src++ & mask;
	ld		b, #8
loopCropL8:
	ld		a, (hl)
	inc		hl
	and		c
	ld		(de), a
	inc		de 
	djnz	loopCropL8

	pop		hl
	inc		sp						// Adjusting stack address
	jp		(hl)					// Return to caller
__endasm;
}
#endif // (SPRITEFX_USE_8x8)

#if (SPRITEFX_USE_16x16)
//-----------------------------------------------------------------------------
// Crop 16x16 sprite left side
void SpriteFX_CropLeft16(const u8* src, u8* dest, u8 offset) __NAKED // Stack: 3 bytes
{
	src;	// HL
	dest;	// DE
	offset;	// SP[2]

__asm
	ld		iy, #2				// Get 'offset' from the stack
	add		iy, sp
	ld		a, 0(iy)			// A = offset

// if(offset < 8) {
	cp		#8
	jp		nc, greaterCropL16	

// 	u8 mask = g_SpriteMaskL[offset];
	ld		b, #0
	ld		c, a				// C = offset
	ld		iy, #_g_SpriteMaskL
	add		iy, bc
	ld		c, 0(iy)			// C = g_SpriteMaskL[offset]

// 	loop(i, 16)
// 		*dest++ = *src++ & mask;
	ld		b, #16
loopCropL16_1:
	ld		a, (hl)
	inc		hl
	and		c
	ld		(de), a
	inc		de 
	djnz	loopCropL16_1

// 	loop(i, 16)
// 		*dest++ = *src++;
	ld		bc, #16
	ldir

	jp		endCropL16

// } else {
greaterCropL16:

// 	loop(i, 16)
// 	{
// 		*dest++ = 0;
// 		src++;
// 	}
	sub		#8
	ld		c, a				// C = offset - 8 {computed this while A value is available}
	xor		a
	ld		b, #16
loopCropL16_2:
	inc		hl
	ld		(de), a
	inc		de 
	djnz	loopCropL16_2

// 	u8 mask = g_SpriteMaskL[offset - 8];
	ld		iy, #_g_SpriteMaskL
	add		iy, bc				// B = 0 {from djnz}  |  C = offset - 8 {computed previously}
	ld		c, 0(iy)			// C = g_SpriteMaskL[offset - 8]

// 	loop(i, 16)
// 		*dest++ = *src++ & mask;
	ld		b, #16
loopCropL16_3:
	ld		a, (hl)
	inc		hl
	and		c
	ld		(de), a
	inc		de 
	djnz	loopCropL16_3

// }
endCropL16:
	pop		hl
	inc		sp						// Adjusting stack address
	jp		(hl)					// Return to caller
__endasm;
}
#endif // (SPRITEFX_USE_16x16)

#if (SPRITEFX_USE_8x8)
//-----------------------------------------------------------------------------
// Crop 8x8 sprite right side
void SpriteFX_CropRight8(const u8* src, u8* dest, u8 offset) __NAKED // Stack: 3 bytes
{
	src;	// HL
	dest;	// DE
	offset;	// SP[2]

__asm
	ld		iy, #2				// Get 'offset' from the stack
	add		iy, sp
	ld		c, 0(iy)			// C = offset

// u8 mask = g_SpriteMaskR[offset];
	ld		b, #0
	ld		iy, #_g_SpriteMaskR
	add		iy, bc
	ld		c, 0(iy)			// C = g_SpriteMaskL[offset]

// loop(i, 8)
// 	*dest++ = *src++ & mask;
	ld		b, #8
loopCropR8:
	ld		a, (hl)
	inc		hl
	and		c
	ld		(de), a
	inc		de 
	djnz	loopCropR8

	pop		hl
	inc		sp						// Adjusting stack address
	jp		(hl)					// Return to caller
__endasm;
}
#endif // (SPRITEFX_USE_8x8)

#if (SPRITEFX_USE_16x16)
//-----------------------------------------------------------------------------
// Crop 16x16 sprite right side
void SpriteFX_CropRight16(const u8* src, u8* dest, u8 offset) __NAKED // Stack: 3 bytes
{
	src;	// HL
	dest;	// DE
	offset;	// SP[2]

__asm
	ld		iy, #2				// Get 'offset' from the stack
	add		iy, sp
	ld		a, 0(iy)			// A = offset

// if(offset < 8) {
	cp		#8
	jp		nc, greaterCropR16	

// 	loop(i, 16)
// 		*dest++ = *src++;
	ld		bc, #16
	ldir

// 	u8 mask = g_SpriteMaskR[offset];
	ld		c, a				// C = offset
	ld		iy, #_g_SpriteMaskR
	add		iy, bc				// B = 0 {from ldir}  |  C = offset
	ld		c, 0(iy)			// C = g_SpriteMaskR[offset]

// 	loop(i, 16)
// 		*dest++ = *src++ & mask;
	ld		b, #16
loopCropR16_1:
	ld		a, (hl)
	inc		hl
	and		c
	ld		(de), a
	inc		de 
	djnz	loopCropR16_1

	jp		endCropR16

// } else {
greaterCropR16:

// 	u8 mask = g_SpriteMaskR[offset - 8];
	sub		#8
	ld		c, a				// C = offset - 8
	ld		b, #0
	ld		iy, #_g_SpriteMaskR
	add		iy, bc
	ld		c, 0(iy)			// C = g_SpriteMaskR[offset - 8]

// 	loop(i, 16)
// 		*dest++ = *src++ & mask;
	ld		b, #16
loopCropR16_2:
	ld		a, (hl)
	inc		hl
	and		c
	ld		(de), a
	inc		de 
	djnz	loopCropR16_2

// 	loop(i, 16)
// 		*dest++ = 0;
	xor		a
	ld		b, #16
loopCropR16_3:
	ld		(de), a
	inc		de 
	djnz	loopCropR16_3

// }
endCropR16:
	pop		hl
	inc		sp						// Adjusting stack address
	jp		(hl)					// Return to caller
__endasm;
}
#endif // (SPRITEFX_USE_8x8)

#if (SPRITEFX_USE_8x8)
//-----------------------------------------------------------------------------
// Crop 8x8 sprite top border.
void SpriteFX_CropTop8(const u8* src, u8* dest, u8 offset) __NAKED // Stack: 3 bytes
{
	src;	// HL
	dest;	// DE
	offset;	// SP[2]

__asm
// u8 n = ++offset;
	ld		iy, #2				// Get 'offset' from the stack
	add		iy, sp
	ld		c, 0(iy)
	inc		c					// C = offset + 1

// loop(i, n)
// {
// 	*dest++ = 0;
// 	src++;
// }
	xor		a
	ld		b, c				// B = offset + 1
loopCropT8:
	ld		(de), a
	inc		hl
	inc		de
	djnz	loopCropT8

// for(u8 i = n; i < 8; ++i)
// 	*dest++ = *src++;
	ld		a, c				// A = C = offset + 1
	neg
	add		a, #8				// A = 8 - C = 7 - offset
	jp		z, skipCropT8
	ld		c, a
	// B = 0 {from djnz}
	ldir
skipCropT8:
	pop		hl
	inc		sp						// Adjusting stack address
	jp		(hl)					// Return to caller
__endasm;
}
#endif // (SPRITEFX_USE_8x8)

#if (SPRITEFX_USE_16x16)
//-----------------------------------------------------------------------------
// Crop 16x16 sprite top border.
void SpriteFX_CropTop16(const u8* src, u8* dest, u8 offset) __NAKED  // Stack: 3 bytes // can be optimized?
{
	src;	// HL
	dest;	// DE
	offset;	// SP[2]

__asm
// u8 n = ++offset;
	ld		iy, #2				// Get 'offset' from the stack
	add		iy, sp
	ld		a, 0(iy)
	inc		a					// A = offset + 1
	ld______iyl_a				// IYL = A {backup}

// loop(i, n)
// {
// 	*dest++ = 0;
// 	src++;
// }
	ld		b, a				// B = offset + 1
	xor		a
loopCropT16_1:
	inc		hl
	ld		(de), a
	inc		de
	djnz	loopCropT16_1

// for(u8 i = n; i < 16; ++i)
// 	*dest++ = *src++;
	ld______a_iyl
	neg
	add		a, #16				// A = 16 - IYL = 15 - offset
	jp		z, skipCropT16_1
	ld		c, a
	// B = 0 {from djnz}
	ldir
skipCropT16_1:

// loop(i, n)
// {
// 	*dest++ = 0;
// 	src += n;
// }
	ld______a_iyl				// A = offset + 1
	ld		b, a				// B = offset + 1
	xor		a
loopCropT16_2:
	inc		hl
	ld		(de), a
	inc		de
	djnz	loopCropT16_2

// for(u8 i = n; i < 16; ++i)
// 	*dest++ = *src++;
	ld______a_iyl
	neg
	add		a, #16				// A = 16 - IYL = 15 - offset
	jp		z, skipCropT16_2
	ld		c, a
	// B = 0 {from djnz}
	ldir
skipCropT16_2:
	pop		hl
	inc		sp						// Adjusting stack address
	jp		(hl)					// Return to caller
__endasm;
}
#endif // (SPRITEFX_USE_16x16)

#if (SPRITEFX_USE_8x8)
//-----------------------------------------------------------------------------
// Crop 8x8 sprite bottom border.
void SpriteFX_CropBottom8(const u8* src, u8* dest, u8 offset) __NAKED // Stack: 3 bytes
{
	src;	// HL
	dest;	// DE
	offset;	// SP[2]

__asm
// u8 n = 7 - offset;
	ld		iy, #2				// Get 'offset' from the stack
	add		iy, sp
	ld		a, 0(iy)
	neg
	add		#7					// A = 7 - offset
	jp		z, skipCropB8

// loop(i, n)
// 	*dest++ = *src++;
	ld		c, a
	ld		b, #0
	ldir

// for(u8 i = n; i < 8; ++i)
// 	*dest++ = 0;
	neg
skipCropB8:
	add		a, #8
	ld		b, a				// B = 8 - A = offset + 1
	xor		a
loopCropB8:
	ld		(de), a
	inc		de
	djnz	loopCropB8

	pop		hl
	inc		sp						// Adjusting stack address
	jp		(hl)					// Return to caller
__endasm;
}
#endif // (SPRITEFX_USE_8x8)

#if (SPRITEFX_USE_16x16)
//-----------------------------------------------------------------------------
// Crop 16x16 sprite bottom border.
void SpriteFX_CropBottom16(const u8* src, u8* dest, u8 offset) __NAKED // Stack: 3 bytes
{
	src;	// HL
	dest;	// DE
	offset;	// SP[2]

__asm
// u8 n = 15 - offset;
	ld		iy, #2				// Get 'offset' from the stack
	add		iy, sp
	ld		a, 0(iy)
	neg
	add		#15					// A = 15 - offset
	ld______iyl_a				// IYL = A (backup)
	jp		z, skipCropB16_1

// loop(i, n)
// 	*dest++ = *src++;
	ld		c, a
	ld		b, #0
	ldir

// for(u8 i = n; i < 16; ++i)
// {
// 	*dest++ = 0;
// 	src++;
// }
	neg
skipCropB16_1:
	add		a, #16
	ld		b, a				// B = 16 - A = offset + 1
	xor		a
loopCropB16_1:
	inc		hl
	ld		(de), a
	inc		de
	djnz	loopCropB16_1

// loop(i, n)
// 	*dest++ = *src++;
	ld______a_iyl				// Retore A
	or		a
	jp		z, skipCropB16_2
	ld		c, a
	// B = 0 {from djnz}
	ldir

// for(u8 i = n; i < 16; ++i)
// 	*dest++ = 0;
	neg
skipCropB16_2:
	add		a, #16
	ld		b, a				// B = 16 - A = offset + 1
	xor		a
loopCropB16_2:
	ld		(de), a
	inc		de
	djnz	loopCropB16_2

	pop		hl
	inc		sp						// Adjusting stack address
	jp		(hl)					// Return to caller
__endasm;
}
#endif // (SPRITEFX_USE_16x16)

#endif // (SPRITEFX_USE_CROP)

//-----------------------------------------------------------------------------
// Flipping
//-----------------------------------------------------------------------------
#if (SPRITEFX_USE_FLIP)

#if (SPRITEFX_USE_8x8)
//-----------------------------------------------------------------------------
// Vertical flip 8x8 sprite
void SpriteFX_FlipVertical8(const u8* src, u8* dest) __NAKED __PRESERVES(iyl, iyh)
{
	src;	// HL
	dest;	// DE

__asm
	ld		bc, #7
	add		hl, bc
	ld		b, #8
flipLoopV:
	ld		a, (hl)
	dec		hl
	ld		(de), a
	inc		de 
	djnz	flipLoopV

	ret
__endasm;
}
#endif

#if (SPRITEFX_USE_16x16)
//-----------------------------------------------------------------------------
// Vertical flip 16x16 sprite
void SpriteFX_FlipVertical16(const u8* src, u8* dest) __NAKED __PRESERVES(iyl, iyh)
{
	src;	// HL
	dest;	// DE

__asm
	ld		bc, #15
	add		hl, bc
	ld		b, #16
flipLoopV1:
	ld		a, (hl)
	dec		hl
	ld		(de), a
	inc		de
	djnz	flipLoopV1

	ld		c, #32				// B is 0 because of djnz
	add		hl, bc
	ld		b, #16
flipLoopV2:
	ld		a, (hl)
	dec		hl
	ld		(de), a
	inc		de 
	djnz	flipLoopV2

	ret
__endasm;
}
#endif

#if (SPRITEFX_USE_8x8)
//-----------------------------------------------------------------------------
// Horizontally flip 8x8 sprite
void SpriteFX_FlipHorizontal8(const u8* src, u8* dest) __NAKED __PRESERVES(iyl, iyh)
{
	src;	// HL
	dest;	// DE

__asm
	ld		b, #8
flipLoopH:
	ld		a, (hl)
	inc		hl
	// Fast byte Fliping routine by John Metcaff
	//  7  6  5  4  3  2  1  0  Bit start position
	//  <1 <3 3> 1> <1 <3 3> 1> Direction and number of shift
	//  0  1  2  3  4  5  6  7  Bit end position
	//  17 bytes / 66 cycles
	ld		c, a		// A = 76543210  |  C = 76543210
	rlca				// A = 65432107  |  C = 76543210
	rlca				// A = 54321076  |  C = 76543210
	xor		c			// A = 54321076  |  C = 76543210  |  in A XOR bit to keep 5x3x1x7x don't care of others because of AND 0xAA :)
	and		#0xAA		// A = 5_3_1_7_  |  C = 76543210  |  mask bit in A to be carryed over from C _6_4_2_0
	xor		c			// A = 56341270  |  C = 76543210  |  restore back in A bit 5x3x1x7x and carry over _6_4_2_0
	ld		c, a		// A = 56341270  |  C = 56341270
	rlca				// A = 63412705  |  C = 56341270
	rlca				// A = 34127056  |  C = 56341270
	rlca				// A = 41270563  |  C = 56341270
	rrc		c			// A = 41270563  |  C = 05634127  |  rotate right C so bit 0 and 7 are at the right place for the XOR
	xor		c			// A = 41270563  |  C = 05634127  |  in A XOR bit to keep x12xx56x don't care of others because of AND 0x66 :)
	and		#0x66 		// A = _12__56_  |  C = 05634127  |  mask bit in A to be carryed over from C 0__34__7
	xor		c			// A = 01234567  |  C = 05634127  |  restore back in A bit x12xx56x and carry over 0__34__7
	ld		(de), a
	inc		de
	djnz	flipLoopH

	ret
__endasm;
}
#endif

#if (SPRITEFX_USE_16x16)
//-----------------------------------------------------------------------------
// Horizontally flip 16x16 sprite
void SpriteFX_FlipHorizontal16(const u8* src, u8* dest) __NAKED __PRESERVES(iyl, iyh)
{
	src;	// HL
	dest;	// DE

__asm
	ld		bc, #16
	add		hl, bc
	ld		b, #16
flipLoopH1:
	ld		a, (hl)
	inc		hl
	// Fast byte Fliping routine by John Metcaff
	ld		c, a		// a = 76543210
	rlca
	rlca				// a = 54321076
	xor		c
	and		#0xAA
	xor		c			// a = 56341270
	ld		c, a
	rlca
	rlca
	rlca				// a = 41270563
	rrc		c			// c = 05634127
	xor		c 
	and		#0x66 
	xor		c			// a = 01234567
	ld		(de), a
	inc		de
	djnz	flipLoopH1

	ld		bc, #-32
	add		hl, bc
	ld		b, #16
flipLoopH2:
	ld		a, (hl)
	inc		hl
	// Fast byte Fliping routine by John Metcaff
	ld		c, a		// a = 76543210
	rlca
	rlca				// a = 54321076
	xor		c
	and		#0xAA
	xor		c			// a = 56341270
	ld		c, a
	rlca
	rlca
	rlca				// a = 41270563
	rrc		c			// c = 05634127
	xor		c 
	and		#0x66 
	xor		c			// a = 01234567
	ld		(de), a
	inc		de
	djnz	flipLoopH2

	ret
__endasm;
}
#endif

#endif // (SPRITEFX_USE_FLIP)

//-----------------------------------------------------------------------------
// Masking
//-----------------------------------------------------------------------------
#if (SPRITEFX_USE_MASK)

#if (SPRITEFX_USE_8x8)
//-----------------------------------------------------------------------------
// Mask 8x8 sprite
void SpriteFX_Mask8(const u8* src, u8* dest, const u8* mask) __NAKED
{
	src;	// HL
	dest;	// DE
	mask;	// SP[2] -> IY

__asm
	pop		bc					// Return address
	pop		iy					// Mask address
	push	bc

	ld		b, #8
maskLoop8:
	ld		a, (hl)
	inc		hl
	and		0(iy)
	inc		iy
	ld		(de), a
	inc		de 
	djnz	maskLoop8

	ret
__endasm;
}
#endif

#if (SPRITEFX_USE_16x16)
//-----------------------------------------------------------------------------
// Mask 16x16 sprite
void SpriteFX_Mask16(const u8* src, u8* dest, const u8* mask) __NAKED
{
	src;	// HL
	dest;	// DE
	mask;	// SP[2] -> IY

	__asm
		pop		bc					// Return address
		pop		iy					// Mask address
		push	bc

		ld		b, #32
	maskLoop16:
		ld		a, (hl)
		and		0(iy)
		ld		(de), a
		inc		hl
		inc		de 
		inc		iy
		djnz	maskLoop16

		ret
	__endasm;
}
#endif

#endif // (SPRITEFX_USE_MASK)

//-----------------------------------------------------------------------------
// Rotating
//-----------------------------------------------------------------------------
#if (SPRITEFX_USE_ROTATE)

//-----------------------------------------------------------------------------
// Rotate 90° to right 8x8 sprite
void SpriteFX_RotateRight8(const u8* src, u8* dest) __NAKED __PRESERVES(iyl, iyh)
{
	src;	// HL -> DE
	dest;	// DE -> HL

	__asm
		ex		de, hl
		ld		c, #8
	rotOuterLoopR:
		ld		a, (de)
		inc		de
		push	hl
		ld		b, #8
	rotInnerLoopR:
		rla
		rr		(hl)
		inc		hl
		djnz	rotInnerLoopR
		pop		hl
		dec		c
		jp		nz, rotOuterLoopR

		ret
	__endasm;
}

#if (SPRITEFX_USE_16x16)
//-----------------------------------------------------------------------------
// Rotate 90° to right 16x16 sprite
void SpriteFX_RotateRight16(const u8* src, u8* dest)
{
	SpriteFX_RotateRight8(src,      dest + 16);
	SpriteFX_RotateRight8(src + 8,  dest);
	SpriteFX_RotateRight8(src + 16, dest + 24);
	SpriteFX_RotateRight8(src + 24, dest + 8);
}
#endif

//-----------------------------------------------------------------------------
// Rotate 90° to left 8x8 sprite
void SpriteFX_RotateLeft8(const u8* src, u8* dest) __NAKED __PRESERVES(iyl, iyh)
{
	src;	// HL -> DE
	dest;	// DE -> HL

	__asm
		ex		de, hl
		ld		c, #8
	rotOuterloopL:
		ld		a, (de)
		inc		de
		push	hl
		ld		b, #8
	rotInnerLoopL:
		rra
		rl		(hl)
		inc		hl
		djnz	rotInnerLoopL
		pop		hl
		dec		c
		jp		nz, rotOuterloopL

		ret
	__endasm;
}

#if (SPRITEFX_USE_16x16)
//-----------------------------------------------------------------------------
// Rotate 90° to left 16x16 sprite
void SpriteFX_RotateLeft16(const u8* src, u8* dest)
{
	SpriteFX_RotateLeft8(src,      dest + 8);
	SpriteFX_RotateLeft8(src + 8,  dest + 24);
	SpriteFX_RotateLeft8(src + 16, dest);
	SpriteFX_RotateLeft8(src + 24, dest + 16);
}
#endif

//-----------------------------------------------------------------------------
// Rotate 180° 8x8 sprite
void SpriteFX_RotateHalfTurn8(const u8* src, u8* dest) __NAKED __PRESERVES(iyl, iyh)
{
	src;	// HL
	dest;	// DE

	__asm
		ld		bc, #7
		add		hl, bc
		ld		b, #8
	rotLoop180:
		ld		a, (hl)
		dec		hl
		// Fast byte Fliping routine by John Metcaff
		ld		c, a		// a = 76543210
		rlca
		rlca				// a = 54321076
		xor		c
		and		#0xAA
		xor		c			// a = 56341270
		ld		c, a
		rlca
		rlca
		rlca				// a = 41270563
		rrc		c			// c = 05634127
		xor		c 
		and		#0x66 
		xor		c			// a = 01234567
		ld		(de), a
		inc		de
		djnz	rotLoop180

		ret
	__endasm;
}

#if (SPRITEFX_USE_16x16)
//-----------------------------------------------------------------------------
// Rotate 180° 16x16 sprite
void SpriteFX_RotateHalfTurn16(const u8* src, u8* dest)
{
	SpriteFX_RotateHalfTurn8(src,      dest + 24);
	SpriteFX_RotateHalfTurn8(src + 8,  dest + 16);
	SpriteFX_RotateHalfTurn8(src + 16, dest + 8);
	SpriteFX_RotateHalfTurn8(src + 24, dest);
}
#endif

#endif // (SPRITEFX_USE_ROTATE)