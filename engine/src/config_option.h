// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄             ▄▄  ▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ▄█▀▄ ██▀▄ ██ ▀ ▄  ▄▀██ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ▀█▄▀ ██ █ ██▀  ██  ▀██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                          ▀▀  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
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
#define MSX_1						0b00000001 // MSX1
#define MSX_2						0b00000010 // MSX2
#define MSX_12						(MSX_1|MSX_2) // MSX1 and 2 (support each)
#define MSX_2K						0b00000100 // Korean MSX2 (SC9 support)
#define MSX_2P						0b00001000 // MSX2+
#define MSX_22P						(MSX_2|MSX_2P) // MSX2 and 2+ (support each)
#define MSX_122P					(MSX_1|MSX_2|MSX_2P) // MSX1, 2 and 2+ (support each)
#define MSX_0						MSX_2P // MSX0 (MSX2+)
#define MSX_TR						0b00010000 // MSX turbo R
#define MSX_3						0b00100000 // MSX3

//-----------------------------------------------------------------------------
// TARGET_TYPE options
#define TYPE_BIN					0 // BASIC binary program
#define TYPE_ROM					1 // ROM program
#define TYPE_DOS					2 // MSX-DOS program

//-----------------------------------------------------------------------------
// TARGET options

// DOS options
#define DOS_0						0 // Disk boot program
#define DOS_1						1 // MSX-DOS 1 program
#define DOS_2						2 // MSX-DOS 2 program
#define DOS_3						3 // NEXTOR program

// ROM_MAPPER options (0-15)
#define ROM_PLAIN					0 // Plain ROM (no mapper)
// 8 KB mappers
#define ROM_MAPPER_8K				2
#define ROM_ASCII8					2 // ASCII-8 mapper
#define ROM_KONAMI					3 // Konami without SCC mapper
#define ROM_KONAMI_SCC				4 // Konami with SCC mapper
#define ROM_NEO8					5 // NEO-8 mapper
#define ROM_YAMANOOTO				6 // Yamanooto mapper
// 16 KB mappers
#define ROM_MAPPER_16K				10
#define ROM_ASCII16					10 // ASCII-16 mmaper
#define ROM_NEO16					11 // NEO-16 mapper
#define ROM_ASCII16X				12 // ASCII16-X mmaper

// ROM_SIZE options (0-15)
#define ROM_8K						0
#define ROM_16K						1
#define ROM_24K						2
#define ROM_32K						3
#define ROM_48K						4
#define ROM_64K						5
#define ROM_128K					6
#define ROM_256K					7
#define ROM_512K					8
#define ROM_1024K					9
#define ROM_2048K					10
#define ROM_4096K					11
#define ROM_8192K					12
#define ROM_16384K					13
#define ROM_32768K					14
#define ROM_65536K					15
#define ROM_1M						ROM_1024K
#define ROM_2M						ROM_2048K
#define ROM_4M						ROM_4096K
#define ROM_8M						ROM_8192K
#define ROM_16M						ROM_16384K
#define ROM_32M						ROM_32768K
#define ROM_64M						ROM_65536K

#define ROM_ISR						(1<<12)
#define ROM_MIRROR					(1<<13)
#define RAM_MAPPER					(1<<13)

// ROM target type define
//	15	14	13	12	11	10	9	8	7	6	5	4	3	2	1	0
//	0	1	MIR	ISR	M3	M2	M1	M0	S3	S2	S1	S0	P1	P0	B1	B0
//			│	│	│	│	│	│	│	│	│	│	│	│	└───┴── ROM boot page (address of 'AB' header)
//			│	│	│	│	│	│	│	│	│	│	└───┴────────── ROM starting page (first address of the ROM ; can be different from booting page)
//			│	│	│	│	│	│	└───┴───┴───┴────────────────── ROM size
//			│	│	└───┴───┴───┴────────────────────────────────── ROM Mapper Type
//			│	└────────────────────────────────────────────────── Use custom Interrupt Service Routine (place custom code in page 0)
//			└────────────────────────────────────────────────────── Mirrored ROM
#define MAKE_ROM(m, s, p, b)		((TYPE_ROM << 14) | ((m) << 8) | ((s) << 4) | ((p) << 2) | (b))

// Basic target type define
#define MAKE_BASIC(x)				((TYPE_BIN << 14) | (x))

