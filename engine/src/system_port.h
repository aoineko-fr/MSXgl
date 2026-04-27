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
__sfr __at(P_PPI_A)					g_PortPrimarySlot; // Port to access the primary Slot selection register. (PPI's port A is used)

//-----------------------------------------------------------------------------
// 0xA9								keyboard status register (Read/Write)
//-----------------------------------------------------------------------------
#define P_PPI_B						0xA9
#define P_KBD_STAT					P_PPI_B
__sfr __at(P_PPI_B)					g_PortReadKeyboard; // Port to read the keyboard matrix row specified via the port AAh. (PPI's port B is used)

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
__sfr __at(P_PPI_C)					g_PortAccessKeyboard; // Port to access the register that control the keyboard CAP LED, two signals to data recorder and a matrix row (use the port C of PPI).

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
__sfr __at(P_PPI_MODE)				g_PortControl; // Port to access the ports control register. (Write only)

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
__sfr __at(P_VDP_DATA)				g_VDP_DataPort; // VRAM data port (read/write)

//-----------------------------------------------------------------------------
// 0x99								General IO port (Read/Write)
//-----------------------------------------------------------------------------
#define P_VDP_1						0x99			// Primary MSX port for VDP port #1
#define P_VDP_REG					P_VDP_1			// Register setup port (write) (bit 7=1 in second write)
__sfr __at(P_VDP_REG)				g_VDP_RegPort;	// Register setup port (write) (bit 7=1 in second write)
#define P_VDP_ADDR					P_VDP_1			// VRAM address port (write) (bit 7=0 in second write, bit 6: read/write access (0=read, 1=write))
__sfr __at(P_VDP_ADDR)				g_VDP_AddrPort;	// VRAM address port (write) (bit 7=0 in second write, bit 6: read/write access (0=read, 1=write))
#define P_VDP_STAT					P_VDP_1			// Status register port (read)
__sfr __at(P_VDP_STAT)				g_VDP_StatPort;	// Status register port (read)

#if (MSX_VERSION >= MSX_2)

//-----------------------------------------------------------------------------
// 0x9A								Palette access port (Write only)
//-----------------------------------------------------------------------------
#define P_VDP_2						0x9A			// Primary MSX port for VDP port #2 (only v9938/v9958)
#define P_VDP_PAL					P_VDP_2			// Palette access port (write)
__sfr __at(P_VDP_PAL)				g_VDP_PalPort;	// Palette access port (write)

//-----------------------------------------------------------------------------
// 0x9B								Indirect register access port (Write only)
//-----------------------------------------------------------------------------
#define P_VDP_3						0x9B			// Primary MSX port for VDP port #3 (only v9938/v9958)
#define P_VDP_IREG					P_VDP_3			// Indirect register access port (write)
__sfr __at(P_VDP_IREG)				g_VDP_IRegPort;	// Indirect register access port (write)

#endif // (MSX_VERSION >= MSX_2)

//=============================================================================
//
// PSG Ports
//
//=============================================================================

// Internal PSG I/O ports
#define P_PSG_REGS 0xA0   // PSG register write port
__sfr __at(P_PSG_REGS) g_PSG_RegPort;
#define P_PSG_DATA 0xA1   // PSG value write port
__sfr __at(P_PSG_DATA) g_PSG_DataPort;
#define P_PSG_STAT 0xA2   // PSG value read port
__sfr __at(P_PSG_STAT) g_PSG_StatPort;

// External PSG I/O ports
#define P_PSG_EXT_REGS 0x10   // PSG register write port
__sfr __at(P_PSG_EXT_REGS) g_PSG_Ext_RegPort;
#define P_PSG_EXT_DATA 0x11   // PSG value write port
__sfr __at(P_PSG_EXT_DATA) g_PSG_Ext_DataPort;
#define P_PSG_EXT_STAT 0x12   // PSG value read port
__sfr __at(P_PSG_EXT_STAT) g_PSG_Ext_StatPort;

//=============================================================================
//
// MSX-Music Ports
//
//=============================================================================

// Internal/external MSX-Music I/O ports
#define P_MSXMUSIC_INDEX	0x7C // Register Index
__sfr __at(P_MSXMUSIC_INDEX) g_MSXMusic_IndexPort;
#define P_MSXMUSIC_DATA	0x7D // Register data
__sfr __at(P_MSXMUSIC_DATA) g_MSXMusic_DataPort;

//=============================================================================
//
// MSX-Audio Ports
//
//=============================================================================

// Primary MSX-Audio I/O ports
#define P_MSXAUDIO_INDEX	0xC0 // Register Index
__sfr __at(P_MSXAUDIO_INDEX) g_MSXAudio_IndexPort;
#define P_MSXAUDIO_DATA		0xC1 // Register data
__sfr __at(P_MSXAUDIO_DATA) g_MSXAudio_DataPort;

// Secondary MSX-Audio I/O ports
#define P_MSXAUDIO_INDEX2	0xC2 // Register Index
__sfr __at(P_MSXAUDIO_INDEX2) g_MSXAudio_IndexPort2;
#define P_MSXAUDIO_DATA2	0xC3 // Register data
__sfr __at(P_MSXAUDIO_DATA2) g_MSXAudio_DataPort2;

//=============================================================================
//
// RTC Ports
//
//=============================================================================

#define P_RTC_ADDR #0xB4   // RTC address
__sfr __at(P_RTC_ADDR) g_RTC_AddrPort;
#define P_RTC_DATA #0xB5   // RTC data
__sfr __at(P_RTC_DATA) g_RTC_DataPort;
