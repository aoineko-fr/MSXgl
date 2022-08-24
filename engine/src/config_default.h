// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄             ▄▄  ▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ▄█▀▄ ██▀▄ ██ ▀ ▄  ▄▀██ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ▀█▄▀ ██ █ ██▀  ██  ▀██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                          ▀▀  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
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
	#define TARGET_TYPE					TYPE_ROM
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

// VDP_USE_VRAM16K
#ifndef VDP_USE_VRAM16K
	#warning VDP_USE_VRAM16K is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define VDP_USE_VRAM16K				TRUE
#endif

// VDP_USE_SPRITE
#ifndef VDP_USE_SPRITE
	#warning VDP_USE_SPRITE is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define VDP_USE_SPRITE				TRUE
#endif

// VDP_USE_COMMAND
#ifndef VDP_USE_COMMAND
	#warning VDP_USE_COMMAND is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define VDP_USE_COMMAND				TRUE
#endif

// VDP_USE_CUSTOM_CMD
#ifndef VDP_USE_CUSTOM_CMD
	#warning VDP_USE_CUSTOM_CMD is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define VDP_USE_CUSTOM_CMD			FALSE
#endif

// VDP_AUTO_INIT
#ifndef VDP_AUTO_INIT
	#warning VDP_AUTO_INIT is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define VDP_AUTO_INIT				TRUE
#endif

// VDP_INIT_50HZ
#ifndef VDP_INIT_50HZ
	#warning VDP_INIT_50HZ is not defined in "msxgl_config.h"! Default value will be used: VDP_INIT_ON
	#define VDP_INIT_50HZ				VDP_INIT_ON
#endif

// VDP_USE_UNDOCUMENTED
#ifndef VDP_USE_UNDOCUMENTED
	#warning VDP_USE_UNDOCUMENTED is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define VDP_USE_UNDOCUMENTED		TRUE
#endif

// VDP_USE_VALIDATOR
#ifndef VDP_USE_VALIDATOR
	#warning VDP_USE_VALIDATOR is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define VDP_USE_VALIDATOR			TRUE
#endif

// VDP_USE_DEFAULT_PALETTE
#ifndef VDP_USE_DEFAULT_PALETTE
	#warning VDP_USE_DEFAULT_PALETTE is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define VDP_USE_DEFAULT_PALETTE		FALSE
#endif

// VDP_USE_MSX1_PALETTE
#ifndef VDP_USE_MSX1_PALETTE
	#warning VDP_USE_MSX1_PALETTE is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define VDP_USE_MSX1_PALETTE		FALSE
#endif

// VDP_USE_DEFAULT_SETTINGS
#ifndef VDP_USE_DEFAULT_SETTINGS
	#warning VDP_USE_DEFAULT_SETTINGS is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define VDP_USE_DEFAULT_SETTINGS	TRUE
#endif

// VDP_USE_16X16_SPRITE
#ifndef VDP_USE_16X16_SPRITE
	#warning VDP_USE_16X16_SPRITE is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define VDP_USE_16X16_SPRITE		TRUE
#endif

// VDP_USE_RESTORE_S0
#ifndef VDP_USE_RESTORE_S0 // Do restore of status register pointer to S#0 (needed onlt for default BIOS ISR)
	#warning VDP_USE_RESTORE_S0 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define VDP_USE_RESTORE_S0			TRUE
#endif

// VDP_ISR_SAFE_MODE
#ifndef VDP_ISR_SAFE_MODE // ISR protection while modifying VDP registers
	#warning VDP_ISR_SAFE_MODE is not defined in "msxgl_config.h"! Default value will be used: VDP_ISR_SAFE_DEFAULT
	#define VDP_ISR_SAFE_MODE			VDP_ISR_SAFE_DEFAULT
#endif

//-----------------------------------------------------------------------------
// INPUR MODULE
//-----------------------------------------------------------------------------

#ifndef INPUT_USE_JOYSTICK // Add functions to handle joystick using I/O port
	#warning INPUT_USE_JOYSTICK is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define INPUT_USE_JOYSTICK			TRUE
#endif

