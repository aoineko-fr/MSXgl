// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄ ▄▄▄  ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██ █ ██▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▀  ██▄▀ ██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Features to manage the VDP
// Support MSX1 (TMS9918A), MSX2 (V9938) and MSX2+ (V9958)
//
// References:
// - Texas Instruments VDP Programmer’s Guide
// - V9938 Technical Data Book Programmer's Guide
// - Yamaha V9958 MSX-VIDEO Application Manual
// - https://www.msx.org/wiki/Category:VDP_Registers
// - http://map.grauw.nl/articles/
//─────────────────────────────────────────────────────────────────────────────
#pragma once
#pragma sdcc_hash +

#include "core.h"
#include "vdp_reg.h"
#include "bios_var.h"

//=============================================================================
// DEFINES
//=============================================================================

// @todo Handle VRAM read/write access timing
//
// Minimum VRAM access timings in 3.58 MHz Z80 cycles
// from https://map.grauw.nl/articles/vdp_tut.php#vramtiming
//
// Screen	VDP		MSX1	MSX2/2+
// mode		mode	cycles	cycles
//-------------------------------------
// 0,W40	T1		12		20
// 0,W80	T2				20
// 1		G1		29		15
// 2		G2		29		15
// 3		MC		13		15
// 4		G3				15
// 5		G4				15
// 6		G5				15
// 7		G6				15
// 8		G7				15
//
// outi			; 18 T-States
//
// otir			; 23 T-States
//
// OutiToVram:  ; 29 T-States (18 + 11)
//     outi
//     jp nz, OutiToVram

// VDP access timing into instructions
//
// Instruction				TS		I/O
//-------------------------------------
// in a,(n) / out (n),a		12		9|3
// in a,(c) / out (c),a		14		11|3
// outi / outd				18		15|3
// ini / ind				18		12|6
// otir / otdr				23		15|8
// 							18		15|3
// inir / indr				23		12|11
//							18		12|6

//-----------------------------------------------------------------------------
// STRUCTURES
//-----------------------------------------------------------------------------

// Structure used to store VDP module data
struct VDP_Data
{
	u8  Mode;		// Current screen mode (see <VDP_MODE>)
	u8  BPC    : 4;	// Bits per color of the current mode (can be 1, 2, 4 or 8)
	u8  Width  : 1;	// Width of the current screen (0: 256 px, 1: 512 px)
	u8  Height : 1;	// Height of the current screen (0: 192 px, 1: 212 px)
};

// Structure used to store register data for VDP command
struct VDP_Command
{
	u16 SX;  // 32-33
	u16 SY;  // 34-35
	u16 DX;  // 36-37
	u16 DY;  // 38-39
	u16 NX;  // 40-41
	u16 NY;  // 42-43
	u8  CLR; // 44
	u8  ARG; // 45
	u8  CMD; // 46
};
#define VDP_Command32 VDP_Command

// Structure used to store register data for VDP command
struct VDP_Command36
{
	u16 DX;  // 36-37
	u16 DY;  // 38-39
	u16 NX;  // 40-41
	u16 NY;  // 42-43
	u8  CLR; // 44
	u8  ARG; // 45
	u8  CMD; // 46
};

// Structure to store a sprite attribute. see function <VDP_SetSprite>
struct VDP_Sprite
{
    u8 Y;			// Y coordinate on screen (all lower priority sprite will be disable if equal to 216 or 0xD0)
    u8 X;			// X coordinate of the sprite
    u8 Pattern;		// Pattern index
    u8 Color;		// Color index (Sprite Mode 1 only)
    // u8 Color   : 4;	// Color index (Sprite Mode 1 only)
    // u8 _unused : 3;	// (unused 3 bits)
    // u8 EC      : 1;	// Early clock ; used to offset sprite by  32  dots  to  the  left  (Sprite Mode 1 only)
};

//-----------------------------------------------------------------------------
// EXTERNALS
//-----------------------------------------------------------------------------


extern u8                 g_VDP_REGSAV[28]; // VDP register backup buffer

extern struct VDP_Data    g_VDP_Data; // VDP configuration structure

#if (VDP_USE_COMMAND)
extern struct VDP_Command g_VDP_Command; // VDP command buffer structure
#endif

#if (VDP_USE_SPRITE)
extern struct VDP_Sprite  g_VDP_Sprite; // Sprite data buffer
#endif

extern u16 g_ScreenLayoutLow;		// Address of the Pattern Layout Table (Name)
extern u16 g_ScreenColorLow;		// Address of the Color Table
extern u16 g_ScreenPatternLow;		// Address of the Pattern Generator Table
#if (VDP_USE_SPRITE)
extern u16 g_SpriteAttributeLow;	// Address of the Sprite Attribute Table
extern u16 g_SpritePatternLow;		// Address of the Sprite Pattern Generator Table
extern u16 g_SpriteColorLow;		// Address of the Sprite Color Table
#endif // (VDP_USE_SPRITE)
#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
extern u8 g_ScreenLayoutHigh;		// Address of the Pattern Layout Table (Name)
extern u8 g_ScreenColorHigh;		// Address of the Color Table
extern u8 g_ScreenPatternHigh;		// Address of the Pattern Generator Table
#if (VDP_USE_SPRITE)
extern u8 g_SpriteAtributeHigh;		// Address of the Sprite Attribute Table
extern u8 g_SpritePatternHigh;		// Address of the Sprite Pattern Generator Table
extern u8 g_SpriteColorHigh;		// Address of the Sprite Color Table
#endif // (VDP_USE_SPRITE)
#endif // (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_14)
	#define VADDR				u16
	#define VADDR_LO(a)			(a)
	#define VADDR_HI(a)			0
	#define VADDR_GET(lo, hi)	(lo)
#else // if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
	#define VADDR				u32
	#define VADDR_LO(a)			(u16)(a)
	#define VADDR_HI(a)			(u16)(a >> 16)
	#define VADDR_GET(lo, hi)	((u32)(lo) | ((u32)hi << 16))
#endif

#if (VDP_UNIT == VDP_UNIT_U16)
	#define UX					u16
	#define USX					i16
	#define UY					u16
	#define USY					i16
#elif (VDP_UNIT == VDP_UNIT_X16)
	#define UX					u16
	#define USX					i16
	#define UY					u8
	#define USY					i8
#elif (VDP_UNIT == VDP_UNIT_Y16)
	#define UX					u8
	#define USX					i8
	#define UY					u16
	#define USY					i16
#else // if (VDP_UNIT == VDP_UNIT_U8)
	#define UX					u8
	#define USX					i8
	#define UY					u8
	#define USY					i8
#endif

#define VRAM16b(a)				(u16)((u32)(a >> 4))
#define VRAM17b(a)				(u16)((u32)(a >> 1))
#define Addr20bTo16b(a)			(u16)((u32)(a >> 4))	// Convert 20-bits (V)RAM address into 16-bits with bit shifting
#define Addr17bTo16b(a)			(u16)((u32)(a >> 1))	// Convert 17-bits (V)RAM address into 16-bits with bit shifting

