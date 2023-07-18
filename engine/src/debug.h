// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Helper macros for OpenMSX profiler tool
//
// Ref: 
//  - https://hg.sr.ht/~grauw/neonlib/browse/tools/profile.tcl
//  - https://github.com/MartinezTorres/openMSX_profiler
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//-----------------------------------------------------------------------------
// No support for debug/profile tool
#if (DEBUG_TOOL == DEBUG_DISABLE)

	#define DEBUG_INIT()

	#define DEBUG_BREAK()
	#define DEBUG_ASSERT(a)
	#define DEBUG_LOG(msg)

	#define PROFILE_SECTION_START(id, level)
	#define PROFILE_SECTION_END(id, level)

	#define PROFILE_FRAME_START()
	#define PROFILE_FRAME_END()

#endif

//-----------------------------------------------------------------------------
// OpenMSX default debugger (no profiler)
#if ((DEBUG_TOOL == DEBUG_OPENMSX) || (DEBUG_TOOL == DEBUG_OPENMSX_G) || (DEBUG_TOOL == DEBUG_OPENMSX_S))

	#define DEBUG_INIT()

	//-----------------------------------------------------------------------------
	// Port 0x2E - Mode Set Register
	//-----------------------------------------------------------------------------
	//	7	6	5	4	3	2	1	0
	//		│	│	│	└───┴───┴───┴── Mode-specific parameters
	//		│	│	│						Single byte mode
	//		│	│	│							bit#3	ASCII mode on/off
	//		│	│	│							bit#2	decimal mode on/off
	//		│	│	│							bit#1	binary mode on/off
	//		│	│	│							bit#0	hexadecimal mode on/off
	//		│	│	│						Multi byte mode
	//		│	│	│							mode (0 = hex, 1 = binary, 2 = decimal, 3 = ASCII mode)
	//		│	└───┴────────────────── Output mode (0 = OFF, 1 = single byte, 2 = multi byte)
	//		└────────────────────────── Line feed mode (0 = line feed at mode change, 1 no line feed)
	__sfr __at(0x2E) g_PortDebugMode; // Mode Set Register. (Write only)
	__sfr __at(0x2F) g_PortDebugData; // Data Register. (Write only)

	inline void DEBUG_LOG(const c8* msg)
	{
		g_PortDebugMode = 0x23; // Multi byte ASCII mode
		while(*msg)
			g_PortDebugData = *msg++;
	}

	#define DEBUG_BREAK()
	#define DEBUG_ASSERT(a)			if(!(a)) DEBUG_BREAK()

#endif

//-----------------------------------------------------------------------------
// Grauw profile script for OpenMSX emulator
#if (DEBUG_TOOL == DEBUG_OPENMSX_G)

	#define P_PROFILE_SECTION		HASH(0x2C)
	#define P_PROFILE_FRAME			HASH(0x2D)

	#define PROFILE_SECTION_START(id, level) if (level < PROFILE_LEVEL) {	\
		__asm																\
			ld		a, HASH(id)												\
			in		a,(P_PROFILE_SECTION)									\
		__endasm; }
																			
	#define PROFILE_SECTION_END(id, level) if (level < PROFILE_LEVEL) {		\
		__asm																\
			ld		a, HASH(id)												\
			out		(P_PROFILE_SECTION), a									\
		__endasm; }
																			
	#define PROFILE_FRAME_START()											\
		__asm																\
			in		a, (P_PROFILE_FRAME)									\
		__endasm;
																			
	#define PROFILE_FRAME_END()												\
		__asm																\
			out		(P_PROFILE_FRAME), a									\
		__endasm;
#endif

//-----------------------------------------------------------------------------
// Salutte profile script for OpenMSX emulator
#if (DEBUG_TOOL == DEBUG_OPENMSX_S)

	// https://github.com/MartinezTorres/OpenMSX_profiler
	#define P_PROFILE_START			HASH(0x2C)
	#define P_PROFILE_END			HASH(0x2D)

	__sfr __at 0x2C g_PortStartProfile;
	__sfr __at 0x2D g_PortEndProfile;

	const c8* __at 0xF931 g_ProfileMsg;

	#define PROFILE_SECTION_START(id, level) if (level < PROFILE_LEVEL) { g_ProfileMsg = #id; g_PortStartProfile = 0; }
	#define PROFILE_SECTION_END(id, level)   if (level < PROFILE_LEVEL) { g_ProfileMsg = #id; g_PortEndProfile = 0; }

	#define PROFILE_FRAME_START()	{ g_ProfileMsg = "FRAME"; g_PortStartProfile = 0; }
	#define PROFILE_FRAME_END()		{ g_ProfileMsg = "FRAME"; g_PortEndProfile = 0; }
#endif

//-----------------------------------------------------------------------------
// Support for Emulicious debugger/profiler
#if (DEBUG_TOOL == DEBUG_EMULICIOUS)

	// Macro from gbdk-2020 (see https://github.com/gbdk-2020/gbdk-2020/blob/develop/gbdk-lib/include/gb/emu_debug.h)
	#define DEBUG_MACRONAME(a)		DEBUG_MERGE(DEBUGNAME, a)
	#define DEBUG_MERGE(a, b)		a##b

	__sfr __at(0x2E) g_PortDummy;
	inline void Debug_PutStringInHL(const c8* str) { g_PortDummy = *str; }

	#define EMU_MESSAGE_HL(macroName, msg)						\
	Debug_PutStringInHL(msg); 									\
	__asm														\
		.macro macroName ?labelEnd								\
			ld		d, d										\
			jr		labelEnd									\
			.dw		0x6464										\
			.dw		0x0102										\
		labelEnd:												\
		.endm													\
		macroName												\
	__endasm

	inline void Debug_Initialize()
	{
		// Fix assertion for un-initialized VDP buffer
		u8* ptr = (u8*)&g_VDP_Command;
		for(u8 i = 0; i < sizeof(g_VDP_Command); ++i)
			*(ptr++) = 0;
		
		// Fix assertion for BIOS-initialized key buffer
		#if (INPUT_KB_UPDATE)
		for(u8 i = INPUT_KB_UPDATE_MIN; i <= INPUT_KB_UPDATE_MAX; ++i)	
			((u8*)g_NEWKEY)[i] = 0xFF;
		#endif
	}
	#define DEBUG_INIT()			Debug_Initialize()

	#define DEBUG_BREAK()			__asm__("ld b, b")
	#define DEBUG_ASSERT(a)			if(!(a)) DEBUG_BREAK()
	#define DEBUG_LOG(msg)			EMU_MESSAGE_HL(DEBUG_MACRONAME(__LINE__), msg)

	#define PROFILE_SECTION_START(id, level) if (level < PROFILE_LEVEL) { EMU_MESSAGE_SUFFIX(#id, ":S%ZEROCLKS%"); }
	#define PROFILE_SECTION_END(id, level)   if (level < PROFILE_LEVEL) { EMU_MESSAGE_SUFFIX(#id, ":E=%-18+LASTCLKS%"); }	

	#define PROFILE_FRAME_START()	PROFILE_SECTION_START(FRAME, 0)
	#define PROFILE_FRAME_END()		PROFILE_SECTION_END(FRAME, 0)
#endif