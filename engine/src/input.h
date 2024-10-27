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

// Handle interrupt disabling
#if (INPUT_USE_ISR_PROTECTION)
	#define INPUT_DI				di
	#define INPUT_EI				ei
#else
	#define INPUT_DI
	#define INPUT_EI
#endif

//=============================================================================
// Group: Joystick
// Direct access to joystick
//=============================================================================
#if (INPUT_USE_JOYSTICK || INPUT_USE_DETECT)

#define JOY_PORT_1					0b00000011
#define JOY_PORT_2					0b01001100

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

#define IS_JOY_PRESSED(stat, input) ((stat & input) == 0)
#define IS_JOY_RELEASED(stat, input) ((stat & input) != 0)

#define JOY_GET_DIR(in)				(~(in) & JOY_INPUT_DIR_MASK))
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
// Returns:
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
// Returns:
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
// Returns:
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
	if(in == prev)
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
// Returns:
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
// Returns:
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

#endif // (INPUT_USE_JOYSTICK || INPUT_USE_DETECT)


//=============================================================================
// Group: Detect
// General purpose port device detection
//=============================================================================
#if (INPUT_USE_DETECT)

// Value returned from device in idle state (allows approximate detection of the connected device)
enum INPUT_TYPE
{
	INPUT_TYPE_UNPLUGGED			= 0x3F,
	INPUT_TYPE_JOYSTICK				= INPUT_TYPE_UNPLUGGED,
	INPUT_TYPE_MOUSE				= 0x30,
	INPUT_TYPE_TRACKBALL			= 0x38,
	INPUT_TYPE_PADDLE				= 0x3E, // Arkanoid Vaus Paddle or MSX-Paddle
	// Unvalidated types
	INPUT_TYPE_TOUCHPAD				= 0x39, // Can also be 3D or 3B
	INPUT_TYPE_LIGHTGUN				= 0x2F, // Can also be 20 or 3F
	INPUT_TYPE_MUSIC_PAD			= 0x3C, // Yamaha MMP-01 music pad
	INPUT_TYPE_IBM_ADAPTER			= 0x3A, // IBM-PC DA15 joystick adapter
	INPUT_TYPE_ATARI_ADAPTER		= 0x36, // Atari dual-paddle adapter
};

#define INPUT_PORT_1				JOY_PORT_1
#define INPUT_PORT_2				JOY_PORT_2

// Function: Input_Detect
// Detect device plugged in General purpose ports
inline u8 Input_Detect(u8 port) { return Joystick_Read(port) & 0x3F; }

#endif

//=============================================================================
// Group: Mouse
// Direct access to mouse
//=============================================================================
#if (INPUT_USE_MOUSE)

#define MOUSE_PORT_1				0b00010011
#define MOUSE_PORT_2				0b01101100

#define MOUSE_NOTFOUND				0xFF

#define MOUSE_BOUTON_1				0b00010000
#define MOUSE_BOUTON_2				0b00100000
#define MOUSE_BOUTON_LEFT			MOUSE_BOUTON_1
#define MOUSE_BOUTON_RIGHT			MOUSE_BOUTON_2

// Mouse sensitivity enumeration
enum MOUSE_SPEED
{
	MOUSE_SPEED_LOWEST  = 16, // [-8:7]
	MOUSE_SPEED_LOW     = 8,  // [-16:15]
	MOUSE_SPEED_MEDIUM  = 4,  // [-32:31]
	MOUSE_SPEED_HIGH    = 2,  // [-64:63]
	MOUSE_SPEED_HIGHEST = 1,  // [-127:127]
	MOUSE_SPEED_DEFAULT = MOUSE_SPEED_MEDIUM,
};

// Mouse state structure (don't change parameter order)
typedef struct
{
	u8			Buttons;
	i8			dX;
	i8			dY;
	u8			PrevButtons;
} Mouse_State;

// Function: Mouse_Read
void Mouse_Read(u8 port, Mouse_State* data);

// Function: Mouse_GetOffsetX
inline i8 Mouse_GetOffsetX(Mouse_State* data) { return -data->dX; }

// Function: Mouse_GetOffsetY
inline i8 Mouse_GetOffsetY(Mouse_State* data) { return -data->dY; }

// Function: Mouse_GetAdjustedOffsetX
inline i8 Mouse_GetAdjustedOffsetX(Mouse_State* data, u8 spd) { return -data->dX / spd; }

// Function: Mouse_GetAdjustedOffsetY
inline i8 Mouse_GetAdjustedOffsetY(Mouse_State* data, u8 spd) { return -data->dY / spd; }