#define REGSAV(a)				#(_g_VDP_REGSAV+a)

#define GET_ADDR_LOW(addr)		((addr) & 0xFFFF)		// Get 17-bits address the 16 LSB
#define GET_ADDR_HIGH(addr)		((addr) >> 16)			// Get 17-bits address the 1 MSB

// Convert VRAM address into 2D coordinate for VDP commands
#define ADDR_TO_X				ADDR_TO_8B_X
#define ADDR_TO_2B_X(addr)		((addr) / 64)
#define ADDR_TO_4B_X(addr)		((addr) / 128)
#define ADDR_TO_8B_X(addr)		((addr) / 256)
#define ADDR_TO_Y				ADDR_TO_8B_Y
#define ADDR_TO_2B_Y(addr)		((addr) % 64)
#define ADDR_TO_4B_Y(addr)		((addr) % 128)
#define ADDR_TO_8B_Y(addr)		((addr) % 256)

// Enum: VDP_MODE
// VDP display modes
enum VDP_MODE
{
	//.........................................................................
	// VDP modes
	VDP_MODE_MSX1        = 0,				// TMS 9918A video modes
	VDP_MODE_TEXT1       = VDP_MODE_MSX1,	// 00 001	TEXT	40 characters per line of text, one colour for each characte
	VDP_MODE_MULTICOLOR,					// 00 010	TILES	pseudo-graphic, one character divided into four block
	VDP_MODE_GRAPHIC1,						// 00 000	TILES	32 characters per one line of text, the COLOURed character available
	VDP_MODE_GRAPHIC2,						// 00 100	TILES	256 x 192, the colour is specififed for each 8 dots
#if (VDP_USE_UNDOCUMENTED)
	VDP_MODE_UNDOC, 						// Undocumented video modes. Only available on TMS 9918A
	VDP_MODE_UNDOC_0_2   = VDP_MODE_UNDOC,	// 00 101	
	VDP_MODE_UNDOC_1_2,						// 00 100	
	VDP_MODE_UNDOC_3_2,						// 00 110	
#endif
#if (MSX_VERSION >= MSX_2)
	VDP_MODE_MSX2,							// V9938 video modes
	VDP_MODE_TEXT2       = VDP_MODE_MSX2,	// 01 001	TEXT	80 characters per line of text, character blinkable selection
	VDP_MODE_GRAPHIC3,						// 01 000	TILES	GRAPHIC 2 which can use sprite mode 2
	VDP_MODE_GRAPHIC4,						// 01 100	BITMAP	256 x 212; 16 colours are available for each dot
	VDP_MODE_GRAPHIC5,						// 10 000	BITMAP	512 x 212; 4 colours are available for each dot
	VDP_MODE_GRAPHIC6,						// 10 100	BITMAP	512 x 212; 16 colours are available for each dot
	VDP_MODE_GRAPHIC7,						// 11 100	BITMAP	256 x 212; 256 colours are available for each dot
#endif
	//.........................................................................
	// BASIC screen modes
	VDP_MODE_SCREEN0     = VDP_MODE_TEXT1,
	VDP_MODE_SCREEN0_W40 = VDP_MODE_SCREEN0,
	VDP_MODE_SCREEN1     = VDP_MODE_GRAPHIC1,
	VDP_MODE_SCREEN2     = VDP_MODE_GRAPHIC2,
	VDP_MODE_SCREEN3     = VDP_MODE_MULTICOLOR,
#if (MSX_VERSION >= MSX_2)
	VDP_MODE_SCREEN0_W80 = VDP_MODE_TEXT2,
	VDP_MODE_SCREEN4     = VDP_MODE_GRAPHIC3,
	VDP_MODE_SCREEN5     = VDP_MODE_GRAPHIC4,
	VDP_MODE_SCREEN6     = VDP_MODE_GRAPHIC5,
	VDP_MODE_SCREEN7     = VDP_MODE_GRAPHIC6,
	VDP_MODE_SCREEN8     = VDP_MODE_GRAPHIC7,
	VDP_MODE_SCREEN9,
	VDP_MODE_SCREEN9_40  = VDP_MODE_SCREEN9,
	VDP_MODE_SCREEN9_80,
#endif
#if (MSX_VERSION >= MSX_2P)
	VDP_MODE_MSX2P,							// V9958 video modes
	VDP_MODE_SCREEN10    = VDP_MODE_MSX2P,
	VDP_MODE_SCREEN11,
	VDP_MODE_SCREEN12,
#endif
	//.........................................................................
	VDP_MODE_MAX,
};

// Enum: VDP_VERSION
// VDP version enumaration
enum VDP_VERSION
{
	VDP_VERSION_TMS9918A = 0,	// MSX1 VDP
	VDP_VERSION_V9938,			// MSX2 VDP
	VDP_VERSION_V9958,			// MSX2+/Turbo-R VDP
};

// Enum: VRAM_SIZE
// VRAM size enumaration
enum VRAM_SIZE
{
	VRAM_16K	= 0b00,
	VRAM_64K	= 0b01,
	VRAM_128K	= 0b10,
	VRAM_192K	= 0b11,	
};

// Get VRAM size macro
#define GET_VRAM_SIZE()		((g_MODE >> 1) & 0x3)

//-----------------------------------------------------------------------------
// Default Screen Mode tables VRAM address

// Screen 0 (Width 40)
#define VDP_T1_MODE			0b00001
#define VDP_T1_ADDR_NT		0x0000 // Name Table
#define VDP_T1_ADDR_PT		0x0800 // Pattern Table

// Screen 1
#define VDP_G1_MODE			0b00000
#define VDP_G1_ADDR_NT		0x1800 // Name Table
#define VDP_G1_ADDR_CT		0x2000 // Color Table
#define VDP_G1_ADDR_PT		0x0000 // Pattern Table
#define VDP_G1_ADDR_SAT		0x1B00 // Sprite Attribute Table
#define VDP_G1_ADDR_SPT		0x3800 // Sprite Pattern Table

// Screen 2
#define VDP_G2_MODE			0b00100
#define VDP_G2_ADDR_NT		0x1800 // Name Table
#define VDP_G2_ADDR_CT		0x2000 // Color Table
#define VDP_G2_ADDR_PT		0x0000 // Pattern Table
#define VDP_G2_ADDR_SAT		0x1B00 // Sprite Attribute Table
#define VDP_G2_ADDR_SPT		0x3800 // Sprite Pattern Table

// Screen 3
#define VDP_MC_MODE			0b00010
#define VDP_MC_ADDR_NT		0x0800 // Name Table
#define VDP_MC_ADDR_PT		0x0000 // Pattern Table
#define VDP_MC_ADDR_SAT		0x1B00 // Sprite Attribute Table
#define VDP_MC_ADDR_SPT		0x3800 // Sprite Pattern Table

// Screen 0 (Width 80)
#define VDP_T2_MODE			0b01001
#define VDP_T2_ADDR_NT		0x0000 // Name Table
#define VDP_T2_ADDR_PT		0x1000 // Pattern Table

