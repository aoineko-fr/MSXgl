// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Example game
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

// Fixed point format
//
// Format	Prec.	Min		Max
//-----------------------------------
// Q2.6		0.016	-2		1.98
// Q4.4		0.06	-8		7.94
// Q10.6	0.016	-512	511.98
// Q12.4	0.06	-2048	2047.94

// Unit conversion (Pixel <> Q10.6)
#define Q10_6_TO_PX(a)				(u8)((a) / 64)
#define PX_TO_Q10_6(a)				(i16)((a) * 64)

// Unit conversion (Pixel <> Q2.6)
#define Q2_6_TO_PX(a)				(u8)((a) / 64)
#define PX_TO_Q2_6(a)				(i8)((a) * 64)

// Unit conversion (Pixel <> Q4.4)
#define Q4_4_TO_PX(a)				(u8)((a) / 16)
#define PX_TO_Q4_4(a)				(i8)((a) * 16)

// Gameplay value
#define JUMP_FORCE					PX_TO_Q4_4(5.0f)
#define FALL_SPEED					PX_TO_Q4_4(5.0f)
#define GRAVITY						PX_TO_Q4_4(0.25f)
#define COL_DIST					16

// Background
#define HORIZON_H					11
#define NET_H						7

// Debug section
#define S_DRAW						0
#define S_UPDATE					1
#define S_INPUT						2

// Input action flag
enum INPUT_ACTION
{
	INPUT_RIGHT = 0b00000001,
	INPUT_LEFT  = 0b00000010,
	INPUT_JUMP  = 0b00000100,
};

// Gameplay character stricture
struct Character
{
	bool		bMoving;
	bool		bInAir;
	i8			VelocityY;	// Format Q4.4
	i8			DX;			// Format Q4.4
	i8			DY;			// Format Q4.4
	u8			Input;
	u8			Score;
	Game_Pawn	Pawn;
};

// Prototypes
bool State_Initialize();
bool State_Game();
bool State_Pause();
void UpdateScore();


//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_carwar.h"
// Sprites by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "content/data_sprt_layer.h"
#include "content/data_sprt_ball.h"
#include "content/data_bg.h"

//.............................................................................
// Player 1 data

// Player's sprite layers
// This character is made by 3 layers (3 sprites) but the first two are special: One is only visible on even frames while the second only on odd frames.
// So, on the 3 layers, only two are visible at a given frame. The blinking of the first two black layers is done to create shaded colors.
// The counterpart is the flickering effect. The character white color comes from the background and is not in a sprite.
const Game_Sprite g_SpriteLayers[] =
{
//	  Sprite ID
//    |  X offset from pawn's position
//    |  |  Y offset
//    |  |  |  Pattern offset from current animation key
//    |  |  |  |   Layer's color
//    |  |  |  |   |                Layer option
	{ 0, 0, 0, 0,  COLOR_BLACK,     PAWN_SPRITE_EVEN }, // Only visible on even frame number
	{ 0, 0, 0, 12, COLOR_BLACK,     PAWN_SPRITE_ODD }, // Only visible on odd frame number
	{ 4, 0, 0, 8,  COLOR_LIGHT_RED, 0 },
};

// Idle animation frames
// Each line describes an animation key
const Game_Frame g_FramesIdle[] =
{
//	  Pattern offset of this animation key in the sprite data
//    |     Animation key duration (in frame number)
//    |     |   Event to trigger during this animation key (function pointer)
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
	{ 2, 0, 0, 4,  COLOR_DARK_GREEN, 0 },
	{ 3, 0, 0, 0,  COLOR_MEDIUM_GREEN, 0 },
};

// Idle animation frames
const Game_Frame g_BallIdle[] =
{
	{ 0*8+224,	4,	NULL },
};

// Bump animation frames
const Game_Frame g_BallBump[] =
{
	{ 1*8+224,	1,	NULL },
	{ 2*8+224,	5,	NULL },
	{ 1*8+224,	2,	NULL },
};

// Actions id
enum ANIM_ACTION_BALL_ID
{
	ACTION_BALL_IDLE = 0,
	ACTION_BALL_BUMP,
};

// List of all ball actions
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

c8   g_StrBuffer[16];

