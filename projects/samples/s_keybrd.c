// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Keyboard sample program
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

/// Key's entry
typedef struct
{
	u8 code;
	u8 posX, posY;
	const c8* text;
} KeyEntry;

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"

// Keys entries table
const KeyEntry keys[] =
{
	{ KEY_F1,		3,		4,	"F1" },
	{ KEY_F2,		6,		4,	"F2" },
	{ KEY_F3,		9,		4,	"F3" },
	{ KEY_F4,		12,		4,	"F4" },
	{ KEY_F5,		15,		4,	"F5" },
	{ KEY_SELECT,	20,		4,	"SEL" },
	{ KEY_STOP,		24,		4,	"STP" },
	{ KEY_HOME,		28,		4,	"HOM" },
	{ KEY_INS,		32,		4,	"INS" },
	{ KEY_DEL,		36,		4,	"DEL" },
	
	{ KEY_ESC,		3,		5,	"ESC" },
	{ KEY_1,		7,		5,	"1" },
	{ KEY_2,		9,		5,	"2" },
	{ KEY_3,		11,		5,	"3" },
	{ KEY_4,		13,		5,	"4" },
	{ KEY_5,		15,		5,	"5" },
	{ KEY_6,		17,		5,	"6" },
	{ KEY_7,		19,		5,	"7" },
	{ KEY_8,		21,		5,	"8" },
	{ KEY_9,		23,		5,	"9" },
	{ KEY_0,		25,		5,	"0" },
	{ 0,			27,		5,	"\7" },
	{ 0,			29,		5,	"\7" },
	{ 0,			31,		5,	"\7" },
	{ KEY_BS,		33,		5,	"BS" },

	{ KEY_TAB,		3,		6,	"TAB" },
	{ KEY_Q,		8,		6,	"Q" },
	{ KEY_W,		10,		6,	"W" },
	{ KEY_E,		12,		6,	"E" },
	{ KEY_R,		14,		6,	"R" },
	{ KEY_T,		16,		6,	"T" },
	{ KEY_Y,		18,		6,	"Y" },
	{ KEY_U,		20,		6,	"U" },
	{ KEY_I,		22,		6,	"I" },
	{ KEY_O,		24,		6,	"O" },
	{ KEY_P,		26,		6,	"P" },
	{ 0,			28,		6,	"\7" },
	{ 0,			30,		6,	"\7" },
	{ KEY_RET,		33,		6,	"RET" },
	
	{ KEY_CTRL,		3,		7,	"CTRL" },
	{ KEY_A,		9,		7,	"A" },
	{ KEY_S,		11,		7,	"S" },
	{ KEY_D,		13,		7,	"D" },
	{ KEY_F,		15,		7,	"F" },
	{ KEY_G,		17,		7,	"G" },
	{ KEY_H,		19,		7,	"H" },
	{ KEY_J,		21,		7,	"J" },
	{ KEY_K,		23,		7,	"K" },
	{ KEY_L,		25,		7,	"L" },
	{ 0,			27,		7,	"\7" },
	{ 0,			29,		7,	"\7" },
	{ 0,			31,		7,	"\7" },
	
	{ KEY_SHIFT,	3,		8,	"SHIFT" },
	{ KEY_Z,		10,		8,	"Z" },
	{ KEY_X,		12,		8,	"X" },
	{ KEY_C,		14,		8,	"C" },
	{ KEY_V,		16,		8,	"V" },
	{ KEY_B,		18,		8,	"B" },
	{ KEY_N,		20,		8,	"N" },
	{ KEY_M,		22,		8,	"M" },
	{ 0,			24,		8,	"\7" },
	{ 0,			26,		8,	"\7" },
	{ 0,			28,		8,	"\7" },
	{ 0,			30,		8,	"\7" },
	{ KEY_SHIFT,	32,		8,	"SHIFT" },

	{ KEY_CAPS,		6,		9,	"CAPS" },
	{ KEY_GRAPH,	11,		9,	"GRPH" },
	{ KEY_SPACE,	19,		9,	"SPACE" },
	{ KEY_CODE,		28,		9,	"CODE" },

	// Arrow key
	{ KEY_UP,		16,		12,	"^" },
	{ KEY_LEFT,		14,		13,	"<" },
	{ KEY_RIGHT,	18,		13,	">" },
	{ KEY_DOWN,		16,		14,	"v" },

	// Numeric keypad
	{ KEY_NUM_7,	3,		12,	"7" },
	{ KEY_NUM_8,	5,		12,	"8" },
	{ KEY_NUM_9,	7,		12,	"9" },
	{ KEY_NUM_MIN,	9,		12,	"-" },

	{ KEY_NUM_4,	3,		13,	"4" },
	{ KEY_NUM_5,	5,		13,	"5" },
	{ KEY_NUM_6,	7,		13,	"6" },
	{ KEY_NUM_DIV,	9,		13,	"/" },

	{ KEY_NUM_1,	3,		14,	"1" },
	{ KEY_NUM_2,	5,		14,	"2" },
	{ KEY_NUM_3,	7,		14,	"3" },
	{ KEY_NUM_MUL,	9,		14,	"*" },

	{ KEY_NUM_0,	3,		15,	"0" },
	{ KEY_NUM_COM,	5,		15,	"," },
	{ KEY_NUM_DOT,	7,		15,	"." },
	{ KEY_NUM_ADD,	9,		15,	"+" },
};

