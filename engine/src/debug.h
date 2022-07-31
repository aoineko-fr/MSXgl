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
#pragma sdcc_hash +

#include "core.h"

//-----------------------------------------------------------------------------
// Grauw profile script for OpenMSX emulator
#if (DEBUG_TOOL == DEBUG_OPENMSX_G)

	#define DEBUG_BREAK()
	#define DEBUG_ASSERT(a)			if(!(a)) DEBUG_BREAK()
	#define DEBUG_LOG(msg)

	#define P_PROFILE_SECTION		#0x2C
	#define P_PROFILE_FRAME			#0x2D

	#define PROFILE_SECTION_START(id, level) if (level < PROFILE_LEVEL) {	\
		__asm																\
			ld		a, #(id)												\
			in		a,(P_PROFILE_SECTION)									\
		__endasm; }
																			
	#define PROFILE_SECTION_END(id, level) if (level < PROFILE_LEVEL) {		\
		__asm																\
			ld		a, #(id)												\
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

//-----------------------------------------------------------------------------
// Salutte profile script for OpenMSX emulator
#elif (DEBUG_TOOL == DEBUG_OPENMSX_S)

	// https://github.com/MartinezTorres/OpenMSX_profiler
	#define DEBUG_BREAK()
	#define DEBUG_ASSERT(a)			if(!(a)) DEBUG_BREAK()
	#define DEBUG_LOG(msg)

	#define P_PROFILE_START			#0x2C
	#define P_PROFILE_END			#0x2D

	__sfr __at 0x2C g_PortStartProfile;
	__sfr __at 0x2D g_PortEndProfile;

	const c8* __at 0xF931 g_ProfileMsg;

	#define PROFILE_SECTION_START(id, level) if (level < PROFILE_LEVEL) { g_ProfileMsg = #id; g_PortStartProfile = 0; }
	#define PROFILE_SECTION_END(id, level)   if (level < PROFILE_LEVEL) { g_ProfileMsg = #id; g_PortEndProfile = 0; }

	#define PROFILE_FRAME_START()	{ g_ProfileMsg = "FRAME"; g_PortStartProfile = 0; }
	#define PROFILE_FRAME_END()		{ g_ProfileMsg = "FRAME"; g_PortEndProfile = 0; }

//-----------------------------------------------------------------------------
// Profile script for Emulicious emulaor
#elif (DEBUG_TOOL == DEBUG_EMULICIOUS)

	// Macro from gbdk-2020 (see https://github.com/gbdk-2020/gbdk-2020/blob/develop/gbdk-lib/include/gb/emu_debug.h)
	#define EMU_MACRONAME(A)		EMU_MACRONAME1(A)
	#define EMU_MACRONAME1(A)		EMULOG##A

	#define EMU_MESSAGE(message_text) EMU_MESSAGE1(EMU_MACRONAME(__LINE__), message_text)
	#define EMU_MESSAGE1(name, message_text)					\
	__asm														\
		.macro name msg_t, ?llbl								\
			ld		d, d										\
			jr		llbl										\
			.dw		0x6464										\
			.dw		0x0000										\
			.ascii	msg_t										\
		llbl:													\
		.endm													\
		name ^/message_text/									\
	__endasm

	#define EMU_MESSAGE_SUFFIX(message_text, message_suffix) EMU_MESSAGE3(EMU_MACRONAME(__LINE__), message_text, message_suffix)
	#define EMU_MESSAGE3(name, message_text, message_suffix)	\
	__asm														\
		.macro name msg_t, msg_s, ?llbl							\
			ld		d, d										\
			jr		llbl										\
			.dw		0x6464										\
			.dw		0x0000										\
			.ascii	msg_t										\
			.ascii	msg_s										\
		llbl:													\
		.endm													\
		name ^/message_text/, ^/message_suffix/					\
	__endasm

	#ifndef DEBUG_MSG_ADDR
	#define DEBUG_MSG_ADDR			0xF931
	#endif

	#define EMU_MESSAGE_ADDR(msg) EMU_MESSAGE4(EMU_MACRONAME(__LINE__), msg)
	#define EMU_MESSAGE4(macroName, msg)						\
	String_Copy((c8*)DEBUG_MSG_ADDR, msg);						\
	__asm														\
		.macro macroName ?llbl									\
			ld		d, d										\
			jr		llbl										\
			.dw		0x6464										\
			.dw		0x0001										\
			.dw		DEBUG_MSG_ADDR								\
			.dw		0x0000										\
		llbl:													\
		.endm													\
		macroName												\
	__endasm

	#define DEBUG_BREAK()			__asm__("ld b, b")
	#define DEBUG_ASSERT(a)			if(!(a)) DEBUG_BREAK()
	#define DEBUG_LOG(msg)			EMU_MESSAGE_ADDR(msg)

	#define PROFILE_SECTION_START(id, level) if (level < PROFILE_LEVEL) { EMU_MESSAGE_SUFFIX(#id, ":S%ZEROCLKS%"); }
	#define PROFILE_SECTION_END(id, level)   if (level < PROFILE_LEVEL) { EMU_MESSAGE_SUFFIX(#id, ":E=%-18+LASTCLKS%"); }	

	#define PROFILE_FRAME_START()	PROFILE_SECTION_START(FRAME, 0)
	#define PROFILE_FRAME_END()		PROFILE_SECTION_END(FRAME, 0)

//-----------------------------------------------------------------------------
#else // if (DEBUG_TOOL == DEBUG_DISABLE)

	#define DEBUG_BREAK()
	#define DEBUG_ASSERT(a)
	#define DEBUG_LOG(msg)

	#define PROFILE_SECTION_START(id, level)
	#define PROFILE_SECTION_END(id, level)

	#define PROFILE_FRAME_START()
	#define PROFILE_FRAME_END()

#endif