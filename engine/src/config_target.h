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
#define GET_TARGET_ISR(t)			(((t) >> 12) & 0x1)
#define GET_TARGET_MAPPER(t)		(((t) >> 8)  & 0xF)
#define GET_TARGET_SIZE(t)			(((t) >> 4)  & 0xF)
#define GET_TARGET_START(t)			(((t) >> 2)  & 0x3)
#define GET_TARGET_BOOT(t)			( (t)        & 0x3)

#define TARGET_TYPE					GET_TARGET_TYPE(TARGET)
#define TARGET_ISR					GET_TARGET_ISR(TARGET)
#define ROM_MAPPER					GET_TARGET_MAPPER(TARGET)
#define ROM_SIZE					GET_TARGET_SIZE(TARGET)
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

#elif (TARGET == TARGET_ROM_ASCII8_64K)
	#define TARGET_NAME "64K ASCII-8 ROM"

#elif (TARGET == TARGET_ROM_ASCII8_128K)
	#define TARGET_NAME "128K ASCII-8 ROM"

#elif (TARGET == TARGET_ROM_ASCII8_256K)
	#define TARGET_NAME "256K ASCII-8 ROM"

#elif (TARGET == TARGET_ROM_ASCII8_512K)
	#define TARGET_NAME "512 ASCII-8 ROM"

#elif (TARGET == TARGET_ROM_ASCII8_1024K)
	#define TARGET_NAME "1M ASCII-8 ROM"

#elif (TARGET == TARGET_ROM_ASCII8_2048K)
	#define TARGET_NAME "2M ASCII-8 ROM"

#elif (TARGET == TARGET_ROM_ASCII16_64K)
	#define TARGET_NAME "64K ASCII-16 ROM"

#elif (TARGET == TARGET_ROM_ASCII16_128K)
	#define TARGET_NAME "128K ASCII-16 ROM"

#elif (TARGET == TARGET_ROM_ASCII16_256K)
	#define TARGET_NAME "256K ASCII-16 ROM"

#elif (TARGET == TARGET_ROM_ASCII16_512K)
	#define TARGET_NAME "512K ASCII-16 ROM"

#elif (TARGET == TARGET_ROM_ASCII16_1024K)
	#define TARGET_NAME "1M ASCII-16 ROM"

#elif (TARGET == TARGET_ROM_ASCII16_2048K)
	#define TARGET_NAME "2M ASCII-16 ROM"

#elif (TARGET == TARGET_ROM_ASCII16_4096K)
	#define TARGET_NAME "4M ASCII-16 ROM"

#elif (TARGET == TARGET_ROM_KONAMI_64K)
	#define TARGET_NAME "64K Konami ROM"

#elif (TARGET == TARGET_ROM_KONAMI_128K)
	#define TARGET_NAME "128K Konami ROM"

#elif (TARGET == TARGET_ROM_KONAMI_256K)
	#define TARGET_NAME "256K Konami ROM"

#elif (TARGET == TARGET_ROM_KONAMI_512K)
	#define TARGET_NAME "512K Konami ROM"

#elif (TARGET == TARGET_ROM_KONAMI_1024K)
	#define TARGET_NAME "1024K Konami ROM"

#elif (TARGET == TARGET_ROM_KONAMI_2048K)
	#define TARGET_NAME "2048K Konami ROM"

#elif (TARGET == TARGET_ROM_KONAMI_SCC_64K)
	#define TARGET_NAME "64K Konami ROM with SCC"

#elif (TARGET == TARGET_ROM_KONAMI_SCC_128K)
	#define TARGET_NAME "128K Konami ROM with SCC"

#elif (TARGET == TARGET_ROM_KONAMI_SCC_256K)
	#define TARGET_NAME "256K Konami ROM with SCC"

#elif (TARGET == TARGET_ROM_KONAMI_SCC_512K)
	#define TARGET_NAME "512K Konami ROM with SCC"

#elif (TARGET == TARGET_ROM_KONAMI_SCC_1024K)
	#define TARGET_NAME "1024K Konami ROM with SCC"

