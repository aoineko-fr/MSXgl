// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄  ▄▄   ▄▄   ▄▄   ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ █▄██ █▄██ █▄██ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▀   ▄█▀  ▄█▀  ▄█▀ ▀▄█▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// References:
// - V9990 Application Manual
// - http://msxbanzai.tni.nl/v9990/manual.html
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"
#include "v9990_reg.h"

//=============================================================================
// DEFINES
//=============================================================================

//-----------------------------------------------------------------------------
// Check parameters
//-----------------------------------------------------------------------------

// V9_USE_MODE_P1
#if !defined(V9_USE_MODE_P1)
	#warning V9_USE_MODE_P1 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define V9_USE_MODE_P1			TRUE
#endif
// V9_USE_MODE_P2
#if !defined(V9_USE_MODE_P2)
	#warning V9_USE_MODE_P2 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define V9_USE_MODE_P2			TRUE
#endif
// V9_USE_MODE_B0
#if !defined(V9_USE_MODE_B0)
	#warning V9_USE_MODE_B0 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define V9_USE_MODE_B0			TRUE
#endif
// V9_USE_MODE_B1
#if !defined(V9_USE_MODE_B1)
	#warning V9_USE_MODE_B1 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define V9_USE_MODE_B1			TRUE
#endif
// V9_USE_MODE_B2
#if !defined(V9_USE_MODE_B2)
	#warning V9_USE_MODE_B2 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define V9_USE_MODE_B2			TRUE
#endif
// V9_USE_MODE_B3
#if !defined(V9_USE_MODE_B3)
	#warning V9_USE_MODE_B3 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define V9_USE_MODE_B3			TRUE
#endif
// V9_USE_MODE_B4
#if !defined(V9_USE_MODE_B4)
	#warning V9_USE_MODE_B4 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define V9_USE_MODE_B4			TRUE
#endif
// V9_USE_MODE_B5
#if !defined(V9_USE_MODE_B5)
	#warning V9_USE_MODE_B5 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define V9_USE_MODE_B5			TRUE
#endif
// V9_USE_MODE_B6
#if !defined(V9_USE_MODE_B6)
	#warning V9_USE_MODE_B6 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define V9_USE_MODE_B6			TRUE
#endif
// V9_USE_MODE_B7
#if !defined(V9_USE_MODE_B7)
	#warning V9_USE_MODE_B7 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define V9_USE_MODE_B7			TRUE
#endif
// V9_INT_PROTECT
#if !defined(V9_INT_PROTECT)
	#warning V9_INT_PROTECT is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define V9_INT_PROTECT			TRUE
#endif
// V9_PALETTE_MODE
#if !defined(V9_PALETTE_MODE)
	#warning V9_PALETTE_MODE is not defined in "msxgl_config.h"! Default value will be used: V9_PALETTE_RGB_24
	#define V9_PALETTE_MODE			V9_PALETTE_RGB_24
#endif

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

// Screen modes
enum V9_SCREEN_MODE
{
	V9_MODE_P1, // Pattern mode 0 256x212
	V9_MODE_P2, // Pattern mode 1 512x212
	V9_MODE_B0, // Bitmap mode 1 192x240 (Undocumented v9990 mode)
	V9_MODE_B1, // Bitmap mode 1 256x212
	V9_MODE_B2, // Bitmap mode 2 384x240
	V9_MODE_B3, // Bitmap mode 3 512x212
	V9_MODE_B4, // Bitmap mode 4 768x240
	V9_MODE_B5, // Bitmap mode 5 640x400 (VGA)
	V9_MODE_B6, // Bitmap mode 6 640x480 (VGA)
	V9_MODE_B7, // Bitmap mode 7 1024x212 (Undocumented v9990 mode)
	V9_MODE_MAX,
};

// Color modes
enum V9_COLOR_MODE
{	//                 R#6  R#13
	//                 CLRM PLTM YAE
	// 2 bits per pixel
	V9_COLOR_BP2,	// 0    0    0	Color palette (4 colors out of 32768 colors)
	// 4 bits per pixel
	V9_COLOR_BP4,	// 1    0    0	Color palette (16 colors out of 32768 colors)
	// 8 bits per pixel
	V9_COLOR_BP6,	// 2    0    0	Color palette (64 colors out of 32768 colors)
	V9_COLOR_BD8,	// 2    1    0	Direct RGB [G:3|R:3|B:2] (256 colors)
	V9_COLOR_BYJK,	// 2    2    0	YJK Decoder (19268 colors)
	V9_COLOR_BYJKP,	// 2    2    1	YJK Decoder + Color palette (12599 colors + 16 colors out of 32768 colors)
	V9_COLOR_BYUV,	// 2    3    0	YUV Decoder (19268 colors)
	V9_COLOR_BYUVP,	// 2    3    1	YUV Decoder + Color palette (12599 colors + 16 colors out of 32768 colors)
	// 16 bits per pixel
	V9_COLOR_BD16,	// 3    0    0	Direct RGB [YS|G:5|R:2][R:3|B:5] (32768 colors)
	V9_COLOR_BMP_MAX,
	// Tile-based screen's color mode
	V9_COLOR_PP,	// 				Display type when the display mode is P1 or P2 (16/32 colors out of 32768 colors)
	V9_COLOR_MAX,
};

// Cursor disable bit
#define V9_CURSOR_DISABLE			0b00010000

// RGB color definition macro
#define V9_RGB(r, g, b)				(u16)(((u16)((g) & 0x1F) << 10) + (((r) & 0x1F) << 5) + ((b) & 0x1F))
#define V9_RGBYs(r, g, b, ys)		(u16)((((ys) & 0x01) << 15) + ((u16)((g) & 0x1F) << 10) + (((r) & 0x1F) << 5) + ((b) & 0x1F))

// Register access
#define V9_REG(n)					n
#define V9_RII						0b01000000
#define V9_WII						0b10000000

// Masks
#define V9_R06_COLOR_MASK			V9_R06_BPP_MASK
#define V9_R13_COLOR_MASK			(V9_R13_PLTM_MASK + V9_R13_YAE)

// P1 mode VRAM layout
#define V9_P1_PGT_A					0x00000	// Pattern Generator Table (Layer A). 8160 patterns max
#define V9_P1_SGT					0x00000	// Sprite Generator Table  
#define V9_P1_SPAT					0x3FE00	// Sprite Attribute Table
#define V9_P1_PGT_B					0x40000	// Pattern Generator Table (Layer B). 7680 patterns max
#define V9_P1_PNT_A					0x7C000	// Pattern Name Table (Layer A)
#define V9_P1_PNT_B					0x7E000	// Pattern Name Table (Layer B)

// P2 mode VRAM layout
#define V9_P2_PGT					0x00000 // Pattern Generator Table. 15360 patterns max
#define V9_P2_SGT					0x00000	// Sprite Generator Table
#define V9_P2_SPAT					0x7BE00 // Sprite Attribute Table
#define V9_P2_PNT					0x7C000 // Pattern Name Table

// Bitmap mode VRAM layout
#define V9_BMP_PGT					0x00000 // Pattern Generator Table (bitmap data)
#define V9_BMP_CUR					0x7FE00	// Cursor area (512 bytes)

// V9990 interruption flag
#define V9_INT_NONE					V9_P06_NONE
#define V9_INT_VBLANK				V9_P06_VI
#define V9_INT_HBLANK				V9_P06_HI
#define V9_INT_CMDEND				V9_P06_CE

//-----------------------------------------------------------------------------
// Structures
//-----------------------------------------------------------------------------

// V9990 sprite structure
struct V9_Sprite
{
	u8		Y;			// Sprite Y-coordinate (Actual display position is one line below specified)
	u8		Pattern;	// Sprite Pattern Number (Pattern Offset is specified in R#25 SGBA)
	u16		X  : 10;	// Sprite X-coordinate
	u16		_1 : 2;
	u16		D  : 1;		// Sprite is disabled when D=1
	u16		P  : 1;		// Sprite is in front of the front layer when P=0, sprite is behind the front layer when P=1.
	u16		SC : 2;		// Palette offset for sprite colors
};