// Screen 4
#define VDP_G3_MODE			0b01000
#define VDP_G3_ADDR_NT		0x1800 // Name Table
#define VDP_G3_ADDR_CT		0x2000 // Color Table
#define VDP_G3_ADDR_PT		0x0000 // Pattern Table
#define VDP_G3_ADDR_SAT		0x1E00 // Sprite Attribute Table
#define VDP_G3_ADDR_SPT		0x3800 // Sprite Pattern Table

// Screen 5
#define VDP_G4_MODE			0b01100
#define VDP_G4_ADDR_NT		0x0000 // Name Table
#define VDP_G4_ADDR_SAT		0x7600 // Sprite Attribute Table
#define VDP_G4_ADDR_SPT		0x7800 // Sprite Pattern Table

// Screen 6
#define VDP_G5_MODE			0b10000
#define VDP_G5_ADDR_NT		0x0000 // Name Table
#define VDP_G5_ADDR_SAT		0x7600 // Sprite Attribute Table
#define VDP_G5_ADDR_SPT		0x7800 // Sprite Pattern Table

// Screen 7
#define VDP_G6_MODE			0b10100
#define VDP_G6_ADDR_NT		0x0000 // Name Table
#define VDP_G6_ADDR_SAT		0xFA00 // Sprite Attribute Table
#define VDP_G6_ADDR_SPT		0xF000 // Sprite Pattern Table

// Screen 8, 10, 11 & 12
#define VDP_G7_MODE			0b11100
#define VDP_G7_ADDR_NT		0x0000 // Name Table
#define VDP_G7_ADDR_SAT		0xFA00 // Sprite Attribute Table
#define VDP_G7_ADDR_SPT		0xF000 // Sprite Pattern Table

// Screen 9 (Korean)

//=============================================================================
// PROTOTYPES
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Helper
//-----------------------------------------------------------------------------

// Function: VDP_Initialize
// Initialize VDP module. [MSX1/2/2+/TR]
// Retreive BIOS initiale value.
// This function is called automatically when <VDP_SetMode> is called if the macro VDP_AUTO_INIT equals 1.
void VDP_Initialize();

// Function: VDP_GetVersion
// Get VDP version. [MSX1/2/2+/TR]
//
// Return:
//   VDP identifier (see <VDP_VERSION>).
u8 VDP_GetVersion() __naked;

// Function: VDP_ClearVRAM
// Clear the VRAM content. [MSX1/2/2+/TR]
void VDP_ClearVRAM();

//-----------------------------------------------------------------------------
// Group: Screen mode
//-----------------------------------------------------------------------------

// Function: VDP_SetMode
// Set screen mode. [MSX1/2/2+/TR]
//
// Parameters:
//   mode - The new screen mode to set (see <VDP_MODE> enumeration)
void VDP_SetMode(const u8 mode);

// Function: VDP_GetMode
// Get current screen mode. [MSX1/2/2+/TR]
//
// Return:
//   Current screen mode
inline u8 VDP_GetMode() { return g_VDP_Data.Mode; }

// Function: VDP_IsBitmapMode
// Tell if the given screen mode is a bitmap mode (pattern/text mode otherwise). [MSX1/2/2+/TR]
//
// Parameters:
//   mode - VDP mode to set (see <VDP_MODE> enumeration)
//
// Return:
//   TRUE if the given screen mode is a bitmap mode, FALSE otherwise.
inline bool VDP_IsBitmapMode(const u8 mode)
{
#if (MSX_VERSION == MSX_1)
	mode;
	return FALSE;
#else
	return mode >= VDP_MODE_GRAPHIC4;
#endif
}

// Function: VDP_IsPatternMode
// Tell if the given screen mode is a pattern/text mode (bitmap mode otherwise). [MSX1/2/2+/TR]
//
// Parameters:
//   mode - VDP mode to set (see <VDP_MODE> enumeration)
//
// Return:
//   TRUE if the given screen mode is a pattern/text mode, FALSE otherwise.
inline bool VDP_IsPatternMode(const u8 mode) { return !VDP_IsBitmapMode(mode); }

// Function: VDP_SetModeFlag
// Set screen mode flag. [MSX1/2/2+/TR]
// VRAM tables address must be set to fit the new screen mode
//
// Parameters:
//   flag - Screen binary flag
void VDP_SetModeFlag(u8 flag);

//-----------------------------------------------------------------------------
// Group: Direct access
//-----------------------------------------------------------------------------

// Function: VDP_RegWrite
// Set register value. [MSX1/2/2+/TR]
//
// Parameters:
//   reg   - Register number
//   value - Value to set
void VDP_RegWrite(u8 reg, u8 value) __PRESERVES(b, c, d, e, iyl, iyh);

// Function: VDP_RegWriteBak
// Set register value after backuping previous value. [MSX1/2/2+/TR]
//
// Parameters:
//   reg   - Register number
//   value - Value to set
void VDP_RegWriteBak(u8 reg, u8 value) __PRESERVES(d, e, iyl, iyh);

// Function: VDP_RegWriteBakMask
// Set register value after backuping previous value using mask. [MSX1/2/2+/TR]
//
// Parameters:
//   reg  - Register number
//   mask - Previous value reset mask
//   flag - Value to set
void VDP_RegWriteBakMask(u8 idx, u8 mask, u8 value);

// Function: VDP_ReadDefaultStatus
// Read default S#0 register. [MSX1/2/2+/TR]
//
// Return:
//   Status register #0 value
u8 VDP_ReadDefaultStatus() __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: VDP_ReadStatus
// Read a given status register then reset status register to default (0). [MSX2/2+/TR]
//
// Parameters:
//   stat - Status register number (0-9)
//
// Return:
//   The given status register value
u8 VDP_ReadStatus(u8 stat) __PRESERVES(b, c, d, e, h, iyl, iyh);

#if ((VDP_USE_VRAM16K) || (MSX_VERSION == MSX_1) || (MSX_VERSION == MSX_12))

// Function: VDP_WriteVRAM_16K
// Write data from RAM to VRAM. [MSX1/2/2+/TR]
//
// Parameters:
//   src   - Source data address in RAM
//   dest  - Destiation address in VRAM (14bits address for 16KB VRAM)
//   count - Number of byte to copy in VRAM
void VDP_WriteVRAM_16K(const u8* src, u16 dest, u16 count) __sdcccall(0);

// Function: VDP_FillVRAM_16K
// Fill VRAM area with a given value. [MSX1/2/2+/TR]
// Use fast 16-bits loop (28 cc)
//
// Parameters:
//   value	- Byte value to copy in VRAM
//   dest	- Destiation address in VRAM (14 bits address form 16 KB VRAM)
//   count	- Number of byte to copy in VRAM
void VDP_FillVRAM_16K(u8 value, u16 dest, u16 count);

#if (VDP_USE_FASTFILL)
// Function: VDP_FastFillVRAM_16K
// Fill VRAM area with a given value [MSX1/2/2+/TR]
// Use ultra fast 16-bits using 16 unrolled out loop (~14,9 cc)
//
// Parameters:
//   value	- Byte value to copy in VRAM
//   dest	- Destiation address in VRAM (14 bits address form 16 KB VRAM)
//   count	- Nomber of byte to copy in VRAM
void VDP_FastFillVRAM_16K(u8 value, u16 dest, u16 count);
#endif