// Animation characters
const u8 chrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// MEMORY DATA
//=============================================================================

// Log
u8 g_LogX, g_LogY;

u8 keyPressed[numberof(keys)];


//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
/// 
void InputEvent(u8 dev, u8 in, u8 evt)
{
	if(g_LogY >= 24)
	{
		for(i8 i = 18; i < 24; i++) // clear
		{
			Print_SetPosition(0, i);
			Print_DrawCharX(' ', 40);
		}
		g_LogX = 1;
		g_LogY = 18;
	}

	Print_SetPosition(g_LogX, g_LogY);
	
	Print_DrawChar('D');
	Print_DrawInt(dev);
	Print_DrawChar('.');
	switch(in)
	{
	case IPM_INPUT_STICK:    Print_DrawText("S"); break;
	case IPM_INPUT_BUTTON_A: Print_DrawText("A"); break;
	case IPM_INPUT_BUTTON_B: Print_DrawText("B"); break;
	}
	Print_DrawChar(':');
	Print_DrawText(IPM_GetEventName(evt));
	Print_DrawChar(' ');
	
	g_LogX = g_PrintData.CursorX;
	g_LogY = g_PrintData.CursorY;
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
/// Program entry point
void main()
{

	// Initialize screen
	VDP_SetMode(VDP_MODE_SCREEN0);
	VDP_ClearVRAM();
	VDP_EnableVBlank(TRUE);

	// Initialize Input manager
	IPM_Initialize(NULL);
	IPM_SetTimer(4, 6);
	IPM_RegisterEvent(IPM_DEVICE_ANY, IPM_INPUT_ANY, IPM_EVENT_CLICK, InputEvent);
	IPM_RegisterEvent(IPM_DEVICE_ANY, IPM_INPUT_ANY, IPM_EVENT_HOLD, InputEvent);
	IPM_RegisterEvent(IPM_DEVICE_ANY, IPM_INPUT_ANY, IPM_EVENT_DOUBLE_CLICK, InputEvent);
	IPM_RegisterEvent(IPM_DEVICE_ANY, IPM_INPUT_ANY, IPM_EVENT_DOUBLE_CLICK_HOLD, InputEvent);

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample6, 1);
	Print_SetColor(COLOR_WHITE, COLOR_BLACK);

	Print_DrawBox(0, 0, 40, 3);
	Print_SetPosition(2, 1);
	Print_DrawText(MSX_GL "  KEYBOARD SAMPLE");
	
	for(u8 i = 0; i < numberof(keys); i++)
	{
		Print_SetPosition(keys[i].posX-1, keys[i].posY);
		Print_DrawText(keys[i].text);
	}

	Print_DrawBox(0, 3, 40, 8);
	Print_DrawBox(0, 11, 11, 6);
	Print_DrawBox(11, 11, 9, 5);

	Print_SetPosition(21, 12);
	Print_DrawText("Raw:");

	Print_SetPosition(0, 17);
	Print_DrawText("Log:");
	g_LogX = 1;
	g_LogY = 18;

	i8 i;
	for(i = 0; i < numberof(keys); i++)
		keyPressed[i] = 1;

	u8 count = 0;
	u8 loop = 1;
	c8 start, end;
	while(loop)
	{
		IPM_Update();
		
		for(i = 0; i < numberof(keys); i++)
		{
			u8 pressed = Keyboard_IsKeyPressed(keys[i].code);
			if(keyPressed[i] != pressed)
			{
				keyPressed[i] = pressed;
				start = end = ' ';
				if(pressed)
				{
					start = '[';
					end = ']';
				}
				Print_SetPosition(keys[i].posX-2, keys[i].posY);
				Print_DrawChar(start);
				Print_DrawText(keys[i].text);
				Print_DrawChar(end);
			}
		}

		for(i = 0; i < 11; i++)
		{
			if((i % 4) == 0)
				Print_SetPosition(22, 13 + (i / 4));
			Print_DrawHex8(Keyboard_Read(i));
			Print_DrawChar(' ');
		}
		
		if(Keyboard_IsKeyPressed(KEY_ESC) && Keyboard_IsKeyPressed(KEY_DEL))
		{
			loop = 0;
		}

		Print_SetPosition(37, 1);
		u8 chr = count++ & 0x03;
		Print_DrawChar(chrAnim[chr]);
		
		Halt();
	}

	Bios_Exit(0);
}