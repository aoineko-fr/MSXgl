// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄ ▄  ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ▄  ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │   ██  ██ ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Tile support for bitmap mode
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"
#include "vdp.h"

//=============================================================================
// OPTIONS VALIDATION
//=============================================================================

// TILE_WIDTH
#ifndef TILE_WIDTH
	#warning TILE_WIDTH is not defined in "msxgl_config.h"! Default value will be used: 8
	#define TILE_WIDTH				8
#endif

// TILE_HEIGHT
#ifndef TILE_HEIGHT
	#warning TILE_HEIGHT is not defined in "msxgl_config.h"! Default value will be used: 8
	#define TILE_HEIGHT				8
#endif

// TILE_BPP
#ifndef TILE_BPP
	#warning TILE_BPP is not defined in "msxgl_config.h"! Default value will be used: 4
	#define TILE_BPP				4
#endif

// TILE_SCREEN_WIDTH
#ifndef TILE_SCREEN_WIDTH
	#warning TILE_SCREEN_WIDTH is not defined in "msxgl_config.h"! Default value will be used: 256
	#define TILE_SCREEN_WIDTH		256
#endif

// TILE_SCREEN_HEIGHT
#ifndef TILE_SCREEN_HEIGHT
	#warning TILE_SCREEN_HEIGHT is not defined in "msxgl_config.h"! Default value will be used: 212
	#define TILE_SCREEN_HEIGHT		212
#endif

// TILE_USE_SKIP
#ifndef TILE_USE_SKIP
	#warning TILE_USE_SKIP is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define TILE_USE_SKIP			TRUE
#endif

// TILE_SKIP_INDEX
#ifndef TILE_SKIP_INDEX
	#warning TILE_SKIP_INDEX is not defined in "msxgl_config.h"! Default value will be used: 0
	#define TILE_SKIP_INDEX			0
#endif

//=============================================================================
// DEFINES
//=============================================================================

// Precomputed values
#define TILE_PER_ROW				(TILE_SCREEN_WIDTH / TILE_WIDTH)
#define TILE_PER_COLUMN				(TILE_SCREEN_HEIGHT / TILE_HEIGHT)
#define TILE_CELL_BYTES				(TILE_WIDTH * TILE_HEIGHT * TILE_BPP / 8)
#define TILE_PAGE_SIZE				(u32)((u32)256 * (u32)256 * (u32)TILE_BPP / (u32)8)
#define TILE_BANK_WIDTH				TILE_SCREEN_WIDTH
#define TILE_BANK_HEIGHT			(TILE_WIDTH * TILE_HEIGHT * 256 / TILE_SCREEN_WIDTH)

// Current pattern bank
extern u8  g_Tile_CurBank;
// Page number where pattern are located
extern u8  g_Tile_BankPage;
// Base bank Y coordinate
extern u16 g_Tile_BankBase;
// Page number to draw at
extern u8  g_Tile_DrawPage;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Bank
// Bank managment functions.

// Function: Tile_SetBankPage
// Set page number where pattern are located in VRAM. Must be called before load data there.
//
// Parameters:
//   page - VRAM page number (0-3)
inline void Tile_SetBankPage(u8 page) { g_Tile_BankPage = page; g_Tile_BankBase = g_Tile_BankPage * 256 + g_Tile_CurBank * TILE_BANK_HEIGHT; }

// Function: Tile_LoadBankEx
// Load data to the given bank (in the selected page ; see <Tile_SetBankPage>).
//
// Parameters:
//   bank - Bank number in current page (number of bank depend of the size of 256 tiles)
//   data - Pointer to source data (format depends on screen mode)
//   offset - Tiles offset in the bank
//   num - Number of tiles to load
void Tile_LoadBankEx(u8 bank, const u8* data, u16 offset, u16 num);

// Function: Tile_LoadBank
// Load data to the given bank (in the selected page ; see <Tile_SetBankPage>).
//
// Parameters:
//   bank - Bank number in current page (number of bank depend of the size of 256 tiles)
//   data - Pointer to source data (format depends on screen mode)
//   num - Number of tiles to load
inline void Tile_LoadBank(u8 bank, const u8* data, u16 num) { Tile_LoadBankEx(bank, data, 0, num); }