// V9990 address structure
struct V9_Address
{
	u8 Lower;
	u8 Center;
	u8 Upper;
};

// External variable containing address to read or write
extern struct V9_Address g_V9_Address;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Core
//-----------------------------------------------------------------------------

// Function: V9_SetPort
// Set port value.
//
// Parameters:
//   port - Port address (V9990 is plug to ports 60h~6Fh).
//   value - Value to write to the port.
void V9_SetPort(u8 port, u8 value) __PRESERVES(b, d, e, h, iyl, iyh);

// Function: V9_GetPort
// Get port value.
//
// Parameters:
//   port - Port address (V9990 is plug to ports 60h~6Fh).
//
// Return:
//   Value read from the givem port.
u8 V9_GetPort(u8 port) __PRESERVES(b, d, e, h, l, iyl, iyh);

// Function: V9_SetRegister
// Set register value.
// This function as no effect if register is read-only.
//
// Parameters:
//   reg - V9990 register number.
//   val - Value to write to the register.
void V9_SetRegister(u8 reg, u8 val) __PRESERVES(b, c, d, e, h, iyl, iyh);

// Function: V9_SetRegister16
// Set register value.
// This function as no effect if register is read-only.
//
// Parameters:
//   reg - V9990 register number (LSB will be write to this register and MSB to the next).
//   val - 16-bits value to write to the registers.
void V9_SetRegister16(u8 reg, u16 val) __PRESERVES(b, h, l, iyl, iyh);

// Function: V9_GetRegister
// Get register value.
//
// Parameters:
//   reg - V9990 register number.
//
// Return:
//   Value read from the register (FFh is returned if register is write only).
u8 V9_GetRegister(u8 reg) __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: V9_SetFlag
// Helper function to change some bits in a given register.
// The function get a register value, apply a mask, 'or' the new value, than set the register.
//
// Parameters:
//   reg - V9990 register number.
//   mask - Bits to be cleared.
//   flag - Bits to be set.
inline void V9_SetFlag(u8 reg, u8 mask, u8 flag) { V9_SetRegister(reg, V9_GetRegister(reg) & (~mask) | flag); }

//-----------------------------------------------------------------------------
// Group: VRAM access
//-----------------------------------------------------------------------------

// Function: V9_SetWriteAddress
// Initialize VRAM address for writing.
//
// Parameters:
//   addr - Address to be setup for write access.
void V9_SetWriteAddress(u32 addr) __PRESERVES(b, iyl, iyh);

// Function: V9_SetReadAddress
// Initialize VRAM address for reading.
//
// Parameters:
//   addr - Address to be setup for read access.
void V9_SetReadAddress(u32 addr) __PRESERVES(b, iyl, iyh);

// Function: V9_FillVRAM_CurrentAddr
// Fill VRAM with 8-bits value from the previously setup write VRAM address.
// <V9_SetWriteAddress> have to be called first.
//
// Parameters:
//   value - 8-bits value to be set.
//   count - Size of the filled space in VRAM.
void V9_FillVRAM_CurrentAddr(u8 value, u16 count);

// Function: V9_FillVRAM16_CurrentAddr
// Fill VRAM with 16-bits value from the previously setup write VRAM address.
// <V9_SetWriteAddress> have to be called first.
//
// Parameters:
//   value - 16-bits value to be set.
//   count - Size of the filled space in VRAM (size in bytes is 'count * 2').
void V9_FillVRAM16_CurrentAddr(u16 value, u16 count);

// Function: V9_WriteVRAM_CurrentAddr
// Copy data from RAM buffer to the previously setup write VRAM address.
// <V9_SetWriteAddress> have to be called first.
//
// Parameters:
//   src - Pointer to the source data buffer to be copied in VRAM.
//   count - Size of the data to be copied in VRAM.
void V9_WriteVRAM_CurrentAddr(const u8* src, u16 count);

// Function: V9_ReadVRAM_CurrentAddr
// Copy data from the previously setup read VRAM address to a RAM buffer.
// <V9_SetReadAddress> have to be called first.
//
// Parameters:
//   dst - Pointer to the destination data buffer.
//   count - Size of the data to be copied in VRAM.
void V9_ReadVRAM_CurrentAddr(const u8* dest, u16 count);

// Function: V9_Poke_CurrentAddr
// Write a 8-bits value to the previously setup write VRAM address.
// <V9_SetWriteAddress> have to be called first.
//
// Parameters:
//   val - The value to be write.
void V9_Poke_CurrentAddr(u8 val) __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: V9_Poke16_CurrentAddr
// Write a 16-bits value to the previously setup write VRAM address.
// <V9_SetWriteAddress> have to be called first.
//
// Parameters:
//   val - The value to be write.
void V9_Poke16_CurrentAddr(u16 val) __PRESERVES(b, c, d, e, iyl, iyh);

// Function: V9_Peek_CurrentAddr
// Read a 8-bits value from the previously setup write VRAM address.
// <V9_SetReadAddress> have to be called first.
//
// Return:
//   The value at the given address.
u8 V9_Peek_CurrentAddr() __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: V9_Peek16_CurrentAddr
// Read a 16-bits value from the previously setup write VRAM address.
// <V9_SetReadAddress> have to be called first.
//
// Return:
//   The value at the given address.
u16 V9_Peek16_CurrentAddr() __PRESERVES(b, c, h, l, iyl, iyh);

// Function: V9_FillVRAM
// Fill VRAM with 8-bits value start to a given VRAM address.
//
// Parameters:
//   addr - Destinaton VRAM address.
//   value - 8-bits value to be set.
//   count - Size of the filled space in VRAM.
inline void V9_FillVRAM(u32 addr, u8 value, u16 count) { V9_SetWriteAddress(addr); V9_FillVRAM_CurrentAddr(value, count); }

// Function: V9_FillVRAM16
// Fill VRAM with 16-bits value start to the given VRAM address.
//
// Parameters:
//   addr - Destination VRAM address.
//   value - 16-bits value to be set.
//   count - Size of the filled space in VRAM (size in bytes is 'count * 2').
inline void V9_FillVRAM16(u32 addr, u16 value, u16 count) { V9_SetWriteAddress(addr); V9_FillVRAM16_CurrentAddr(value, count); }

// Function: V9_WriteVRAM
// Copy data from a RAM buffer to the given VRAM address.
//
// Parameters:
//   addr - Destination VRAM address.
//   src - Pointer to the source data buffer to be copied in VRAM.
//   count - Size of the data to be copied in VRAM.
inline void V9_WriteVRAM(u32 addr, const u8* src, u16 count) { V9_SetWriteAddress(addr); V9_WriteVRAM_CurrentAddr(src, count); }

// Function: V9_ReadVRAM
// Copy data from a given VRAM address to a RAM buffer.
//
// Parameters:
//   dst - Source VRAM address.
//   dst - Pointer to the destination data buffer.
//   count - Size of the data to be copied in VRAM.
inline void V9_ReadVRAM(u32 addr, const u8* dest, u16 count) { V9_SetReadAddress(addr); V9_ReadVRAM_CurrentAddr(dest, count); }

// Function: V9_Poke
// Write a 8-bits value to the given VRAM address.
//
// Parameters:
//   addr - Destination VRAM address.
//   val - The value to be write.
inline void V9_Poke(u32 addr, u8 val) { V9_SetWriteAddress(addr); V9_Poke_CurrentAddr(val); }

// Function: V9_Poke16
// Write a 16-bits value to the given VRAM address.
//
// Parameters:
//   addr - Destination VRAM address.
//   val - The value to be write.
inline void V9_Poke16(u32 addr, u16 val) { V9_SetWriteAddress(addr); V9_Poke16_CurrentAddr(val); }

