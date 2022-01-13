// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄             ▄▄  ▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ▄█▀▄ ██▀▄ ██ ▀ ▄  ▄▀██ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ▀█▄▀ ██ █ ██▀  ██  ▀██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                          ▀▀  
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// Validation of configuration defines
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//-----------------------------------------------------------------------------
//
// V A L I D A T I O N
//
//-----------------------------------------------------------------------------

#if (SDCC_VERSION_CURRENT < VERSION(4,1,12))
	#error MSXGL need SDCC version 4.1.12 or newer!
#endif

// Check MSX 1 dependency
#if (MSX_VERSION == MSX_1)

	// Check for MSX1 supported 64K max VRAM size
	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		#warning Cant use VDP_VRAM_ADDR_17 with MSX1! VDP_VRAM_ADDR_14 will be used instead
		#undef  VDP_VRAM_ADDR
		#define VDP_VRAM_ADDR			VDP_VRAM_ADDR_14
	#endif

	// Check for MSX1 supported screen mode
	#if (USE_VDP_MODE_T2)
		#warning Cant use USE_VDP_MODE_T2 with MSX1! Option will be disable
		#undef  USE_VDP_MODE_T2
		#define USE_VDP_MODE_T2			0
	#endif
	#if (USE_VDP_MODE_G3)
		#warning Cant use USE_VDP_MODE_G3 with MSX1! Option will be disable
		#undef  USE_VDP_MODE_G3
		#define USE_VDP_MODE_G3			0
	#endif
	#if (USE_VDP_MODE_G4)
		#warning Cant use USE_VDP_MODE_G4 with MSX1! Option will be disable
		#undef  USE_VDP_MODE_G4
		#define USE_VDP_MODE_G4			0
	#endif
	#if (USE_VDP_MODE_G5)
		#warning Cant use USE_VDP_MODE_G5 with MSX1! Option will be disable
		#undef  USE_VDP_MODE_G5
		#define USE_VDP_MODE_G5			0
	#endif
	#if (USE_VDP_MODE_G6)
		#warning Cant use USE_VDP_MODE_G6 with MSX1! Option will be disable
		#undef  USE_VDP_MODE_G6
		#define USE_VDP_MODE_G6			0
	#endif
	#if (USE_VDP_MODE_G7)
		#warning Cant use USE_VDP_MODE_G7 with MSX1! Option will be disable
		#undef  USE_VDP_MODE_G7
		#define USE_VDP_MODE_G7			0
	#endif
	#if (USE_PRINT_BITMAP)
		#warning Cant use USE_PRINT_BITMAP with MSX1! Option will be disable
		#undef  USE_PRINT_BITMAP
		#define USE_PRINT_BITMAP		0
	#endif
#endif // (MSX_VERSION == MSX_1)

// Check USE_PRINT_BITMAP dependency
#if(!USE_PRINT_BITMAP)
	#if(USE_PRINT_VRAM)
		#warning USE_PRINT_VRAM need USE_PRINT_BITMAP to be set! Option will be disable
		#undef  USE_PRINT_VRAM
		#define USE_PRINT_VRAM			0
	#endif

	#if(USE_PRINT_FX_SHADOW)
		#warning USE_PRINT_FX_SHADOW need USE_PRINT_BITMAP to be set! Option will be disable
		#undef  USE_PRINT_FX_SHADOW
		#define USE_PRINT_FX_SHADOW			0
	#endif

	#if(USE_PRINT_FX_OUTLINE)
		#warning USE_PRINT_FX_OUTLINE need USE_PRINT_BITMAP to be set! Option will be disable
		#undef  USE_PRINT_FX_OUTLINE
		#define USE_PRINT_FX_OUTLINE			0
	#endif
#endif // (!USE_PRINT_BITMAP)