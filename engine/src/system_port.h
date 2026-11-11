// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄           ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  █ ██  ██▀ ██▀ ▄███ ▄█▄█
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀  ▀██ ▄██  ▀█▄ ▀█▄▄ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘        ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// I/O Ports
//
// References:
// - http://map.grauw.nl/resources/msx_io_ports.php
// - https://www.msx.org/wiki/I/O_Ports_List
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
//
// PPI Ports
//
//=============================================================================

//-----------------------------------------------------------------------------
// 0xA8								Slot status register (Read/Write)
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//  │	│	│	│	│	│	└───┴── Number of slot to select on page 0 (0000h~3FFFh)
//	│	│	│	│	└───┴────────── Number of slot to select on page 1 (4000h~7FFFh)
//	│	│	└───┴────────────────── Number of slot to select on page 2 (8000h~BFFFh)
//	└───┴────────────────────────── Number of slot to select on page 3 (C000h~FFFFh)
#define	P_PPI_A						0xA8
#define P_PSL_STAT					P_PPI_A
__sfr __at(P_PPI_A)					io_PPI_Slot; // Port to access the primary Slot selection register. (PPI's port A is used)

//-----------------------------------------------------------------------------
// 0xA9								keyboard status register (Read/Write)
//-----------------------------------------------------------------------------
#define P_PPI_B						0xA9
#define P_KBD_STAT					P_PPI_B
__sfr __at(P_PPI_B)					io_PPI_KBRead; // Port to read the keyboard matrix row specified via the port AAh. (PPI's port B is used)

//-----------------------------------------------------------------------------
// 0xAA								General IO register (Read/Write)
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//  │	│	│	│	└───┴───┴───┴── Row number of specified keyboard matrix to read via port B
//	│	│	│	└────────────────── Data recorder motor (reset to turn on)
//	│	│	└────────────────────── Set to write on tape
//	│	└────────────────────────── Keyboard LED CAPS (reset to turn on)
//	└────────────────────────────── 1, then 0 shortly thereafter to make a clicking sound (used for the keyboard).
#define P_PPI_C						0xAA
#define P_GIO_REGS					P_PPI_C
__sfr __at(P_PPI_C)					io_PPI_KBAccess; // Port to access the register that control the keyboard CAP LED, two signals to data recorder and a matrix row (use the port C of PPI).

//-----------------------------------------------------------------------------
// 0xAB								Control register (Write only)
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//  │	│	│	│	│	│	│	└── Bit status to change
//	│	│	│	│	└───┴───┴────── Number of the bit to change at port C of the PPI
//	│	└───┴───┴────────────────── Unused
//	└────────────────────────────── Must be always reset on MSX.
#define P_PPI_MODE					0xAB
#define P_PPI_REGS					P_PPI_MODE
__sfr __at(P_PPI_MODE)				io_PPI_Control; // Port to access the ports control register. (Write only)


//=============================================================================
//
// VDP Ports
//
//=============================================================================

//-----------------------------------------------------------------------------
// 0x98								VRAM data port (Read/Write)
//-----------------------------------------------------------------------------
#define P_VDP_0						0x98			// Primary MSX port for VDP port #0
#define P_VDP_DATA					P_VDP_0			// VRAM data port (read/write)
__sfr __at(P_VDP_DATA)				io_VDP_Data; // VRAM data port (read/write)

//-----------------------------------------------------------------------------
// 0x99								General IO port (Read/Write)
//-----------------------------------------------------------------------------
#define P_VDP_1						0x99			// Primary MSX port for VDP port #1
#define P_VDP_REG					P_VDP_1			// Register setup port (write) (bit 7=1 in second write)
__sfr __at(P_VDP_REG)				io_VDP_Reg;	// Register setup port (write) (bit 7=1 in second write)
#define P_VDP_ADDR					P_VDP_1			// VRAM address port (write) (bit 7=0 in second write, bit 6: read/write access (0=read, 1=write))
__sfr __at(P_VDP_ADDR)				io_VDP_Addr;	// VRAM address port (write) (bit 7=0 in second write, bit 6: read/write access (0=read, 1=write))
#define P_VDP_STAT					P_VDP_1			// Status register port (read)
__sfr __at(P_VDP_STAT)				io_VDP_Stat;	// Status register port (read)

#if (MSX_VERSION >= MSX_2)

//-----------------------------------------------------------------------------
// 0x9A								Palette access port (Write only)
//-----------------------------------------------------------------------------
#define P_VDP_2						0x9A			// Primary MSX port for VDP port #2 (only v9938/v9958)
#define P_VDP_PAL					P_VDP_2			// Palette access port (write)
__sfr __at(P_VDP_PAL)				io_VDP_Pal;	// Palette access port (write)

//-----------------------------------------------------------------------------
// 0x9B								Indirect register access port (Write only)
//-----------------------------------------------------------------------------
#define P_VDP_3						0x9B			// Primary MSX port for VDP port #3 (only v9938/v9958)
#define P_VDP_IREG					P_VDP_3			// Indirect register access port (write)
__sfr __at(P_VDP_IREG)				io_VDP_IReg;	// Indirect register access port (write)

#endif // (MSX_VERSION >= MSX_2)


//=============================================================================
//
// PSG Ports
//
//=============================================================================

// Internal PSG I/O ports
#define P_PSG_REGS					0xA0   // PSG register write port
__sfr __at(P_PSG_REGS)				io_PSG_Reg;
#define P_PSG_DATA					0xA1   // PSG value write port
__sfr __at(P_PSG_DATA)				io_PSG_Data;
#define P_PSG_STAT					0xA2   // PSG value read port
__sfr __at(P_PSG_STAT)				io_PSG_Stat;

// External PSG I/O ports
#define P_PSG_EXT_REGS				0x10   // PSG register write port
__sfr __at(P_PSG_EXT_REGS)			io_PSG_Ext_Reg;
#define P_PSG_EXT_DATA				0x11   // PSG value write port
__sfr __at(P_PSG_EXT_DATA)			io_PSG_Ext_Data;
#define P_PSG_EXT_STAT				0x12   // PSG value read port
__sfr __at(P_PSG_EXT_STAT)			io_PSG_Ext_Stat;


//=============================================================================
//
// Printer Ports
//
//=============================================================================

//-----------------------------------------------------------------------------
// 0x90								Control port (Read/Write)
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//  						│	└── Write: Strobe
//  						└────── Read: Busy (0=ready, 1=busy)
#define P_LPT_CONTROL				0x90 // Control port
__sfr __at(P_LPT_CONTROL)			io_Printer_Control;

//-----------------------------------------------------------------------------
// 0x91								Data port (Write)
//-----------------------------------------------------------------------------
#define P_LPT_DATA					0x91 // Data port
__sfr __at(P_LPT_DATA)				io_Printer_Data;


//=============================================================================
//
// RTC Ports
//
//=============================================================================

#define P_RTC_ADDR					0xB4   // RTC address
__sfr __at(P_RTC_ADDR)				io_RTC_Reg;
#define P_RTC_DATA					0xB5   // RTC data
__sfr __at(P_RTC_DATA)				io_RTC_Data;


//=============================================================================
//
// System Timer Ports
//
//=============================================================================
		
// Kanji Level 1 I/O ports
#define P_TIMER_LOW					0xE6 // Timer lower 8 bits
__sfr __at(P_TIMER_LOW)				io_Timer_Low;

#define P_TIMER_HIGH				0xE7 // Timer higher 8 bits
__sfr __at(P_TIMER_HIGH)			io_Timer_High;