// Function: V9_Peek
// Read a 8-bits value from at the given VRAM address.
//
// Parameters:
//   addr - Source VRAM address.
//
// Return:
//   The value at the given address.
inline u8 V9_Peek(u32 addr) { V9_SetReadAddress(addr); return V9_Peek_CurrentAddr(); }

// Function: V9_Peek16
// Read a 16-bits value from at the given VRAM address.
//
// Parameters:
//   addr - Source VRAM address.
//
// Return:
//   The value at the given address.
inline u16 V9_Peek16(u32 addr) { V9_SetReadAddress(addr); return V9_Peek16_CurrentAddr(); }

//-----------------------------------------------------------------------------
// Group: Setting
//-----------------------------------------------------------------------------

// Function: V9_SetScreenMode
// Set the current screen mode.
//
// Parameters:
//   mode - Screen mode ID (see <V9_SCREEN_MODE>).
void V9_SetScreenMode(u8 mode);

#if ((V9_USE_MODE_B0) || (V9_USE_MODE_B1) || (V9_USE_MODE_B2) || (V9_USE_MODE_B3) || (V9_USE_MODE_B4) || (V9_USE_MODE_B5) || (V9_USE_MODE_B6) || (V9_USE_MODE_B7))

// Function: V9_SetBPP
// Set bitmap bit-per-pixel value.
//
// Parameters:
//   bpp - New bits per pixel to set. Can be: <V9_R06_BPP_2>, <V9_R06_BPP_4>, <V9_R06_BPP_8> or <V9_R06_BPP_16>.
inline void V9_SetBPP(u8 bpp) { V9_SetFlag(6, V9_R06_BPP_MASK, bpp); }

// Function: V9_SetColorMode
// Set bitmap color mode.
//
// Parameters:
//   mode - New color mode to set. Can be any of the bitmap modes of <V9_COLOR_MODE> enum.
void V9_SetColorMode(u8 mode);

#endif

// Function: V9_GetBPP
// Get bit number per pixel.
//
// Return:
//   Current bits per pixel. Can be: <V9_R06_BPP_2>, <V9_R06_BPP_4>, <V9_R06_BPP_8> or <V9_R06_BPP_16>.
inline u8 V9_GetBPP() { return V9_GetRegister(6) & (~V9_R06_BPP_MASK); }

// Function: V9_SetImageSpaceWidth
// Set number of pixels in X direction of image space.
//
// Parameters:
//   width - New image space width to set. Can be: <V9_R06_WIDH_256>, <V9_R06_WIDH_512>, <V9_R06_WIDH_1024> or <V9_R06_WIDH_2048>.
inline void V9_SetImageSpaceWidth(u8 width) { V9_SetFlag(6, V9_R06_WIDH_MASK, width); }

// Function: V9_GetImageSpaceWidth
// Get number of pixels in X direction of image space.
//
// Return:
//   Current image space width. Can be: <V9_R06_WIDH_256>, <V9_R06_WIDH_512>, <V9_R06_WIDH_1024> or <V9_R06_WIDH_2048>.
inline u8 V9_GetImageSpaceWidth() { return V9_GetRegister(6) & (~V9_R06_WIDH_MASK); }

// Function: V9_SetDisplayEnable
// Enalbe or disable the screen display.
//
// Parameters:
//   enable - TRUE to enable or FALSE to disable.
inline void V9_SetDisplayEnable(bool enable) { V9_SetFlag(8, V9_R08_DISP_ON, enable ? V9_R08_DISP_ON : 0); }

// Function: V9_SetBackgroundColor
// Set background color.
//
// Parameters:
//   color - New background color.
inline void V9_SetBackgroundColor(u8 color) { V9_SetRegister(15, color); }

// Function: V9_GetBackgroundColor
// Get background color.
//
// Return:
//   Current background color.
inline u8 V9_GetBackgroundColor() { return V9_GetRegister(15); }

// Function: V9_SetAdjustOffset
// Adjustment of the display location on the screen.
//
// Parameters:
//   offset - Screen display position offset (MSB 4-bits: vertical offset, LSB 4-bits: horizontal offset).
inline void V9_SetAdjustOffset(u8 offset) { V9_SetRegister(16, offset); }

// Function: V9_SetAdjustOffsetXY
// Adjustment of the display location on the screen.
//
// Parameters:
//   x - Horizontal screen display position offset [-7:+8].
//   y - Vertical screen display position offset [-7:+8].
inline void V9_SetAdjustOffsetXY(i8 x, i8 y) { V9_SetAdjustOffset(((-x) & 0x0F) | (((-y) & 0x0F) << 4)); }

// Function: V9_SetLayerPriority
// Set layer priority for P1 mode.
//
// Parameters:
//   x - X coordinate where priority apply (2-bits value x 64 pixel).
//   y - Y coordinate where priority apply (2-bits value x 64 pixel).
inline void V9_SetLayerPriority(u8 x, u8 y) { V9_SetRegister(27, (y << 2) + x); }

//-----------------------------------------------------------------------------
// Group: Status
//-----------------------------------------------------------------------------

// Function: V9_GetStatus
// Get status port value.
//
// Return:
//   Value of the status port (include various flags about frame rendering events).
inline u8 V9_GetStatus() { return V9_GetPort(V9_P05); }

// Function: V9_IsVBlank
// Is vertical non-display period.
//
// Return:
//   FALSE if not in vertical non-display period.
inline bool V9_IsVBlank() { return V9_GetStatus() & V9_P05_VR; }

// Function: V9_IsHBlank
// Is horizontal non-display period.
//
// Return:
//   FALSE if not in horizontal non-display period.
inline bool V9_IsHBlank() { return V9_GetStatus() & V9_P05_HR; }

// Function: V9_IsCmdDataReady
// Check if command data transfer ready.
//
// Return:
//   FALSE if command data transfer not ready.
inline bool V9_IsCmdDataReady() { return V9_GetStatus() & V9_P05_TR; }

// Function: V9_IsCmdRunning
// Check if a command is in process.
//
// Return:
//   TRUE if command still running.
inline bool V9_IsCmdRunning() { return V9_GetStatus() & V9_P05_CE; }

// Function: V9_IsCmdComplete
// Check if a no command is in process.
//
// Return:
//   TRUE if command finished.
inline bool V9_IsCmdComplete() { return !V9_IsCmdRunning(); }

// Function: V9_IsSecondField
// Check if render is in the second field period during interlace.
//
// Return:
//   FALSE if not in the second field period during interlace.
inline bool V9_IsSecondField() { return V9_GetStatus() & V9_P05_E0; }

//-----------------------------------------------------------------------------
// Group: Interrupt
//-----------------------------------------------------------------------------

// Function: V9_SetInterrupt
// Set interruption flags.
//
// Parameters:
//   flags - Can be any combination of <V9_INT_VBLANK>, <V9_INT_HBLANK> or <V9_INT_CMDEND>.
inline void V9_SetInterrupt(u8 flags) { V9_SetRegister(9, flags); }

// Function: V9_DisableInterrupt
// Disable all interruptions.
inline void V9_DisableInterrupt() { V9_SetRegister(9, 0); }

// Function: V9_SetVBlankInterrupt
// Set vertical blank interruption.
//
// Parameters:
//   enable - TRUE to enable or FALSE to disable.
inline void V9_SetVBlankInterrupt(bool enable) { V9_SetFlag(9, V9_R08_IEV_ON, enable ? V9_R08_IEV_ON : 0); }

// Function: V9_SetHBlankInterrupt
// Set horizontal blank interruption.
//
// Parameters:
//   enable - TRUE to enable or FALSE to disable.
inline void V9_SetHBlankInterrupt(bool enable) { V9_SetFlag(9, V9_R08_IEH_ON, enable ? V9_R08_IEH_ON : 0); }

