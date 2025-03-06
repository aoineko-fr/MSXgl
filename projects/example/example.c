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

// MSXgl
#include "msxgl.h"
#include "game/game.h"
#include "game/pawn.h"
#include "game/menu.h"
#include "math.h"
#include "debug.h"
#include "string.h"
#include "fixed_point.h"
#include "version.h"
#include "compress/pletter.h"

// Game
#include "pixel_phenix.h"

//=============================================================================
// DEFINES
//=============================================================================

// Fixed point format
//
// Format	Prec.	Min		Max
//-----------------------------------
// Q4.4		0.06	-8		7.94
// Q12.4	0.06	-2048	2047.94

// Gameplay value
#define BALL_GRAVITY				Q4_4_SET(0.15f)
#define FALL_MAX_SPEED				Q4_4_SET(3.0f)

#define JUMP_FORCE					Q4_4_SET(-3.0f)
#define GRAVITY						Q4_4_SET(0.2f)
#define MOVE_FRICTION				Q4_4_SET(0.1f)
#define MOVE_ACCEL					Q4_4_SET(0.33f)
#define MOVE_MAX_SPEED				Q4_4_SET(1.5f)

#define BOUNCE_FORCE				Q4_4_SET(-2.5f)
#define COL_DIST					16

// Background
#define HORIZON_H					11
#define NET_H						7

// Debug section
#define S_DRAW						0
#define S_UPDATE					1
#define S_INPUT						2

// Index of all menu pages
enum MENU_PAGES
{
	MENU_MAIN = 0, // Main page
	MENU_START,    // Start page
	MENU_OPTIONS,  // Options page
	MENU_CREDITS,  // Credits page
	MENU_MAX,      // Number of menu
};

// Input action flag
enum INPUT_ACTION
{
	INPUT_RIGHT = 0b00000001,
	INPUT_LEFT  = 0b00000010,
	INPUT_JUMP  = 0b00000100,
};

// Actions id
enum ACTION_PLAYER_ID
{
	ACTION_PLAYER_IDLE = 0,
	ACTION_PLAYER_MOVE,
	ACTION_PLAYER_JUMP,
	ACTION_PLAYER_FALL,
	ACTION_PLAYER_HIT,
	ACTION_PLAYER_WIN,
	ACTION_PLAYER_LOOSE,
	ACTION_PLAYER_MAX,
};

// Actions id
enum ACTION_BALL_ID
{
	ACTION_BALL_IDLE = 0,
	ACTION_BALL_BUMP,
	ACTION_BALL_MAX,
};

// Gameplay character stricture
struct Character
{
	bool		bFreeze;
	bool		bMoving;
	bool		bInAir;
	i8			VelocityX;	// Format Q4.4 [-8:7.94]
	i8			VelocityY;	// Format Q4.4 [-8:7.94]
	i8			RestX;	// Format Q4.4 [-8:7.94]
	i8			RestY;	// Format Q4.4 [-8:7.94]
	u8			Input;
	u8			Score;
	Pawn		Pawn;
};

// Gameplay rule structure
struct Rule
{
	u8			GameScore;
	u8			MaxBounce;
	u8			MaxPass;
};

// Cloud data structure
struct Cloud
{
	u8			X;
	u8			Y;
	u8			Pattern;
	u8			Sprite;
	u8			Mask;
};

// Prototypes
bool State_LogoInit();
bool State_LogoUpdate();
bool State_MenuInit();
bool State_MenuUpdate();
bool State_GameInit();
bool State_KickOff();
bool State_Game();
bool State_Pause();
bool State_Point();
bool State_VictoryInit();
bool State_VictoryUpdate();

void DrawScore();

const c8* MenuAction_Start(u8 op, i8 value);

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "content/data_font.h"
// Sprites by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "content/data_sprt_layer.h"
#include "content/data_sprt_ball.h"
#include "content/data_sprt_cloud.h"
#include "content/data_bg2.h"

//.............................................................................
// Player 1 data

// Player's sprite layers
// This character is made by 3 layers (3 sprites) but the first two are special: One is only visible on even frames while the second only on odd frames.
// So, on the 3 layers, only two are visible at a given frame. The blinking of the first two black layers is done to create shaded colors.
// The counterpart is the flickering effect. The character white color comes from the background and is not in a sprite.
const Pawn_Sprite g_SpriteLayers[] =
{
//	  Sprite ID
//    |  X offset from pawn's position
//    |  |  Y offset
//    |  |  |  Pattern offset from current animation key
//    |  |  |  |  Layer's color
//    |  |  |  |  |                Layer option
	{ 4, 0, 0, 0, COLOR_LIGHT_RED, 0 },
	{ 2, 0, 0, 4, COLOR_BLACK,     PAWN_SPRITE_FLIP }, // Only visible on even frame number
};

