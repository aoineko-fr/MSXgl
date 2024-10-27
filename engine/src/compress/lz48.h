// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄ ▄▄ ▄  ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ▄█▀ ▀█▄▀ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▄ ██ █ ▀▄█▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// LZ48 decoder by Roudoudou
//  https://www.cpcwiki.eu/forum/programming/lz48-cruncherdecruncher/
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//-----------------------------------------------------------------------------
// OPTIONS VALIDATION
//-----------------------------------------------------------------------------

// LZ48_MODE
#ifndef LZ48_MODE
	#warning LZ48_MODE is not defined in "msxgl_config.h"! Default value will be used: LZ48_MODE_STANDARD
	#define LZ48_MODE					LZ48_MODE_STANDARD
#endif

//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

// Function: LZ48_UnpackToRAM
// Unpack LZ48 compressed data to a RAM buffer
//
// Paramaters:
//   source	- Address of the source data.
//   dest	- Address of unpack destination in RAM.
void LZ48_UnpackToRAM(const void* source, void* dest);