// Function: V9_SetCmdEndInterrupt
// Set command end interruption.
//
// Parameters:
//   enable - TRUE to enable or FALSE to disable.
inline void V9_SetCmdEndInterrupt(bool enable) { V9_SetFlag(9, V9_R08_IECE_ON, enable ? V9_R08_IECE_ON : 0); }

// Function: V9_SetInterruptLine
// Specification of vertical position where display position interrupt occurs.
//
// Parameters:
//   line - Line where h-blank interruption will occur (Specified by means of line No. with the display start line as "0").
inline void V9_SetInterruptLine(u16 line) { V9_SetRegister16(10, line); }

// Function: V9_SetInterruptEveryLine
// Set line interrupt on every line.
inline void V9_SetInterruptEveryLine() { V9_SetRegister(11, V9_R09_EVERYLINE); }

// Function: V9_SetInterruptX
// Specification of horizontal position where display position interrupt occurs.
//
// Parameters:
//   x - Horizontal position where h-blank interruption will occur (Specified by unit of 64 master clock with the display start position as "0").
inline void V9_SetInterruptX(u8 x) { V9_SetRegister(12, x); }

//-----------------------------------------------------------------------------
// Group: Scrolling
//-----------------------------------------------------------------------------

// Function: V9_SetScrollingX
// Set horizontal scrolling value.
//
// Parameters:
//   x - Scrolling value (between 0 and image space width-1).
void V9_SetScrollingX(u16 x);

// Function: V9_SetScrollingY
// Set vertical scrolling value.
//
// Parameters:
//   y - Scrolling value (between 0 and image space height-1).
void V9_SetScrollingY(u16 y);

// Function: V9_SetScrolling
// Set horizontal and vertical scrolling values.
//
// Parameters:
//   x - Scrolling value (between 0 and image space width-1).
//   y - Scrolling value (between 0 and image space height-1).
inline void V9_SetScrolling(u16 x, u16 y) { V9_SetScrollingX(x);  V9_SetScrollingY(y); }

#if (V9_USE_MODE_P1)

// Function: V9_SetScrollingBX
// Set horizontal scrolling value for layer B (P1 mode only).
//
// Parameters:
//   x - Scrolling value (between 0 and image space width-1).
void V9_SetScrollingBX(u16 x);

// Function: V9_SetScrollingBY
// Set vertical scrolling value for layer B (P1 mode only).
//
// Parameters:
//   y - Scrolling value (between 0 and image space height-1).
void V9_SetScrollingBY(u16 y);

// Function: V9_SetScrollingB
// Set horizontal and vertical scrolling values for layer B (P1 mode only).
//
// Parameters:
//   x - Scrolling value (between 0 and image space width-1).
//   y - Scrolling value (between 0 and image space height-1).
inline void V9_SetScrollingB(u16 x, u16 y) { V9_SetScrollingBX(x);  V9_SetScrollingBY(y); }

#endif // (V9_USE_MODE_P1)

//-----------------------------------------------------------------------------
// Group: Cursor
//-----------------------------------------------------------------------------

#if ((V9_USE_MODE_B0) || (V9_USE_MODE_B1) || (V9_USE_MODE_B2) || (V9_USE_MODE_B3) || (V9_USE_MODE_B4) || (V9_USE_MODE_B5) || (V9_USE_MODE_B6) || (V9_USE_MODE_B7))

// Function: V9_SetCursorEnable
// Enable/disable all cursor display.
//
// Parameters:
//   enable - TRUE to enable or FALSE to disable.
inline void V9_SetCursorEnable(bool enable) { V9_SetFlag(8, V9_R08_SPD_OFF, enable ? 0 : V9_R08_SPD_OFF); }

// Function: V9_SetCursorAttribute
// Set the given cursor atribute (for bitmap modes).
//
// Parameters:
//   id - Cursor index (0 or 1).
//   x - Cursor X coordinate (from 0 to 1023 regardless of the screen mode).
//   y - Cursor Y coordinate (from 0 to 511 regardless of the screen mode).
//   color - Cursor color (2 bits: 0 to 3).
void V9_SetCursorAttribute(u8 id, u16 x, u16 y, u8 color);

// Function: V9_SetCursorDisplay
// Display/hide the given cursor
//
// Parameters:
//   id - Cursor index (0 or 1).
//   enable - TRUE to display or FALSE to hide.
void V9_SetCursorDisplay(u8 id, bool enable);

// Function: V9_SetCursorPattern
// Set the given cursor pattern.
//
// Parameters:
//   id - Cursor index (0 or 1).
//   data - 128 bytes pattern buffer (32 x 32 binary data, 4 bytes per line).
inline void V9_SetCursorPattern(u8 id, const u8* data) { V9_WriteVRAM(0x7FF00 + (id * 0x80), data, 128); }

// Function: V9_SetCursorPalette
// Set cursor palette offset.
//
// Parameters:
//   offset - Palette offset (4 bits: 0 to 15).
inline void V9_SetCursorPalette(u8 offset) { V9_SetRegister(28, offset); }

#endif // ((V9_USE_MODE_B0) || (V9_USE_MODE_B1) || (V9_USE_MODE_B2) || (V9_USE_MODE_B3) || (V9_USE_MODE_B4) || (V9_USE_MODE_B5) || (V9_USE_MODE_B6) || (V9_USE_MODE_B7))

//-----------------------------------------------------------------------------
// Group: Sprite
//-----------------------------------------------------------------------------

#if ((V9_USE_MODE_P1) || (V9_USE_MODE_P2))

// Function: V9_SetSpriteEnable
// Enable/disable all sprites display.
//
// Parameters:
//   enable - TRUE to enable or FALSE to disable.
inline void V9_SetSpriteEnable(bool enable) { V9_SetFlag(8, V9_R08_SPD_OFF, enable ? 0 : V9_R08_SPD_OFF); }

// Function: V9_SetSpritePatternAddr
// Set sprite patterns VRAM address.
//
// Parameters:
//   addr - Sprite pattern table address in VRAM.
//     P1 mode: V9_P1_SGT_00000 to V9_P1_SGT_38000 with 8000 step
//     P2 mode: V9_P2_SGT_00000 to V9_P2_SGT_78000 with 8000 step
inline void V9_SetSpritePatternAddr(u8 addr) { V9_SetRegister(25, addr); }

// Function: V9_SetSpritePalette
// Set sprite palette offset.
//
// Parameters:
//   offset - Palette offset (4 bits: 0 to 15).
inline void V9_SetSpritePaletteOffset(u8 offset) { V9_SetRegister(28, offset); }

// Attribute table
//-----------------------------------------------------------------------------
// +0: Y-coordinate (Actual display position is one line below specified)
// +1: Pattern number
// +2: X-coordinate (bits 0:7)
// +3: Additionnal information
//
//	7	6	5	4	3	2	1	0
//	SC5	SC4	PR1	PR0	-	-	X9	X8
//  │	│	│	│	 	 	└───┴── X-coordinate (bits 8:9)
//	│	│	│	└────────────────── Disable flag (0: Enable, 1: Disable)
//	│	│	└────────────────────── Priority flag (0: Front, 1: Between layers)
//	└───┴────────────────────────── Color palette base (0-3)
#define V9_SPAT_X_MASK				0b00000011
#define V9_SPAT_DISABLE_MASK		0b00010000
#define V9_SPAT_PRIORITY_MASK		0b00100000
#define V9_SPAT_PALETTE_MASK		0b11000000

#define V9_SPAT_INFO_ENABLE			0
#define V9_SPAT_INFO_DISABLE		V9_SPAT_DISABLE_MASK
#define V9_SPAT_INFO_PRIO_FRONT		0
#define V9_SPAT_INFO_PRIO_BETWEEN	V9_SPAT_PRIORITY_MASK
#define V9_SPAT_INFO_PALETTE(p)		((p) << 6)

//.............................................................................
#if (V9_USE_MODE_P1)

