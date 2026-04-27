// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
//  available on GitHub (https://github.com/aoineko-fr/MSXgl/tree/main/tools/MSXtk)
//  under CC-BY-SA free license (https://creativecommons.org/licenses/by-sa/2.0/)
//─────────────────────────────────────────────────────────────────────────────

// std
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <ctime>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <map>

// MSXtk
#include "MSXtk.h"

//=============================================================================
// DEFINES
//=============================================================================

const char* VERSION = "0.0.1";

#define TRUE  (u8)true
#define FALSE (u8)false

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */
struct TestStruct
{
	u8 Foo;
	u16 Bar;
	c8 Tab[3];
	u8 Buz;

	void Print() { printf("%d, 0x%04X, [%d, %d, %d], %d\n", Foo, Bar, Tab[0], Tab[1], Tab[2], Buz); }
	void Clear() { memset(this, 0, sizeof(TestStruct)); }
};
#pragma pack(pop)   /* restore original alignment from stack */

//=============================================================================
// CONST DATA
//=============================================================================

//=============================================================================
// VARIABLES
//=============================================================================

struct TestStruct g_TestStruct = { 0xAB, 0x1234, { 'M', 'S', 'X' }, TRUE };

c8 g_Buffer[1024];

//=============================================================================
// CRYPT MODULE
//=============================================================================

const c8* g_CryptDefaultMap = "HJ48CKP9AWBRSDT3MYLQ0FG1NUE7X562";
const u16 g_CryptDefaultCode[8] =
{ //      43210     43210
	0b000'00000'000'01000, // 0
	0b000'01000'000'00100, // 1
	0b000'00100'000'00000, // 2
	0b000'00000'000'00001, // 3
	0b000'00000'000'10000, // 4
	0b000'00010'000'00000, // 5
	0b000'10001'000'00000, // 6
	0b000'00000'000'00010, // 7
};
const u8 g_ShiftTable[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

const c8* g_CryptKey = NULL;
const c8* g_CryptMap = g_CryptDefaultMap;
const u16* g_CryptCode = g_CryptDefaultCode;

//-----------------------------------------------------------------------------
//
inline void Crypt_SetKey(const c8* key) { g_CryptKey = key; }

//-----------------------------------------------------------------------------
//
inline void Crypt_SetMap(const c8* map) { g_CryptMap = map; }

//-----------------------------------------------------------------------------
//
inline void Crypt_SetCode(const u16* code) { g_CryptCode = code; }

//-----------------------------------------------------------------------------
// Result buffer size must be: size x 2 + 1 (3 if signature is used)
bool Crypt_Encode(const void* data, u8 size, c8* str)
{
	if (!g_CryptKey)
		return FALSE;

	const c8* key = g_CryptKey;
	const u8* ptr = (const u8*)data;

	loop(i, size)
	{
		u8 val = *ptr++;
		val += *key;

		u8 cnt = *key++;
		if (!*key)
			key = g_CryptKey;

		u16 bits = 0;
		u8 flag = 0x01;
		loop(j, 8)
		{
			if (val & flag)
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

//
u8 Crypt_SearchMap(u8 chr)
{
	u8 ret = 0xFF;
	const c8* map = g_CryptDefaultMap;
	loop(i, 32)
		if (chr == *map++)
			return i;
	return ret;
}

//-----------------------------------------------------------------------------
//
bool Crypt_Decode(const c8* str, void* data)
{
	const c8* key = g_CryptKey;
	u8* ptr = (u8*)data;

	while (*str)
	{
		// Retreive the less significant bits
		u8 lsb = Crypt_SearchMap(*str++);
		if (lsb == 0xFF)
			return FALSE; // Error: Invalid character (not found in the map)
		if (!*str)
			return FALSE; // Error: Invalid number of character

		// Retreive the most significant bits
		u8 msb = Crypt_SearchMap(*str++);
		if (msb == 0xFF)
			return FALSE; // Error: Invalid character (not found in the map)

		// Convert characters to value
		u16 bits = lsb + (msb << 8);
		u8 val = 0;
		u8 cnt = *key;
		loop(i, 8)
		{
			u16 code = g_CryptCode[cnt++ & 0x07];
			u16 test = bits & code;
			if (test)
			{
				if (test != code)
					return FALSE; // Error: Invalid control bits
				val |= g_ShiftTable[i];
			}
		}
		val -= *key++;
		if (!*key)
			key = g_CryptKey;

		// Write value
		*ptr++ = val;
	}

	return TRUE;
}


//=============================================================================
// FUNCTIONS
//=============================================================================


//-----------------------------------------------------------------------------
// Display help information
void PrintHelp()
{
	printf("MSXcrypt %s - Encrypt or decrypt the input data\n", VERSION);

}

//const char* ARGV[] = { "", "../testcases/crt0_rom_mapper.ihx", "-e", "rom", "-s", "0x4000", "-l", "256K", "-b", "16K", "-p", "0xFF" };
//#define DEBUG_ARGS

//-----------------------------------------------------------------------------
// MAIN LOOP 
int main(int argc, const char* argv[])
{
#ifdef DEBUG_ARGS
	argc = sizeof(ARGV) / sizeof(ARGV[0]); argv = ARGV;
#endif

	Crypt_SetKey("MSXgl");

	printf("\nSource:\n");
	g_TestStruct.Print();
	
	printf("\nEncode:\n");
	Crypt_Encode(&g_TestStruct, sizeof(g_TestStruct), g_Buffer);
	printf("%s\n", g_Buffer);

	printf("\nClear:\n");
	g_TestStruct.Clear();
	g_TestStruct.Print();

	printf("\nDecode:\n");
	Crypt_Decode(g_Buffer, &g_TestStruct);
	g_TestStruct.Print();

	return 0;
}