// DOS target type define
#define MAKE_DOS(x)					((TYPE_DOS << 14) | (x))

// -- BASIC program
#define TARGET_BIN_DISK		  		MAKE_BASIC(0) // BASIC binary on disk (8000h~)
#define TARGET_BIN_TAPE		  		MAKE_BASIC(1) // BASIC binary on tape (8000h~)
#define TARGET_BIN_USR		  		MAKE_BASIC(2) // BASIC USR driver (C000h~)
// -- DOS program
#define TARGET_DOS0					MAKE_DOS(DOS_0) // Disk boot program (0100h~). Access to BDOS functions through F37Dh
#define TARGET_DOS1					MAKE_DOS(DOS_1) // MSX-DOS 1 program (0100h~). No direct acces to Main-ROM
#define TARGET_DOS2					MAKE_DOS(DOS_2) // MSX-DOS 2 program (0100h~). No direct acces to Main-ROM
#define TARGET_DOS2_MAPPER			MAKE_DOS(DOS_2) + RAM_MAPPER // MSX-DOS 2 launcher to RAM mapper
#define TARGET_DOS3					MAKE_DOS(DOS_3) // NEXTOR program (0100h~). No direct acces to Main-ROM
#define TARGET_NEXTOR				TARGET_DOS3
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
// -- ASCII-8 ROM
#define TARGET_ROM_ASCII8_64K		MAKE_ROM(ROM_ASCII8, ROM_64K,   1, 1) // ASCII 8KB ROM Mapper (8 segments)
#define TARGET_ROM_ASCII8_128K		MAKE_ROM(ROM_ASCII8, ROM_128K,  1, 1) // ASCII 8KB ROM Mapper (16 segments)
#define TARGET_ROM_ASCII8_256K		MAKE_ROM(ROM_ASCII8, ROM_256K,  1, 1) // ASCII 8KB ROM Mapper (32 segments)
#define TARGET_ROM_ASCII8_512K		MAKE_ROM(ROM_ASCII8, ROM_512K,  1, 1) // ASCII 8KB ROM Mapper (64 segments)
#define TARGET_ROM_ASCII8_1024K		MAKE_ROM(ROM_ASCII8, ROM_1024K, 1, 1) // ASCII 8KB ROM Mapper (128 segments)
#define TARGET_ROM_ASCII8_2048K		MAKE_ROM(ROM_ASCII8, ROM_2048K, 1, 1) // ASCII 8KB ROM Mapper (256 segments)
#define TARGET_ROM_ASCII8			TARGET_ROM_ASCII8_128K		
// -- ASCII-16 ROM
#define TARGET_ROM_ASCII16_64K		MAKE_ROM(ROM_ASCII16, ROM_64K,   1, 1) // ASCII 16KB ROM Mapper (4 segments)
#define TARGET_ROM_ASCII16_128K		MAKE_ROM(ROM_ASCII16, ROM_128K,  1, 1) // ASCII 16KB ROM Mapper (8 segments)
#define TARGET_ROM_ASCII16_256K		MAKE_ROM(ROM_ASCII16, ROM_256K,  1, 1) // ASCII 16KB ROM Mapper (16 segments)
#define TARGET_ROM_ASCII16_512K		MAKE_ROM(ROM_ASCII16, ROM_512K,  1, 1) // ASCII 16KB ROM Mapper (32 segments)
#define TARGET_ROM_ASCII16_1024K	MAKE_ROM(ROM_ASCII16, ROM_1024K, 1, 1) // ASCII 16KB ROM Mapper (64 segments)
#define TARGET_ROM_ASCII16_2048K	MAKE_ROM(ROM_ASCII16, ROM_2048K, 1, 1) // ASCII 16KB ROM Mapper (128 segments)
#define TARGET_ROM_ASCII16_4096K	MAKE_ROM(ROM_ASCII16, ROM_4096K, 1, 1) // ASCII 16KB ROM Mapper (256 segments)
#define TARGET_ROM_ASCII16			TARGET_ROM_ASCII16_128K
// -- Konami ROM
#define TARGET_ROM_KONAMI_64K		MAKE_ROM(ROM_KONAMI, ROM_64K,   1, 1) // Konami 8KB ROM Mapper (8 segments)
#define TARGET_ROM_KONAMI_128K		MAKE_ROM(ROM_KONAMI, ROM_128K,  1, 1) // Konami 8KB ROM Mapper (16 segments)
#define TARGET_ROM_KONAMI_256K		MAKE_ROM(ROM_KONAMI, ROM_256K,  1, 1) // Konami 8KB ROM Mapper (32 segments)
#define TARGET_ROM_KONAMI_512K		MAKE_ROM(ROM_KONAMI, ROM_512K,  1, 1) // Konami 8KB ROM Mapper (64 segments)
#define TARGET_ROM_KONAMI_1024K		MAKE_ROM(ROM_KONAMI, ROM_1024K, 1, 1) // Konami 8KB ROM Mapper (128 segments)
#define TARGET_ROM_KONAMI_2048K		MAKE_ROM(ROM_KONAMI, ROM_2048K, 1, 1) // Konami 8KB ROM Mapper (256 segments)
#define TARGET_ROM_KONAMI			TARGET_ROM_KONAMI_128K
// -- Konami ROM with SCC
#define TARGET_ROM_KONAMI_SCC_64K	MAKE_ROM(ROM_KONAMI_SCC, ROM_64K,   1, 1) // Konami 8KB ROM Mapper (8 segments) + SCC sound chip
#define TARGET_ROM_KONAMI_SCC_128K	MAKE_ROM(ROM_KONAMI_SCC, ROM_128K,  1, 1) // Konami 8KB ROM Mapper (16 segments) + SCC sound chip
#define TARGET_ROM_KONAMI_SCC_256K	MAKE_ROM(ROM_KONAMI_SCC, ROM_256K,  1, 1) // Konami 8KB ROM Mapper (32 segments) + SCC sound chip
#define TARGET_ROM_KONAMI_SCC_512K	MAKE_ROM(ROM_KONAMI_SCC, ROM_512K,  1, 1) // Konami 8KB ROM Mapper (64 segments) + SCC sound chip
#define TARGET_ROM_KONAMI_SCC_1024K	MAKE_ROM(ROM_KONAMI_SCC, ROM_1024K, 1, 1) // Konami 8KB ROM Mapper (128 segments) + SCC sound chip
#define TARGET_ROM_KONAMI_SCC_2048K	MAKE_ROM(ROM_KONAMI_SCC, ROM_2048K, 1, 1) // Konami 8KB ROM Mapper (256 segments) + SCC sound chip
#define TARGET_ROM_KONAMI_SCC		TARGET_ROM_KONAMI_SCC_128K
// -- NEO-8 ROM
#define TARGET_ROM_NEO8_128K		MAKE_ROM(ROM_NEO8, ROM_128K, 0, 1) + ROM_ISR // NEO 8KB ROM Mapper (16 segments)
#define TARGET_ROM_NEO8_256K		MAKE_ROM(ROM_NEO8, ROM_256K, 0, 1) + ROM_ISR // NEO 8KB ROM Mapper (32 segments)
#define TARGET_ROM_NEO8_512K		MAKE_ROM(ROM_NEO8, ROM_512K, 0, 1) + ROM_ISR // NEO 8KB ROM Mapper (64 segments)
#define TARGET_ROM_NEO8_1M			MAKE_ROM(ROM_NEO8, ROM_1M,   0, 1) + ROM_ISR // NEO 8KB ROM Mapper (128 segments)
#define TARGET_ROM_NEO8_2M			MAKE_ROM(ROM_NEO8, ROM_2M,   0, 1) + ROM_ISR // NEO 8KB ROM Mapper (256 segments)
#define TARGET_ROM_NEO8_4M			MAKE_ROM(ROM_NEO8, ROM_4M,   0, 1) + ROM_ISR // NEO 8KB ROM Mapper (512 segments)
#define TARGET_ROM_NEO8_8M			MAKE_ROM(ROM_NEO8, ROM_8M,   0, 1) + ROM_ISR // NEO 8KB ROM Mapper (1024 segments)
#define TARGET_ROM_NEO8_16M			MAKE_ROM(ROM_NEO8, ROM_16M,  0, 1) + ROM_ISR // NEO 8KB ROM Mapper (2048 segments)
#define TARGET_ROM_NEO8_32M			MAKE_ROM(ROM_NEO8, ROM_32M,  0, 1) + ROM_ISR // NEO 8KB ROM Mapper (4096 segments)
#define TARGET_ROM_NEO8				TARGET_ROM_NEO8_8M		
// -- NEO-16 ROM
#define TARGET_ROM_NEO16_128K		MAKE_ROM(ROM_NEO16, ROM_128K, 0, 1) + ROM_ISR // NEO 16KB ROM Mapper (16 segments)
#define TARGET_ROM_NEO16_256K		MAKE_ROM(ROM_NEO16, ROM_256K, 0, 1) + ROM_ISR // NEO 16KB ROM Mapper (32 segments)
#define TARGET_ROM_NEO16_512K		MAKE_ROM(ROM_NEO16, ROM_512K, 0, 1) + ROM_ISR // NEO 16KB ROM Mapper (64 segments)
#define TARGET_ROM_NEO16_1M			MAKE_ROM(ROM_NEO16, ROM_1M,   0, 1) + ROM_ISR // NEO 16KB ROM Mapper (64 segments)
#define TARGET_ROM_NEO16_2M			MAKE_ROM(ROM_NEO16, ROM_2M,   0, 1) + ROM_ISR // NEO 16KB ROM Mapper (128 segments)
#define TARGET_ROM_NEO16_4M			MAKE_ROM(ROM_NEO16, ROM_4M,   0, 1) + ROM_ISR // NEO 16KB ROM Mapper (256 segments)
#define TARGET_ROM_NEO16_8M			MAKE_ROM(ROM_NEO16, ROM_8M,   0, 1) + ROM_ISR // NEO 16KB ROM Mapper (512 segments)
#define TARGET_ROM_NEO16_16M		MAKE_ROM(ROM_NEO16, ROM_16M,  0, 1) + ROM_ISR // NEO 16KB ROM Mapper (1024 segments)
#define TARGET_ROM_NEO16_32M		MAKE_ROM(ROM_NEO16, ROM_32M,  0, 1) + ROM_ISR // NEO 16KB ROM Mapper (2048 segments)
#define TARGET_ROM_NEO16_64M		MAKE_ROM(ROM_NEO16, ROM_64M,  0, 1) + ROM_ISR // NEO 16KB ROM Mapper (4096 segments)
#define TARGET_ROM_NEO16			TARGET_ROM_NEO16_8M		
// -- YAMANOOTO ROM
#define TARGET_ROM_YAMANOOTO_2M		MAKE_ROM(ROM_YAMANOOTO, ROM_2M,  1, 1) // YAMANOOTO 8KB ROM Mapper (256 segments)
#define TARGET_ROM_YAMANOOTO_4M		MAKE_ROM(ROM_YAMANOOTO, ROM_4M,  1, 1) // YAMANOOTO 8KB ROM Mapper (512 segments)
#define TARGET_ROM_YAMANOOTO_8M		MAKE_ROM(ROM_YAMANOOTO, ROM_8M,  1, 1) // YAMANOOTO 8KB ROM Mapper (1024 segments)
#define TARGET_ROM_YAMANOOTO_16M	MAKE_ROM(ROM_YAMANOOTO, ROM_16M, 1, 1) // YAMANOOTO 8KB ROM Mapper (2048 segments)
#define TARGET_ROM_YAMANOOTO_32M	MAKE_ROM(ROM_YAMANOOTO, ROM_32M, 1, 1) // YAMANOOTO 8KB ROM Mapper (4096 segments)
#define TARGET_ROM_YAMANOOTO		TARGET_ROM_YAMANOOTO_8M
// -- ASCII16-X ROM
#define TARGET_ROM_ASCII16X_8M		MAKE_ROM(ROM_ASCII16X, ROM_8M,   1, 1) // ASCII16-X 16KB ROM Mapper (512 segments)
#define TARGET_ROM_ASCII16X_16M		MAKE_ROM(ROM_ASCII16X, ROM_16M,  1, 1) // ASCII16-X 16KB ROM Mapper (1024 segments)
#define TARGET_ROM_ASCII16X_32M		MAKE_ROM(ROM_ASCII16X, ROM_32M,  1, 1) // ASCII16-X 16KB ROM Mapper (2048 segments)
#define TARGET_ROM_ASCII16X_64M		MAKE_ROM(ROM_ASCII16X, ROM_64M,  1, 1) // ASCII16-X 16KB ROM Mapper (4096 segments)
#define TARGET_ROM_ASCII16X			TARGET_ROM_ASCII16X_8M

