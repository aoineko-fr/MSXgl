// ____________________________
// ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄             ▄▄  ▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ▄█▀▄ ██▀▄ ██ ▀ ▄  ▄▀██ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ▀█▄▀ ██ █ ██▀  ██  ▀██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                          ▀▀  
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// Configuration defines
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// BUILD
//-----------------------------------------------------------------------------
// Those defines are setup by the Build tool

//-----------------------------------------------------------------------------
// MSX_VERSION options
#define MSX_1				0 // MSX 1
#define MSX_2				1 // MSX 2
#define MSX_2P				2 // MSX 2+
#define MSX_TR				3 // MSX Turbo-R
#define MSX_3				4 // MSX 3 (reserved)
#define MSX_12				3 // MSX 1/2

//-----------------------------------------------------------------------------
// TARGET_TYPE options
#define TYPE_BIN			0 // BASIC binary program
#define TYPE_ROM			1 // ROM program
#define TYPE_DOS			2 // MSX-DOS program

//-----------------------------------------------------------------------------
// TARGET options

// DOS options
#define DOS_1				0 // MSX-DOS 1
#define DOS_2				2 // MSX-DOS 2
#define DOS_2_ARGS			3 // MSX-DOS 2 with command-line arguments

// ROM_MAPPER options
#define ROM_PLAIN			0
#define ROM_ASCII8			1
#define ROM_ASCII16			2
#define ROM_KONAMI			3
#define ROM_KONAMI_SCC		4

// ROM_SIZE options
#define ROM_8K				0	
#define ROM_16K				1	
#define ROM_32K				2	
#define ROM_48K				3	
#define ROM_64K				4
#define ROM_128K			5	
#define ROM_256K			6	
#define ROM_512K			7	
#define ROM_1024K			8	
#define ROM_2048K			9	
#define ROM_4096K			10	

#define ROM_ISR				(1<<12)	
#define ROM_MIRROR			(1<<13)	

// ROM target type define
//	15	14	13	12	11	10	9	8	7	6	5	4	3	2	1	0
//	0	1	MIR	ISR	M3	M2	M1	M0	S3	S2	S1	S0	P1	P0	B1	B0	
//  		│	│	│	│	│	│	│	│	│	│	│	│	└───┴── ROM boot page (address of 'AB' header)
//  		│	│	│	│	│	│	│	│	│	│	└───┴────────── ROM starting page (first address of the ROM ; can be different from booting page)
//  		│	│	│	│	│	│	└───┴───┴───┴────────────────── ROM size
//			│	│	└───┴───┴───┴────────────────────────────────── ROM Mapper Type
//			│	└────────────────────────────────────────────────── Use custom Interrupt Service Routine (place custom code in page 0)
//			└────────────────────────────────────────────────────── Mirrored ROM
#define MAKE_ROM(m, s, p, b)		((TYPE_ROM << 14) | ((m) << 8) | ((s) << 4) | ((p) << 2) | (b))
#define MAKE_BASIC(x)				((TYPE_BIN << 14) | (x))
#define MAKE_DOS(x)					((TYPE_DOS << 14) | (x))
	
