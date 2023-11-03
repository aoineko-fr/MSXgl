#include "msxgl.h"

const u8 g_SpriteMask[8] = 
{
	0b11111111,
	0b01111111,
	0b00111111,
	0b00011111,
	0b00001111,
	0b00000111,
	0b00000011,
	0b00000001,
};

//-----------------------------------------------------------------------------
// Crop 8x8 sprite left side
void Sprite_CropLeft8(const u8* src, u8* dest, u8 offset)
{
	u8 mask = g_SpriteMask[offset];
	loop(i, 8)
		*dest++ = *src++ & mask;
}

//-----------------------------------------------------------------------------
// Crop 16x16 sprite left side
void Sprite_CropLeft16(const u8* src, u8* dest, u8 offset)
{
	if(offset < 8)
	{
		u8 mask = g_SpriteMask[offset];
		loop(i, 16)
			*dest++ = *src++ & mask;
		loop(i, 16)
			*dest++ = *src++;
	}
	else
	{
		loop(i, 16)
			*dest++ = 0;
		u8 mask = g_SpriteMask[offset - 8];
		loop(i, 16)
			*dest++ = *src++ & mask;
	}
}

//-----------------------------------------------------------------------------
// Crop 8x8 sprite right side
void Sprite_CropRight8(const u8* src, u8* dest, u8 offset)
{
	u8 mask = ~g_SpriteMask[offset];
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
		u8 mask = ~g_SpriteMask[offset];
		loop(i, 16)
		*dest++ = *src++ & mask;
	}
	else
	{
		u8 mask = ~g_SpriteMask[offset - 8];
		loop(i, 16)
			*dest++ = *src++ & mask;
		loop(i, 16)
			*dest++ = 0;
	}
}

//-----------------------------------------------------------------------------
// Flip 8x8 sprite horizontally
void Sprite_FlipH8(const u8* src, u8* dest)
{
	src += 7;
	loop(i, 8)
		*dest++ = *src--;
}

//-----------------------------------------------------------------------------
// Flip 16x16 sprite horizontally
void Sprite_FlipH16(const u8* src, u8* dest)
{
	src += 15;
	loop(i, 16)
		*dest++ = *src--;
}

//-----------------------------------------------------------------------------
// Flip 8x8 sprite vertically
void Sprite_FlipV8(const u8* src, u8* dest)
{
}

//-----------------------------------------------------------------------------
// Flip 16x16 sprite vertically
void Sprite_FlipV16(const u8* src, u8* dest)
{
}
