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
#include "math.h"
#include "game/sequence.h"

//=============================================================================
// DEFINES
//=============================================================================

// Functions prototypes
void Sequence_UpdateFixed();
void Sequence_UpdateOnce();
void Sequence_UpdateOnceRevert();
void Sequence_UpdateLoop();
void Sequence_UpdateLoopRevert();
void Sequence_UpdatePanBound();
void Sequence_UpdatePanLoop();
void Sequence_UpdateTimeline();
void Sequence_UpdateInput();

//=============================================================================
// VARIABLES
//=============================================================================

// Render variables
u8 g_SeqFrameCount = 0;				// Render frame count
u8 g_SeqDrawFrame = SEQ_DRAW_SKIP;	// Frame to render
u8 g_SeqFlag = SEQ_FLAG_NONE;		// Sequencer flag

// Sequence variables
const Sequence* g_SeqCur = NULL;	// Current sequence
u8 g_SeqFrame = 0;					// Current sequence's frame
SeqEventCB g_SeqEventCB;			// Event callback
callback g_SeqActionCB = NULL;		// Action callback

// Cursor
Mouse_State g_SeqMouseData;
u8 g_SeqCursorPosX;
u8 g_SeqCursorPosY;
u8 g_SeqInput = 0;
u8 g_SeqCustomCursor = SEQ_CUR_NONE;

const SeqAction* g_SeqActionHover;
u8 g_SeqActionCond;

// Configuration
u8 g_SeqFrameWait = 6;
SeqDrawCB g_SeqDrawCB;
SeqInputCB g_SeqInputCB;
const SeqAction* g_SeqActionMoveLeft;
const SeqAction* g_SeqActionMoveRight;
Sequence g_SeqTransition = { 0, 0, 0, 0, 0, 0, { SEQ_NEXT_AUTO, NULL, 0 }, { NULL } };

#if (SEQ_USE_TIMELINE)
const SeqKey** g_SeqTimelines;
u8 g_SeqTimelineTimer;
#endif

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Sequence updqte callback
const callback g_SeqUpdateModes[SEQ_MODE_MAX] =
{
	Sequence_UpdateFixed,
	Sequence_UpdateOnce,
	Sequence_UpdateOnceRevert,
	Sequence_UpdateLoop,
	Sequence_UpdateLoopRevert,
#if (SEQ_USE_PAN)
	Sequence_UpdatePanBound,
	Sequence_UpdatePanLoop,
#endif
#if (SEQ_USE_TIMELINE)
	Sequence_UpdateTimeline
#endif
};

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Interruption handler
void Sequence_Interrupt()
{
	g_SeqFrameCount++;
}

//-----------------------------------------------------------------------------
// Wait for sequence synchronization
void Sequence_Wait()
{
	while (g_SeqFrameCount < g_SeqFrameWait)
	{
	}
	g_SeqFrameCount = 0;
}

//-----------------------------------------------------------------------------
// Play a sequence
void Sequence_Play(const Sequence* seq, u8 frame)
{
	g_SeqFlag = SEQ_FLAG_NONE;
	g_SeqCur = seq;
	g_SeqEventCB(seq->StartEvent);

#if (SEQ_USE_TIMELINE)
	if (g_SeqCur->Mode == SEQ_MODE_TIMELINE)
	{
		g_SeqTimelineTimer = 0;
		g_SeqFrame = frame;
		const SeqKey* key = &g_SeqTimelines[g_SeqCur->FirstFrame][frame];
		if (key->Event)
			g_SeqEventCB(key->Event);
		Sequence_SetNextFrame(key->Frame);
	}
	else
#endif
	{
		u8 min = MIN(g_SeqCur->FirstFrame, g_SeqCur->LastFrame);
		u8 max = MAX(g_SeqCur->FirstFrame, g_SeqCur->LastFrame);
		if ((frame >= min) && (frame <= max))
			g_SeqFrame = frame;
		else
			g_SeqFrame = seq->FirstFrame;
		Sequence_SetNextFrame(g_SeqFrame);
	}
}

#if (SEQ_USE_PAN)
//-----------------------------------------------------------------------------
// Start a pan sequence transition
void Sequence_PlayPanTransition(u8 from, const Sequence* nextSeq, u8 nextFrame)
{
	if ((g_SeqCur->Mode == SEQ_MODE_PAN_BOUND) || (g_SeqCur->Mode == SEQ_MODE_PAN_LOOP))
	{
		g_SeqTransition.ID         = g_SeqCur->ID;
		g_SeqTransition.Mode       = (from > g_SeqFrame) ? SEQ_MODE_ONCE : SEQ_MODE_ONCE_REVERT;
		g_SeqTransition.FirstFrame = g_SeqFrame;
		g_SeqTransition.LastFrame  = from;
		g_SeqTransition.Next.Seq   = nextSeq;
		g_SeqTransition.Next.Frame = nextFrame;
		Sequence_Play(&g_SeqTransition, 0);
	}
}
#endif

