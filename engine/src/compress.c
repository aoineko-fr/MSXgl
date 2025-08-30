// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "compress.h"
#include "memory.h"

//=============================================================================
// RLEp
//=============================================================================
#if (COMPRESS_USE_RLEP)

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

//-----------------------------------------------------------------------------
/// Unpack RLEp compressed data to memory
u16 RLEp_UnpackToRAM(const u8* src, u8* dst RLEP_FIXSIZE_PARAM)
{
	RLEP_DEFAULT_SET
	
	u16 start = (u16)dst; 
	
	RLEP_FIXSIZE_WHILE
	{
		// Unpack chunk header
		u8 type = *src >> 6;
		u8 count = (*src & 0x3F) + 1;
		
		src++;
		if (type == 0) // Chunk of zeros
		{
			Mem_Set(RLEP_DEFAULT_GET, dst, count);
		}
		else if (type == 1) // Chunk of same byte
		{
			Mem_Set(*src, dst, count);
			src++;
		}
		else if (type == 2) // Chunk of same 2 bytes
		{
			count <<= 1;
			for (u8 i = 0; i < count; ++i)
				dst[i] = src[i & 0x1];
			src += 2;
		}
		else // type == 3 // Chunk of uncompressed data
		{
			Mem_Copy(src, dst, count);
			src += count;
		}
		dst += count;
	}
	return ((u16)dst - start);
}

#endif // (COMPRESS_USE_RLEP)