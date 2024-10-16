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
	Print_DrawText(MSX_GL " MOUSE SAMPLE");
	Print_DrawLineH(0, 1, 32);
	Print_SetPosition(0, 3);
	Print_DrawText("     Port 1    Port 2\n\nDev\n\n\nX :\nY :\ndX:\ndY:\nLB:\nRB:");
	Print_DrawLineH(0, 1, 22);
	Print_SetPosition(0, 23);
	Print_DrawText("D:Detect");

	Detect();

	// Init sprite
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_8);
	// Load and setup sprite
	VDP_LoadSpritePattern(g_Cursor_MGL1, 0, 32);
	VDP_SetSpriteSM1(0, 0, 0, 0, COLOR_LIGHT_BLUE);
	VDP_SetSpriteSM1(1, 0, 0, 0, COLOR_LIGHT_RED);
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
			Mouse_Read((i == 0) ? MOUSE_PORT_1 : MOUSE_PORT_2, &g_MouseState[i]);

			if(g_DeviceID[i] != INPUT_TYPE_MOUSE)
				continue;

			i8 dX = REDUCE(Mouse_GetOffsetX(&g_MouseState[i]));
			i8 dY = REDUCE(Mouse_GetOffsetY(&g_MouseState[i]));
			g_MouseX[i] += dX;
			g_MouseY[i] += dY;
			VDP_SetSpritePosition(i, g_MouseX[i], g_MouseY[i]-1);
		
			c8 btn1 = '-';
			if(Mouse_IsButtonClick(&g_MouseState[i], MOUSE_BOUTON_1))
			{
				g_MouseCur[i]++;
				g_MouseCur[i] &= 0x1F;
				VDP_SetSpritePattern(i, g_MouseCur[i]);
				btn1 = 'X';
			}
			else if(Mouse_IsButtonPress(&g_MouseState[i], MOUSE_BOUTON_1))
				btn1 = 'O';

			c8 btn2 = '-';
			if(Mouse_IsButtonClick(&g_MouseState[i], MOUSE_BOUTON_2))
			{
				g_MouseCur[i]--;
				g_MouseCur[i] &= 0x1F;
				VDP_SetSpritePattern(i, g_MouseCur[i]);
				btn2 = 'X';
			}
			else if(Mouse_IsButtonPress(&g_MouseState[i], MOUSE_BOUTON_2))
				btn2 = 'O';

			u8 px = 5 + i * 10;

			Print_SetPosition(px, 8);
			Print_DrawInt(g_MouseX[i]);
			Print_DrawText("  ");
			Print_SetPosition(px, 9);
			Print_DrawInt(g_MouseY[i]);
			Print_DrawText("  ");

			Print_SetPosition(px, 10);
			Print_DrawInt(dX);
			Print_DrawText("  ");
			Print_SetPosition(px, 11);
			Print_DrawInt(dY);
			Print_DrawText("  ");

			Print_SetPosition(px, 12);
			Print_DrawChar(btn1);
			Print_SetPosition(px, 13);
			Print_DrawChar(btn2);
		}
		
		if(Keyboard_IsKeyPressed(KEY_D))
			Detect();
	}
}