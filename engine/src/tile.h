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
	#warning TILE_WIDTH is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define TILE_WIDTH				8
#endif

// TILE_HEIGHT
#ifndef TILE_HEIGHT
	#warning TILE_HEIGHT is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define TILE_HEIGHT				8
#endif

// TILE_BPP
#ifndef TILE_BPP
	#warning TILE_BPP is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define TILE_BPP				4
#endif

// TILE_SCREEN_WIDTH
#ifndef TILE_SCREEN_WIDTH
	#warning TILE_SCREEN_WIDTH is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define TILE_SCREEN_WIDTH		256
#endif

//=============================================================================
// DEFINES
//=============================================================================

// 
#define TILE_PER_ROW				(TILE_SCREEN_WIDTH / TILE_WIDTH)
#define TILE_PER_COLUMN				(212 / TILE_HEIGHT)
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

// Set page number where pattern are located
inline void Tile_SetBankPage(u8 page) { g_Tile_BankPage = page; g_Tile_BankBase = g_Tile_BankPage * 256 + g_Tile_CurBank * TILE_BANK_HEIGHT; }

// Set current pattern bank
inline void Tile_SelectBank(u8 bank) { g_Tile_CurBank = bank; g_Tile_BankBase = g_Tile_BankPage * 256 + g_Tile_CurBank * TILE_BANK_HEIGHT; }

// Set page number to draw at
inline void Tile_SetDrawPage(u8 page) { g_Tile_DrawPage = page; }

// Draw a tile at the given coordinate
inline void Tile_DrawTileTrans(u8 x, u8 y, u8 tile) { VDP_CommandLMMM((tile % TILE_PER_ROW) * TILE_WIDTH, g_Tile_BankBase + (tile / TILE_PER_ROW) * TILE_HEIGHT, x * TILE_WIDTH, g_Tile_DrawPage * 256 + y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, VDP_OP_TIMP); }

// Draw a tile at the given coordinate
inline void Tile_DrawTileOpac(u8 x, u8 y, u8 tile) { VDP_CommandHMMM((tile % TILE_PER_ROW) * TILE_WIDTH, g_Tile_BankBase + (tile / TILE_PER_ROW) * TILE_HEIGHT, x * TILE_WIDTH, g_Tile_DrawPage * 256 + y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT); }

// 
void Tile_FillScreen(u8 value);

// Draw a whole screen with using a tilemap
void Tile_DrawScreen(const u8* map);

// 
void Tile_FillBank(u8 bank, u8 value);

// 
void Tile_LoadBank(u8 bank, const u8* data, u16 size);