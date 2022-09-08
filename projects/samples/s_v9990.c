// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "v9990.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font\font_mgl_sample6.h"

// Sprite
#include "content\v9990\data_v9_chr.h"

// Tiles
#include "content\v9990\data_v9_bg.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

const u16 g_Palette[] = 
{
	V9_RGB( 0,  0,  0), // Black
	V9_RGB(16, 16, 16), // Gray
	V9_RGB(31, 31, 31), // White

	V9_RGB(16,  0,  0), // Red dark
	V9_RGB(31,  0,  0), // Red med
	V9_RGB(31, 16, 16), // Red light

	V9_RGB( 0, 16,  0), // Green dark
	V9_RGB( 0, 31,  0), // Green med
	V9_RGB(16, 31, 16), // Green light

	V9_RGB( 0,  0, 16), // Blue dark
	V9_RGB( 0,  0, 31), // Blue med
	V9_RGB(16, 16, 31), // Blue light
};

//=============================================================================
// MEMORY DATA
//=============================================================================


//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
//
void DisplayMSX()
{
	Print_SetTextFont(g_Font_MGL_Sample6, 1); // Initialize font
	Print_SetColor(0xF, 0x0);
	Print_DrawText(MSX_GL " V9990 SAMPLE");
	Print_DrawLineH(0, 1, 40);

	const c8* str = "V9990 not found!";
	if(V9_Detect())
		str = "V9990 found!";
	Print_SetPosition(0, 3);
	Print_DrawText(str);

	Print_SetPosition(0, 5);
	Print_DrawText("Ports:\n\n 0-7\n 8-F");
	for(u8 i = 0; i < 16; ++i)
	{
		u8 val = V9_GetPort(V9_P00 + i);
		u8 x = 9 + (i % 8) * 3;
		u8 y = 7 + (i / 8);
		Print_SetPosition(x, y);
		Print_DrawHex8(val);
	}

	Print_SetPosition(0, 10);
	Print_DrawText("Registers:\n\n 00-07\n 08-0F\n 10-17\n 18-1F\n 20-27\n 28-2F\n 30-37\n 38-3F");
	for(u8 i = 0; i < 64; ++i)
	{
		u8 val = V9_GetRegister(i);
		u8 x = 9 + (i % 8) * 3;
		u8 y = 12 + (i / 8);
		Print_SetPosition(x, y);
		Print_DrawHex8(val);
	}

	Print_DrawLineH(0, 22, 40);
	Print_SetPosition(0, 23);
	Print_DrawText("R:Refresh C:Color");

}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_EnableVBlank(TRUE);
	VDP_ClearVRAM();

	DisplayMSX();

	V9_SetMode(V9_MODE_P1);
	// V9_SetRegister(9, 0/*V9_R08_IEV_ON*/);
	V9_ClearVRAM();
	
	V9_SetPalette(0, numberof(g_Palette), g_Palette);

	// V9_P1_PGT_A		0x00000	// Pattern Generator Table (Layer A). 8160 patterns max
	// V9_P1_SGT		0x00000	// Sprite Generator Table  
	// V9_P1_SPAT		0x3FE00	// Sprite Attribute Table
	// V9_P1_PGT_B		0x40000	// Pattern Generator Table (Layer B). 7680 patterns max
	// V9_P1_PNT_A		0x7C000	// Pattern Name Table (Layer A)
	// V9_P1_PNT_B		0x7E000	// Pattern Name Table (Layer B)

	V9_WriteVRAM(V9_P1_PGT_A, g_DataV9BG, sizeof(g_DataV9BG));
	// V9_WriteVRAM(V9_P1_PGT_B, g_DataV9BG, sizeof(g_DataV9BG));

	for(u16 i = 0; i < 32*27; i++)
	{
		V9_FillVRAM(V9_P1_PNT_A + (i * 2), i & 0xFF, 1);
		// V9_FillVRAM(V9_P1_PNT_B + (i * 2), i & 0xFF, 1);
	}

	u8 count = 0;
	u8 clr = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();
		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);

		// V9_SetPort(6, 0x07);
		// 

		if(Keyboard_IsKeyPressed(KEY_R))
			DisplayMSX();
		if(Keyboard_IsKeyPressed(KEY_C))
			V9_SetRegister(15, clr++);
	}
}