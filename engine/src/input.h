// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄                ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ██▀▄ ██▀▄ ██ █ ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄██▄ ██ █ ██▀  ▀█▄█ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘            ▀▀  
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// User input handler using direct access to ports
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//-----------------------------------------------------------------------------
// Group: Joystick
// Direct access to joystick
//-----------------------------------------------------------------------------
#if (USE_INPUT_JOYSTICK || USE_INPUT_MANAGER)

#define JOY_PORT_1					0
#define JOY_PORT_2					(1 << 6)

#define JOY_INPUT_DIR_NONE			0
#define JOY_INPUT_DIR_UP			(1 << 0)
#define JOY_INPUT_DIR_DOWN			(1 << 1)
#define JOY_INPUT_DIR_LEFT			(1 << 2)
#define JOY_INPUT_DIR_RIGHT			(1 << 3)
#define JOY_INPUT_DIR_UP_RIGTH		(JOY_INPUT_DIR_UP + JOY_INPUT_DIR_RIGHT)
#define JOY_INPUT_DIR_UP_LEFT		(JOY_INPUT_DIR_UP + JOY_INPUT_DIR_LEFT)
#define JOY_INPUT_DIR_DOWN_RIGTH	(JOY_INPUT_DIR_DOWN + JOY_INPUT_DIR_RIGHT)
#define JOY_INPUT_DIR_DOWN_LEFT		(JOY_INPUT_DIR_DOWN + JOY_INPUT_DIR_LEFT)
#define JOY_INPUT_DIR_MASK			0x0F
#define JOY_INPUT_TRIGGER_A			(1 << 4)
#define JOY_INPUT_TRIGGER_B			(1 << 5)

#define IS_JOY_PRESSED(stat, input) ((stat & input) == 0)
#define IS_JOY_RELEASED(stat, input) ((stat & input) != 0)

// Function: Joystick_Read
// Get the current joystick information
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
u8 Joystick_Read(u8 port) __FASTCALL;

// Function: Joystick_GetDirection
// Get current direction of the given joystick
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
u8 Joystick_GetDirection(u8 port) __FASTCALL;

// Function: Joystick_GetTrigger
// Get current trigger status of the given joystick (0: released; 1: pressed)
//
// Parameters:
//   port - JOY_PORT_1 or JOY_PORT_2
//   trigger - JOY_INPUT_TRIGGER_A or JOY_INPUT_TRIGGER_B
inline u8 Joystick_GetTrigger(u8 port, u8 trigger)
{
	u8 in = Joystick_Read(port);
	return ((in & trigger) == 0);
}
#endif // (USE_INPUT_JOYSTICK || USE_INPUT_MANAGER)

//-----------------------------------------------------------------------------
// Group: Keyboard
// Direct access to keyboard
//-----------------------------------------------------------------------------
#if (USE_INPUT_KEYBOARD || USE_INPUT_MANAGER)

#define MAKE_KEY(r, b)		((b << 4) | r)
#define KEY_ROW(key)		(key & 0x0F)
#define KEY_IDX(key)		(key >> 4)
#define KEY_FLAG(key)		(1 << KEY_IDX(key))

#define IS_KEY_PRESSED(row, key) ((row & KEY_FLAG(key)) == 0)
#define IS_KEY_RELEASED(row, key) ((row & KEY_FLAG(key)) != 0)