//-----------------------------------------------------------------------------
// Check if cursor is into the giben area
bool Sequence_CheckArea(const SeqActionArea* area)
{
	if ((g_SeqCursorPosX >= area->StartX) && (g_SeqCursorPosX <= area->EndX) && (g_SeqCursorPosY >= area->StartY) && (g_SeqCursorPosY <= area->EndY))
		return TRUE;
	return FALSE;
}

//-----------------------------------------------------------------------------
// Update sequence for mode SEQ_MODE_FIXED
void Sequence_UpdateFixed()
{
}

//-----------------------------------------------------------------------------
// Update sequence for mode SEQ_MODE_ONCE
void Sequence_UpdateOnce()
{
	if (g_SeqFrame < g_SeqCur->LastFrame)
	{
		g_SeqFrame++;
		Sequence_SetNextFrame(g_SeqFrame);
	}
	else if ((g_SeqFlag & SEQ_FLAG_FINISHED) == 0) // Sequence end reached
	{
		Sequence_SetFinished();
		g_SeqEventCB(g_SeqCur->EndEvent);
		if (g_SeqCur->Next.Mode & SEQ_NEXT_AUTO)
			Sequence_Play(g_SeqCur->Next.Seq, g_SeqCur->Next.Frame);
	}
}

//-----------------------------------------------------------------------------
// Update sequence for mode SEQ_MODE_ONCE_REVERT
void Sequence_UpdateOnceRevert()
{
	if (g_SeqFrame > g_SeqCur->LastFrame)
	{
		g_SeqFrame--;
		Sequence_SetNextFrame(g_SeqFrame);
	}
	else if ((g_SeqFlag & SEQ_FLAG_FINISHED) == 0) // Sequence end reached
	{
		Sequence_SetFinished();
		g_SeqEventCB(g_SeqCur->EndEvent);
		if (g_SeqCur->Next.Mode & SEQ_NEXT_AUTO)
			Sequence_Play(g_SeqCur->Next.Seq, g_SeqCur->Next.Frame);
	}
}

//-----------------------------------------------------------------------------
// Update sequence for mode SEQ_MODE_LOOP
void Sequence_UpdateLoop()
{
	g_SeqFrame++;
	if (g_SeqFrame > g_SeqCur->LastFrame) // reached the loop point
	{
		g_SeqFrame = g_SeqCur->FirstFrame;
		Sequence_SetNextFrame(g_SeqFrame);
		g_SeqEventCB(g_SeqCur->EndEvent);
	}
	else
	{
		Sequence_SetNextFrame(g_SeqFrame);
	}
}

//-----------------------------------------------------------------------------
// Update sequence for mode SEQ_MODE_LOOP_REVERT
void Sequence_UpdateLoopRevert()
{
	if (g_SeqFrame == g_SeqCur->LastFrame) // reached the loop point
	{
		g_SeqFrame = g_SeqCur->FirstFrame;
		g_SeqEventCB(g_SeqCur->EndEvent);
	}
	else
		g_SeqFrame--;
	Sequence_SetNextFrame(g_SeqFrame);
}

#if (SEQ_USE_PAN)
//-----------------------------------------------------------------------------
// Update sequence for mode SEQ_MODE_PAN_BOUND
void Sequence_UpdatePanBound()
{
	// to be implemented...
}
#endif

#if (SEQ_USE_PAN)
//-----------------------------------------------------------------------------
// Update sequence for mode SEQ_MODE_PAN_LOOP
void Sequence_UpdatePanLoop()
{	
	// Check area
	if ((Sequence_CheckInput(SEQ_INPUT_MOVE_LEFT)) || (Sequence_CheckArea(g_SeqActionMoveLeft->Areas)))
	{
		Sequence_SetHoverAction(g_SeqActionMoveLeft);

		if ((Sequence_CheckInput(SEQ_INPUT_MOVE_LEFT) || (Sequence_CheckInput(SEQ_INPUT_PRESS_1))))
		{
			if (g_SeqFrame > g_SeqCur->FirstFrame)
				g_SeqFrame--;
			else
				g_SeqFrame = g_SeqCur->LastFrame;
			Sequence_SetNextFrame(g_SeqFrame);
			return;
		}
	}
	if ((Sequence_CheckInput(SEQ_INPUT_MOVE_RIGHT)) || (Sequence_CheckArea(g_SeqActionMoveRight->Areas)))
	{
		Sequence_SetHoverAction(g_SeqActionMoveRight);
		if ((Sequence_CheckInput(SEQ_INPUT_MOVE_RIGHT)) || (Sequence_CheckInput(SEQ_INPUT_PRESS_1)))
		{
			if (g_SeqFrame < g_SeqCur->LastFrame)
				g_SeqFrame++;
			else
				g_SeqFrame = g_SeqCur->FirstFrame;
			Sequence_SetNextFrame(g_SeqFrame);
			return;
		}
	}
}
#endif

