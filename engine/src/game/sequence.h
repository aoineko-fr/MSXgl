// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄███ ▄▀██ ██ █ ▄███ ██▀▄ ▄█▀▀ ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀█▄▄  ▀██ ▀█▄█ ▀█▄▄ ██ █ ▀█▄▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘              ▀▀
//  Gameplay sequence module
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

#define SEQ_CUR_NONE				0xFF	// No custom cursor

#define SEQ_FRAME_ALL				0xFF	// Special frame number for action frame start (non frame depend action)
#define SEQ_FRAME_BLACK				0xFF	// Special frame number for timelined sequences
#define SEQ_FRAME_WHITE				0xFE	// Special frame number for timelined sequences
#define SEQ_DIRECT					0xFF	// Special frame number for transitions

// Functions callback
typedef void (*SeqEventCB)(u8 id);	// Sequence event callback signature
typedef u8   (*SeqCondCB)(u8 id);	// Sequence action condition check callback signature
typedef void (*SeqDrawCB)(u8 frame); // Sequence draw callback signature

// Sequence playback modes
enum SEQ_MODE_ID
{
	SEQ_MODE_FIXED = 0,				// Display a fixed image
	SEQ_MODE_ONCE,					// Play sequence once and stop at last image
	SEQ_MODE_ONCE_REVERT,			// Play sequence backward once and stop at last image
	SEQ_MODE_LOOP,					// Play sequence looping from end to start
	SEQ_MODE_LOOP_REVERT,			// Play sequence backward looping from end to start
	SEQ_MODE_PAN_BOUND,				// Manual horizontal span between 2 bounds
	SEQ_MODE_PAN_LOOP,				// Manual horizontal span looping from end to start
#if (SEQ_USE_TIMELINE)
	SEQ_MODE_TIMELINE,				// Play a sequence using a timeline
#endif
//.....................................
	SEQ_MODE_MAX,
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
	SEQ_NEXT_NONE = 0,				// No auto transition
	SEQ_NEXT_AUTO,					// Automatic transition when sequence ends
	SEQ_NEXT_MANUAL,				// Manual transition to next sequence
//.....................................
	SEQ_NEXT_MAX,
};

// Sequence action conditions response
enum SEQ_CONDITION_ID
{
	SEQ_COND_OK = 0,				// Condition is OK
	SEQ_COND_LOCK,					// Condition not met (action is locked)
	SEQ_COND_DISABLE,				// Condition not met (action is disabled)
//.....................................
	SEQ_COND_MAX,
};

// Sequence action cursors
enum SEQ_CURSOR_ID
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

// Sequence custom area structure
typedef struct SeqCustomArea
{
	struct SeqActionArea Area;
	u8 Event;
} SeqCustomArea;

// Sequence transition structure
typedef struct SeqNext
{
	u8 Mode;						// Next sequence mode
	struct Sequence* Seq;			// Sequence to transition to
	u8 Frame;						// Transition sequence start frame
} SeqNext;

// Sequence transition structure
typedef struct SeqTransition
{
	u8 From;						// Pan's frame to transition from
	struct Sequence* Seq;			// Sequence to transition to
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

// Sequence variables
extern const Sequence* g_SeqCur;	// Current sequence
extern u8 g_SeqFrame;				// Current sequence's frame
extern SeqEventCB g_SeqEventCB;		// Event callback

// Cursor
extern Mouse_State g_SeqMouseData;
extern u8 g_SeqCursorPosX;
extern u8 g_SeqCursorPosY;
extern u8 g_SeqCursorAccX;
extern u8 g_SeqCursorAccY;
extern u8 g_SeqInput;
extern u8 g_SeqCustomCursor;
extern u8 g_SeqPrevRaw8;

extern const SeqAction* g_SeqActionHover;

// Configuration
extern u8 g_SeqFrameWait;
extern SeqDrawCB g_SeqDrawCB;
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
//   left  - Action to move left
//   right - Action to move right
//   up    - Action to move up
//   down  - Action to move down
inline void Sequence_Initialize(u8 wait, SeqEventCB event, SeqDrawCB draw, const SeqAction* left, const SeqAction* right) { g_SeqFrameWait = wait; g_SeqEventCB = event; g_SeqDrawCB = draw; g_SeqActionMoveLeft = left; g_SeqActionMoveRight = right; }

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

// Function: SeqStartTransition
// Start a pan sequence transition
//
// Parameters:
//   frame    - Pan's frame number to start the transition
//   nextSeq  - Pointer to the next sequence structure
//   nextFrame - Frame number to start the next sequence
void Sequence_PlayPanTransition(u8 frame, struct Sequence* nextSeq, u8 nextFrame);

// Function: Sequence_ForceDraw
// Force redraw of the current frame
inline void Sequence_ForceDraw() { g_SeqDrawCB(g_SeqFrame); }

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
