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
// DEFINES
//=============================================================================

//=============================================================================
// Group: Keyboard
// Direct access to keyboard
//=============================================================================

#define MAKE_KEY(r, b)		(((b) << 4) | (r))
#define KEY_ROW(key)		((key) & 0x0F)
#define KEY_IDX(key)		((key) >> 4)
#define KEY_FLAG(key)		(1 << KEY_IDX(key))

#define IS_KEY_PRESSED(row, key)  (((row) & KEY_FLAG(key)) == 0)
#define IS_KEY_RELEASED(row, key) (((row) & KEY_FLAG(key)) != 0)
#define IS_KEY_PUSHED(row, prev, key)  ((((row) & KEY_FLAG(key)) == 0) && (((prev) & KEY_FLAG(key)) != 0))

// Function: KEY_ID
// Value encoded by combining a row number with a given physical key index in that row
// Can be KEY_1, KEY_SPACE, KEY_A, KEY_F1, etc.
enum KEY_ID
{
	// Row #0 keys
	KEY_0		= MAKE_KEY(0, 0),
	KEY_1		= MAKE_KEY(0, 1),
	KEY_2		= MAKE_KEY(0, 2),
	KEY_3		= MAKE_KEY(0, 3),
	KEY_4		= MAKE_KEY(0, 4),
	KEY_5		= MAKE_KEY(0, 5),
	KEY_6		= MAKE_KEY(0, 6),
	KEY_7		= MAKE_KEY(0, 7),
	// Row #1 keys
	KEY_8		= MAKE_KEY(1, 0),
	KEY_9		= MAKE_KEY(1, 1),
	KEY_1_2		= MAKE_KEY(1, 2), // Undefined ?
	KEY_1_3		= MAKE_KEY(1, 3), // Undefined ?
	KEY_1_4		= MAKE_KEY(1, 4), // Undefined ?
	KEY_1_5		= MAKE_KEY(1, 5), // Undefined ?
	KEY_1_6		= MAKE_KEY(1, 6), // Undefined ?
	KEY_1_7		= MAKE_KEY(1, 7), // Undefined ?
	// Row #2 keys                   
	KEY_2_0		= MAKE_KEY(2, 0), // Undefined ?
	KEY_2_1		= MAKE_KEY(2, 1), // Undefined ?
	KEY_2_2		= MAKE_KEY(2, 2), // Undefined ?
	KEY_2_3		= MAKE_KEY(2, 3), // Undefined ?
	KEY_2_4		= MAKE_KEY(2, 4), // Undefined ?
	KEY_2_5		= MAKE_KEY(2, 5), // Undefined ?
	KEY_A		= MAKE_KEY(2, 6),
	KEY_B		= MAKE_KEY(2, 7),
	// Row #3 keys
	KEY_C		= MAKE_KEY(3, 0),
	KEY_D		= MAKE_KEY(3, 1),
	KEY_E		= MAKE_KEY(3, 2),
	KEY_F		= MAKE_KEY(3, 3),
	KEY_G		= MAKE_KEY(3, 4),
	KEY_H		= MAKE_KEY(3, 5),
	KEY_I		= MAKE_KEY(3, 6),
	KEY_J		= MAKE_KEY(3, 7),
	// Row #4 keys
	KEY_K		= MAKE_KEY(4, 0),
	KEY_L		= MAKE_KEY(4, 1),
	KEY_M		= MAKE_KEY(4, 2),
	KEY_N		= MAKE_KEY(4, 3),
	KEY_O		= MAKE_KEY(4, 4),
	KEY_P		= MAKE_KEY(4, 5),
	KEY_Q		= MAKE_KEY(4, 6),
	KEY_R		= MAKE_KEY(4, 7),
	// Row #5 keys
	KEY_S		= MAKE_KEY(5, 0),
	KEY_T		= MAKE_KEY(5, 1),
	KEY_U		= MAKE_KEY(5, 2),
	KEY_V		= MAKE_KEY(5, 3),
	KEY_W		= MAKE_KEY(5, 4),
	KEY_X		= MAKE_KEY(5, 5),
	KEY_Y		= MAKE_KEY(5, 6),
	KEY_Z		= MAKE_KEY(5, 7),
	// Row #6 keys
	KEY_SHIFT	= MAKE_KEY(6, 0),
	KEY_CTRL	= MAKE_KEY(6, 1),
	KEY_GRAPH	= MAKE_KEY(6, 2),
	KEY_CAPS	= MAKE_KEY(6, 3),
	KEY_CODE	= MAKE_KEY(6, 4),
	KEY_F1		= MAKE_KEY(6, 5),
	KEY_F2		= MAKE_KEY(6, 6),
	KEY_F3		= MAKE_KEY(6, 7),
	// Row #7 keys
	KEY_F4		= MAKE_KEY(7, 0),
	KEY_F5		= MAKE_KEY(7, 1),
	KEY_ESC		= MAKE_KEY(7, 2),
	KEY_TAB		= MAKE_KEY(7, 3),
	KEY_STOP	= MAKE_KEY(7, 4),
	KEY_BS		= MAKE_KEY(7, 5),
	KEY_SELECT	= MAKE_KEY(7, 6),
	KEY_RETURN	= MAKE_KEY(7, 7),
	// Row #8 keys
	KEY_SPACE	= MAKE_KEY(8, 0),
	KEY_HOME	= MAKE_KEY(8, 1),
	KEY_INS		= MAKE_KEY(8, 2),
	KEY_DEL		= MAKE_KEY(8, 3),
	KEY_LEFT	= MAKE_KEY(8, 4),
	KEY_UP		= MAKE_KEY(8, 5),
	KEY_DOWN	= MAKE_KEY(8, 6),
	KEY_RIGHT	= MAKE_KEY(8, 7),
	// Row #9 keys
	KEY_NUM_MUL	= MAKE_KEY(9, 0),
	KEY_NUM_ADD	= MAKE_KEY(9, 1),
	KEY_NUM_DIV	= MAKE_KEY(9, 2),
	KEY_NUM_0	= MAKE_KEY(9, 3),
	KEY_NUM_1	= MAKE_KEY(9, 4),
	KEY_NUM_2	= MAKE_KEY(9, 5),
	KEY_NUM_3	= MAKE_KEY(9, 6),
	KEY_NUM_4	= MAKE_KEY(9, 7),
	// Row #10 keys
	KEY_NUM_5	= MAKE_KEY(10, 0),
	KEY_NUM_6	= MAKE_KEY(10, 1),
	KEY_NUM_7	= MAKE_KEY(10, 2),
	KEY_NUM_8	= MAKE_KEY(10, 3),
	KEY_NUM_9	= MAKE_KEY(10, 4),
	KEY_NUM_MIN	= MAKE_KEY(10, 5),
	KEY_NUM_COM	= MAKE_KEY(10, 6),
	KEY_NUM_DOT	= MAKE_KEY(10, 7),
};
// Key
#define KEY_RET			KEY_RETURN
#define KEY_ENTER		KEY_RETURN
#define KEY_BACK		KEY_BS

