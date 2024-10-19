// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Mouse handling sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "string.h"
#include "input.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x02\x03\x04\x05"

// #define REDUCE(a)	Math_SignedDiv2(a)
#define REDUCE(a)	(a)

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Font
#include "font/font_mgl_sample8.h"

// Cursors
#include "cursor/cur_mgl1.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_DeviceID[2];
u8 g_MouseX[2] = { 127, 127 };
u8 g_MouseY[2] = { 95, 95 };
u8 g_MouseCur[2] = { 0, 0 };
Mouse_State g_MouseState[2];

//=============================================================================
// MAIN LOOP
//=============================================================================

/*u16 Input_DetectEx(u8 port)
{
__asm
	detect_setup:

		or		a
		jr		nz, detect_port2
		// Port A
		ld		bc, #0x1F0F
		jr		detect_start

	detect_port2:
		// Port B
		ld		bc, #0x6F4F

	detect_start:

		ld		a, #15				// Select R#15
		out		(P_PSG_REGS), a
		ld		a, b
		out		(P_PSG_DATA), a		// Select pin 8 LOW

		ld		a, #14				// Select R#14
		out		(P_PSG_REGS), a
		in		a, (P_PSG_STAT)		// Reads
		and		#0x3F
		ld		l, a

		ld		a, #15				// Select R#15
		out		(P_PSG_REGS), a
		ld		a, c
		out		(P_PSG_DATA), a		// Select pin 8 HIGH

		ld		a, #14				// Select R#14
		out		(P_PSG_REGS), a
		in		a, (P_PSG_STAT)		// Reads
		and		#0x3F
		ld		h, a

__endasm;
}*/


//-----------------------------------------------------------------------------
// Program entry point
void Detect()
{
	for(u8 i = 0; i < 2; ++i)
	{
		u8 px = 5 + i * 10;
		u8 device = Input_Detect((i == 0) ? INPUT_PORT_1 : INPUT_PORT_2);
		Print_SetPosition(px, 5);
		Print_DrawHex8(device);
		const c8* str;
		switch(device)
		{
			case INPUT_TYPE_JOYSTICK:	str = "Joy/None "; break;
			case INPUT_TYPE_MOUSE:		str = "Mouse    "; break;
			case INPUT_TYPE_TRACKBALL:	str = "Trackball"; break;
			case INPUT_TYPE_PADDLE:		str = "Paddle   "; break;
			case INPUT_TYPE_TOUCHPAD:	str = "Touchpad "; break;
			case INPUT_TYPE_LIGHTGUN:	str = "Lightgun "; break;
			default:					str = "Unknow   "; break;
		}
		Print_SetPosition(px, 6);
		Print_DrawText(str);
		g_DeviceID[i] = device;
	}
}

//-----------------------------------------------------------------------------
// Vertical-blank interruption handler
void VDP_InterruptHandler()
{
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize screen
	VDP_SetMode(VDP_MODE_SCREEN1); // Initialize screen mode 1
	VDP_ClearVRAM();
	VDP_SetColor(COLOR_BLACK);

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample8, 1);
	Print_SetColor(COLOR_WHITE, COLOR_BLACK);
	// Draw static text
	Print_DrawText(MSX_GL " PADDLE SAMPLE");
	Print_DrawLineH(0, 1, 32);
	Print_SetPosition(0, 3);
	Print_DrawText("     Port 1    Port 2\n\nDev\n\n\nX:\nBtn:\nDsc:");
	Print_DrawLineH(0, 1, 22);
	Print_SetPosition(0, 23);
	Print_DrawText("D:Detect");

	Detect();

	// Init sprite
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_8);
	// Load and setup sprite
	VDP_LoadSpritePattern(g_Cursor_MGL1, 0, 32);
	VDP_SetSpriteSM1(0, 0, 0, 6, COLOR_LIGHT_BLUE);
	VDP_SetSpriteSM1(1, 0, 0, 6, COLOR_LIGHT_RED);
	VDP_DisableSpritesFrom(2);
	VDP_EnableDisplay(TRUE);

	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();

		Print_SetPosition(31, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);

		for(u8 i = 0; i < 2; ++i)
		{
			// if(g_DeviceID[i] != INPUT_TYPE_PADDLE)
			// 	continue;

		#if (1)
			Paddle_Update();
			u16 padX = Paddle_GetAngle(i);
			bool padB = Paddle_IsButtonPressed(i);
			bool padC = Paddle_IsConnected(i);
		#else
			u16 pad = Paddle_Read(i);
			u16 padX = Paddle_GetAngle2(pad);
			bool padB = Paddle_IsButtonPressed2(pad);
			bool padC = Paddle_IsConnected2(pad);
		#endif

			VDP_SetSpritePosition(i, padX - 3, i * 32 + 96 - 3);
		
			u8 px = 5 + i * 10;

			Print_SetPosition(px, 8);
			Print_DrawInt(padX);
			Print_DrawText("  ");

			Print_SetPosition(px, 9);
			Print_DrawChar(padB ? 'O' : '-');

			Print_SetPosition(px, 10);
			Print_DrawChar(!padC ? 'O' : '-');
		}
		
		if(Keyboard_IsKeyPressed(KEY_D))
		{
			Detect();
		}
	}
}