// Row #0 keys
#define KEY_0			MAKE_KEY(0, 0)
#define KEY_1			MAKE_KEY(0, 1)
#define KEY_2			MAKE_KEY(0, 2)
#define KEY_3			MAKE_KEY(0, 3)
#define KEY_4			MAKE_KEY(0, 4)
#define KEY_5			MAKE_KEY(0, 5)
#define KEY_6			MAKE_KEY(0, 6)
#define KEY_7			MAKE_KEY(0, 7)
// Row #1 keys
#define KEY_8			MAKE_KEY(1, 0)
#define KEY_9			MAKE_KEY(1, 1)
#define KEY_1_2			MAKE_KEY(1, 2) // Undefined ?
#define KEY_1_3			MAKE_KEY(1, 3) // Undefined ?
#define KEY_1_4			MAKE_KEY(1, 4) // Undefined ?
#define KEY_1_5			MAKE_KEY(1, 5) // Undefined ?
#define KEY_1_6			MAKE_KEY(1, 6) // Undefined ?
#define KEY_1_7			MAKE_KEY(1, 7) // Undefined ?
// Row #2 keys
#define KEY_2_0			MAKE_KEY(2, 0) // Undefined ?
#define KEY_2_1			MAKE_KEY(2, 1) // Undefined ?
#define KEY_2_2			MAKE_KEY(2, 2) // Undefined ?
#define KEY_2_3			MAKE_KEY(2, 3) // Undefined ?
#define KEY_2_4			MAKE_KEY(2, 4) // Undefined ?
#define KEY_2_5			MAKE_KEY(2, 5) // Undefined ?
#define KEY_A			MAKE_KEY(2, 6)
#define KEY_B			MAKE_KEY(2, 7)
// Row #3 keys
#define KEY_C			MAKE_KEY(3, 0)
#define KEY_D			MAKE_KEY(3, 1)
#define KEY_E			MAKE_KEY(3, 2)
#define KEY_F			MAKE_KEY(3, 3)
#define KEY_G			MAKE_KEY(3, 4)
#define KEY_H			MAKE_KEY(3, 5)
#define KEY_I			MAKE_KEY(3, 6)
#define KEY_J			MAKE_KEY(3, 7)
// Row #4 keys
#define KEY_K			MAKE_KEY(4, 0)
#define KEY_L			MAKE_KEY(4, 1)
#define KEY_M			MAKE_KEY(4, 2)
#define KEY_N			MAKE_KEY(4, 3)
#define KEY_O			MAKE_KEY(4, 4)
#define KEY_P			MAKE_KEY(4, 5)
#define KEY_Q			MAKE_KEY(4, 6)
#define KEY_R			MAKE_KEY(4, 7)
// Row #5 keys
#define KEY_S			MAKE_KEY(5, 0)
#define KEY_T			MAKE_KEY(5, 1)
#define KEY_U			MAKE_KEY(5, 2)
#define KEY_V			MAKE_KEY(5, 3)
#define KEY_W			MAKE_KEY(5, 4)
#define KEY_X			MAKE_KEY(5, 5)
#define KEY_Y			MAKE_KEY(5, 6)
#define KEY_Z			MAKE_KEY(5, 7)
// Row #6 keys
#define KEY_SHIFT		MAKE_KEY(6, 0)
#define KEY_CTRL		MAKE_KEY(6, 1)
#define KEY_GRAPH		MAKE_KEY(6, 2)
#define KEY_CAPS		MAKE_KEY(6, 3)
#define KEY_CODE		MAKE_KEY(6, 4)
#define KEY_F1			MAKE_KEY(6, 5)
#define KEY_F2			MAKE_KEY(6, 6)
#define KEY_F3			MAKE_KEY(6, 7)
// Row #7 keys
#define KEY_F4			MAKE_KEY(7, 0)
#define KEY_F5			MAKE_KEY(7, 1)
#define KEY_ESC			MAKE_KEY(7, 2)
#define KEY_TAB			MAKE_KEY(7, 3)
#define KEY_STOP		MAKE_KEY(7, 4)
#define KEY_BS			MAKE_KEY(7, 5)
#define KEY_SELECT		MAKE_KEY(7, 6)
#define KEY_RET			MAKE_KEY(7, 7)
// Row #8 keys
#define KEY_SPACE		MAKE_KEY(8, 0)
#define KEY_HOME		MAKE_KEY(8, 1)
#define KEY_INS			MAKE_KEY(8, 2)
#define KEY_DEL			MAKE_KEY(8, 3)
#define KEY_LEFT		MAKE_KEY(8, 4)
#define KEY_UP			MAKE_KEY(8, 5)
#define KEY_DOWN		MAKE_KEY(8, 6)
#define KEY_RIGHT		MAKE_KEY(8, 7)
// Row #9 keys
#define KEY_NUM_MUL		MAKE_KEY(9, 0)
#define KEY_NUM_ADD		MAKE_KEY(9, 1)
#define KEY_NUM_DIV		MAKE_KEY(9, 2)
#define KEY_NUM_0		MAKE_KEY(9, 3)
#define KEY_NUM_1		MAKE_KEY(9, 4)
#define KEY_NUM_2		MAKE_KEY(9, 5)
#define KEY_NUM_3		MAKE_KEY(9, 6)
#define KEY_NUM_4		MAKE_KEY(9, 7)
// Row #10 keys
#define KEY_NUM_5		MAKE_KEY(10, 0)
#define KEY_NUM_6		MAKE_KEY(10, 1)
#define KEY_NUM_7		MAKE_KEY(10, 2)
#define KEY_NUM_8		MAKE_KEY(10, 3)
#define KEY_NUM_9		MAKE_KEY(10, 4)
#define KEY_NUM_MIN		MAKE_KEY(10, 5)
#define KEY_NUM_COM		MAKE_KEY(10, 6)
#define KEY_NUM_DOT		MAKE_KEY(10, 7)

// Function: Keyboard_Read
// Read keyboard matrix row
//
// Parameters:
//   row - The row to read (0-10)
u8 Keyboard_Read(u8 row) __FASTCALL;

// Function: Keyboard_IsKeyPressed
// Check if a given key is pressed
//
// Parameters:
//   key - The key ID to check
bool Keyboard_IsKeyPressed(u8 key) __FASTCALL;

#endif // (USE_INPUT_KEYBOARD || USE_INPUT_MANAGER)