// Function: VDP_ReadVRAM_16K
// Read data from VRAM to RAM. [MSX1/2/2+/TR]
//
// Parameters:
//   src	- Source address in VRAM (14bits address form 16KB VRAM)
//   dest	- Desitation data address in RAM
//   count	- Number of byte to copy from VRAM
void VDP_ReadVRAM_16K(u16 src, u8* dest, u16 count) __sdcccall(0);

// Function: VDP_Poke_16K
// Write a value to VRAM. [MSX1/2/2+/TR]
//
// Parameters:
//   val	- Value to write in VRAM
//   dest	- Desitation address in VRAM (14bits address form 16KB VRAM)
void VDP_Poke_16K(u8 val, u16 dest) __PRESERVES(c, h, l, iyl, iyh);

// Function: VDP_Peek_16K
// Read a value from VRAM. [MSX1/2/2+/TR]
//
// Parameters:
//   src	- Source address in VRAM (14bits address form 16KB VRAM)
//
// Return:
//   Value read in VRAM
u8 VDP_Peek_16K(u16 src) __PRESERVES(b, c, d, e, iyl, iyh);

#endif // (VDP_USE_VRAM16K)

// Set generic VRAM access function
#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_14)

	#define VDP_WriteVRAM(src, destLow, destHigh, count)	VDP_WriteVRAM_16K(src, destLow, count)
	#define VDP_FillVRAM(value, destLow, destHigh, count)	VDP_FillVRAM_16K(value, destLow, count)
	#define VDP_ReadVRAM(srcLow, srcHigh, dest, count)		VDP_ReadVRAM_16K(srcLow, dest, count)
	#define VDP_Poke(val, destLow, destHigh)				VDP_Poke_16K(val, destLow)
	#define VDP_Peek(srcLow, srcHigh)						VDP_Peek_16K(srcLow)

#else // (MSX_VERSION >= MSX_2)

	// Function: VDP_WriteVRAM_128K
	// Write data from RAM to VRAM. [MSX2/2+/TR]
	//
	// Parameters:
	//   src		- Source data address in RAM
	//   destLow	- Destiation address in VRAM (16 LSB of 17-bits VRAM address)
	//   destHigh	- Destiation address in VRAM (1 MSB of 17-bits VRAM address)
	//   count		- Nomber of byte to copy in VRAM
	void VDP_WriteVRAM_128K(const u8* src, u16 destLow, u8 destHigh, u16 count) __sdcccall(0);

	// Function: VDP_FillVRAM_128K
	// Fill VRAM area with a given value. [MSX2/2+/TR]
	//
	// Parameters:
	//   value		- Byte value to copy in VRAM
	//   destLow	- Destiation address in VRAM (16 LSB of 17-bits VRAM address)
	//   destHigh	- Destiation address in VRAM (1 MSB of 17-bits VRAM address)
	//   count		- Nomber of byte to copy in VRAM
	void VDP_FillVRAM_128K(u8 value, u16 destLow, u8 destHigh, u16 count) __sdcccall(0);

	// Function: VDP_ReadVRAM_128K
	// Read data from VRAM to RAM. [MSX2/2+/TR]
	//
	// Parameters:
	//   srcLow		- Source address in VRAM (16 LSB of 17-bits VRAM address)
	//   srcHigh	- Source address in VRAM (1 MSB of 17-bits VRAM address)
	//   dest		- Desitation data address in RAM
	//   count		- Nomber of byte to copy from VRAM
	void VDP_ReadVRAM_128K(u16 srcLow, u8 srcHigh, u8* dest, u16 count) __sdcccall(0);

	// Function: VDP_Poke_128K
	// Write a value to VRAM. [MSX2/2+/TR]
	//
	// Parameters:
	//   val		- Value to write in VRAM
	//   destLow	- Destiation address in VRAM (16 LSB of 17-bits VRAM address)
	//   destHigh	- Destiation address in VRAM (1 MSB of 17-bits VRAM address)
	void VDP_Poke_128K(u8 val, u16 destLow, u8 destHigh) __sdcccall(0);

	// Function: VDP_Peek_128K
	// Read a value from VRAM. [MSX2/2+/TR]
	//
	// Parameters:
	//   srcLow		- Source address in VRAM (16 LSB of 17-bits VRAM address)
	//   srcHigh	- Source address in VRAM (1 MSB of 17-bits VRAM address)
	//
	// Return:
	//   Value read in VRAM
	u8 VDP_Peek_128K(u16 srcLow, u8 srcHigh) __sdcccall(0);

	#define VDP_WriteVRAM									VDP_WriteVRAM_128K
	#define VDP_FillVRAM									VDP_FillVRAM_128K
	#define VDP_ReadVRAM									VDP_ReadVRAM_128K
	#define VDP_Poke										VDP_Poke_128K
	#define VDP_Peek										VDP_Peek_128K

#endif

//-----------------------------------------------------------------------------
// Group: Display setup
//-----------------------------------------------------------------------------

//.............................................................................
// MSX1

// Function: VDP_EnableDisplay
// Enable/disable screen display (register 1). [MSX1/2/2+/TR]
//
// Parameters:
//   enable - True to enable, FALSE do disable
inline void VDP_EnableDisplay(bool enable) { VDP_RegWriteBakMask(1, (u8)~R01_BL, enable ? R01_BL : 0); }

// Function: VDP_EnableVBlank
// Enable/disable vertical interruption (register 1). [MSX1/2/2+/TR]
//
// Parameters:
//   enable - True to enable, FALSE do disable
inline void VDP_EnableVBlank(bool enable) { VDP_RegWriteBakMask(1, (u8)~R01_IE0, enable ? R01_IE0 : 0); }

// Function: VDP_SetColor
// Set text and border default color (register 7). [MSX1/2/2+/TR]
//
// Parameters:
//   color - Defaut color (Format: [TXT:4|BG:4])
inline void VDP_SetColor(u8 color) { VDP_RegWrite(7, color); }

//.............................................................................
// MSX2
#if (MSX_VERSION >= MSX_2)

// Function: VDP_EnableSprite
// Enable/disable sprite rendering (register 8). [MSX2/2+/TR]
//
// Parameters:
//   enable - True to enable, FALSE do disable
inline void VDP_EnableSprite(u8 enable) { VDP_RegWriteBakMask(8, (u8)~R08_SPD, !enable ? R08_SPD : 0); }

// Function: VDP_DisableSprite
// Disable sprite rendering (see <VDP_EnableSprite>). [MSX1/2/2+/TR]
inline void VDP_DisableSprite() { VDP_EnableSprite(FALSE); }

// Function: VDP_EnableHBlank
// Enable/disable horizontal interruption (register 0). [MSX2/2+/TR]
//
// Parameters:
//   enable - True to enable, FALSE do disable
inline void VDP_EnableHBlank(bool enable) { VDP_RegWriteBakMask(0, (u8)~R00_IE1, enable ? R00_IE1 : 0); }

