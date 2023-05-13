// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█    │  ▄▄▄        ▄▄
// ██ ▀ █▄ ▀██▄ ▀ ▄█ ▄▀▀ █ │ ▀█▄ ▄▀██ ▄█▄█ ██▀▄ ██ ▄███
// █ █ █ ▀▀ ▄█ █ █ ▀▄█ █▄ │ ▄▄█▀ ▀▄██ ██ █ ██▀ ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘         ▀▀
// Software tiles module sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"
#include "game_pawn.h"
#include "tile.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL		"\x02\x03\x04\x05"

#define OOO			0

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// V9990 4-bits background data
#include "content/v9990/data_v9_bg.h"

// Sprites by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "content/data_sprt_layer.h"

// Fonts
#include "font/font_mgl_sample8.h"

// Sample level tiles map
const u8 g_TileMap[] =
{
	 OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 
	 OOO, OOO, OOO, 132, 133, 134, 135, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 
	 OOO, OOO, OOO, 164, 165, 166, 167, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 132, 133, 134, 135, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 136, 137, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 164, 165, 166, 167, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 168, 169, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 136, 137, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO, 136, 137, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 168, 169, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO, 168, 169, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 136, 137, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 168, 169, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO,  28,  29, OOO, OOO, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO,  32,  33,  34,  35, OOO, OOO, OOO, OOO, OOO, OOO,  59,  60,  61,  62, OOO, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO, OOO,  96,  97,  98,  99, OOO, OOO, OOO, OOO, OOO,  90,  91,  92,  93,  94,  95, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO,  32,  33,  34,  35, OOO, OOO, OOO, OOO, OOO, OOO,  64,  65,  66,  67, OOO, OOO, OOO, OOO, OOO, 122, 123, 124, 125, 126, 127, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO,  96,  97,  98,  99, OOO, OOO, OOO, OOO, OOO, OOO,  96,  97,  98,  99, OOO, OOO, OOO, OOO, OOO, 154, 155, 156, 157, 158, 159, OOO, 
	 OOO, OOO, OOO, OOO, OOO, OOO,  64,  65,  66,  67, OOO, OOO, OOO, OOO, OOO, OOO,  64,  65,  66,  67, OOO, OOO, OOO, OOO, OOO, OOO, OOO, 188, 189, OOO, OOO, OOO, 
	  33,  34,  33,  34,  33,  34,   8,  97,  98,  69,  33,  34,  33,  34,  33,  34,   8,  97,  98,  69,  33,  34,  33,  34,  33,  34,  33,  34,  33,  34,  33,  34, 
	  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65, 
	  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97, 
	  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65, 
	  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97, 
};

const u8 g_TreeTileMap[] =
{
	 OOO, OOO,  28,  29, OOO, OOO,
	 OOO,  59,  60,  61,  62, OOO,
	  90,  91,  92,  93,  94,  95,
	 122, 123, 124, 125, 126, 127,
	 154, 155, 156, 157, 158, 159,
	 OOO, OOO, 188, 189, OOO, OOO,
};

// Pawn sprite layers
const Game_Sprite g_SpriteLayers[] =
{
	{ 0, 0, 0,  1, PAWN_SPRITE_EVEN }, // Black
	{ 0, 0, 12, 1, PAWN_SPRITE_ODD }, // Black
	{ 0, 0, 4,  9, 0 }, // White
	{ 0, 0, 8,  15, 0 }, // Orange
};

// Idle animation frames
const Game_Frame g_FramesIdle[] =
{
	{ 6*16,	48,	NULL },
	{ 7*16,	24,	NULL },
};

// Move animation frames
const Game_Frame g_FramesMove[] =
{
	{ 0*16,	4,	NULL },
	{ 1*16,	4,	NULL },
	{ 2*16,	4,	NULL },
	{ 3*16,	4,	NULL },
	{ 4*16,	4,	NULL },
	{ 5*16,	4,	NULL },
};

// Jump animation frames
const Game_Frame g_FramesJump[] =
{
	{ 3*16,	4,	NULL },
	{ 8*16,	4,	NULL },
};

// Fall animation frames
const Game_Frame g_FramesFall[] =
{
	{ 9*16,	4,	NULL },
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
	{ g_FramesIdle, numberof(g_FramesIdle), TRUE, TRUE },
	{ g_FramesMove, numberof(g_FramesMove), TRUE, TRUE },
	{ g_FramesJump, numberof(g_FramesJump), TRUE, TRUE },
	{ g_FramesFall, numberof(g_FramesFall), TRUE, TRUE },
};