#include "config_target.h"

//-----------------------------------------------------------------------------
// BIOS MODULE
//-----------------------------------------------------------------------------

// RENDER_MODE options
#define RENDER_BIOS					0 // Use BIOS routines
#define RENDER_VDP					1 // Use direct VDP registers

// BIOS_CALL options		
#define CALL_DIRECT					0 // Use direct access to Bios routines (Main-ROM must be in Slot 0)
#define CALL_INTERSLOT				1 // Use inter-slot access to Bios routines

//-----------------------------------------------------------------------------
// PRINT MODULE
//-----------------------------------------------------------------------------

// PRINT_WIDTH
#define PRINT_WIDTH_1				1 // 
#define PRINT_WIDTH_6				6 // 
#define PRINT_WIDTH_8				8 // 
#define PRINT_WIDTH_X				0 // Not supported (fall back to PRINT_WIDTH_8)
// PRINT_HEIGHT
#define PRINT_HEIGHT_1				1 // 
#define PRINT_HEIGHT_8				8 // 
#define PRINT_HEIGHT_X				0 // 

//-----------------------------------------------------------------------------
// VDP MODULE
//-----------------------------------------------------------------------------

// VDP_VRAM options
#define VDP_VRAM_ADDR_14			0 // Use 14-bits 16K VRAM addressing for MSX1 (u16)
#define VDP_VRAM_ADDR_17			1 // Use 17-bits 128K VRAM addressing for MSX2/2+/Turbo-R (u32)

