// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Data encryption and decryption module.
//─────────────────────────────────────────────────────────────────────────────
#include "crypt.h"

//-----------------------------------------------------------------------------
// CONST DATA
//-----------------------------------------------------------------------------

// Default string containing the 32 valid characters
const c8 g_CryptDefaultMap[] = "HJ48CKP9AWBRSDT3MYLQ0FG1NUE7X562";

// Default bit-field coding table
const u16 g_CryptDefaultCode[8] =
{ //     43210   43210
	0b0000000000001000, // 0
	0b0000100000000100, // 1
	0b0000010000000000, // 2
	0b0000000000000001, // 3
	0b0000000000010000, // 4
	0b0000001000000000, // 5
	0b0001000100000000, // 6
	0b0000000000000010, // 7
};

const u8 g_ShiftTable[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

//-----------------------------------------------------------------------------
// VARIABLES
//-----------------------------------------------------------------------------

// 
const c8* g_CryptKey = NULL;

// String containing the 32 valid characters
const c8* g_CryptMap = g_CryptDefaultMap;

// Bit field coding table
const u16* g_CryptCode = g_CryptDefaultCode;

//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Encrypt data
// Note: Destination buffer size must be: input data size x 2 + 1
bool Crypt_Encode(const void* data, u8 size, c8* str)
{
	if(!g_CryptKey)
		return FALSE;

	const c8* key = g_CryptKey;
	const u8* ptr = (const u8*)data;

	u8 prev = 0;
	loop(i, size)
	{
		u8 val = *ptr;
		val += prev;
		prev = *ptr++;
		val += *key;

		u8 cnt = *key++;
		if(!*key)
			key = g_CryptKey;

		u16 bits = 0;
		u8 flag = 0x01;
		loop(j, 8)
		{
			if(val & flag)
				bits |= g_CryptCode[cnt & 0x07];
			cnt++;
			flag <<= 1;
		}
		*str++ = g_CryptMap[bits & 0x00FF];
		*str++ = g_CryptMap[bits >> 8];
	}
	*str = 0;

	return TRUE;
}

//-----------------------------------------------------------------------------
//
u8 Crypt_SearchMap(u8 chr)
{
	const c8* map = g_CryptMap;
	loop(i, 32)
		if(chr == *map++)
			return i;

	return 0xFF;
}

//-----------------------------------------------------------------------------
// Decrypt data
// Note: Destination buffer size must be: length of the string / 2
bool Crypt_Decode(const c8* str, void* data)
{
	const c8* key = g_CryptKey;
	u8* ptr = (u8*)data;

	u8 prev = 0;
	while(*str)
	{
		// Retreive the less significant bits
		u8 lsb = Crypt_SearchMap(*str++);
		if(lsb == 0xFF)
			return FALSE; // Error: Invalid character (not found in the map)
		if(!*str)
			return FALSE; // Error: Invalid number of character

		// Retreive the most significant bits
		u8 msb = Crypt_SearchMap(*str++);
		if(msb == 0xFF)
			return FALSE; // Error: Invalid character (not found in the map)

		// Convert characters to value
		u16 bits = lsb + (msb << 8);
		u8 val = 0;
		u8 cnt = *key;
		loop(i, 8)
		{
			u16 code = g_CryptCode[cnt++ & 0x07];
			u16 test = bits & code;
			if(test)
			{
				if(test != code)
					return FALSE; // Error: Invalid control bits
				val |= g_ShiftTable[i];
			}
		}
		val -= *key++;
		val -= prev;
		if(!*key)
			key = g_CryptKey;

		// Write value
		*ptr++ = val;
		prev = val;
	}

	return TRUE;
}
