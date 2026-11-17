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

// LIGHTGUN_DRIVER
#ifndef LIGHTGUN_DRIVER
	#warning LIGHTGUN_DRIVER is not defined in "msxgl_config.h"! Default value will be used: (LIGHTGUN_ASCII + LIGHTGUN_GUNSTICK + LIGHTGUN_PHENIX)
	#define LIGHTGUN_DRIVER			(LIGHTGUN_ASCII + LIGHTGUN_GUNSTICK + LIGHTGUN_PHENIX)
#endif
// LIGHTGUN_USE_UPDATE
#ifndef LIGHTGUN_USE_UPDATE
	#warning LIGHTGUN_USE_UPDATE is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define LIGHTGUN_USE_UPDATE		TRUE
#endif
// LIGHTGUN_FRAME_DURATION
#ifndef LIGHTGUN_FRAME_DURATION
	#warning LIGHTGUN_FRAME_DURATION is not defined in "msxgl_config.h"! Default value will be used: 2
	#define LIGHTGUN_FRAME_DURATION	2
#endif
// LIGHTGUN_FRAME_LATENCY
#ifndef LIGHTGUN_FRAME_LATENCY
	#warning LIGHTGUN_FRAME_LATENCY is not defined in "msxgl_config.h"! Default value will be used: 4
	#define LIGHTGUN_FRAME_LATENCY	4
#endif

//=============================================================================
// DEFINES
//=============================================================================

// ASCII Plus-X Terminator Laser mapping
#define LIGHTGUN_ASCII_TRIGGER		(JOY_INPUT_TRIGGER_B)
#define LIGHTGUN_ASCII_LIGHT		(JOY_INPUT_TRIGGER_A)

// MHT Ingenieros Gun-Stick mapping
#define LIGHTGUN_GUNSTICK_TRIGGER	(JOY_INPUT_TRIGGER_A)		
#define LIGHTGUN_GUNSTICK_LIGHT		(JOY_INPUT_DIR_DOWN)

// Phenix Light Gun mapping
#define LIGHTGUN_PHENIX_TRIGGER_A	(JOY_INPUT_TRIGGER_A)
#define LIGHTGUN_PHENIX_TRIGGER_B	(JOY_INPUT_TRIGGER_B)
#define LIGHTGUN_PHENIX_LIGHT_MASK	(0x0F) // 3-bit RGB color (0b000: Black, 0b111: White) + 1-bit luminance
#define LIGHTGUN_PHENIX_LIGHT		(0x08)

#if (LIGHTGUN_DRIVER & LIGHTGUN_PHENIX)

// Light gun detection states
enum LIGHTGUN_STATE_ID
{
	LIGHTGUN_STATE_IDLE = 0,		// Idle
	LIGHTGUN_STATE_DETECT,			// Idle -> Trigger
	LIGHTGUN_STATE_BLACK,			// Trigger -> Black
	LIGHTGUN_STATE_WHITE,			// Black -> White
	LIGHTGUN_STATE_HIT,				// White -> Hit!
//.....................................
	LIGHTGUN_STATE_MAX,
};

// Light gun detection event
enum LIGHTGUN_EVENT_ID
{
	LIGHTGUN_EVENT_REF1 = 0,		// Reference frame (black screen)
	LIGHTGUN_EVENT_DETECT,			// Detection frame (white square)
	LIGHTGUN_EVENT_REF2,			// Reference frame (black screen)
	LIGHTGUN_EVENT_GAME,			// Detection ended (back to )
	LIGHTGUN_EVENT_HIT,				// Hit occurs
//.....................................
	LIGHTGUN_EVENT_MAX,
};

// Event callback
typedef void (*LightGun_EventCB)(u8);				

// Function to be called if hit occurs
extern LightGun_EventCB g_LightGun_CB;

// Joystick port to check
extern u8 g_LightGun_Port;

// Current lightgun raw value
extern u8 g_LightGun_Value;

// Previous trigger value
extern bool g_LightGun_TrigPrev;

// Light detection counter
extern u8 g_LightGun_Counter;

// Light detection state
extern u8 g_LightGun_State;

// 
#if (LIGHTGUN_FRAME_DURATION == LIGHTGUN_CUSTOM)
extern u8 g_LightGun_Duration;
#endif

// 
#if (LIGHTGUN_FRAME_LATENCY == LIGHTGUN_CUSTOM)
extern u8 g_LightGun_Latency;
#endif

#endif // (LIGHTGUN_DRIVER & LIGHTGUN_PHENIX)


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

#if (LIGHTGUN_DRIVER & LIGHTGUN_ASCII)

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

#endif // (LIGHTGUN_DRIVER & LIGHTGUN_ASCII)

