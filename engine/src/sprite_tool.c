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
#include "sprite_tool.h"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

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

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Cropping
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Crop 8x8 sprite left side
void Sprite_CropLeft8(const u8* src, u8* dest, u8 offset)
{
	u8 mask = g_SpriteMaskL[offset];
	loop(i, 8)
		*dest++ = *src++ & mask;
}

//-----------------------------------------------------------------------------
// Crop 16x16 sprite left side
void Sprite_CropLeft16(const u8* src, u8* dest, u8 offset)
{
	if(offset < 8)
	{
		u8 mask = g_SpriteMaskL[offset];
		loop(i, 16)
			*dest++ = *src++ & mask;
		loop(i, 16)
			*dest++ = *src++;
	}
	else
	{
		loop(i, 16)
			*dest++ = 0;
		src += 16;
		u8 mask = g_SpriteMaskL[offset - 8];
		loop(i, 16)
			*dest++ = *src++ & mask;
	}
}

//-----------------------------------------------------------------------------
// Crop 8x8 sprite right side
void Sprite_CropRight8(const u8* src, u8* dest, u8 offset)
{
	u8 mask = g_SpriteMaskR[offset];
	loop(i, 8)
		*dest++ = *src++ & mask;
}

//-----------------------------------------------------------------------------
// Crop 16x16 sprite right side
void Sprite_CropRight16(const u8* src, u8* dest, u8 offset)
{
	if(offset < 8)
	{
		loop(i, 16)
			*dest++ = *src++;
		u8 mask = g_SpriteMaskR[offset];
		loop(i, 16)
			*dest++ = *src++ & mask;
	}
	else
	{
		u8 mask = g_SpriteMaskR[offset - 8];
		loop(i, 16)
			*dest++ = *src++ & mask;
		loop(i, 16)
			*dest++ = 0;
	}
}

//-----------------------------------------------------------------------------
// Crop 8x8 sprite top border.
void Sprite_CropTop8(const u8* src, u8* dest, u8 offset)
{
	u8 n = ++offset;
	loop(i, n)
		*dest++ = 0;
	for(u8 i = n; i < 8; ++i)
		*dest++ = *src++;
}

//-----------------------------------------------------------------------------
// Crop 16x16 sprite top border.
void Sprite_CropTop16(const u8* src, u8* dest, u8 offset)
{
	u8 n = ++offset;
	loop(i, n)
		*dest++ = 0;
	for(u8 i = n; i < 16; ++i)
		*dest++ = *src++;
	loop(i, n)
		*dest++ = 0;
	for(u8 i = n; i < 16; ++i)
		*dest++ = *src++;
}

//-----------------------------------------------------------------------------
// Crop 8x8 sprite bottom border.
void Sprite_CropBottom8(const u8* src, u8* dest, u8 offset)
{
	u8 n = 15 - offset;
	loop(i, n)
		*dest++ = *src++;
	for(u8 i = n; i < 8; ++i)
		*dest++ = 0;
}

//-----------------------------------------------------------------------------
// Crop 16x16 sprite bottom border.
void Sprite_CropBottom16(const u8* src, u8* dest, u8 offset)
{
	u8 n = 15 - offset;
	loop(i, n)
		*dest++ = *src++;
	for(u8 i = n; i < 16; ++i)
		*dest++ = 0;
	loop(i, n)
		*dest++ = *src++;
	for(u8 i = n; i < 16; ++i)
		*dest++ = 0;
}

//-----------------------------------------------------------------------------
// Flipping
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Vertical flip 8x8 sprite
void Sprite_FlipVertical8(const u8* src, u8* dest)
{
	src;	// HL
	dest;	// DE

	__asm
		ex		de, hl
		ld		bc, #7
		add		hl, bc
		ld		b, #8
	flipLoopV:
		ld		a, (de)
		ld		(hl), a
		inc		de 
		dec		hl
		djnz	flipLoopV
	__endasm;
}