// Function: V9_SetSpriteP1
// Set sprite attribute for P1 mode.
//
// Parameters:
//   id - Sprite index (0 to 124).
//   attr - Pointer to sprite structure.
inline void V9_SetSpriteP1(u8 id, const struct V9_Sprite* attr) { V9_WriteVRAM(V9_P1_SPAT + (id * 4), (const u8*)attr, 4); }

// Function: V9_SetSpritePatternP1
// Set sprite pattern for P1 mode.
//
// Parameters:
//   id - Sprite index (0 to 124).
//   pat - Pattern index (0 to 255).
inline void V9_SetSpritePatternP1(u8 id, u8 pat) { V9_Poke(V9_P1_SPAT + 1 + (id * 4), pat); }

// Function: V9_SetSpritePositionP1
// Set sprite position for P1 mode.
//
// Parameters:
//   id - Sprite index (0 to 124).
//   x - Sprite X coordinate.
//   y - Sprite Y coordinate.
inline void V9_SetSpritePositionP1(u8 id, u8 x, u8 y) { V9_Poke(V9_P1_SPAT + 0 + (id * 4), y); V9_Poke(V9_P1_SPAT + 2 + (id * 4), x); }

// Function: V9_SetSpriteInfoP1
// Set sprite additionnal information for P1 mode.
//
// Parameters:
//   id - Sprite index (0 to 124).
//   info - Sprite additionnal information.
// >	7	6	5	4	3	2	1	0
// >	SC5	SC4	PR1	PR0	-	-	X9	X8
// >	│	│	│	│	 	 	└───┴── X-coordinate (bits 8:9)
// >	│	│	│	└────────────────── Disable flag (0: Enable, 1: Disable)
// >	│	│	└────────────────────── Priority flag (0: Front, 1: Between layers)
// >	└───┴────────────────────────── Color palette base (0-3)
//          Can be any combination of:
//             <V9_SPAT_INFO_ENABLE> or <V9_SPAT_INFO_DISABLE>
//           + <V9_SPAT_INFO_PRIO_FRONT> or <V9_SPAT_INFO_PRIO_BETWEEN>
//           + <V9_SPAT_INFO_PALETTE>
inline void V9_SetSpriteInfoP1(u8 id, u8 info) { V9_Poke(V9_P1_SPAT + 3 + (id * 4), info); }

// Function: V9_SetSpriteEnableP1
// Set sprite enable flag for P1 mode.
//
// Parameters:
//   id - Sprite index (0 to 124).
//   enable - Is sprite display is enable?
inline void V9_SetSpriteEnableP1(u8 id, bool enable)
{
	u8 val = V9_Peek(V9_P1_SPAT + 3 + (id * 4));
	if (enable)
		val &= ~V9_SPAT_DISABLE_MASK;
	else
		val |= V9_SPAT_DISABLE_MASK;
	V9_Poke(V9_P1_SPAT + 3 + (id * 4), val); 
}

// Function: V9_SetSpriteDisableP1
// Set sprite disable flag for P1 mode.
//
// Parameters:
//   id - Sprite index (0 to 124).
//   disable - Is sprite display is disable?
inline void V9_SetSpriteDisableP1(u8 id, bool disable) { V9_SetSpriteEnableP1(id, !disable); }

// Function: V9_SetSpritePriorityP1
// Set sprite priority for P1 mode.
//
// Parameters:
//   id - Sprite index (0 to 124).
//   prio - Priority flag (0: Front, 1: Between layers).
inline void V9_SetSpritePriorityP1(u8 id, u8 prio)
{
	u8 val = V9_Peek(V9_P1_SPAT + 3 + (id * 4));
	if (prio)
		val &= ~V9_SPAT_PRIORITY_MASK;
	else
		val |= V9_SPAT_PRIORITY_MASK;
	V9_Poke(V9_P1_SPAT + 3 + (id * 4), val); 
}

// Function: V9_SetSpritePaletteP1
// Set sprite palette for P1 mode.
//
// Parameters:
//   id - Sprite index (0 to 124).
//   info - Sprite additionnal information.
inline void V9_SetSpritePaletteP1(u8 id, u8 pal)
{
	u8 val = V9_Peek(V9_P1_SPAT + 3 + (id * 4));
	val &= ~V9_SPAT_PALETTE_MASK;
	val |= pal << 6;
	V9_Poke(V9_P1_SPAT + 3 + (id * 4), val); 
}

#endif // (V9_USE_MODE_P1)

//.............................................................................
#if (V9_USE_MODE_P2)

// Function: V9_SetSpriteP2
// Set sprite attribute for P2 mode.
//
// Parameters:
//   id - Sprite index (0 to 124).
//   attr - Pointer to sprite structure.
inline void V9_SetSpriteP2(u8 id, const struct V9_Sprite* attr) { V9_WriteVRAM(V9_P2_SPAT + 4 * id, (const u8*)attr, 4); }

// Function: V9_SetSpritePatternP2
// Set sprite pattern for P2 mode.
//
// Parameters:
//   id - Sprite index (0 to 124).
//   pat - Pattern index (0 to 255).
inline void V9_SetSpritePatternP2(u8 id, u8 pat) { V9_Poke(V9_P2_SPAT + 4 * id + 1, pat); }

// Function: V9_SetSpritePositionP2
// Set sprite position for P2 mode.
//
// Parameters:
//   id - Sprite index (0 to 124).
//   x - Sprite X coordinate.
//   y - Sprite Y coordinate.
inline void V9_SetSpritePositionP2(u8 id, u8 x, u8 y) { V9_Poke(V9_P2_SPAT + (4 * id), y); V9_Poke(V9_P2_SPAT + (4 * id) + 2, x); }

// Function: V9_SetSpriteInfoP2
// Set sprite additionnal information for P2 mode.
//
// Parameters:
//   id - Sprite index (0 to 124).
//   info - Sprite additionnal information.
// >	7	6	5	4	3	2	1	0
// >	SC5	SC4	PR1	PR0	-	-	X9	X8
// >	│	│	│	│	 	 	└───┴── X-coordinate (bits 8:9)
// >	│	│	│	└────────────────── Disable flag (0: Enable, 1: Disable)
// >	│	│	└────────────────────── Priority flag (0: Front, 1: Between layers)
// >	└───┴────────────────────────── Color palette base (0-3)
//          Can be any combination of:
//             <V9_SPAT_INFO_ENABLE> or <V9_SPAT_INFO_DISABLE>
//           + <V9_SPAT_INFO_PRIO_FRONT> or <V9_SPAT_INFO_PRIO_BETWEEN>
//           + <V9_SPAT_INFO_PALETTE>
inline void V9_SetSpriteInfoP2(u8 id, u8 info) { V9_Poke(V9_P2_SPAT + 3 + (id * 4), info); }

// Function: V9_SetSpriteEnableP2
// Set sprite enable flag for P2 mode.
//
// Parameters:
//   id - Sprite index (0 to 124).
//   enable - Is sprite display is enable?
inline void V9_SetSpriteEnableP2(u8 id, bool enable)
{
	u8 val = V9_Peek(V9_P2_SPAT + 3 + (id * 4));
	if (enable)
		val &= ~V9_SPAT_DISABLE_MASK;
	else
		val |= V9_SPAT_DISABLE_MASK;
	V9_Poke(V9_P2_SPAT + 3 + (id * 4), val); 
}

// Function: V9_SetSpriteDisableP2
// Set sprite disable flag for P2 mode.
//
// Parameters:
//   id - Sprite index (0 to 124).
//   disable - Is sprite display is disable?
inline void V9_SetSpriteDisableP2(u8 id, bool disable) { V9_SetSpriteEnableP2(id, !disable); }

