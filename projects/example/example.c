// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Game module sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"
#include "game.h"
#include "game_pawn.h"
#include "math.h"
#include "debug.h"
#include "string.h"
#include "version.h"

//=============================================================================
// DEFINES
//=============================================================================

#define FORCE		20
#define GRAVITY		1
#define GROUND		192
#define HORIZON		11

// Prototype
bool State_Initialize();
bool State_Game();
bool State_Pause();
void UpdateScore();

//
struct Character
{
	bool bMoving;
	bool bInAir;
	i8   VelocityY;
	i8   DX;
	i8   DY;
	u8   Input;
	u8   Score;
	Game_Pawn Pawn;
};

//
enum INPUT_ACTION
{
	INPUT_RIGHT = 0b00000001,
	INPUT_LEFT  = 0b00000010,
	INPUT_JUMP  = 0b00000100,
};

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font\font_carwar.h"
// Sprites by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "..\samples\content\data_sprt_layer.h"
#include "..\samples\content\data_sprt_ball.h"
#include "..\samples\content\data_bg.h"

//.............................................................................
// Player 1 data

// Pawn sprite layers
const Game_Sprite g_SpriteLayers[] =
{
	{ 0, 0, 0, 0,  COLOR_BLACK, PAWN_SPRITE_EVEN },
	{ 0, 0, 0, 12, COLOR_BLACK, PAWN_SPRITE_ODD },
	{ 4, 0, 0, 8,  COLOR_LIGHT_RED, 0 },
};

// Idle animation frames
const Game_Frame g_FramesIdle[] =
{
	{ 4*16,	64,	NULL },
	{ 2*16,	24,	NULL },
};

// Move animation frames
const Game_Frame g_FramesMove[] =
{
	{ 0*16,	4,	NULL },
	{ 1*16,	4,	NULL },
	{ 2*16,	4,	NULL },
	{ 3*16,	4,	NULL },
};

// Jump animation frames
const Game_Frame g_FramesJump[] =
{
	{ 5*16,	4,	NULL },
};

// Fall animation frames
const Game_Frame g_FramesFall[] =
{
	{ 6*16,	4,	NULL },
};

// Actions id
enum ANIM_ACTION_ID
{
	ACTION_IDLE = 0,
	ACTION_MOVE,
	ACTION_JUMP,
	ACTION_FALL,
};

// List of all player actions
const Game_Action g_AnimActions[] =
{ //  Frames        Number                  Loop?  Interrupt?
	{ g_FramesIdle, numberof(g_FramesIdle), TRUE,  TRUE },
	{ g_FramesMove, numberof(g_FramesMove), TRUE,  TRUE },
	{ g_FramesJump, numberof(g_FramesJump), TRUE,  TRUE },
	{ g_FramesFall, numberof(g_FramesFall), TRUE,  TRUE },
};

//.............................................................................
// Player 2 data

// Pawn sprite layers
const Game_Sprite g_SpriteLayers2[] =
{
	{ 1, 0, 0, 0,  COLOR_BLACK, PAWN_SPRITE_EVEN },
	{ 1, 0, 0, 12, COLOR_BLACK, PAWN_SPRITE_ODD },
	{ 5, 0, 0, 8,  COLOR_LIGHT_RED, 0 },
};

// Idle animation frames
const Game_Frame g_FramesIdle2[] =
{
	{ 4*16+7*16,	64,	NULL },
	{ 2*16+7*16,	24,	NULL },
};

// Move animation frames
const Game_Frame g_FramesMove2[] =
{
	{ 0*16+7*16,	4,	NULL },
	{ 1*16+7*16,	4,	NULL },
	{ 2*16+7*16,	4,	NULL },
	{ 3*16+7*16,	4,	NULL },
};

// Jump animation frames
const Game_Frame g_FramesJump2[] =
{
	{ 5*16+7*16,	4,	NULL },
};

// Fall animation frames
const Game_Frame g_FramesFall2[] =
{
	{ 6*16+7*16,	4,	NULL },
};

// List of all player actions
const Game_Action g_AnimActions2[] =
{ //  Frames        Number                  Loop?  Interrupt?
	{ g_FramesIdle2, numberof(g_FramesIdle2), TRUE,  TRUE },
	{ g_FramesMove2, numberof(g_FramesMove2), TRUE,  TRUE },
	{ g_FramesJump2, numberof(g_FramesJump2), TRUE,  TRUE },
	{ g_FramesFall2, numberof(g_FramesFall2), TRUE,  TRUE },
};

//.............................................................................
// Ball data

// Pawn sprite layers
const Game_Sprite g_BallLayers[] =
{
	{ 2, 0, 0, 4,  COLOR_DARK_RED, 0 },
	{ 3, 0, 0, 0,  COLOR_MEDIUM_RED, 0 },
};