// -- BASIC program
#define TARGET_BIN			  		MAKE_BASIC(0) // BASIC binary program (8000h~)
// -- DOS program
#define TARGET_DOS1					MAKE_DOS(DOS_1) // MSX-DOS 1 program (0100h~). No direct acces to Main-ROM
#define TARGET_DOS2					MAKE_DOS(DOS_2) // MSX-DOS 2 program (0100h~). No direct acces to Main-ROM
#define TARGET_DOS2_ARG				MAKE_DOS(DOS_2_ARGS) // MSX-DOS 2 program (using command line arguments ; 0100h~). No direct acces to Main-ROM
#define TARGET_DOS					TARGET_DOS1
// -- Plain ROM 8KB
#define TARGET_ROM_8K_P0			MAKE_ROM(ROM_PLAIN, ROM_8K, 0, 0) + ROM_ISR + ROM_MIRROR // 8KB ROM in page 0 boot at 0000h (detected at 4000h by mirroring). With ISR replacement
#define TARGET_ROM_8K_P1			MAKE_ROM(ROM_PLAIN, ROM_8K, 1, 1) // 8KB ROM in page 1 boot at 4000h
#define TARGET_ROM_8K_P2			MAKE_ROM(ROM_PLAIN, ROM_8K, 2, 2) // 8KB ROM in page 2 boot at 8000h
#define TARGET_ROM_8K				TARGET_ROM_8K_P1
// -- Plain ROM 16KB
#define TARGET_ROM_16K_P0			MAKE_ROM(ROM_PLAIN, ROM_16K, 0, 0) + ROM_ISR + ROM_MIRROR // 16KB ROM in page 0 boot at 0000h (detected at 4000h by mirroring). With ISR replacement
#define TARGET_ROM_16K_P1			MAKE_ROM(ROM_PLAIN, ROM_16K, 1, 1) // 16KB ROM in page 1 boot at 4000h
#define TARGET_ROM_16K_P2			MAKE_ROM(ROM_PLAIN, ROM_16K, 2, 2) // 16KB ROM in page 2 boot at 8000h
#define TARGET_ROM_16K				TARGET_ROM_16K_P1
// -- Plain ROM 32KB
#define TARGET_ROM_32K_P0			MAKE_ROM(ROM_PLAIN, ROM_32K, 0, 1) // 32KB ROM in pages 0-1 boot at 4000h
#define TARGET_ROM_32K_P0_ISR		MAKE_ROM(ROM_PLAIN, ROM_32K, 0, 1) + ROM_ISR // 32KB ROM in pages 0-1 boot at 4000h. With ISR replacement
#define TARGET_ROM_32K_P1			MAKE_ROM(ROM_PLAIN, ROM_32K, 1, 1) // 32KB ROM in pages 1-2 boot at 4000h
#define TARGET_ROM_32K_P1_B2		MAKE_ROM(ROM_PLAIN, ROM_32K, 1, 2) // 32KB ROM in pages 1-2 boot at 8000h
#define TARGET_ROM_32K				TARGET_ROM_32K_P1
#define TARGET_ROM					TARGET_ROM_32K_P1
// -- Plain ROM 48KB
#define TARGET_ROM_48K_B1			MAKE_ROM(ROM_PLAIN, ROM_48K, 0, 1) // 48KB ROM in pages 0-2 boot at 4000h
#define TARGET_ROM_48K_B1_ISR		MAKE_ROM(ROM_PLAIN, ROM_48K, 0, 1) + ROM_ISR // 48KB ROM in pages 0-2 boot at 4000h. With ISR replacement
#define TARGET_ROM_48K_B2			MAKE_ROM(ROM_PLAIN, ROM_48K, 0, 2) // 48KB ROM in pages 0-2 boot at 8000h
#define TARGET_ROM_48K_B2_ISR		MAKE_ROM(ROM_PLAIN, ROM_48K, 0, 2) + ROM_ISR // 48KB ROM in pages 0-2 boot at 8000h. With ISR replacement
#define TARGET_ROM_48K				TARGET_ROM_48K_B1
#define TARGET_ROM_48K_ISR			TARGET_ROM_48K_B1_ISR
// -- Plain ROM 64KB
#define TARGET_ROM_64K_B1			MAKE_ROM(ROM_PLAIN, ROM_64K, 0, 1) // 48KB ROM in pages 0-3 boot at 4000h
#define TARGET_ROM_64K_B1_ISR		MAKE_ROM(ROM_PLAIN, ROM_64K, 0, 1) + ROM_ISR // 48KB ROM in pages 0-3 boot at 4000h. With ISR replacement
#define TARGET_ROM_64K_B2			MAKE_ROM(ROM_PLAIN, ROM_64K, 0, 2) // 48KB ROM in pages 0-3 boot at 8000h
#define TARGET_ROM_64K_B2_ISR		MAKE_ROM(ROM_PLAIN, ROM_64K, 0, 2) + ROM_ISR // 48KB ROM in pages 0-3 boot at 8000h. With ISR replacement
#define TARGET_ROM_64K				TARGET_ROM_64K_B1
#define TARGET_ROM_64K_ISR			TARGET_ROM_64K_B1_ISR
// -- ASCII 8 ROM
#define TARGET_ROM_ASCII8_64K		MAKE_ROM(ROM_ASCII8, ROM_64K, 1, 1) // ASCII 8KB ROM Mapper (8 segments)
#define TARGET_ROM_ASCII8_128K		MAKE_ROM(ROM_ASCII8, ROM_128K, 1, 1) // ASCII 8KB ROM Mapper (16 segments)
#define TARGET_ROM_ASCII8_256K		MAKE_ROM(ROM_ASCII8, ROM_256K, 1, 1) // ASCII 8KB ROM Mapper (32 segments)
#define TARGET_ROM_ASCII8_512K		MAKE_ROM(ROM_ASCII8, ROM_512K, 1, 1) // ASCII 8KB ROM Mapper (64 segments)
#define TARGET_ROM_ASCII8_1024K		MAKE_ROM(ROM_ASCII8, ROM_1024K, 1, 1) // ASCII 8KB ROM Mapper (128 segments)
#define TARGET_ROM_ASCII8_2048K		MAKE_ROM(ROM_ASCII8, ROM_2048K, 1, 1) // ASCII 8KB ROM Mapper (256 segments)
#define TARGET_ROM_ASCII8			TARGET_ROM_ASCII8_128K		
// -- ASCII 16 ROM
#define TARGET_ROM_ASCII16_64K		MAKE_ROM(ROM_ASCII16, ROM_64K, 1, 1) // ASCII 16KB ROM Mapper (4 segments)
#define TARGET_ROM_ASCII16_128K		MAKE_ROM(ROM_ASCII16, ROM_128K, 1, 1) // ASCII 16KB ROM Mapper (8 segments)
#define TARGET_ROM_ASCII16_256K		MAKE_ROM(ROM_ASCII16, ROM_256K, 1, 1) // ASCII 16KB ROM Mapper (16 segments)
#define TARGET_ROM_ASCII16_512K		MAKE_ROM(ROM_ASCII16, ROM_512K, 1, 1) // ASCII 16KB ROM Mapper (32 segments)
#define TARGET_ROM_ASCII16_1024K	MAKE_ROM(ROM_ASCII16, ROM_1024K, 1, 1) // ASCII 16KB ROM Mapper (64 segments)
#define TARGET_ROM_ASCII16_2048K	MAKE_ROM(ROM_ASCII16, ROM_2048K, 1, 1) // ASCII 16KB ROM Mapper (128 segments)
#define TARGET_ROM_ASCII16_4096K	MAKE_ROM(ROM_ASCII16, ROM_4096K, 1, 1) // ASCII 16KB ROM Mapper (256 segments)
#define TARGET_ROM_ASCII16			TARGET_ROM_ASCII16_128K
// -- Konami ROM
#define TARGET_ROM_KONAMI_64K		MAKE_ROM(ROM_KONAMI, ROM_64K, 1, 1) // Konami 8KB ROM Mapper (8 segments)
#define TARGET_ROM_KONAMI_128K		MAKE_ROM(ROM_KONAMI, ROM_128K, 1, 1) // Konami 8KB ROM Mapper (16 segments)
#define TARGET_ROM_KONAMI_256K		MAKE_ROM(ROM_KONAMI, ROM_256K, 1, 1) // Konami 8KB ROM Mapper (32 segments)
#define TARGET_ROM_KONAMI_512K		MAKE_ROM(ROM_KONAMI, ROM_512K, 1, 1) // Konami 8KB ROM Mapper (64 segments)
#define TARGET_ROM_KONAMI_1024K		MAKE_ROM(ROM_KONAMI, ROM_1024K, 1, 1) // Konami 8KB ROM Mapper (128 segments)
#define TARGET_ROM_KONAMI_2048K		MAKE_ROM(ROM_KONAMI, ROM_2048K, 1, 1) // Konami 8KB ROM Mapper (256 segments)
#define TARGET_ROM_KONAMI			TARGET_ROM_KONAMI_128K
// -- Konami ROM with SCC
#define TARGET_ROM_KONAMI_SCC_64K	MAKE_ROM(ROM_KONAMI_SCC, ROM_64K, 1, 1) // Konami 8KB ROM Mapper (8 segments) + SCC sound chip
#define TARGET_ROM_KONAMI_SCC_128K	MAKE_ROM(ROM_KONAMI_SCC, ROM_128K, 1, 1) // Konami 8KB ROM Mapper (16 segments) + SCC sound chip
#define TARGET_ROM_KONAMI_SCC_256K	MAKE_ROM(ROM_KONAMI_SCC, ROM_256K, 1, 1) // Konami 8KB ROM Mapper (32 segments) + SCC sound chip
#define TARGET_ROM_KONAMI_SCC_512K	MAKE_ROM(ROM_KONAMI_SCC, ROM_512K, 1, 1) // Konami 8KB ROM Mapper (64 segments) + SCC sound chip
#define TARGET_ROM_KONAMI_SCC_1024K	MAKE_ROM(ROM_KONAMI_SCC, ROM_1024K, 1, 1) // Konami 8KB ROM Mapper (128 segments) + SCC sound chip
#define TARGET_ROM_KONAMI_SCC_2048K	MAKE_ROM(ROM_KONAMI_SCC, ROM_2048K, 1, 1) // Konami 8KB ROM Mapper (256 segments) + SCC sound chip
#define TARGET_ROM_KONAMI_SCC		TARGET_ROM_KONAMI_SCC_128K