// Function: V9_SetSpritePriorityP2
// Set sprite priority for P2 mode.
//
// Parameters:
//   id - Sprite index (0 to 124).
//   prio - Priority flag (0: Front, 1: Between layers).
inline void V9_SetSpritePriorityP2(u8 id, u8 prio)
{
	u8 val = V9_Peek(V9_P2_SPAT + 3 + (id * 4));
	if (prio)
		val &= ~V9_SPAT_PRIORITY_MASK;
	else
		val |= V9_SPAT_PRIORITY_MASK;
	V9_Poke(V9_P2_SPAT + 3 + (id * 4), val); 
}

// Function: V9_SetSpritePaletteP2
// Set sprite palette for P2 mode.
//
// Parameters:
//   id - Sprite index (0 to 124).
//   info - Sprite additionnal information.
inline void V9_SetSpritePaletteP2(u8 id, u8 pal)
{
	u8 val = V9_Peek(V9_P2_SPAT + 3 + (id * 4));
	val &= ~V9_SPAT_PALETTE_MASK;
	val |= pal << 6;
	V9_Poke(V9_P2_SPAT + 3 + (id * 4), val); 
}

#endif // (V9_USE_MODE_P2)

#endif // ((V9_USE_MODE_P1 || V9_USE_MODE_P2))

//-----------------------------------------------------------------------------
// Group: Palette
//-----------------------------------------------------------------------------

#if ((V9_PALETTE_MODE == V9_PALETTE_GBR_16) || (V9_PALETTE_MODE == V9_PALETTE_YSGBR_16))

// Function: V9_SetPaletteEntry
// Set the color of a given palette entry.
//
// Parameters:
//   index - Index of the palette entry (0-63).
//   color - 16 bits color value.
//           Format: [Ys:1|green:5|red:5|blue:5]
void V9_SetPaletteEntry(u8 index, u16 color) __PRESERVES(h, l, iyl, iyh);

// Function: V9_SetPalette
// Set the colors of a given palette entries.
//
// Parameters:
//   first - Index of the first entry to set (0-63).
//   num   - Numer of entries to set (1-64).
//   table - 16 bits color table.
//           Format: [Ys:1|green:5|red:5|blue:5]
inline void V9_SetPalette(u8 first, u8 num, const u16* table) { for(u8 i = 0; i < num; ++i) V9_SetPaletteEntry(first++, *(table++)); }

// Function: V9_SetPaletteAll
// Set the colors of all the palette 64 entries.
//
// Parameters:
//   table - 16 bits color table.
//           Format: [Ys:1|green:5|red:5|blue:5]
inline void V9_SetPaletteAll(const u16* table) { V9_SetPalette(0, 64, table); }

#elif (V9_PALETTE_MODE == V9_PALETTE_RGB_24)

// Set the color of a given palette entry.
//
// Parameters:
//   index - Index of the palette entry (0-63).
//   table - 24 bits color table.
//           Format: [x:3|red:5] [x:3|green:5] [x:3|blue:5]
void V9_SetPaletteEntry(u8 index, const u8* color);

// Set the colors of a given palette entries.
//
// Parameters:
//   first - Index of the first entry to set (0-63).
//   num   - Numer of entries to set (1-64).
//   table - 24 bits color table.
//           Format: [x:3|red:5] [x:3|green:5] [x:3|blue:5]
inline void V9_SetPalette(u8 first, u8 num, const u8* table) { for(u8 i = 0; i < num; ++i) { V9_SetPaletteEntry(first++, table); table += 3; } }

// Set the colors of all the palette 64 entries.
//
// Parameters:
//   table - 24 bits color table.
//           Format: [x:3|red:5] [x:3|green:5] [x:3|blue:5]
inline void V9_SetPaletteAll(const u8* table) { V9_SetPalette(0, 64, table); }

#endif

#if (V9_USE_MODE_P1)

// Function: V9_SelectPaletteP1
// Set the P1 mode palette offset.
//
// Parameters:
//   a - Layer A palette index (2 bits).
//   b - Layer B palette index (2 bits).
inline void V9_SelectPaletteP1(u8 a, u8 b) { V9_SetRegister(13, ((b & 0x3) << 2) + (a & 0x3)); }

#endif

#if (V9_USE_MODE_P2)

// Function: V9_SelectPaletteP2
// Set the P2 mode palette offset.
//
// Parameters:
//   a - Palette index (2 bits).
inline void V9_SelectPaletteP2(u8 a) { V9_SetRegister(13, ((a & 0x3) << 2) + (a & 0x3)); }

#endif

#if ((V9_USE_MODE_B0) || (V9_USE_MODE_B1) || (V9_USE_MODE_B2) || (V9_USE_MODE_B3) || (V9_USE_MODE_B4) || (V9_USE_MODE_B5) || (V9_USE_MODE_B6) || (V9_USE_MODE_B7))

// Function: V9_SelectPaletteBP2
// Set the BP2 mode palette offset (R#13 PLTO5-2).
//
// Parameters:
//   a - Palette index (4 bits).
inline void V9_SelectPaletteBP2(u8 a) { V9_SetRegister(13, a & 0x7); }

// Function: V9_SelectPaletteBP4
// Set the BP4 mode palette offset (R#13 PLTO5-4).
//
// Parameters:
//   a - Palette index (2 bits).
inline void V9_SelectPaletteBP4(u8 a) { V9_SetRegister(13, (a & 0x3) << 2); }

#endif

//-----------------------------------------------------------------------------
// Group: Command helper
//-----------------------------------------------------------------------------

// Function: V9_SetCommandSX
// Set source X-coordinate.
//
// Parameters:
//   sx - Specified with the X-coordinate of the image coordinate space (by pixels) and rolling to "0" occurs when a larger value than the image size is used for setting. (When in P1 mode, layer "A" is selected at SX9=0 and layer "B" at SX9=1.)
inline void V9_SetCommandSX(u16 sx) { V9_SetRegister16(32, sx); }

// Function: V9_SetCommandSY
// Set source Y-coordinate.
//
// Parameters:
//   sy - Specified with the Y-coordinate of the image coordinate space (by pixels) and rolling to "0" occurs when a larger value than the image size is used for setting.
inline void V9_SetCommandSY(u16 sy) { V9_SetRegister16(34, sy); }

// Function: V9_SetCommandSA
// Set source VRAM address.
//
// Parameters:
//   sa - Specified with the address on the VRAM map (by bytes).
inline void V9_SetCommandSA(u32 sa) { V9_SetRegister(32, sa & 0xFF); V9_SetRegister(34, (sa >> 8) & 0xFF); V9_SetRegister(35, (sa >> 16) & 0xFF); }

// Function: V9_SetCommandDX
// Set destination X-coordinate.
//
// Parameters:
//   dx - Specified with the X-coordinate of the image coordinate space (by pixels) and rolling to "0" occurs when a larger value than the image size is used for setting. (When in P1 mode, layer "A" is selected at DX9=0 and layer "B" at DX9=1.)
inline void V9_SetCommandDX(u16 dx) { V9_SetRegister16(36, dx); }

// Function: V9_SetCommandDY
// Set destination Y-coordinate.
//
// Parameters:
//   dy - Specified with the Y-coordinate of the image coordinate space (by pixels) and rolling to "0" occurs when a larger value than the image size is used for setting.
inline void V9_SetCommandDY(u16 dy) { V9_SetRegister16(38, dy); }

// Function: V9_SetCommandDA
// Set destination VRAM address.
//
// Parameters:
//   da - Specified with the address on the VRAM map (by bytes).
inline void V9_SetCommandDA(u32 da) { V9_SetRegister(36, da & 0xFF); V9_SetRegister(38, (da >> 8) & 0xFF); V9_SetRegister(39, (da >> 16) & 0xFF); }

// Function: V9_SetCommandNX
// Set the number of pixels in X direction.
//
// Parameters:
//   nx - The number of pixels in X direction of the image coordinate space is specified and rolling to "0" occurs when the specified range exceeds the image size. (Maximum value is 2048 pixels when all "0".)
inline void V9_SetCommandNX(u16 nx) { V9_SetRegister16(40, nx); }

