// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄            ▄ ▄               
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ▄█▀▄ █ ██ ██▀█ ▄███ ▄▀██ ▄▀██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄█▀  ▀█▄▀  ▀██ ██ █ ▀█▄▄  ▀██ ▀▄██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘             ▀▀             ▀▀ 
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// JoyMega driver module
//
// References:
// - JoyMega (MRC): https://www.msx.org/wiki/JoyMega
// - JoyMega Adapter (FRS): https://frs.badcoffee.info/hardware/joymega-en.html
// - Six Button Control Pad (Sega Retro): https://segaretro.org/Six_Button_Control_Pad_(Mega_Drive)#Technical_information
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"
#include "input.h"

//=============================================================================
// OPTIONS VALIDATION
//=============================================================================

//=============================================================================
// DEFINES
//=============================================================================

//		MSX				JOYMEGA			MEGA
//-------------------------------------------------------
// 1	Up				=				Up
// 2	Down			=				Down
// 3	Left			=				Left
// 4	Right			=				Right
// 5	+5V				=				+5V
// 6	Trigger 1		=				TL (A/B)
// 7	Trigger 2		TR (Start/C)	TH (Select)
// 8	OUT				TH (Select)		Ground
// 9	Ground			Ground			TR (Start/C)

//  F E D C B A 9 8 | 7 6 5 4 3 2 1 0
// –-–-–-–-–-–-–-–--–-–-–-–-–-–-–-–--–
//  x x x x M X Y Z   S A C B R L D U
//          │ │ │ │   │ │ │ │ │ │ │ └── Up
//          │ │ │ │   │ │ │ │ │ │ └──── Down
//          │ │ │ │   │ │ │ │ │ └────── Left
//          │ │ │ │   │ │ │ │ └──────── Right
//          │ │ │ │   │ │ │ └────────── Trigger B
//          │ │ │ │   │ │ └──────────── Trigger C
//          │ │ │ │   │ └────────────── Trigger A
//          │ │ │ │   └──────────────── Trigger Start
//          │ │ │ └──────────────────── Trigger Z
//          │ │ └────────────────────── Trigger Y
//          │ └──────────────────────── Trigger X
//          └────────────────────────── Trigger Mode
#define JOYMEGA_IN_UP				0x01
#define JOYMEGA_IN_DOWN				0x02
#define JOYMEGA_IN_LEFT				0x04
#define JOYMEGA_IN_RIGHT			0x08
#define JOYMEGA_IN_B				0x10
#define JOYMEGA_IN_C				0x20
#define JOYMEGA_IN_A				0x40
#define JOYMEGA_IN_START			0x80
#define JOYMEGA_IN_Z				0x0100
#define JOYMEGA_IN_Y				0x0200
#define JOYMEGA_IN_X				0x0400
#define JOYMEGA_IN_MODE				0x0800

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Read state
//-----------------------------------------------------------------------------

// Function: JoyMega_Read3
// Read Megadrive 3-button joypad through JoyMega adapter
//
// Parameters:
//   port - The port to read (can be <INPUT_PORT1> or <INPUT_PORT2>)
//
// Return:
//   State of the buttons (0: pressed, 1: released)
// >  7 6 5 4 3 2 1 0
// > –-–-–-–-–-–-–-–--
// >  S A C B R L D U
// >  │ │ │ │ │ │ │ └── Up
// >  │ │ │ │ │ │ └──── Down
// >  │ │ │ │ │ └────── Left
// >  │ │ │ │ └──────── Right
// >  │ │ │ └────────── Trigger B
// >  │ │ └──────────── Trigger C
// >  │ └────────────── Trigger A
// >  └──────────────── Trigger Start
u8 JoyMega_Read3(enum INPUT_PORT port);

// Function: JoyMega_Read6
// Read Megadrive 6-button joypad through JoyMega adapter
//
// Parameters:
//   port - The port to read (can be <INPUT_PORT1> or <INPUT_PORT2>)
//
// Return:
//   State of the buttons (0: pressed, 1: released)
// >  F E D C B A 9 8 | 7 6 5 4 3 2 1 0
// > –-–-–-–-–-–-–-–--–-–-–-–-–-–-–-–--–
// >  x x x x M X Y Z   S A C B R L D U
// >          │ │ │ │   │ │ │ │ │ │ │ └── Up
// >          │ │ │ │   │ │ │ │ │ │ └──── Down
// >          │ │ │ │   │ │ │ │ │ └────── Left
// >          │ │ │ │   │ │ │ │ └──────── Right
// >          │ │ │ │   │ │ │ └────────── Trigger B
// >          │ │ │ │   │ │ └──────────── Trigger C
// >          │ │ │ │   │ └────────────── Trigger A
// >          │ │ │ │   └──────────────── Trigger Start
// >          │ │ │ └──────────────────── Trigger Z
// >          │ │ └────────────────────── Trigger Y
// >          │ └──────────────────────── Trigger X
// >          └────────────────────────── Trigger Mode
u16 JoyMega_Read6(enum INPUT_PORT port);

