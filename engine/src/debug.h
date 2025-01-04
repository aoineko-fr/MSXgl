// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Helper macros for openMSX profiler tool
//
// Ref: 
//  - https://hg.sr.ht/~grauw/neonlib/browse/tools/profile.tcl
//  - https://github.com/MartinezTorres/openMSX_profiler
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//-----------------------------------------------------------------------------
// Support for debug/profile tool
#if (DEBUG_TOOL != DEBUG_DISABLE)

//.............................................................................
// Group: Debug

	// Function: DEBUG_INIT
	// Initialize Debug module.
	//
	// Notes:
	// - [Emulicious] This allow to mark area reserved by BIOS to help detection of unset memory access
	void DEBUG_INIT();

	// Function: DEBUG_BREAK
	// Force a break point.
	//
	// Notes:
	// - [Emulicious] You need the "Break on ld b, b instruction" exception to be activated.
	// - [openMSX] You need the "tools/script/openMSX/debugger_pvm.tcl" script to be loaded to use this function.
	void DEBUG_BREAK();

	// Function: DEBUG_ASSERT
	// Conditionnal break.
	//
	// Parameters:
	//   a - Instructions to evialuate. A 'false' result will trigger a break.
	//
	// Notes:
	// - [Emulicious] You need the "Break on ld b, b instruction" exception to be activated.
	// - [openMSX] You need the "tools/script/openMSX/debugger_pvm.tcl" script to be loaded to use this function.
	void DEBUG_ASSERT(bool a);

	// Function: DEBUG_LOG
	// Display debug message (and return to next line).
	//
	// Parameters:
	//   msg - Null-terminated string with message to display
	void DEBUG_LOG(const c8* msg);

	// Function: DEBUG_LOGNUM
	// Display debug message and a 8-bits value (and return to next line).
	//
	// Parameters:
	//   msg - Null-terminated string with numer name
	//   num - 8-bit number value
	void DEBUG_LOGNUM(const c8* msg, u8 num);

	// Function: DEBUG_PRINT
	// Display debug formated message.
	// No new line is added at the end of the message.
	//
	// Parameters:
	//   format - Null-terminated string with format specifiers
	//
	// Notes:
	// - [Emulicious] Supported format specifiers: https://www.tutorialspoint.com/format-specifiers-in-c
	// - [openMSX] You need the "tools/script/openMSX/debugger_pvm.tcl" script to be loaded to use this function. 
	void DEBUG_PRINT(const c8* format, ...);

//-----------------------------------------------------------------------------
// No support for debug/profile tool
#else

	// Initialize Debug module
	#define DEBUG_INIT()

	// Force a break point
	#define DEBUG_BREAK()

	// Conditionnal break
	#define DEBUG_ASSERT(a)

	// Display debug message
	#define DEBUG_LOG(msg)

	// Display debug message and a 8-bits value
	#define DEBUG_LOGNUM(msg, num)

	// Display debug formated message
	#define DEBUG_PRINT(...)

#endif


//.............................................................................
// Group: Profile

//-----------------------------------------------------------------------------
// Grauw profile script for openMSX emulator
#if (PROFILE_TOOL == PROFILE_OPENMSX_G)

	#define P_PROFILE_SECTION		0x2C
	#define P_PROFILE_FRAME			0x2D

	__sfr __at(P_PROFILE_SECTION)	g_PortDebugSection;
	__sfr __at(P_PROFILE_FRAME)		g_PortDebugFrame;

	inline void do_PROFILE_SECTION_START(u8 id)
	{
		id; // A
		__asm
			in a, (P_PROFILE_SECTION)
		__endasm;	
	}																		
	inline void PROFILE_SECTION_START(u8 level, u8 section, const c8* msg) { msg; if (level <= PROFILE_LEVEL) { do_PROFILE_SECTION_START(section); } }

	inline void do_PROFILE_SECTION_END(u8 id)
	{
		id; // A
		__asm
			out (P_PROFILE_SECTION), a
		__endasm;	
	}
	inline void PROFILE_SECTION_END(u8 level, u8 section, const c8* msg) { msg; if (level <= PROFILE_LEVEL) { do_PROFILE_SECTION_END(section); } }
																			
	inline void PROFILE_FRAME_START()
	{
		__asm
			in a, (P_PROFILE_FRAME)
		__endasm;
	}
	inline void PROFILE_FRAME_END()
	{
		__asm
			out (P_PROFILE_FRAME), a
		__endasm;
	}

//-----------------------------------------------------------------------------
// Salutte profile script for openMSX emulator
// Doc: https://github.com/MartinezTorres/OpenMSX_profiler
#elif (PROFILE_TOOL == PROFILE_OPENMSX_S)

	#define P_PROFILE_START			0x2C
	#define P_PROFILE_END			0x2D

	__sfr __at(P_PROFILE_START)		g_PortStartProfile;
	__sfr __at(P_PROFILE_END)		g_PortEndProfile;

	const c8* __at(0xF931) g_ProfileMsg;

	inline void PROFILE_SECTION_START(u8 level, u8 section, const c8* msg)
	{
		if (level < PROFILE_LEVEL)
		{
			g_ProfileMsg = msg;
			g_PortStartProfile = section;
		}
	}

	inline void PROFILE_SECTION_END(u8 level, u8 section, const c8* msg)
	{
		if (level < PROFILE_LEVEL)
		{
			g_ProfileMsg = msg;
			g_PortEndProfile = section;
		}
	}

	inline void PROFILE_FRAME_START()	{ PROFILE_SECTION_START(0, 0, "FRAME"); }
	inline void PROFILE_FRAME_END()		{ PROFILE_SECTION_END(0, 0, "FRAME"); }

#else

	// Function: PROFILE_FRAME_START
	// Signal the start of a frame to be measured.
	inline void PROFILE_FRAME_START() {}

	// Function: PROFILE_FRAME_END
	// Signal the end of a frame to be measured.
	inline void PROFILE_FRAME_END() {}

	// Function: PROFILE_SECTION_START
	// Signal the start of a section to be measured.
	//
	// Parameters:
	//   level - Minimal level to display the section
	//   section - Section identifier
	//   msg - Null-terminated string with section name
	inline void PROFILE_SECTION_START(u8 level, u8 section, const c8* msg) { level; section; msg; }

	// Function: PROFILE_SECTION_END
	// Signal the end of a section to be measured.
	//
	// Parameters:
	//   level - Minimal level to display the section
	//   section - Section identifier
	//   msg - Null-terminated string with section name
	inline void PROFILE_SECTION_END(u8 level, u8 section, const c8* msg) { level; section; msg; }

#endif