#elif (TARGET == TARGET_ROM_KONAMI_SCC_2048K)
	#define TARGET_NAME "2048K Konami ROM with SCC"

#elif (TARGET == TARGET_ROM_NEO8_128K)
	#define TARGET_NAME "128K NEO-8 ROM"

#elif (TARGET == TARGET_ROM_NEO8_256K)
	#define TARGET_NAME "256K NEO-8 ROM"

#elif (TARGET == TARGET_ROM_NEO8_512K)
	#define TARGET_NAME "512K NEO-8 ROM"

#elif (TARGET == TARGET_ROM_NEO8_1M)
	#define TARGET_NAME "1M NEO-8 ROM"

#elif (TARGET == TARGET_ROM_NEO8_2M)
	#define TARGET_NAME "2M NEO-8 ROM"

#elif (TARGET == TARGET_ROM_NEO8_4M)
	#define TARGET_NAME "4M NEO-8 ROM"

#elif (TARGET == TARGET_ROM_NEO8_8M)
	#define TARGET_NAME "8M NEO-8 ROM"

#elif (TARGET == TARGET_ROM_NEO8_16M)
	#define TARGET_NAME "16M NEO-8 ROM"

#elif (TARGET == TARGET_ROM_NEO8_32M)
	#define TARGET_NAME "32M NEO-8 ROM"

#elif (TARGET == TARGET_ROM_NEO16_128K)
	#define TARGET_NAME "128K NEO-16 ROM"

#elif (TARGET == TARGET_ROM_NEO16_256K)
	#define TARGET_NAME "256K NEO-16 ROM"

#elif (TARGET == TARGET_ROM_NEO16_512K)
	#define TARGET_NAME "512K NEO-16 ROM"

#elif (TARGET == TARGET_ROM_NEO16_1M)
	#define TARGET_NAME "1M NEO-16 ROM"

#elif (TARGET == TARGET_ROM_NEO16_2M)
	#define TARGET_NAME "2M NEO-16 ROM"

#elif (TARGET == TARGET_ROM_NEO16_4M)
	#define TARGET_NAME "4M NEO-16 ROM"

#elif (TARGET == TARGET_ROM_NEO16_8M)
	#define TARGET_NAME "8M NEO-16 ROM"

#elif (TARGET == TARGET_ROM_NEO16_16M)
	#define TARGET_NAME "16M NEO-16 ROM"

#elif (TARGET == TARGET_ROM_NEO16_32M)
	#define TARGET_NAME "32M NEO-16 ROM"

#elif (TARGET == TARGET_ROM_NEO16_64M)
	#define TARGET_NAME "64M NEO-16 ROM"

#elif (TARGET == TARGET_ROM_YAMANOOTO_2M)
	#define TARGET_NAME "2M YAMANOOTO ROM"

#elif (TARGET == TARGET_ROM_YAMANOOTO_4M)
	#define TARGET_NAME "4M YAMANOOTO ROM"

#elif (TARGET == TARGET_ROM_YAMANOOTO_8M)
	#define TARGET_NAME "8M YAMANOOTO ROM"

#elif (TARGET == TARGET_ROM_YAMANOOTO_16M)
	#define TARGET_NAME "16M YAMANOOTO ROM"

#elif (TARGET == TARGET_ROM_YAMANOOTO_32M)
	#define TARGET_NAME "32M YAMANOOTO ROM"

#elif (TARGET == TARGET_ROM_ASCII16X_8M)
	#define TARGET_NAME "8M ASCII16-X ROM"

#elif (TARGET == TARGET_ROM_ASCII16X_16M)
	#define TARGET_NAME "16M ASCII16-X ROM"

#elif (TARGET == TARGET_ROM_ASCII16X_32M)
	#define TARGET_NAME "32M ASCII16-X ROM"

#elif (TARGET == TARGET_ROM_ASCII16X_64M)
	#define TARGET_NAME "64M ASCII16-X ROM"

#endif