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

// MACRO
#define SEQ_MOUSE_SPEED				2
#define SEQ_CURSOR_SPD_TABLE		32


//=============================================================================
// VARIABLES
//=============================================================================

u8 g_SeqFrameCount = 0;				// Render frame count

const Sequence* g_SeqCur;	// Current sequence
u8 g_SeqFrame = 0;					// Current sequence's frame

// Cursor
Mouse_State g_SeqMouseData;
u8 g_SeqCursorPosX;
u8 g_SeqCursorPosY;
u8 g_SeqCursorAccX = 0;
u8 g_SeqCursorAccY = 0;
u8 g_SeqInput = 0;
u8 g_SeqCustomCursor = SEQ_CUR_NONE;
u8 g_SeqPrevRaw8 = 0xFF;

const SeqAction* g_SeqActionHover;
u8 g_SeqActionCond;

// Configuration
u8 g_SeqFrameWait = 6;
SeqDrawCB g_SeqDrawCB;
const SeqAction* g_SeqActionMoveLeft;
const SeqAction* g_SeqActionMoveRight;
Sequence g_SeqTransition;

#if (SEQ_USE_TIMELINE)
const SeqTime** g_SeqTimelines;
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
	Sequence_UpdatePanBound,
	Sequence_UpdatePanLoop,
#if (SEQ_USE_TIMELINE)
	Sequence_UpdateTimeline
#endif
};

// Cursor acceleration over time
const u8 g_seqCursorMoveSpd[SEQ_CURSOR_SPD_TABLE] = {	1,  0,  0,  1,  1,  1,  1,  1,  
														1,  1,  1,  1,  1,  1,  1,  1,  
														2,  2,  2,  2,  2,  2,  2,  2,  
														2,  2,  3,  3,  3,  3,  4,  4 };

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
	g_SeqCur = seq;

#if (SEQ_USE_TIMELINE)
	if (g_SeqCur->Mode == SEQ_MODE_TIMELINE)
	{
		g_SeqTimelineTimer = 0;
		g_SeqFrame = frame;
		g_SeqDrawCB(g_SeqTimelines[g_SeqCur->FirstFrame][frame].Frame);
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
		g_SeqDrawCB(g_SeqFrame);
	}

	g_SeqCur->EventCB(SEQ_EVENT_START);
}

//-----------------------------------------------------------------------------
// Start a pan sequence transition
void Sequence_PlayPanTransition(u8 from, struct Sequence* nextSeq, u8 nextFrame)
{
	if ((g_SeqCur->Mode == SEQ_MODE_PAN_BOUND) || (g_SeqCur->Mode == SEQ_MODE_PAN_LOOP))
	{
		g_SeqTransition.ID         = g_SeqCur->ID;
		g_SeqTransition.Mode       = (from > g_SeqFrame) ? SEQ_MODE_ONCE : SEQ_MODE_ONCE_REVERT;
		g_SeqTransition.FirstFrame = g_SeqFrame;
		g_SeqTransition.LastFrame  = from;
		g_SeqTransition.EventCB    = g_SeqCur->EventCB;
		g_SeqTransition.Next.Seq   = nextSeq;
		g_SeqTransition.Next.Frame = nextFrame;
		g_SeqTransition.ActionNum  = 0;
		Sequence_Play(&g_SeqTransition, 0);
	}
}

//-----------------------------------------------------------------------------
// Check if cursor is into the giben area
bool Sequence_CheckArea(const SeqActionArea* area)
{
	if ((g_SeqCursorPosX >= area->StartX) && (g_SeqCursorPosX <= area->EndX) && (g_SeqCursorPosY >= area->StartY) && (g_SeqCursorPosY <= area->EndY))
		return TRUE;
	return FALSE;
}

