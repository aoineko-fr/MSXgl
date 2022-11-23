// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// RLEp
//=============================================================================
#if (COMPRESS_USE_RLEP)

#if (COMPRESS_USE_RLEP_DEFAULT)
	#define RLEP_DEFAULT_SET	u8 def = *src++;
	#define RLEP_DEFAULT_GET	def
#else
	#define RLEP_DEFAULT_SET
	#define RLEP_DEFAULT_GET	0
#endif

#if (COMPRESS_USE_RLEP_FIXSIZE)
	#define RLEP_FIXSIZE_PARAM	, u8 size
	#define RLEP_FIXSIZE_WHILE	while(((u16)dst - start) < size)
#else
	#define RLEP_FIXSIZE_PARAM
	#define RLEP_FIXSIZE_WHILE	while(*src != 0)
#endif

//-----------------------------------------------------------------------------
// RLEp Chunk Header
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	T1	T0	C5	C4	C3	C2	C1	C0	
//	│	│	└───┴───┴───┴───┴───┴── Count (0-63). Must be increment to get the right value.
//	└───┴────────────────────────── Type (0-3). 
//									0: A sequence of zeros.
//									1: A sequence of a given 1 byte (provided after the header).
//									2: A sequence of a given 2 bytes (provided after the header).
//									3: A sequence of uncompressed data (provided after the header).

// Function: RLEp_UnpackToRAM
// Unpack RLEp compressed data to memory
//
// Parameters:
//   src - Source data
//   dst - Destination data in RAM
//   size - Size of the data to unpack (only if COMPRESS_USE_RLEP_FIXSIZE is defined)
u16 RLEp_UnpackToRAM(const u8* src, u8* dst RLEP_FIXSIZE_PARAM);

#endif // (COMPRESS_USE_RLEP)
