// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄             ▄▄  ▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ▄█▀▄ ██▀▄ ██ ▀ ▄  ▄▀██ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ▀█▄▀ ██ █ ██▀  ██  ▀██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                          ▀▀  
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// Default defines configuration
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//-----------------------------------------------------------------------------
//
// D E F A U L T
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// BUILD
//-----------------------------------------------------------------------------

// TARGET
#if !defined(TARGET)
	#warning TARGET is not defined in "msxgl_config.h"! Default value will be used: TARGET_ROM32
	#define TARGET						TARGET_ROM32
#endif

// TARGET_TYPE
#if !defined(TARGET_TYPE)
	#warning TARGET_TYPE is not defined in "msxgl_config.h"! Default value will be used: TARGET_ROM32
	#define TARGET_TYPE					TARGET_TYPE_ROM
#endif

// MSX_VERSION
#if !defined(MSX_VERSION)
	#warning MSX_VERSION is not defined in "msxgl_config.h"! Default value will be used: MSX_2
	#define MSX_VERSION					MSX_2
#endif

//-----------------------------------------------------------------------------
// BIOS MODULE
//-----------------------------------------------------------------------------

// BIOS_CALL
#ifndef BIOS_CALL_MAINROM
	#warning BIOS_CALL_MAINROM is not defined in "msxgl_config.h"! Default value will be used: BIOS_CALL_DIRECT
	#define BIOS_CALL_MAINROM			BIOS_CALL_DIRECT
#endif
#ifndef BIOS_CALL_SUBROM
	#warning BIOS_CALL_SUBROM is not defined in "msxgl_config.h"! Default value will be used: BIOS_CALL_INTERSLOT
	#define BIOS_CALL_SUBROM			BIOS_CALL_INTERSLOT
#endif
#ifndef BIOS_CALL_DISKROM
	#warning BIOS_CALL_DISKROM is not defined in "msxgl_config.h"! Default value will be used: BIOS_CALL_INTERSLOT
	#define BIOS_CALL_DISKROM			BIOS_CALL_INTERSLOT
#endif

//-----------------------------------------------------------------------------
// VDP MODULE
//-----------------------------------------------------------------------------

// VDP_VRAM_ADDR
#ifndef VDP_VRAM_ADDR
	#if (MSX_VERSION == MSX_1)
		#warning VDP_VRAM_ADDR is not defined in "msxgl_config.h"! Default value will be used: VDP_VRAM_ADDR_14
		#define VDP_VRAM_ADDR			VDP_VRAM_ADDR_14
	#else
		#warning VDP_VRAM_ADDR is not defined in "msxgl_config.h"! Default value will be used: VDP_VRAM_ADDR_17
		#define VDP_VRAM_ADDR			VDP_VRAM_ADDR_17
	#endif	
#endif

// VDP_UNIT
#ifndef VDP_UNIT
	#warning VDP_UNIT is not defined in "msxgl_config.h"! Default value will be used: VDP_UNIT_U8
	#define VDP_UNIT					VDP_UNIT_U8
#endif


//-----------------------------------------------------------------------------
// DRAW MODULE
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PRINT MODULE
//-----------------------------------------------------------------------------

// PRINT_USE_VALIDATOR
#ifndef PRINT_USE_VALIDATOR
	#warning PRINT_USE_VALIDATOR is not defined in "msxgl_config.h"! Default value will be used: 0
	#define PRINT_USE_VALIDATOR			0
#endif
// PRINT_USE_VRAM
#ifndef PRINT_USE_VRAM
	#warning PRINT_USE_VRAM is not defined in "msxgl_config.h"! Default value will be used: 0
	#define PRINT_USE_VRAM				0
#endif
// PRINT_USE_SPRITE
#ifndef PRINT_USE_SPRITE
	#warning PRINT_USE_SPRITE is not defined in "msxgl_config.h"! Default value will be used: 0
	#define PRINT_USE_SPRITE			0
#endif
// PRINT_USE_FX_SHADOW
#ifndef PRINT_USE_FX_SHADOW
	#warning PRINT_USE_FX_SHADOW is not defined in "msxgl_config.h"! Default value will be used: 0
	#define PRINT_USE_FX_SHADOW			0
#endif
// PRINT_USE_FX_OUTLINE
#ifndef PRINT_USE_FX_OUTLINE
	#warning PRINT_USE_FX_OUTLINE is not defined in "msxgl_config.h"! Default value will be used: 0
	#define PRINT_USE_FX_OUTLINE		0
