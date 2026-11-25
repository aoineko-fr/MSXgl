// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

// ROM mapper segment size
#if (ROM_MAPPER >= ROM_MAPPER_16K)
	#define ROM_SEGMENT_SIZE		16
#else 
	#define ROM_SEGMENT_SIZE		8
#endif

// ROM mapper segments count
#define ROM_SEGMENTS				(ROM_SIZE/ROM_SEGMENT_SIZE)

// Check if the current target is a mapped-ROM
#define IS_ROM_MAPPED()				((TARGET_TYPE == TYPE_ROM) && (ROM_MAPPER > ROM_PLAIN))

// Check if the current target is a ROM using the given mapper
#define IS_ROM_MAPPER(x)			((TARGET_TYPE == TYPE_ROM) && (ROM_MAPPER == x))


//.............................................................................
//  ▄▄▄  ▄▄▄▄  ▄▄  
//  ██▀█ ██▄  ██ █ 
//  ██ █ ██▄▄ ▀█▄▀ 
//.............................................................................
// https://aoineko.org/msxgl/index.php?title=NEO_mapper
#if (IS_ROM_MAPPER(ROM_NEO8) || IS_ROM_MAPPER(ROM_NEO16))

	#if IS_ROM_MAPPER(ROM_NEO8)
	#define MAPPER_BANKS			6
	#define ADDR_BANK_0				0x5000 // 0000h - 1FFFh
	#define ADDR_BANK_1				0x5800 // 2000h - 3FFFh
	#define ADDR_BANK_2				0x6000 // 4000h - 5FFFh
	#define ADDR_BANK_3				0x6800 // 6000h - 7FFFh
	#define ADDR_BANK_4				0x7000 // 8000h - 9FFFh
	#define ADDR_BANK_5				0x7800 // A000h - BFFFh
	#else // IS_ROM_MAPPER(ROM_NEO16)
	#define MAPPER_BANKS			3
	#define ADDR_BANK_0				0x5000 // 0000h - 3FFFh
	#define ADDR_BANK_1				0x6000 // 4000h - 7FFFh
	#define ADDR_BANK_2				0x7000 // 8000h - BFFFh
	#endif

	extern u16 g_Bank0Segment[MAPPER_BANKS];

	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT(u8 b, u16 s)
	{ 
		g_Bank0Segment[b] = s;
		if (b == 0)			Poke16(ADDR_BANK_0, s);
		else if (b == 1)	Poke16(ADDR_BANK_1, s);
		else if (b == 2)	Poke16(ADDR_BANK_2, s);
	#if (ROM_MAPPER == ROM_NEO8)
		else if (b == 3)	Poke16(ADDR_BANK_3, s);
		else if (b == 4)	Poke16(ADDR_BANK_4, s);
		else if (b == 5)	Poke16(ADDR_BANK_5, s);
	#endif
	}
	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT_LOW(u8 b, u8 s)
	{
		Poke((u16)&g_Bank0Segment[b] + 0, s);
		if (b == 0)			Poke(ADDR_BANK_0 + 0, s);
		else if (b == 1)	Poke(ADDR_BANK_1 + 0, s);
		else if (b == 2)	Poke(ADDR_BANK_2 + 0, s);
	#if (ROM_MAPPER == ROM_NEO8)
		else if (b == 3)	Poke(ADDR_BANK_3 + 0, s);
		else if (b == 4)	Poke(ADDR_BANK_4 + 0, s);
		else if (b == 5)	Poke(ADDR_BANK_5 + 0, s);
	#endif
	}
	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT_HIGH(u8 b, u8 s)
	{
		Poke((u16)&g_Bank0Segment[b] + 1, s);
		if (b == 0)			Poke(ADDR_BANK_0 + 1, s);
		else if (b == 1)	Poke(ADDR_BANK_1 + 1, s);
		else if (b == 2)	Poke(ADDR_BANK_2 + 1, s);
	#if (ROM_MAPPER == ROM_NEO8)
		else if (b == 3)	Poke(ADDR_BANK_3 + 1, s);
		else if (b == 4)	Poke(ADDR_BANK_4 + 1, s);
		else if (b == 5)	Poke(ADDR_BANK_5 + 1, s);
	#endif
	}

	// Get the current segment of the given bank
	inline u16 GET_BANK_SEGMENT(u8 b) { return g_Bank0Segment[b]; }
	// Get the current segment of the given bank
	inline u8 GET_BANK_SEGMENT_LOW(u8 b) { return (u8)(g_Bank0Segment[b] & 0xFF); }
	// Get the current segment of the given bank
	inline u8 GET_BANK_SEGMENT_HIGH(u8 b) { return (u8)(g_Bank0Segment[b] >> 8); }

