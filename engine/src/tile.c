// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄ ▄  ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ▄  ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │   ██  ██ ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Tile support for bitmap mode
//─────────────────────────────────────────────────────────────────────────────
#include "tile.h"
#include "vdp.h"

//=============================================================================
// MEMORY DATA
//=============================================================================

// Current pattern bank
u8  g_Tile_CurBank;
// Page number where pattern are located
u8  g_Tile_BankPage;
// Base bank Y coordinate
u16 g_Tile_BankBase;
// Page number to draw at
u8  g_Tile_DrawPage;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// 
void Tile_FillScreen(u8 value)
{
	VDP_CommandLMMV(0, g_Tile_DrawPage * 256, TILE_SCREEN_WIDTH, 212, value, VDP_OP_IMP);
}

//-----------------------------------------------------------------------------
// Draw a whole screen with using a tilemap
void Tile_DrawScreen(const u8* map)
{
	u8 x = 0;
	u8 y = 0;
	for(u16 i = 0; i < TILE_PER_ROW * TILE_PER_COLUMN; ++i)
	{
		Tile_DrawTileTrans(x, y, *map++);
		x++;
		if(x == TILE_PER_ROW)
		{
			x = 0;
			y++;
		}
	}
}

//-----------------------------------------------------------------------------
// 
void Tile_FillBank(u8 bank, u8 value)
{
	VDP_CommandLMMV(0, g_Tile_BankPage * 256 + bank * TILE_BANK_HEIGHT, TILE_BANK_WIDTH, TILE_BANK_HEIGHT, value, VDP_OP_IMP);
}

//-----------------------------------------------------------------------------
// 
void Tile_LoadBank(u8 bank, const u8* data, u16 size)
{
	u32 addr = g_Tile_BankPage * TILE_PAGE_SIZE;
	addr += bank * 256 * TILE_CELL_BYTES;

	VDP_WriteVRAM_128K(data, (u16)addr, addr >> 16, size * TILE_CELL_BYTES);
}