#endif
// PRINT_USE_UNIT
#ifndef PRINT_USE_UNIT
	#warning PRINT_USE_UNIT is not defined in "msxgl_config.h"! Default value will be used: 0
	#define PRINT_USE_UNIT				0
#endif
// PRINT_WIDTH
#ifndef PRINT_WIDTH
	#warning PRINT_WIDTH is not defined in "msxgl_config.h"! Default value will be used: PRINT_WIDTH_8
	#define PRINT_WIDTH					PRINT_WIDTH_8
#endif
// PRINT_HEIGHT
#ifndef PRINT_HEIGHT
	#warning PRINT_HEIGHT is not defined in "msxgl_config.h"! Default value will be used: PRINT_HEIGHT_8
	#define PRINT_HEIGHT				PRINT_HEIGHT_8
#endif
// PRINT_COLOR
#ifndef PRINT_COLOR_NUM
	#warning PRINT_COLOR_NUM is not defined in "msxgl_config.h"! Default value will be used: 1
	#define PRINT_COLOR_NUM				1
#endif
// PRINT_SKIP_SPACE
#ifndef PRINT_SKIP_SPACE
	#warning PRINT_SKIP_SPACE is not defined in "msxgl_config.h"! Default value will be used: 0
	#define PRINT_SKIP_SPACE			0
#endif
// PRINT_USE_GRAPH
#ifndef PRINT_USE_GRAPH
	#warning PRINT_USE_GRAPH is not defined in "msxgl_config.h"! Default value will be used: 1
	#define PRINT_USE_GRAPH				1
#endif
// PRINT_USE_FORMAT
#ifndef PRINT_USE_FORMAT
	#warning PRINT_USE_FORMAT is not defined in "msxgl_config.h"! Default value will be used: 1
	#define PRINT_USE_FORMAT			1
#endif
// PRINT_USE_32B
#ifndef PRINT_USE_32B
	#warning PRINT_USE_32B is not defined in "msxgl_config.h"! Default value will be used: 1
	#define PRINT_USE_32B				0
#endif


//-----------------------------------------------------------------------------
// GAME MODULE
//-----------------------------------------------------------------------------

// GAME_USE_STATE
#ifndef GAME_USE_STATE
	#warning GAME_USE_STATE is not defined in "msxgl_config.h"! Default value will be used: 1
	#define GAME_USE_STATE				1
#endif

// GAME_USE_VSYNC
#ifndef GAME_USE_VSYNC
	#warning GAME_USE_VSYNC is not defined in "msxgl_config.h"! Default value will be used: 1
	#define GAME_USE_VSYNC				1
#endif

// GAME_USE_LOOP
#ifndef GAME_USE_LOOP
	#warning GAME_USE_LOOP is not defined in "msxgl_config.h"! Default value will be used: 1
	#define GAME_USE_LOOP				1
#endif

//-----------------------------------------------------------------------------
// MATH MODULE
//-----------------------------------------------------------------------------

// RANDOM_8_METHOD
#ifndef RANDOM_8_METHOD
	#warning RANDOM_8_METHOD is not defined in "msxgl_config.h"! Default value will be used: RANDOM_8_ION
	#define RANDOM_8_METHOD				RANDOM_8_ION
#endif

// RANDOM_16_METHOD
#ifndef RANDOM_16_METHOD
	#warning RANDOM_16_METHOD is not defined in "msxgl_config.h"! Default value will be used: RANDOM_16_XORSHIFT
	#define RANDOM_16_METHOD			RANDOM_16_XORSHIFT
#endif

//-----------------------------------------------------------------------------
// PCM ENCDER
//-----------------------------------------------------------------------------

// PCMENC_FREQ
#ifndef PCMENC_FREQ
	#warning PCMENC_FREQ is not defined in "msxgl_config.h"! Default value will be used: PCMENC_11K
	#define PCMENC_FREQ					PCMENC_11K
#endif

//-----------------------------------------------------------------------------
// MISC
//-----------------------------------------------------------------------------

// PROFILE_MODE
#if !defined(PROFILE_MODE)
	#warning PROFILE_MODE is not defined in "msxgl_config.h"! Default value will be used: PROFILE_DISABLE
	#define PROFILE_MODE				PROFILE_DISABLE
#endif

// PROFILE_LEVEL
#if !defined(PROFILE_LEVEL)
	#warning PROFILE_LEVEL is not defined in "msxgl_config.h"! Default value will be used: 10
	#define PROFILE_LEVEL				10
#endif

// RANDOM
#if !defined(RANDOM_METHOD)
#define RANDOM_METHOD RANDOM_XORSHIFT
#endif