// VDP_UNIT options
#define VDP_UNIT_U8					0 // X and Y use 8-bits values
#define VDP_UNIT_X16				1 // X use 16-bits and Y use 8-bits values
#define VDP_UNIT_Y16				2 // X use 8-bits and Y use 16-bits values
#define VDP_UNIT_U16				3 // X and Y use 16-bits values

// VDP initial value
#define VDP_INIT_OFF				0b00 // Force option to be disable
#define VDP_INIT_ON					0b01 // Force option to be enable
#define VDP_INIT_AUTO				0b10 // Determining the best value for the context
#define VDP_INIT_DEFAULT			0b11 // Keep default value

// VDP_ISR_SAFE_MODE options
#define VDP_ISR_SAFE_NONE			0 // No ISR protection (for program not using VDP interruption)
#define VDP_ISR_SAFE_DEFAULT		1 // Protect only VDP register pair writing (default behavior; ISR can read/write registers but VRAM ones)
#define VDP_ISR_SAFE_ALL			2 // Protect all VDP writing process

//-----------------------------------------------------------------------------
// V9990 MODULE
//-----------------------------------------------------------------------------

// Palette input data format
#define V9_PALETTE_YSGBR_16			0	// 16 bits RGB + Ys [Ys|G|G|G|G|G|R|R] [R|R|R|B|B|B|B|B]
#define V9_PALETTE_GBR_16			1	// 16 bits RGB      [0|G|G|G|G|G|R|R] [R|R|R|B|B|B|B|B]
#define V9_PALETTE_RGB_24			2	// 24 bits RGB      [0|0|0|R|R|R|R|R] [0|0|0|G|G|G|G|G] [0|0|0|B|B|B|B|B]