//-----------------------------------------------------------------------------
// Vertical flip 16x16 sprite
void Sprite_FlipVertical16(const u8* src, u8* dest)
{
	src;	// HL
	dest;	// DE

	__asm
		ex		de, hl

		ld		bc, #15
		add		hl, bc
		ld		b, #16
	flipLoopV1:
		ld		a, (de)
		ld		(hl), a
		inc		de 
		dec		hl
		djnz	flipLoopV1

		ld		bc, #32
		add		hl, bc
		ld		b, #16
	flipLoopV2:
		ld		a, (de)
		ld		(hl), a
		inc		de 
		dec		hl
		djnz	flipLoopV2

	__endasm;
}

//-----------------------------------------------------------------------------
// Horizontally flip 8x8 sprite
void Sprite_FlipHorizontal8(const u8* src, u8* dest)
{
	src;	// HL
	dest;	// DE

	__asm
		ld		b, #8
	flipLoopH:
		ld		a, (hl)
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
		inc		hl
		inc		de
		djnz	flipLoopH
	__endasm;
}

//-----------------------------------------------------------------------------
// Horizontally flip 16x16 sprite
void Sprite_FlipHorizontal16(const u8* src, u8* dest)
{
	src;	// HL
	dest;	// DE

	__asm
		ld		bc, #16
		add		hl, bc
		ld		b, #16
	flipLoopH1:
		ld		a, (hl)
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
		inc		hl
		inc		de
		djnz	flipLoopH1

		ld		bc, #-32
		add		hl, bc
		ld		b, #16
	flipLoopH2:
		ld		a, (hl)
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
		inc		hl
		inc		de
		djnz	flipLoopH2
	__endasm;
}

//-----------------------------------------------------------------------------
// Masking
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mask 8x8 sprite
void Sprite_Mask8(const u8* src, u8* dest, const u8* mask) __NAKED
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
		and		0(iy)
		ld		(de), a
		inc		hl
		inc		de 
		inc		iy
		djnz	maskLoop8

		ret
	__endasm;
}

//-----------------------------------------------------------------------------
// Mask 16x16 sprite
void Sprite_Mask16(const u8* src, u8* dest, const u8* mask) __NAKED
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

//-----------------------------------------------------------------------------
// Rotating
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Rotate 90° to right 8x8 sprite
void Sprite_RotateRight8(const u8* src, u8* dest) __PRESERVES(iyl, iyh)
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
		jr		nz, rotOuterLoopR
	__endasm;
}

//-----------------------------------------------------------------------------
// Rotate 90° to right 16x16 sprite
void Sprite_RotateRight16(const u8* src, u8* dest)
{
	Sprite_RotateRight8(src,      dest + 16);
	Sprite_RotateRight8(src + 8,  dest);
	Sprite_RotateRight8(src + 16, dest + 24);
	Sprite_RotateRight8(src + 24, dest + 8);
}

//-----------------------------------------------------------------------------
// Rotate 90° to left 8x8 sprite
void Sprite_RotateLeft8(const u8* src, u8* dest) __PRESERVES(iyl, iyh)
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
		jr		nz, rotOuterloopL
	__endasm;
}

//-----------------------------------------------------------------------------
// Rotate 90° to left 16x16 sprite
void Sprite_RotateLeft16(const u8* src, u8* dest)
{
	Sprite_RotateLeft8(src,      dest + 8);
	Sprite_RotateLeft8(src + 8,  dest + 24);
	Sprite_RotateLeft8(src + 16, dest);
	Sprite_RotateLeft8(src + 24, dest + 16);
}

//-----------------------------------------------------------------------------
// Rotate 180° 8x8 sprite
void Sprite_RotateHalfTurn8(const u8* src, u8* dest) __PRESERVES(iyl, iyh)
{
	src;	// HL
	dest;	// DE

	__asm
		ld		bc, #7
		add		hl, bc
		ld		b, #8
	rotLoop180:
		ld		a, (hl)
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
		dec		hl
		inc		de
		djnz	rotLoop180
	__endasm;
}

//-----------------------------------------------------------------------------
// Rotate 180° 16x16 sprite
void Sprite_RotateHalfTurn16(const u8* src, u8* dest)
{
	Sprite_RotateHalfTurn8(src,      dest + 24);
	Sprite_RotateHalfTurn8(src + 8,  dest + 16);
	Sprite_RotateHalfTurn8(src + 16, dest + 8);
	Sprite_RotateHalfTurn8(src + 24, dest);
}
