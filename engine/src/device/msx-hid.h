// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄  ▄▄▄ ▄▄ ▄     ▄▄ ▄ ▄▄▄▄ ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▀█▄  ▀█▄▀ ▄▄▄ ██▄█  ██  ██ █
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▄▄█▀ ██ █     ██ █ ▄██▄ ██▄▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// User input handler using direct access to ports
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"
#include "input.h"

enum HID_DEVICE
{
	HID_DEVICE_NONE			= 0x3F3F,
	HID_DEVICE_JOYSTICK		= 0x3F3F,
	HID_DEVICE_MOUSE		= 0x3030,
	HID_DEVICE_TRACKPAD		= 0x3838,
	HID_DEVICE_TOUCHPAD		= 0x3B3D,
	HID_DEVICE_VAUSPADDLE	= 0x3F3E,
	HID_DEVICE_JOYMEGA		= 0x3F33,
	HID_DEVICE_NINJATAP		= 0x3F1F,
	HID_DEVICE_JSX_A0_B1	= 0x0F01, // Joypad: 6 boutons
	HID_DEVICE_JSX_A2_B1	= 0x0F09, // Joypad: 2 axies, 6 boutons
	HID_DEVICE_JSX_A6_B2	= 0x0F1A, // Joypad: 6 axies, 12 boutons (Xbox gamepad)
};

// Function: HID_Detect
// Detect device plugged to joystick port
//
// Parameters:
//   port - ID of the port to check. Can be <INPUT_PORT1> or <INPUT_PORT1>.
//
// Returns:
//   16-bit footprint of the device
u16 HID_Detect(enum INPUT_PORT port);
