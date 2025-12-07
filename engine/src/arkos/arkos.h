// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄            
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄█ ██▄▀ ██▄▀ ▄█▀▄  ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ██   ██ █ ▀█▄▀ ▄██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Arkos Tracker defines
//
// Arkos Tracker and the Z80 players are coded and designed by Julien Névo a.k.a Targhan/Arkos.
//  https://www.julien-nevo.com/arkostracker/index.php/credits/
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// VALIDATE
//=============================================================================

// ARKOS_BUFFER_ADDR
#if !defined(ARKOS_BUFFER_ADDR)
	#warning ARKOS_BUFFER_ADDR is not defined in "msxgl_config.h"! Default value will be used: 0xF000
	#define ARKOS_BUFFER_ADDR		0xF000
#endif

// ARKOS_ISR_PROTECTION
#if !defined(ARKOS_ISR_PROTECTION)
	#warning ARKOS_ISR_PROTECTION is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define ARKOS_ISR_PROTECTION	FALSE
#endif

// ARKOS_SFX_START_IDX
#if !defined(ARKOS_SFX_START_IDX)
	#warning ARKOS_SFX_START_IDX is not defined in "msxgl_config.h"! Default value will be used: 1
	#define ARKOS_SFX_START_IDX		1
#endif

// ARKOS_USE_EVENT
#if !defined(ARKOS_USE_EVENT)
	#warning ARKOS_USE_EVENT is not defined in "msxgl_config.h"! Default value will be used: FALSE
	#define ARKOS_USE_EVENT			FALSE
#endif

//=============================================================================
// DEFINES
//=============================================================================

// Channel indexes
#define ARKOS_CHANNEL_A				0
#define ARKOS_CHANNEL_B				1
#define ARKOS_CHANNEL_C				2

// Interrupt protection macros
#if (ARKOS_ISR_PROTECTION)
	#define ARKOS_INT_DISABLE		di
	#define ARKOS_INT_ENABLE		ei
#else
	#define ARKOS_INT_DISABLE		
	#define ARKOS_INT_ENABLE
#endif

// Address conversion macro
#define ARKOS_ADDR(a)				(a - 0xF000 + ARKOS_BUFFER_ADDR) 