// Idle animation frames
// Each line describes an animation key
const Pawn_Frame g_FramesIdle[] =
{
//	  Pattern offset of this animation key in the sprite data
//    |     Animation key duration (in frame number)
//    |     |   Event to trigger during this animation key (function pointer)
	{ 6*12, 64, NULL },
	{ 7*12, 24, NULL },
};

// Move animation frames
const Pawn_Frame g_FramesMove[] =
{
	{ 0*12, 4, NULL },
	{ 1*12, 4, NULL },
	{ 2*12, 4, NULL },
	{ 3*12, 4, NULL },
};

// Jump animation frames
const Pawn_Frame g_FramesJump[] =
{
	{ 5*12, 4, NULL },
};

// Fall animation frames
const Pawn_Frame g_FramesFall[] =
{
	{ 6*12,	4, NULL },
};

// 
const Pawn_Frame g_FramesHit[] =
{
	{ 12*12, 10, NULL },
};

// 
const Pawn_Frame g_FramesWin[] =
{
	{ 14*12, 25, NULL },
	{ 15*12, 25, NULL },
};

// 
const Pawn_Frame g_FramesLost[] =
{
	{ 13*12, 4,	NULL },
};

// List of all player actions
const Pawn_Action g_AnimActions[ACTION_PLAYER_MAX] =
{ //  Frames        Number               Loop? Interrupt?
	{ g_FramesIdle, numberof(g_FramesIdle), 1, 1 }, // ACTION_PLAYER_IDLE
	{ g_FramesMove, numberof(g_FramesMove), 1, 1 }, // ACTION_PLAYER_MOVE
	{ g_FramesJump, numberof(g_FramesJump), 1, 1 }, // ACTION_PLAYER_JUMP
	{ g_FramesFall, numberof(g_FramesFall), 1, 1 }, // ACTION_PLAYER_FALL
	{ g_FramesHit,  numberof(g_FramesHit),  0, 0 }, // ACTION_PLAYER_HIT
	{ g_FramesWin,  numberof(g_FramesWin),  1, 0 }, // ACTION_PLAYER_WIN
	{ g_FramesLost, numberof(g_FramesLost), 1, 0 }, // ACTION_PLAYER_LOOSE
};

//.............................................................................
// Player 2 data

// Pawn sprite layers
const Pawn_Sprite g_SpriteLayers2[] =
{
	{ 5, 0, 0, 0, COLOR_LIGHT_RED, 0 },
	{ 3, 0, 0, 4, COLOR_BLACK, PAWN_SPRITE_FLIP },
};

//.............................................................................
// Ball data

// Pawn sprite layers
const Pawn_Sprite g_BallLayers[] =
{
	{ 0, 0, 0, 4,  COLOR_DARK_RED, 0 },
	{ 1, 0, 0, 0,  COLOR_MEDIUM_RED, 0 },
};

// Idle animation frames
const Pawn_Frame g_BallIdle[] =
{
	{ 0*8,	4,	NULL },
};

// Bump animation frames
const Pawn_Frame g_BallBump[] =
{
	{ 1*8,	1,	NULL },
	{ 2*8,	5,	NULL },
	{ 1*8,	2,	NULL },
};

// List of all ball actions
const Pawn_Action g_BallActions[ACTION_BALL_MAX] =
{ //  Frames      Number             Loop? Interrupt?
	{ g_BallIdle, numberof(g_BallIdle), 1, 1 }, // ACTION_BALL_IDLE
	{ g_BallBump, numberof(g_BallBump), 0, 0 }, // ACTION_BALL_BUMP
};

//.............................................................................

// Clouds data
const struct Cloud g_Cloud[] =
{
	{  30,  8,  80,  6, 0b00000111 },
	{  46,  8,  88,  8, 0b00000111 },
	{ 140, 20,  96, 10, 0b00001111 },
	{ 156, 20, 104, 12, 0b00001111 },
	{   4, 30,  96, 14, 0b00011111 },
	{  20, 30, 104, 16, 0b00011111 },
	{ 124, 37, 112, 18, 0b00111111 },
	{ 248, 38, 112, 20, 0b00111111 },
};


//=============================================================================
// MEMORY DATA
//=============================================================================

struct Character g_Player[2];
struct Character g_Ball;

// bool g_bFlicker = TRUE;
u8   g_PrevRow3 = 0xFF;
u8   g_PrevRow8 = 0xFF;

i16  g_CloudX[numberof(g_Cloud)];
u8   g_FrameCount = 0;
u16  g_StateTimer = 0;

u8   g_CollisionMap[32*24];

u8   g_Field = 0;
u8   g_Bounce = 0;
u8   g_Pass = 0;

u8   g_LastTouch = 0;
u8   g_Victorious = 0;

struct Rule g_Rule = { 11, 1, 0 };

bool g_OptMusic;
bool g_OptSFX;
bool g_OptBlend;

