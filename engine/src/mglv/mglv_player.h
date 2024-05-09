// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄  ▄▄ ▄  ▄▄▄  ▄ ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██  ██ █ ██   ██▀█
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄ ▀█▀  ▀█▄█ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "msxgl.h"

//-----------------------------------------------------------------------------
// CHECK OPTIONS
//-----------------------------------------------------------------------------

// MGLV_HAS_HEADER
#ifndef MGLV_HAS_HEADER
	#warning MGLV_HAS_HEADER is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define MGLV_HAS_HEADER			TRUE
#endif

//=============================================================================
// DEFINES
//=============================================================================

// Flag values
#define MGLV_VERSION				0
#define MGLV_VERSION_MASK			0b00001111
#define MGLV_SEGMENT_MASK			0b00010000
#define MGLV_SEGMENT_8K				0b00000000
#define MGLV_SEGMENT_16K			0b00010000
#define MGLV_IMAGE_HEADER			0b10000000

// Image values
#define MGLV_SCR_MODE_MASK			0b00000111
#define MGLV_SCR_MODE_TEXT			0 // Screen 0, width 40 & 80
#define MGLV_SCR_MODE_GM1			1 // Screen 1
#define MGLV_SCR_MODE_GM2			2 // Screen 2 & 4
#define MGLV_SCR_MODE_MC			3 // Screen 3
#define MGLV_SCR_MODE_BITMAP		4 // Screen 5 ~ 8
#define MGLV_SCR_MODE_YAE			5 // Screen 10 (GM7 + YJK + YAE)
#define MGLV_SCR_MODE_YJK			6 // Screen 12 (GM7 + YJK)
#define MGLV_SCR_MODE_7				7
#define MGLV_SCR_WIDTH_MASK			0b00100000
#define MGLV_SCR_WIDTH_256			0b00000000
#define MGLV_SCR_WIDTH_512			0b00100000
#define MGLV_SCR_BPP_MASK			0b11000000
#define MGLV_SCR_BPP_1				0b00000000
#define MGLV_SCR_BPP_2				0b01000000
#define MGLV_SCR_BPP_4				0b10000000
#define MGLV_SCR_BPP_8				0b11000000

// Replay values
#define MGLV_FRAME_SKIP_MASK		0b00001111
#define MGLV_LOOP					0b01000000
#define MGLV_FREQ_MASK				0b10000000
#define MGLV_FREQ_50HZ				0b00000000
#define MGLV_FREQ_60HZ				0b10000000

// Replay values
#define MGLV_SKIP_COUNT_MIN			1 // Minimum number of consecutive unchanged bytes to start a "skip" chunk
#define MGLV_FILL_COUNT_MIN			2 // Minimum number of consecutive identical bytes to start a "fill" chunk

// MGLV file header
struct MGLV_Header
{
	u8 Sign[3];
	u8 Flag;
	//----------------------------------------
	u8 Image;
	u8 Replay;
	u8 Width;
	u8 Height;
};

// MGLV header mode
enum MGLV_HEADER_MODE
{
	MGLV_HEADER_NONE = 0,
	MGLV_HEADER_SHORT,
	MGLV_HEADER_FULL,
};

// MGLV tables
enum MGLV_TABLE
{
	MGLV_TAB_PATTERN = 0,
	MGLV_TAB_COLOR,
	MGLV_TAB_LAYOUT,
	//----------------------------------------
	MGLV_TAB_MAX,
	//----------------------------------------
	MGLV_TAB_BITMAP = MGLV_TAB_PATTERN,
};

// MGLV commands
enum MGLV_COMAND
{
	MGLV_CMD_END_VIDEO = 0,	// 00					End of data
	MGLV_CMD_END_SEGMENT,	// 01					End of segment(increment segment index and reset data pointer)
	MGLV_CMD_END_LINE,		// 02					End of line
	MGLV_CMD_END_TABLE,		// x3					End table and switch to next one
							//						  0: Pattern table(default)
							//						  1: Color table(screen mode 1, 2 and 3)
							//						  2: Layout table(tile based modes)
	MGLV_CMD_SKIP_4B,		// n4					Skip n bytes(1 - 15)
	MGLV_CMD_SKIP_8B,		// 05	nn				Skip nn bytes(1 - 255)
	MGLV_CMD_SKIP_16B,		// 06	nnnn			Skip nnnn bytes(1 - 65535)
	MGLV_CMD_SKIP_FRAME,	// 07					Skip a frame / End of frame
	MGLV_CMD_FILL_4B,		// n8	vv				Fill n bytes(1 - 15) with vv value
	MGLV_CMD_FILL_8B,		// 09	vv,nn			Fill nn bytes(1 - 255) with vv value
	MGLV_CMD_FILL_16B,		// 0A	vv,nnnn			Fill nnnn bytes(1 - 65535) with vv value
	MGLV_CMD_FILL_FRAME,	// 0B	vv				Fill a full frame with vv value
	MGLV_CMD_COPY_4B,		// nC	vv[n]			Copy n bytes(1 - 15) from vv[n] data table
	MGLV_CMD_COPY_8B,		// 0D	nn,vv[nn]		Copy nn bytes(1 - 255) from vv[nn] data table
	MGLV_CMD_COPY_16B,		// 0E	nnnn,vv[nnnn]	Copy nnnn bytes(1 - 65535) from vv[nnnn] data table
	MGLV_CMD_COPY_FRAME,	// 0F	vv[]			Copy a full frame from data table(raw frame)
	//----------------------------------------
	MGLV_CMD_MAX,
	//----------------------------------------
	MGLV_CMD_END_FRAME = MGLV_CMD_SKIP_FRAME,
};

//=============================================================================
// VARIABLES
//=============================================================================

// Extern data
extern const struct MGLV_Header* g_MGLV_Header;
extern const u8* g_MGLV_Start;
extern const u8* g_MGLV_Pointer;
extern u16 g_MGLV_VRAMAddr;

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: MGLV_Play
// Start movie playback.
bool MGLV_Play(const void* addr);

// Function: MGLV_Decode
// Decode a frame of movie
void MGLV_Decode();