#ifndef INPUT_USE_KEYBOARD // Add functions to handle keyboard using I/O port
	#warning INPUT_USE_KEYBOARD is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define INPUT_USE_KEYBOARD			TRUE
#endif

#ifndef INPUT_USE_MOUSE // Add support for Mouse handling functions
	#warning INPUT_USE_MOUSE is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define INPUT_USE_MOUSE				FALSE
#endif

#ifndef INPUT_USE_DETECT // Add feature to detect device plugged in General purpose ports
	#warning INPUT_USE_DETECT is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define INPUT_USE_DETECT			FALSE
#endif

#ifndef INPUT_USE_ISR_PROTECTION // Disable interruptions while access PSG registers (needed if you use BIOS or access PSG in your own ISR)
	#warning INPUT_USE_ISR_PROTECTION is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define INPUT_USE_ISR_PROTECTION	TRUE
#endif

#ifndef INPUT_USE_MANAGER // Add input manager (IPM) with advanced input features
	#warning INPUT_USE_MANAGER is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define INPUT_USE_MANAGER			FALSE
#endif

#ifndef INPUT_JOY_UPDATE // Add function to update all joystick states at once
	#warning INPUT_JOY_UPDATE is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define INPUT_JOY_UPDATE			FALSE
#endif

#ifndef INPUT_KB_UPDATE // Add function to update all keyboard rows at once
	#warning INPUT_KB_UPDATE is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define INPUT_KB_UPDATE				FALSE
#endif

#ifndef INPUT_KB_UPDATE // First row to update
	#define INPUT_KB_UPDATE_MIN			0
#endif

#ifndef INPUT_KB_UPDATE // Last row to update (10 for numerical-pad, 8 otherwise)
	#define INPUT_KB_UPDATE_MAX			8
#endif

//-----------------------------------------------------------------------------
// MEMORY MODULE
//-----------------------------------------------------------------------------

// MEM_USE_VALIDATOR
#ifndef MEM_USE_VALIDATOR
	#warning MEM_USE_VALIDATOR is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define MEM_USE_VALIDATOR		FALSE
#endif

// MEM_USE_FASTCOPY
#ifndef MEM_USE_FASTCOPY
	#warning MEM_USE_FASTCOPY is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define MEM_USE_FASTCOPY		FALSE
#endif

// MEM_USE_FASTSET
#ifndef MEM_USE_FASTSET
	#warning MEM_USE_FASTSET is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define MEM_USE_FASTSET			FALSE
#endif

//-----------------------------------------------------------------------------
// DRAW MODULE
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PRINT MODULE
//-----------------------------------------------------------------------------

// PRINT_USE_VALIDATOR
#ifndef PRINT_USE_VALIDATOR
	#warning PRINT_USE_VALIDATOR is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define PRINT_USE_VALIDATOR			FALSE
#endif
// PRINT_USE_VRAM
#ifndef PRINT_USE_VRAM
	#warning PRINT_USE_VRAM is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define PRINT_USE_VRAM				FALSE
#endif
// PRINT_USE_SPRITE
#ifndef PRINT_USE_SPRITE
	#warning PRINT_USE_SPRITE is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define PRINT_USE_SPRITE			FALSE
#endif
// PRINT_USE_FX_SHADOW
#ifndef PRINT_USE_FX_SHADOW
	#warning PRINT_USE_FX_SHADOW is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define PRINT_USE_FX_SHADOW			FALSE
#endif
// PRINT_USE_FX_OUTLINE
#ifndef PRINT_USE_FX_OUTLINE
	#warning PRINT_USE_FX_OUTLINE is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define PRINT_USE_FX_OUTLINE		FALSE
#endif
// PRINT_USE_UNIT
#ifndef PRINT_USE_UNIT
	#warning PRINT_USE_UNIT is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define PRINT_USE_UNIT				FALSE
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
	#warning PRINT_SKIP_SPACE is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define PRINT_SKIP_SPACE			FALSE
#endif
// PRINT_USE_GRAPH
#ifndef PRINT_USE_GRAPH
	#warning PRINT_USE_GRAPH is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define PRINT_USE_GRAPH				TRUE