// Idle animation frames
const Game_Frame g_BallIdle[] =
{
	{ 0*8+224,	4,	NULL },
};

// Idle animation frames
const Game_Frame g_BallBump[] =
{
	{ 1*8+224,	1,	NULL },
	{ 2*8+224,	7,	NULL },
	{ 1*8+224,	4,	NULL },
};

// Actions id
enum ANIM_ACTION_BALL_ID
{
	ACTION_BALL_IDLE = 0,
	ACTION_BALL_BUMP,
};

// List of all player actions
const Game_Action g_BallActions[] =
{ //  Frames      Number                Loop?  Interrupt?
	{ g_BallIdle, numberof(g_BallIdle), TRUE,  TRUE },
	{ g_BallBump, numberof(g_BallBump), FALSE, TRUE },
};


//=============================================================================
// MEMORY DATA
//=============================================================================

struct Character g_Player1;
struct Character g_Player2;
struct Character g_Ball;

bool g_bFlicker = TRUE;
u8   g_PrevRow3 = 0xFF;
u8   g_PrevRow8 = 0xFF;

c8 g_StrBuffer[16];

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Physics callback
void PhysicsEventPlayer1(u8 event, u8 tile)
{
	switch(event)
	{
	case PAWN_PHYSICS_COL_DOWN: // Handle downward collisions 
		g_Player1.bInAir = FALSE;
		break;

	case PAWN_PHYSICS_FALL: // Handle falling
		if(!g_Player1.bInAir)
		{
			g_Player1.bInAir = TRUE;
			g_Player1.VelocityY = 0;
		}
		break;
	};
}

//-----------------------------------------------------------------------------
// Physics callback
void PhysicsEventPlayer2(u8 event, u8 tile)
{
	switch(event)
	{
	case PAWN_PHYSICS_COL_DOWN: // Handle downward collisions 
		g_Player2.bInAir = FALSE;
		break;

	case PAWN_PHYSICS_FALL: // Handle falling
		if(!g_Player2.bInAir)
		{
			g_Player2.bInAir = TRUE;
			g_Player2.VelocityY = 0;
		}
		break;
	};
}

//-----------------------------------------------------------------------------
// Physics callback
void PhysicsEventBall(u8 event, u8 tile)
{
	switch(event)
	{
	case PAWN_PHYSICS_COL_DOWN: // Handle downward collisions 
		g_Ball.VelocityY = -g_Ball.VelocityY;
		GamePawn_SetAction(&g_Ball.Pawn, ACTION_BALL_BUMP);
		if(g_Ball.Pawn.PositionY > 128)
		{
			if(g_Ball.Pawn.PositionX < 128) // Player 1 score a point!
			{
				if(g_Player1.Score < 99)
					g_Player1.Score++;
				UpdateScore();
			}
			else // Player 2 score a point!
			{
				if(g_Player2.Score < 99)
					g_Player2.Score++;
				UpdateScore();
			}
		}
		break;

	case PAWN_PHYSICS_COL_RIGHT: // Handle net
	case PAWN_PHYSICS_COL_LEFT:
	case PAWN_PHYSICS_BORDER_RIGHT: // Handle border
	case PAWN_PHYSICS_BORDER_LEFT:
		g_Ball.DX = -g_Ball.DX;
		break;

	case PAWN_PHYSICS_FALL: // Handle falling
		if(!g_Ball.bInAir)
		{
			g_Ball.bInAir = TRUE;
			g_Ball.VelocityY = 0;
		}
		break;
	};
}

//-----------------------------------------------------------------------------
// Collision callback
bool PhysicsCollision(u8 tile)
{
	return (tile < 8);
}

//-----------------------------------------------------------------------------
//
void UpdateScore()
{
	String_Format(g_StrBuffer, "%02i-%02i", g_Player2.Score, g_Player1.Score);
	Print_SetPosition(16, 0);
	Print_DrawText(g_StrBuffer);
}

//-----------------------------------------------------------------------------
//
void DrawLevel()
{
	// Background
	loop(i, 24-1)
		VDP_FillVRAM((i <= HORIZON) ? 16 : 8, g_ScreenLayoutLow + (i+1) * 32, 0, 32);

	// Ground
	VDP_FillVRAM(1, g_ScreenLayoutLow + 23 * 32, 0, 32);

	// "Net"
	loop(i, 7)
	{
		u16 addr = g_ScreenLayoutLow + 15 + (i + (23 - 7)) * 32;
		VDP_Poke_16K((i == 0) ? 0 : 5, addr++);
		VDP_Poke_16K((i == 0) ? 2 : 6, addr);
	}
}

