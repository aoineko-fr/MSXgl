// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄  ▄▄▄ ▄▄ ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ▀█▄  ▀█▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄█▀  ▄▄█▀ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// JSX driver module
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

#define JSX_SIGNATURE				0x0F

#define JSX_BUTTONS_PER_ROW			(6)
#define JSX_AXIS_VALUES				(64)
#define JSX_AXIS_MEDIAN				(32)

#define JSX_MAX_AXIS				(15)
#define JSX_MAX_ROWS				(3)
#define JSX_MAX_BUTTONS				(JSX_MAX_ROWS * JSX_BUTTONS_PER_ROW)
#define JSX_MAX_INPUT				(JSX_MAX_AXIS + JSX_MAX_ROWS)

#define JSX_GET_AXIS(a)				(i8)(((a) < 0) ? (a) : ((a) | 0xC0))
#define JSX_GET_BUTTON(a, b)		((a) & (1 << (b)))

// Input buffer
// extern u8 g_JSX_InputBuffer[2][JSX_MAX_INPUT];

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: JSX_Detect
// Detect JSX device in the given port.
//
// Parameters:
//   port	- Input port to detect (INPUT_PORT1 or INPUT_PORT2).
//
// Return:
//   JSX device ID.
//>  7 6 5 4 3 2 1 0
//> –-–-–-–-–-–-–-–--
//>  0 0 A A A A B B
//>      │ │ │ │ └─┴── Number of button rows (0-3)
//>      └─┴─┴─┴────── Number of axis (0-15)
u8 JSX_Detect(enum INPUT_PORT port);

// Function: JSX_Read
// Read JSX device in the given port.
//
// Parameters:
//   port	- Input port to read (INPUT_PORT1 or INPUT_PORT2).
//   buffer	- Buffer to store the input data.
//
// Return:
//   JSX device ID.
u8 JSX_Read(enum INPUT_PORT port, u8* buffer);

// // Function: JSX_GetInput
// inline u8 JSX_GetInput(u8 port, u8 idx) { return g_JSX_InputBuffer[port][idx]; }

// // Function: JSX_GetInputBuffer
// inline u8* JSX_GetInputBuffer(u8 port) { return g_JSX_InputBuffer[port]; }

// Function: JSX_GetAxisNumber
// Get the number of axis from the given JSX device ID.
//
// Parameters:
//   id	- JSX device ID.
//
// Return:
//   Number of axis.
inline u8 JSX_GetAxisNumber(u8 id) { return id >> 2; }

// Function: JSX_GetRowsNumber
// Get the number of button rows from the given JSX device ID.
//
// Parameters:
//   id	- JSX device ID.
//
// Return:
//   Number of button rows.
inline u8 JSX_GetRowsNumber(u8 id) { return id & 0x03; }

// Function: JSX_GetButtonsNumber
// Get the number of buttons from the given JSX device ID.
//
// Parameters:
//   id	- JSX device ID.
//
// Return:
//   Number of buttons.
inline u8 JSX_GetButtonsNumber(u8 id) { return (id & 0x03) * 6; }

// Function: JSXC_Detect
// Detect JSX device in the given port.
//
// Parameters:
//   port	- Input port to detect (INPUT_PORT1 or INPUT_PORT2).
//
// Return:
//   JSX device ID.
//>  7 6 5 4 3 2 1 0
//> –-–-–-–-–-–-–-–--
//>  0 0 A A A A B B
//>      │ │ │ │ └─┴── Number of button rows (0-3)
//>      └─┴─┴─┴────── Number of axis (0-15)
u8 JSXC_Detect(enum INPUT_PORT port);

// Function: JSXC_Read
// Read JSX device in the given port.
//
// Parameters:
//   port	- Input port to read (INPUT_PORT1 or INPUT_PORT2).
//   buffer	- Buffer to store the input data.
//
// Return:
//   JSX device ID.
u8 JSXC_Read(enum INPUT_PORT port, u8* buffer);
