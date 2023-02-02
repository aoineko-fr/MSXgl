// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Real-time clock module sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

// Graph base position
#define GRAPH_X			16
#define GRAPH_Y			48
#define GRAPH_U			4

// Random sample count
#define RAND_SAMPLE		0x8000 // 16384

#define CURVE_STEP		64

//
enum CURVE_FORMAT
{
	CURVE_8B_SIGNED		= 0,
	CURVE_8B_UNSIGNED,
	CURVE_16B_SIGNED,
	CURVE_16B_UNSIGNED,
};

//
struct CurveEntry
{
	const c8*		Name;
	const void*		Data;
	const c8*		Min;
	const c8*		Max;
	u16				Count;
	u16				Offset;
	u8				ZeroX;
	u8				ZeroY;
	u8				Format;
};

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"

// Sinus & cosinus table
#include "mathtable/mt_trigo_64.inc"
#include "mathtable/mt_trigo_ext_64.inc"
#include "mathtable/mt_sq_256.inc"
#include "mathtable/mt_sqrt_256.inc"

// Character animation
const u8 chrAnim[] = { '|', '\\', '-', '/' };

const struct CurveEntry g_Curves[] =
{
	{ "Sine", 			g_Sinus64,			"0",		"Pi*2", 64, 	6,	0,		106, CURVE_16B_SIGNED },
	{ "Cosine", 		g_Cosinus64,		"0",		"Pi*2", 64, 	6,	0,		106, CURVE_16B_SIGNED },
	{ "Tangent", 		g_Tangent64,		"-Pi/2",	"Pi*2", 64, 	3,	128,	106, CURVE_16B_SIGNED },
	{ "Arcsine", 		g_ArcSinus64,		"-1",		"1", 	64, 	2,	128,	106, CURVE_16B_SIGNED },
	{ "Arccosine", 		g_ArcCosinus64,		"-1",		"1", 	64, 	2,	128,	180, CURVE_16B_SIGNED },
	{ "Arctangent",		g_ArcTangent64,		"0",		"?", 	64, 	2,	0,		170, CURVE_16B_SIGNED },
	{ "Square", 		g_Square256,		"0",		"1", 	256,	5,	0,		170, CURVE_16B_UNSIGNED },
	{ "Square Root",	g_SquareRoot256,	"0",		"?", 	256,	1,	0,		170, CURVE_8B_UNSIGNED },
};


//=============================================================================
// MEMORY DATA
//=============================================================================

// Screen mode setting index
u8  g_VBlank = 0;
u16 g_Frame = 0;
i8  g_CurveIdx = 0;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// H_TIMI interrupt hook
void VBlankHook()
{
	g_VBlank = 1;
	g_Frame++;
}

//-----------------------------------------------------------------------------
// Wait for V-Blank period
void WaitVBlank()
{
	while(g_VBlank == 0) {}
	g_VBlank = 0;
}

//-----------------------------------------------------------------------------
// Diplay page header
void DisplayHeader(const c8* title)
{
	// Setup screen
	VDP_SetMode(VDP_MODE_SCREEN5);
	VDP_SetColor(COLOR_DARK_BLUE);
	VDP_CommandHMMV(0, 0, 256, 1024, COLOR_MERGE2(COLOR_DARK_BLUE)); // Clear VRAM
	VDP_CommandWait();

	// Setup print
	Print_SetBitmapFont(g_Font_MGL_Sample6);
	Print_SetColor(COLOR_WHITE, COLOR_DARK_BLUE);
	Print_Clear();
	Print_SetPosition(0, 2);
	Print_DrawText("\x1\x2\x3\x4\x5\x6   MATH SAMPLE - ");
	Print_DrawText(title);
	Draw_LineH(0, 255, 12, COLOR_WHITE, 0);
}


//-----------------------------------------------------------------------------
// Diplay page footer
void DisplayFooter()
{
	Draw_LineH(0, 255, 199, COLOR_WHITE, 0);
	Print_SetColor(COLOR_WHITE, COLOR_DARK_BLUE);
	Print_SetPosition(0, 203);
	Print_DrawText("F1:Rnd8  F2:Rnd16  F3:Curve  F4:Func");
}

