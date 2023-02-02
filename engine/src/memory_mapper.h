// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄                     
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▄███ ▄█▄█ ▄█▀▄ ██▄▀ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀█▄▄ ██ █ ▀█▄▀ ██    ▀██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                            ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

#define P_MEM_P0					0xFC // (252) selects the segment in page 0 (0000-3FFFh).
#define P_MEM_P1					0xFD // (253) selects the segment in page 1 (4000-7FFFh).
#define P_MEM_P2					0xFE // (254) selects the segment in page 2 (8000-BFFFh).
#define P_MEM_P3					0xFF // (255) selects the segment in page 3 (C000-FFFFh).

#define MEM_SETSEGMENT(page, seg)	MemMap_SetPage##page(seg)

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: MemMap_SetPage0
// Set memory mapper segment to switch in page 0.
//
// Parameters:
//   seg - Segment number to set
inline void MemMap_SetPage0(u8 seg) { seg; /* A */ __asm__("out (P_MEM_P0), a"); }

// Function: MemMap_SetPage1
// Set memory mapper segment to switch in page 1.
//
// Parameters:
//   seg - Segment number to set
inline void MemMap_SetPage1(u8 seg) { seg; /* A */ __asm__("out (P_MEM_P1), a"); }

// Function: MemMap_SetPage2
// Set memory mapper segment to switch in page 2.
//
// Parameters:
//   seg - Segment number to set
inline void MemMap_SetPage2(u8 seg) { seg; /* A */ __asm__("out (P_MEM_P2), a"); }

// Function: MemMap_SetPage3
// Set memory mapper segment to switch in page 3.
//
// Parameters:
//   seg - Segment number to set
inline void MemMap_SetPage3(u8 seg) { seg; /* A */ __asm__("out (P_MEM_P3), a"); }

// Function: MemMap_Initialize
// Initialize memory mapper
inline void MemMap_Initialize() __PRESERVES(b, c, d, e, h, l, iyl, iyh)
{
	__asm
		xor		a
		out		(P_MEM_P3), a
		inc		a
		out		(P_MEM_P2), a
		inc		a
		out		(P_MEM_P1), a
		inc		a
		out		(P_MEM_P0), a
	__endasm;
}