//-----------------------------------------------------------------------------
// DRAW MODULE
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// MATH MODULE
//-----------------------------------------------------------------------------

// RANDOM_8_METHOD
#define RANDOM_8_NONE				0 // No 8-bits random
#define RANDOM_8_REGISTER			1 // R Register value (7-bits)
#define RANDOM_8_RACC				2 // R Register accumulation (7-bits)
#define RANDOM_8_ION				3 // Ion Random
#define RANDOM_8_MEMORY				4 // Memory Peek from R

// RANDOM_16_METHOD
#define RANDOM_16_NONE				0 // No 16-bits random
#define RANDOM_16_LINEAR			1 // Linear congruential
#define RANDOM_16_XORSHIFT			2 // XOR Shift
#define RANDOM_16_LFSR_LCG			3 // Combined LFSR/LCG

//-----------------------------------------------------------------------------
// AUDIO
//-----------------------------------------------------------------------------

// PSG_CHIP
#define PSG_INTERNAL				0 // Use internal PSG chip (port A0-A2)
#define PSG_EXTERNAL				1 // Use external PSG chip (port 10-12)
#define PSG_BOTH					2 // Use both internal and external PSG chips

// PSG_ACCESS
#define PSG_DIRECT					0 // Function set directly the PSG registers
#define PSG_INDIRECT				1 // Function set a buffer (Apply() function must be call once a frame)