//-----------------------------------------------------------------------------
// Diplay page footer
void DisplayRandom16()
{
	DisplayHeader("Random 16b");
	
	Print_SetPosition(0, 20);
	Print_DrawText("Method: ");
	Print_DrawText(RANDOM_16_NAME);

	Draw_Box(GRAPH_X - 1, GRAPH_Y, (u8)(GRAPH_X + 128), (u8)(GRAPH_Y + 128 + 1), COLOR_WHITE, 0);

	Print_SetColor(COLOR_LIGHT_BLUE, COLOR_DARK_BLUE);
	Print_SetPosition(158, GRAPH_Y);
	Print_DrawText("0 hit");
	Print_SetPosition(158, GRAPH_Y + 8 * 5);
	Print_DrawText("5+ hits");

	VDP_CommandHMMV(150, GRAPH_Y + 8 * 1, 4, 8, COLOR_MERGE2(COLOR_LIGHT_BLUE));
	VDP_CommandHMMV(150, GRAPH_Y + 8 * 2, 4, 8, COLOR_MERGE2(COLOR_CYAN));
	VDP_CommandHMMV(150, GRAPH_Y + 8 * 3, 4, 8, COLOR_MERGE2(COLOR_LIGHT_GREEN));
	VDP_CommandHMMV(150, GRAPH_Y + 8 * 4, 4, 8, COLOR_MERGE2(COLOR_LIGHT_YELLOW));
	VDP_CommandHMMV(150, GRAPH_Y + 8 * 5, 4, 8, COLOR_MERGE2(COLOR_LIGHT_RED));

	u16 startFrame = g_Frame;
	for(u16 i = 0; i < RAND_SAMPLE; ++i)
	{
		u16 rand = Math_GetRandom16();
		u16 x = GRAPH_X + ((rand % 0x00FF) / 2);
		u16 y = GRAPH_Y + 128 - ((rand >> 8) / 2);
		switch(VDP_CommandPOINT(x, y))
		{
		case COLOR_DARK_BLUE:		VDP_CommandPSET(x, y, COLOR_LIGHT_BLUE, 0); break;
		case COLOR_LIGHT_BLUE:		VDP_CommandPSET(x, y, COLOR_CYAN, 0); break;
		case COLOR_CYAN:			VDP_CommandPSET(x, y, COLOR_LIGHT_GREEN, 0); break;
		case COLOR_LIGHT_GREEN:  	VDP_CommandPSET(x, y, COLOR_LIGHT_YELLOW, 0); break;
		default: 					VDP_CommandPSET(x, y, COLOR_LIGHT_RED, 0); break;
		};
		
	}
	
	Print_SetPosition(0, 184);
	Print_DrawText("t=");
	Print_DrawInt(g_Frame - startFrame);	
	
	DisplayFooter();
}

