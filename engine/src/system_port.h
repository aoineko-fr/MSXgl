// ____________________________ 
// ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄           ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  █ ██  ██▀ ██▀ ▄███ ▄█▄█
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀  ▀██ ▄██  ▀█▄ ▀█▄▄ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘        ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// I/O Ports
//
// References:
// - http://map.grauw.nl/resources/msx_io_ports.php
// - https://www.msx.org/wiki/I/O_Ports_List
//─────────────────────────────────────────────────────────────────────────────

//-----------------------------------------------------------------------------
// PPI
//-----------------------------------------------------------------------------

#define P_PSL_STAT #0xA8   // slot status
#define P_KBD_STAT #0xA9   // keyboard status
#define P_GIO_REGS #0xAA   // General IO Register
#define P_PPI_REGS #0xAB   // PPI register

#define P_PPI_A 0xA8
__sfr __at(P_PPI_A) g_PortPrimarySlot; // Port to access the primary Slot selection register. (PPI's port A is used)
// bit 0~1 = Number of slot to select on page 0 (0000h~3FFFh)
// bit 2~3 = Number of slot to select on page 1 (4000h~7FFFh)
// bit 4~5 = Number of slot to select on page 2 (8000h~BFFFh)
// bit 6~7 = Number of slot to select on page 3 (C000h~FFFFh)

#define P_PPI_B 0xA9
__sfr __at(P_PPI_B) g_PortReadKeyboard; // Port to read the keyboard matrix row specified via the port AAh. (PPI's port B is used)

#define P_PPI_C 0xAA
__sfr __at(P_PPI_C) g_PortAccessKeyboard; // Port to access the register that control the keyboard CAP LED, two signals to data recorder and a matrix row (use the port C of PPI).
// bits 0~3 = Row number of specified keyboard matrix to read via port B
// bit 4 = Data recorder motor (reset to turn on)
// bit 5 = Set to write on tape
// bit 6 = Keyboard LED CAPS (reset to turn on)
// bit 7 = 1, then 0 shortly thereafter to make a clicking sound (used for the keyboard).

#define P_PPI_MODE 0xAB
__sfr __at(P_PPI_MODE) g_PortControl; // Port to access the ports control register. (Write only)
// bit 0 = Bit status to change
// bit 1~3 = Number of the bit to change at port C of the PPI
// bit 4~6 = Unused
// bit 7 = Must be always reset on MSX.

//-----------------------------------------------------------------------------
// VDP Ports
//-----------------------------------------------------------------------------

// 98h
#define P_VDP_0			0x98			///< Primary MSX port for VDP port #0
#define P_VDP_DATA		P_VDP_0			///< VRAM data port (read/write)
__sfr __at(P_VDP_DATA)	g_VDP_DataPort; ///< VRAM data port (read/write)

// 99h
#define P_VDP_1			0x99			///< Primary MSX port for VDP port #1
#define P_VDP_REG		P_VDP_1			///< Register setup port (write) (bit 7=1 in second write)
__sfr __at(P_VDP_REG)	g_VDP_RegPort;	///< Register setup port (write) (bit 7=1 in second write)
#define P_VDP_ADDR		P_VDP_1			///< VRAM address port (write) (bit 7=0 in second write, bit 6: read/write access (0=read, 1=write))
__sfr __at(P_VDP_ADDR)	g_VDP_AddrPort;	///< VRAM address port (write) (bit 7=0 in second write, bit 6: read/write access (0=read, 1=write))
#define P_VDP_STAT		P_VDP_1			///< Status register port (read)
__sfr __at(P_VDP_STAT)	g_VDP_StatPort;	///< Status register port (read)

#if (MSX_VERSION >= MSX_2)

// 9Ah
#define P_VDP_2			0x9A			///< Primary MSX port for VDP port #2 (only v9938/v9958)
#define P_VDP_PAL		P_VDP_2			///< Palette access port (write)
__sfr __at(P_VDP_PAL)	g_VDP_PalPort;	///< Palette access port (write)

// 9Bh
#define P_VDP_3			0x9B			///< Primary MSX port for VDP port #3 (only v9938/v9958)
#define P_VDP_IREG		P_VDP_3			///< Indirect register access port (write)
__sfr __at(P_VDP_IREG)	g_VDP_IRegPort;	///< Indirect register access port (write)

#endif // (MSX_VERSION >= MSX_2)

//-----------------------------------------------------------------------------
// PSG
//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------
// RTC

#define P_RTC_ADDR #0xB4   // RTC address
__sfr __at(P_RTC_ADDR) g_RTC_AddrPort;
#define P_RTC_DATA #0xB5   // RTC data
__sfr __at(P_RTC_DATA) g_RTC_DataPort;
