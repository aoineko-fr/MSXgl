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
#include "debug.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL		"\x02\x03\x04\x05"

#define OOO			0

// Physics values
#define FORCE		20
#define GRAVITY		1

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// V9990 4-bits background data
#include "content/tile/data_bg_4b.h"

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

// Sign-of-life animation
const u8 chrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// MEMORY DATA
//=============================================================================

// V-synch variables
u8 g_VBlank = 0;
u8 g_Frame = 0;

// Player pawn data
Game_Pawn g_PlayerPawn;

// Input variables
bool g_bMoving = FALSE;
bool g_bJumping = FALSE;
i8   g_VelocityY;
u8   g_LastEvent = 0;
u8   g_PrevRow8 = 0xFF;
i8   g_DX = 0;
i8   g_DY = 0;

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

// Physics callback
void PhysicsEvent(u8 event, u8 tile)
{
	tile;
	switch(event)
	{
	case PAWN_PHYSICS_BORDER_LEFT:
	case PAWN_PHYSICS_BORDER_RIGHT:
		DEBUG_LOG("Touch border!");
		DEBUG_BREAK();
		g_LastEvent = event;
		break;
	
	case PAWN_PHYSICS_COL_DOWN: // Handle downward collisions 
		if (g_bJumping)
		{
			DEBUG_LOGNUM("Velocity", g_VelocityY);
			DEBUG_LOG("Land on ground!");
		}
		g_bJumping = FALSE;
		break;
	
	case PAWN_PHYSICS_COL_UP: // Handle upward collisions
		g_VelocityY = 0;
		break;
	
	case PAWN_PHYSICS_FALL: // Handle falling
		if(!g_bJumping)
		{
			g_bJumping = TRUE;
			g_VelocityY = 0;
		}
		break;
	};
}

// Collision callback
bool PhysicsCollision(u8 tile)
{
	return (((tile >= 32) && (tile <= 35)) || ((tile >= 96) && (tile <= 99)) || ((tile >= 64) && (tile <= 67)));
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
	DEBUG_PRINT("Print: %c %s %i %d %u %f %x %X %%", '*', "Example game", -1234, -1234, -1, -0.123, -1, 0xABCD);
	DEBUG_PRINT("Align: [%04i] [% 4d] [%-4u] [%+i] [%04X] [%016b]", 123, 123, 123, 123, 123, 1234);

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
	Tile_LoadBank(0, g_DataBG4b, sizeof(g_DataBG4b) / TILE_CELL_BYTES);
	Tile_LoadBank(2, g_DataBG4b, sizeof(g_DataBG4b) / TILE_CELL_BYTES);
	for(u8 i = 0; i < 15; ++i)
		VDP_SetPaletteEntry(i + 1, *(u16*)&g_DataBG4b_palette[i*2]);

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
	GamePawn_SetTileMap(g_TileMap);
	GamePawn_SetPosition(&g_PlayerPawn, 100, 100);
	GamePawn_InitializePhysics(&g_PlayerPawn, PhysicsEvent, PhysicsCollision, 16, 16);

	Bios_SetKeyClick(FALSE);
	Bios_SetHookCallback(H_TIMI, VBlankHook);
	VDP_EnableVBlank(TRUE);

	bool bContinue = TRUE;
	while(bContinue)
	{
		WaitVBlank();

		Print_SetPosition(247, 0);
		Print_DrawChar(chrAnim[g_Frame & 0x03]);

		if(Keyboard_IsKeyPressed(KEY_1))
			VDP_SetPage(0);
		if(Keyboard_IsKeyPressed(KEY_2))
			VDP_SetPage(1);
		if(Keyboard_IsKeyPressed(KEY_3))
			VDP_SetPage(2);
		if(Keyboard_IsKeyPressed(KEY_4))
			VDP_SetPage(3);

		g_DX = 0;
		g_DY = 0;
		u8 row8 = Keyboard_Read(8);
		if(IS_KEY_PRESSED(row8, KEY_RIGHT))
		{
			g_DX++;
			g_bMoving = TRUE;
		}
		else if(IS_KEY_PRESSED(row8, KEY_LEFT))
		{
			g_DX--;
			g_bMoving = TRUE;
		}
		else
			g_bMoving = FALSE;
		
		if(g_bJumping)
		{
			g_DY -= g_VelocityY / 4;
			
			g_VelocityY -= GRAVITY;
			if(g_VelocityY < -FORCE)
				g_VelocityY = -FORCE;

		}
		else if(IS_KEY_PRESSED(row8, KEY_SPACE) || IS_KEY_PRESSED(row8, KEY_UP))
		{
			g_bJumping = TRUE;
			g_VelocityY = FORCE;
		}
		g_PrevRow8 = row8;

		// Update player animation & physics
		u8 act = ACTION_IDLE;
		if(g_bJumping && (g_VelocityY >= 0))
			act = ACTION_JUMP;
		else if(g_bJumping)
			act = ACTION_FALL;
		else if(g_bMoving)
			act = ACTION_MOVE;

		GamePawn_SetAction(&g_PlayerPawn, act);
		GamePawn_SetMovement(&g_PlayerPawn, g_DX, g_DY);
		GamePawn_Update(&g_PlayerPawn);
		GamePawn_Draw(&g_PlayerPawn);
	}

	DEBUG_LOG("End debug session!");
	Bios_Exit(0);
}