// Function: Mouse_IsButtonPress
inline bool Mouse_IsButtonPress(Mouse_State* data, u8 btn) { return (data->Buttons & btn) == 0; }

// Function: Mouse_IsButtonClick
inline bool Mouse_IsButtonClick(Mouse_State* data, u8 btn) { return ((data->Buttons & btn) == 0) && ((data->PrevButtons & btn) != 0); }

#endif // (INPUT_USE_MOUSE)

//=============================================================================
// Group: Keyboard
// Direct access to keyboard
//=============================================================================
#if (INPUT_USE_KEYBOARD)

#define MAKE_KEY(r, b)		((b << 4) | r)
#define KEY_ROW(key)		(key & 0x0F)
#define KEY_IDX(key)		(key >> 4)
#define KEY_FLAG(key)		(1 << KEY_IDX(key))

#define IS_KEY_PRESSED(row, key)  ((row & KEY_FLAG(key)) == 0)
#define IS_KEY_RELEASED(row, key) ((row & KEY_FLAG(key)) != 0)
#define IS_KEY_PUSHED(row, prev, key)  (((row & KEY_FLAG(key)) == 0) && ((prev & KEY_FLAG(key)) != 0))

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
// Returns:
//   8-bits value where each bit to 0 represents the pressed keys
u8 Keyboard_Read(u8 row) __FASTCALL __PRESERVES(b, c, d, e, h, iyl, iyh);

#if (INPUT_KB_UPDATE)

// Buffer to store current frame keys state
extern u8* g_InputBufferNew;
// Buffer to store previous frame keys state
extern u8* g_InputBufferOld;

// Function: Keyboard_SetBuffer
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
// Returns:
//   TRUE if key is pressed
bool Keyboard_IsKeyPressed(u8 key);

// Function: Keyboard_IsKeyPushed
// Checks if a given key was just pushed
// Only available when INPUT_KB_UPDATE is TRUE
//
// Parameters:
//   key - The key ID to check
//
// Returns:
//   TRUE if the key was pushed on this frame
bool Keyboard_IsKeyPushed(u8 key);

#else

// Check if a given key is pressed
bool Keyboard_IsKeyPressed(u8 key);

#endif // (INPUT_KB_UPDATE)

#endif // (INPUT_USE_KEYBOARD)

//=============================================================================
// Group: Paddle
// Direct access to paddle
//=============================================================================
#if (INPUT_USE_PADDLE)

extern u16 g_PaddleStates[2];

// Function: Paddle_Update
// Update both joystick port and store paddles information
void Paddle_Update();

// Function: Paddle_IsConnected
// Check if a paddle is connected to the given port
//
// Parameters:
//   port - Index of the port to check (0: Port A, 1: Port B)
//
// Returns:
//   TRUE if a paddle is connected to that port
inline bool Paddle_IsConnected(u8 port) { return (g_PaddleStates[port] & 0x8000) == 0; }

// Function: Paddle_GetAngle
// Get the rotation angle of a paddle on the given port
//
// Parameters:
//   port - Index of the port to check (0: Port A, 1: Port B)
//
// Returns:
//   Paddle angle (theorical value goes from 0 to 510, but valid range is around 110 to 380)
inline u16 Paddle_GetAngle(u8 port) { return g_PaddleStates[port] & 0x01FF; }

// Function: Paddle_IsButtonPressed
// Check if the given paddle's button is pressed
//
// Parameters:
//   port - Index of the port to check (0: Port A, 1: Port B)
//
// Returns:
//   TRUE if the given paddle's button is pressed
inline bool Paddle_IsButtonPressed(u8 port) { return (g_PaddleStates[port] & 0x0200) == 0; }

#if (INPUT_USE_PADDLE_CALIB)

extern i16 g_PaddleOffset[2];

// Function: Paddle_SetCalibration
// Calibrate the given paddle
//
// Parameters:
//   port - Index of the port to check (0: Port A, 1: Port B)
//   min - Minimum recoreded value
//   max - Maximum recoreded value
inline void Paddle_SetCalibration(u8 port, u16 min, u16 max) { g_PaddleOffset[port] = min + (max - min) / 2 - 128; }

// Function: Paddle_GetCalibratedAngle
// Get the callibrated rotation angle of a paddle on the given port
//
// Parameters:
//   port - Index of the port to check (0: Port A, 1: Port B)
//
// Returns:
//   Paddle angle (from 0 to 255)
u8 Paddle_GetCalibratedAngle(u8 port);

#endif // (INPUT_USE_PADDLE_CALIB)

#endif // (INPUT_USE_PADDLE)
