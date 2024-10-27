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

#define SEG_PANEL_X 25

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

//
const c8 g_SampleText[] = "Sample text";

__at(0x0F8000) const c8 g_SampleText15[] = "Seg 15 text";
// __at(0xFFA000) const c8 g_SampleText255[] = "Seg 255 text";

//=============================================================================
// MEMORY DATA
//=============================================================================

// Screen mode setting index
u8 g_VBlank = 0;
u8 g_Frame = 0;
u8 g_Phase = 0;
u8 g_Temp;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// H_TIMI interrupt hook
void VBlankHook()
{
	g_VBlank = 1;
	g_Frame++;

	u8 sb3 = GET_BANK_SEGMENT(3);
	SET_BANK_SEGMENT(3, g_Frame); // Make segment #1 visible through bank #3
	g_Temp = GET_BANK_SEGMENT(3);
	SET_BANK_SEGMENT(3, sb3); // Restore segment in bank #2
}

//-----------------------------------------------------------------------------
// Wait for V-Blank period
void WaitVBlank()
{
	while(g_VBlank == 0) {}
	g_VBlank = 0;
}

//-----------------------------------------------------------------------------
//
void DrawSegments(u8 y, const c8* txt)
{
	Print_DrawTextAt(0, y, txt);
	Print_DrawIntAt(SEG_PANEL_X,      y, GET_BANK_SEGMENT(0));
	Print_DrawIntAt(SEG_PANEL_X + 4,  y, GET_BANK_SEGMENT(1));
	Print_DrawIntAt(SEG_PANEL_X + 8,  y, GET_BANK_SEGMENT(2));
	Print_DrawIntAt(SEG_PANEL_X + 12, y, GET_BANK_SEGMENT(3));
}

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

	Print_SetPosition(SEG_PANEL_X, 3);
	Print_DrawText("B#0 B#1 B#2 B#3");

	// INIT
	//   Bank 0 => Segment 0
	//   Bank 1 => Segment 1
	//   Bank 2 => Segment 2
	//   Bank 3 => Segment 3
	DrawSegments(5, "Initial segments");
	
	// SIMPLE SWITCH
	//   Bank 0 => Segment 0
	//   Bank 1 => Segment 1
	//   Bank 2 => Segment 2
	//   Bank 3 => Segment 5 (content of s_mapper_s5_b3.asm)
	SET_BANK_SEGMENT(3, 5); // Make segment #5 visible through bank #3
	DrawSegments(7, "Switch bank 3 to seg 5");
	Print_SetPosition(0, 8);
	Print_DrawFormat("Data: [%s]", &Seg5AsmData);

	// BANKED CALL
	//   Bank 0 => Segment 0
	//   Bank 1 => Segment 1
	//   Bank 2 => Segment 2 (before/after calling) | 4 (content of s_mapper_s4_b2.c during banked call)
	//   Bank 3 => Segment 5 (content of s_mapper_s5_b3.asm)
	Print_DrawTextAt(0, 10, "Call banked functions");
	Seg4Func1(11); // Bank 2 => Segment 4 (content of s_mapper_s4_b2.c)
	Seg4Func1(12); // Bank 2 => Segment 4 (content of s_mapper_s4_b2.c)
	DrawSegments(13, "After banked call");

	// MIRRORING
	//   Bank 0 => Segment 0
	//   Bank 1 => Segment 1
	//   Bank 2 => Segment 0
	//   Bank 3 => Segment 1
	u8 sb2 = GET_BANK_SEGMENT(2);
	SET_BANK_SEGMENT(2, 0); // Make segment #0 visible through bank #2
	u8 sb3 = GET_BANK_SEGMENT(3);
	SET_BANK_SEGMENT(3, 1); // Make segment #1 visible through bank #3
	DrawSegments(15, "Set mirroring");
	Print_SetPosition(0, 16);
	Print_DrawFormat("At %4xh: [%s]", g_SampleText, g_SampleText);
	Print_SetPosition(0, 17);
	Print_DrawFormat("At %4xh: [%s]", g_SampleText + 0x4000, g_SampleText + 0x4000);

	// RESTORE
	//   Bank 0 => Segment 0
	//   Bank 1 => Segment 1
	//   Bank 2 => Segment 2
	//   Bank 3 => Segment 5 (content of s_mapper_s5_b3.asm)
	SET_BANK_SEGMENT(2, sb2); // Make previous segment visible through bank #2
	SET_BANK_SEGMENT(3, sb3); // Make previous segment visible through bank #3
	DrawSegments(19, "Restore segments");

	// OUT OF BOUNDS
	//   Bank 0 => Segment 0
	//   Bank 1 => Segment 1
	//   Bank 2 => Segment 15 (last segment)
	//   Bank 3 => Segment 255
	SET_BANK_SEGMENT(2, 15); // Make segment #15 visible through bank #2
	SET_BANK_SEGMENT(3, 255); // Make segment #255 visible through bank #3
	DrawSegments(19, "Out of bounds seg");
	Print_SetPosition(0, 20);
	Print_DrawFormat("Bank 2: [%s]", g_SampleText15);
	Print_SetPosition(0, 21);
	Print_DrawFormat("Bank 3: [%s]", 0xA000);

	VDP_EnableVBlank(TRUE);
	Bios_SetHookCallback(H_TIMI, VBlankHook);

	u8 frameCount = 0;
	u8 errorCount = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		// WaitVBlank();

		SET_BANK_SEGMENT(3, frameCount);

		if(GET_BANK_SEGMENT(3) != frameCount)
			errorCount++;

		Print_DrawIntAt(0, 23, errorCount);
		Print_DrawCharAt(39, 0, g_ChrAnim[frameCount++ & 0x03]);
	}

	Bios_ClearHook(H_TIMI);
	Bios_Exit(0);
}