bool g_FreezeBall = TRUE;


//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Physics callback
void PhysicsEventPlayer1(u8 event, u8 tile)
{
	tile;
	switch(event)
	{
	case PAWN_PHYSICS_BORDER_DOWN: // Handle downward collisions 
	case PAWN_PHYSICS_COL_DOWN:
		g_Player1.bInAir = FALSE;
		break;

	case PAWN_PHYSICS_FALL: // Handle falling
		if (!g_Player1.bInAir)
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
	tile;
	switch(event)
	{
	case PAWN_PHYSICS_BORDER_DOWN: // Handle downward collisions 
	case PAWN_PHYSICS_COL_DOWN:
		g_Player2.bInAir = FALSE;
		break;

	case PAWN_PHYSICS_FALL: // Handle falling
		if (!g_Player2.bInAir)
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
	tile;
	switch(event)
	{
	case PAWN_PHYSICS_BORDER_DOWN: // Handle downward collisions 
	case PAWN_PHYSICS_COL_DOWN:
		g_Ball.VelocityY *= -1; // Reverse
		GamePawn_SetAction(&g_Ball.Pawn, ACTION_BALL_BUMP);
		// if (g_Ball.Pawn.PositionY > 128)
		// {
			// if (g_Ball.Pawn.PositionX < 128) // Player 1 score a point!
			// {
				// if (g_Player1.Score < 99)
					// g_Player1.Score++;
			// }
			// else // Player 2 score a point!
			// {
				// if (g_Player2.Score < 99)
					// g_Player2.Score++;
			// }
			// UpdateScore();
		// }
		break;

	case PAWN_PHYSICS_COL_RIGHT: // Handle net
	case PAWN_PHYSICS_COL_LEFT:
	case PAWN_PHYSICS_BORDER_RIGHT: // Handle border
	case PAWN_PHYSICS_BORDER_LEFT:
		g_Ball.DX = -g_Ball.DX;
		break;

	case PAWN_PHYSICS_FALL: // Handle falling
		if (!g_Ball.bInAir)
		{
			g_Ball.bInAir = TRUE;
			g_Ball.VelocityY = 0;
		}
		break;
	};
}

//-----------------------------------------------------------------------------
// Collision callback
//
// Parameters:
//   tile - Number o the tile to check
//
// Return:
//   TRUE if the tile is a blocker, FALSE otherwise
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
// Draw level background
void DrawLevel()
{
	// Background
	loop(i, 24-1)
		VDP_FillVRAM_16K((i <= HORIZON_H) ? 16 : 8, VDP_GetLayoutTable() + (i + 1) * 32, 32);

	// Ground
	VDP_FillVRAM_16K(1, VDP_GetLayoutTable() + 23 * 32, 32);

	// "Net"
	loop(i, NET_H)
	{
		u16 addr = VDP_GetLayoutTable() + 15 + (i + (23 - NET_H)) * 32;
		VDP_Poke_16K((i == 0) ? 0 : 5, addr++);
		VDP_Poke_16K((i == 0) ? 2 : 6, addr);
	}
}

//-----------------------------------------------------------------------------
// Initialize player
void InitPlayer(struct Character* ply, u8 id)
{
	Mem_Set(0, ply, sizeof(struct Character));

	Game_Pawn* pawn = &ply->Pawn;
	if (id == 0)
	{
		GamePawn_Initialize(pawn, g_SpriteLayers, numberof(g_SpriteLayers), 0, g_AnimActions);
		GamePawn_SetPosition(pawn, (u8)(255 - 16 - 16), 128);
		GamePawn_InitializePhysics(pawn, PhysicsEventPlayer1, PhysicsCollision, 16, 16);
	}
	else
	{
		GamePawn_Initialize(pawn, g_SpriteLayers2, numberof(g_SpriteLayers2), 0, g_AnimActions2);
		GamePawn_SetPosition(pawn, 16, 128);
		GamePawn_InitializePhysics(pawn, PhysicsEventPlayer2, PhysicsCollision, 16, 16);
	}
}

//-----------------------------------------------------------------------------
// Update player gameplay and display
void UpdatePlayer(struct Character* ply)
{
	// Update movement
	ply->DX = 0;
	ply->DY = 0;
	
	if (ply->Input & INPUT_RIGHT)
	{
		ply->DX = 16;
		ply->bMoving = TRUE;
	}
	else if (ply->Input & INPUT_LEFT)
	{
		ply->DX = -16;
		ply->bMoving = TRUE;
	}
	else
		ply->bMoving = FALSE;

	if (ply->bInAir) // Handle in air state (jump or fall)
	{
		ply->DY -= ply->VelocityY; // Apply vertical force

		ply->VelocityY -= GRAVITY; // Apply gravity
		if (ply->VelocityY < -FALL_SPEED) // Clamp fall speed
			ply->VelocityY = -FALL_SPEED;
	}
	else if (ply->Input & INPUT_JUMP) // Initialize jump force
	{
		ply->bInAir = TRUE;
		ply->VelocityY = JUMP_FORCE;
	}

	// Update player animation & physics
	u8 act = ACTION_IDLE;
	if (ply->bInAir && (ply->VelocityY >= 0))
		act = ACTION_JUMP;
	else if (ply->bInAir)
		act = ACTION_FALL;
	else if (ply->bMoving)
		act = ACTION_MOVE;

	Game_Pawn* pawn = &ply->Pawn;
	GamePawn_SetAction(pawn, act);
	GamePawn_SetMovement(pawn, ply->DX / 16, ply->DY / 16);
	GamePawn_Update(pawn);
}

//-----------------------------------------------------------------------------
//
void InitBall()
{
	Mem_Set(0, &g_Ball, sizeof(struct Character));

	Game_Pawn* pawn = &g_Ball.Pawn;
	GamePawn_Initialize(pawn, g_BallLayers, numberof(g_BallLayers), 6, g_BallActions);
	GamePawn_SetPosition(pawn, 150, 128);
	GamePawn_InitializePhysics(pawn, PhysicsEventBall, PhysicsCollision, 16, 16);
	GamePawn_SetAction(pawn, ACTION_BALL_IDLE);
}

//-----------------------------------------------------------------------------
// Update ball gameplay and display
void UpdateBall()
{
	// Apply gravity
	g_Ball.DY = -g_Ball.VelocityY;
	g_Ball.VelocityY -= GRAVITY;
	if (g_Ball.VelocityY < -FALL_SPEED)
		g_Ball.VelocityY = -FALL_SPEED;

	// Select the player to tests
	Game_Pawn* ballPawn = &g_Ball.Pawn;
	struct Character* ply = (ballPawn->PositionX > 128 - 8) ? &g_Player1 : &g_Player2;

	// Compute distance
	Game_Pawn* plyPawn = &ply->Pawn;
	i8 dx = ballPawn->PositionX - plyPawn->PositionX;
	i8 dy = ballPawn->PositionY - plyPawn->PositionY;

	u16 sqrtDist = (dx*dx) + (dy*dy);
	if (sqrtDist < COL_DIST * COL_DIST)
	{
		DEBUG_LOGNUM("Velocity", g_Ball.VelocityY);

		g_Ball.DX = dx * 4; // Math_SignedDiv4
		g_Ball.VelocityY = JUMP_FORCE;

		GamePawn_SetAction(ballPawn, ACTION_BALL_BUMP);

		// Print_SetPosition(0, 2);
		// Print_DrawFormat("%i..\n%i..", dx, dy);

		DEBUG_PRINT("DX/Y: %s%i %s%i", (dx > 0) ? "+" : "", dx, (dy > 0) ? "+" : "", dy);
		// DEBUG_BREAK();
	}

	// Update player animation & physics
	GamePawn_SetMovement(ballPawn, g_Ball.DX / 16, g_Ball.DY / 16);
	GamePawn_Update(ballPawn);
}

//-----------------------------------------------------------------------------
// Load pattern data into VRAM
void VDP_LoadSpritePattern16_VSym(const u8* src, u8 index, u8 count)
{
	u16 dest = g_SpritePatternLow + (index * 8);
	for (u16 i = 0; i < count * 8; i++)
	{
		u8 val = Math_Flip(*src++);
		u16 addr = dest++;
		if (i & 0x10) // Shift every 16 bytes to flip 16x16 sprite's 4 blocks
			addr -= 16;
		else
			addr += 16;
		VDP_Poke_16K(val, addr);
	}
}

//-----------------------------------------------------------------------------
// Data initialization state
bool State_Initialize()
{
	// Initialize display
	VDP_EnableDisplay(FALSE);
	VDP_SetColor(COLOR_BLACK);
	
	// Initialize pattern
	VDP_FillVRAM_16K(32, VDP_GetLayoutTable(), 32*24);
	VDP_FillVRAM_16K(0, VDP_GetPatternTable(), 256*8); // Clear pattern
	VDP_WriteVRAM_16K(g_DataBackground, VDP_GetPatternTable(), 24*8);

	// Initialize text
	Print_SetTextFont(g_Font_Carwar, 65);
	Print_SetColor(0xF, 0x1);
	Print_SetPosition(0, 0);
	Print_DrawText("PENG.PONG....00..00");

	// Initialize color
	VDP_FillVRAM_16K(0xF0, VDP_GetColorTable(), 32); // Clear color
	VDP_Poke_16K(0x7F, VDP_GetColorTable() + 0);
	VDP_Poke_16K(0x5F, VDP_GetColorTable() + 1);
	VDP_Poke_16K(0xF5, VDP_GetColorTable() + 2);
	VDP_Poke_16K(0x99, VDP_GetColorTable() + 3);

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
// Game update state
bool State_Game()
{
	PROFILE_FRAME_START();

	PROFILE_SECTION_START(100, S_DRAW, "");
	GamePawn_Draw(&g_Ball.Pawn);
	GamePawn_Draw(&g_Player1.Pawn);
	GamePawn_Draw(&g_Player2.Pawn);
	// Background horizon blink
	if (g_bFlicker)
		VDP_FillVRAM_16K(g_GameFrame & 1 ? 9 : 10, VDP_GetLayoutTable() + (HORIZON_H + 2) * 32, 32);
	PROFILE_SECTION_END(100, S_DRAW, "");

	PROFILE_SECTION_START(100, S_UPDATE, "");
	UpdatePlayer(&g_Player1);
	UpdatePlayer(&g_Player2);
	UpdateBall();
	PROFILE_SECTION_END(100, S_UPDATE, "");

	PROFILE_SECTION_START(100, S_INPUT, "");
	// Update input
	u8 row3 = Keyboard_Read(3);
	u8 row8 = Keyboard_Read(8);

	g_Player1.Input = 0;
	if (IS_KEY_PRESSED(row8, KEY_RIGHT))
		g_Player1.Input |= INPUT_RIGHT;
	if (IS_KEY_PRESSED(row8, KEY_LEFT))
		g_Player1.Input |= INPUT_LEFT;
	if (IS_KEY_PRESSED(row8, KEY_UP))
		g_Player1.Input |= INPUT_JUMP;

	g_Player2.Input = 0;
	if (IS_KEY_PRESSED(row3, KEY_D))
		g_Player2.Input |= INPUT_LEFT;
	if (IS_KEY_PRESSED(row3, KEY_G))
		g_Player2.Input |= INPUT_RIGHT;
	if (IS_KEY_PRESSED(row3, KEY_F))
		g_Player2.Input |= INPUT_JUMP;

	if (IS_KEY_PUSHED(row3, g_PrevRow3, KEY_I))
		g_bFlicker = 1 - g_bFlicker;

	if (IS_KEY_PUSHED(row8, g_PrevRow8, KEY_DEL))
	{
		g_FreezeBall = !g_FreezeBall;
		GamePawn_SetEnable(&g_Ball.Pawn, g_FreezeBall);
	}

	g_PrevRow3 = row3;
	g_PrevRow8 = row8;

	if (Keyboard_IsKeyPressed(KEY_ESC))
		Game_Exit();
	PROFILE_SECTION_END(100, S_INPUT, "");

	PROFILE_FRAME_END();
		
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
	DEBUG_INIT();
	DEBUG_LOG("Start debug session!");

	Game_SetState(State_Initialize);
	Game_MainLoop(VDP_MODE_GRAPHIC1);

	DEBUG_LOG("End debug session!");
}