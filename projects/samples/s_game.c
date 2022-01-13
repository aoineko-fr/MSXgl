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

//=============================================================================
// DEFINES
//=============================================================================

#define FORCE		8
#define GRAVITY		1
#define GROUND		112

// Library's logo
#define MSX_GL "\x02\x03\x04\x05\x06"

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
#include "content\data_sprt_16il.h"
// Sinus & cosinus table
#include "mathtable\mt_trigo_64.inc"

//=============================================================================
// MEMORY DATA
//=============================================================================

bool g_bFlicker = true;

u8   g_X = 16;
u8   g_Y = GROUND;

bool g_bMoving = false;
bool g_bJumping = false;
u8   g_JumpForce;
u8   g_PrevRow8 = 0xFF;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

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
	u8 chrSprt = 0;
	for(u8 j = 0; j < 6; j++) // Pattern 0-95
	{
		for(u8 i = 0; i < 6; i++)
		{
			VDP_LoadSpritePattern(g_DataSprt16il + (i * 2 +  0 + 24 * j) * 8, chrSprt++, 1);
			VDP_LoadSpritePattern(g_DataSprt16il + (i * 2 + 12 + 24 * j) * 8, chrSprt++, 1);
			VDP_LoadSpritePattern(g_DataSprt16il + (i * 2 +  1 + 24 * j) * 8, chrSprt++, 1);
			VDP_LoadSpritePattern(g_DataSprt16il + (i * 2 + 13 + 24 * j) * 8, chrSprt++, 1);	
		}
	}
	VDP_SetSpriteSM1(0, 16, 16,  0, COLOR_BLACK);
	VDP_SetSpriteSM1(1, 16, 16, 24, COLOR_WHITE);
	VDP_SetSpriteSM1(2, 16, 16, 48, COLOR_LIGHT_RED);
	VDP_SetSpriteSM1(3, 0, 208, 0, 0); // hide

	// Initialize text
	Print_SetPosition(0, 0);
	Print_DrawCharX(' ', 32);
	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL "  GAME SAMPLE");
	Print_DrawLineH(0, 1, 32);

	VDP_EnableDisplay(true);

	Game_SetState(State_Game);
	return false; // Frame finished
}

//-----------------------------------------------------------------------------
//
bool State_Game()
{
	u8 animId = g_bJumping ? 16 : g_bMoving ? ((g_GameFrame >> 2) % 6) * 4 : 16;
	bool bOdd = g_bFlicker ? (g_GameFrame & 1) == 1 : 0;

	VDP_SetSpriteSM1(0, g_X, g_Y, 72 * bOdd + animId, 0x01);
	VDP_SetSpriteSM1(1, g_X, g_Y, 24 + animId, 0x0F);
	VDP_SetSpriteSM1(2, g_X, g_Y, 48 + animId, 0x09);
	
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
		g_Y -= g_JumpForce;
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