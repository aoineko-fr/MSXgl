// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄  ▄▄▄  ▄▄   ▄    
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██▄▀ ██   █▄██ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ██   ██▄▄   ██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// OL4 (YMF278B) handler module
//
// References:
//  - OPL4 YMF278B Application Manual
//     https://map.grauw.nl/resources/sound/yamaha_opl4.pdf
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"
#include "system_port.h"
#include "audio/opl4_reg.h"

//-----------------------------------------------------------------------------
// CHECK OPTIONS
//-----------------------------------------------------------------------------

// OPL4_USE_WAVE
#ifndef OPL4_USE_WAVE
	#warning OPL4_USE_WAVE is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define OPL4_USE_WAVE			TRUE
#endif

// OPL4_USE_RESUME
#ifndef OPL4_USE_RESUME
	#warning OPL4_USE_RESUME is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define OPL4_USE_RESUME			TRUE
#endif

//=============================================================================
// DEFINES
//=============================================================================

// Moonsound (OPL4/YMF278) FM I/O ports
#define P_OPL4_ADDR0				0xC4 // Part 0 address
__sfr __at(P_OPL4_ADDR0)			io_OPL4_Reg0;
#define P_OPL4_DATA0				0xC5 // Part 0 data
__sfr __at(P_OPL4_DATA0)			io_OPL4_Data0;
#define P_OPL4_ADDR1				0xC6 // Part 1 address
__sfr __at(P_OPL4_ADDR1)			io_OPL4_Reg1;
#define P_OPL4_DATA1				0xC7 // Part 1 data
__sfr __at(P_OPL4_DATA1)			io_OPL4_Data1;
#define P_OPL4_STATUS				0xC4 // Status
const __sfr __at(P_OPL4_STATUS)		io_OPL4_Stat;

// Moonsound (OPL4/YMF278) Wave I/O ports
#define P_OPL4_WAV_ADDR				0x7E // Wave address
__sfr __at(P_OPL4_WAV_ADDR)			io_OPL4_WaveReg;
#define P_OPL4_WAV_DATA				0x7F // Wave data
__sfr __at(P_OPL4_WAV_DATA)			io_OPL4_WaveData;

// OPL4 compatibility mode
enum OPL4_MODE
{
	OPL4_MODE_OPL2 = 0, // 
	OPL4_MODE_OPL3,     // 
	OPL4_MODE_OPL4,     // YMF278B
	OPL4_MODE_MAX,
};

//
enum OPL4_MEMSIZE
{
	OPL4_MEMSIZE_128K = 1,
	OPL4_MEMSIZE_256K = 2,
	OPL4_MEMSIZE_512K = 4,
	OPL4_MEMSIZE_640K = 5,
	OPL4_MEMSIZE_1M   = 8,
	OPL4_MEMSIZE_2M   = 16,
	OPL4_MEMSIZE_4M   = 32,
};

//=============================================================================
// PROTOTYPE
//=============================================================================

//.............................................................................
// Group: Core

// Function: OPL4_Initialize
// Initialize OPL4 module
// Note: Must be called first before any other access to OPL4 sound card
//
// Parameters:
//   mode - Compatilibity mode
//> OPL4_MODE_OPL2
//> OPL4_MODE_OPL3
//> OPL4_MODE_OPL4
//
// Return:
//   FALSE if OPL4 not found
bool OPL4_Initialize(u8 mode);

// Function: OPL4_Detect
// Detect OPL4 device
//
// Return:
//   FALSE if OPL4 not found
bool OPL4_Detect();

// Function: OPL4_GetStatus
// Get status register value
//
// Return:
//   Status value (combination of following flags)
//> OPL4_STATUS_BUSY	Busy
//> OPL4_STATUS_LD		Wave header loading
//> OPL4_STATUS_FT2		Timer 2 overflow flag
//> OPL4_STATUS_FT1		Timer 1 overflow flag
//> OPL4_STATUS_IRQ		IRQ
inline u8 OPL4_GetStatus() { return io_OPL4_Stat; }

// Function: OPL4_IsBusy
// Is OPL4 busy (registers are not ready to be written)
//
// Return:
//   FALSE if OPL4 is ready to be written
inline bool OPL4_IsBusy() { return OPL4_GetStatus() & OPL4_STATUS_BUSY; }

// Function: OPL4_WaitBusy
// Wait while OPL4 is busy (with timeout)
inline void OPL4_WaitBusy() { u8 timeout = 0; while (OPL4_IsBusy() && --timeout) {} }

// Function: OPL4_IsLoading
// Is OPL4 loading data
//
// Return:
//   FALSE if OPL4 has finished to load data
inline bool OPL4_IsLoading() { return OPL4_GetStatus() & OPL4_STATUS_LD; }

// Function: OPL4_WaitBusy
// Wait while OPL4 is loading data (with timeout)
inline void OPL4_WaitLoad() { u8 timeout = 0; while (OPL4_IsLoading() && --timeout) {} }

// Function: OPL4_SetPart0Register
// Set OPL4 FM register value for part 0 (timers, rhythm, FM global control and FM channels 0-8)
//
// Parameters:
//   reg   - Register to set
//   value - Value to set
#if (OPL4_USE_RESUME)
void OPL4_SetPart0Register(u8 reg, u8 value);
#else
inline void OPL4_SetPart0Register(u8 reg, u8 value) { io_OPL4_Reg0 = reg; io_OPL4_Data0 = value; }
#endif

// Function: OPL4_GetPart0Register
// Get OPL4 FM register value for part 0 (timers, rhythm, FM global control and FM channels 0-8)
//
// Parameters:
//   reg - Register to get
//
// Return:
//   Value of the register
inline u8 OPL4_GetPart0Register(u8 reg) { io_OPL4_Reg0 = reg; return io_OPL4_Data0; }

