// __________________________
// ██▀█▀██▀▀▀█▀▀█▀█  ▄▄▄ ▄▄  │   ▄▄▄                ▄▄      
// █  ▄ █▄ ▀██▄ ▀▄█ ██   ██  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █▀▀ ▄█  █ █ ▀█▄█ ██▄▄│  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀──────────┘                 ▀▀
//  Game module sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"
#include "game.h"
#include "game_pawn.h"

//=============================================================================
// DEFINES
//=============================================================================

#define FORCE		24
#define GRAVITY		1
#define GROUND		112

// Library's logo
#define MSX_GL "\x02\x03\x04\x05"

// Prototype
bool State_Initialize();
bool State_Game();
bool State_Pause();

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font\font_mgl_sample8.h"
// Sprites by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "content\data_sprt_layer.h"
// Sinus & cosinus table
#include "mathtable\mt_trigo_64.inc"

// Pawn sprite layers
const Game_Sprite g_SpriteLayers[] =
{
	{ 0, 0, 0, 16,  0, COLOR_BLACK, PAWN_SPRITE_EVEN },
	{ 0, 0, 0, 16, 12, COLOR_BLACK, PAWN_SPRITE_ODD },
	{ 1, 0, 0, 16,  4, COLOR_WHITE, 0 },
	{ 2, 0, 0, 16,  8, COLOR_LIGHT_RED, 0 },
};

// Idle animation frames
const Game_Frame g_FramesIdle[] =
{
	{ 6,	48,	null },
	{ 7,	24,	null },
};

// Move animation frames
const Game_Frame g_FramesMove[] =
{
	{ 0,	4,	null },
	{ 1,	4,	null },
	{ 2,	4,	null },
	{ 3,	4,	null },
	{ 4,	4,	null },
	{ 5,	4,	null },
};

// Jump animation frames
const Game_Frame g_FramesJump[] =
{
	{ 3,	4,	null },
	{ 8,	4,	null },
};

// Fall animation frames
const Game_Frame g_FramesFall[] =
{
	{ 9,	4,	null },
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
{ //  Frames        Number                  Loop? Interrupt?
	{ g_FramesIdle, numberof(g_FramesIdle), true, true },
	{ g_FramesMove, numberof(g_FramesMove), true, true },
	{ g_FramesJump, numberof(g_FramesJump), true, true },
	{ g_FramesFall, numberof(g_FramesFall), true, true },
};

//=============================================================================
// MEMORY DATA
//=============================================================================

Game_Pawn g_PlayerPawn;

bool g_bFlicker = true;
bool g_bMoving = false;
bool g_bJumping = false;
i8   g_JumpForce;
u8   g_PrevRow8 = 0xFF;
u8   g_X = 16;
u8   g_Y = GROUND;

//-----------------------------------------------------------------------------
//
bool State_Initialize()
{
	// Initialize display
	VDP_EnableDisplay(false);
	VDP_SetColor(1);
	
	// Initialize pattern
	VDP_FillVRAM(0xFF, g_ScreenPatternLow, 0, 256*8); // pattern
	Print_SetTextFont(g_Font_MGL_Sample8, 16);
	Print_SetColor(0xF, 0x1);

	// Initialize color
	VDP_FillVRAM(0x51, g_ScreenColorLow + 0, 0, 1); // color
	VDP_FillVRAM(0x41, g_ScreenColorLow + 1, 0, 1); // color
	for(u8 i = 0; i < 24; ++i)
		VDP_FillVRAM(i < 16 ? 8 : 0, g_ScreenLayoutLow + i * 32, 0, 32); // name

	// Initialize sprite
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);
	VDP_LoadSpritePattern(g_DataSprtLayer, 0, 13*4*4);	
	VDP_SetSpriteSM1(3, 0, 208, 0, 0); // hide

	// Init player pawn
	GamePawn_Initialize(&g_PlayerPawn, g_SpriteLayers, numberof(g_SpriteLayers), g_AnimActions);
	GamePawn_SetPosition(&g_PlayerPawn, 16, GROUND);

	// Initialize text
	Print_SetPosition(0, 0);
	Print_DrawCharX(' ', 32);
	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL " GAME SAMPLE");
	Print_DrawLineH(0, 1, 32);

	VDP_EnableDisplay(true);

	Game_SetState(State_Game);
	return false; // Frame finished
}

//-----------------------------------------------------------------------------
//
bool State_Game()
{
	u8 act = ACTION_IDLE;
	if(g_bJumping && (g_JumpForce >= 0))
		act = ACTION_JUMP;
	else if(g_bJumping && (g_JumpForce < 0))
		act = ACTION_FALL;
	else if(g_bMoving)
		act = ACTION_MOVE;
	GamePawn_SetAction(&g_PlayerPawn, act);
	GamePawn_SetPosition(&g_PlayerPawn, g_X, g_Y);

	GamePawn_Update(&g_PlayerPawn);
	GamePawn_Draw(&g_PlayerPawn);

	u8 row8 = Keyboard_Read(8);
	if(IS_KEY_PRESSED(row8, KEY_RIGHT))
	{
		g_X++;
		g_bMoving = true;
	}
	else if(IS_KEY_PRESSED(row8, KEY_LEFT))
	{
		g_X--;
		g_bMoving = true;
	}
	else
		g_bMoving = false;
	
	if(g_bJumping)
	{
		g_Y -= (g_JumpForce / 4);
		g_JumpForce -= GRAVITY;
		if(g_Y > GROUND)
		{
			g_Y = GROUND;
			g_bJumping = false;
		}
	}
	else if(IS_KEY_PRESSED(row8, KEY_SPACE) || IS_KEY_PRESSED(row8, KEY_UP))
	{
		g_bJumping = true;
		g_JumpForce = FORCE;
	}

	if(IS_KEY_PRESSED(row8, KEY_HOME) && !IS_KEY_PRESSED(g_PrevRow8, KEY_HOME))
		g_bFlicker = 1 - g_bFlicker;
	
	g_PrevRow8 = row8;

	if(Keyboard_IsKeyPressed(KEY_ESC))
		Game_Exit();

		
	return true; // Frame finished
}

//-----------------------------------------------------------------------------
//
bool State_Pause()
{
	return true; // Frame finished
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