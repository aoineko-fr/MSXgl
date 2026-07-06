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


//=============================================================================
// DEFINES
//=============================================================================

// Handle interrupt disabling
#if (INPUT_USE_ISR_PROTECTION)
	#define INPUT_DI				di
	#define INPUT_EI				ei
#else
	#define INPUT_DI
	#define INPUT_EI
#endif

#define INPUT_PORT1_LOW				0b00000011
#define INPUT_PORT1_HIGH			0b00010011

#define INPUT_PORT2_LOW				0b01001100
#define INPUT_PORT2_HIGH			0b01101100

enum INPUT_PORT
{
	INPUT_PORT1 = INPUT_PORT1_HIGH,
	INPUT_PORT2 = INPUT_PORT2_HIGH,
};

// Mask
#define INPUT_PIN8_ONLY				0b00110000
#define INPUT_PIN8_MASK				0b11001111
#define INPUT_PORT1_ONLY			0b00010011
#define INPUT_PORT1_MASK			0b11101100
#define INPUT_PORT2_ONLY			0b00101100
#define INPUT_PORT2_MASK			0b11010011


//=============================================================================
// Group: Detect
// General purpose port device detection
//=============================================================================
#if (INPUT_USE_DETECT)

// Value returned from device in idle state (allows approximate detection of the connected device)
enum INPUT_TYPE
{
	INPUT_TYPE_JSX					= 0x0F,
	INPUT_TYPE_JSX_A0_B1			= 0x01, // Joypad: 6 boutons
	INPUT_TYPE_JSX_A2_B1			= 0x09, // Joypad: 2 axies, 6 boutons
	INPUT_TYPE_JSX_A6_B2			= 0x1A, // Joypad: 6 axies, 12 boutons (Xbox gamepad)
	INPUT_TYPE_NINJATAP				= 0x1F,
	INPUT_TYPE_MOUSE				= 0x30,
	INPUT_TYPE_JOYMEGA				= 0x33,
	INPUT_TYPE_TRACKBALL			= 0x38,
	INPUT_TYPE_TOUCHPAD				= 0x3D,
	INPUT_TYPE_PADDLE				= 0x3E, // Arkanoid Vaus Paddle or MSX-Paddle
	INPUT_TYPE_JOYSTICK				= 0x3F,
	INPUT_TYPE_LIGHTGUN_ASCII		= 0x20,
	INPUT_TYPE_LIGHTGUN_GUNSTICK	= 0x3F,
	INPUT_TYPE_LIGHTGUN_PHENIX		= 0x37, // Backward compatible with Gun-Stick
	INPUT_TYPE_UNPLUGGED			= INPUT_TYPE_JOYSTICK,

	// // Unvalidated types
	// INPUT_TYPE_LIGHTGUN				= 0x2F, // Can also be 20 or 3F
	// INPUT_TYPE_MUSIC_PAD			= 0x3C, // Yamaha MMP-01 music pad
	// INPUT_TYPE_IBM_ADAPTER			= 0x3A, // IBM-PC DA15 joystick adapter
	// INPUT_TYPE_ATARI_ADAPTER		= 0x36, // Atari dual-paddle adapter
};

// Function: Input_Detect
// Detect device plugged in General purpose ports
//
// Parameters:
//   port - Port to check (INPUT_PORT1 or INPUT_PORT2)
//
// Return:
//   Device type (INPUT_TYPE_JOYSTICK, INPUT_TYPE_MOUSE, etc.)
u8 Input_Detect(enum INPUT_PORT port);

#endif

//=============================================================================
// Direct access to joystick
//=============================================================================
#if (INPUT_USE_JOYSTICK)
#include "joystick.h"
#endif

//=============================================================================
// Direct access to mouse
//=============================================================================
#if (INPUT_USE_MOUSE)
#include "mouse.h"
#endif

//=============================================================================
// Direct access to keyboard
//=============================================================================
#if (INPUT_USE_KEYBOARD)
#include "keyboard.h"
#endif