#if (SEQ_USE_TIMELINE)
//-----------------------------------------------------------------------------
// Update sequence for mode SEQ_MODE_TIMELINE
void Sequence_UpdateTimeline()
{
	if (g_SeqFrame <= g_SeqCur->LastFrame)
	{
		const SeqKey* key = &g_SeqTimelines[g_SeqCur->FirstFrame][g_SeqFrame];
		g_SeqTimelineTimer++;
		if (g_SeqTimelineTimer >= key->Time)
		{
			if (g_SeqFrame < g_SeqCur->LastFrame) // End of step
			{
				g_SeqTimelineTimer = 0;
				g_SeqFrame++;
				key++;
				if (key->Event)
					g_SeqEventCB(key->Event);
				Sequence_SetNextFrame(key->Frame);
			}
			else if (g_SeqCur->Next.Mode & SEQ_NEXT_AUTO) // End of sequence
			{
				Sequence_Play(g_SeqCur->Next.Seq, g_SeqCur->Next.Frame);
			}
		}
	}
}
#endif

//-----------------------------------------------------------------------------
// Check if specific input flag is set
void Sequence_UpdateInput()
{

}

//-----------------------------------------------------------------------------
// Check all current sequence's actions
void Sequence_CheckActions()
{
	// Check for action hover
	for (u8 i = 0; g_SeqCur->Actions[i] != NULL; i++)
	{
		const SeqAction* const act = g_SeqCur->Actions[i];
		if (act->FrameMin == SEQ_FRAME_ALL)
		{
			if (Sequence_CheckArea(act->Areas))
			{
				Sequence_SetHoverAction(act);
				if (act->Condition)
				{
					g_SeqActionCond = act->Condition(act->Event);
					if (g_SeqActionCond != SEQ_COND_DISABLE)
						break;
				}
			}
		}
		else if ((g_SeqFrame >= act->FrameMin) && (g_SeqFrame <= act->FrameMax))
		{
			if (Sequence_CheckArea(&act->Areas[g_SeqFrame - act->FrameMin]))
			{
				Sequence_SetHoverAction(act);
				if (act->Condition)
				{
					g_SeqActionCond = act->Condition(act->Event);
					if (g_SeqActionCond != SEQ_COND_DISABLE)
						break;
				}
			}
		}
	}

	if (g_SeqActionCB)
		g_SeqActionCB();

	// Check for action click
	if (Sequence_CheckInput(SEQ_INPUT_CLICK_1))
	{
		if ((g_SeqActionCond == SEQ_COND_OK) && g_SeqActionHover)
		{
			// Trigger action event
			g_SeqEventCB(g_SeqActionHover->Event);
			// Check for auto-transition
			const SeqTransition* trans = &g_SeqActionHover->Trans;
			if (trans->Seq)
			{
#if (SEQ_USE_PAN)
				if (trans->From == SEQ_DIRECT)
					Sequence_Play(trans->Seq, trans->Frame);
				else
					Sequence_PlayPanTransition(trans->From, trans->Seq, trans->Frame);
#else
				Sequence_Play(trans->Seq, trans->Frame);
#endif
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Update cursor
void Sequence_UpdateCursor()
{
	// Update cursor pattern
	u8 pat = SEQ_CUR_DEFAULT;
	if (Sequence_HasCustomCursor())
		pat = Sequence_GetCustomCursor();
	if (g_SeqActionHover)
	{
		switch (g_SeqActionCond)
		{
		case SEQ_COND_OK:
			pat = g_SeqActionHover->Cursor;
			break;
		case SEQ_COND_LOCK:
			pat = SEQ_CUR_LOCK;
			break;
		}
	}

	// Update sprites data
	g_VDP_Sprite.Y = g_SeqCursorPosY - 8;
	g_VDP_Sprite.X = g_SeqCursorPosX - 7;
	g_VDP_Sprite.Pattern = pat * 8;
	g_VDP_Sprite.Color = 0;
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, g_SpriteAtributeLow, g_SpriteAtributeHigh, 3);
	g_VDP_Sprite.Pattern += 4;
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, g_SpriteAtributeLow + 4, g_SpriteAtributeHigh, 3);
}

//-----------------------------------------------------------------------------
// Update current sequence and check for interactions
void Sequence_Update()
{
	// Reset parameters
	g_SeqActionHover = NULL;
	g_SeqActionCond = SEQ_COND_OK;

	// Update input
	g_SeqInput = g_SeqInputCB();

	// Update sequence (mode specific function)
	g_SeqUpdateModes[g_SeqCur->Mode]();

	// Check actions
	Sequence_CheckActions();

	// Update cursor
	Sequence_UpdateCursor();

	// Draw current frame
	if (g_SeqFlag & SEQ_FLAG_DIRTY)
	{
		if (g_SeqDrawFrame != SEQ_DRAW_SKIP)
			g_SeqDrawCB(g_SeqDrawFrame);
		g_SeqFlag &= ~SEQ_FLAG_DIRTY;
	}
}