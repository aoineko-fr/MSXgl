// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄  ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄█▀▀ ██▄▀ ▄█▀▄ ██  ██  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀█▄▄ ██   ▀█▄▀ ▀█▄ ▀█▄ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#pragma once
#include "core.h"
#include "vdp.h"

//=============================================================================
// DEFINES
//=============================================================================

#define SCROLL_SRC_X			0
#define SCROLL_SRC_Y			0
#define SCROLL_SRC_W			128
#define SCROLL_SRC_H			24

#define SCROLL_DST_X			0
#define SCROLL_DST_Y			2
#define SCROLL_DST_W			32
#define SCROLL_DST_H			22

#define SCROLL_WRAP				1

#define SCROLL_SKIP_NONE		0
#define SCROLL_SKIP_1			0x1
#define SCROLL_SKIP_2			0x3
#define SCROLL_SKIP_4			0x7
#define SCROLL_SKIP_8			0xF

#define SCROLL_SKIP				SCROLL_SKIP_NONE

#if (MSX_VERSION >= MSX_2)
	#define SCROLL_ADJUST		1
	#define SCROLL_MASK			1
	#define SCROLL_MASK_ID		0
#endif

extern u8  g_Scroll_Count;
extern u16 g_Scroll_OffsetX;
extern u16 g_Scroll_OffsetY;
extern u16 g_Scroll_Map;

//=============================================================================
// PROTOTYPE
//=============================================================================

// 
void Scroll_Initialize(u16 map);

// 
void Scroll_SetOffset(i8 offset);

// 
void Scroll_Update();