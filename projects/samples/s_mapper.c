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

extern const c8* Seg4Data1;
extern const c8* Seg4Data2;
extern const u16 Seg5AsmData;

void Seg4Func1(u8 id) __banked;

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initial bank setting (initialized into crt0 code):
	//   Bank 0 => Segment 0
	//   Bank 1 => Segment 1
	//   Bank 2 => Segment 2
	//   Bank 3 => Segment 3
	
	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_ClearVRAM();

	Print_SetTextFont(g_Font_MGL_Sample6, 1); // Initialize font
	Print_DrawText(MSX_GL " MAPPER SAMPLE"); // Display title
	Print_DrawLineH(0, 1, 40);

	Print_SetPosition(0, 3);
	Print_DrawFormat("Banks segment: 0->%i 1->%i 2->%i 3->%i\n\n", GET_BANK_SEGMENT(0), GET_BANK_SEGMENT(1), GET_BANK_SEGMENT(2), GET_BANK_SEGMENT(3));
	Print_DrawText("Switch bank 3 to segment 5\n\n");
	SET_BANK_SEGMENT(3, 5); // Make segment #5 visible through bank #3
	Print_DrawFormat("Banks segment: 0->%i 1->%i 2->%i 3->%i\n\n", GET_BANK_SEGMENT(0), GET_BANK_SEGMENT(1), GET_BANK_SEGMENT(2), GET_BANK_SEGMENT(3));
	u8 seg = GET_BANK_SEGMENT(3);
	SET_BANK_SEGMENT(3, seg);

	// New bank setting:
	//   Bank 0 => Segment 0
	//   Bank 1 => Segment 1
	//   Bank 2 => Segment 2
	//   Bank 3 => Segment 5 (content of s_mapper_s5_b3.asm)
	Print_DrawText(&Seg5AsmData);
	
	Print_Return();
	Print_DrawCharX('.', 40);
	
	Print_Return();
	Print_DrawText("Call banked function (bank 2 seg 4)\n");
	Seg4Func1(0); // Bank 2 => Segment 4 (content of s_mapper_s4_b2.c)
	Seg4Func1(1); // Bank 2 => Segment 4 (content of s_mapper_s4_b2.c)

	// After banked function call, previous bank setting is restored:
	//   Bank 0 => Segment 0
	//   Bank 1 => Segment 1
	//   Bank 2 => Segment 2
	//   Bank 3 => Segment 5 (content of s_mapper_s5_b3.asm)

	Print_Return();
	Print_Return();
	Print_DrawFormat("Banks segment: 0->%i 1->%i 2->%i 3->%i\n\n", GET_BANK_SEGMENT(0), GET_BANK_SEGMENT(1), GET_BANK_SEGMENT(2), GET_BANK_SEGMENT(3));

	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();

		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);
	}
}