// Function: VDP_SetHBlankLine
// Set the horizontal-blank interruption line (register 19). [MSX2/2+/TR]
//
// Parameters:
//   line - Line number where the interruption will occure
inline void VDP_SetHBlankLine(u8 line) { VDP_RegWrite(19, line); }

// Function: VDP_SetVerticalOffset
// Set the vertical rendering offset (register 23). [MSX2/2+/TR]
//
// Parameters:
//   offset - Number of lines of offset from the top of the screen
inline void VDP_SetVerticalOffset(u8 offset) { VDP_RegWrite(23, offset); }

// Function: VDP_SetAdjustOffset
// Adjustment of the display location on the screen (register 18). [MSX2/2+/TR]
//
// Parameters:
//   offset - Screen display position offset (MSB 4-bits: vertical offset, LSB 4-bits: horizontal offset)
void VDP_SetAdjustOffset(u8 offset);

// Function: VDP_SetAdjustOffsetXY
// Adjustment of the display location on the screen (register 18). [MSX2/2+/TR]
//
// Parameters:
//   x - Horizontal screen display position offset [-7:+8]
//   y - Vertical screen display position offset [-7:+8]
inline void VDP_SetAdjustOffsetXY(i8 x, i8 y) { VDP_SetAdjustOffset(((-x) & 0x0F) | (((-y) & 0x0F) << 4)); }

// Function: VDP_SetGrayScale
//
// Parameters:
//   enable - True to enable, FALSE do disable
// Enable/disable grayscale (register 8). [MSX2/2+/TR]
inline void VDP_SetGrayScale(bool enable) { VDP_RegWriteBakMask(8, (u8)~R08_BW, enable ? R08_BW : 0); }

// Enum: VDP_FREQ
// VDP frequency flags
enum VDP_FREQ
{
	VDP_FREQ_50HZ = R09_NT,		// Frequency at 50 Hz
	VDP_FREQ_60HZ = 0,			// Frequency at 60 Hz
};
// Function: VDP_SetFrequency
// Change VDP frequency (register 9). [MSX2/2+/TR]
//
// Parameters:
//   lines - Can be 50 (VDP_FREQ_50HZ) or 60 Hz (VDP_FREQ_60HZ)
inline void VDP_SetFrequency(u8 freq) { VDP_RegWriteBakMask(9, (u8)~R09_NT, freq); }

// Enum: VDP_LINE
// VDP line flags
enum VDP_LINE
{
	VDP_LINE_192 = 0,			// 192 lines mode
	VDP_LINE_212 = R09_LN,		// 212 lines mode
};
// Function: VDP_SetLineCount
// Set line count for the current screen mode (register 9). [MSX2/2+/TR]
//
// Parameters:
//   lines - Can be VDP_LINE_192 or VDP_LINE_212
inline void VDP_SetLineCount(u8 lines) { VDP_RegWriteBakMask(9, (u8)~R09_LN, lines); }

// Function: VDP_SetPageAlternance
// Enable automatic page switch on even/odd frames. [MSX2/2+/TR]
//
// Parameters:
//   enable - True to enable, FALSE do disable
inline void VDP_SetPageAlternance(bool enable) { VDP_RegWriteBakMask(9, (u8)~R09_EO, enable ? R09_EO : 0); }

// Function: VDP_SetInterlace
// Enable or disable interlace mode. [MSX2/2+/TR]
// Needs <VDP_SetPageAlternance> to be set to TRUE and <VDP_SetPage> to a odd value.
//
// Parameters:
//   enable - True to enable, FALSE do disable
inline void VDP_SetInterlace(bool enable) { VDP_RegWriteBakMask(9, (u8)~R09_IL, enable ? R09_IL : 0); }

// Enum: VDP_FRAME
// VDP render modes
enum VDP_FRAME
{
	VDP_FRAME_STATIC     = 0,               // Render always the same frame
	VDP_FRAME_ALTERNANCE = R09_EO,          // Render 2 frames in alternace
	VDP_FRAME_INTERLACE  = (R09_EO|R09_IL), // Render 2 frames at the same time interlaced
};
// Function: VDP_SetFrameRender
// Set the frame render mode. [MSX2/2+/TR]
//
// Parameters:
//   mode - Can be VDP_FRAME_STATIC, VDP_FRAME_ALTERNANCE or VDP_FRAME_INTERLACE
inline void VDP_SetFrameRender(u8 mode) { VDP_RegWriteBakMask(9, (u8)~(R09_EO|R09_IL), mode); }

// Function: VDP_SetPalette
// Set a new color palette from index 1 to 15. [MSX2/2+/TR]
//
// Parameters:
//   pal - Address of the palette in RAM (table of 15 * 2 bytes)
//         Format: [0:5|green:3|0|red:3|0|blue:3]
void VDP_SetPalette(const u8* pal) __FASTCALL __PRESERVES(d, e, iyl, iyh);

// Function: VDP_SetPaletteEntry
// Set a given color entry in the palette. [MSX2/2+/TR]
//
// Parameters:
//   index - Index of the palette entry (0-15)
//   color - 16 bits color value (@see RGB16 macro)
//           Format: [0:5|green:3|0|red:3|0|blue:3]
void VDP_SetPaletteEntry(u8 index, u16 color);

#if (VDP_USE_DEFAULT_PALETTE)
// Function: VDP_SetDefaultPalette
// Reset the default MSX2 palette. [MSX2/2+/TR]
void VDP_SetDefaultPalette();
#endif

#if (VDP_USE_MSX1_PALETTE)
// Function: VDP_SetMSX1Palette
// Reset the default MSX1 palette. [MSX2/2+/TR]
void VDP_SetMSX1Palette();
#endif

#endif

//.............................................................................
// MSX2+
#if (MSX_VERSION >= MSX_2P)

// Enum: VDP_YJK
// YJK modes
enum VDP_YJK
{
	VDP_YJK_OFF				= 0,					// Use classic 8-bits RGB mode
	VDP_YJK_ON				= R25_YJK,				// Use YJK mode (5-bits for lightness)
	VDP_YJK_YAE				= (R25_YJK|R25_YAE),	// Use YJK mode (4-bits for lightness) + 16 colors palette
};
// Function: VDP_SetYJK
// Set YJK mode. [MSX2+/TR].
// See <VDP_YJK> enum.
//
// Parameters:
//   mode - YJK mode (can be VDP_YJK_OFF, VDP_YJK_ON or VDP_YJK_YAE)
inline void VDP_SetYJK(u8 mode) { VDP_RegWriteBakMask(25, (u8)~VDP_YJK_YAE, mode); }

// Function: VDP_ExpendCommand
// Enables the VDP commands for screens 0 to 4 (register 25). [MSX2+/TR]
//
// Parameters:
//   enable - True to enable, FALSE do disable
inline void VDP_ExpendCommand(u8 enable) { VDP_RegWriteBakMask(25, (u8)~R25_CMD, enable ? R25_CMD : 0); }

