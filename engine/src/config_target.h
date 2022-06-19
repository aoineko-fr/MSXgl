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
// Get current targer parameters
#if (ROM_MAPPER == ROM_ASCII8)
	#define ADDR_BANK_0			0x6000 // 4000h - 5FFFh
	#define ADDR_BANK_1			0x6800 // 6000h - 7FFFh
	#define ADDR_BANK_2			0x7000 // 8000h - 9FFFh
	#define ADDR_BANK_3			0x7800 // A000h - BFFFh
	#define ROM_SEGMENT			()
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
#endif

//-----------------------------------------------------------------------------
// Segment counts

#if (ROM_SIZE == ROM_64K)
	#if (ROM_MAPPER == ROM_ASCII16)
		#define ROM_SEGMENTS	(64/16)
	#else 
		#define ROM_SEGMENTS	(64/8)
	#endif
#elif (ROM_SIZE == ROM_128K)
	#if (ROM_MAPPER == ROM_ASCII16)
		#define ROM_SEGMENTS	(128/16)
	#else 
		#define ROM_SEGMENTS	(128/8)
	#endif
#elif (ROM_SIZE == ROM_256K)
	#if (ROM_MAPPER == ROM_ASCII16)
		#define ROM_SEGMENTS	(256/16)
	#else 
		#define ROM_SEGMENTS	(256/8)
	#endif
#elif (ROM_SIZE == ROM_512K)
	#if (ROM_MAPPER == ROM_ASCII16)
		#define ROM_SEGMENTS	(512/16)
	#else 
		#define ROM_SEGMENTS	(512/8)
	#endif
#elif (ROM_SIZE == ROM_1024K)
	#if (ROM_MAPPER == ROM_ASCII16)
		#define ROM_SEGMENTS	(1024/16)
	#else 
		#define ROM_SEGMENTS	(1024/8)
	#endif
#elif (ROM_SIZE == ROM_2048K)
	#if (ROM_MAPPER == ROM_ASCII16)
		#define ROM_SEGMENTS	(2048/16)
	#else 
		#define ROM_SEGMENTS	(2048/8)
	#endif
#elif (ROM_SIZE == ROM_4096K)
	#if (ROM_MAPPER == ROM_ASCII16)
		#define ROM_SEGMENTS	(4096/16)
	#endif
#endif

//-----------------------------------------------------------------------------
// Target names
#if (TARGET == TARGET_BIN)
	#define TARGET_NAME "BASIC binary"

#elif (TARGET == TARGET_DOS1)
	#define TARGET_NAME "MSX-DOS 1"

#elif (TARGET == TARGET_DOS2)
	#define TARGET_NAME "MSX-DOS 2"

#elif (TARGET == TARGET_DOS2_ARG)
	#define TARGET_NAME "MSX-DOS 2 with arguments"

#elif (TARGET == TARGET_ROM_8K_P1)
	#define TARGET_NAME "8K ROM in page 1"

#elif (TARGET == TARGET_ROM_8K_P2)
	#define TARGET_NAME "8K ROM in page 2"

#elif (TARGET == TARGET_ROM_16K_P1)
	#define TARGET_NAME "16K ROM in page 1"

#elif (TARGET == TARGET_ROM_16K_P2)
	#define TARGET_NAME "16K ROM in page 2"

#elif (TARGET == TARGET_ROM_32K_P0)
	#define TARGET_NAME "32K ROM in page 01"

#elif (TARGET == TARGET_ROM_32K_P0_ISR)
	#define TARGET_NAME "32K ROM in page 01 with ISR"

#elif (TARGET == TARGET_ROM_32K_P1)
	#define TARGET_NAME "32K ROM in pages 12 @4000h"

#elif (TARGET == TARGET_ROM_32K_P1_B2)
	#define TARGET_NAME "32K ROM in pages 12 @8000h"

#elif (TARGET == TARGET_ROM_48K_B1)
	#define TARGET_NAME "48K ROM @4000h"

#elif (TARGET == TARGET_ROM_48K_B1_ISR)
	#define TARGET_NAME "48K ROM @4000h with ISR"

#elif (TARGET == TARGET_ROM_48K_B2)
	#define TARGET_NAME "48K ROM @8000h"

#elif (TARGET == TARGET_ROM_48K_B2_ISR)
	#define TARGET_NAME "48K ROM @8000h with ISR"

#elif (TARGET == TARGET_ROM_64K_B1)
	#define TARGET_NAME "ROM_64K_B1"

#elif (TARGET == TARGET_ROM_64K_B1_ISR)
	#define TARGET_NAME "ROM_64K_B1_ISR"

#elif (TARGET == TARGET_ROM_64K_B2)
	#define TARGET_NAME "ROM_64K_B2"

#elif (TARGET == TARGET_ROM_64K_B2_ISR)
	#define TARGET_NAME "ROM_64K_B2_ISR"

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

#endif



