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
#include "math.h"
#include "game/sequence.h"

//=============================================================================
// DEFINES
//=============================================================================

// Functions prototypes
void Sequence_UpdateFixed();
void Sequence_UpdateOnce();
void Sequence_UpdateLoop();
void Sequence_UpdateBackOnce();
void Sequence_UpdateBackLoop();
void Sequence_UpdatePanBound();
void Sequence_UpdatePanLoop();

//=============================================================================
// VARIABLES
//=============================================================================

u8 g_SeqFrameCount = 0;				// Render frame count

const struct Sequence* g_SeqCur;	// Current sequence
u16 g_SeqFrame = 0;					// Current sequence's frame

// Cursor
Mouse_State g_SeqMouseData;
u8 g_SeqCursorPosX;
u8 g_SeqCursorPosY;
i8 g_SeqCursorAccX;
i8 g_SeqCursorAccY;
u8 g_SeqInput;

const struct SeqAction* g_SeqActionHover;
u8 g_SeqActionCond;

// Configuration
u8 g_SeqFrameWait = 6;
SeqDrawCB g_SeqDrawCB;
const struct SeqAction* g_SeqActionMoveLeft;
const struct SeqAction* g_SeqActionMoveRight;

//=============================================================================
// READ-ONLY DATA
//=============================================================================

const callback g_SeqUpdateModes[SEQ_MODE_MAX] =
{
	Sequence_UpdateFixed,
	Sequence_UpdateOnce,
	Sequence_UpdateLoop,
	Sequence_UpdateBackOnce,
	Sequence_UpdateBackLoop,
	Sequence_UpdatePanBound,
	Sequence_UpdatePanLoop
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
	while(g_SeqFrameCount < g_SeqFrameWait)
	{
	}
	g_SeqFrameCount = 0;
}

//-----------------------------------------------------------------------------
// 
void Sequence_Play(const struct Sequence* seq, u16 frame)
{
	g_SeqCur = seq;
	u16 min = MIN(g_SeqCur->FirstFrame, g_SeqCur->LastFrame);
	u16 max = MAX(g_SeqCur->FirstFrame, g_SeqCur->LastFrame);
	if((frame >= min) && (frame <= max))
		g_SeqFrame = frame;
	else
		g_SeqFrame = seq->FirstFrame;

	g_SeqDrawCB(g_SeqFrame);
	g_SeqCur->EventCB(SEQ_EVENT_START);
}

//-----------------------------------------------------------------------------
// 
bool Sequence_CheckArea(const struct SeqActionArea* area)
{
	if((g_SeqCursorPosX >= area->StartX) && (g_SeqCursorPosX <= area->EndX) && (g_SeqCursorPosY >= area->StartY) && (g_SeqCursorPosY <= area->EndY))
		return TRUE;
	return FALSE;
}

