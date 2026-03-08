// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄███ ▄▀██ ██ █ ▄███ ██▀▄ ▄█▀▀ ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀█▄▄  ▀██ ▀█▄█ ▀█▄▄ ██ █ ▀█▄▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘              ▀▀
//  Gameplay sequence module
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

// Configuration
#define SEQ_USE_PAN					TRUE	// Add support for manual panning sequences
#define SEQ_USE_TIMELINE			TRUE	// Add support for timelined sequences
#define SEQ_USE_CUSTOM				TRUE	// Add support for custom sequences

// Cursor
#define SEQ_CUR_NONE				0xFF	// No custom cursor

// Frame
#define SEQ_FRAME_ALL				0xFF	// Special frame number for action frame start (non frame depend action)
#define SEQ_DIRECT					0xFF	// Special frame number for transitions
#define SEQ_DRAW_SKIP				0xFF	// Special frame number to skip rendering

// Functions callback
typedef void (*SeqEventCB)(u8 id);   // Sequence event callback signature
typedef u8   (*SeqCondCB)(u8 id);    // Sequence action condition check callback signature
typedef u8   (*SeqInputCB)();        // Sequence action input check callback signature
typedef void (*SeqDrawCB)(u8 frame); // Sequence draw callback signature

// Sequence playback modes
enum SEQ_MODE_ID
{
	SEQ_MODE_FIXED = 0,				// Display a fixed image
	SEQ_MODE_ONCE,					// Play sequence once and stop at last image
	SEQ_MODE_ONCE_REVERT,			// Play sequence backward once and stop at last image
	SEQ_MODE_LOOP,					// Play sequence looping from end to start
	SEQ_MODE_LOOP_REVERT,			// Play sequence backward looping from end to start
#if (SEQ_USE_PAN)
	SEQ_MODE_PAN_BOUND,				// Manual horizontal span between 2 bounds
	SEQ_MODE_PAN_LOOP,				// Manual horizontal span looping from end to start
#endif
#if (SEQ_USE_TIMELINE)
	SEQ_MODE_TIMELINE,				// Play a sequence using a timeline
#endif
//.....................................
	SEQ_MODE_MAX,
};

// Sequence flag
enum SEQ_FLAG_ID
{
	SEQ_FLAG_NONE		= 0b00000000,
	SEQ_FLAG_DIRTY		= 0b00000001,	// Dirty flag
	SEQ_FLAG_FINISHED	= 0b00000010,	// Sequence end reached
};

// Sequence events
enum SEQ_EVENT_ID
{
	SEQ_EVENT_NONE = 0,				// No event
	SEQ_EVENT_LEFT,					// Left click event
	SEQ_EVENT_RIGHT,				// Right click event
	SEQ_EVENT_UP,					// Up click event
	SEQ_EVENT_DOWN,					// Down click event
	SEQ_EVENT_NEXT,					// Next sequence event
//.....................................
	SEQ_EVENT_MAX,
	SEQ_EVENT_USER = SEQ_EVENT_MAX, // User defined event
};

// Sequence events
enum SEQ_NEXT_ID
{
	SEQ_NEXT_NONE   		= 0b00000000, // No auto transition
	SEQ_NEXT_AUTO   		= 0b00000001, // Automatic transition when sequence ends
	SEQ_NEXT_MANUAL 		= 0b00000010, // Manual transition to next sequence
	SEQ_NEXT_BOTH 		    = SEQ_NEXT_AUTO|SEQ_NEXT_MANUAL, // Automatic or manual transition
};

// Sequence action conditions response
enum SEQ_CONDITION_ID
{
	SEQ_COND_OK = 0,				// Condition is OK
	SEQ_COND_LOCK,					// Condition not met (action is locked)
	SEQ_COND_DISABLE,				// Condition not met (action is disabled)
	SEQ_COND_HIDE,					// Condition not met (cursor is hidden)
//.....................................
	SEQ_COND_MAX,
};