// Function: Keyboard_Read
// Reads keyboard matrix row
//
// Parameters:
//   row - The row to read (0-10)
//
// Return:
//   8-bits value where each bit to 0 represents the pressed keys
u8 Keyboard_Read(u8 row) __FASTCALL __PRESERVES(b, c, d, e, h, iyl, iyh);

#if (INPUT_KB_UPDATE)

// Buffer to store current frame keys state
extern u8* g_InputBufferNew;
// Buffer to store previous frame keys state
extern u8* g_InputBufferOld;

// Function: Keyboard_SetBuffer
// Set the buffer to store keyboard state
//
// Parameters:
//   new - Buffer to store current frame keys state
//   old - Buffer to store previous frame keys state
inline void Keyboard_SetBuffer(u8* new, u8* old) { g_InputBufferNew = new; g_InputBufferOld = old; }

// Function: Keyboard_Update
// Updates all keyboard rows at once
// Only available when INPUT_KB_UPDATE is TRUE
void Keyboard_Update();

// Function: Keyboard_IsKeyPressed
// When INPUT_KB_UPDATE is TRUE, this function uses data retrieved by Keyboard_Update() function
// Otherwise, the function reads the key's row before checking the key.
//
// Parameters:
//   key - The key ID to check
//
// Return:
//   TRUE if key is pressed
inline bool Keyboard_IsKeyPressed(u8 key) { return (g_InputBufferNew[KEY_ROW(key)] & (u8)(1 << KEY_IDX(key))) == 0; }

// Function: Keyboard_IsKeyPushed
// Checks if a given key was just pushed
// Only available when INPUT_KB_UPDATE is TRUE
//
// Parameters:
//   key - The key ID to check
//
// Return:
//   TRUE if the key was pushed on this frame
bool Keyboard_IsKeyPushed(u8 key);

#else

// Check if a given key is pressed
inline bool Keyboard_IsKeyPressed(u8 key) { return (Keyboard_Read(KEY_ROW(key)) & (u8)(1 << KEY_IDX(key))) == 0; }

#endif // (INPUT_KB_UPDATE)

#if (INPUT_KB_AS_JOYSTICK)

// Function: Keyboard_ReadAsJoystick
// Emulate joystick input using keyboard keys (see Joystick_Read)
//
// Return:
//   Keyboard state (bit=0: pressed)
// : xxBARLDU
// :   │││││└─ Up
// :   ││││└── Down
// :   │││└─── Left
// :   ││└──── Right
// :   │└───── Trigger A (space)
// :   └────── Trigger B (N)
u8 Keyboard_ReadAsJoystick() __NAKED __PRESERVES(e, h, l, iyl, iyh);

#endif // (INPUT_KB_AS_JOYSTICK)