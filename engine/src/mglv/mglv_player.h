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

#define MGLV_HAS_HEADER				TRUE

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

// Offset		Size	Desc.		Value
// --------------------------------------------------------
// 0x0000		3		Signature	"MGV"
// ........................................................
// 0x0003		1		Flag		7  6  5  4  3  2  1  0
// 									│        │  └──┴──┴──┴── File format version (0-15)
// 									│        └────────────── Segment size (0: 8 KB, 1: 16 KB)
// 									└─────────────────────── Include video format data (4 bytes)
// ........................................................
// (0x0004)		1		Image		7  6  5  4  3  2  1  0
// 									│  │  │        └──┴──┴── Screen mode (0-7)
// 									│  │  └───────────────── Screen width (0: 256 px, 1: 512 px)
// 									└──┴──────────────────── Bit per pixel (0: 1-bit, 1: 2-bit, 2: 4-bit, 3: 8-bit)
// 									Screen mode:
// 										0: TEXT - Screen 0, width 40 & 80
// 										1: GM1 - Screen 1
// 										2: GM2 - Screen 2 & 4
// 										3: MC - Screen 3
// 										4: BITMAP - Screen 5 ~ 8
// 										5: YAE - Screen 10 (GM7 + YJK + YAE)
// 										6: YJK - Screen 12 (GM7 + YJK)
// ........................................................
// (0x0005)		1	Replay			7  6  5  4  3  2  1  0
//	 								│  │        └──┴──┴──┴── Frame skip (0-15)
// 									│  └──────────────────── Loop (0: No, 1: Yes)
// 									└─────────────────────── Frequency (0: 50 Hz, 1: 60 Hz)
// 									At 50 Hz, a frame skip of 1 means 25 fps.
// 									At 60 Hz, a frame skip of 5 means 10 fps.
// ........................................................
// (0x0006)		1	Width			1-256 (or 2-512 for Screen mode 6 and 7)
// ........................................................
// (0x0007)		1	Height			1-256

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

// Event callback
typedef void (*MGLV_EventCallback)(u8 cmd);

//=============================================================================
// VARIABLES
//=============================================================================

// Extern data
extern const struct MGLV_Header* g_MGLV_Header;
extern const u8* g_MGLV_Start;
extern const u8* g_MGLV_Pointer;
extern u16 g_MGLV_VRAMAddr;
extern MGLV_EventCallback g_MGLV_EventCallback;
extern u8 g_MGLV_Timer;
extern bool g_MGLV_Loop;

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: MGLV_Init
// Initialize the movie player.
//
// Parameters:
//   cb - Event callback function
inline void MGLV_Init(MGLV_EventCallback cb) { g_MGLV_EventCallback = cb; }

// Function: MGLV_SetTimer
// Set frame duration in number of screen refresh.
//
// Parameters:
//   timer - Duration of a frame
inline void MGLV_SetFrameDuration(u8 timer) { g_MGLV_Timer = timer; }

// Function: MGLV_SetLoop
// Set looping flag.
//
// Parameters:
//   loop - TRUE if movie should loop when reaching the end
inline void MGLV_SetLoop(bool loop) { g_MGLV_Loop = loop; }

// Function: MGLV_IsLooping
// Get looping flag.
//
// Parameters:
//   FALSE if the loop flag is set
inline bool MGLV_IsLooping() { return g_MGLV_Loop; }

// Function: MGLV_Play
// Start movie playback.
//
// Parameters:
//   addr - Address of the movie data
//
// Returns:
//   TRUE if initialization is successful
bool MGLV_Play(const void* addr);

// Function: MGLV_VBlankHandler
// Function to be called in the interruption handler
void MGLV_VBlankHandler();

// Function: MGLV_Decode
// Decode a frame of movie
void MGLV_Decode();

#if (MGLV_HAS_HEADER)

// Function: MGLV_GetHeaderType
// Get the initialized movie's header type.
// Note: MGLV_Init() function must be called first.
//
// Return:
//   Header type MGLV_HEADER_NONE, MGLV_HEADER_FULL or MGLV_HEADER_SHORT.
inline enum MGLV_HEADER_MODE MGLV_GetHeaderType()
{
	#if (MGLV_HAS_HEADER)
		return (g_MGLV_Header->Flag & MGLV_IMAGE_HEADER) ? MGLV_HEADER_FULL : MGLV_HEADER_SHORT;
	#else
		return MGLV_HEADER_NONE;
	#endif
}

// Function: MGLV_GetVersion
// Get the initialized movie's format version.
// Note: MGLV_Init() function must be called first.
//
// Return:
//   Format version (0-15).
inline u8 MGLV_GetVersion() { return g_MGLV_Header->Flag & MGLV_VERSION_MASK; }

// Function: MGLV_GetSegmentSize
// Get the initialized movie's segment size.
// Note: MGLV_Init() function must be called first.
//
// Return:
//   Segment size in kilo-bytes.
inline u8 MGLV_GetSegmentSize() { return (g_MGLV_Header->Flag & MGLV_SEGMENT_16K) ? 16 : 8; }

#endif // (MGLV_HAS_HEADER)