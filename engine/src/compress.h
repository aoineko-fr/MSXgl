// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#pragma once
#include "core.h"

//=============================================================================
// RLEp
//=============================================================================
#if (USE_COMPRESS_RLEP)

#if (USE_COMPRESS_RLEP_DEFAULT)
	#define RLEP_DEFAULT_SET	u8 def = *src++;
	#define RLEP_DEFAULT_GET	def
#else
	#define RLEP_DEFAULT_SET
	#define RLEP_DEFAULT_GET	0
#endif

#if (USE_COMPRESS_RLEP_FIXSIZE)
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

/// Unpack RLEp compressed data to memory
u16 UnpackRLEpToRAM(const u8* src, u8* dst RLEP_FIXSIZE_PARAM);

#endif // (USE_COMPRESS_RLEP)
