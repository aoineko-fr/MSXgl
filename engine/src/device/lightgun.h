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

#define LIGHTGUN_USE_ASCII			TRUE
#define LIGHTGUN_USE_GUNSTICK		TRUE
#define LIGHTGUN_USE_PHENIX			TRUE
#define LIGHTGUN_USE_UPDATE			TRUE


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
#define LIGHTGUN_PHENIX_LIGHT		(0x0F) // 3-bit RGB color (0b000: Black, 0b111: White) + 1-bit luminance

#define LGUN_WHITE_THRESHOLD		(8) // x >= LGUN_WHITE_THRESHOLD --> white
#define LGUN_BLACK_THRESHOLD		(8) // x < LGUN_BLACK_THRESHOLD --> black

#define LGUN_FRAME_BLACK			(2)
#define LGUN_FRAME_WHITE			(2)

#define LGUN_COUNT_BLACK1			(0)
#define LGUN_COUNT_WHITE			(LGUN_FRAME_BLACK)
#define LGUN_COUNT_BLACK2			((LGUN_FRAME_BLACK) + (LGUN_FRAME_WHITE))
#define LGUN_COUNT_DEFAULT			((LGUN_FRAME_BLACK) + (LGUN_FRAME_WHITE) + (LGUN_FRAME_BLACK))
#define LGUN_COUNT_MAX				((LGUN_COUNT_DEFAULT) + 8)

enum LGUN_STATE_ID
{
	LGUN_STATE_DEFAULT = 0,			// Default
	LGUN_STATE_DETECT,				// Default -> Trigger
	LGUN_STATE_BLACK,				// Trigger -> Black
	LGUN_STATE_WHITE,				// Black -> White
	LGUN_STATE_HIT,					// White -> Hit
//.....................................
	LGUN_STATE_MAX,
};

enum LGUN_EVENT_ID
{
	LGUN_EVENT_REF1 = 0,			// Reference frame (black screen)
	LGUN_EVENT_DETECT,				// Detection frame (white square)
	LGUN_EVENT_REF2,				// Reference frame (black screen)
	LGUN_EVENT_GAME,				// Detection ended (back to )
	LGUN_EVENT_HIT,					// Hit occurs
//.....................................
	LGUN_EVENT_MAX,
};

// Event callback
typedef void (*LGun_EventCB)(u8);				

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

#if (LIGHTGUN_USE_ASCII)

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

#endif // (LIGHTGUN_USE_ASCII)

//-----------------------------------------------------------------------------
// Group: MHT Ingenieros Gun-Stick
//-----------------------------------------------------------------------------

#if (LIGHTGUN_USE_GUNSTICK)

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

#endif // (LIGHTGUN_USE_GUNSTICK)

//-----------------------------------------------------------------------------
// Group: Phenix Light Gun
//-----------------------------------------------------------------------------

#if (LIGHTGUN_USE_PHENIX)

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
inline u8 LightGun_Phenix_GetLight(u8 state) { return state & LIGHTGUN_PHENIX_LIGHT; }

#else // if (LIGHTGUN_USE_UPDATE)

// Function to be called if hit occurs
extern LGun_EventCB g_LGun_CB;

// Joystick port to check
extern u8 g_LGun_Port;

// Current lightgun raw value
extern u8 g_LGun_Value;

// Previous trigger value
extern bool g_LGun_TrigPrev;

// Light detection counter
extern u8 g_LGun_Counter;

// Light detection state
extern u8 g_LGun_State;

// Function: LightGun_Phenix_GetTriggerA
// Get the trigger A status of the Phenix light gun.
//
// Return:
//   FALSE if the trigger A is not pressed.
inline bool LightGun_Phenix_GetTriggerA() { return (g_LGun_Value & LIGHTGUN_PHENIX_TRIGGER_A) == 0; }

// Function: LightGun_Phenix_GetTriggerB
// Get the trigger B status of the Phenix light gun.

//
// Return:
//   FALSE if the trigger B is not pressed.
inline bool LightGun_Phenix_GetTriggerB() { return (g_LGun_Value & LIGHTGUN_PHENIX_TRIGGER_B) == 0; }

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
inline u8 LightGun_Phenix_GetLight() { return g_LGun_Value & LIGHTGUN_PHENIX_LIGHT; }

// Function: LightGun_Phenix_Init
// Initialize the Phenix light gun.
//
// Parameters:
//   cb - Function to be called when detection occurs
inline void LightGun_Phenix_Init(LGun_EventCB cb, u8 port) { g_LGun_CB = cb; g_LGun_Port = port; }

// Function: LightGun_Phenix_Update
// Update the Phenix light gun.
void LightGun_Phenix_Update();

// Get current lightgun raw value
inline u8 Lightgun_Phenix_GetRaw() { return g_LGun_Value; }

// Get light detection counter
inline u8 Lightgun_Phenix_GetCounter() { return g_LGun_Counter; }

// Get light detection state
inline u8 Lightgun_Phenix_GetState() { return g_LGun_State; }


#endif // (LIGHTGUN_USE_UPDATE)

#endif // (LIGHTGUN_USE_PHENIX)