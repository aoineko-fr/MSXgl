// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Program template
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

#define SEQ_USE_PAN					TRUE
#define SEQ_USE_TIMELINE			TRUE

// Functions callback
typedef void (*SeqEventCB)(u8 id);	// Sequence event callback signature
typedef u8   (*SeqCondCB)(u8 id);	// Sequence action condition check callback signature
typedef void (*SeqDrawCB)(u8 frame); // Sequence draw callback signature

// Sequence playback modes
enum SEQ_MODE
{
	SEQ_MODE_FIXED = 0,				// Display a fixed image
	SEQ_MODE_ONCE,					// Play sequence once and stop at last image
	SEQ_MODE_ONCE_REVERT,			// Play sequence backward once and stop at last image
	SEQ_MODE_LOOP,					// Play sequence looping from end to start
	SEQ_MODE_LOOP_REVERT,			// Play sequence backward looping from end to start
	SEQ_MODE_PAN_BOUND,				// Manual horizontal span between 2 bounds
	SEQ_MODE_PAN_LOOP,				// Manual horizontal span looping from end to start
	SEQ_MODE_TIMELINE,				// Play a sequence using a timeline
//.....................................
	SEQ_MODE_MAX,
};

// Sequence events
enum SEQ_EVENT
{
	SEQ_EVENT_START = 0,			// Sequence started
	SEQ_EVENT_END,					// Sequence ended
	SEQ_EVENT_LOOP,					// Sequence looped
	SEQ_EVENT_ACTION,				// Action triggered
//.....................................
	SEQ_EVENT_MAX,
//.....................................
	SEQ_EVENT_USER = 128,			// User defined event
};

// Sequence action types
enum SEQ_ACTION
{
	SEQ_ACT_AUTO = 0,				// Automatic action
	SEQ_ACT_CLICK_LEFT,				// Left click action
	SEQ_ACT_CLICK_RIGHT,			// Right click action
	SEQ_ACT_CLICK_UP,				// Up click action
	SEQ_ACT_CLICK_DOWN,				// Bottom click action
	SEQ_ACT_CLICK_AREA,				// Click on custom area
//.....................................
	SEQ_ACT_MAX,
};

// Sequence action conditions response
enum SEQ_CONDITION
{
	SEQ_COND_OK = 0,				// Condition is OK
	SEQ_COND_LOCK,					// Condition not met (action is locked)
	SEQ_COND_HIDE,					// Condition not met (action is hidden)
//.....................................
	SEQ_COND_MAX,
};

// Sequence action cursors
enum SEQ_CURSOR
{
	SEQ_CUR_DEFAULT = 0,
	SEQ_CUR_MOVE_LEFT,				// 
	SEQ_CUR_MOVE_RIGHT,				// 
	SEQ_CUR_MOVE_UP,				//
	SEQ_CUR_MOVE_DOWN,				//
	SEQ_CUR_USE,					//
	SEQ_CUR_TAKE,					//
	SEQ_CUR_LOOK,					//
	SEQ_CUR_LOCK,					//
//.....................................
	SEQ_CUR_MAX,
};

#define SEQ_CUR_NONE			0

// Sequence input flags
enum SEQ_INPUT
{
	SEQ_INPUT_CLICK_1		= 0b00000001,
	SEQ_INPUT_CLICK_2		= 0b00000010,
	SEQ_INPUT_PRESS_1		= 0b00000100,
	SEQ_INPUT_PRESS_2		= 0b00001000,
	SEQ_INPUT_MOVE_RIGHT	= 0b00010000,
	SEQ_INPUT_MOVE_LEFT		= 0b00100000,
	SEQ_INPUT_MOVE_UP		= 0b01000000,
	SEQ_INPUT_MOVE_DOWN		= 0b10000000,
};

// Sequence action area coordinate definition
typedef struct SeqActionArea
{
	u8 StartX;						// X coordinate of the start of the area
	u8 StartY;						// Y coordinate of the start of the area
	u8 EndX;						// X coordinate of the end of the area
	u8 EndY;						// Y coordinate of the end of the area
} SeqActionArea;

// Sequence action structure
typedef struct SeqAction
{
	u8 Action;						// Action type (see <SEQ_ACTION>)
	u8 Id;							// Action ID sent to the callback 
	u8 Cursor;						// Cursor type (see <SEQ_CURSOR>)
	SeqCondCB Condition;			// Condition check callback
	u8 FrameMin;					// Minimum frame to trigger the action
	u8 FrameMax;					// Maximum frame to trigger the action
	SeqActionArea Areas[];			// Action area
} SeqAction;

// Sequence timeline entry
typedef struct SeqTime
{
	u8 Time;						// Duration of the frame (in sequencer unit)
	u8 Frame;						// Frame number (relative to start)
} SeqTime;

// Sequence structure
typedef struct Sequence
{
	u16 ID;							// Sequence ID
	u8  Mode;						// Playback mode (see <SEQ_MODE>)
	u8  FirstFrame;					// First frame of the sequence (or table index for timelined sequence)
	u8  LastFrame;					// Last frame of the sequence (or number of entries for timelined sequence)
	SeqEventCB EventCB;				// Event callback
	struct Sequence* NextSeq;		// Sequence to start when reaching the end
	u8  NextFrame;					// Next sequence start frame
	u8  ActionNum;					// Number of actions
	const SeqAction* const Actions[];
} Sequence;