//-----------------------------------------------------------------------------
// Diplay page footer
void DisplayRandom8()
{
	DisplayHeader("Random 8b");

	Print_SetPosition(0, 20);
	Print_DrawText("Method: ");
	Print_DrawText(RANDOM_8_NAME);

	Draw_Box(GRAPH_X - 1, GRAPH_Y, (u8)(GRAPH_X + 128), (u8)(GRAPH_Y + 128 + 1), COLOR_WHITE, 0);

	Print_SetColor(COLOR_LIGHT_BLUE, COLOR_DARK_BLUE);
	Print_SetPosition(158, GRAPH_Y);
	Print_DrawText("0 hit");
	Print_SetPosition(158, GRAPH_Y + 8 * 5);
	Print_DrawText("5+ hits");

	VDP_CommandHMMV(150, GRAPH_Y + 8 * 1, 4, 8, COLOR_MERGE2(COLOR_LIGHT_BLUE));
	VDP_CommandHMMV(150, GRAPH_Y + 8 * 2, 4, 8, COLOR_MERGE2(COLOR_CYAN));
	VDP_CommandHMMV(150, GRAPH_Y + 8 * 3, 4, 8, COLOR_MERGE2(COLOR_LIGHT_GREEN));
	VDP_CommandHMMV(150, GRAPH_Y + 8 * 4, 4, 8, COLOR_MERGE2(COLOR_LIGHT_YELLOW));
	VDP_CommandHMMV(150, GRAPH_Y + 8 * 5, 4, 8, COLOR_MERGE2(COLOR_LIGHT_RED));

	u16 startFrame = g_Frame;
	for(u16 i = 0; i < RAND_SAMPLE; ++i)
	{
		#if (MSX_VERSION != MSX_TR) && ((RANDOM_8_METHOD == RANDOM_8_REGISTER) || (RANDOM_8_METHOD == RANDOM_8_RACC)) // 7-bits RNG
		u16 x = GRAPH_X + Math_GetRandom8();
		u16 y = GRAPH_Y + 128 - Math_GetRandom8();
		#else // 8-bits RNG
		u16 x = GRAPH_X + (Math_GetRandom8() / 2);
		u16 y = GRAPH_Y + 128 - (Math_GetRandom8() / 2);
		#endif
		switch(VDP_CommandPOINT(x, y))
		{
		case COLOR_DARK_BLUE:		VDP_CommandPSET(x, y, COLOR_LIGHT_BLUE, 0); break;
		case COLOR_LIGHT_BLUE:		VDP_CommandPSET(x, y, COLOR_CYAN, 0); break;
		case COLOR_CYAN:			VDP_CommandPSET(x, y, COLOR_LIGHT_GREEN, 0); break;
		case COLOR_LIGHT_GREEN:  	VDP_CommandPSET(x, y, COLOR_LIGHT_YELLOW, 0); break;
		default: 					VDP_CommandPSET(x, y, COLOR_LIGHT_RED, 0); break;
		};
		
	}

	Print_SetPosition(0, 184);
	Print_DrawText("t=");
	Print_DrawInt(g_Frame - startFrame);	

	DisplayFooter();
}

//-----------------------------------------------------------------------------
// Diplay 
void DisplayFunc()
{
	DisplayHeader("Function");

	Print_SetPosition(0, 20);

	i16 rnd8, val8;
	i16 rnd16, val16;

	// 8-bits
	rnd8 = (i16)Math_GetRandom8();
	Print_DrawFormat("A=Random8():  %i [%8b]\n", rnd8, rnd8);

	val8 = Math_Div10(rnd8);
	Print_DrawFormat("Div10(A):     %i\n", val8);

	val8 = Math_Mod10(rnd8);
	Print_DrawFormat("Mod10(A):     %i\n", val8);

	val8 = Math_Flip(rnd8);
	Print_DrawFormat("Flip(A):      %i [%8b]\n", val8, val8);

	val8 = Math_Negative(rnd8);
	Print_DrawFormat("Negative(B):  %i\n", val8);

	val8 = Math_SignedDiv2(rnd8);
	Print_DrawFormat("Div2(A):      %i\n", val8);

	val8 = Math_SignedDiv4(rnd8);
	Print_DrawFormat("Div4(A):      %i\n", val8);

	val8 = Math_SignedDiv8(rnd8);
	Print_DrawFormat("Div8(A):      %i\n", val8);

	val8 = Math_SignedDiv16(rnd8);
	Print_DrawFormat("Div16(A):     %i\n", val8);

	val8 = Math_SignedDiv32(rnd8);
	Print_DrawFormat("Div32(A):     %i\n", val8);

	// 16-bits
	Print_Return();

	rnd16 = Math_GetRandom16();
	Print_DrawFormat("B=Random16(): %i [%b]\n", rnd16, rnd16);

	val16 = Math_Div10_16b(rnd16);
	Print_DrawFormat("Div10_16b(B): %i\n", val16);

	val16 = Math_Mod10_16b(rnd16);
	Print_DrawFormat("Mod10_16b(B): %i\n", val16);

	val16 = Math_Flip_16b(rnd16);
	Print_DrawFormat("Flip_16b(B):  %i [%b]\n", val16, val16);

	val16 = Math_Negative16(rnd16);
	Print_DrawFormat("Negative16(B):%i\n", val16);

	val16 = Math_Swap(rnd16);
	Print_DrawFormat("Swap(B):      %i [%b]\n", val16, val16);

	DisplayFooter();
}