//-----------------------------------------------------------------------------
// 
void Sequence_CheckActions()
{
	for(u8 i = 0; i < g_SeqCur->ActionNum; i++)
	{
		const struct SeqAction* const act = g_SeqCur->Actions[i];
		if((g_SeqFrame >= act->FrameMin) && (g_SeqFrame <= act->FrameMax))
		{
			if(Sequence_CheckArea(&act->Areas[g_SeqFrame - act->FrameMin]))
			{
				g_SeqActionHover = act;
				if(act->Condition)
					g_SeqActionCond = act->Condition(act->Id); 
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
void Sequence_UpdateFixed()
{
	Sequence_CheckActions();
	if((g_SeqInput & SEQ_INPUT_CLICK_1) && g_SeqActionHover && (g_SeqActionHover->Action == SEQ_ACT_CLICK_AREA))
		g_SeqCur->EventCB(g_SeqActionHover->Id);
}

//-----------------------------------------------------------------------------
//
void Sequence_UpdateOnce()
{
	if(g_SeqFrame < g_SeqCur->LastFrame)
	{
		g_SeqFrame++;
		g_SeqDrawCB(g_SeqFrame);
		if(g_SeqFrame == g_SeqCur->LastFrame)
			g_SeqCur->EventCB(SEQ_EVENT_END);
	}

	Sequence_CheckActions();
	if((g_SeqInput & SEQ_INPUT_CLICK_1) && g_SeqActionHover && (g_SeqActionHover->Action == SEQ_ACT_CLICK_AREA))
		g_SeqCur->EventCB(g_SeqActionHover->Id);
}

//-----------------------------------------------------------------------------
//
void Sequence_UpdateLoop()
{
	g_SeqFrame++;
	if(g_SeqFrame > g_SeqCur->LastFrame)
	{
		g_SeqFrame = g_SeqCur->FirstFrame;
		g_SeqDrawCB(g_SeqFrame);
		g_SeqCur->EventCB(SEQ_EVENT_LOOP);
	}
	else
		g_SeqDrawCB(g_SeqFrame);

	Sequence_CheckActions();
	if((g_SeqInput & SEQ_INPUT_CLICK_1) && g_SeqActionHover && (g_SeqActionHover->Action == SEQ_ACT_CLICK_AREA))
		g_SeqCur->EventCB(g_SeqActionHover->Id);
}

//-----------------------------------------------------------------------------
//
void Sequence_UpdateBackOnce()
{
	if(g_SeqFrame > g_SeqCur->LastFrame)
	{
		g_SeqFrame--;
		g_SeqDrawCB(g_SeqFrame);
	}
	else
		g_SeqCur->EventCB(SEQ_EVENT_END);

	Sequence_CheckActions();
	if((g_SeqInput & SEQ_INPUT_CLICK_1) && g_SeqActionHover && (g_SeqActionHover->Action == SEQ_ACT_CLICK_AREA))
		g_SeqCur->EventCB(g_SeqActionHover->Id);
}

//-----------------------------------------------------------------------------
//
void Sequence_UpdateBackLoop()
{
	if(g_SeqFrame == g_SeqCur->LastFrame)
	{
		g_SeqFrame = g_SeqCur->FirstFrame;
		g_SeqCur->EventCB(SEQ_EVENT_LOOP);
	}
	else
		g_SeqFrame--;
	g_SeqDrawCB(g_SeqFrame);

	Sequence_CheckActions();
	if((g_SeqInput & SEQ_INPUT_CLICK_1) && g_SeqActionHover && (g_SeqActionHover->Action == SEQ_ACT_CLICK_AREA))
		g_SeqCur->EventCB(g_SeqActionHover->Id);
}

//-----------------------------------------------------------------------------
//
void Sequence_UpdatePanBound()
{
}

//-----------------------------------------------------------------------------
//
void Sequence_UpdatePanLoop()
{
	// Check area
	if(g_SeqInput & SEQ_INPUT_MOVE_LEFT)
		g_SeqActionHover = g_SeqActionMoveLeft;
	else if(g_SeqInput & SEQ_INPUT_MOVE_RIGHT)
		g_SeqActionHover = g_SeqActionMoveRight;
	else if(Sequence_CheckArea(&(g_SeqActionMoveLeft->Areas[0])))
		g_SeqActionHover = g_SeqActionMoveLeft;
	else if(Sequence_CheckArea(&(g_SeqActionMoveRight->Areas[0])))
		g_SeqActionHover = g_SeqActionMoveRight;
	else
		Sequence_CheckActions();

	if(g_SeqActionHover)
	{
		switch(g_SeqActionHover->Action)
		{
		case SEQ_ACT_CLICK_AREA:
			if((g_SeqInput & SEQ_INPUT_CLICK_1) && (g_SeqActionCond == SEQ_COND_OK))
				g_SeqCur->EventCB(g_SeqActionHover->Id);
			break;

		case SEQ_ACT_CLICK_RIGHT:
			if((g_SeqInput & SEQ_INPUT_CLICK_1) || (g_SeqInput & SEQ_INPUT_MOVE_RIGHT))
			{
				g_SeqFrame++;
				if(g_SeqFrame > g_SeqCur->LastFrame)
					g_SeqFrame = g_SeqCur->FirstFrame;
				g_SeqDrawCB(g_SeqFrame);
			}
			break;

		case SEQ_ACT_CLICK_LEFT:
			if((g_SeqInput & SEQ_INPUT_CLICK_1) || (g_SeqInput & SEQ_INPUT_MOVE_LEFT))
			{
				g_SeqFrame--;
				if(g_SeqFrame < g_SeqCur->FirstFrame)
					g_SeqFrame = g_SeqCur->LastFrame;
				g_SeqDrawCB(g_SeqFrame);
			}
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//
void Sequence_UpdateInput()
{
	g_SeqInput = 0;

	// Update mouse
	Mouse_Read(MOUSE_PORT_1, &g_SeqMouseData);

	// Update keyboard
	u8 row3 = Keyboard_Read(3); // KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J
	u8 row5 = Keyboard_Read(5); // KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z
	u8 row8 = Keyboard_Read(8); // KEY_SPACE, KEY_HOME, KEY_INS, KEY_DEL, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_RIGHT

	// Update cusror X coordinate
	g_SeqCursorPosX += Mouse_GetOffsetX(&g_SeqMouseData) / 2;
	if(IS_KEY_PRESSED(row8, KEY_RIGHT))
	{
		if(g_SeqCursorAccX > 0)
			g_SeqCursorAccX++;
		else
			g_SeqCursorAccX = 1;
	}
	else if(IS_KEY_PRESSED(row8, KEY_LEFT))
	{
		if(g_SeqCursorAccX < 0)
			g_SeqCursorAccX--;
		else
			g_SeqCursorAccX = -1;
	}
	else
		g_SeqCursorAccX = 0;
	g_SeqCursorPosX += g_SeqCursorAccX;

	// Update cusror Y coordinate
	g_SeqCursorPosY += Mouse_GetOffsetY(&g_SeqMouseData) / 2;
	if(IS_KEY_PRESSED(row8, KEY_DOWN))
	{
		if(g_SeqCursorAccY > 0)
			g_SeqCursorAccY++;
		else
			g_SeqCursorAccY = 1;
	}
	else if(IS_KEY_PRESSED(row8, KEY_UP))
	{
		if(g_SeqCursorAccY < 0)
			g_SeqCursorAccY--;
		else
			g_SeqCursorAccY = -1;
	}
	else
		g_SeqCursorAccY = 0;

	g_SeqCursorPosY += g_SeqCursorAccY;

	// Update cursor click state
	if(Mouse_IsButtonPress(&g_SeqMouseData, MOUSE_BOUTON_LEFT) || IS_KEY_PRESSED(row8, KEY_SPACE))
		g_SeqInput |= SEQ_INPUT_CLICK_1;

	if(IS_KEY_PRESSED(row3, KEY_F))
		g_SeqInput |= SEQ_INPUT_MOVE_RIGHT;
	else if(IS_KEY_PRESSED(row5, KEY_S))
		g_SeqInput |= SEQ_INPUT_MOVE_LEFT;
	
	if(IS_KEY_PRESSED(row3, KEY_E))
		g_SeqInput |= SEQ_INPUT_MOVE_UP;
	else if(IS_KEY_PRESSED(row3, KEY_D))
		g_SeqInput |= SEQ_INPUT_MOVE_DOWN;
}

//-----------------------------------------------------------------------------
//
void Sequence_Update()
{
	Sequence_UpdateInput();

	g_SeqActionHover = NULL;
	g_SeqActionCond = SEQ_COND_OK;

	g_SeqUpdateModes[g_SeqCur->Mode]();	

	// Update cursor
	u8 pat = SEQ_CUR_DEFAULT;
	if(g_SeqActionHover)
	{
		switch(g_SeqActionCond)
		{
		case SEQ_COND_OK:
			pat = g_SeqActionHover->Cursor * 8;
			break;

		case SEQ_COND_LOCK:
			pat = SEQ_CUR_LOCK * 8;
			break;
		}
	}

	g_VDP_Sprite.Y = g_SeqCursorPosY - 7;
	g_VDP_Sprite.X = g_SeqCursorPosX - 7;
	g_VDP_Sprite.Pattern = pat;
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, g_SpriteAtributeLow, g_SpriteAtributeHigh, 3);
	g_VDP_Sprite.Pattern += 4;
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, g_SpriteAtributeLow + 4, g_SpriteAtributeHigh, 3);
}