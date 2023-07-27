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
// No support for debug/profile tool
#if (DEBUG_TOOL == DEBUG_DISABLE)

	// Function: DEBUG_INIT
	// Initialize Debug module
	inline void DEBUG_INIT() {}

	// Function: DEBUG_BREAK
	// Force a break point
	inline void DEBUG_BREAK() {}

	// Function: DEBUG_ASSERT
	// Conditionnal break
	inline void DEBUG_ASSERT(bool a) { a; }

	// Function: DEBUG_LOG
	// Display debug message
	inline void DEBUG_LOG(const c8* msg) { msg; }

	// Function: DEBUG_LOGNUM
	// Display debug message and a 8-bits value
	inline void DEBUG_LOGNUM(const c8* msg, u8 num) { msg; num; }

	// Function: PROFILE_FRAME_START
	// Signal the start of a frame to be measured.
	inline void PROFILE_FRAME_START() {}

	// Function: PROFILE_FRAME_END
	// Signal the end of a section to be measured.
	inline void PROFILE_FRAME_END() {}

	// Function: PROFILE_SECTION_START
	// Signal the start of a section to be measured.
	inline void PROFILE_SECTION_START(u8 id, u8 level) { id; level; }

	// Function: PROFILE_SECTION_END
	// Signal the end of a section to be measured.
	inline void PROFILE_SECTION_END(u8 id, u8 level) { id; level; }

#endif

//-----------------------------------------------------------------------------
// openMSX default debugger (no profiler)
// https://openmsx.org/manual/user.html#debugdevice
#if (DEBUG_EMULATOR(DEBUG_TOOL) == DEBUG_OPENMSX)

	inline void DEBUG_INIT() {}

	#define P_DEBUG_MODE		0x2E
	#define P_DEBUG_DATA		0x2F
	__sfr __at(P_DEBUG_MODE) g_PortDebugMode; // Mode Set Register. (Write only)
	__sfr __at(P_DEBUG_DATA) g_PortDebugData; // Data Register. (Write only)

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
	#define DEBUG_NOLINEFEED	0b01000000
	#define DEBUG_MODE			0b00110000
	#define DEBUG_MODE_OFF		0b00000000
	#define DEBUG_MODE_SINGLE	0b00010000
	#define DEBUG_MODE_MULTI	0b00100000
	#define DEBUG_SINGLE_HEX	0b00000001
	#define DEBUG_SINGLE_BIN	0b00000010
	#define DEBUG_SINGLE_DEC	0b00000100
	#define DEBUG_SINGLE_ASCII	0b00001000
	#define DEBUG_MULTI_HEX		0b00000000
	#define DEBUG_MULTI_BIN		0b00000001
	#define DEBUG_MULTI_DEC		0b00000010
	#define DEBUG_MULTI_ASCII	0b00000011

	inline void DEBUG_LOG(const c8* msg)
	{
		g_PortDebugMode = DEBUG_MODE_MULTI|DEBUG_MULTI_ASCII; // Multi byte ASCII mode
		while(*msg)
			g_PortDebugData = *msg++;
	}

	inline void DEBUG_LOGNUM(const c8* msg, u8 num)
	{
		DEBUG_LOG(msg);
		g_PortDebugData = ':';
		g_PortDebugMode = DEBUG_NOLINEFEED|DEBUG_MODE_MULTI|DEBUG_MULTI_DEC; // Multi byte ASCII mode
		g_PortDebugData = num;
	}

	//-----------------------------------------------------------------------------
	// Grauw profile script for openMSX emulator
	#if (DEBUG_TOOL == DEBUG_OPENMSX_G)

		#define P_PROFILE_SECTION		0x2C
		#define P_PROFILE_FRAME			0x2D
		__sfr __at(P_PROFILE_SECTION) g_PortDebugSection;
		__sfr __at(P_PROFILE_FRAME) g_PortDebugFrame;

		inline void do_PROFILE_SECTION_START(u8 id)
		{
			id; // A
			__asm
				in a, (P_PROFILE_SECTION)
			__endasm;	
		}																		
		inline void PROFILE_SECTION_START(u8 id, u8 level) { if (level <= PROFILE_LEVEL) { do_PROFILE_SECTION_START(id); } }

		inline void do_PROFILE_SECTION_END(u8 id)
		{
			id; // A
			__asm
				out (P_PROFILE_SECTION), a
			__endasm;	
		}
		inline void PROFILE_SECTION_END(u8 id, u8 level) { if (level <= PROFILE_LEVEL) { do_PROFILE_SECTION_END(id); } }
																				
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

		inline void DEBUG_BREAK() {}

	//-----------------------------------------------------------------------------
	// Salutte profile script for openMSX emulator
	#elif (DEBUG_TOOL == DEBUG_OPENMSX_S)

		// https://github.com/MartinezTorres/OpenMSX_profiler
		#define P_PROFILE_START			0x2C
		#define P_PROFILE_END			0x2D

		__sfr __at(P_PROFILE_START) g_PortStartProfile;
		__sfr __at(P_PROFILE_END) g_PortEndProfile;

		const c8* __at(0xF931) g_ProfileMsg;

		inline void do_PROFILE_SECTION_START(const c8* id, u8 level) { if (level <= PROFILE_LEVEL) { g_ProfileMsg = id; g_PortStartProfile = 0; } }
		#define PROFILE_SECTION_START(id, level)	do_PROFILE_SECTION_START(#id, level)
		inline void do_PROFILE_SECTION_END(const c8* id, u8 level)   { if (level <= PROFILE_LEVEL) { g_ProfileMsg = id; g_PortEndProfile = 0; } }
		#define PROFILE_SECTION_END(id, level)		do_PROFILE_SECTION_END(#id, level)

		inline void PROFILE_FRAME_START()	{ g_ProfileMsg = "FRAME"; g_PortStartProfile = 0; }
		inline void PROFILE_FRAME_END()		{ g_ProfileMsg = "FRAME"; g_PortEndProfile = 0; }

		inline void DEBUG_BREAK() {}

	//-----------------------------------------------------------------------------
	// 
	#else

		inline void PROFILE_FRAME_START() {}
		inline void PROFILE_FRAME_END() {}

		inline void PROFILE_SECTION_START(u8 id, u8 level) { id; level; }
		inline void PROFILE_SECTION_END(u8 id, u8 level) { id; level; }

		// PVM debug script for openMSX emulator
		#if (DEBUG_TOOL == DEBUG_OPENMSX_P)
			inline void DEBUG_BREAK() { g_PortDebugMode = 0xFF; }
		#else
			inline void DEBUG_BREAK() {}
		#endif

	#endif

	inline void DEBUG_ASSERT(bool a) { if(!(a)) DEBUG_BREAK(); }

