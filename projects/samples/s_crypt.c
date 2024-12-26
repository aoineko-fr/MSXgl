// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Data encryption and decryption sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "crypt.h"
#include "print.h"
#include "memory.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

// Test structure (7 bytes)
struct TestStruct
{
	u8 Foo;
	u16 Bar;
	c8 Tab[3];
	u8 Buz;
};

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Test data
const c8* g_InvalidData = "12345678901234";

//=============================================================================
// MEMORY DATA
//=============================================================================

// Data source test
struct TestStruct g_TestData = { 0xAB, 0x1234, { 'M', 'S', 'X' }, TRUE };

// String buffer to store decoded data
c8 g_Buffer[256];

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Print test structure
void PrintStruct()
{
	Print_DrawFormat("> %d, 0x%04x, [%d, %d, %d], %d\n", g_TestData.Foo, g_TestData.Bar, g_TestData.Tab[0], g_TestData.Tab[1], g_TestData.Tab[2], g_TestData.Buz);
}


//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize screen mode 0 (text)
	VDP_SetMode(VDP_MODE_SCREEN0);
	VDP_ClearVRAM();

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample6, 1);
	Print_DrawText(MSX_GL " ENCRYPTION SAMPLE");
	Print_DrawLineH(0, 1, 40);

	// Source
	Print_DrawText("\nSource:\n");
	PrintStruct();

	// Encode
	Print_DrawText("\nEncode:\n");
	Crypt_SetKey("MSXgl");
	Crypt_Encode((const void*)g_TestData, sizeof(g_TestData), g_Buffer);
	Print_DrawFormat("> %s\n", g_Buffer);

	// Clear
	Print_DrawText("\nClear:\n");
	Mem_Set(0, &g_TestData, sizeof(g_TestData));
	PrintStruct();

	// Decode
	Print_DrawText("\nDecode:\n");
	Crypt_Decode(g_Buffer, (void*)g_TestData);
	PrintStruct();

	// Clear
	Print_DrawText("\nClear:\n");
	Mem_Set(0, &g_TestData, sizeof(g_TestData));
	PrintStruct();

	// Source
	Print_DrawText("\nSource:\n");
	Print_DrawFormat("> %s\n", g_InvalidData);

	// Decode
	Print_DrawText("\nDecode:\n");
	if(Crypt_Decode(g_InvalidData, (void*)g_TestData))
		PrintStruct();
	else
		Print_DrawText("> Invalid data\n");

	u8 frame = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();

		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[frame++ & 0x03]);
	}
}