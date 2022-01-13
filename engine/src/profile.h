// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// Helper macros for OpenMSX profiler tool
//
// Ref: 
//  - https://hg.sr.ht/~grauw/neonlib/browse/tools/profile.tcl
//  - https://github.com/MartinezTorres/openMSX_profiler
//─────────────────────────────────────────────────────────────────────────────
#pragma once
#pragma sdcc_hash +

#include "core.h"

//-----------------------------------------------------------------------------
#if (PROFILE_MODE == PROFILE_OPENMSX_G)

	#define P_PROFILE_SECTION	#0x2C
	#define P_PROFILE_FRAME		#0x2D

	#define PROFILE_SECTION_START(id, level) if (level < PROFILE_LEVEL) {		\
		__asm																	\
			ld		a, #(id)													\
			in		a,(P_PROFILE_SECTION)										\
		__endasm; }
	
	#define PROFILE_SECTION_END(id, level) if (level < PROFILE_LEVEL) {			\
		__asm																	\
			ld		a, #(id)													\
			out		(P_PROFILE_SECTION), a										\
		__endasm; }

	#define PROFILE_FRAME_START()												\
		__asm																	\
			in		a, (P_PROFILE_FRAME)										\
		__endasm;
		
	#define PROFILE_FRAME_END()													\
		__asm																	\
			out		(P_PROFILE_FRAME), a										\
		__endasm;

//-----------------------------------------------------------------------------
#elif (PROFILE_MODE == PROFILE_OPENMSX_S)

	#define P_PROFILE_START		#0x2C
	#define P_PROFILE_END		#0x2D

	__sfr __at 0x2C g_PortStartProfile;
	__sfr __at 0x2D g_PortEndProfile;

	const c8* __at 0xF931 g_ProfileMsg;

	#define PROFILE_SECTION_START(id, level) if (level < PROFILE_LEVEL) { g_ProfileMsg = #id; g_PortStartProfile = 0; }
	#define PROFILE_SECTION_END(id, level)   if (level < PROFILE_LEVEL) { g_ProfileMsg = #id; g_PortEndProfile = 0; }

	#define PROFILE_FRAME_START()	{ g_ProfileMsg = "frame"; g_PortStartProfile = 0; }
	#define PROFILE_FRAME_END()		{ g_ProfileMsg = "frame"; g_PortEndProfile = 0; }

//-----------------------------------------------------------------------------
#else // if (PROFILE_MODE == PROFILE_DISABLE)

	#define PROFILE_SECTION_START(id, level)
	#define PROFILE_SECTION_END(id, level)

	#define PROFILE_FRAME_START()
	#define PROFILE_FRAME_END()

#endif