// SCC_SLOT_MODE
#define SCC_SLOT_DIRECT				0 // Program on a SCC cartridge
#define SCC_SLOT_FIXED				1 // Fixed slot-id (non-expanded second cartridge slot)
#define SCC_SLOT_USER				2 // Defined by the user
#define SCC_SLOT_AUTO				3 // First auto-detected cartridge

// PCMENC_FREQ
#define PCMENC_NONE					0 // 0000
#define PCMENC_8K					1 // 0001
#define PCMENC_11K					2 // 0010
#define PCMENC_22K					4 // 0100
#define PCMENC_44K					8 // 1000

// PCMPLAY_FREQ
#define PCMPLAY_8K					1
#define PCMPLAY_11K					2

// WYZ_CHANNELS
#define WYZ_3CH 					0
#define WYZ_6CH 					1

//-----------------------------------------------------------------------------
// GAME PAWN MODULE
//-----------------------------------------------------------------------------

// Border collision options
#define PAWN_BORDER_NONE			0 // 0000
#define PAWN_BORDER_DOWN			1 // 0001
#define PAWN_BORDER_UP				2 // 0010
#define PAWN_BORDER_RIGHT			4 // 0100
#define PAWN_BORDER_LEFT			8 // 1000

// Collision position options
#define PAWN_COL_NONE				0  // 00000000
#define PAWN_COL_0					1  // 00000001
#define PAWN_COL_25					2  // 00000010
#define PAWN_COL_50					4  // 00000100
#define PAWN_COL_75					8  // 00001000
#define PAWN_COL_100				16 // 00010000

#define PAWN_COL_1P_MIDDLE			PAWN_COL_50
#define PAWN_COL_2P_MIDDLE			(PAWN_COL_25|PAWN_COL_75)
#define PAWN_COL_2P_CORNER			(PAWN_COL_0|PAWN_COL_100)

#define PAWN_BOUND_CUSTOM			0x10000 // Use variable bound value for each pawn

// Collision tilemap source
#define PAWN_TILEMAP_SRC_AUTO		0 // Backward compatibility option
#define PAWN_TILEMAP_SRC_RAM		1 // Tilemap located in a buffer in RAM (best for performance)
#define PAWN_TILEMAP_SRC_VRAM		2 // Tilemap located in VRAM (slow but don't need additionnal data)
#define PAWN_TILEMAP_SRC_V9			3 // Tilemap located in V9990's VRAM

// Pawn's sprite mode
#define PAWN_SPT_MODE_AUTO			0 // Backward compatibility option
#define PAWN_SPT_MODE_MSX1			11 // Sprite Mode 1 (MSX1 screens)
#define PAWN_SPT_MODE_MSX2			12 // Sprite Mode 2 (MSX2 screens)
#define PAWN_SPT_MODE_MSX12			13 // Sprite Mode 1 & 2 (MSX1 and MSX2 screens)
#define PAWN_SPT_MODE_BITMAP		50 // V9938/58 software sprite (Todo)
#define PAWN_SPT_MODE_V9_P1			91 // V9990 sprite in P1 mode
#define PAWN_SPT_MODE_V9_P2			92 // V9990 sprite in P2 mode

//-----------------------------------------------------------------------------
// GAME MENU MODULE
//-----------------------------------------------------------------------------

#define MENU_VARIABLE               0x10000
#define MENU_AUTO               	0x10001

//-----------------------------------------------------------------------------
// COMPRESS MODULE
//-----------------------------------------------------------------------------

#define PLETTER_DI_NONE				0 // Don't disable interruption
#define PLETTER_DI_FULL				1 // Disable interruption during the whole function
#define PLETTER_DI_LOOP				2 // Disable interruption during VRAM write loop