//.............................................................................
// Menu

// Entries description for the Main menu
const MenuItem g_MenuMain[] =
{
	{ "GAME",                MENU_ITEM_GOTO, NULL, MENU_START },	// Entry to start a game (will trigger MenuAction_Start with value equal to '1')
	{ "OPTIONS",             MENU_ITEM_GOTO, NULL, MENU_OPTIONS },	// Entry to go to Option menu page
	{ "CREDITS",             MENU_ITEM_GOTO, NULL, MENU_CREDITS },	// Entry to go to Option menu page
	// { NULL,                  MENU_ITEM_EMPTY, NULL, 0 },			// Blank entry to create a gap
	// { "EXIT",                MENU_ITEM_ACTION, NULL, NULL },			// Entry to exit the game (will trigger MenuAction_Start with value equal to '0')
};

// 
MenuItem g_MenuStart[] =
{
	{ "START>",              MENU_ITEM_ACTION, MenuAction_Start, 0 }, // Entry to change the screen mode (will trigger MenuAction_Screen)
	{ "PLAYER1",             MENU_ITEM_TEXT, NULL, 0 }, // Entry display a text aligned to left
	{ "PLAYER2",             MENU_ITEM_TEXT, NULL, 0 }, // Entry display a text aligned to center
	{ "POINTS",              MENU_ITEM_INT, &g_Rule.GameScore, NULL },
	{ "BOUNCES",             MENU_ITEM_INT, &g_Rule.MaxBounce, NULL },
	{ "PASSES",              MENU_ITEM_INT, &g_Rule.MaxPass, NULL },
	{ NULL,                  MENU_ITEM_EMPTY, NULL, 0 },               // Blank entry to create a gap
	{ "<BACK",               MENU_ITEM_GOTO, NULL, MENU_MAIN },        // Entry to go back to the main menu
};

// 
MenuItem g_MenuOptions[] =
{
	{ "MUSIC",               MENU_ITEM_BOOL, &g_OptMusic, NULL },
	{ "SFX",                 MENU_ITEM_BOOL, &g_OptSFX, NULL },
	{ "COLOR BLEND",         MENU_ITEM_BOOL, &g_OptBlend, NULL },
	{ "FREQ",                MENU_ITEM_TEXT, NULL, 0 },
	{ "PALETTE",             MENU_ITEM_TEXT, NULL, 0 },
	{ NULL,                  MENU_ITEM_EMPTY, NULL, 0 },                       // Blank entry to create a gap
	{ "<BACK",               MENU_ITEM_GOTO, NULL, MENU_MAIN },                // Entry to go back to the main menu
};

// 
MenuItem g_MenuCredits[] =
{
	{ "CODE    AOINEKO",     MENU_ITEM_TEXT, NULL, 0 }, // Entry display a text aligned to left
	{ "SPRITE  GRAFXKID",    MENU_ITEM_TEXT, NULL, 0 }, // Entry display a text aligned to center
	{ "GRAPH   YAZ",         MENU_ITEM_TEXT, NULL, 0 }, // Entry display a text aligned to center
	{ "FONT    LUDO 'GFX'",  MENU_ITEM_TEXT, NULL, 0 }, // Entry display a text aligned to center
	{ "MUSIC   ???",         MENU_ITEM_TEXT, NULL, 0 }, // Entry display a text aligned to center
	{ "SFX     ???",         MENU_ITEM_TEXT, NULL, 0 }, // Entry display a text aligned to center
	{ NULL,                  MENU_ITEM_EMPTY, NULL, 0 },                       // Blank entry to create a gap
	{ "<BACK",               MENU_ITEM_GOTO, NULL, MENU_MAIN },                // Entry to go back to the main menu
};

// List of all menus
const Menu g_Menus[MENU_MAX] =
{
	{ NULL,	g_MenuMain,    numberof(g_MenuMain),    NULL }, // MENU_MAIN
	{ NULL,	g_MenuStart,   numberof(g_MenuStart),   NULL }, // MENU_START
	{ NULL,	g_MenuOptions, numberof(g_MenuOptions), NULL }, // MENU_OPTIONS
	{ NULL,	g_MenuCredits, numberof(g_MenuCredits), NULL }, // MENU_CREDITS
};