//-----------------------------------------------------------------------------
// Diplay page footer
void DisplayCurve()
{
	DisplayHeader("Curve");
	
	const struct CurveEntry* curve = &g_Curves[g_CurveIdx];

	Print_SetPosition(0, 20);
	Print_DrawText("Table: ");
	Print_DrawText(curve->Name);

	Draw_LineH(0, 255, curve->ZeroY, COLOR_LIGHT_BLUE, 0);
	Draw_LineV(curve->ZeroX, 32, 196, COLOR_LIGHT_BLUE, 0);

	Print_SetColor(COLOR_LIGHT_BLUE, COLOR_DARK_BLUE);
	Print_SetPosition(2, curve->ZeroY + 2);
	Print_DrawText(curve->Min);
	Print_SetPosition(255 - 2 - String_Length(curve->Max) * 6, curve->ZeroY + 2);
	Print_DrawText(curve->Max);

	u8 prevX = 0, prevY = 0;
	for(u16 i = 0; i < CURVE_STEP; ++i)
	{
		u8 idx = i * (u8)((u16)curve->Count / CURVE_STEP);
		
		u8 x = i * (u8)((u16)256 / CURVE_STEP);
		
		u8 y = curve->ZeroY;
		switch(curve->Format)
		{
			case CURVE_8B_SIGNED:
			{
				const i8* ptr = (const i8*)curve->Data;
				y -= ptr[idx] >> curve->Offset;
				break;
			}
			case CURVE_8B_UNSIGNED:
			{
				const u8* ptr = (const u8*)curve->Data;
				y -= ptr[idx] >> curve->Offset;
				break;
			}
			case CURVE_16B_SIGNED:
			{
				const i16* ptr = (const i16*)curve->Data;
				y -= ptr[idx] >> curve->Offset;
				break;
			}
			case CURVE_16B_UNSIGNED:
			{
				const u16* ptr = (const u16*)curve->Data;
				y -= ptr[idx] >> curve->Offset;
				break;
			}
		};
		
		#if(0)
		VDP_CommandPSET(x, y, COLOR_WHITE, 0);
		#else
		if(i > 0)
			Draw_Line(prevX, prevY, x, y, COLOR_WHITE, 0);
		#endif
		
		prevX = x;
		prevY = y;
	}
	
	DisplayFooter();
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	#if (MSX_VERSION == MSX_TR)
	Bios_SetCPUMode(CPU_MODE_R800_ROM + CPU_TURBO_LED);
	#endif

	Bios_SetHookCallback(H_TIMI, VBlankHook);

	DisplayCurve();
	
	bool bContinue = TRUE;
	while(bContinue)
	{
		WaitVBlank();
		
		Print_SetPosition(248, 2);
		Print_DrawChar(chrAnim[g_Frame & 0x03]);

		u8 row6 = Keyboard_Read(6);
		if(IS_KEY_PRESSED(row6, KEY_F1))
			DisplayRandom8();
		else if(IS_KEY_PRESSED(row6, KEY_F2))
			DisplayRandom16();
		else if(IS_KEY_PRESSED(row6, KEY_F3))
			DisplayCurve();

		u8 row7 = Keyboard_Read(7);
		if(IS_KEY_PRESSED(row7, KEY_F4))
			DisplayFunc();

		u8 row8 = Keyboard_Read(8);
		if(IS_KEY_PRESSED(row8, KEY_UP) || IS_KEY_PRESSED(row8, KEY_LEFT))
		{
			g_CurveIdx--;
			if(g_CurveIdx < 0)
				g_CurveIdx = numberof(g_Curves) - 1;
			DisplayCurve();
		}
		else if(IS_KEY_PRESSED(row8, KEY_DOWN) || IS_KEY_PRESSED(row8, KEY_RIGHT))
		{
			g_CurveIdx++;
			if(g_CurveIdx >= numberof(g_Curves))
				g_CurveIdx = 0;
			DisplayCurve();
		}

		if(Keyboard_IsKeyPressed(KEY_ESC))
			bContinue = FALSE;
	}

	Bios_ClearHook(H_TIMI);
	Bios_Exit(0);
}