//-----------------------------------------------------------------------------
// Group: MHT Ingenieros Gun-Stick
//-----------------------------------------------------------------------------

#if (LIGHTGUN_DRIVER & LIGHTGUN_GUNSTICK)

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

#endif // (LIGHTGUN_DRIVER & LIGHTGUN_GUNSTICK)

//-----------------------------------------------------------------------------
// Group: Phenix Light Gun
//-----------------------------------------------------------------------------

#if (LIGHTGUN_DRIVER & LIGHTGUN_PHENIX)

#if (!LIGHTGUN_USE_UPDATE)

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

// Function: LightGun_Phenix_GetLightValue
// Get the light value of the Phenix light gun.
//
// Parameters:
//   state - Current state of the light gun (from <LightGun_Read>)
//
// Return:
//   Detected 4-bit light intensity (0b0000: Black, 0b1111: White)
inline u8 LightGun_Phenix_GetLightValue(u8 state) { return state & LIGHTGUN_PHENIX_LIGHT_MASK; }

// Function: LightGun_Phenix_GetLight
// Get the light status of the Phenix light gun.
//
// Parameters:
//   state - Current state of the light gun (from <LightGun_Read>)
//
// Return:
//   FALSE if the gun don't detect light (not pointing to white image on screen).
inline bool LightGun_Phenix_GetLight(u8 state) { return (state >= LIGHTGUN_PHENIX_LIGHT); }

#else // if (LIGHTGUN_USE_UPDATE)

// Function: LightGun_Phenix_GetTriggerA
// Get the trigger A status of the Phenix light gun.
//
// Return:
//   FALSE if the trigger A is not pressed.
inline bool LightGun_Phenix_GetTriggerA() { return (g_LightGun_Value & LIGHTGUN_PHENIX_TRIGGER_A) == 0; }

// Function: LightGun_Phenix_GetTriggerB
// Get the trigger B status of the Phenix light gun.

//
// Return:
//   FALSE if the trigger B is not pressed.
inline bool LightGun_Phenix_GetTriggerB() { return (g_LightGun_Value & LIGHTGUN_PHENIX_TRIGGER_B) == 0; }

// Function: LightGun_Phenix_GetLightValue
// Get the light value of the Phenix light gun.
//
// Parameters:
//   state - Current state of the light gun (from <LightGun_Read>)
//
// Return:
//   Detected 4-bit light intensity (0b0000: Black, 0b1111: White)
inline u8 LightGun_Phenix_GetLightValue() { return g_LightGun_Value & LIGHTGUN_PHENIX_LIGHT_MASK; }

// Function: LightGun_Phenix_GetLight
// Get the light status of the Phenix light gun.
//
// Parameters:
//   state - Current state of the light gun (from <LightGun_Read>)
//
// Return:
//   FALSE if the gun don't detect light (not pointing to white image on screen).
inline bool LightGun_Phenix_GetLight() { return g_LightGun_Value >= LIGHTGUN_PHENIX_LIGHT; }

// Function: LightGun_Phenix_Init
// Initialize the Phenix light gun.
//
// Parameters:
//   cb - Function to be called when detection occurs
inline void LightGun_Phenix_Init(LightGun_EventCB cb, u8 port) { g_LightGun_CB = cb; g_LightGun_Port = port; }

// Function: LightGun_Phenix_Update
// Update the Phenix light gun.
void LightGun_Phenix_Update();

// Function: LightGun_Phenix_GetRaw
// Get current lightgun raw value
inline u8 LightGun_Phenix_GetRaw() { return g_LightGun_Value; }

// Function: LightGun_Phenix_GetCounter
// Get light detection counter
inline u8 LightGun_Phenix_GetCounter() { return g_LightGun_Counter; }

// Function: LightGun_Phenix_GetState
// Get light detection state
inline u8 LightGun_Phenix_GetState() { return g_LightGun_State; }

#if (LIGHTGUN_FRAME_DURATION == LIGHTGUN_CUSTOM)
// Function: LightGun_SetFrameDuration
// Light gun detection frames duration
//
// Paremeters:
//  frame - Number of frames for each detection phase (black > white > black)
inline void LightGun_SetFrameDuration(u8 frame) { g_LightGun_Duration = frame; }
#endif

#if (LIGHTGUN_FRAME_LATENCY == LIGHTGUN_CUSTOM)
// Function: LightGun_SetFrameDuration
// Light gun detection frames latency
//
// Paremeters:
//  latency - Number of frames allowed to support display screen latency
inline void LightGun_SetFrameLatency(u8 latency) { g_LightGun_Latency = latency; }
#endif

#endif // (LIGHTGUN_USE_UPDATE)

#endif // (LIGHTGUN_DRIVER & LIGHTGUN_PHENIX)