//-----------------------------------------------------------------------------
// Group: Input Manager
// Advanced input manager
//-----------------------------------------------------------------------------
#if USE_INPUT_MANAGER

// Device ID
enum IPM_DEVICE
{
	IPM_DEVICE_JOYSTICK_1 = 0,
	IPM_DEVICE_JOYSTICK_2,
	IPM_DEVICE_KEYBOARD_1,
	IPM_DEVICE_KEYBOARD_2,
	//---------------------------------
	IPM_DEVICE_MAX,
	IPM_DEVICE_ANY,
};

// Device input ID
enum IPM_INPUT
{
	IPM_INPUT_STICK = 0,
	IPM_INPUT_BUTTON_A,
	IPM_INPUT_BUTTON_B,
	//---------------------------------
	IPM_INPUT_MAX,
	IPM_INPUT_ANY,
};

// Device event ID
enum IPM_EVENT
{
	IPM_EVENT_CLICK = 0,
	IPM_EVENT_HOLD,
	IPM_EVENT_DOUBLE_CLICK,	
	IPM_EVENT_DOUBLE_CLICK_HOLD,
	IPM_EVENT_RELEASE,
	//---------------------------------
	IPM_EVENT_MAX,
	IPM_EVENT_ANY,
	IPM_EVENT_NONE,
};

// State flag
#define IPM_STATE_OFF			0x00
#define IPM_STATE_PRESS			0x01
#define IPM_STATE_ON			0x02
#define IPM_STATE_RELEASE		0x03
#define IPM_STATE_PRESSMASK		0x03
#define IPM_STATE_HOLD			0x10
#define IPM_STATE_HOLDING		0x20
#define IPM_STATE_HOLDMASK		0x30
#define IPM_STATE_DOUBLE		0x80

#define IPM_EVENT_TAB_SIZE		16

typedef void (*IPM_cb)(u8 joy, u8 in, u8 evt);
typedef u8 (*IPM_check)(u8 joy, u8 in);

typedef struct
{
	u8			Device;
	u8			Input;
	u8			Event;
	IPM_cb		Callback;
} IPM_Event;

typedef struct
{
	u8			CurrentStatus;
	u8			PreviousStatus;
	u8			State[IPM_INPUT_MAX];
	u8			Timer[IPM_INPUT_MAX];
} IPM_Process;	

typedef struct
{
	u8			Up;
	u8			Right;
	u8			Down;
	u8			Left;
	u8			TriggerA;
	u8			TriggerB;
} IPM_KeySet;	

typedef struct
{
	u8			DeviceSupport[IPM_DEVICE_MAX];
	u8			HoldTimer;
	u8			DoubleClickTimer;
	IPM_KeySet	KeySet[2];
} IPM_Config;	

static IPM_Config g_DefaultConfig =
{
	// DeviceSupport
	{	true,	true,	true,	true },
	// Timer
	0x10, 0x04,
	// Keyboard set
	{
		{ KEY_UP,	KEY_RIGHT,	KEY_DOWN,	KEY_LEFT,	KEY_SPACE,	KEY_N },
		{ KEY_W,	KEY_D,		KEY_S,		KEY_A,		KEY_CTRL,	KEY_SHIFT },
	}
};

typedef struct
{
	IPM_Process	Process[IPM_DEVICE_MAX];
	IPM_Event	Events[IPM_EVENT_TAB_SIZE];
	u8			EventsNum;
	IPM_check	Checker[IPM_EVENT_MAX];
	IPM_Config  Config;
} IPM_Data;

extern IPM_Data g_IPM;

// Function: IPM_Initialize
// Initialize input manager
void IPM_Initialize(IPM_Config* config);

// Function: IPM_SetTimer
// Initialize input manager
inline void IPM_SetTimer(u8 doubleClk, u8 hold)
{
	g_IPM.Config.DoubleClickTimer = doubleClk;
	g_IPM.Config.HoldTimer = hold;
}

// Function: IPM_Update
// Update device manager
void IPM_Update();

// Function: IPM_RegisterEvent
// Register a callback to a given device manager's event
bool IPM_RegisterEvent(u8 joy, u8 input, u8 event, IPM_cb cb);

// Function: IPM_GetStatus
// Get current device status
inline u8 IPM_GetStatus(u8 joy);

// Function: IPM_GetStickDirection
// Get current direction of the given device
inline u8 IPM_GetStickDirection(u8 joy);

// Function: IPM_GetInputState
// Get current device state
inline u8 IPM_GetInputState(u8 joy, u8 in);

// Function: IPM_GetInputTimer
// Get current device state timer
inline u8 IPM_GetInputTimer(u8 joy, u8 in);

// Function: IPM_GetEventName
// Get event name
const c8* IPM_GetEventName(u8 ev);

#endif // USE_INPUT_MANAGER