//-----------------------------------------------------------------------------
// Check all current sequence's actions
void Sequence_CheckActions()
{
	for (u8 i = 0; i < g_SeqCur->ActionNum; i++)
	{
		const SeqAction* const act = g_SeqCur->Actions[i];
		if (act->FrameMin == SEQ_FRAME_ALL)
		{
			if (Sequence_CheckArea(&act->Areas[0]))
			{
				g_SeqActionHover = act;
				if (act->Condition)
					g_SeqActionCond = act->Condition(act->Id); 
			}
		}
		if ((g_SeqFrame >= act->FrameMin) && (g_SeqFrame <= act->FrameMax))
		{
			if (Sequence_CheckArea(&act->Areas[g_SeqFrame - act->FrameMin]))
			{
				g_SeqActionHover = act;
				if (act->Condition)
					g_SeqActionCond = act->Condition(act->Id); 
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Apply event linked to click on area
void Sequence_ApplyClick()
{
	if (g_SeqActionHover && (g_SeqActionHover->Action == SEQ_ACT_CLICK_AREA) && (g_SeqActionCond == SEQ_COND_OK))
	{
		g_SeqCur->EventCB(g_SeqActionHover->Id);
		const SeqTransition* trans = &g_SeqActionHover->Trans;
		if (trans->Seq)
		{
			if (trans->From == SEQ_DIRECT)
				Sequence_Play(trans->Seq, trans->Frame);
			else
				Sequence_PlayPanTransition(trans->From, trans->Seq, trans->Frame);
		}
	}
}

//-----------------------------------------------------------------------------
// Update sequence for mode SEQ_MODE_FIXED
void Sequence_UpdateFixed()
{
	Sequence_CheckActions();
	if (g_SeqInput & SEQ_INPUT_CLICK_1)
		Sequence_ApplyClick();
}

//-----------------------------------------------------------------------------
// Update sequence for mode SEQ_MODE_ONCE
void Sequence_UpdateOnce()
{
	if (g_SeqFrame < g_SeqCur->LastFrame)
	{
		g_SeqFrame++;
		g_SeqDrawCB(g_SeqFrame);
		if (g_SeqFrame == g_SeqCur->LastFrame)
		{
			g_SeqCur->EventCB(SEQ_EVENT_END);
			if (g_SeqCur->Next.Seq)
				Sequence_Play(g_SeqCur->Next.Seq, g_SeqCur->Next.Frame);
		}
	}

	Sequence_CheckActions();
	if (g_SeqInput & SEQ_INPUT_CLICK_1)
		Sequence_ApplyClick();
}

//-----------------------------------------------------------------------------
// Update sequence for mode SEQ_MODE_LOOP
void Sequence_UpdateLoop()
{
	g_SeqFrame++;
	if (g_SeqFrame > g_SeqCur->LastFrame)
	{
		g_SeqFrame = g_SeqCur->FirstFrame;
		g_SeqDrawCB(g_SeqFrame);
		g_SeqCur->EventCB(SEQ_EVENT_LOOP);
	}
	else
		g_SeqDrawCB(g_SeqFrame);

	Sequence_CheckActions();
	if (g_SeqInput & SEQ_INPUT_CLICK_1)
		Sequence_ApplyClick();
}

//-----------------------------------------------------------------------------
// Update sequence for mode SEQ_MODE_LOOP_REVERT
void Sequence_UpdateLoopRevert()
{
	if (g_SeqFrame == g_SeqCur->LastFrame)
	{
		g_SeqFrame = g_SeqCur->FirstFrame;
		g_SeqCur->EventCB(SEQ_EVENT_LOOP);
	}
	else
		g_SeqFrame--;
	g_SeqDrawCB(g_SeqFrame);

	Sequence_CheckActions();
	if (g_SeqInput & SEQ_INPUT_CLICK_1)
		Sequence_ApplyClick();
}

//-----------------------------------------------------------------------------
// Update sequence for mode SEQ_MODE_ONCE_REVERT
void Sequence_UpdateOnceRevert()
{
	if (g_SeqFrame > g_SeqCur->LastFrame)
	{
		g_SeqFrame--;
		g_SeqDrawCB(g_SeqFrame);
	}
	else
	{
		g_SeqCur->EventCB(SEQ_EVENT_END);
		if (g_SeqCur->Next.Seq)
			Sequence_Play(g_SeqCur->Next.Seq, g_SeqCur->Next.Frame);
	}

	Sequence_CheckActions();
	if (g_SeqInput & SEQ_INPUT_CLICK_1)
		Sequence_ApplyClick();
}

//-----------------------------------------------------------------------------
// Update sequence for mode SEQ_MODE_PAN_BOUND
void Sequence_UpdatePanBound()
{
}

//-----------------------------------------------------------------------------
// Update sequence for mode SEQ_MODE_PAN_LOOP
void Sequence_UpdatePanLoop()
{
	// Check area
	if (g_SeqInput & SEQ_INPUT_MOVE_LEFT)
		g_SeqActionHover = g_SeqActionMoveLeft;
	else if (g_SeqInput & SEQ_INPUT_MOVE_RIGHT)
		g_SeqActionHover = g_SeqActionMoveRight;
	else if (Sequence_CheckArea(&(g_SeqActionMoveLeft->Areas[0])))
		g_SeqActionHover = g_SeqActionMoveLeft;
	else if (Sequence_CheckArea(&(g_SeqActionMoveRight->Areas[0])))
		g_SeqActionHover = g_SeqActionMoveRight;
	else
		Sequence_CheckActions();

	if (g_SeqActionHover)
	{
		switch (g_SeqActionHover->Action)
		{
		case SEQ_ACT_CLICK_AREA:
			if (g_SeqInput & SEQ_INPUT_CLICK_1)
				Sequence_ApplyClick();
			break;

		case SEQ_ACT_CLICK_RIGHT:
			if ((g_SeqInput & SEQ_INPUT_PRESS_1) || (g_SeqInput & SEQ_INPUT_MOVE_RIGHT))
			{
				if (g_SeqFrame < g_SeqCur->LastFrame)
					g_SeqFrame++;
				else
					g_SeqFrame = g_SeqCur->FirstFrame;
				g_SeqDrawCB(g_SeqFrame);
			}
			break;

		case SEQ_ACT_CLICK_LEFT:
			if ((g_SeqInput & SEQ_INPUT_PRESS_1) || (g_SeqInput & SEQ_INPUT_MOVE_LEFT))
			{
				if (g_SeqFrame > g_SeqCur->FirstFrame)
					g_SeqFrame--;
				else
					g_SeqFrame = g_SeqCur->LastFrame;
				g_SeqDrawCB(g_SeqFrame);
			}
			break;
		}
	}
}

#if (SEQ_USE_TIMELINE)
//-----------------------------------------------------------------------------
// Update sequence for mode SEQ_MODE_TIMELINE
void Sequence_UpdateTimeline()
{
	if (g_SeqFrame < g_SeqCur->LastFrame)
	{
		const SeqTime* timeline = g_SeqTimelines[g_SeqCur->FirstFrame];

		g_SeqTimelineTimer++;
		if (g_SeqTimelineTimer >= timeline[g_SeqFrame].Time)
		{
			g_SeqTimelineTimer = 0;
			g_SeqFrame++;
			g_SeqDrawCB(timeline[g_SeqFrame].Frame);
			if (g_SeqFrame == g_SeqCur->LastFrame)
			{
				g_SeqCur->EventCB(SEQ_EVENT_END);
				if (g_SeqCur->Next.Seq)
					Sequence_Play(g_SeqCur->Next.Seq, g_SeqCur->Next.Frame);
			}
		}
	}

	Sequence_CheckActions();
	if (g_SeqInput & SEQ_INPUT_CLICK_1)
		Sequence_ApplyClick();
}
#endif

//-----------------------------------------------------------------------------
//
void Sequence_UpdateInput()
{
	// Update mouse
	Mouse_Read(MOUSE_PORT_1, &g_SeqMouseData);

	// Update keyboard
	u8 row3 = Keyboard_Read(3); // KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J
	u8 row5 = Keyboard_Read(5); // KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z
	u8 row8 = Keyboard_Read(8); // KEY_SPACE, KEY_HOME, KEY_INS, KEY_DEL, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_RIGHT

	// Update cursor X coordinate
	g_SeqCursorPosX += Mouse_GetOffsetX(&g_SeqMouseData) / SEQ_MOUSE_SPEED;
	if (IS_KEY_PRESSED(row8, KEY_RIGHT))
	{
		if (g_SeqCursorAccX < (SEQ_CURSOR_SPD_TABLE - 1))
			g_SeqCursorAccX++;
		g_SeqCursorPosX += g_SeqCursorAccX;
		// g_SeqCursorPosX += g_seqCursorMoveSpd[g_SeqCursorAccX];
	}
	else if (IS_KEY_PRESSED(row8, KEY_LEFT))
	{
		if  (g_SeqCursorAccX < (SEQ_CURSOR_SPD_TABLE - 1))
			g_SeqCursorAccX++;
		g_SeqCursorPosX -= g_SeqCursorAccX;
		// g_SeqCursorPosX -= g_seqCursorMoveSpd[g_SeqCursorAccX];
	}
	else
		g_SeqCursorAccX = 0;

	// Update cursor Y coordinate
	g_SeqCursorPosY += Mouse_GetOffsetY(&g_SeqMouseData) / SEQ_MOUSE_SPEED;
	if (IS_KEY_PRESSED(row8, KEY_DOWN))
	{
		if (g_SeqCursorAccY < (SEQ_CURSOR_SPD_TABLE - 1))
			g_SeqCursorAccY++;
		g_SeqCursorPosY += g_SeqCursorAccY;
		// g_SeqCursorPosY += g_seqCursorMoveSpd[g_SeqCursorAccY];
	}
	else if (IS_KEY_PRESSED(row8, KEY_UP))
	{
		if (g_SeqCursorAccY < (SEQ_CURSOR_SPD_TABLE - 1))
			g_SeqCursorAccY++;
		g_SeqCursorPosY -= g_SeqCursorAccY;
		// g_SeqCursorPosY -= g_seqCursorMoveSpd[g_SeqCursorAccY];
	}
	else
		g_SeqCursorAccY = 0;

	// Update cursor click states
	if (Mouse_IsButtonClick(&g_SeqMouseData, MOUSE_BOUTON_LEFT) || IS_KEY_PUSHED(row8, g_SeqPrevRaw8, KEY_SPACE))
		g_SeqInput |= SEQ_INPUT_CLICK_1;
	if (Mouse_IsButtonClick(&g_SeqMouseData, MOUSE_BOUTON_RIGHT) || IS_KEY_PUSHED(row3, g_SeqPrevRaw8, KEY_C))
		g_SeqInput |= SEQ_INPUT_CLICK_2;

		// Update cursor click states
	if (Mouse_IsButtonPress(&g_SeqMouseData, MOUSE_BOUTON_LEFT) || IS_KEY_PRESSED(row8, KEY_SPACE))
		g_SeqInput |= SEQ_INPUT_PRESS_1;
	if (Mouse_IsButtonPress(&g_SeqMouseData, MOUSE_BOUTON_RIGHT) || IS_KEY_PRESSED(row3, KEY_C))
		g_SeqInput |= SEQ_INPUT_PRESS_2;

	if (IS_KEY_PRESSED(row3, KEY_F))
		g_SeqInput |= SEQ_INPUT_MOVE_RIGHT;
	else if (IS_KEY_PRESSED(row5, KEY_S))
		g_SeqInput |= SEQ_INPUT_MOVE_LEFT;
	
	if (IS_KEY_PRESSED(row3, KEY_E))
		g_SeqInput |= SEQ_INPUT_MOVE_UP;
	else if (IS_KEY_PRESSED(row3, KEY_D))
		g_SeqInput |= SEQ_INPUT_MOVE_DOWN;

	g_SeqPrevRaw8 = row8;
}

//-----------------------------------------------------------------------------
//
void Sequence_UpdateCursor()
{
	// Update cursor
	if (g_SeqInput & SEQ_INPUT_CLICK_2) // Cancel custom cursor
		Sequence_ClearCustomCursor();

	u8 pat = SEQ_CUR_DEFAULT;
	if (Sequence_HasCustomCursor())
		pat = g_SeqCustomCursor;
	if (g_SeqActionHover)
	{
		switch (g_SeqActionCond)
		{
		case SEQ_COND_OK:
			pat = g_SeqActionHover->Cursor * 8;
			break;

		case SEQ_COND_LOCK:
			pat = SEQ_CUR_LOCK * 8;
			break;
		}
	}

	g_VDP_Sprite.Y = g_SeqCursorPosY - 8;
	g_VDP_Sprite.X = g_SeqCursorPosX - 7;
	g_VDP_Sprite.Pattern = pat;
	g_VDP_Sprite.Color = 0;
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, g_SpriteAtributeLow, g_SpriteAtributeHigh, 3);
	g_VDP_Sprite.Pattern += 4;
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, g_SpriteAtributeLow + 4, g_SpriteAtributeHigh, 3);
}

//-----------------------------------------------------------------------------
//
void Sequence_Update()
{
	g_SeqActionHover = NULL;
	g_SeqActionCond = SEQ_COND_OK;

	// Update input
	Sequence_UpdateInput();

	// Update sequence (mode specific function)
	g_SeqUpdateModes[g_SeqCur->Mode]();	

	// Update cursor
	Sequence_UpdateCursor();

	g_SeqInput = 0;
}