#endif
// PRINT_USE_FORMAT
#ifndef PRINT_USE_FORMAT
	#warning PRINT_USE_FORMAT is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define PRINT_USE_FORMAT			TRUE
#endif
// PRINT_USE_32B
#ifndef PRINT_USE_32B
	#warning PRINT_USE_32B is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define PRINT_USE_32B				FALSE
#endif

//-----------------------------------------------------------------------------
// GAME MODULE
//-----------------------------------------------------------------------------

// GAME_USE_STATE
#ifndef GAME_USE_STATE
	#warning GAME_USE_STATE is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define GAME_USE_STATE				TRUE
#endif

// GAME_USE_VSYNC
#ifndef GAME_USE_VSYNC
	#warning GAME_USE_VSYNC is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define GAME_USE_VSYNC				TRUE
#endif

// GAME_USE_LOOP
#ifndef GAME_USE_LOOP
	#warning GAME_USE_LOOP is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define GAME_USE_LOOP				TRUE
#endif

//-----------------------------------------------------------------------------
// GAME PAWN MODULE
//-----------------------------------------------------------------------------

// GAMEPAWN_ID_PER_LAYER
#ifndef GAMEPAWN_ID_PER_LAYER
	#warning GAMEPAWN_ID_PER_LAYER is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define GAMEPAWN_ID_PER_LAYER		FALSE
#endif

// GAMEPAWN_USE_PHYSICS
#ifndef GAMEPAWN_USE_PHYSICS
	#warning GAMEPAWN_USE_PHYSICS is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define GAMEPAWN_USE_PHYSICS		FALSE
#endif

// GAMEPAWN_BORDER_EVENT
#ifndef GAMEPAWN_BORDER_EVENT
	#warning GAMEPAWN_BORDER_EVENT is not defined in "msxgl_config.h"! Default value will be used: 0
	#define GAMEPAWN_BORDER_EVENT		0
#endif

// GAMEPAWN_BORDER_BLOCK
#ifndef GAMEPAWN_BORDER_BLOCK
	#warning GAMEPAWN_BORDER_BLOCK is not defined in "msxgl_config.h"! Default value will be used: 0
	#define GAMEPAWN_BORDER_BLOCK		0
#endif

// GAMEPAWN_BORDER_MIN_Y
#ifndef GAMEPAWN_BORDER_MIN_Y
	#warning GAMEPAWN_BORDER_MIN_Y is not defined in "msxgl_config.h"! Default value will be used: 0
	#define GAMEPAWN_BORDER_MIN_Y		0
#endif

// GAMEPAWN_BORDER_MAX_Y
#ifndef GAMEPAWN_BORDER_MAX_Y
	#warning GAMEPAWN_BORDER_MAX_Y is not defined in "msxgl_config.h"! Default value will be used: 192
	#define GAMEPAWN_BORDER_MAX_Y		192
#endif


// GAMEPAWN_COL_DOWN
#ifndef GAMEPAWN_COL_DOWN
	#warning GAMEPAWN_COL_DOWN is not defined in "msxgl_config.h"! Default value will be used: GAMEPAWN_COL_50
	#define GAMEPAWN_COL_DOWN			GAMEPAWN_COL_50
#endif

// GAMEPAWN_COL_UP
#ifndef GAMEPAWN_COL_UP
	#warning GAMEPAWN_COL_UP is not defined in "msxgl_config.h"! Default value will be used: GAMEPAWN_COL_50
	#define GAMEPAWN_COL_UP				GAMEPAWN_COL_50
#endif

// GAMEPAWN_COL_RIGHT
#ifndef GAMEPAWN_COL_RIGHT
	#warning GAMEPAWN_COL_RIGHT is not defined in "msxgl_config.h"! Default value will be used: GAMEPAWN_COL_50
	#define GAMEPAWN_COL_RIGHT			GAMEPAWN_COL_50
#endif

// GAMEPAWN_COL_LEFT
#ifndef GAMEPAWN_COL_LEFT
	#warning GAMEPAWN_COL_LEFT is not defined in "msxgl_config.h"! Default value will be used: GAMEPAWN_COL_50
	#define GAMEPAWN_COL_LEFT			GAMEPAWN_COL_50