//.............................................................................
//  ▄▄▄                 ▄▄  
//  ██▄▀ ▄█▀▄ ██▀▄ ▄█▀▄ ██  ▄█▀▄ ██▀▄
//  ██   ▀█▄▀ ██▀  ▀█▄▀ ▀█▄ ▀█▄▀ ██ █
//............▀▀...............................................................
// https://github.com/popolonfr/Flash-ROM-SCC-Cartridge
// Note: 16-bit mapper mode (Game and Developer editions). For ROM up to 2 MB, use ROM_KONAMI_SCC.
#elif IS_ROM_MAPPER(ROM_POPOLON)

	#define MAPPER_BANKS			4
	#define ADDR_BANK_0				0x5000 // 4000h - 5FFFh
	#define ADDR_BANK_1				0x7000 // 6000h - 7FFFh
	#define ADDR_BANK_2				0x9000 // 8000h - 9FFFh
	#define ADDR_BANK_3				0xB000 // A000h - BFFFh

	// Offset register (Multi-ROM and Developer editions)
	#define POPOLON_OFFSET			0x3800
	#define POPOLON_OFFSET_LOW		0x3800
	//	7	6	5	4	3	2	1	0
	//	S7	S6	S5	S4	S3	S2	S1	S0
	//	└───┴───┴───┴───┴───┴───┴───┴── 8 bits of the low part of the Offset (1 step = 1 segment)
	#define POPOLON_OFFSET_HIGH		0x3801
	//	7	6	5	4	3	2	1	0
	//	x	x	S13	S12	S11	S10	S9	S8
	//			└───┴───┴───┴───┴───┴── 6 bits of the high part of the Offset (1 step = 256 segments)

	// Offset register (Basic edition)
	#define POPOLON_OFFSET_BASIC	0x3FFF

	// Operating mode register (Game and Developer editions)
	#define POPOLON_CONFIG			0x3804
	//	7	6	5	4	3	2	1	0
	//	x	x	x	x	x	x	M	R
	//  						│	└── Reserved
	//  						└────── Mapper mode (0: 8-bit, 1: 16-bit)
	#define POPOLON_CONFIG_M16		0b00000010

	extern u16 g_Bank0Segment[MAPPER_BANKS];

	void BIOS_InterSlotWrite(u8 slot, u16 addr, u8 value);

	// Set the mapper offset added to all banks segment (Multi-ROM and Developer editions; needs BIOS module)
	inline void POPOLON_SET_OFFSET(u16 offset) { BIOS_InterSlotWrite(g_ROMSlotID, POPOLON_OFFSET_LOW, offset & 0xFF); BIOS_InterSlotWrite(g_ROMSlotID, POPOLON_OFFSET_HIGH, offset >> 8); }

	// Set the mapper offset added to all banks segment (Basic edition; needs BIOS module)
	inline void POPOLON_SET_OFFSET_BASIC(u8 offset) { BIOS_InterSlotWrite(g_ROMSlotID, POPOLON_OFFSET_BASIC, offset); }

	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT(u8 b, u16 s)
	{ 
		g_Bank0Segment[b] = s;
		if (b == 0)			Poke16(ADDR_BANK_0, s);
		else if (b == 1)	Poke16(ADDR_BANK_1, s);
		else if (b == 2)	Poke16(ADDR_BANK_2, s);
		else if (b == 3)	Poke16(ADDR_BANK_3, s);
	}
	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT_LOW(u8 b, u8 s)
	{
		Poke((u16)&g_Bank0Segment[b] + 0, s);
		if (b == 0)			Poke(ADDR_BANK_0 + 0, s);
		else if (b == 1)	Poke(ADDR_BANK_1 + 0, s);
		else if (b == 2)	Poke(ADDR_BANK_2 + 0, s);
		else if (b == 3)	Poke(ADDR_BANK_3 + 0, s);
	}
	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT_HIGH(u8 b, u8 s)
	{
		Poke((u16)&g_Bank0Segment[b] + 1, s);
		if (b == 0)			Poke(ADDR_BANK_0 + 1, s);
		else if (b == 1)	Poke(ADDR_BANK_1 + 1, s);
		else if (b == 2)	Poke(ADDR_BANK_2 + 1, s);
		else if (b == 3)	Poke(ADDR_BANK_3 + 1, s);
	}

	// Get the current segment of the given bank
	inline u16 GET_BANK_SEGMENT(u8 b) { return g_Bank0Segment[b]; }
	// Get the current segment of the given bank
	inline u8 GET_BANK_SEGMENT_LOW(u8 b) { return (u8)(g_Bank0Segment[b] & 0xFF); }
	// Get the current segment of the given bank
	inline u8 GET_BANK_SEGMENT_HIGH(u8 b) { return (u8)(g_Bank0Segment[b] >> 8); }
	