//-----------------------------------------------------------------------------
//
void InitPlayer(struct Character* ply, u8 id)
{
	ply->bMoving = FALSE;
	ply->bInAir = FALSE;
	ply->VelocityY = 0;
	ply->Score = 0;

	Game_Pawn* pawn = &ply->Pawn;
	if(id == 0)
		GamePawn_Initialize(pawn, g_SpriteLayers, numberof(g_SpriteLayers), 0, g_AnimActions);
	else
		GamePawn_Initialize(pawn, g_SpriteLayers2, numberof(g_SpriteLayers2), 0, g_AnimActions2);

	GamePawn_SetPosition(pawn, (id == 0) ? 255 - 16 - 16 : 16, 128);
	GamePawn_InitializePhysics(pawn, (id == 0) ? PhysicsEventPlayer1 : PhysicsEventPlayer2, PhysicsCollision, 16, 16);
}

//-----------------------------------------------------------------------------
//
void UpdatePlayer(struct Character* ply)
{
	// Update movement
	ply->DX = 0;
	ply->DY = 0;
	
	if(ply->Input & INPUT_RIGHT)
	{
		ply->DX++;
		ply->bMoving = TRUE;
	}
	else if(ply->Input & INPUT_LEFT)
	{
		ply->DX--;
		ply->bMoving = TRUE;
	}
	else
		ply->bMoving = FALSE;

	if(ply->bInAir) // Jump/fall
	{
		ply->DY -= ply->VelocityY / 4;
		ply->VelocityY -= GRAVITY;
		if(ply->VelocityY < -FORCE)
			ply->VelocityY = -FORCE;
	}
	else if(ply->Input & INPUT_JUMP)
	{
		ply->bInAir = TRUE;
		ply->VelocityY = FORCE;
	}

	// Update player animation & physics
	u8 act = ACTION_IDLE;
	if(ply->bInAir && (ply->VelocityY >= 0))
		act = ACTION_JUMP;
	else if(ply->bInAir)
		act = ACTION_FALL;
	else if(ply->bMoving)
		act = ACTION_MOVE;

	Game_Pawn* pawn = &ply->Pawn;
	GamePawn_SetAction(pawn, act);
	GamePawn_SetMovement(pawn, ply->DX, ply->DY);
	GamePawn_Update(pawn);
	// GamePawn_Draw(pawn);
}

//-----------------------------------------------------------------------------
//
void InitBall()
{
	Game_Pawn* pawn = &g_Ball.Pawn;
	GamePawn_Initialize(pawn, g_BallLayers, numberof(g_BallLayers), 6, g_BallActions);
	GamePawn_SetPosition(pawn, 150, 128);
	GamePawn_InitializePhysics(pawn, PhysicsEventBall, PhysicsCollision, 16, 16);
	GamePawn_SetAction(pawn, ACTION_BALL_IDLE);
}

//-----------------------------------------------------------------------------
//
void UpdateBall()
{
	// Update movement
	// g_Ball.DX = 0;
	g_Ball.DY = 0;
	
	// Jump/fall
	g_Ball.DY -= g_Ball.VelocityY / 4;
	g_Ball.VelocityY -= GRAVITY;
	if(g_Ball.VelocityY < -FORCE)
		g_Ball.VelocityY = -FORCE;

	// Test player collision
	Game_Pawn* ballPawn = &g_Ball.Pawn;
	struct Character* ply;
	if(ballPawn->PositionX > 128 - 8)
		ply = &g_Player1;
	else
		ply = &g_Player2;

	// Compute distance
	Game_Pawn* plyPawn = &ply->Pawn;
	i8 dx = ballPawn->PositionX - plyPawn->PositionX;
	i8 dy = ballPawn->PositionY - plyPawn->PositionY;

	u16 sqrtDist = (dx*dx) + (dy*dy);
	if(sqrtDist < 16*16)
	{
		g_Ball.VelocityY = FORCE;
		if(ply->bInAir)
			g_Ball.VelocityY += ply->VelocityY;
		g_Ball.DX = dx / 4;
	}

	// Update player animation & physics
	GamePawn_SetMovement(ballPawn, g_Ball.DX, g_Ball.DY);
	GamePawn_Update(ballPawn);
	// GamePawn_Draw(ballPawn);
}
//-----------------------------------------------------------------------------
// Load pattern data into VRAM
void VDP_LoadSpritePattern16_VSym(const u8* src, u8 index, u8 count)
{
	u16 dest = g_SpritePatternLow + (index * 8);
	for(u16 i = 0; i < count * 8; i++)
	{
		u8 val = Math_Flip(*src++);
		u16 addr = dest++;
		if(i & 0x10) // Shift every 16 bytes to flip 16x16 sprite's 4 blocks
			addr -= 16;
		else
			addr += 16;
		VDP_Poke_16K(val, addr);
	}
}