// Function: V9_SetCommandNY
// Set the number of pixels in Y direction.
//
// Parameters:
//   ny - The number of pixels in Y direction of the image coordinate space is specified and rolling to "0" occurs when the specified range exceeds the image size. (Maximum value is 4096 pixels when all "0".)
inline void V9_SetCommandNY(u16 ny) { V9_SetRegister16(42, ny); }

// Function: V9_SetCommandMJ
// Set the length of the longer side of the rectangle.
//
// Parameters:
//   mj - The length of the longer side of the rectangle whose diagonal line is the same as the drawn line is specified by pixels. When the specified range exceeds the image size, rolling to "0" occurs.
inline void V9_SetCommandMJ(u16 mj) { V9_SetRegister16(40, mj); }

// Function: V9_SetCommandMI
// Set the length of the shorter side of the rectangle.
//
// Parameters:
//   mi - The length of the shorter side of the rectangle whose diagonal line is the same as the drawn line is specified by pixels. When the specified range exceeds the image size, rolling to "0" occurs.
inline void V9_SetCommandMI(u16 mi) { V9_SetRegister16(42, mi); }

// Function: V9_SetCommandNA
// Set the number of bytes.
//
// Parameters:
//   na - The number of bytes on the VRAM map is specified and rolling to address "0" occurs when the specified range exceeds the VRAM capacity. (Maximum value is 512K bytes when all "0".)
//        Note) 256 bytes when NA bit 7-0 are all "0".
inline void V9_SetCommandNA(u32 na) { V9_SetRegister(40, na & 0xFF); V9_SetRegister(42, (na >> 8) & 0xFF); V9_SetRegister(43, (na >> 16) & 0xFF); }

// Function: V9_SetCommandArgument
// Set command argument.
//
// Parameters:
//   arg - Argument.
//	7	6	5	4	3	2	1	0
//	0	0	0	0	DIY	DIX	NEQ	MAJ
//					│	│	│	└── In the line inclination specification for LINE, X is the longer side and Y the shorter side at "0" while Y is the longer side and X the shorter side at "1".
//					│	│	└────── In the border color specification for SEARCH, "0" is for specified color detection and "1" is for non-specified color detection.
//					└───┴────────── The transfer direction (plus direction at "0" and minus direction at "1") from the base coordinates is specified. With BMXL and BMLX, however, the linear address specified side is fixed to "plus" and with BMLL, both X and Y are specified to the same direction.
inline void V9_SetCommandArgument(u8 arg) { V9_SetRegister(44, arg); }

// Function: V9_SetCommandLogicalOp
// Set command logical operation.
//
// Parameters:
//   lop - Logical operation.
//	7	6	5	4	3	2	1	0
//	0	0	0	TP	L11	L10	L01	L00
//				│	│	│	│	└── WC value for the bit with SC=0 and DC=0.
//				│	│	│	└────── WC value for the bit with SC=0 and DC=1.
//				│	│	└────────── WC value for the bit with SC=1 and DC=0.
//				│	└────────────── WC value for the bit with SC=1 and DC=1.
//				└────────────────── When this bit is "1", the data with SC as all "0" (by pixel for the X/Y-coordinates and by byte for the linear address) is not transferred.
inline void V9_SetCommandLogicalOp(u8 lop) { V9_SetRegister(45, lop); }

// Function: V9_SetCommandWriteMask
// Set command write mask.
//
// Parameters:
//   wm - Write mask.
inline void V9_SetCommandWriteMask(u16 wm) { V9_SetRegister16(46, wm); }

// Function: V9_SetCommandFC
// Set color code of font data "1".
//
// Parameters:
//   fc - Forground color.
inline void V9_SetCommandFC(u16 fc) { V9_SetRegister16(48, fc); }

// Function: V9_SetCommandBC
// Set color code of font data "0".
//
// Parameters:
//   bc - Background color.
inline void V9_SetCommandBC(u16 bc) { V9_SetRegister16(50, bc); }

// Function: V9_ExecCommand
// Set operation code.
//
// Parameters:
//   op - Operation code.
//        V9_CMD_STOP, V9_CMD_LMMC, V9_CMD_LMMV, V9_CMD_LMCM, V9_CMD_LMMM, V9_CMD_CMMC, V9_CMD_CMMM,
//        V9_CMD_BMXL, V9_CMD_BMLX, V9_CMD_BMLL, V9_CMD_LINE, V9_CMD_SEARCH, V9_CMD_POINT, V9_CMD_PSET, V9_CMD_ADVANCE
inline void V9_ExecCommand(u8 op) { V9_SetRegister(52, op); }

// Function: GetCommandBX
// Set search command border coordinate.
//
// Return:
//   Command border coordinate.
inline u16 GetCommandBX() { return V9_GetRegister(53) + (V9_GetRegister(54) << 8); }

//-----------------------------------------------------------------------------
// Group: Command
//-----------------------------------------------------------------------------

// Function: V9_CommandSTOP
// Command being executed is stopped.
inline void V9_CommandSTOP() { V9_ExecCommand(V9_CMD_STOP); }

// Function: V9_CommandLMMC
// Data is transferred from CPU to VRAM rectangle area.
// Note: Infinished function; don't use it.
//
// Parameters:
//   src - Source buffer in RAM.
//   dx - Destination X-coordinate.
//   dy - Destination Y-coordinate.
//   nx - Number of pixels in X direction.
//   ny - Number of pixels in Y direction.
//   arg - Command argument.
// inline void V9_CommandLMMC(const u8* src, u16 dx, u16 dy, u16 nx, u16 ny, u8 arg) { V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_SetCommandNX(nx); V9_SetCommandNY(ny); V9_SetCommandArgument(arg); V9_ExecCommand(V9_CMD_LMMC); }

// Function: V9_CommandLMMV
// VRAM rectangle area is painted out.
//
// Parameters:
//   dx - Destination X-coordinate.
//   dy - Destination Y-coordinate.
//   nx - Number of pixels in X direction.
//   ny - Number of pixels in Y direction.
//   arg - Command argument.
//   fc - Forground color.
inline void V9_CommandLMMV(u16 dx, u16 dy, u16 nx, u16 ny, u8 arg, u16 fc) { V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_SetCommandNX(nx); V9_SetCommandNY(ny); V9_SetCommandArgument(arg); V9_SetCommandFC(fc); V9_ExecCommand(V9_CMD_LMMV); }

// Function: V9_CommandLMCM
// VRAM rectangle area data is transferred to CPU.
// Note: Infinished function; don't use it.
//
// Parameters:
//   sx - Source X-coordinate.
//   sy - Source Y-coordinate.
//   dx - Destination X-coordinate.
//   dy - Destination Y-coordinate.
//   nx - Number of pixels in X direction.
//   ny - Number of pixels in Y direction.
//   arg - Command argument.
//   dest - Destination buffer in RAM.
// inline void V9_CommandLMCM(u16 sx, u16 sy, u16 nx, u16 ny, u8 arg, u8* dest) { V9_SetCommandSX(sx); V9_SetCommandSY(sy); V9_SetCommandNX(nx); V9_SetCommandNY(ny); V9_SetCommandArgument(arg); V9_ExecCommand(V9_CMD_LMCM); }

// Function: V9_CommandLMMM
// Rectangle area data is transferred from VRAM to VRAM.
//
// Parameters:
//   sx - Source X-coordinate.
//   sy - Source Y-coordinate.
//   dx - Destination X-coordinate.
//   dy - Destination Y-coordinate.
//   nx - Number of pixels in X direction.
//   ny - Number of pixels in Y direction.
//   arg - Command argument.
inline void V9_CommandLMMM(u16 sx, u16 sy, u16 dx, u16 dy, u16 nx, u16 ny, u8 arg) { V9_SetCommandSX(sx); V9_SetCommandSY(sy); V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_SetCommandNX(nx); V9_SetCommandNY(ny); V9_SetCommandArgument(arg); V9_ExecCommand(V9_CMD_LMMM); }