//.............................................................................
//  ▄▄ ▄                               ▄▄       
//  ▀█▄▀ ▄▀██ ▄█▄█ ▄▀██ ██▀▄ ▄█▀▄ ▄█▀▄ ██▀ ▄█▀▄ 
//   ██  ▀▄██ ██ █ ▀▄██ ██ █ ▀█▄▀ ▀█▄▀ ▀█▄ ▀█▄▀ 
//.............................................................................
// https://docs.google.com/document/d/1VOiwKtc_0Lrv9cU2BOcAge0uUyHKitxG3ccWc_uWTes/edit
// https://genami.shop/blogs/news/programming-the-yamanooto
#elif IS_ROM_MAPPER(ROM_YAMANOOTO)

	#define MAPPER_BANKS			4
	#define ADDR_BANK_0				0x5000 // 5000h - 57FFh
	#define ADDR_BANK_1				0x7000 // 7000h - 77FFh
	#define ADDR_BANK_2				0x9000 // 9000h - 97FFh
	#define ADDR_BANK_3				0xB000 // B000h - B7FFh

	// Features enable register
	#define YAMANOOTO_ENAR			0x7FFF
	//	7	6	5	4	3	2	1	0
	//	x	x	x	WE	x	R	R	RE
	//  			│				└── REGEN: Registers enable. Setting this bit allows writing to all other registers and all register readability.
	//				└────────────────── WREN: Write enable. Set to 1 to enable writes to flash rom
	#define YAMANOOTO_ENAR_REGEN	0b00000001
	#define YAMANOOTO_ENAR_WREN		0b00010000

	// Mapper offset register (segment = OFFR * 4 + bank value; applied when a bank is written)
	#define YAMANOOTO_OFFR			0x7FFE

	// Configuration and control register
	#define YAMANOOTO_CFGR			0x7FFD
	//	7	6	5	4	3	2	1	0	
	//	x	x	SO2	SO1	K44	RD	PE	DM 	
	//  		│	│	│	│	│	└── MDIS: This bit disables mapping so you avoid mapper changes with small (up to 32 kbyte) roms that poke the switching area. Usually this is a problem only in K4 mode. Remember to reset this bit to make changes again.
	//  		│	│	│	│	└────── ECHO: This bit allows the built-in PSG to respond to the port number of the internal PSG of the MSX, causing music intended for the internal PSG to be played too in the Yamanooto and be heard through the stereo output of the cartridge. This is set only during boot when you press the HOME key.
	//  		│	│	│	└────────── ROMDIS: Setting this bit disables access to flash rom. This is automatically set during boot when the DEL key is pressed. You need to clear this bit in software to be able read/write the flash rom.
	//  		│	│	└────────────── K4: Changes mapper configuration from Konami5 (SCC) to Konami4 for compatibility with game compilations including non-SCC games.
	//  		└───┴────────────────── SUBOFF: Sub-offset. This provides 2 additional lower bits for offset register, to provide 8 kbyte granularity for offset.
	#define YAMANOOTO_CFGR_MDIS		0b00000001
	#define YAMANOOTO_CFGR_ECHO		0b00000010
	#define YAMANOOTO_CFGR_ROMDIS	0b00000100
	#define YAMANOOTO_CFGR_K4		0b00001000
	#define YAMANOOTO_CFGR_SUBOFF	0b00110000

	// Segment value backup
	extern u16 g_Bank0Segment[MAPPER_BANKS];

	// Enable or disable Yamanooto extended features
	inline void YAMANOOTO_ENABLE(bool enable)
	{
		Poke(YAMANOOTO_ENAR, (enable) ? YAMANOOTO_ENAR_REGEN : 0);
	}

	// Set the ENAR register of the Yamanooto mapper
	inline void YAMANOOTO_SET_ENAR(u8 value)
	{
		Poke(YAMANOOTO_ENAR, value);
	}

	// Set the OFFR register of the Yamanooto mapper
	inline void YAMANOOTO_SET_OFFR(u8 value)
	{
		Poke(YAMANOOTO_OFFR, value);
	}

	// Set the CFGR register of the Yamanooto mapper
	inline void YAMANOOTO_SET_CFGR(u8 value)
	{
		Poke(YAMANOOTO_CFGR, value);
	}

	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT(u8 b, u16 s)
	{ 
		g_Bank0Segment[b] = s;
		YAMANOOTO_SET_OFFR((s >> 2) & 0xC0);
		if (b == 0)			Poke(ADDR_BANK_0, s & 0xFF);
		else if (b == 1)	Poke(ADDR_BANK_1, s & 0xFF);
		else if (b == 2)	Poke(ADDR_BANK_2, s & 0xFF);
		else if (b == 3)	Poke(ADDR_BANK_3, s & 0xFF);
	}
	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT_LOW(u8 b, u8 s) { SET_BANK_SEGMENT(b, (g_Bank0Segment[b] & 0xFF00) | s); }
	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT_HIGH(u8 b, u8 s) { SET_BANK_SEGMENT(b, ((u16)s << 8) | (g_Bank0Segment[b] & 0x00FF)); }

	// Get the current segment of the given bank
	inline u16 GET_BANK_SEGMENT(u8 b) { return g_Bank0Segment[b]; }
	// Get the current segment of the given bank
	inline u8 GET_BANK_SEGMENT_LOW(u8 b) { return (u8)(g_Bank0Segment[b] & 0xFF); }
	// Get the current segment of the given bank
	inline u8 GET_BANK_SEGMENT_HIGH(u8 b) { return (u8)(g_Bank0Segment[b] >> 8); }
	
