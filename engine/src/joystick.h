// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ██▀▄ ██▀▄ ██ █ ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄██▄ ██ █ ██▀  ▀█▄█ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘            ▀▀  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// User input handler using direct access to ports
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"
#include "input.h"

//=============================================================================
// Group: Joystick
// Direct access to joystick
//=============================================================================

#define JOY_PORT_1					0b00001111
#define JOY_PORT_2					0b01001111

#define JOY_INPUT_DIR_NONE			0
#define JOY_INPUT_DIR_UP			(1 << 0)
#define JOY_INPUT_DIR_DOWN			(1 << 1)
#define JOY_INPUT_DIR_LEFT			(1 << 2)
#define JOY_INPUT_DIR_RIGHT			(1 << 3)
#define JOY_INPUT_DIR_UP_RIGHT		(JOY_INPUT_DIR_UP + JOY_INPUT_DIR_RIGHT)
#define JOY_INPUT_DIR_UP_LEFT		(JOY_INPUT_DIR_UP + JOY_INPUT_DIR_LEFT)
#define JOY_INPUT_DIR_DOWN_RIGHT	(JOY_INPUT_DIR_DOWN + JOY_INPUT_DIR_RIGHT)
#define JOY_INPUT_DIR_DOWN_LEFT		(JOY_INPUT_DIR_DOWN + JOY_INPUT_DIR_LEFT)
#define JOY_INPUT_DIR_MASK			0x0F
#define JOY_INPUT_DIR_UNCHANGED		0xFF
#define JOY_INPUT_TRIGGER_A			(1 << 4)
#define JOY_INPUT_TRIGGER_B			(1 << 5)
#define JOY_INPUT_TRIGGER_RUN		(JOY_INPUT_DIR_RIGHT + JOY_INPUT_DIR_LEFT) // FM-Town input method
#define JOY_INPUT_TRIGGER_SELECT	(JOY_INPUT_DIR_UP + JOY_INPUT_DIR_DOWN) // FM-Town input method

#define IS_JOY_PRESSED(stat, input)	(((stat) & (input)) == 0)
#define IS_JOY_RELEASED(stat, input) (((stat) & (input)) != 0)

#define JOY_GET_DIR(in)				(~(in) & JOY_INPUT_DIR_MASK)
#define JOY_GET_TRIG1(in)			(((in) & JOY_INPUT_TRIGGER_A) == 0)
#define JOY_GET_TRIG2(in)			(((in) & JOY_INPUT_TRIGGER_B) == 0)
#define JOY_GET_A(in)				(((in) & JOY_INPUT_TRIGGER_A) == 0)
#define JOY_GET_B(in)				(((in) & JOY_INPUT_TRIGGER_B) == 0)
#define JOY_GET_RUN(in)				(((in) & JOY_INPUT_TRIGGER_RUN) == 0)
#define JOY_GET_SELECT(in)			(((in) & JOY_INPUT_TRIGGER_SELECT) == 0)

// Function: Joystick_Read
// Gets current joystick information
//
// Parameters:
//   port - JOY_PORT_1 or JOY_PORT_2
//
// Return:
//   Joystick state (bit=0: pressed)
// : xxBARLDU
// :   │││││└─ Up
// :   ││││└── Down
// :   │││└─── Left
// :   ││└──── Right
// :   │└───── Trigger A
// :   └────── Trigger B
u8 Joystick_Read(u8 port) __FASTCALL __PRESERVES(b, c, d, e, h, iyl, iyh);

#if (INPUT_JOY_UPDATE)

extern u8 g_JoyStats[2];
extern u8 g_JoyStatsPrev[2];

// Function: Joystick_Update
// Updates both joystick stats at once and stores the result
// Only available when INPUT_JOY_UPDATE is TRUE
void Joystick_Update();

// Function: Joystick_GetDirection
// Gets current direction of the given joystick
// If INPUT_JOY_UPDATE is TRUE, this function uses data retrieved by Joystick_Update().
// Otherwise, this function reads I/O data.
//
// Parameters:
//   port - JOY_PORT_1 or JOY_PORT_2
//
// Return:
//   One or two of those defines:
// : JOY_INPUT_DIR_NONE
// : JOY_INPUT_DIR_UP
// : JOY_INPUT_DIR_DOWN
// : JOY_INPUT_DIR_LEFT
// : JOY_INPUT_DIR_RIGHT
inline u8 Joystick_GetDirection(u8 port) { return g_JoyStats[port >> 6] & JOY_INPUT_DIR_MASK; }

// Function: Joystick_GetDirectionChange
// Gets current direction of the given joystick if different from previous one
// Only available if INPUT_JOY_UPDATE is TRUE.
//
// Parameters:
//   port - JOY_PORT_1 or JOY_PORT_2
//
// Return:
//   One or two of those defines:
// : JOY_INPUT_DIR_NONE
// : JOY_INPUT_DIR_UP
// : JOY_INPUT_DIR_DOWN
// : JOY_INPUT_DIR_LEFT
// : JOY_INPUT_DIR_RIGHT
// : JOY_INPUT_DIR_UNCHANGED
inline u8 Joystick_GetDirectionChange(u8 port)
{
	u8 in = g_JoyStats[port >> 6]  & JOY_INPUT_DIR_MASK;
	u8 prev = g_JoyStatsPrev[port >> 6]  & JOY_INPUT_DIR_MASK;
	if (in == prev)
		in = JOY_INPUT_DIR_UNCHANGED;
	return in;
}

// Function: Joystick_IsButtonPressed
// Gets current trigger status of the given joystick
// If INPUT_JOY_UPDATE is TRUE, this function uses data retrieved by Joystick_Update().
// Otherwise, this function read I/O data.
//
// Parameters:
//   port - JOY_PORT_1 or JOY_PORT_2
//   trigger - JOY_INPUT_TRIGGER_A or JOY_INPUT_TRIGGER_B
//
// Return:
//   TRUE if given button is pressed
inline bool Joystick_IsButtonPressed(u8 port, u8 trigger) { return ((g_JoyStats[port >> 6] & trigger) != 0); }

// Function: Joystick_IsButtonPushed
// Gets current trigger status of the given joystick
// Only available if INPUT_JOY_UPDATE is TRUE.
//
// Parameters:
//   port - JOY_PORT_1 or JOY_PORT_2
//   trigger - JOY_INPUT_TRIGGER_A or JOY_INPUT_TRIGGER_B
//
// Return:
//   TRUE if given button is pressed
inline bool Joystick_IsButtonPushed(u8 port, u8 trigger)
{
	u8 in = g_JoyStats[port >> 6];
	u8 prev = g_JoyStatsPrev[port >> 6];
	return ((in & trigger) != 0) && ((prev & trigger) == 0);
}

#else // (!INPUT_JOY_UPDATE)

// Gets current direction of the given joystick
inline u8 Joystick_GetDirection(u8 port)
{
	u8 in = ~Joystick_Read(port);
	return (in & JOY_INPUT_DIR_MASK);
}

// Gets current trigger status of the given joystick (0: released; 1: pressed)
inline bool Joystick_IsButtonPressed(u8 port, u8 trigger)
{
	u8 in = Joystick_Read(port);
	return ((in & trigger) == 0);
}

#endif // (INPUT_JOY_UPDATE)