//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// MENU
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
const c8* MenuAction_Start(u8 op, i8 value)
{
	value;
	switch(op)
	{
	case MENU_ACTION_SET:
	case MENU_ACTION_INC:
	case MENU_ACTION_DEC:
		Game_SetState(State_GameInit);
		break;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// RULES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Handle change field rule
void Rules_ChangeField(u8 field)
{
	g_Field = field;
	g_Bounce = 0;
	g_Pass = 0;
}

//-----------------------------------------------------------------------------
// Handle match starting rule
void Rules_Init()
{
	g_Player[0].Score = 0;
	g_Player[1].Score = 0;
	DrawScore();
	Rules_ChangeField(0);
}	

//-----------------------------------------------------------------------------
// Handle score rule
void Rules_Score(u8 ply)
{
	g_Victorious = ply;
	g_Player[ply].Score++;
	if ((g_Player[ply].Score >= g_Rule.GameScore) && (g_Player[ply].Score > g_Player[1 - ply].Score + 1))
	{
		Game_SetState(State_VictoryInit);
	}
	else
	{
		g_StateTimer = 20;
		VDP_SetColor(COLOR_WHITE);
		Game_SetState(State_Point);
	}
	DrawScore();
}	

//-----------------------------------------------------------------------------
// Handle bounces rule
void Rules_Bounce()
{
	g_Bounce++;
	if ((g_Rule.MaxBounce != 0xFF) && (g_Bounce > g_Rule.MaxBounce))
		Rules_Score(1 - g_Field);
	g_LastTouch = g_Field;
}

//-----------------------------------------------------------------------------
// Handle pass rule
void Rules_Pass()
{
	g_Pass++;
	if ((g_Rule.MaxPass != 0) && (g_Pass > g_Rule.MaxPass))
		Rules_Score(1 - g_Field);
	g_LastTouch = g_Field;
}

//-----------------------------------------------------------------------------
// Handle out-of-field rule
void Rules_Out()
{
	Rules_Score(1 - g_LastTouch);
}

//-----------------------------------------------------------------------------
// PHYSICS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Physics callback
void PhysicsEventPlayer1(u8 event, u8 tile)
{
	tile;
	switch(event)
	{
	case PAWN_PHYSICS_BORDER_DOWN: // Handle downward collisions 
	case PAWN_PHYSICS_COL_DOWN:
		g_Player[0].bInAir = FALSE;
		break;

	case PAWN_PHYSICS_FALL: // Handle falling
		if (!g_Player[0].bInAir)
		{
			g_Player[0].bInAir = TRUE;
			g_Player[0].VelocityY = 0;
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
		g_Player[1].bInAir = FALSE;
		break;

	case PAWN_PHYSICS_FALL: // Handle falling
		if (!g_Player[1].bInAir)
		{
			g_Player[1].bInAir = TRUE;
			g_Player[1].VelocityY = 0;
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
		Pawn_SetAction(&g_Ball.Pawn, ACTION_BALL_BUMP);
		if (g_Ball.Pawn.PositionY > 140)
			Rules_Bounce();
		break;

	case PAWN_PHYSICS_COL_RIGHT: // Handle net
	case PAWN_PHYSICS_COL_LEFT:
		g_Ball.VelocityX *= -1;
		break;
	
	case PAWN_PHYSICS_BORDER_RIGHT: // Handle border
	case PAWN_PHYSICS_BORDER_LEFT:
		Rules_Out();
		break;

	case PAWN_PHYSICS_FALL: // Handle falling
		// if (!g_Ball.bInAir)
		// {
		// 	g_Ball.bInAir = TRUE;
		// 	g_Ball.VelocityY = 0;
		// }
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
	return tile;
}

//-----------------------------------------------------------------------------
// MISC
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Draw the updated score
void DrawScore()
{
	Print_SetPosition(13, 0);
	if (g_Player[0].Score < 10)
		Print_DrawChar('0');
	Print_DrawInt(g_Player[0].Score);

	Print_SetPosition(17, 0);
	if (g_Player[1].Score < 10)
		Print_DrawChar('0');
	Print_DrawInt(g_Player[1].Score);
}

//-----------------------------------------------------------------------------
// Load in all 3 screen sections
void Pletter_LoadGM2(const u8* src, u16 dst)
{
	Pletter_UnpackToVRAM(src, dst);
	dst += 0x800;
	Pletter_UnpackToVRAM(src, dst);
	dst += 0x800;
	Pletter_UnpackToVRAM(src, dst);
}

//-----------------------------------------------------------------------------
// 
void CollisionInit()
{
	Mem_Set(0, g_CollisionMap, sizeof(g_CollisionMap));
	
	// Ground
	Mem_Set(1, g_CollisionMap + 23 * 32, 32);

	// "Net"
	loop(i, NET_H)
	{
		u8* ptr = g_CollisionMap + 15 + (i + (23 - NET_H)) * 32;
		*ptr++ = 1;
		*ptr   = 1;
	}

	Pawn_SetTileMap(g_CollisionMap);
}

//-----------------------------------------------------------------------------
// Draw level background
void DrawLevel()
{
	// Background
	Pletter_LoadGM2(g_DataBackground_Patterns, VDP_GetPatternTable());
	Pletter_LoadGM2(g_DataBackground_Colors, VDP_GetColorTable());
	Pletter_UnpackToVRAM(g_DataBackground_Names, VDP_GetLayoutTable());
}

//-----------------------------------------------------------------------------
//
bool Logo_Skip()
{
	return FALSE;
}

//-----------------------------------------------------------------------------
// PLAYER
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Initialize player
void InitPlayerPosition(u8 id)
{
	struct Character* ply = &g_Player[id];
	Pawn* pawn = &ply->Pawn;
	if (id == 0)
		Pawn_SetPosition(pawn, 32, 168);
	else
		Pawn_SetPosition(pawn, (u8)(255 - 32 - 16), 168);
}

//-----------------------------------------------------------------------------
// Initialize player
void InitPlayer(u8 id)
{
	struct Character* ply = &g_Player[id];
	Mem_Set(0, ply, sizeof(struct Character)); // Set all paramters to 0

	Pawn* pawn = &ply->Pawn;
	if (id == 0)
	{
		Pawn_Initialize(pawn, g_SpriteLayers, numberof(g_SpriteLayers), 0, g_AnimActions);
		Pawn_InitializePhysics(pawn, PhysicsEventPlayer1, PhysicsCollision, 16, 16);
	}
	else
	{
		Pawn_Initialize(pawn, g_SpriteLayers2, numberof(g_SpriteLayers2), 0, g_AnimActions);
		Pawn_InitializePhysics(pawn, PhysicsEventPlayer2, PhysicsCollision, 16, 16);
		Pawn_SetSpriteFX(pawn, PAWN_SPRITE_FX_FLIP_X);
	}
	Pawn_SetPatternAddress(pawn, g_DataSprtLayer);

	InitPlayerPosition(id);
}

//-----------------------------------------------------------------------------
// Update character gameplay and display
void UpdateCharacter(struct Character* ply)
{
	Pawn* pawn = &ply->Pawn;

	if (!ply->bFreeze)
	{
		// Compute delta movement and store conversion rest
		i8 dx = Q4_4_GET(ply->VelocityX);
		ply->RestX += ply->VelocityX - dx;
		dx += Q4_4_GET(ply->RestX);
		ply->RestX = Q4_4_FRAC(ply->RestX);

		// Compute delta movement and store conversion rest
		i8 dy = Q4_4_GET(ply->VelocityY);
		ply->RestY += ply->VelocityY - dy;
		dy += Q4_4_GET(ply->RestY);
		ply->RestY = Q4_4_FRAC(ply->RestY);

		Pawn_SetMovement(pawn, dx, dy);
	}
	Pawn_Update(pawn);
}

//-----------------------------------------------------------------------------
// Update player gameplay and display
void UpdatePlayer(struct Character* ply)
{
	// HORIZONTAL MOVEMENT

	// Check movement input
	if (ply->Input & INPUT_RIGHT)
	{
		ply->VelocityX += MOVE_ACCEL;
		ply->bMoving = TRUE;
	}
	else if (ply->Input & INPUT_LEFT)
	{
		ply->VelocityX -= MOVE_ACCEL;
		ply->bMoving = TRUE;
	}
	else // Apply friction
	{
		ply->bMoving = FALSE;
		// if(ply->VelocityX > MOVE_FRICTION)
		// 	ply->VelocityX -= MOVE_FRICTION;
		// else if(ply->VelocityX < -MOVE_FRICTION)
		// 	ply->VelocityX += MOVE_FRICTION;
		// else
			ply->VelocityX = 0;
	}
	// Clamp speed
	ply->VelocityX = CLAMP8(ply->VelocityX, -MOVE_MAX_SPEED, MOVE_MAX_SPEED);

	// VERTICAL MOVEMENT
	
	if (ply->bInAir) // Handle in air state (jump or fall)
	{
		ply->VelocityY += GRAVITY; // Apply gravity
		if (ply->VelocityY > FALL_MAX_SPEED) // Clamp fall speed
			ply->VelocityY = FALL_MAX_SPEED;
	}
	else if (ply->Input & INPUT_JUMP) // Initialize jump force
	{
		ply->bInAir = TRUE;
		ply->VelocityY = JUMP_FORCE;
	}
	else
		ply->VelocityY = 0;

	// UPDATE

	// Update player animation & physics
	u8 act = ACTION_PLAYER_IDLE;
	if (ply->bInAir && (ply->VelocityY >= 0))
		act = ACTION_PLAYER_JUMP;
	else if (ply->bInAir)
		act = ACTION_PLAYER_FALL;
	else if (ply->bMoving)
		act = ACTION_PLAYER_MOVE;

	Pawn* pawn = &ply->Pawn;
	Pawn_SetAction(pawn, act);
	UpdateCharacter(ply);
}

//-----------------------------------------------------------------------------
// BALL
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Initialize ball position
void InitBallPosition()
{
	Pawn* pawn = &g_Ball.Pawn;
	Pawn_SetPosition(pawn, (g_Field == 0) ? 56 : 184, 128);
	g_Ball.bFreeze = TRUE;
}

//-----------------------------------------------------------------------------
// Initialize ball structure
void InitBall()
{
	Mem_Set(0, &g_Ball, sizeof(struct Character));

	Pawn* pawn = &g_Ball.Pawn;
	Pawn_Initialize(pawn, g_BallLayers, numberof(g_BallLayers), 6, g_BallActions);
	Pawn_InitializePhysics(pawn, PhysicsEventBall, PhysicsCollision, 16, 16);
	Pawn_SetAction(pawn, ACTION_BALL_IDLE);
	Pawn_SetPatternAddress(pawn, g_DataSprtBall);

	InitBallPosition();
}

//-----------------------------------------------------------------------------
// Update ball gameplay and display
void UpdateBall()
{
	// Apply gravity
	g_Ball.VelocityY += BALL_GRAVITY; // Apply gravity
	if (g_Ball.VelocityY > FALL_MAX_SPEED) // Clamp fall speed
		g_Ball.VelocityY = FALL_MAX_SPEED;

	// Select the player to tests
	Pawn* ballPawn = &g_Ball.Pawn;
	struct Character* ply = (ballPawn->PositionX < 128 - 8) ? &g_Player[0] : &g_Player[1];

	// Compute distance
	Pawn* plyPawn = &ply->Pawn;
	i8 dx = ballPawn->PositionX - plyPawn->PositionX;
	i8 dy = ballPawn->PositionY - plyPawn->PositionY;

	u16 sqrtDist = (dx * dx) + (dy * dy);
	if (sqrtDist < COL_DIST * COL_DIST)
	{
		DEBUG_PRINT("--- Collision ---\n");
		DEBUG_PRINT("Coll dx/dy: %i/%i\n", dx, dy);

		g_Ball.VelocityX = dx * 2;
		g_Ball.VelocityX += ply->VelocityX / 2;

		g_Ball.VelocityY = Q4_4_SET(-1.5f);
		// if (dy < 0)
		// 	g_Ball.VelocityY += dy * 2; // Math_SignedDiv4
		if (ply->VelocityY < 0)
			g_Ball.VelocityY += ply->VelocityY / 2;

		// g_Ball.VelocityX = dx * 2; // Math_SignedDiv4
		// g_Ball.VelocityX += ply->VelocityX / 2;
		// g_Ball.VelocityY = BOUNCE_FORCE;
		// // g_Ball.VelocityY -= ABS8(dy); // Math_SignedDiv4

		g_Ball.bFreeze = FALSE;
		Pawn_SetAction(ballPawn, ACTION_BALL_BUMP);
		Pawn_SetAction(plyPawn, ACTION_PLAYER_HIT);
		Rules_Pass();

		DEBUG_PRINT("Ball Vel: %i %i\n", g_Ball.VelocityX, g_Ball.VelocityY);
		DEBUG_BREAK();
	}

	// Update ball animation & physics
	u8 x = ballPawn->PositionX;
	UpdateCharacter(&g_Ball);

	if ((x <= 120) && (ballPawn->PositionX > 120))
		Rules_ChangeField(1);
	else if ((x > 120) && (ballPawn->PositionX <= 120))
		Rules_ChangeField(0);
}

//-----------------------------------------------------------------------------
// CLOUDS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Initialize cloud sprites
void InitCloud(u8 id)
{
	const struct Cloud* cloud = &g_Cloud[id];

	u8 sprt = cloud->Sprite;
	u8 pat = cloud->Pattern;
	VDP_SetSpriteSM1(sprt++, cloud->X, cloud->Y, pat + 0, COLOR_GRAY);
	VDP_SetSpriteSM1(sprt,   cloud->X, cloud->Y, pat + 4, COLOR_WHITE);
	g_CloudX[id] = cloud->X;
}

//-----------------------------------------------------------------------------
// Update cloud sprites position
void UpdateCloud(u8 id)
{
	const struct Cloud* cloud = &g_Cloud[id];

	if ((g_FrameCount & cloud->Mask) != 0)
		return;

	g_CloudX[id]--;
	u8 x = g_CloudX[id];
	if (g_CloudX[id] == -16)
	{
		u8 sprt = cloud->Sprite;
		VDP_SetSpriteColorSM1(sprt++, COLOR_GRAY);
		VDP_SetSpriteColorSM1(sprt,   COLOR_WHITE);
		x = g_CloudX[id] = 255;
	}
	else if (g_CloudX[id] < 0)
	{
		u8 sprt = cloud->Sprite;
		VDP_SetSpriteColorSM1(sprt++, VDP_SPRITE_EC | COLOR_GRAY);
		VDP_SetSpriteColorSM1(sprt,   VDP_SPRITE_EC | COLOR_WHITE);
		x = g_CloudX[id] + 32;
	}

	u8 sprt = cloud->Sprite;
	VDP_SetSpritePositionX(sprt++, x);
	VDP_SetSpritePositionX(sprt,   x);
}

//-----------------------------------------------------------------------------
// STATES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
bool State_LogoInit()
{
	Logo_Initialize();
	Game_SetState(State_LogoUpdate);
	return FALSE;
}

//-----------------------------------------------------------------------------
//
bool State_LogoUpdate()
{
	Halt();

	bool bFinish = Logo_Update();

	if (bFinish)
		Game_SetState(State_MenuInit);

	return FALSE;
}

//-----------------------------------------------------------------------------
// 
bool State_MenuInit()
{
	// Initialize display
	VDP_EnableDisplay(FALSE);
	VDP_SetColor(COLOR_BLACK);
	VDP_ClearVRAM();

	DrawLevel();
	
	// Initialize text
	Print_SetTextFont(g_Font, 192);
	Print_SetColor(0x1, 0xF);
	
	Menu_Initialize(g_Menus); // Initialize the menu
	Menu_DrawPage(MENU_MAIN); // Display the first page

	VDP_EnableDisplay(TRUE);

	Game_SetState(State_MenuUpdate);
	return FALSE; // Frame finished
}

//-----------------------------------------------------------------------------
// 
bool State_MenuUpdate()
{
	// Update menu
	Menu_Update();

	return FALSE; // Frame finished
}

//-----------------------------------------------------------------------------
// Data initialization state
bool State_GameInit()
{
	// Initialize display
	VDP_EnableDisplay(FALSE);
	VDP_SetColor(COLOR_BLACK);
	
	// Initialize background tiles
	DrawLevel();
	CollisionInit();

	// Initialize text
	Print_SetTextFont(g_Font, 192);
	Print_SetColor(0xF, 0x1);

	// Initialize sprite
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);
	Pletter_UnpackToVRAM(g_DataSprtCloud, VDP_GetSpritePatternTable() + 8 * 64);
	VDP_DisableSpritesFrom(22); // hide

	// Init cloud
	loop(i, numberof(g_Cloud))
		InitCloud(i);
	
	Rules_Init();

	// Init player 1 pawn (left)
	InitPlayer(0);

	// Init player 2 pawn (right)
	InitPlayer(1);

	// Init ball
	InitBall();

	VDP_EnableDisplay(TRUE);

	Game_SetState(State_KickOff);
	return FALSE; // Frame finished
}

//-----------------------------------------------------------------------------
//
bool State_KickOff()
{
	// Init player 1 pawn (left)
	InitPlayerPosition(0);

	// Init player 2 pawn (right)
	InitPlayerPosition(1);

	// Init ball
	InitBallPosition();

	Game_SetState(State_Game);
	return FALSE; // Frame finished
}

//-----------------------------------------------------------------------------
// Game update state
bool State_Game()
{
// VDP_SetColor(COLOR_DARK_BLUE);

	PROFILE_FRAME_START();

	PROFILE_SECTION_START(100, S_DRAW, "");
	Pawn_Draw(&g_Ball.Pawn);
	Pawn_Draw(&g_Player[0].Pawn);
	Pawn_Draw(&g_Player[1].Pawn);
	// Background horizon blink
	// if (g_bFlicker)
	// 	VDP_FillVRAM_16K(g_GameFrame & 1 ? 16 : 17, VDP_GetLayoutTable() + (HORIZON_H + 2) * 32, 32);
	PROFILE_SECTION_END(100, S_DRAW, "");

	PROFILE_SECTION_START(100, S_UPDATE, "");
	UpdatePlayer(&g_Player[0]);
	UpdatePlayer(&g_Player[1]);
	UpdateBall();
	loop(i, numberof(g_Cloud))
		UpdateCloud(i);
	PROFILE_SECTION_END(100, S_UPDATE, "");

	PROFILE_SECTION_START(100, S_INPUT, "");
	// Update input
	u8 row3 = Keyboard_Read(3);
	u8 row8 = Keyboard_Read(8);
	
	g_Player[0].Input = 0;
	if (IS_KEY_PRESSED(row3, KEY_D))
		g_Player[0].Input |= INPUT_LEFT;
	else if (IS_KEY_PRESSED(row3, KEY_G))
		g_Player[0].Input |= INPUT_RIGHT;
	if (IS_KEY_PRESSED(row3, KEY_F))
		g_Player[0].Input |= INPUT_JUMP;

	g_Player[1].Input = 0;
	if (IS_KEY_PRESSED(row8, KEY_RIGHT))
		g_Player[1].Input |= INPUT_RIGHT;
	else if (IS_KEY_PRESSED(row8, KEY_LEFT))
		g_Player[1].Input |= INPUT_LEFT;
	if (IS_KEY_PRESSED(row8, KEY_UP))
		g_Player[1].Input |= INPUT_JUMP;
	
	// if (IS_KEY_PUSHED(row3, g_PrevRow3, KEY_I))
	// 	g_bFlicker = 1 - g_bFlicker;

	g_PrevRow3 = row3;
	g_PrevRow8 = row8;

	u8 joy = Joystick_Read(JOY_PORT_1);
	if ((joy & JOY_INPUT_DIR_LEFT) == 0)
		g_Player[0].Input |= INPUT_LEFT;
	else if ((joy & JOY_INPUT_DIR_RIGHT) == 0)
		g_Player[0].Input |= INPUT_RIGHT;
	if ((joy & JOY_INPUT_TRIGGER_A) == 0)
		g_Player[0].Input |= INPUT_JUMP;
	
	joy = Joystick_Read(JOY_PORT_2);
	if ((joy & JOY_INPUT_DIR_LEFT) == 0)
		g_Player[1].Input |= INPUT_LEFT;
	else if ((joy & JOY_INPUT_DIR_RIGHT) == 0)
		g_Player[1].Input |= INPUT_RIGHT;
	if ((joy & JOY_INPUT_TRIGGER_A) == 0)
		g_Player[1].Input |= INPUT_JUMP;
	
	if (Keyboard_IsKeyPressed(KEY_ESC))
	{
		// Game_Exit();
		Game_SetState(State_MenuInit);
	}
	PROFILE_SECTION_END(100, S_INPUT, "");

	g_FrameCount++;

	PROFILE_FRAME_END();
	
// VDP_SetColor(COLOR_BLACK);

	return TRUE; // Frame finished
}

//-----------------------------------------------------------------------------
//
bool State_Pause()
{
	Game_SetState(State_Game);
	return TRUE; // Frame finished
}

//-----------------------------------------------------------------------------
//
bool State_Point()
{
	Pawn_SetDirty(&g_Ball.Pawn);
	Pawn_SetDirty(&g_Player[0].Pawn);
	Pawn_SetDirty(&g_Player[1].Pawn);

	Pawn_Draw(&g_Ball.Pawn);
	Pawn_Draw(&g_Player[0].Pawn);
	Pawn_Draw(&g_Player[1].Pawn);
	// Background horizon blink
	// if (g_bFlicker)
	// 	VDP_FillVRAM_16K(g_GameFrame & 1 ? 16 : 17, VDP_GetLayoutTable() + (HORIZON_H + 2) * 32, 32);

	Pawn_Update(&g_Ball.Pawn);
	Pawn_Update(&g_Player[0].Pawn);
	Pawn_Update(&g_Player[1].Pawn);
	loop(i, numberof(g_Cloud))
		UpdateCloud(i);

	g_StateTimer--;
	if ((g_StateTimer == 0) || (Keyboard_IsKeyPressed(KEY_ESC)))
	{
		VDP_SetColor(COLOR_BLACK);
		Rules_ChangeField(1 - g_Victorious);
		Game_SetState(State_KickOff);
	}

	g_FrameCount++;

	return TRUE; // Frame finished
}

//-----------------------------------------------------------------------------
//
bool State_VictoryInit()
{
	g_StateTimer = 500;

	InitPlayerPosition(0);
	InitPlayerPosition(1);

	Pawn_SetEnable(&g_Ball.Pawn, FALSE);
	Pawn_SetAction(&g_Player[g_Victorious].Pawn, ACTION_PLAYER_WIN);
	Pawn_SetAction(&g_Player[1 - g_Victorious].Pawn, ACTION_PLAYER_LOOSE);

	Game_SetState(State_VictoryUpdate);
	return TRUE; // Frame finished
}

//-----------------------------------------------------------------------------
//
bool State_VictoryUpdate()
{
	Pawn_SetDirty(&g_Player[0].Pawn);
	Pawn_SetDirty(&g_Player[1].Pawn);
	Pawn_Draw(&g_Player[0].Pawn);
	Pawn_Draw(&g_Player[1].Pawn);

	// Background horizon blink
	// if (g_bFlicker)
	// 	VDP_FillVRAM_16K(g_GameFrame & 1 ? 16 : 17, VDP_GetLayoutTable() + (HORIZON_H + 2) * 32, 32);

	Pawn_Update(&g_Player[0].Pawn);
	Pawn_Update(&g_Player[1].Pawn);
	loop(i, numberof(g_Cloud))
		UpdateCloud(i);

	g_StateTimer--;
	if ((g_StateTimer == 0) || (Keyboard_IsKeyPressed(KEY_ESC)))
	{
		Game_SetState(State_MenuInit);
	}

	g_FrameCount++;

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

	Game_SetState(State_LogoInit);
	Game_MainLoop(VDP_MODE_SCREEN2);

	DEBUG_LOG("End debug session!");
}