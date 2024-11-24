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
#define JOYMEGA_IN_UP				0x0001
#define JOYMEGA_IN_DOWN				0x0002
#define JOYMEGA_IN_LEFT				0x0004
#define JOYMEGA_IN_RIGHT			0x0008
#define JOYMEGA_IN_B				0x0010
#define JOYMEGA_IN_C				0x0020
#define JOYMEGA_IN_A				0x0040
#define JOYMEGA_IN_START			0x0080
#define JOYMEGA_IN_Z				0x0100
#define JOYMEGA_IN_Y				0x0200
#define JOYMEGA_IN_X				0x0400
#define JOYMEGA_IN_MODE				0x0800

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: JoyMega_Read3
// Read Megadrive 3-button joypad through JoyMega adapter
u8 JoyMega_Read3(enum INPUT_PORT port);

// Function: JoyMega_Read6
// Read Megadrive 6-button joypad through JoyMega adapter
u16 JoyMega_Read6(enum INPUT_PORT port);