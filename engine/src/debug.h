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

	// Function: DEBUG_INIT
	// Initialize Debug module
	void DEBUG_INIT();

	// Function: DEBUG_BREAK
	// Force a break point
	//
	// Notes:
	// - [Emulicious] You need the "Break on ld b, b instruction" exception to be activated.
	// - [openMSX] You need the "tools/script/openMSX/debugger_pvm.tcl" script to be loaded to use this function.
	void DEBUG_BREAK();

	// Function: DEBUG_ASSERT
	// Conditionnal break
	//
	// Parameters:
	//   a - Instructions to evialuate. A 'false' result will trigger a break.
	//
	// Notes:
	// - [Emulicious] You need the "Break on ld b, b instruction" exception to be activated.
	// - [openMSX] You need the "tools/script/openMSX/debugger_pvm.tcl" script to be loaded to use this function.
	void DEBUG_ASSERT(bool a);

	// Function: DEBUG_LOG
	// Display debug message
	//
	// Parameters:
	//   msg - Null-terminated string with message to display
	void DEBUG_LOG(const c8* msg);

	// Function: DEBUG_LOGNUM
	// Display debug message and a 8-bits value
	//
	// Parameters:
	//   msg - Null-terminated string with numer name
	//   num - 8-bit number value
	void DEBUG_LOGNUM(const c8* msg, u8 num);

	// Function: DEBUG_PRINT
	// Display debug formated message
	//
	// Parameters:
	//   format - Null-terminated string with format specifiers
	//
	// Notes:
	// - [Emulicious] Supported format specifiers: https://www.tutorialspoint.com/format-specifiers-in-c
	// - [openMSX] You need the "tools/script/openMSX/debugger_pvm.tcl" script to be loaded to use this function. Supported format specifiers: 
	void DEBUG_PRINT(const c8* format, ...);

	// Function: PROFILE_FRAME_START
	// Signal the start of a frame to be measured.
	void PROFILE_FRAME_START();

	// Function: PROFILE_FRAME_END
	// Signal the end of a frame to be measured.
	void PROFILE_FRAME_END();

	// Function: PROFILE_SECTION_START
	// Signal the start of a section to be measured.
	void PROFILE_SECTION_START(u8 id, u8 level);

	// Function: PROFILE_SECTION_END
	// Signal the end of a section to be measured.
	void PROFILE_SECTION_END(u8 id, u8 level);

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

	// Signal the start of a frame to be measured.
	#define PROFILE_FRAME_START()

	// Signal the end of a frame to be measured.
	#define PROFILE_FRAME_END()

	// Signal the start of a section to be measured.
	#define PROFILE_SECTION_START(id, level)

	// Signal the end of a section to be measured.
	#define PROFILE_SECTION_END(id, level)

#endif