#endif

//-----------------------------------------------------------------------------
// Support for Emulicious debugger/profiler
#if (DEBUG_TOOL == DEBUG_EMULICIOUS)

	// Macro from gbdk-2020 (see https://github.com/gbdk-2020/gbdk-2020/blob/develop/gbdk-lib/include/gb/emu_debug.h)
	#define DEBUG_MERGE(a, b)		a##b
	#define DEBUG_MACRONAME(a)		DEBUG_MERGE(DEBUGNAME, a)
	#define EMU_MESSAGE_SUFFIX(message_text, message_suffix) EMU_MESSAGE3(EMU_MACRONAME(__LINE__), message_text, message_suffix)
	#define EMU_MESSAGE3(name, message_text, message_suffix) \
	__asm \
		.macro name msg_t, msg_s, ?llbl\
			ld d, d \
			jr llbl \
			.dw 0x6464 \
			.dw 0x0000 \
			.ascii msg_t \
			.ascii msg_s \
		llbl: \
		.endm \
		name ^/message_text/, ^/message_suffix/ \
	__endasm

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
	#define DEBUG_LOGNUM(msg, num)

	#define PROFILE_SECTION_START(id, level) //if (level <= PROFILE_LEVEL) { EMU_MESSAGE_SUFFIX(#id, ":S%ZEROCLKS%"); }
	#define PROFILE_SECTION_END(id, level)   //if (level <= PROFILE_LEVEL) { EMU_MESSAGE_SUFFIX(#id, ":E=%-18+LASTCLKS%"); }	

	#define PROFILE_FRAME_START()	PROFILE_SECTION_START(FRAME, 0)
	#define PROFILE_FRAME_END()		PROFILE_SECTION_END(FRAME, 0)
#endif