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
#define GET_TARGET_TYPE(t)		(((t) >> 14) & 0x3)
#define GET_TARGET_ISR(t)		(((t) >> 12) & 0x1)
#define GET_TARGET_MAPPER(t)	(((t) >> 8)  & 0xF)
#define GET_TARGET_SIZE(t)		(((t) >> 4)  & 0xF)
#define GET_TARGET_START(t)		(((t) >> 2)  & 0x3)
#define GET_TARGET_BOOT(t)		( (t)        & 0x3)

#define TARGET_TYPE				GET_TARGET_TYPE(TARGET)
#define ROM_MAPPER				GET_TARGET_MAPPER(TARGET)
#define ROM_SIZE				GET_TARGET_SIZE(TARGET)

//-----------------------------------------------------------------------------
// ROM mapper segment switch addresses
#if (ROM_MAPPER == ROM_ASCII8)
	#define ADDR_BANK_0			0x6000 // 4000h - 5FFFh
	#define ADDR_BANK_1			0x6800 // 6000h - 7FFFh
	#define ADDR_BANK_2			0x7000 // 8000h - 9FFFh
	#define ADDR_BANK_3			0x7800 // A000h - BFFFh
#elif (ROM_MAPPER == ROM_ASCII16)
	#define ADDR_BANK_0			0x6000 // 4000h - 7FFFh
	#define ADDR_BANK_1			0x77FF // 8000h - BFFFh (or 0x7000 ?)
#elif (ROM_MAPPER == ROM_KONAMI)
	//#define ADDR_BANK_0		        / 4000h - 5FFFh
	#define ADDR_BANK_1			0x6000 // 6000h - 7FFFh
	#define ADDR_BANK_2			0x8000 // 8000h - 9FFFh
	#define ADDR_BANK_3			0xA000 // A000h - BFFFh
#elif (ROM_MAPPER == ROM_KONAMI_SCC)
	#define ADDR_BANK_0			0x5000 // 4000h - 5FFFh
	#define ADDR_BANK_1			0x7000 // 6000h - 7FFFh
	#define ADDR_BANK_2			0x9000 // 8000h - 9FFFh
	#define ADDR_BANK_3			0xB000 // A000h - BFFFh
#elif (ROM_MAPPER == ROM_NEO8)
	#define ADDR_BANK_0			0x5000 // 0000h - 1FFFh
	#define ADDR_BANK_1			0x5800 // 2000h - 3FFFh
	#define ADDR_BANK_2			0x6000 // 4000h - 5FFFh
	#define ADDR_BANK_3			0x6800 // 6000h - 7FFFh
	#define ADDR_BANK_4			0x7000 // 8000h - 9FFFh
	#define ADDR_BANK_5			0x7800 // A000h - BFFFh
#elif (ROM_MAPPER == ROM_NEO16)
	#define ADDR_BANK_0			0x5000 // 0000h - 3FFFh
	#define ADDR_BANK_1			0x6000 // 4000h - 7FFFh
	#define ADDR_BANK_2			0x7000 // 8000h - BFFFh
#endif

//-----------------------------------------------------------------------------
// ROM mapper segment size
#if (ROM_MAPPER >= ROM_MAPPER_16K)
	#define ROM_SEGMENT_SIZE	16
#else 
	#define ROM_SEGMENT_SIZE	8
#endif

//-----------------------------------------------------------------------------
// ROM mapper segments count
#if (ROM_SIZE == ROM_64K)
	#define ROM_SEGMENTS	(64/ROM_SEGMENT_SIZE)
#elif (ROM_SIZE == ROM_128K)
	#define ROM_SEGMENTS	(128/ROM_SEGMENT_SIZE)
#elif (ROM_SIZE == ROM_256K)
	#define ROM_SEGMENTS	(256/ROM_SEGMENT_SIZE)
#elif (ROM_SIZE == ROM_512K)
	#define ROM_SEGMENTS	(512/ROM_SEGMENT_SIZE)
#elif (ROM_SIZE == ROM_1M)
	#define ROM_SEGMENTS	(1*1024/ROM_SEGMENT_SIZE)
#elif (ROM_SIZE == ROM_2M)
	#define ROM_SEGMENTS	(2*1024/ROM_SEGMENT_SIZE)
#elif (ROM_SIZE == ROM_4M)
	#define ROM_SEGMENTS	(4*1024/ROM_SEGMENT_SIZE)
#elif (ROM_SIZE == ROM_8M)
	#define ROM_SEGMENTS	(8*1024/ROM_SEGMENT_SIZE)
#elif (ROM_SIZE == ROM_16M)
	#define ROM_SEGMENTS	(16*1024/ROM_SEGMENT_SIZE)
#elif (ROM_SIZE == ROM_32M)
	#define ROM_SEGMENTS	(32*1024/ROM_SEGMENT_SIZE)
#elif (ROM_SIZE == ROM_64M)
	#define ROM_SEGMENTS	(64*1024/ROM_SEGMENT_SIZE)
#endif

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

#endif