#endif

// GAMEPAWN_BOUND_X
#ifndef GAMEPAWN_BOUND_X
	#warning GAMEPAWN_BOUND_X is not defined in "msxgl_config.h"! Default value will be used: GAMEPAWN_BOUND_CUSTOM
	#define GAMEPAWN_BOUND_X			GAMEPAWN_BOUND_CUSTOM
#endif

// GAMEPAWN_BOUND_Y
#ifndef GAMEPAWN_BOUND_Y
	#warning GAMEPAWN_BOUND_Y is not defined in "msxgl_config.h"! Default value will be used: GAMEPAWN_BOUND_CUSTOM
	#define GAMEPAWN_BOUND_Y			GAMEPAWN_BOUND_CUSTOM
#endif

//-----------------------------------------------------------------------------
// STRING MODULE
//-----------------------------------------------------------------------------

// STRING_USE_FORMAT
#ifndef STRING_USE_FORMAT
	#warning STRING_USE_FORMAT is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define STRING_USE_FORMAT			TRUE
#endif

// STRING_USE_INT32
#ifndef STRING_USE_INT32
	#warning STRING_USE_INT32 is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define STRING_USE_INT32			FALSE
#endif

// STRING_USE_FROM_INT8
#ifndef STRING_USE_FROM_INT8
	#warning STRING_USE_FROM_INT8 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define STRING_USE_FROM_INT8		TRUE
#endif

// STRING_USE_FROM_UINT8
#ifndef STRING_USE_FROM_UINT8
	#warning STRING_USE_FROM_UINT8 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define STRING_USE_FROM_UINT8		TRUE
#endif

// STRING_USE_FROM_INT16
#ifndef STRING_USE_FROM_INT16
	#warning STRING_USE_FROM_INT16 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define STRING_USE_FROM_INT16		TRUE
#endif

// STRING_USE_FROM_UINT16
#ifndef STRING_USE_FROM_UINT16
	#warning STRING_USE_FROM_UINT16 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define STRING_USE_FROM_UINT16		TRUE
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
// PT3 MODULE
//-----------------------------------------------------------------------------

// PT3_SKIP_HEADER
#ifndef PT3_SKIP_HEADER
	#warning PT3_SKIP_HEADER is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define PT3_SKIP_HEADER				TRUE
#endif

// PT3_AUTOPLAY
#ifndef PT3_AUTOPLAY
	#warning PT3_AUTOPLAY is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define PT3_AUTOPLAY				FALSE
#endif

// PT3_EXTRA
#ifndef PT3_EXTRA
	#warning PT3_EXTRA is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define PT3_EXTRA					TRUE
#endif

//-----------------------------------------------------------------------------
// TRILO MODULE
//-----------------------------------------------------------------------------

// TRILO_USE_SFXPLAY
#ifndef TRILO_USE_SFXPLAY
	#warning TRILO_USE_SFXPLAY is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define TRILO_USE_SFXPLAY			TRUE
#endif

// TRILO_USE_TREMOLO
#ifndef TRILO_USE_TREMOLO
	#warning TRILO_USE_TREMOLO is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define TRILO_USE_TREMOLO			TRUE
#endif

// TRILO_USE_TAIL
#ifndef TRILO_USE_TAIL
	#warning TRILO_USE_TAIL is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define TRILO_USE_TAIL				FALSE
#endif

//-----------------------------------------------------------------------------
// MISC
//-----------------------------------------------------------------------------

// DEBUG_TOOL
#if !defined(DEBUG_TOOL)
	#warning DEBUG_TOOL is not defined in "msxgl_config.h"! Default value will be used: DEBUG_DISABLE
	#define DEBUG_TOOL					DEBUG_DISABLE
#endif

// PROFILE_LEVEL
#if !defined(PROFILE_LEVEL)
	#warning PROFILE_LEVEL is not defined in "msxgl_config.h"! Default value will be used: 10
	#define PROFILE_LEVEL				10
#endif