// Sequence action cursors
enum SEQ_CURSOR_ID
{
	SEQ_CUR_DEFAULT = 0,			// Default cursor
	SEQ_CUR_LEFT,					// 
	SEQ_CUR_RIGHT,					// 
	SEQ_CUR_UP,						//
	SEQ_CUR_DOWN,					//
	SEQ_CUR_USE,					//
	SEQ_CUR_TAKE,					//
	SEQ_CUR_LOOK,					//
	SEQ_CUR_LOCK,					//
	SEQ_CUR_MUSIC,					//
//.....................................
	SEQ_CUR_MAX,
};

// Sequence input flags
enum SEQ_INPUT_ID
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

// Sequence transition structure
typedef struct SeqNext
{
	u8 Mode;						// Next sequence mode
	const struct Sequence* Seq;		// Sequence to transition to
	u8 Frame;						// Transition sequence start frame
} SeqNext;

// Sequence transition structure
typedef struct SeqTransition
{
#if (SEQ_USE_PAN)
	u8 From;						// Pan's frame to transition from
#endif
	const struct Sequence* Seq;		// Sequence to transition to
	u8 Frame;						// Transition sequence start frame
} SeqTransition;

// Sequence action structure
typedef struct SeqAction
{
	u8 Event;						// Action ID sent to the callback 
	u8 Cursor;						// Cursor type (see <SEQ_CURSOR>)
	SeqCondCB Condition;			// Condition check callback
	SeqTransition Trans;			// Sequence to auto-start when action triggered (if Trans.Seq is not NULL)
	u8 FrameMin;					// Minimum frame to trigger the action
	u8 FrameMax;					// Maximum frame to trigger the action
	SeqActionArea Areas[];			// Action area
} SeqAction;

#if (SEQ_USE_TIMELINE)
// Sequence timeline entry
typedef struct SeqKey
{
	u8 Time;						// Duration of the frame (in sequencer unit)
	u8 Frame;						// Frame number (relative to start)
	u8 Event;						// Event to be triggered at the begining of each step
} SeqKey;
#endif

// Sequence structure
typedef struct Sequence
{
	u16 ID;							// Sequence ID
	u8  Mode;						// Playback mode (see <SEQ_MODE>)
	u8  FirstFrame;					// First frame of the sequence (or table index for timelined sequence)
	u8  LastFrame;					// Last frame of the sequence (or number of entries for timelined sequence)
	u8  StartEvent;					// Event triggered when sequence starts
	u8  EndEvent;					// Event triggered when sequence ends or loops
	SeqNext Next;					// Sequence to be played after this one (if Next.Seq is not NULL)
	const SeqAction* const Actions[]; // NULL-terminated list of actions
} Sequence;

//=============================================================================
// VARIABLES
//=============================================================================

// Render variables
extern u8 g_SeqFrameCount;			// Render frame count
extern u8 g_SeqDrawFrame;			// Frame to render
extern u8 g_SeqFlag;				// Sequencer flag

// Sequence variables
extern const Sequence* g_SeqCur;	// Current sequence
extern u8 g_SeqFrame;				// Current sequence's frame
extern SeqEventCB g_SeqEventCB;		// Event callback
extern callback g_SeqActionCB;		// Action callback

// Cursor
extern Mouse_State g_SeqMouseData;
extern u8 g_SeqCursorPosX;
extern u8 g_SeqCursorPosY;
extern u8 g_SeqInput;
extern u8 g_SeqCustomCursor;

extern const SeqAction* g_SeqActionHover;

// Configuration
extern u8 g_SeqFrameWait;
extern SeqDrawCB g_SeqDrawCB;
extern SeqInputCB g_SeqInputCB;
extern const SeqAction* g_SeqActionMoveLeft;
extern const SeqAction* g_SeqActionMoveRight;
extern Sequence g_SeqTransition;

#if (SEQ_USE_TIMELINE)
extern const SeqKey** g_SeqTimelines;
extern u8 g_SeqTimelineTimer;
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: Sequence_Initialize
// Initialize the sequencer
//
// Parameters:
//   wait  - Frame wait time
//   event - Event callback function
//   draw  - Draw callback function
//   input - Input check callback function
inline void Sequence_Initialize(u8 wait, SeqEventCB event, SeqDrawCB draw, SeqInputCB input) { g_SeqFrameWait = wait; g_SeqEventCB = event; g_SeqDrawCB = draw; g_SeqInputCB = input; }