// Function: VDP_EnableMask
// Allows to hide the first 8 vertical lines at left of screen (register 25). [MSX2+/TR]
//
// Parameters:
//   enable - True to enable, FALSE do disable
inline void VDP_EnableMask(u8 enable) { VDP_RegWriteBakMask(25, (u8)~R25_MAK, enable ? R25_MAK : 0); }

// Function: VDP_SetHorizontalOffset
// Set the horizontal rendeing offset. [MSX2+/TR]
//
// Parameters:
//   offset - Screen horizontal offset (9-bits value in pixel)
void VDP_SetHorizontalOffset(u16 offset);

#define VDP_HSCROLL_SINGLE		0		// Scroll within a single screen page
#define VDP_HSCROLL_DOUBLE		R25_SP2 // Scroll between two screen pages
// Function: VDP_SetHorizontalMode
// Set horizontal scrolling to occurs on a single page or two pages (register 25). [MSX2+/TR]
//
// Parameters:
//   mode - Can be VDP_HSCROLL_SINGLE or VDP_HSCROLL_DOUBLE
inline void VDP_SetHorizontalMode(u8 mode) { VDP_RegWriteBakMask(25, (u8)~R25_SP2, mode); }

#endif

//-----------------------------------------------------------------------------
// Group: VRAM tables address
//-----------------------------------------------------------------------------

// Function: VDP_SetLayoutTable
// Set layout table VRAM address. [MSX1/2/2+/TR]
//
// Parameters:
//   addr - VRAM address where to put the table (u16 for 14-bits address and u32 for 17-bits)
void VDP_SetLayoutTable(VADDR addr);

// Function: VDP_SetColorTable
// Set color table VRAM address. [MSX1/2/2+/TR]
//
// Parameters:
//   addr - VRAM address where to put the table (u16 for 14-bits address and u32 for 17-bits)
void VDP_SetColorTable(VADDR addr);

// Function: VDP_SetPatternTable
// Set pattern table VRAM address. [MSX1/2/2+/TR]
//
// Parameters:
//   addr - VRAM address where to put the table (u16 for 14-bits address and u32 for 17-bits)
void VDP_SetPatternTable(VADDR addr);

#if (VDP_USE_SPRITE)

// Function: VDP_SetSpriteAttributeTable
// Set sprite attribute table address. [MSX1/2/2+/TR]
//
// Parameters:
//   addr - VRAM address where to put the table (u16 for 14-bits address and u32 for 17-bits)
void VDP_SetSpriteAttributeTable(VADDR addr);

// Function: VDP_SetSpritePatternTable
// Set sprite pattern table address. [MSX1/2/2+/TR]
//
// Parameters:
//   addr - VRAM address where to put the table (u16 for 14-bits address and u32 for 17-bits)
void VDP_SetSpritePatternTable(VADDR addr);

#endif

// Function: VDP_GetLayoutTable
// Get address of the Pattern Layout Table (Name)
//
// Return:
//   VRAM address of the table (u16 for 14-bits address and u32 for 17-bits)
inline VADDR VDP_GetLayoutTable() { return VADDR_GET(g_ScreenLayoutLow, g_ScreenLayoutHigh); }

// Function: VDP_GetColorTable
// Get address of the Color Table
//
// Return:
//   VRAM address of the table (u16 for 14-bits address and u32 for 17-bits)
inline VADDR VDP_GetColorTable() { return VADDR_GET(g_ScreenColorLow, g_ScreenColorHigh); }

// Function: VDP_GetPatternTable
// Get address of the Pattern Generator Table
//
// Return:
//   VRAM address of the table (u16 for 14-bits address and u32 for 17-bits)
inline VADDR VDP_GetPatternTable() { return VADDR_GET(g_ScreenPatternLow, g_ScreenPatternHigh); }

#if (VDP_USE_SPRITE)

// Function: VDP_GetSpriteAttributeTable
// Get address of the Sprite Attribute Table
//
// Return:
//   VRAM address of the table (u16 for 14-bits address and u32 for 17-bits)
inline VADDR VDP_GetSpriteAttributeTable() { return VADDR_GET(g_SpriteAttributeLow, g_SpriteAtributeHigh); }

// Function: VDP_GetSpritePatternTable
// Get address of the Sprite Pattern Generator Table
//
// Return:
//   VRAM address of the table (u16 for 14-bits address and u32 for 17-bits)
inline VADDR VDP_GetSpritePatternTable() { return VADDR_GET(g_SpritePatternLow, g_SpritePatternHigh); }

// Function: VDP_GetSpriteColorTable
// Get address of the Sprite Color Table
//
// Return:
//   VRAM address of the table (u16 for 14-bits address and u32 for 17-bits)
inline VADDR VDP_GetSpriteColorTable() { return VADDR_GET(g_SpriteColorLow, g_SpriteColorHigh); }

#endif

#if (MSX_VERSION >= MSX_2)

// Function: VDP_SetPage
// Set current VRAM page. [MSX2/2+/TR]
//
// Parameters:
//   page - Page number to set
void VDP_SetPage(u8 page);

#endif

//-----------------------------------------------------------------------------
// Group: Sprite
//-----------------------------------------------------------------------------
#if (VDP_USE_SPRITE)

#define VDP_SPRITE_SIZE_8		0			// Use 8x8 sprite size
#if (VDP_USE_16X16_SPRITE)
#define VDP_SPRITE_SIZE_16		R01_ST		// Use 16x16 sprite size
#endif
#define VDP_SPRITE_SCALE_1		0			// Normal size of the sprite (1 dot = 1 px)
#define VDP_SPRITE_SCALE_2		R01_MAG		// Double the size of the sprite (1 dot = 2 px)
// Function: VDP_SetSpriteFlag
// Set sprite rendering parameters. [MSX1/2/2+/TR]
//
// Parameters:
//   flag - Srptie flag to be set. Can be a combinations of:
//          VDP_SPRITE_SIZE_8 (Use 8x8 sprite size)
//          VDP_SPRITE_SIZE_16 (Use 16x16 sprite size)
//          VDP_SPRITE_SCALE_1 (Normal size of the sprite; 1 dot = 1 px)
//          VDP_SPRITE_SCALE_2 (Double the size of the sprite; 1 dot = 2 px)
inline void VDP_SetSpriteFlag(u8 flag) { VDP_RegWriteBakMask(1, (u8)~(R01_ST|R01_MAG), flag); }

// Function: VDP_SetSpriteTables
// Set sprite tables address. [MSX1/2/2+/TR]
//
// Parameters:
//   pattern - Sprite pattern table address
//   attrib	 - Sprite attribute table address
inline void VDP_SetSpriteTables(VADDR patAddr, VADDR attAddr) { VDP_SetSpritePatternTable(patAddr); VDP_SetSpriteAttributeTable(attAddr); }