// Function: V9_CommandCMMC
// CPU character data is color-developed and transferred to VRAM rectangle area.
//
// Parameters:
//   dx - Destination X-coordinate.
//   dy - Destination Y-coordinate.
//   nx - Number of pixels in X direction.
//   ny - Number of pixels in Y direction.
//   arg - Command argument.
//   fc - Forground color.
//   bc - Background color.
inline void V9_CommandCMMC(u16 dx, u16 dy, u16 nx, u16 ny, u8 arg, u16 fc, u16 bc) { V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_SetCommandNX(nx); V9_SetCommandNY(ny); V9_SetCommandArgument(arg); V9_SetCommandFC(fc); V9_SetCommandBC(bc); V9_ExecCommand(V9_CMD_CMMC); }

// Function: V9_CommandCMMM
// VRAM character data is color-developed and transferred to VRAM rectangle area.
//
// Parameters:
//   sa - Source address.
//   dx - Destination X-coordinate.
//   dy - Destination Y-coordinate.
//   nx - Number of pixels in X direction.
//   ny - Number of pixels in Y direction.
//   arg - Command argument.
//   fc - Forground color.
//   bc - Background color.
inline void V9_CommandCMMM(u32 sa, u16 dx, u16 dy, u16 nx, u16 ny, u8 arg, u16 fc, u16 bc) { V9_SetCommandSA(sa); V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_SetCommandNX(nx); V9_SetCommandNY(ny); V9_SetCommandArgument(arg); V9_SetCommandFC(fc); V9_SetCommandBC(bc); V9_ExecCommand(V9_CMD_CMMM); }

// Function: V9_CommandBMXL
// Data on VRAM linear address is transferred to VRAM rectangle area.
//
// Parameters:
//   sa - Source address.
//   dx - Destination X-coordinate.
//   dy - Destination Y-coordinate.
//   nx - Number of pixels in X direction.
//   ny - Number of pixels in Y direction.
//   arg - Command argument.
inline void V9_CommandBMXL(u32 sa, u16 dx, u16 dy, u16 nx, u16 ny, u8 arg) { V9_SetCommandSA(sa); V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_SetCommandNX(nx); V9_SetCommandNY(ny); V9_SetCommandArgument(arg); V9_ExecCommand(V9_CMD_BMXL); }

// Function: V9_CommandBMLX
// VRAM rectangle area data is transferred onto VRAM linear address.
//
// Parameters:
//   sx - Source X-coordinate.
//   sy - Source Y-coordinate.
//   da - Destination address.
//   nx - Number of pixels in X direction.
//   ny - Number of pixels in Y direction.
//   arg - Command argument.
inline void V9_CommandBMLX(u16 sx, u16 sy, u32 da, u16 nx, u16 ny, u8 arg) { V9_SetCommandSX(sx); V9_SetCommandSY(sy); V9_SetCommandDA(da); V9_SetCommandNX(nx); V9_SetCommandNY(ny); V9_SetCommandArgument(arg); V9_ExecCommand(V9_CMD_BMLX); }

// Function: V9_CommandBMLL
// Data on VRAM linear address is transferred onto VRAM linear address.
//
// Parameters:
//   sa - Source address.
//   da - Destination address.
//   na - Number of bytes.
//   arg - Command argument.
inline void V9_CommandBMLL(u32 sa, u32 da, u32 na, u8 arg) { V9_SetCommandSA(sa); V9_SetCommandDA(da); V9_SetCommandNA(na); V9_SetCommandArgument(arg); V9_ExecCommand(V9_CMD_BMLL); }

// Function: V9_CommandLINE
// Straight line is drawn on X/Y-coordinates.
//
// Parameters:
//   dx - Destination X-coordinate.
//   dy - Destination Y-coordinate.
//   mj - Shorter side of the rectangle.
//   mi - Longer side of the rectangle.
//   arg - Command argument.
//   fc - Forground color.
inline void V9_CommandLINE(u16 dx, u16 dy, u16 mj, u16 mi, u8 arg, u16 fc) { V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_SetCommandMJ(mj); V9_SetCommandMI(mi); V9_SetCommandArgument(arg); V9_SetCommandFC(fc); V9_ExecCommand(V9_CMD_LINE); }

// Function: V9_CommandSEARCH
// Border color coordinates on X/Y space are detected.
//
// Parameters:
//   sx - Source X-coordinate.
//   sy - Source Y-coordinate.
//   arg - Command argument.
//   fc - Forground color.
inline void V9_CommandSEARCH(u16 sx, u16 sy, u8 arg, u16 fc) { V9_SetCommandSX(sx); V9_SetCommandSY(sy); V9_SetCommandArgument(arg); V9_SetCommandFC(fc); V9_ExecCommand(V9_CMD_SEARCH); }

// Function: V9_CommandPOINT
// Color code of specified point on X/Y-coordinates is read out.
//
// Parameters:
//   sx - Source X-coordinate.
//   sy - Source Y-coordinate.
inline void V9_CommandPOINT(u16 sx, u16 sy) { V9_SetCommandSX(sx); V9_SetCommandSY(sy); V9_ExecCommand(V9_CMD_POINT); }

// Function: V9_CommandPSET
// Drawing is executed at drawing point on X/Y-coordinates.
//
// Parameters:
//   dx - Destination X-coordinate.
//   dy - Destination Y-coordinate.
//   fc - Forground color.
//   shift - Pointer shift.
inline void V9_CommandPSET(u16 dx, u16 dy, u16 fc, u8 shift) { V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_SetCommandFC(fc); V9_ExecCommand(V9_CMD_PSET | shift); }

// Function: V9_CommandADVANCE
// Drawing point on X/Y-coordinates is shifted.
//
// Parameters:
//   dx - Destination X-coordinate.
//   dy - Destination Y-coordinate.
//   shift - Pointer shift.
inline void V9_CommandADVANCE(u16 dx, u16 dy, u8 shift) { V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_ExecCommand(V9_CMD_ADVANCE | shift); }

//-----------------------------------------------------------------------------
// Group: Helper
//-----------------------------------------------------------------------------

// Function: V9_Detect
// Detecting the presence of a V9990 card
//
// Return:
//   TRUE if V9990 video chip is detected.
bool V9_Detect();

// Function: V9_ClearVRAM
// Clear the whole 512 KB of VRAM with zero
void V9_ClearVRAM() __PRESERVES(d, e, h, l, iyl, iyh);

// Function: V9_WaitCmdEnd
// Wait for current command completion
inline void V9_WaitCmdEnd() { while(V9_IsCmdRunning()) {} }

#if (V9_USE_MODE_P1)

// Function: V9_CellAddrP1A
// Get address fo the given X/Y cell coordiante for player A of P1 mode.
//
// Parameters:
//   x - Cell X-coordinate.
//   y - Cell Y-coordinate.
//
// Return:
//   VRAM address of the given cell.
inline u32 V9_CellAddrP1A(u8 x, u8 y) { return V9_P1_PNT_A + (((64 * y) + x) * 2); }

// Function: V9_CellAddrP1B
// Get address fo the given X/Y cell coordiante for player B of P1 mode.
//
// Parameters:
//   x - Cell X-coordinate.
//   y - Cell Y-coordinate.
//
// Return:
//   VRAM address of the given cell.
inline u32 V9_CellAddrP1B(u8 x, u8 y) { return V9_P1_PNT_B + (((64 * y) + x) * 2); }

#endif // (V9_USE_MODE_P1)

#if (V9_USE_MODE_P2)

// Function: V9_CellAddrP2
// Get address fo the given X/Y cell coordiante for P2 mode.
//
// Parameters:
//   x - Cell X-coordinate.
//   y - Cell Y-coordinate.
//
// Return:
//   VRAM address of the given cell.
inline u32 V9_CellAddrP2(u8 x, u8 y) { return V9_P2_PNT + (((128 * y) + x) * 2); }

#endif