//-----------------------------------------------------------------------------
//
bool State_Initialize()
{
	// Initialize display
	VDP_EnableDisplay(FALSE);
	VDP_SetColor(COLOR_BLACK);
	
	// Initialize pattern
	VDP_FillVRAM(32, g_ScreenLayoutLow, 0, 32*24);
	VDP_FillVRAM(0, g_ScreenPatternLow, 0, 256*8); // Clear pattern
	VDP_WriteVRAM(g_DataBackground, g_ScreenPatternLow, 0, 24*8);

	// Initialize text
	Print_SetTextFont(g_Font_Carwar, 65);
	Print_SetColor(0xF, 0x1);
	Print_SetPosition(0, 0);
	Print_DrawText("EXAMPLE");

	// Initialize color
	VDP_FillVRAM(0xF0, g_ScreenColorLow, 0, 32); // Clear color
	VDP_Poke_16K(0x7F, g_ScreenColorLow + 0);
	VDP_Poke_16K(0x5F, g_ScreenColorLow + 1);
	VDP_Poke_16K(0xF5, g_ScreenColorLow + 2);
	VDP_Poke_16K(0x99, g_ScreenColorLow + 3);

	// Initialize sprite
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);
	//                           Source data              SprtID  Num
	VDP_LoadSpritePattern16_VSym(g_DataSprtLayer+8*4*4*1, 4*4*0,  4*4*2);
	VDP_LoadSpritePattern16_VSym(g_DataSprtLayer+8*4*4*4, 4*4*2,  4*4*3);
	VDP_LoadSpritePattern16_VSym(g_DataSprtLayer+8*4*4*8, 4*4*5,  4*4*2);
	//                    Source data              SprtID  Num
	VDP_LoadSpritePattern(g_DataSprtLayer+8*4*4*1, 4*4*7,  4*4*2);
	VDP_LoadSpritePattern(g_DataSprtLayer+8*4*4*4, 4*4*9,  4*4*3);
	VDP_LoadSpritePattern(g_DataSprtLayer+8*4*4*8, 4*4*12, 4*4*2);
	//                    Source data              SprtID  Num
	VDP_LoadSpritePattern(g_DataSprtBall,          4*4*14, 4*2*3);
	// VDP_SetSpriteSM1(6, 0, 208, 0, 0); // hide

	// Init player 1 pawn
	InitPlayer(&g_Player1, 0);

	// Init player 2 pawn
	InitPlayer(&g_Player2, 1);

	// Init ball
	InitBall();

	// Initialize layout
	DrawLevel();

	VDP_EnableDisplay(TRUE);

	Game_SetState(State_Game);
	return FALSE; // Frame finished
}

//-----------------------------------------------------------------------------
//
bool State_Game()
{
	GamePawn_Draw(&g_Ball.Pawn);
	GamePawn_Draw(&g_Player1.Pawn);
	GamePawn_Draw(&g_Player2.Pawn);

	// Background horizon blink
	if(g_bFlicker)
		VDP_FillVRAM(g_GameFrame & 1 ? 9 : 10, g_ScreenLayoutLow + (HORIZON+2) * 32, 0, 32);

	UpdatePlayer(&g_Player1);
	UpdatePlayer(&g_Player2);
	UpdateBall();

	// Update input
	u8 row3 = Keyboard_Read(3);
	u8 row8 = Keyboard_Read(8);

	g_Player1.Input = 0;
	if(IS_KEY_PRESSED(row8, KEY_RIGHT))
		g_Player1.Input |= INPUT_RIGHT;
	if(IS_KEY_PRESSED(row8, KEY_LEFT))
		g_Player1.Input |= INPUT_LEFT;
	if(IS_KEY_PRESSED(row8, KEY_UP))
		g_Player1.Input |= INPUT_JUMP;

	g_Player2.Input = 0;
	if(IS_KEY_PRESSED(row3, KEY_D))
		g_Player2.Input |= INPUT_LEFT;
	if(IS_KEY_PRESSED(row3, KEY_G))
		g_Player2.Input |= INPUT_RIGHT;
	if(IS_KEY_PRESSED(row3, KEY_F))
		g_Player2.Input |= INPUT_JUMP;

	if(IS_KEY_PRESSED(row3, KEY_I) && !IS_KEY_PRESSED(g_PrevRow3, KEY_I))
		g_bFlicker = 1 - g_bFlicker;

	g_PrevRow3 = row3;
	g_PrevRow8 = row8;

	if(Keyboard_IsKeyPressed(KEY_ESC))
		Game_Exit();

		
	return TRUE; // Frame finished
}

//-----------------------------------------------------------------------------
//
bool State_Pause()
{
	return TRUE; // Frame finished
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Programme entry point
void main()
{
	Game_SetState(State_Initialize);
	Game_MainLoop(VDP_MODE_GRAPHIC1);
}