// Function: VDP_LoadSpritePattern
// Load sprite pattern data into VRAM. [MSX1/2/2+/TR]
//
// Parameters:
//   addr  - Source address for sprite patterns
//   index - Index of the first VRAM sprite pattern to copy to.
//   count - Number of patterns to copy (1 per sprite shape in 8x8 mode, and 4 in 16x16 mode.
void VDP_LoadSpritePattern(const u8* addr, u8 index, u8 count);

#define VDP_SPRITE_EC			0x80		// Early clock ; used to offset sprite by 32 dots to the left
#define VDP_SPRITE_CC			0x40		// Sprite priority control
#define VDP_SPRITE_IC			0x20		// Line collision detection
// Function: VDP_SetSpriteSM1
// Set sprite attribute for Sprite Mode 1. [MSX1/2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table
//   x     - Initial X screen coordinate
//   y     - Initial Y screen coordinate
//   shape - Sprite pattern index (in 16x16 mode, only multiple of 4 can be used)
//   color - Sprite color (LSB 4-bits) + optional EC flag (VDP_SPRITE_EC)
void VDP_SetSpriteSM1(u8 index, u8 x, u8 y, u8 shape, u8 color);

#if (MSX_VERSION >= MSX_2)
	
// Function: VDP_SetSprite
// Set sprite attribute for Sprite Mode 2. [MSX2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table
//   x     - Initial X screen coordinate
//   y     - Initial Y screen coordinate
//   shape - Sprite pattern index (in 16x16 mode, only multiple of 4 can be used)
void VDP_SetSprite(u8 index, u8 x, u8 y, u8 shape);

// Function: VDP_SetSpriteExMultiColor
// Set sprite attribute for Sprite Mode 2 and fill color table with color data. [MSX2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table
//   x     - Initial X screen coordinate
//   y     - Initial Y screen coordinate
//   shape - Sprite pattern index (in 16x16 mode, only multiple of 4 can be used)
//   ram   - Address to a 16 bytes buffer that determine the sprite colors (1 byte = 1 line).
//           With this function, you must provide a 16 bytes buffer even for 8x8 size mode.
void VDP_SetSpriteExMultiColor(u8 index, u8 x, u8 y, u8 shape, const u8* ram);

// Function: VDP_SetSpriteExUniColor
// Set sprite attribute for Sprite Mode 2 and fill color table with unique color. [MSX2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table
//   x     - Initial X screen coordinate
//   y     - Initial Y screen coordinate
//   shape - Sprite pattern index (in 16x16 mode, only multiple of 4 can be used)
//   color - Sprite color (LSB 4-bits) + optional EC, CC & IC flags 
//           (this color is used for the all 8/16 lines of the sprite)
void VDP_SetSpriteExUniColor(u8 index, u8 x, u8 y, u8 shape, u8 color);

#endif // (MSX_VERSION >= MSX_2)

// Function: VDP_SetSpritePosition
// Update sprite position. [MSX1/2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table
//   x     - Initial X screen coordinate
//   y     - Initial Y screen coordinate
void VDP_SetSpritePosition(u8 index, u8 x, u8 y);

// Function: VDP_SetSpritePositionX
// Update sprite position X. [MSX1/2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table
//   x     - Initial X screen coordinate
void VDP_SetSpritePositionX(u8 index, u8 x);

// Function: VDP_SetSpritePositionY
// Update sprite position Y. [MSX1/2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table
//   y     - Initial Y screen coordinate
void VDP_SetSpritePositionY(u8 index, u8 y);

// Function: VDP_SetSpritePattern
// Update sprite pattern. [MSX1/2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table
//   shape - Sprite pattern index (in 16x16 mode, only multiple of 4 can be used)
void VDP_SetSpritePattern(u8 index, u8 shape);

// Function: VDP_SetSpriteColorSM1
// Update sprite pattern (Sprite Mode 1). [MSX1/2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table
//   color - Sprite color (LSB 4-bits) + optional EC flag (see <VDP_SPRITE_EC>)
void VDP_SetSpriteColorSM1(u8 index, u8 color);

#if (MSX_VERSION >= MSX_2)
	
// Function: VDP_SetSpriteUniColor
// Update sprite color (Uni-color). [MSX2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table
//   color - Sprite color (LSB 4-bits) + optional EC, CC & IC flags 
//           (this color is used for the all 8 lines of the sprite)
void VDP_SetSpriteUniColor(u8 index, u8 color);

// Function: VDP_SetSpriteMultiColor
// Update sprite color (Multi-color). [MSX2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table
//   ram   - Address to a 16 bytes buffer that determine the sprite colors (1 byte = 1 line).
//           With this function, you must provide a 16 bytes buffer even for 8x8 size mode.
void VDP_SetSpriteMultiColor(u8 index, const u8* ram);

// Function: VDP_SetSpriteData
// Set sprite data for Sprite Mode 2. [MSX2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table
//   data  - Address to a 3 bytes data buffer including sprite position and shape
void VDP_SetSpriteData(u8 index, const u8* data);

#endif // (MSX_VERSION >= MSX_2)

#define VDP_SPRITE_DISABLE_SM1	208			// This sprite and all lower priority sprites will be disabled (Sprite Mode 1).
#define VDP_SPRITE_DISABLE_SM2	216			// This sprite and all lower priority sprites will be disabled (Sprite Mode 2).
#define VDP_SPRITE_HIDE			213			// Coordinate to hide sprite in any screen mode (work on both MSX 1 and 2).
// Function: VDP_DisableSpritesFrom
// Disable all sprites from a given index. [MSX1/2/2+/TR]
//
// Parameters:
//   index - Disable sprite rendering starting from this sprite index
void VDP_DisableSpritesFrom(u8 index);

// Function: VDP_HideSprite
// Hide a given sprite (don't disable following sprites). [MSX1/2/2+/TR]
//
// Parameters:
//   index - Index of the sprite to hide
inline void VDP_HideSprite(u8 index) { VDP_SetSpritePositionY(index, VDP_SPRITE_HIDE); }

#endif // (VDP_USE_SPRITE)

//-----------------------------------------------------------------------------
// Group: GM2
// Graph mode 2 & 3 specific functions
//-----------------------------------------------------------------------------
#if (VDP_USE_MODE_G2 || VDP_USE_MODE_G3)

// Function: VDP_FillScreen_GM2
// Fill the full screen with a given pattern value. [MSX1/2/2+/TR]
//
// Parameters:
//   value - Pattern index to fill
//           In G2/G3 modes, the same index can define different pattern in the 3 vertical sections
inline void VDP_FillScreen_GM2(u8 value) { VDP_FillVRAM(value, g_ScreenLayoutLow, g_ScreenLayoutHigh, 32*24); }

// Function: VDP_Poke_GM2
// Set a GM cell value from x/y coordinate
inline void VDP_Poke_GM2(u8 x, u8 y, u8 value) { VDP_Poke(value, g_ScreenLayoutLow + (y * 32) + x, g_ScreenLayoutHigh); }

// Function: VDP_Peek_GM2
// Get a GM cell value from x/y coordinate
inline u8 VDP_Peek_GM2(u8 x, u8 y) { return VDP_Peek(g_ScreenLayoutLow + (y * 32) + x, g_ScreenLayoutHigh); }

