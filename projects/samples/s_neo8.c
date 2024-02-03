// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  ROM mapper sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

extern const c8* Seg500Data1;
extern const c8* Seg500Data2;
extern const c8* Seg800Data1;
extern const c8* Seg800Data2;

void Seg500Func1(u8 id) __banked;
void Seg800Func1(u8 id) __banked;

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Sample text
__at(0xA000) const c8 g_SampleText[] = "Segment #3 Data";

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Display from address
void DisplayMemory(u8 x, u8 y, u16 addr)
{
	Print_SetPosition(x, y);
	Print_DrawFormat("%i: ", GET_BANK_SEGMENT(5));
	const u8* ptr = (const u8*)addr; 
	Print_SetPosition(x + 5, y);
	loop(i, 32)
		Print_DrawChar(*ptr++);
}

#define TEST_SEG	5
#define TEST_ADDR	0xA000

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_ClearVRAM();

	Print_SetTextFont(g_Font_MGL_Sample6, 1); // Initialize font
	Print_DrawText(MSX_GL " NEO8 mapper sample"); // Display title
	Print_DrawLineH(0, 1, 40);

	SET_BANK_SEGMENT(0, 4); // Bank 0 (0000-1FFF) = Segment 4
	SET_BANK_SEGMENT(1, 5); // Bank 1 (2000-3FFF) = Segment 5
	SET_BANK_SEGMENT(2, 0); // Bank 2 (4000-5FFF) = Segment 0
	SET_BANK_SEGMENT(3, 1); // Bank 3 (6000-7FFF) = Segment 1
	SET_BANK_SEGMENT(4, 2); // Bank 4 (8000-9FFF) = Segment 2
	SET_BANK_SEGMENT(5, 3); // Bank 5 (A000-BFFF) = Segment 3

	DisplayMemory(0, 4, TEST_ADDR);
	u16 seg = GET_BANK_SEGMENT(5); // Backup segment in bank 5
	SET_BANK_SEGMENT(5, 500); // Set segment 500 in bank 5
	DisplayMemory(0, 6, TEST_ADDR);
	SET_BANK_SEGMENT(5, 800); // Set segment 800 in bank 5
	DisplayMemory(0, 8, TEST_ADDR);
	SET_BANK_SEGMENT(5, seg); // Restore segment in bank 5
	DisplayMemory(0, 10, TEST_ADDR);

	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();

		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);
	}
}