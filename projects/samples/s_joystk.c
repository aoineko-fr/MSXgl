// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Joystick sample program
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "input.h"
#include "input_manager.h"

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

//=============================================================================
// MEMORY DATA
//=============================================================================

// Log
u8 g_LogX, g_LogY;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
/// 
void JoystickEvent(u8 joy, u8 in, u8 evt)
{
	if(g_LogY >= 24)
	{
		for(i8 i = 17; i < 24; i++) // clear
		{
			Print_SetPosition(1, i);
			Print_DrawCharX(' ', 40);
		}
		g_LogX = 1;
		g_LogY = 17;
	}

	Print_SetPosition(g_LogX, g_LogY);
	Print_DrawChar('J');
	Print_DrawInt(joy);
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
	IPM_RegisterEvent(IPM_DEVICE_JOYSTICK_1, IPM_INPUT_ANY, IPM_EVENT_CLICK, JoystickEvent);
	IPM_RegisterEvent(IPM_DEVICE_JOYSTICK_1, IPM_INPUT_ANY, IPM_EVENT_HOLD, JoystickEvent);
	IPM_RegisterEvent(IPM_DEVICE_JOYSTICK_1, IPM_INPUT_ANY, IPM_EVENT_DOUBLE_CLICK, JoystickEvent);
	IPM_RegisterEvent(IPM_DEVICE_JOYSTICK_1, IPM_INPUT_ANY, IPM_EVENT_DOUBLE_CLICK_HOLD, JoystickEvent);

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample6, 1); // Initialize font

	// Draw static text
	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL " JOYSTICK SAMPLE");
	Print_SetPosition(0, 1);
	Print_DrawCharX(0x17, 40);

	Print_SetPosition(0, 4);
	Print_DrawCharX(0x17, 40);
	Print_SetPosition(0, 5);
	Print_DrawText("Raw access:");

	Print_SetPosition(0, 10);
	Print_DrawCharX(0x17, 40);
	Print_SetPosition(0, 11);
	Print_DrawText("JS Manager:");

	Print_SetPosition(0, 15);
	Print_DrawCharX(0x17, 40);
	Print_SetPosition(0, 16);
	Print_DrawText("Events log:");
	
	for(i8 joy = 0; joy < 2; joy++)
	{
		// ID
		Print_SetPosition(21 * joy, 3);
		Print_DrawText("Port#");
		Print_DrawInt(joy);

		// Raw access
		// Status
		Print_SetPosition(21 * joy, 6);
		Print_DrawText("\7""Status");
		// Direction
		Print_SetPosition(21 * joy, 7);
		Print_DrawText("\7""Direction");
		// Trigger A
		Print_SetPosition(21 * joy, 8);
		Print_DrawText("\7""Trig A");
		// Trigger B
		Print_SetPosition(21 * joy, 9);
		Print_DrawText("\7""Trig B");

		// Joystick Manager
		// Stick
		Print_SetPosition(21 * joy, 12);
		Print_DrawText("\7""Stick");
		// Button A
		Print_SetPosition(21 * joy, 13);
		Print_DrawText("\7""Button A");
		// Button B
		Print_SetPosition(21 * joy, 14);
		Print_DrawText("\7""Button B");
	}

	g_LogX = 1;
	g_LogY = 17;
	u8 count = 0;
	
	// Main loop
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();
		count++;
		#if (INPUT_JOY_UPDATE)
		Joystick_Update();
		#endif

		Print_SetPosition(39, 0);
		Print_DrawChar(0xB0 + ((count) & 0x07));

		IPM_Update();
		
		for(i8 joy = 0; joy < 2; joy++)
		{
			// Raw access
			
			// Status
			Print_SetPosition(12 + (21 * joy), 6);
			Print_DrawHex8(Joystick_Read((joy == 0) ? JOY_PORT_1 : JOY_PORT_2));
			// Direction
			Print_SetPosition(12 + (21 * joy), 7);
			u8 dir = Joystick_GetDirection((joy == 0) ? JOY_PORT_1 : JOY_PORT_2);
			Print_DrawInt(dir);
			Print_DrawChar(' ');
			// Trigger A
			Print_SetPosition(12 + (21 * joy), 8);
			Print_DrawInt(Joystick_IsButtonPressed((joy == 0) ? JOY_PORT_1 : JOY_PORT_2, JOY_INPUT_TRIGGER_A));
			// Trigger B
			Print_SetPosition(12 + (21 * joy), 9);
			Print_DrawInt(Joystick_IsButtonPressed((joy == 0) ? JOY_PORT_1 : JOY_PORT_2, JOY_INPUT_TRIGGER_B));

			// Joystick
			Print_SetPosition(16 + (21 * joy), 7);
			Print_DrawText("   ");		
			Print_SetPosition(16 + (21 * joy), 8);
			Print_DrawText(" O ");		
			Print_SetPosition(16 + (21 * joy), 9);
			Print_DrawText("   ");
			switch(dir)
			{
			case JOY_INPUT_DIR_UP:
				Print_SetPosition(17 + (21 * joy), 7);
				Print_DrawChar('|');
				break;
			case JOY_INPUT_DIR_DOWN:
				Print_SetPosition(17 + (21 * joy), 9);
				Print_DrawChar('|');
				break;
			case JOY_INPUT_DIR_LEFT:
				Print_SetPosition(16 + (21 * joy), 8);
				Print_DrawChar('-');
				break;
			case JOY_INPUT_DIR_RIGHT:
				Print_SetPosition(18 + (21 * joy), 8);
				Print_DrawChar('-');
				break;
			case JOY_INPUT_DIR_UP_RIGHT:
				Print_SetPosition(18 + (21 * joy), 7);
				Print_DrawChar('/');
				break;
			case JOY_INPUT_DIR_UP_LEFT:
				Print_SetPosition(16 + (21 * joy), 7);
				Print_DrawChar('\\');
				break;
			case JOY_INPUT_DIR_DOWN_RIGHT:
				Print_SetPosition(18 + (21 * joy), 9);
				Print_DrawChar('\\');
				break;
			case JOY_INPUT_DIR_DOWN_LEFT:
				Print_SetPosition(16 + (21 * joy), 9);
				Print_DrawChar('/');
				break;
			}

			// Stick
			Print_SetPosition(12 + (21 * joy), 12);
			Print_DrawHex8(IPM_GetStickDirection(joy));
			Print_SetPosition(16 + (21 * joy), 12);
			u8 timer = IPM_GetInputTimer(joy, IPM_INPUT_STICK);
			Print_DrawInt(timer);
			if(timer < 100) Print_DrawChar(' ');
			if(timer < 10) Print_DrawChar(' ');
			// Button A
			Print_SetPosition(12 + (21 * joy), 13);
			switch(IPM_GetInputState(joy, IPM_INPUT_BUTTON_A) & IPM_STATE_PRESSMASK)
			{
			case IPM_STATE_OFF:     Print_DrawText("OFF"); break;
			case IPM_STATE_PRESS:   Print_DrawText("PRE"); break;
			case IPM_STATE_ON:      Print_DrawText("ON "); break;
			case IPM_STATE_RELEASE: Print_DrawText("REL"); break;
			}
			Print_SetPosition(16 + (21 * joy), 13);
			timer = IPM_GetInputTimer(joy, IPM_INPUT_BUTTON_A);
			Print_DrawInt(timer);
			if(timer < 100) Print_DrawChar(' ');
			if(timer < 10) Print_DrawChar(' ');
			// Button B
			Print_SetPosition(12 + (21 * joy), 14);
			Print_DrawText(IPM_GetInputState(joy, IPM_INPUT_BUTTON_B) ? "ON " : "OFF");
			Print_SetPosition(16 + (21 * joy), 14);
			timer = IPM_GetInputTimer(joy, IPM_INPUT_BUTTON_B);
			Print_DrawInt(timer);
			if(timer < 100) Print_DrawChar(' ');
			if(timer < 10) Print_DrawChar(' ');
		}
	}

	Bios_Exit(0);
}