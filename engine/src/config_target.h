// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄             ▄▄  ▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ▄█▀▄ ██▀▄ ██ ▀ ▄  ▄▀██ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ▀█▄▀ ██ █ ██▀  ██  ▀██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                          ▀▀  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Target media handling defines
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//-----------------------------------------------------------------------------
// Get current targer parameters
#define GET_TARGET_TYPE(t)			(((t) >> 14) & 0x3)
#define GET_TARGET_ISR(t)			(((t) >> 13) & 0x1)
#define GET_TARGET_MIRROR(t)		(((t) >> 12) & 0x1)
#define GET_TARGET_MAPPER(t)		(((t) >> 8)  & 0xF)
#define GET_TARGET_FRAME(t)			(((t) >> 4)  & 0xF)
#define GET_TARGET_START(t)			(((t) >> 2)  & 0x3)
#define GET_TARGET_BOOT(t)			( (t)        & 0x3)

#define TARGET_TYPE					GET_TARGET_TYPE(TARGET)
#define ROM_MAPPER					GET_TARGET_MAPPER(TARGET)
#define ROM_FRAME					GET_TARGET_FRAME(TARGET)
#define ROM_START					GET_TARGET_START(TARGET)
#define ROM_BOOT					GET_TARGET_BOOT(TARGET)

//-----------------------------------------------------------------------------
// Target names
#if (TARGET == TARGET_BIN_DISK)
	#define TARGET_NAME "BASIC binary on disk"

#elif (TARGET == TARGET_BIN_TAPE)
	#define TARGET_NAME "BASIC binary on tape"

#elif (TARGET == TARGET_BIN_USR)
	#define TARGET_NAME "BASIC USR driver"

#elif (TARGET == TARGET_DOS0)
	#define TARGET_NAME "Auto boot disk"

#elif (TARGET == TARGET_DOS1)
	#define TARGET_NAME "MSX-DOS 1"

#elif (TARGET == TARGET_DOS2)
	#define TARGET_NAME "MSX-DOS 2"

#elif (TARGET == TARGET_DOS2_MAPPER)
	#define TARGET_NAME "MSX-DOS 2 with RAM mapper"

#elif (TARGET == TARGET_DOS3)
	#define TARGET_NAME "NEXTOR"

#elif (TARGET == TARGET_ROM_8K_P1)
	#define TARGET_NAME "8K ROM in page 1"

#elif (TARGET == TARGET_ROM_8K_P2)
	#define TARGET_NAME "8K ROM in page 2"

#elif (TARGET == TARGET_ROM_16K_P1)
	#define TARGET_NAME "16K ROM in page 1"

#elif (TARGET == TARGET_ROM_16K_P2)
	#define TARGET_NAME "16K ROM in page 2"

#elif (TARGET == TARGET_ROM_32K_P0)
	#define TARGET_NAME "32K ROM in pages 0-1"

#elif (TARGET == TARGET_ROM_32K_P0_ISR)
	#define TARGET_NAME "32K ROM in pages 0-1 with ISR"

#elif (TARGET == TARGET_ROM_32K_P1)
	#define TARGET_NAME "32K ROM in pages 1-2 @4000h"

#elif (TARGET == TARGET_ROM_32K_P1_B2)
	#define TARGET_NAME "32K ROM in pages 1-2 @8000h"

#elif (TARGET == TARGET_ROM_48K_B1)
	#define TARGET_NAME "48K ROM in pages 0-2 @4000h"

#elif (TARGET == TARGET_ROM_48K_B1_ISR)
	#define TARGET_NAME "48K ROM in pages 0-2 @4000h with ISR"

#elif (TARGET == TARGET_ROM_48K_B2)
	#define TARGET_NAME "48K ROM in pages 0-2 @8000h"

#elif (TARGET == TARGET_ROM_48K_B2_ISR)
	#define TARGET_NAME "48K ROM in pages 0-2 @8000h with ISR"

#elif (TARGET == TARGET_ROM_64K_B1)
	#define TARGET_NAME "64K ROM in pages 0-3 @4000"

#elif (TARGET == TARGET_ROM_64K_B1_ISR)
	#define TARGET_NAME "64K ROM in pages 0-3 @4000 with ISR"

#elif (TARGET == TARGET_ROM_64K_B2)
	#define TARGET_NAME "64K ROM in pages 0-3 @8000"

#elif (TARGET == TARGET_ROM_64K_B2_ISR)
	#define TARGET_NAME "64K ROM in pages 0-3 @8000 with ISR"

#elif (TARGET == TARGET_ROM_ASCII8)
	#define TARGET_NAME "ASCII-8 ROM"

#elif (TARGET == TARGET_ROM_ASCII16)
	#define TARGET_NAME "ASCII-16 ROM"

#elif (TARGET == TARGET_ROM_KONAMI)
	#define TARGET_NAME "Konami ROM"

#elif (TARGET == TARGET_ROM_KONAMI_SCC)
	#define TARGET_NAME "Konami ROM with SCC"

#elif (TARGET == TARGET_ROM_NEO8)
	#define TARGET_NAME "NEO-8 ROM"

#elif (TARGET == TARGET_ROM_NEO16)
	#define TARGET_NAME "NEO-16 ROM"

#elif (TARGET == TARGET_ROM_YAMANOOTO)
	#define TARGET_NAME "YAMANOOTO ROM"

#elif (TARGET == TARGET_ROM_ASCII16X)
	#define TARGET_NAME "ASCII16-X ROM"

#endif