// Function: Tile_FillBank
// Fill the whole bank with the given color.
//
// Parameters:
//   bank - Bank number in current page (number of bank depend of the size of 256 tiles)
//   value - color to fill the whole bank
inline void Tile_FillBank(u8 bank, u8 value) { VDP_CommandLMMV(0, g_Tile_BankPage * 256 + bank * TILE_BANK_HEIGHT, TILE_BANK_WIDTH, TILE_BANK_HEIGHT, value, VDP_OP_IMP); }

//-----------------------------------------------------------------------------
// Group: Draw
// Tiles draw functions.

// Function: Tile_SelectBank
// Set current pattern bank. Must be called at least once before using drawing functions.
//
// Parameters:
//   bank - Bank number in current page (number of bank depend of the size of 256 tiles)
inline void Tile_SelectBank(u8 bank) { g_Tile_CurBank = bank; g_Tile_BankBase = g_Tile_BankPage * 256 + g_Tile_CurBank * TILE_BANK_HEIGHT; }

// Function: Tile_SetDrawPage
// Set page number where to draw in VRAM. Must be called at least once before using drawing functions.
//
// Parameters:
//   page - VRAM page number (0-3)
inline void Tile_SetDrawPage(u8 page) { g_Tile_DrawPage = page; }

// Function: Tile_DrawTile
// Draw a tile at the given coordinate.
//
// Parameters:
//   x - X coordinate
//   y - Y coordinate
//   tile - Tile index to draw
inline void Tile_DrawTile(u8 x, u8 y, u8 tile) { VDP_CommandLMMM((tile % TILE_PER_ROW) * TILE_WIDTH, g_Tile_BankBase + (tile / TILE_PER_ROW) * TILE_HEIGHT, x * TILE_WIDTH, g_Tile_DrawPage * 256 + y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, VDP_OP_TIMP); }

// Function: Tile_FillTile
// Fill a cell at the given position with the given color.
//
// Parameters:
//   x - X coordinate
//   y - Y coordinate
//   color - Color to fill
inline void Tile_FillTile(u8 x, u8 y, u8 color) { VDP_CommandLMMV(x * TILE_WIDTH, g_Tile_DrawPage * 256 + y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, color, VDP_OP_IMP); }

// Function: Tile_DrawMapChunk
// Drawn a chunk of tilemap at a given coordinate.
//
// Parameters:
//   x - Destination X coordinate
//   y - Destination Y coordinate
//   map - Pointer to source tilemap data
//   width - Width of the tilemap
//   height - Height of the tilemap
void Tile_DrawMapChunk(u8 x, u8 y, const u8* map, u8 width, u8 height);

// Function: Tile_DrawBlock
// Drawn a block of tilemap at a given coordinate.
//
// Parameters:
//   dx - Destination X coordinate
//   dy - Destination Y coordinate
//   sx - Source X coordinate
//   sy - Source Y coordinate
//   width - Width to draw
//   height - Height to draw
inline void Tile_DrawBlock(u8 dx, u8 dy, u8 sx, u8 sy, u8 width, u8 height) { VDP_CommandLMMM(sx * TILE_WIDTH, g_Tile_BankBase + sy * TILE_HEIGHT, dx * TILE_WIDTH, g_Tile_DrawPage * 256 + dy * TILE_HEIGHT, width * TILE_WIDTH, height * TILE_HEIGHT, VDP_OP_TIMP); }

// Function: Tile_DrawScreen
// Draw a whole screen with using a tilemap.
//
// Parameters:
//   map - Pointer to source tilemap data
void Tile_DrawScreen(const u8* map);

// Function: Tile_FillScreen
// Fill the screen with the given color.
//
// Parameters:
//   color - Color to fill the screen with
inline void Tile_FillScreen(u8 color) { VDP_CommandLMMV(0, g_Tile_DrawPage * 256, TILE_SCREEN_WIDTH, TILE_SCREEN_HEIGHT, color, VDP_OP_IMP); }