//.............................................................................
//   ▄▄   ▄▄▄  ▄▄  ▄▄▄▄ ▄▄▄▄  ▄▄   ▄▄        ▄▄ ▄
//  ██▄█ ▀█▄  ██ ▀  ██   ██  ▀██  ██▄▄  ▄▄▄  ▀█▄▀
//  ██ █ ▄▄█▀ ▀█▄▀ ▄██▄ ▄██▄ ▄██▄ ▀█▄▀       ██ █
//.............................................................................
// https://www.grauw.nl/projects/ascii-x/ascii16-x/
// https://www.grauw.nl/projects/ascii-x/
#elif IS_ROM_MAPPER(ROM_ASCII16X)

	#define MAPPER_BANKS			2
	#define ADDR_BANK_0				0x6000 // 4000h - 7FFFh
	#define ADDR_BANK_1				0x7000 // 8000h - BFFFh (A8-A11 give the segment high bits, so 77FFh can't be used)

	// Segment value backup
	extern u16 g_Bank0Segment[MAPPER_BANKS];

	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT(u8 b, u16 s)
	{
		g_Bank0Segment[b] = s;
		if (b == 0)			Poke(ADDR_BANK_0 | (s & 0x0F00), s & 0xFF);
		else if (b == 1)	Poke(ADDR_BANK_1 | (s & 0x0F00), s & 0xFF);
	}
	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT_LOW(u8 b, u8 s) { SET_BANK_SEGMENT(b, (g_Bank0Segment[b] & 0xFF00) | s); }
	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT_HIGH(u8 b, u8 s) { SET_BANK_SEGMENT(b, ((u16)s << 8) | (g_Bank0Segment[b] & 0x00FF)); }

	// Get the current segment of the given bank
	inline u16 GET_BANK_SEGMENT(u8 b) { return g_Bank0Segment[b]; }
	// Get the current segment of the given bank
	inline u8 GET_BANK_SEGMENT_LOW(u8 b) { return (u8)(g_Bank0Segment[b] & 0xFF); }
	// Get the current segment of the given bank
	inline u8 GET_BANK_SEGMENT_HIGH(u8 b) { return (u8)(g_Bank0Segment[b] >> 8); }

