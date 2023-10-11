// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄            
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄  ▄███ ▀█▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀█▄▄ ▄▀█▄
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

struct TestStruct
{
	u8 Foo;
	u16 Bar;
	c8 Tab[3];
	u8 Buz;

	void Print() { printf("%d, %X, [%d, %d, %d], %d\n", Foo, Bar, Tab[0], Tab[1], Tab[2], Buz); }
	void Clear() { memset(this, 0, sizeof(TestStruct)); }
};

//=============================================================================
// CONST DATA
//=============================================================================

const c8* g_DefaultKey = "MSXgl.Encryptor";
const c8* g_DefaultCharMap = "0123456789ABCDEFGHJKLMNPQRSTUWXY";

//=============================================================================
// VARIABLES
//=============================================================================

//#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */

struct TestStruct g_TestStruct = { 0xAB, 0x1234, { 'M', 'S', 'X' }, TRUE };

//#pragma pack(pop)   /* restore original alignment from stack */

c8 g_Buffer[1024];

//=============================================================================
// CRYPT MODULE
//=============================================================================

const c8* g_CryptKey = g_DefaultKey;
const c8* g_CryptMap = g_DefaultCharMap;
bool g_CryptSign = FALSE;

inline void Crypt_SetKey(const c8* key) { g_CryptKey = key; }
inline void Crypt_SetMap(const c8* map) { g_CryptMap = map; }
inline void Crypt_SetSign(bool sign) { g_CryptSign = sign; }

// Result buffer size must be: size x 2 + 1 (3 if signature is used)
void Crypt_Encode(const void* data, u8 size, c8* str)
{
	u8 keyCnt = 0;
	const u8* ptr = (const u8*)data;
	for (u8 i = 0; i < size; ++i)
	{
		u8 bits = ptr[i] & 0x0F;
		if (bits & 0b0100)
			bits |= 0x10;
		*str++ = g_CryptMap[bits];

		bits = ptr[i] >> 4;
		if (bits & 0b0010)
			bits |= 0x10;
		*str++ = g_CryptMap[bits];
	}
	*str = 0;
}

//
void Crypt_Decode(const c8* str, void* data)
{
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

	//const c8* str = "T35T 5TR1NG!";
	//printf("%s\n", str);
	
	g_TestStruct.Print();
	
	Crypt_Encode(&g_TestStruct, sizeof(g_TestStruct), g_Buffer);
	printf("%s\n", g_Buffer);

	g_TestStruct.Clear();

	Crypt_Decode(g_Buffer, &g_TestStruct);
	g_TestStruct.Print();

	return 0;
}