// Function: Sequence_SetActions
// Initialize the left and right move actions
//
// Parameters:
//   left  - Action to move left
//   right - Action to move right
inline void Sequence_SetActions(const SeqAction* left, const SeqAction* right) { g_SeqActionMoveLeft = left; g_SeqActionMoveRight = right; }

#if (SEQ_USE_TIMELINE)
// Function: Sequence_InitializeTimeline
// Initialize the timeline table
//
// Parameters:
//   timelines - Pointer to the timeline table
inline void Sequence_InitializeTimeline(const SeqKey** timelines) { g_SeqTimelines = timelines; }
#endif

// Function: Sequence_SetSynchFrames
// Set the synchronization display frame count
//
// Parameters:
//   frames - Number of display frames to wait before updating
inline void Sequence_SetSynchFrames(u8 frames) { g_SeqFrameWait = frames; }

// Function: Sequence_SetEventCallback
// Set the event callback function
//
// Parameters:
//   event - Event callback function
inline void Sequence_SetEventCallback(SeqEventCB event) { g_SeqEventCB = event; }

// Function: Sequence_SetDrawCallback
// Set the draw callback function
//
// Parameters:
//   draw - Draw callback function
inline void Sequence_SetDrawCallback(SeqDrawCB draw) { g_SeqDrawCB = draw; }

// Function: Sequence_SetDirty
// Set dirty flag to force redraw
inline void Sequence_SetDirty() { g_SeqFlag |= SEQ_FLAG_DIRTY; }

// Function: Sequence_SetFinished
// Set finish flag to force redraw
inline void Sequence_SetFinished() { g_SeqFlag |= SEQ_FLAG_FINISHED; }

// Function: Sequence_SetNextFrame
// Set next frame to be rendered
inline void Sequence_SetNextFrame(u8 frame) { g_SeqDrawFrame = frame; Sequence_SetDirty(); }

// Function: Sequence_SetCursor
// Set the cursor position
//
// Parameters:
//   x - X coordinate of the cursor
//   y - Y coordinate of the cursor
inline void Sequence_SetCursor(u8 x, u8 y) { g_SeqCursorPosX = x; g_SeqCursorPosY = y; }

// Function: Sequence_GetCursorX
// Get the cursor X coordinate
//
// Return:
//   Cursor X coordinate
inline u8 Sequence_GetCursorX() { return g_SeqCursorPosX; }

// Function: Sequence_GetCursorY
// Get the cursor Y coordinate
//
// Return:
//   Cursor Y coordinate
inline u8 Sequence_GetCursorY() { return g_SeqCursorPosY; }

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

#if (SEQ_USE_PAN)
// Function: SeqStartTransition
// Start a pan sequence transition
//
// Parameters:
//   frame    - Pan's frame number to start the transition
//   nextSeq  - Pointer to the next sequence structure
//   nextFrame - Frame number to start the next sequence
void Sequence_PlayPanTransition(u8 frame, const Sequence* nextSeq, u8 nextFrame);
#endif

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

// Function: Sequence_SetActionCallback
// Set action callback 
//
// Parameters:
//   cb - Function to be called during action check
inline void Sequence_SetActionCallback(callback cb) { g_SeqActionCB = cb;}

// Function: Sequence_SetHoverAction
// Set the action hovered by the cursor
//
// Parameters:
//   act - Pointer to hover action (can be NULL)
inline void Sequence_SetHoverAction(const SeqAction* act) { g_SeqActionHover = act; }

// Function: Sequence_GetHoverAction
// Get the action hovered by the cursor
//
// Return:
//   Pointer to hover action (can be NULL)
inline const SeqAction* Sequence_GetHoverAction() { return g_SeqActionHover; }

// Function: Sequence_Update
// Update current sequence and check for interactions
void Sequence_Update();

// Function: Sequence_Interrupt
// Interruption handler
void Sequence_Interrupt();

// Function: Sequence_Wait
// Wait for sequence synchronization
void Sequence_Wait();