//=============================================================================
// MEMORY DATA
//=============================================================================

// V-synch variables
u8 g_VBlank = 0;
u8 g_Frame = 0;

// Player pawn data
Game_Pawn g_PlayerPawn;

//=============================================================================
// FUNCTIONS
//=============================================================================




//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// VBlank interrupt
void VBlankHook()
{
	g_VBlank = 1;
}

//-----------------------------------------------------------------------------
// Wait for V-Blank period
void WaitVBlank()
{
	while(g_VBlank == 0) {}
	g_VBlank = 0;
	g_Frame++;
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Programme entry point
void main()
{
	// Initialize VDP
	VDP_SetMode(VDP_MODE_SCREEN5);
	VDP_SetColor(6);
	VDP_SetPage(0);

	// Load tiles data
	Tile_SetBankPage(2);
	Tile_FillBank(0, 6);
	Tile_FillBank(1, 7);
	Tile_FillBank(2, 8);
	Tile_FillBank(3, 9);
	Tile_LoadBank(0, g_DataV9BG, sizeof(g_DataV9BG) / TILE_CELL_BYTES);
	Tile_LoadBank(2, g_DataV9BG, sizeof(g_DataV9BG) / TILE_CELL_BYTES);
	for(u8 i = 0; i < 15; ++i) // Convert V9990 palette (15 bits) to MSX2 palette (9 bits)
	{
		u8 R = g_DataV9BG_palette[i * 3 + 0] >> 2;
		u8 G = g_DataV9BG_palette[i * 3 + 1] >> 2;
		u8 B = g_DataV9BG_palette[i * 3 + 2] >> 2;
		u16 color = B | (R << 4) | (G << 8);
		VDP_SetPaletteEntry(i + 1, color);
	}

	// Draw level
	Tile_SetDrawPage(0);
	Tile_SelectBank(0);
	Tile_FillScreen(6);
	Tile_DrawMapChunk( 0, 15, g_TreeTileMap, 6, 6); // Draw tree tilemap
	Tile_DrawMapChunk( 3, 15, g_TreeTileMap, 6, 6);
	Tile_DrawMapChunk(11, 15, g_TreeTileMap, 6, 6);
	Tile_DrawScreen(g_TileMap); // Draw the whole screen tilemap
	Tile_DrawBlock(10, 8, 4, 4, 4, 2); // Draw a cloud (4x2 tiles)

	// Draw title
	Print_SetBitmapFont(g_Font_MGL_Sample8);
	Print_SetColor(0x11, 0x66);
	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL " Software Tile Sample");


	// Initialize sprite
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);
	VDP_LoadSpritePattern(g_DataSprtLayer, 0, 13 * 4 * 4);
	VDP_DisableSpritesFrom(3);

	// Init player pawn
	GamePawn_Initialize(&g_PlayerPawn, g_SpriteLayers, numberof(g_SpriteLayers), 0, g_AnimActions);
	GamePawn_SetPosition(&g_PlayerPawn, 100, 100);

	Bios_SetKeyClick(FALSE);
	Bios_SetHookCallback(H_TIMI, VBlankHook);
	VDP_EnableVBlank(TRUE);

	u8 X = 56, Y = 128;

	bool bContinue = TRUE;
	while(bContinue)
	{
		WaitVBlank();

		if(Keyboard_IsKeyPressed(KEY_1))
			VDP_SetPage(0);
		if(Keyboard_IsKeyPressed(KEY_2))
			VDP_SetPage(1);
		if(Keyboard_IsKeyPressed(KEY_3))
			VDP_SetPage(2);
		if(Keyboard_IsKeyPressed(KEY_4))
			VDP_SetPage(3);

		bool bMove = FALSE;
		if(Keyboard_IsKeyPressed(KEY_RIGHT))
		{
			X++;
			bMove = TRUE;
		}
		if(Keyboard_IsKeyPressed(KEY_LEFT))
		{
			X--;
			bMove = TRUE;
		}

		if(Keyboard_IsKeyPressed(KEY_DOWN))
		{
			Y++;
			bMove = TRUE;
		}
		if(Keyboard_IsKeyPressed(KEY_UP))
		{
			Y--;
			bMove = TRUE;
		}

		GamePawn_SetAction(&g_PlayerPawn, bMove ? ACTION_MOVE : ACTION_IDLE);
		GamePawn_SetPosition(&g_PlayerPawn, X, Y);
		GamePawn_Update(&g_PlayerPawn);
		GamePawn_Draw(&g_PlayerPawn);
	}

	Bios_Exit(0);
}