#include "msxgl.h"

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
// Vertical flip 8x8 sprite
void Sprite_FlipV8(const u8* src, u8* dest)
{
	src += 7;
	loop(i, 8)
		*dest++ = *src--;
}

//-----------------------------------------------------------------------------
// Vertical flip 16x16 sprite
void Sprite_FlipV16(const u8* src, u8* dest)
{
	src += 15;
	loop(i, 16)
		*dest++ = *src--;
	src += 32;
	loop(i, 16)
		*dest++ = *src--;
}

//-----------------------------------------------------------------------------
// Flip 8 bits value
u8 Sprite_Flip(u8 val) __PRESERVES(c, d, e, h, l, iyl, iyh)
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
// Horizontally flip 8x8 sprite
void Sprite_FlipH8(const u8* src, u8* dest)
{
	loop(i, 8)
		*dest++ = Sprite_Flip(*src++);
}

//-----------------------------------------------------------------------------
// Horizontally flip 16x16 sprite
void Sprite_FlipH16(const u8* src, u8* dest)
{
	dest += 16;
	loop(i, 16)
		*dest++ = Sprite_Flip(*src++);
	dest -= 32;
	loop(i, 16)
		*dest++ = Sprite_Flip(*src++);
}

//-----------------------------------------------------------------------------
// Mask 8x8 sprite
void Sprite_Mask8(const u8* src, u8* dest, const u8* mask)
{
	loop(i, 8)
		*dest++ = *src++ & *mask++;
}

//-----------------------------------------------------------------------------
// Mask 16x16 sprite
void Sprite_Mask16(const u8* src, u8* dest, const u8* mask)
{
	loop(i, 32)
		*dest++ = *src++ & *mask++;
}
