// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄   ▄       ▄▄   ▄▄      ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██   ▄  ▄▀██ ██▄  ██▀    ██   ██ █ ██▀▄
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▄ ██  ▀██ ██ █ ▀█▄    ▀█▄█ ▀█▄█ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘           ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Light gun driver module
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

// ASCII Plus-X Terminator Laser mapping
#define LIGHTGUN_ASCII_TRIGGER		JOY_INPUT_TRIGGER_B
#define LIGHTGUN_ASCII_LIGHT		JOY_INPUT_TRIGGER_A

// MHT Ingenieros Gun-Stick mapping
#define LIGHTGUN_GUNSTICK_TRIGGER	JOY_INPUT_TRIGGER_A		
#define LIGHTGUN_GUNSTICK_LIGHT		JOY_INPUT_DIR_DOWN

// Phenix Light Gun mapping
#define LIGHTGUN_PHENIX_TRIGGER_A	JOY_INPUT_TRIGGER_A
#define LIGHTGUN_PHENIX_TRIGGER_B	JOY_INPUT_TRIGGER_B
#define LIGHTGUN_PHENIX_LIGHT		0x07 // 3-bit RGB color (0b000: Black, 0b111: White)

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Common
//-----------------------------------------------------------------------------

// Function: LightGun_Read
// Read light gun data from the given port.
//
// Parameters:
//   port - Input port to read (INPUT_PORT1 or INPUT_PORT2)
//
// Return:
//   Light gun state (use GetTrigger and GetLight functions to get state information).
inline u8 LightGun_Read(u8 port) { return Joystick_Read(port) & 0x3F; }

//-----------------------------------------------------------------------------
// Group: ASCII Plus-X Terminator Laser
//-----------------------------------------------------------------------------

// Function: LightGun_ASCII_GetTrigger
// Get the trigger status of the ASCII light gun.
//
// Parameters:
//   state - Current state of the light gun (from <LightGun_Read>)
//
// Return:
//   FALSE if the trigger is not pressed.
inline bool LightGun_ASCII_GetTrigger(u8 state) { return (state & LIGHTGUN_ASCII_TRIGGER) == 0; }

// Function: LightGun_ASCII_GetLight
// Get the light status of the ASCII light gun.
//
// Parameters:
//   state - Current state of the light gun (from <LightGun_Read>)
//
// Return:
//   FALSE if the gun don't detect light (not pointing to white image on screen).
inline u8 LightGun_ASCII_GetLight(u8 state) { return (state & LIGHTGUN_ASCII_LIGHT) != 0; }

//-----------------------------------------------------------------------------
// Group: MHT Ingenieros Gun-Stick
//-----------------------------------------------------------------------------

// Function: LightGun_GunStick_GetTrigger
// Get the trigger status of the MHT Ingenieros Gun-Stick light gun.
//
// Parameters:
//   state - Current state of the light gun (from <LightGun_Read>)
//
// Return:
//   FALSE if the trigger is not pressed.
inline bool LightGun_GunStick_GetTrigger(u8 state) { return (state & LIGHTGUN_GUNSTICK_TRIGGER) == 0; }

// Function: LightGun_GunStick_GetLight
// Get the light status of the MHT Ingenieros Gun-Stick light gun.
// Parameters:
//   state - Current state of the light gun (from <LightGun_Read>)
//
// Return:
//   FALSE if the gun don't detect light (not pointing to white image on screen).
inline u8 LightGun_GunStick_GetLight(u8 state) { return (state & LIGHTGUN_GUNSTICK_LIGHT) == 0; }

//-----------------------------------------------------------------------------
// Group: Phenix Light Gun
//-----------------------------------------------------------------------------

// Function: LightGun_Phenix_GetTriggerA
// Get the trigger A status of the Phenix light gun.
//
// Parameters:
//   state - Current state of the light gun (from <LightGun_Read>)
//
// Return:
//   FALSE if the trigger A is not pressed.
inline bool LightGun_Phenix_GetTriggerA(u8 state) { return (state & LIGHTGUN_PHENIX_TRIGGER_A) == 0; }

// Function: LightGun_Phenix_GetTriggerB
// Get the trigger B status of the Phenix light gun.
//
// Parameters:
//   state - Current state of the light gun (from <LightGun_Read>)
//
// Return:
//   FALSE if the trigger B is not pressed.
inline bool LightGun_Phenix_GetTriggerB(u8 state) { return (state & LIGHTGUN_PHENIX_TRIGGER_B) == 0; }

// Function: LightGun_Phenix_GetLight
// Get the light status of the Phenix light gun.
//
// Parameters:
//   state - Current state of the light gun (from <LightGun_Read>)
//
// Return:
//   Detected 3-bit RGB color
// ==== Code ====
// 0b000: Black (no light detected)
// 0b001: Blue
// 0b010: Green
// 0b011: Cyan
// 0b100: Red
// 0b101: Magenta
// 0b110: Yellow
// 0b111: White (light detected)
// ==============
inline u8 LightGun_Phenix_GetLight(u8 state) { return (~state) & LIGHTGUN_PHENIX_LIGHT; }