//-----------------------------------------------------------------------------
// Group: Check state
//-----------------------------------------------------------------------------

// Function: JoyMega_IsPressedUp
// Check if 'up' button is pressed
//
// Parameters:
//   state - State of the joystick returned by <JoyMega_Read3> or <JoyMega_Read6> (lower part)
//
// Return:
//   TRUE if the button is pressed
inline bool JoyMega_IsPressedUp(u8 state) { return ((state & JOYMEGA_IN_UP) == 0); }

// Function: JoyMega_IsPressedDown
// Check if 'down' button is pressed
//
// Parameters:
//   state - State of the joystick returned by <JoyMega_Read3> or <JoyMega_Read6> (lower part)
//
// Return:
//   TRUE if the button is pressed
inline bool JoyMega_IsPressedDown(u8 state) { return ((state & JOYMEGA_IN_DOWN) == 0); }

// Function: JoyMega_IsPressedLeft
// Check if 'left' button is pressed
//
// Parameters:
//   state - State of the joystick returned by <JoyMega_Read3> or <JoyMega_Read6> (lower part)
//
// Return:
//   TRUE if the button is pressed
inline bool JoyMega_IsPressedLeft(u8 state) { return ((state & JOYMEGA_IN_LEFT) == 0); }

// Function: JoyMega_IsPressedRight
// Check if 'right' button is pressed
//
// Parameters:
//   state - State of the joystick returned by <JoyMega_Read3> or <JoyMega_Read6> (lower part)
//
// Return:
//   TRUE if the button is pressed
inline bool JoyMega_IsPressedRight(u8 state) { return ((state & JOYMEGA_IN_RIGHT) == 0); }

// Function: JoyMega_IsPressedA
// Check if 'A' button is pressed
//
// Parameters:
//   state - State of the joystick returned by <JoyMega_Read3> or <JoyMega_Read6> (lower part)
//
// Return:
//   TRUE if the button is pressed
inline bool JoyMega_IsPressedA(u8 state) { return ((state & JOYMEGA_IN_A) == 0); }

// Function: JoyMega_IsPressedB
// Check if 'B' button is pressed
//
// Parameters:
//   state - State of the joystick returned by <JoyMega_Read3> or <JoyMega_Read6> (lower part)
//
// Return:
//   TRUE if the button is pressed
inline bool JoyMega_IsPressedB(u8 state) { return ((state & JOYMEGA_IN_B) == 0); }

// Function: JoyMega_IsPressedC
// Check if 'C' button is pressed
//
// Parameters:
//   state - State of the joystick returned by <JoyMega_Read3> or <JoyMega_Read6> (lower part)
//
// Return:
//   TRUE if the button is pressed
inline bool JoyMega_IsPressedC(u8 state) { return ((state & JOYMEGA_IN_C) == 0); }

// Function: JoyMega_IsPressedStart
// Check if 'start' button is pressed
//
// Parameters:
//   state - State of the joystick returned by <JoyMega_Read3> or <JoyMega_Read6> (lower part)
//
// Return:
//   TRUE if the button is pressed
inline bool JoyMega_IsPressedStart(u8 state) { return ((state & JOYMEGA_IN_START) == 0); }

// Function: JoyMega_IsPressedX
// Check if 'X' button is pressed
//
// Parameters:
//   state - State of the joystick returned by <JoyMega_Read3> or <JoyMega_Read6>
//
// Return:
//   TRUE if the button is pressed
inline bool JoyMega_IsPressedX(u16 state) { return ((state & JOYMEGA_IN_X) == 0); }

// Function: JoyMega_IsPressedY
// Check if 'Y' button is pressed
//
// Parameters:
//   state - State of the joystick returned by <JoyMega_Read3> or <JoyMega_Read6>
//
// Return:
//   TRUE if the button is pressed
inline bool JoyMega_IsPressedY(u16 state) { return ((state & JOYMEGA_IN_Y) == 0); }

// Function: JoyMega_IsPressedZ
// Check if 'Z' button is pressed
//
// Parameters:
//   state - State of the joystick returned by <JoyMega_Read3> or <JoyMega_Read6>
//
// Return:
//   TRUE if the button is pressed
inline bool JoyMega_IsPressedZ(u16 state) { return ((state & JOYMEGA_IN_Z) == 0); }

// Function: JoyMega_IsPressedMode
// Check if 'mode' button is pressed
//
// Parameters:
//   state - State of the joystick returned by <JoyMega_Read3> or <JoyMega_Read6>
//
// Return:
//   TRUE if the button is pressed
inline bool JoyMega_IsPressedMode(u16 state) { return ((state & JOYMEGA_IN_MODE) == 0); }