#include "config_target.h"

//-----------------------------------------------------------------------------
// BIOS MODULE
//-----------------------------------------------------------------------------

// RENDER_MODE options
#define RENDER_BIOS			0 // Use BIOS routines
#define RENDER_VDP			1 // Use direct VDP registers

// BIOS_CALL options
#define CALL_DIRECT			0 // Use direct access to Bios routines (Main-ROM must be in Slot 0)
#define CALL_INTERSLOT		1 // Use inter-slot access to Bios routines

//-----------------------------------------------------------------------------
// PRINT MODULE
//-----------------------------------------------------------------------------

// PRINT_WIDTH
#define PRINT_WIDTH_1		1 // 
#define PRINT_WIDTH_6		6 // 
#define PRINT_WIDTH_8		8 // 
#define PRINT_WIDTH_X		0 // Not supported (fall back to PRINT_WIDTH_8)
// PRINT_HEIGHT
#define PRINT_HEIGHT_1		1 // 
#define PRINT_HEIGHT_8		8 // 
#define PRINT_HEIGHT_X		0 // 

//-----------------------------------------------------------------------------
// VDP MODULE
//-----------------------------------------------------------------------------

// VDP_VRAM
#define VDP_VRAM_ADDR_14	0 // Use 14-bits 16K VRAM addressing for MSX 1 (u16)
#define VDP_VRAM_ADDR_17	1 // Use 17-bits 128K VRAM addressing for MSX 2/2+/Turbo-R (u32)