//.............................................................................
//   ▄▄  ▄▄                 ▄      
//  ██ ▀ ██  ▄▀██ ▄██▀ ▄██▀ ▄  ▄█▀▀
//  ▀█▄▀ ▀█▄ ▀▄██ ▄██▀ ▄██▀ ██ ▀█▄▄
//.............................................................................
// https://www.msx.org/wiki/MegaROM_Mappers
// http://bifi.msxnet.org/msxnet/tech/megaroms
#elif IS_ROM_MAPPED() // ROM_ASCII8, ROM_ASCII16, ROM_KONAMI, ROM_KONAMI_SCC

	#if IS_ROM_MAPPER(ROM_ASCII8)
	#define MAPPER_BANKS			4
	#define ADDR_BANK_0				0x6000 // 4000h - 5FFFh
	#define ADDR_BANK_1				0x6800 // 6000h - 7FFFh
	#define ADDR_BANK_2				0x7000 // 8000h - 9FFFh
	#define ADDR_BANK_3				0x7800 // A000h - BFFFh
	#elif IS_ROM_MAPPER(ROM_ASCII16)
	#define MAPPER_BANKS			2
	#define ADDR_BANK_0				0x6000 // 4000h - 7FFFh
	#define ADDR_BANK_1				0x77FF // 8000h - BFFFh (or 0x7000 ?)
	#elif IS_ROM_MAPPER(ROM_KONAMI)
	#define MAPPER_BANKS			4
	#define ADDR_BANK_0		        0      // 4000h - 5FFFh
	#define ADDR_BANK_1				0x6000 // 6000h - 7FFFh
	#define ADDR_BANK_2				0x8000 // 8000h - 9FFFh
	#define ADDR_BANK_3				0xA000 // A000h - BFFFh
	#elif IS_ROM_MAPPER(ROM_KONAMI_SCC)
	#define MAPPER_BANKS			4
	#define ADDR_BANK_0				0x5000 // 4000h - 5FFFh
	#define ADDR_BANK_1				0x7000 // 6000h - 7FFFh
	#define ADDR_BANK_2				0x9000 // 8000h - 9FFFh
	#define ADDR_BANK_3				0xB000 // A000h - BFFFh
	#else
	#error Unsupported mapper type!
	#endif

	// Segment value backup
	extern u8 g_Bank0Segment[MAPPER_BANKS];

	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT(u8 b, u8 s)
	{ 
		g_Bank0Segment[b] = s;
		if (b == 0)		Poke(ADDR_BANK_0, s);
		else if (b == 1)	Poke(ADDR_BANK_1, s);
	#if (MAPPER_BANKS > 2)
		else if (b == 2)	Poke(ADDR_BANK_2, s);
		else if (b == 3)	Poke(ADDR_BANK_3, s);
	#endif
	}
	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT_LOW(u8 b, u8 s) { SET_BANK_SEGMENT(b, s); }
	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT_HIGH(u8 b, u8 s) { b; s; }

	// Get the current segment of the given bank
	inline u8 GET_BANK_SEGMENT(u8 b) { return g_Bank0Segment[b]; }
	// Get the current segment of the given bank
	inline u8 GET_BANK_SEGMENT_LOW(u8 b) { return GET_BANK_SEGMENT(b); }
	// Get the current segment of the given bank
	inline u8 GET_BANK_SEGMENT_HIGH(u8 b) { b; return 0; }