//=============================================================================
// VARIABLES
//=============================================================================

// Render variables
extern u8 g_SeqFrameCount;

// Sequence variables
extern const Sequence* g_SeqCur;
extern u8 g_SeqFrame;

// Cursor
extern Mouse_State g_SeqMouseData;
extern u8 g_SeqCursorPosX;
extern u8 g_SeqCursorPosY;
extern i8 g_SeqCursorAccX;
extern i8 g_SeqCursorAccY;
extern u8 g_SeqInput;
extern u8 g_SeqCustomCursor;
extern u8 g_SeqPrevRaw8;

extern const SeqAction* g_SeqActionHover;

// Configuration
extern u8 g_SeqFrameWait;
extern SeqDrawCB g_SeqDrawCB;
extern const SeqAction* g_SeqActionMoveLeft;
extern const SeqAction* g_SeqActionMoveRight;
extern const SeqTime** g_SeqTimelines;

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: Sequence_Initialize
// Initialize the sequencer
//
// Parameters:
//   wait  - Frame wait time
//   draw  - Draw callback function
//   left  - Action to move left
//   right - Action to move right
inline void Sequence_Initialize(u8 wait, SeqDrawCB draw, const SeqAction* left, const SeqAction* right) { g_SeqFrameWait = wait; g_SeqDrawCB = draw; g_SeqActionMoveLeft = left; g_SeqActionMoveRight = right; }

// Function: Sequence_InitializeTimeline
// Initialize the timeline table
//
// Parameters:
//   timelines - Pointer to the timeline table
inline void Sequence_InitializeTimeline(const SeqTime** timelines) { g_SeqTimelines = timelines; }

// Function: Sequence_SetCursor
// Set the cursor position
//
// Parameters:
//   x - X coordinate of the cursor
//   y - Y coordinate of the cursor
inline void Sequence_SetCursor(u8 x, u8 y) { g_SeqCursorPosX = x; g_SeqCursorPosY = y; g_SeqCursorAccX = 0; g_SeqCursorAccY = 0; }

// Function: Sequence_MoveCursor
// Move the cursor position
//
// Parameters:
//   x - X coordinate move offset
//   y - Y coordinate move offset
inline void Sequence_MoveCursor(i8 x, i8 y) { g_SeqCursorPosX += x; g_SeqCursorPosY += y; }

// Function: Sequence_SetCustomCursor
// Set a custom cursor pattern
// Parameters:
//   cursor - Custum cursor pattern index (can't be 0)
inline void Sequence_SetCustomCursor(u8 cursor) { g_SeqCustomCursor = cursor; }

// Function: Sequence_GetCustomCursor
// Get the current custom cursor pattern
//
// Return:
//   Current custom cursor pattern index
inline u8 Sequence_GetCustomCursor() { return g_SeqCustomCursor; }

// Function: Sequence_HasCustomCursor
// Check if a custom cursor pattern is set
//
// Return:
//   FALSE if no custom cursor pattern is set
inline bool Sequence_HasCustomCursor() { return g_SeqCustomCursor != SEQ_CUR_NONE; }

// Function: Sequence_ClearCustomCursor
// Clear custom cursor pattern
inline void Sequence_ClearCustomCursor() { g_SeqCustomCursor = SEQ_CUR_NONE; }

// Function: Sequence_Play
// Play a sequence
//
// Parameters:
//   seq   - Pointer to the sequence structure
//   frame - Frame number to start playing
void Sequence_Play(const Sequence* seq, u8 frame);

// Function: Sequence_GetCurrent
// Get the current sequence
//
// Return:
//   Pointer to the current sequence structure
inline const Sequence* Sequence_GetCurrent() { return g_SeqCur; }

// Function: Sequence_GetFrame
// Get the current frame number
//
// Return:
//   Current frame number
inline u8 Sequence_GetFrame() { return g_SeqFrame; }

// Function: Sequence_GetInput
// Get the current input flags
// Return:
//   Current input flags
inline u8 Sequence_GetInput() { return g_SeqInput; }

// Function: Sequence_CheckInput
// Check if specific input flag is set
//
// Parameters:
//   in - Input flag to check
//
// Return:
//   FALSE if input flag is not set
inline bool Sequence_CheckInput(u8 in) { return g_SeqInput & in; }

// Function: Sequence_CheckArea
// Check if cursor is in area
//
// Parameters:
//   area - Pointer to the action area to check
//
// Return:
//   FALSE if cursor is not in area
bool Sequence_CheckArea(const SeqActionArea* area);

// Function: Sequence_Update
// Update sequence and check for interaction
void Sequence_Update();

// Function: Sequence_Interrupt
// Interruption handler
void Sequence_Interrupt();

// Function: Sequence_Wait
// Wait for sequence synchronization
void Sequence_Wait();