// VDP_UNIT
#define VDP_UNIT_U8			0 // X and Y use 8-bits values
#define VDP_UNIT_X16		1 // X use 16-bits and Y use 8-bits values
#define VDP_UNIT_Y16		2 // X use 8-bits and Y use 16-bits values
#define VDP_UNIT_U16		3 // X and Y use 16-bits values


//-----------------------------------------------------------------------------
// DRAW MODULE
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// MATH MODULE
//-----------------------------------------------------------------------------

// RANDOM_8_METHOD
#define RANDOM_8_NONE		0 // No 8-bits random
#define RANDOM_8_REGISTER	1 // R Register value (7-bits)
#define RANDOM_8_RACC		2 // R Register accumulation (7-bits)
#define RANDOM_8_ION		3 // Ion Random
#define RANDOM_8_MEMORY		4 // Memory Peek from R

// RANDOM_16_METHOD
#define RANDOM_16_NONE		0 // No 16-bits random
#define RANDOM_16_LINEAR	1 // Linear congruential
#define RANDOM_16_XORSHIFT	2 // XOR Shift
#define RANDOM_16_LFSR_LCG	3 // Combined LFSR/LCG

//-----------------------------------------------------------------------------
// PSG
//-----------------------------------------------------------------------------

// PSG_CHIP
#define PSG_INTERNAL		0 // Use internal PSG chip (port A0-A2)
#define PSG_EXTERNAL		1 // Use external PSG chip (port 10-12)
#define PSG_BOTH			2 // Use both internal and external PSG chips

// PSG_ACCESS
#define PSG_DIRECT			0 // Function set directly the PSG registers
#define PSG_INDIRECT		1 // Function set a buffer (Apply() function must be used once a frame)


//-----------------------------------------------------------------------------
// PCM ENCODER
//-----------------------------------------------------------------------------

// PCMENC_FREQ
#define PCMENC_NONE			0 // 0000
#define PCMENC_8K			1 // 0001
#define PCMENC_11K			2 // 0010
#define PCMENC_22K			4 // 0100
#define PCMENC_44K			8 // 1000

//-----------------------------------------------------------------------------
// MISC
//-----------------------------------------------------------------------------

// PROFILE_MODE
#define PROFILE_DISABLE		0 // No profiler
#define PROFILE_OPENMSX_G	1 // Grauw profile script for OpenMSX
#define PROFILE_OPENMSX_S	2 // Salutte profile script for OpenMSX