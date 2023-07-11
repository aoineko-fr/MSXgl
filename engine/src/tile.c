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
u8  g_Tile_CurBank = 0;
// Page number where pattern are located
u8  g_Tile_BankPage = 2;
// Base bank Y coordinate
u16 g_Tile_BankBase;
// Page number to draw at
u8  g_Tile_DrawPage = 0;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Load data to the given bank (in the selected page ; see <Tile_SetBankPage>).
void Tile_LoadBankEx(u8 bank, const u8* data, u16 offset, u16 num)
{
	u32 addr = g_Tile_BankPage * TILE_PAGE_SIZE;
	addr += ((bank * 256) + offset) * TILE_CELL_BYTES;

	VDP_WriteVRAM_128K(data, (u16)addr, addr >> 16, num * TILE_CELL_BYTES);
}

//-----------------------------------------------------------------------------
// Drawn a chunk of tilemap at a given coordinate.
void Tile_DrawMapChunk(u8 dx, u8 dy, const u8* map, u8 width, u8 height)
{
	g_VDP_Command.NX = TILE_WIDTH;
	g_VDP_Command.NY = TILE_HEIGHT;
	g_VDP_Command.ARG = 0; 
	g_VDP_Command.CMD = (u8)(VDP_CMD_LMMM + VDP_OP_TIMP);

	u8 x = dx;
	u8 y = dy;
	g_VDP_Command.DY = g_Tile_DrawPage * 256 + y * TILE_HEIGHT;
	for(u16 j = 0; j < height; ++j)
	{
		x = dx;
		g_VDP_Command.DX = dx * TILE_WIDTH;
		for(u16 i = 0; i < width; ++i)
		{
			#if (TILE_USE_SKIP)
			if(*map != TILE_SKIP_INDEX) // only draw file not the transparency
			#endif
			{
				g_VDP_Command.SX = (*map % TILE_PER_ROW) * TILE_WIDTH;
				g_VDP_Command.SY = g_Tile_BankBase + (*map / TILE_PER_ROW) * TILE_HEIGHT;
				VPD_CommandSetupR32();
			}

			map++;		
			x++;
			g_VDP_Command.DX += TILE_WIDTH;
		}
		y++;
		g_VDP_Command.DY += TILE_HEIGHT;
	}
} 

//-----------------------------------------------------------------------------
// Draw a whole screen with using a tilemap.
void Tile_DrawScreen(const u8* map)
{
	g_VDP_Command.NX = TILE_WIDTH;
	g_VDP_Command.NY = TILE_HEIGHT;
	g_VDP_Command.ARG = 0; 
	g_VDP_Command.CMD = (u8)(VDP_CMD_LMMM + VDP_OP_TIMP);

	g_VDP_Command.DY = g_Tile_DrawPage * 256;
	for(u16 j = 0; j < TILE_PER_COLUMN; ++j)
	{
		g_VDP_Command.DX = 0;
		for(u16 i = 0; i < TILE_PER_ROW; ++i)
		{
			#if (TILE_USE_SKIP)
			if(*map != TILE_SKIP_INDEX) // only draw file not the transparency
			#endif
			{
				g_VDP_Command.SX = (*map % TILE_PER_ROW) * TILE_WIDTH;
				g_VDP_Command.SY = g_Tile_BankBase + (*map / TILE_PER_ROW) * TILE_HEIGHT;
				VPD_CommandSetupR32();
			}

			map++;		
			g_VDP_Command.DX += TILE_WIDTH;
		}
		g_VDP_Command.DY += TILE_HEIGHT;
	}
}


