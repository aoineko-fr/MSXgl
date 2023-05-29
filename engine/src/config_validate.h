// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄             ▄▄  ▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ▄█▀▄ ██▀▄ ██ ▀ ▄  ▄▀██ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ▀█▄▀ ██ █ ██▀  ██  ▀██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                          ▀▀  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Validation of configuration defines
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//-----------------------------------------------------------------------------
//
// V A L I D A T I O N
//
//-----------------------------------------------------------------------------

// Check SDCC version
#if (SDCC_VERSION_CURRENT < VERSION(4,1,12))
	#error MSXgl need SDCC version 4.1.12 or newer!
#endif

// Check MSX 1 dependency
#if (MSX_VERSION == MSX_1)

	// Check for MSX1 supported 64K max VRAM size
	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		#warning Cant use VDP_VRAM_ADDR_17 with MSX1! VDP_VRAM_ADDR_14 will be used instead
		#undef  VDP_VRAM_ADDR
		#define VDP_VRAM_ADDR			VDP_VRAM_ADDR_14
	#endif
	#if (!VDP_USE_VRAM16K)
		#warning VDP_USE_VRAM16K is mandatory with MSX1! Option will be enable
		#undef  VDP_USE_VRAM16K
		#define VDP_USE_VRAM16K			TRUE
	#endif

	// Check for MSX1 supported screen mode
	#if (VDP_USE_MODE_T2)
		#warning Cant use VDP_USE_MODE_T2 with MSX1! Option will be disable
		#undef  VDP_USE_MODE_T2
		#define VDP_USE_MODE_T2			FALSE
	#endif
	#if (VDP_USE_MODE_G3)
		#warning Cant use VDP_USE_MODE_G3 with MSX1! Option will be disable
		#undef  VDP_USE_MODE_G3
		#define VDP_USE_MODE_G3			FALSE
	#endif
	#if (VDP_USE_MODE_G4)
		#warning Cant use VDP_USE_MODE_G4 with MSX1! Option will be disable
		#undef  VDP_USE_MODE_G4
		#define VDP_USE_MODE_G4			FALSE
	#endif
	#if (VDP_USE_MODE_G5)
		#warning Cant use VDP_USE_MODE_G5 with MSX1! Option will be disable
		#undef  VDP_USE_MODE_G5
		#define VDP_USE_MODE_G5			FALSE
	#endif
	#if (VDP_USE_MODE_G6)
		#warning Cant use VDP_USE_MODE_G6 with MSX1! Option will be disable
		#undef  VDP_USE_MODE_G6
		#define VDP_USE_MODE_G6			FALSE
	#endif
	#if (VDP_USE_MODE_G7)
		#warning Cant use VDP_USE_MODE_G7 with MSX1! Option will be disable
		#undef  VDP_USE_MODE_G7
		#define VDP_USE_MODE_G7			FALSE
	#endif

	// Print module
	#if (PRINT_USE_BITMAP)
		#warning Cant use PRINT_USE_BITMAP with MSX1! Option will be disable
		#undef  PRINT_USE_BITMAP
		#define PRINT_USE_BITMAP		FALSE
	#endif
#endif // (MSX_VERSION == MSX_1)

// Check PRINT_USE_BITMAP dependency
#if(!PRINT_USE_BITMAP)
	#if(PRINT_USE_VRAM)
		#warning PRINT_USE_VRAM need PRINT_USE_BITMAP to be set! Option will be disable
		#undef  PRINT_USE_VRAM
		#define PRINT_USE_VRAM			FALSE
	#endif

	#if(PRINT_USE_FX_SHADOW)
		#warning PRINT_USE_FX_SHADOW need PRINT_USE_BITMAP to be set! Option will be disable
		#undef  PRINT_USE_FX_SHADOW
		#define PRINT_USE_FX_SHADOW		FALSE
	#endif

	#if(PRINT_USE_FX_OUTLINE)
		#warning PRINT_USE_FX_OUTLINE need PRINT_USE_BITMAP to be set! Option will be disable
		#undef  PRINT_USE_FX_OUTLINE
		#define PRINT_USE_FX_OUTLINE	FALSE
	#endif
#endif // (!PRINT_USE_BITMAP)

#if(PRINT_USE_BITMAP)
	#if(!VDP_USE_COMMAND)
		#warning PRINT_USE_BITMAP need VDP_USE_COMMAND to be set! Option will be enable
		#undef  VDP_USE_COMMAND
		#define VDP_USE_COMMAND			TRUE
	#endif
#endif

// Check PRINT_USE_SPRITE dependency
#if(PRINT_USE_SPRITE)
	#if(!VDP_USE_SPRITE)
		#warning PRINT_USE_SPRITE need VDP_USE_SPRITE to be set! Option will be enable
		#undef  VDP_USE_SPRITE
		#define VDP_USE_SPRITE			TRUE
	#endif
#endif