// Function: VDP_LoadPattern_GM2
// Load patterns in all 3 screen sections. [MSX1/2/2+/TR]
//
// Parameters:
//   src    - Address of data buffer to copy to VRAM
//   count  - Number of pattern to copy
//   offset - Pattern index from which to copy data 
void VDP_LoadPattern_GM2(const u8* src, u8 count, u8 offset);

// Function: VDP_LoadColor_GM2
// Load colors in all 3 screen sections. [MSX1/2/2+/TR]
//
// Parameters:
//   src    - Address of data buffer to copy to VRAM
//   count  - Number of pattern to copy
//   offset - Pattern index from which to copy data 
void VDP_LoadColor_GM2(const u8* src, u8 count, u8 offset);

// Function: VDP_WriteLayout_GM2
// Copy patterns layout to a given rectangle. [MSX1/2/2+/TR]
//
// Parameters:
//   src    - Address of data buffer to copy to VRAM
//   dx     - Destination starting X position 
//   dy     - Destination starting Y position 
//   dx     - Destination width
//   dy     - Destination height
void VDP_WriteLayout_GM2(const u8* src, u8 dx, u8 dy, u8 nx, u8 ny);

// Function: VDP_FillLayout_GM2
// Fill a rectangle with a given value. [MSX1/2/2+/TR]
//
// Parameters:
//   value - Pattern index to fill
//   dx    - Destination starting X position 
//   dy    - Destination starting Y position 
//   dx    - Destination width
//   dy    - Destination height
void VDP_FillLayout_GM2(u8 value, u8 dx, u8 dy, u8 nx, u8 ny);

#endif

//-----------------------------------------------------------------------------
// Group: Command
// VDP commands wrapper functions
//-----------------------------------------------------------------------------
#if ((MSX_VERSION >= MSX_2) && ((VDP_USE_COMMAND) || (VDP_USE_CUSTOM_CMD)))

// Function: VDP_CommandWait
// Wait for previous VDP command to be finished. [MSX2/2+/TR]
void VDP_CommandWait() __PRESERVES(b, c, d, e, h, l, iyl, iyh);

#endif

#if ((MSX_VERSION >= MSX_2) && (VDP_USE_COMMAND))

// Function: VPD_CommandSetupR32
// Send VDP command (form registres 32 to 46). [MSX2/2+/TR]
void VPD_CommandSetupR32();

// Function: VPD_CommandSetupR36
// Send VDP command (form registres 36 to 46). [MSX2/2+/TR]
void VPD_CommandSetupR36();

// Function: VPD_CommandWriteLoop
// Write to VRAM command loop. [MSX2/2+/TR]
//
// Parameters:
//   addr - Address of the source buffer
void VPD_CommandWriteLoop(const u8* addr) __FASTCALL __PRESERVES(b, d, e, iyl, iyh);

// Function: VPD_CommandReadLoop
// Read to VRAM command loop. [MSX2/2+/TR]
//
// Parameters:
//   addr - Address of the destination buffer (in RAM)
void VPD_CommandReadLoop(u8* addr) __FASTCALL;

//-----------------------------------------------------------------------------
// INLINE FUNCTIONS
//-----------------------------------------------------------------------------

#include "vdp_inl.h"

// VDP COMMANDS

// inline void VDP_CommandHMMC(const u8* addr, u16 dx, u16 dy, u16 nx, u16 ny); // High speed move CPU to VRAM.
// inline void VDP_CommandYMMM(u16 sy, u16 dx, u16 dy, u16 ny, u8 dir); // High speed move VRAM to VRAM, Y coordinate only.
// inline void VDP_CommandHMMM(u16 sx, u16 sy, u16 dx, u16 dy, u16 nx, u16 ny); // High speed move VRAM to VRAM.
// inline void VDP_CommandHMMV(u16 dx, u16 dy, u16 nx, u16 ny, u8 col); // High speed move VDP to VRAM.
// inline void VDP_CommandLMMC(); // Logical move CPU to VRAM.
// inline void VDP_CommandLMCM(); // Logical move VRAM to CPU.
// inline void VDP_CommandLMMM(u16 sx, u16 sy, u16 dx, u16 dy, u16 nx, u16 ny, u8 op); // Logical move VRAM to VRAM.
// inline void VDP_CommandLMMV(u16 dx, u16 dy, u16 nx, u16 ny, u8 col, u8 op); // Logical move VDP to VRAM.
// inline void VDP_CommandLINE(u16 dx, u16 dy, u16 nx, u16 ny, u8 col, u8 arg, u8 op); // Draw straight line in VRAM.
// inline void VDP_CommandSRCH(u16 sx, u16 sy, u8 col, u8 arg); // Search for the specific color in VRAM to the right or left of the starting point.
// inline void VDP_CommandPSET(u16 dx, u16 dy, u8 col, u8 op); // Draw a dot in VRAM.
// inline u8   VDP_CommandPOINT(u16 sx, u16 sy); // Read the color of the specified dot located in VRAM.
// inline void VDP_CommandSTOP(); // Abort current command.

#define VDP_CopyRAMtoVRAM			VDP_CommandHMMC		// High speed move CPU to VRAM
#define VDP_YMoveVRAM				VDP_CommandYMMM		// High speed move VRAM to VRAM, Y coordinate only
#define VDP_MoveVRAM				VDP_CommandHMMM		// High speed move VRAM to VRAM
//#define VDP_FillVRAM				VDP_CommandHMMV		// High speed move VDP to VRAM
#define VDP_LogicalCopyRAMtoVRAM	VDP_CommandLMMC		// Logical move CPU to VRAM
#define VDP_LogicalYMoveVRAM		VDP_CommandLMCM		// Logical move VRAM to CPU
#define VDP_LogicalMoveVRAM			VDP_CommandLMMM		// Logical move VRAM to VRAM
#define VDP_LogicalFillVRAM			VDP_CommandLMMV		// Logical move VDP to VRAM
#define VDP_DrawLine				VDP_CommandLINE		// Draw straight line in VRAM
#define VDP_SearchColor				VDP_CommandSRCH		// Search for the specific color in VRAM to the right or left of the starting point
#define VDP_DrawPoint				VDP_CommandPSET		// Draw a dot in VRAM 
#define VDP_ReadPoint				VDP_CommandPOINT	// Read the color of the specified dot located in VRAM 
#define VDP_AbortCommand			VDP_CommandSTOP		// Abort current command

#endif // ((MSX_VERSION >= MSX_2) && (VDP_USE_COMMAND))

#if ((MSX_VERSION >= MSX_2) && (VDP_USE_CUSTOM_CMD))

// Function: VDP_CommandCustomR32
// Send VDP custom command through buffer (form registres 32 to 46). [MSX2/2+/TR]
void VDP_CommandCustomR32(const struct VDP_Command* data);

// Function: VDP_CommandCustomR36
// Send VDP custom command through buffer (form registres 36 to 46). [MSX2/2+/TR]
void VDP_CommandCustomR36(const struct VDP_Command36* data);

#endif