// Function: OPL4_SetPart1Register
// Set OPL4 FM register value for part 1 (4-operator mode, expansion and FM channels 9-17)
// Note: Only R#261 (expansion) is accessible while NEW = 0 (OPL2 mode)
//
// Parameters:
//   reg   - Register to set
//   value - Value to set
#if (OPL4_USE_RESUME)
void OPL4_SetPart1Register(u8 reg, u8 value);
#else
inline void OPL4_SetPart1Register(u8 reg, u8 value) { io_OPL4_Reg1 = reg; io_OPL4_Data1 = value; }
#endif

// Function: OPL4_GetPart1Register
// Get OPL4 FM register value for part 1 (4-operator mode, expansion and FM channels 9-17)
//
// Parameters:
//   reg - Register to get
//
// Return:
//   Value of the register
inline u8 OPL4_GetPart1Register(u8 reg) { io_OPL4_Reg1 = reg; return io_OPL4_Data1; }

// Function: OPL4_SetWaveRegister
// Set OPL4 wave register value (memory control and wave channels 0-23)
// Note: Writes are ignored while NEW2 = 0 (see <OPL4_Initialize> with OPL4_MODE_OPL4)
//
// Parameters:
//   reg   - Register to set
//   value - Value to set
#if (OPL4_USE_RESUME)
void OPL4_SetWaveRegister(u8 reg, u8 value);
#else
inline void OPL4_SetWaveRegister(u8 reg, u8 value) { io_OPL4_WaveReg = reg; io_OPL4_WaveData = value; }
#endif

// Function: OPL4_GetWaveRegister
// Get OPL4 wave register value (memory control and wave channels 0-23)
// Note: Register selection is ignored while NEW2 = 0 (see <OPL4_Initialize> with OPL4_MODE_OPL4)
//
// Parameters:
//   reg - Register to get
//
// Return:
//   Value of the register
inline u8 OPL4_GetWaveRegister(u8 reg) { io_OPL4_WaveReg = reg; return io_OPL4_WaveData; }

// Function: OPL4_Mute
// Mute OPL4 sound (FM channels output and wave channels total level)
// Note: A channel stays muted until <OPL4_Resume> is called or its register (R#C0-C8 or W#50-67) is written
void OPL4_Mute();

#if (OPL4_USE_RESUME)
// Function: OPL4_Resume
// Resume OPL4 sound (restore registers values from before <OPL4_Mute>)
void OPL4_Resume();
#endif

// Function: OPL4_SetMix
// Set FM and wave mix control registers (W#248 and W#249), whatever the compatibility mode
//
// Parameters:
//   fm   - FM mix control value (see OPL4_REG_MIX_FM)
//   wave - Wave mix control value (see OPL4_REG_MIX_WAVE)
void OPL4_SetMix(u8 fm, u8 wave);

#if (OPL4_USE_WAVE)

// Function: OPL4_WaveLoad
// Write Wave data into the OPL4 memory
// Note: OPL4_USE_WAVE need to be set to TRUE to use this function
//
// Parameters:
//   data - Pointer to the wave data to load
//   addr_h - Start address in wave memory, bits 21-16 (MoonSound RAM starts at 20h, RAM-only 4 MB cartridges at 00h)
//   addr_l - Start address in wave memory, bits 15-0
//   size - Data size (in bytes)
void OPL4_WaveLoad(const u8* data, u8 addr_h, u16 addr_l, u16 size);

// Function: OPL4_WaveGetMemSize
// Get OPL4 total wave memory size (RAM found in the whole 4 MB wave memory space)
// Note: OPL4_USE_WAVE need to be set to TRUE to use this function
// Note: The first 2 bytes of each 128 KB block of the RAM are overwritten (on a RAM-only cartridge, this includes the header of wave 0)
//
// Return:
//   Number of 128 KB RAM blocks
//> OPL4_MEMSIZE_128K
//> OPL4_MEMSIZE_256K
//> OPL4_MEMSIZE_512K
//> OPL4_MEMSIZE_640K
//> OPL4_MEMSIZE_1M
//> OPL4_MEMSIZE_2M
//> OPL4_MEMSIZE_4M
u8 OPL4_WaveGetMemSize();

// Function: OPL4_WaveStartMemAccess
// Enable memory access mode
// Note: OPL4_USE_WAVE need to be set to TRUE to use this function
inline void OPL4_WaveStartMemAccess() { OPL4_SetWaveRegister(OPL4_REG_WAV_MEMORY, (OPL4_GetWaveRegister(OPL4_REG_WAV_MEMORY) & ~OPL4_ID_MASK) | OPL4_F_MEM_ACCESS); }

// Function: OPL4_WaveEndMemAccess
// End of memory access
// Note: OPL4_USE_WAVE need to be set to TRUE to use this function
inline void OPL4_WaveEndMemAccess() { OPL4_SetWaveRegister(OPL4_REG_WAV_MEMORY, (OPL4_GetWaveRegister(OPL4_REG_WAV_MEMORY) & ~OPL4_ID_MASK) & ~OPL4_F_MEM_ACCESS); }

// Function: OPL4_WaveSetMemAddr
// Set wave memory address (and select the memory data register)
// Note: OPL4_USE_WAVE need to be set to TRUE to use this function
//
// Parameters:
//   addr_h - Bits 16-21 of the memory address
//   addr_l - Bits 0-15 of the memory address
void OPL4_WaveSetMemAddr(u8 addr_h, u16 addr_l);

#endif // (OPL4_USE_WAVE)