//.............................................................................
//  ▄▄▄   ▄▄   ▄▄▄
//  ██ █ ██ █ ▀█▄ 
//  ██▄▀ ▀█▄▀ ▄▄█▀
//.............................................................................
#elif (TARGET == TARGET_DOS2_MAPPER)

	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT(u8 b, u8 s) { DOSMapper_SetPage(b + 1, s); }
	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT_LOW(u8 b, u8 s) { SET_BANK_SEGMENT(b, s); }
	// Set the current segment of the given bank
	inline void SET_BANK_SEGMENT_HIGH(u8 b, u8 s) { b; s; }

	// Get the current segment of the given bank
	inline u8 GET_BANK_SEGMENT(u8 b) { return DOSMapper_GetPage(b + 1); }
	// Get the current segment of the given bank
	inline u8 GET_BANK_SEGMENT_LOW(u8 b) { return GET_BANK_SEGMENT(b); }
	// Get the current segment of the given bank
	inline u8 GET_BANK_SEGMENT_HIGH(u8 b) { b; return 0; }

//.............................................................................
//  ▄▄▄  
//  ██ █ ██ █ ▄█▄█ ▄█▄█ █ ██
//  ██▄▀ ▀█▄█ ██ █ ██ █  ▀██
//.......................▀▀....................................................
#else

	// Function: SET_BANK_SEGMENT
	// Set the current segment of the given bank
	//
	// Parameters:
	//   b - Bank number to set (0-5, depending of the mapper)
	//   s - Segment to select in this bank (8 or 16-bit depending on the mapper)
	inline void SET_BANK_SEGMENT(u8 b, u8 s) { b; s; }
	
	// Function: SET_BANK_SEGMENT_LOW
	// Set the low byte of the current segment of the given bank (for 16-bit mappers)
	//
	// Parameters:
	//   b - Bank number to set (0-5, depending of the mapper)
	//   s - Low byte of the segment to select in this bank
	inline void SET_BANK_SEGMENT_LOW(u8 b, u8 s) { b; s; }

	// Function: SET_BANK_SEGMENT_HIGH
	// Set the high byte of the current segment of the given bank (for 16-bit mappers)
	//
	// Parameters:
	//   b - Bank number to set (0-5, depending of the mapper)
	//   s - High byte of the segment to select in this bank
	inline void SET_BANK_SEGMENT_HIGH(u8 b, u8 s) { b; s; }

	// Function: GET_BANK_SEGMENT
	// Get the current segment of the given bank
	//
	// Parameters:
	//   b - Bank number to get (0-5, depending of the mapper)
	//
	// Return:
	//   Segment selected in this bank (8 or 16-bit depending on the mapper)
	inline u8 GET_BANK_SEGMENT(u8 b) { b; return 0; }

	// Function: GET_BANK_SEGMENT_LOW
	// Get the low byte of the current segment of the given bank (for 16-bit mappers)
	//
	// Parameters:
	//   b - Bank number to get (0-5, depending of the mapper)
	//
	// Return:
	//   Low byte of the segment selected in this bank
	inline u8 GET_BANK_SEGMENT_LOW(u8 b) { b; return 0; }

	// Function: GET_BANK_SEGMENT_HIGH
	// Get the high byte of the current segment of the given bank (for 16-bit mappers)
	//
	// Parameters:
	//   b - Bank number to get (0-5, depending of the mapper)
	//
	// Return:
	//   High byte of the segment selected in this bank
	inline u8 GET_BANK_SEGMENT_HIGH(u8 b) { b; return 0; }

#endif