#define PLETTER_WRITE_SAFE			0 // Safe VRAM write speed (30 t-states)
#define PLETTER_WRITE_NODISPLAY		1 // Safe VRAM write speed when screen display disable (22 t-states)
#define PLETTER_WRITE_MINIMAL		2 // Minimal wait beetween write (17 t-states)
#define PLETTER_WRITE_QUICK			3 // No wait beetween write (12 t-states)
#define PLETTER_WRITE_AUTO			4 // Determine the worst case according to selected screen mode (12~30 t-states)

#define BITBUSTER_WRITE_SAFE		0 // Safe VRAM write speed (include nop between write)
#define BITBUSTER_WRITE_QUICK		1 // No wait beetween write

#define ZX0_MODE_STANDARD			0 // Standard routine: 68 bytes only
#define ZX0_MODE_TURBO				1 // Turbo routine: 126 bytes, about 21% faster
#define ZX0_MODE_FAST				2 // Fast routine: 187 bytes, about 25% faster
#define ZX0_MODE_MEGA				3 // Mega routine: 673 bytes, about 28% faster

#define LZ48_MODE_STANDARD			0 // Standard routine
#define LZ48_MODE_SPEED				1 // Version optimized for speed
#define LZ48_MODE_SIZE				2 // Version optimized for size

//-----------------------------------------------------------------------------
// PAC MODULE
//-----------------------------------------------------------------------------

#define PAC_ACCESS_DIRECT			0 // Direct access to SRAM (must be selected in page 1)
#define PAC_ACCESS_BIOS				1 // Access through BIOS routines
#define PAC_ACCESS_SWITCH_BIOS		2 // Access through BIOS routines with BIOS switched in
#define PAC_ACCESS_SYSTEM			3 // Access through MSXgl routine (no need BIOS)

//-----------------------------------------------------------------------------
// QR CODE MODULE
//-----------------------------------------------------------------------------

#define QRCODE_PARAM_CUSTOM			0x10000 // Give access to function to customize the parameter

// The error correction level in a QR Code symbol.
#define QRCODE_ECC_LOW				0 // The QR Code can tolerate about  7% erroneous codewords
#define QRCODE_ECC_MEDIUM			1 // The QR Code can tolerate about 15% erroneous codewords
#define QRCODE_ECC_QUARTILE			2 // The QR Code can tolerate about 25% erroneous codewords
#define QRCODE_ECC_HIGH				3 // The QR Code can tolerate about 30% erroneous codewords

// The mask pattern used in a QR Code symbol.
#define QRCODE_MASK_0				0 // (i + j) % 2 = 0
#define QRCODE_MASK_1				1 // i % 2 = 0
#define QRCODE_MASK_2				2 // j % 3 = 0
#define QRCODE_MASK_3				3 // (i + j) % 3 = 0
#define QRCODE_MASK_4				4 // (i / 2 + j / 3) % 2 = 0
#define QRCODE_MASK_5				5 // (i * j) % 2 + (i * j) % 3 = 0
#define QRCODE_MASK_6				6 // ((i * j) % 3 + i * j) % 2 = 0
#define QRCODE_MASK_7				7 // ((i * j) % 3 + i + j) % 2 = 0

//-----------------------------------------------------------------------------
// MISC
//-----------------------------------------------------------------------------

// DEBUG_TOOL
#define DEBUG_NONE					0x00 // No debug tool
#define DEBUG_DEVICE				0x10 // Debug features for debugdevice device (https://aoineko.org/msxgl/index.php?title=Debug_device)
#define DEBUG_EMULICIOUS			0x20 // Debug features for Emulicious emulator
#define DEBUG_OPENMSX				0x30 // Debug features for openMSX using 'debugdevice' extension
#define DEBUG_OPENMSX_P				0x31 // Debug features for openMSX using PVM script (tools/script/openMSX/debugger_pvm.tcl)

#define DEBUG_DISABLE				DEBUG_NONE
#define DEBUG_EMULATOR(tool)		(tool & 0xF0)

// PROFILE_TOOL
#define PROFILE_NONE				0x00 // No profile tool
#define PROFILE_OPENMSX_G			0x38 // Profiler features for openMSX using Grauw script (tools/script/openMSX/profiler_grauw.tcl)
#define PROFILE_OPENMSX_S			0x39 // Profiler features for openMSX using Salutte script (tools/script/openMSX/profiler_salutte.tcl)

#define PROFILE_DISABLE				PROFILE_NONE
#define PROFILE_EMULATOR(tool)		(tool & 0xF0)