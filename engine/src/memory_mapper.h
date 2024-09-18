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
#define P_MEM_P3					0xFF // (255) selects the segment in page 3 (C000-FFFEh).

#define MEM_SETSEGMENT(page, seg)	MemMap_SetPage##page(seg)

__sfr __at(P_MEM_P0)				g_PortMemPage0; // Port to access memory mapper for memory page 0 (0000-3FFFh)
__sfr __at(P_MEM_P1)				g_PortMemPage1; // Port to access memory mapper for memory page 1 (4000-7FFFh)
__sfr __at(P_MEM_P2)				g_PortMemPage2; // Port to access memory mapper for memory page 2 (8000-BFFFh)
__sfr __at(P_MEM_P3)				g_PortMemPage3; // Port to access memory mapper for memory page 3 (C000-FFFEh)

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: MemMap_SetPage0
// Set memory mapper segment to switch in page 0.
//
// Parameters:
//   seg - Segment number to set
inline void MemMap_SetPage0(u8 seg) { g_PortMemPage0 = seg; }

// Function: MemMap_SetPage1
// Set memory mapper segment to switch in page 1.
//
// Parameters:
//   seg - Segment number to set
inline void MemMap_SetPage1(u8 seg) { g_PortMemPage1 = seg; }

// Function: MemMap_SetPage2
// Set memory mapper segment to switch in page 2.
//
// Parameters:
//   seg - Segment number to set
inline void MemMap_SetPage2(u8 seg) { g_PortMemPage2 = seg; }

// Function: MemMap_SetPage3
// Set memory mapper segment to switch in page 3.
//
// Parameters:
//   seg - Segment number to set
inline void MemMap_SetPage3(u8 seg) { g_PortMemPage3 = seg; }

// Function: MemMap_Initialize
// Initialize memory mapper
inline void MemMap_Initialize()
{
	MemMap_SetPage3(0);
	MemMap_SetPage2(1);
	MemMap_SetPage1(2);
	MemMap_SetPage0(3);
}