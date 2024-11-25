// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Joystick sample program
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "input.h"
#include "device/msx-hid.h"
#include "device/joymega.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"

// Sign-of-life animation
const u8 g_LifeAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// MEMORY DATA
//=============================================================================

u16 g_PrevID[2] = { 0xFF, 0xFF };

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// 
void Update_MSXHID(u8 i)
{
	u16 id = HID_Detect(i == 0 ? INPUT_PORT1 : INPUT_PORT2);
	if (id == g_PrevID[i])
		return;

	g_PrevID[i] = id;

	Print_SetPosition(8 + i * 20, 5);
	Print_DrawHex16(id);

	const c8* dev = "Unknow       ";
	switch(id)
	{
	case HID_DEVICE_JOYSTICK:	dev = "None/Joystick"; break;
	case HID_DEVICE_MOUSE:		dev = "Mouse        "; break;
	case HID_DEVICE_TRACKPAD:	dev = "Trackpad     "; break;
	case HID_DEVICE_TOUCHPAD:	dev = "Touchpad     "; break;
	case HID_DEVICE_VAUSPADDLE:	dev = "Vaus Paddle  "; break;
	case HID_DEVICE_JOYMEGA:	dev = "JoyMega      "; break;
	case HID_DEVICE_NINJATAP:	dev = "NinjaTap     "; break;
	case HID_DEVICE_JSX_A0_B1:	dev = "JSX 0A/1B    "; break;
	case HID_DEVICE_JSX_A2_B1:	dev = "JSX 2A/1B    "; break;
	case HID_DEVICE_JSX_A6_B2:	dev = "JSX 6A/2B    "; break;
	}

	Print_SetPosition(2 + i * 20, 6);
	Print_DrawText(dev);
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize screen
	VDP_SetMode(VDP_MODE_SCREEN0);
	VDP_ClearVRAM();
	VDP_EnableVBlank(TRUE);

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample6, 1); // Initialize font

	// Draw static text
	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL " JoyMega Sample");
	Print_DrawLineH(0, 1, 40);
	Print_DrawLineV(19, 2, 20);
	Print_DrawLineH(0, 22, 40);

	loop(i, 2)
	{
		Print_DrawTextAt(8 + i * 20,  3, (i == 0) ? "Port 1" : "Port 2");

		Print_DrawTextAt(1 + i * 20,  5, "HID:");

		Print_DrawTextAt(1 + i * 20,  8, "Data:");
		Print_DrawTextAt(1 + i * 20,  9, "\x07" "Up:");
		Print_DrawTextAt(1 + i * 20, 10, "\x07" "Down:");
		Print_DrawTextAt(1 + i * 20, 11, "\x07" "Left:");
		Print_DrawTextAt(1 + i * 20, 12, "\x07" "Right:");
		Print_DrawTextAt(1 + i * 20, 13, "\x07" "A:");
		Print_DrawTextAt(1 + i * 20, 14, "\x07" "B:");
		Print_DrawTextAt(1 + i * 20, 15, "\x07" "C:");
		Print_DrawTextAt(1 + i * 20, 16, "\x07" "X:");
		Print_DrawTextAt(1 + i * 20, 17, "\x07" "Y:");
		Print_DrawTextAt(1 + i * 20, 18, "\x07" "Z:");
		Print_DrawTextAt(1 + i * 20, 19, "\x07" "Start:");
		Print_DrawTextAt(1 + i * 20, 20, "\x07" "Mode:");
	}

	u8 frameCount = 0;
	bool bContinue = TRUE;
	while(bContinue)
	{
		Halt();

		// Sign of life
		Print_SetPosition(39, 0);
		Print_DrawChar(g_LifeAnim[frameCount++ % 4]);

		loop(i, 2)
		{
			u16 jm = JoyMega_Read6(i == 0 ? INPUT_PORT1 : INPUT_PORT2);

			Print_SetPosition(8 + i * 20, 8);
			Print_DrawHex16(jm);
			Print_DrawCharAt(8 + i * 20,  9, JoyMega_IsPressedUp(jm) ? 0x88 : 0x07);
			Print_DrawCharAt(8 + i * 20, 10, JoyMega_IsPressedDown(jm) ? 0x89 : 0x07);
			Print_DrawCharAt(8 + i * 20, 11, JoyMega_IsPressedLeft(jm) ? 0x8B : 0x07);
			Print_DrawCharAt(8 + i * 20, 12, JoyMega_IsPressedRight(jm) ? 0x8A : 0x07);
			Print_DrawCharAt(8 + i * 20, 13, JoyMega_IsPressedA(jm) ? 0x0C : 0x0B);
			Print_DrawCharAt(8 + i * 20, 14, JoyMega_IsPressedB(jm) ? 0x0C : 0x0B);
			Print_DrawCharAt(8 + i * 20, 15, JoyMega_IsPressedC(jm) ? 0x0C : 0x0B);
			Print_DrawCharAt(8 + i * 20, 16, JoyMega_IsPressedX(jm) ? 0x0C : 0x0B);
			Print_DrawCharAt(8 + i * 20, 17, JoyMega_IsPressedY(jm) ? 0x0C : 0x0B);
			Print_DrawCharAt(8 + i * 20, 18, JoyMega_IsPressedZ(jm) ? 0x0C : 0x0B);
			Print_DrawCharAt(8 + i * 20, 19, JoyMega_IsPressedStart(jm) ? 0x0C : 0x0B);
			Print_DrawCharAt(8 + i * 20, 20, JoyMega_IsPressedMode(jm) ? 0x0C : 0x0B);

			Update_MSXHID(i); // Pin 8 will stay HIGH long enough to don't disturb next call to JoyMega_Read6
		}
	}

	Bios_Exit(0);
}