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

#define V9_INT_PROTECT				1

// Palette input data format
#define V9_PALETTE_YSGBR_16			0	// [Ys|G|G|G|G|G|R|R] [R|R|R|B|B|B|B|B]
#define V9_PALETTE_GBR_16			1	// [0|G|G|G|G|G|R|R] [R|R|R|B|B|B|B|B]
#define V9_PALETTE_RGB_24			2	// [0|0|0|R|R|R|R|R] [0|0|0|G|G|G|G|G] [0|0|0|B|B|B|B|B]

// Palette data format (can be YsGRB 16-bits or RGB 24-bits)
#define V9_PALETTE_MODE				V9_PALETTE_RGB_24

// RGB color definition macro
#define V9_RGB(r, g, b)				(u16)(((u16)((g) & 0x1F) << 10) + (((r) & 0x1F) << 5) + ((b) & 0x1F))
#define V9_RGBYs(r, g, b, ys)		(u16)((((ys) & 0x01) << 15) + ((u16)((g) & 0x1F) << 10) + (((r) & 0x1F) << 5) + ((b) & 0x1F))

// V9990 address structure
struct V9_Address
{
	u8 Lower;
	u8 Center;
	u8 Upper;
};

//
extern struct V9_Address g_V9_Address;

//
#define V9_REG(n)					n
#define V9_RII						0b01000000
#define V9_WII						0b10000000

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
};

// Color modes
enum V9_COLOR_MODE
{	//                 R#6  R#13
	//                 CLRM PLTM YAE
	// 2 bits per pixel
	V9_COLOR_BP2,	// 0    0    0	Color palette (4 colors out of 32768 colors)
	// 4 bits per pixel
	V9_COLOR_BP4,	// 1    0    0	Color palette (16 colors out of 32768 colors)
	V9_COLOR_PP,	// 				Display type when the display mode is P1 or P2
	// 8 bits per pixel
	V9_COLOR_BP6,	// 2    0    0	Color palette (64 colors out of 32768 colors)
	V9_COLOR_BD8,	// 2    1    0	Direct RGB [G:3|R:3|B:2] (256 colors)
	V9_COLOR_BYJK,	// 2    2    0	YJK Decoder (19268 colors)
	V9_COLOR_BYJKP,	// 2    2    1	YJK Decoder + Color palette (12599 colors + 16 colors out of 32768 colors)
	V9_COLOR_BYUV,	// 2    3    0	YUV Decoder (19268 colors)
	V9_COLOR_BYUVP,	// 2    3    1	YUV Decoder + Color palette (12599 colors + 16 colors out of 32768 colors)
	// 16 bits per pixel
	V9_COLOR_BD16,	// 3    0    0	Direct RGB [YS|G:5|R:2][R:3|B:5] (32768 colors)
};

#define V9_R06_COLOR_MASK			0b00000011
#define V9_R13_COLOR_MASK			0b11100000

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

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Core
//-----------------------------------------------------------------------------

// Function: V9_SetPort
// Set port value
//
// Parameters:
//   port - Port address (V9990 is plug to ports 60h~6Fh)
//   value - Value to write to the port
void V9_SetPort(u8 port, u8 value) __PRESERVES(b, d, e, h, iyl, iyh);

// Function: V9_GetPort
// Get port value
//
// Parameters:
//   port - Port address (V9990 is plug to ports 60h~6Fh)
//
// Return:
//   Value read from the givem port
u8 V9_GetPort(u8 port) __PRESERVES(b, d, e, h, l, iyl, iyh);

// Function: V9_SetRegister
// Set register value
// This function as no effect if register is read-only
//
// Parameters:
//   reg - V9990 register number
//   val - Value to write to the register
void V9_SetRegister(u8 reg, u8 val) __PRESERVES(b, c, d, e, h, iyl, iyh);

// Function: V9_SetRegister16
// Set register value
// This function as no effect if register is read-only
//
// Parameters:
//   reg - V9990 register number (LSB will be write to this register and MSB to the next)
//   val - 16-bits value to write to the registers
void V9_SetRegister16(u8 reg, u16 val) __PRESERVES(b, h, l, iyl, iyh);

// Function: V9_GetRegister
// Get register value
//
// Parameters:
//   reg - V9990 register number
//
// Return:
//   Value read from the register (FFh is returned if register is write only)
u8 V9_GetRegister(u8 reg) __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: V9_SetFlag
// Helper function to change some bits in a given register
// The function get a register value, apply a mask, 'or' the new value, than set the register
//
// Parameters:
//   reg - V9990 register number
//   mask - Bits to be cleared
//   flag - Bits to be set
inline void V9_SetFlag(u8 reg, u8 mask, u8 flag) { V9_SetRegister(reg, V9_GetRegister(reg) & (~mask) | flag); }

//-----------------------------------------------------------------------------
// Group: VRAM access
//-----------------------------------------------------------------------------

// Function: V9_SetWriteAddress
// Initialize VRAM address for writing
//
// Parameters:
//   addr - Address to be setup for write access
void V9_SetWriteAddress(u32 addr) __PRESERVES(b, iyl, iyh);

// Function: V9_SetReadAddress
// Initialize VRAM address for reading
//
// Parameters:
//   addr - Address to be setup for read access
void V9_SetReadAddress(u32 addr) __PRESERVES(b, iyl, iyh);

// Function: V9_FillVRAM_CurrentAddr
// Fill VRAM with 8-bits value from the previously setup write VRAM address
//
// Parameters:
//   value - 8-bits value to be set
//   count - Size of the filled space in VRAM
void V9_FillVRAM_CurrentAddr(u8 value, u16 count);

// Function: V9_FillVRAM16_CurrentAddr
// Fill VRAM with 16-bits value from the previously setup write VRAM address
//
// Parameters:
//   value - 16-bits value to be set
//   count - Size of the filled space in VRAM (size in bytes is 'count * 2')
void V9_FillVRAM16_CurrentAddr(u16 value, u16 count);

// Function: V9_WriteVRAM_CurrentAddr
// Copy data from RAM buffer to the previously setup write VRAM address
//
// Parameters:
//   src - Pointer to the source data buffer to be copied in VRAM
//   count - Size of the data to be copied in VRAM
void V9_WriteVRAM_CurrentAddr(const u8* src, u16 count);

// Function: V9_ReadVRAM_CurrentAddr
// Copy data from the previously setup read VRAM address to a RAM buffer
//
// Parameters:
//   dst - Pointer to the destination data buffer
//   count - Size of the data to be copied in VRAM
void V9_ReadVRAM_CurrentAddr(const u8* dest, u16 count);

// Function: V9_Poke_CurrentAddr
// Write a 8-bits value to the previously setup write VRAM address
//
// Parameters:
//   val - The value to be write
void V9_Poke_CurrentAddr(u8 val) __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: V9_Poke16_CurrentAddr
// Write a 16-bits value to the previously setup write VRAM address
//
// Parameters:
//   val - The value to be write
void V9_Poke16_CurrentAddr(u16 val) __PRESERVES(b, c, d, e, iyl, iyh);

// Function: V9_Peek_CurrentAddr
// Read a 8-bits value from the previously setup write VRAM address
//
// Return:
//   The value at the given address
u8 V9_Peek_CurrentAddr() __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: V9_Peek16_CurrentAddr
// Read a 16-bits value from the previously setup write VRAM address
//
// Return:
//   The value at the given address
u16 V9_Peek16_CurrentAddr() __PRESERVES(b, c, h, l, iyl, iyh);

// Function: V9_FillVRAM
// Fill VRAM with 8-bits value start to a given VRAM address
//
// Parameters:
//   addr - Destinaton VRAM address
//   value - 8-bits value to be set
//   count - Size of the filled space in VRAM
inline void V9_FillVRAM(u32 addr, u8 value, u16 count) { V9_SetWriteAddress(addr); V9_FillVRAM_CurrentAddr(value, count); }

// Function: V9_FillVRAM16
// Fill VRAM with 16-bits value start to the given VRAM address
//
// Parameters:
//   addr - Destination VRAM address
//   value - 16-bits value to be set
//   count - Size of the filled space in VRAM (size in bytes is 'count * 2')
inline void V9_FillVRAM16(u32 addr, u16 value, u16 count) { V9_SetWriteAddress(addr); V9_FillVRAM16_CurrentAddr(value, count); }

// Function: V9_WriteVRAM
// Copy data from a RAM buffer to the given VRAM address
//
// Parameters:
//   addr - Destination VRAM address
//   src - Pointer to the source data buffer to be copied in VRAM
//   count - Size of the data to be copied in VRAM
inline void V9_WriteVRAM(u32 addr, const u8* src, u16 count) { V9_SetWriteAddress(addr); V9_WriteVRAM_CurrentAddr(src, count); }

// Function: V9_ReadVRAM
// Copy data from a given VRAM address to a RAM buffer
//
// Parameters:
//   dst - Source VRAM address
//   dst - Pointer to the destination data buffer
//   count - Size of the data to be copied in VRAM
inline void V9_ReadVRAM(u32 addr, const u8* dest, u16 count) { V9_SetReadAddress(addr); V9_ReadVRAM_CurrentAddr(dest, count); }

// Function: V9_Poke
// Write a 8-bits value to the given VRAM address
//
// Parameters:
//   addr - Destination VRAM address
//   val - The value to be write
inline void V9_Poke(u32 addr, u8 val) { V9_SetWriteAddress(addr); V9_Poke_CurrentAddr(val); }

// Function: V9_Poke16
// Write a 16-bits value to the given VRAM address
//
// Parameters:
//   addr - Destination VRAM address
//   val - The value to be write
inline void V9_Poke16(u32 addr, u16 val) { V9_SetWriteAddress(addr); V9_Poke16_CurrentAddr(val); }

// Function: V9_Peek
// Read a 8-bits value from at the given VRAM address
//
// Parameters:
//   addr - Source VRAM address
//
// Return:
//   The value at the given address
inline u8 V9_Peek(u32 addr) { V9_SetWriteAddress(addr); return V9_Peek_CurrentAddr(); }

// Function: V9_Peek16
// Read a 16-bits value from at the given VRAM address
//
// Parameters:
//   addr - Source VRAM address
//
// Return:
//   The value at the given address
inline u16 V9_Peek16(u32 addr) { V9_SetWriteAddress(addr); return V9_Peek16_CurrentAddr(); }

//-----------------------------------------------------------------------------
// Group: Setting
//-----------------------------------------------------------------------------

// Function: V9_SetMode
// Set the current screen mode
//
// Parameters:
//   mode - Screen mode ID (see <V9_SCREEN_MODE>)
void V9_SetMode(u8 mode);

// Function: V9_SetBPP
// Set bit number per pixel
//
// Parameters:
//   bpp - New bits per pixel to set. Can be: V9_R06_BPP_2, V9_R06_BPP_4, V9_R06_BPP_8 or V9_R06_BPP_16
inline void V9_SetBPP(u8 bpp) { V9_SetRegister(6, V9_GetRegister(6) & (~V9_R06_BPP_MASK) | bpp); }

// Function: V9_GetBPP
// Get bit number per pixel
//
// Return:
//   Current bits per pixel. Can be: V9_R06_BPP_2, V9_R06_BPP_4, V9_R06_BPP_8 or V9_R06_BPP_16
inline u8 V9_GetBPP() { return V9_GetRegister(6) & (~V9_R06_BPP_MASK); }

// Function: V9_SetImageSpaceWidth
// Set number of pixels in X direction of image space
//
// Parameters:
//   width - New image space width to set. Can be: V9_R06_WIDH_256, V9_R06_WIDH_512, V9_R06_WIDH_1024 or V9_R06_WIDH_2048
inline void V9_SetImageSpaceWidth(u8 width) { V9_SetRegister(6, V9_GetRegister(6) & (~V9_R06_WIDH_MASK) | width); }

// Function: V9_GetImageSpaceWidth
// Get number of pixels in X direction of image space
//
// Return:
//   Current image space width. Can be: V9_R06_WIDH_256, V9_R06_WIDH_512, V9_R06_WIDH_1024 or V9_R06_WIDH_2048
inline u8 V9_GetImageSpaceWidth() { return V9_GetRegister(6) & (~V9_R06_WIDH_MASK); }

// Function: V9_SetDisplayEnable
// Enalbe or disable the screen display
//
// Parameters:
//   enable - TRUE to enable or FALSE to disable
inline void V9_SetDisplayEnable(bool enable) { V9_SetFlag(8, V9_R08_DISP_ON, enable ? V9_R08_DISP_ON : 0); }

// Function: V9_SetBackgroundColor
// Set background color
//
// Parameters:
//   color - New background color
inline void V9_SetBackgroundColor(u8 color) { V9_SetRegister(15, color); }

// Function: V9_GetBackgroundColor
// Get background color
//
// Return:
//   Current background color
inline u8 V9_GetBackgroundColor() { return V9_GetRegister(15); }

// Function: V9_SetAdjustOffset
// Adjustment of the display location on the screen
//
// Parameters:
//   offset - Screen display position offset (MSB 4-bits: vertical offset, LSB 4-bits: horizontal offset)
inline void V9_SetAdjustOffset(u8 offset) { V9_SetRegister(16, offset); }

// Function: V9_SetAdjustOffsetXY
// Adjustment of the display location on the screen
//
// Parameters:
//   x - Horizontal screen display position offset [-7:+8]
//   y - Vertical screen display position offset [-7:+8]
inline void V9_SetAdjustOffsetXY(i8 x, i8 y) { V9_SetAdjustOffset(((-x) & 0x0F) | (((-y) & 0x0F) << 4)); }

// Function: V9_SetLayerPriority
// Set layer priority for P1 mode
//
// Parameters:
//   x - X coordinate where priority apply (2-bits value x 64 pixel)
//   y - Y coordinate where priority apply (2-bits value x 64 pixel)
inline void V9_SetLayerPriority(u8 x, u8 y) { V9_SetRegister(27, (y << 2) + x); }

//-----------------------------------------------------------------------------
// Group: Status
//-----------------------------------------------------------------------------

// Function: V9_GetStatus
// Get status port value
//
// Return:
//   Value of the status port (include various flags about frame rendering events)
inline u8 V9_GetStatus() { return V9_GetPort(V9_P05); }

// Function: V9_IsVBlank
// Is vertical non-display period
//
// Return:
//   FALSE if not in vertical non-display period
inline bool V9_IsVBlank() { return V9_GetStatus() & V9_P05_VR; }

// Function: V9_IsHBlank
// Is horizontal non-display period
//
// Return:
//   FALSE if not in horizontal non-display period
inline bool V9_IsHBlank() { return V9_GetStatus() & V9_P05_HR; }

// Function: V9_IsCmdDataReady
// Check if command data transfer ready
//
// Return:
//   FALSE if command data transfer not ready
inline bool V9_IsCmdDataReady() { return V9_GetStatus() & V9_P05_TR; }

// Function: V9_IsCmdRunning
// Check if a command is in process
//
// Return:
//   TRUE if command still running
inline bool V9_IsCmdRunning() { return V9_GetStatus() & V9_P05_CE; }

// Function: V9_IsCmdComplete
// Check if a no command is in process
//
// Return:
//   TRUE if command finished
inline bool V9_IsCmdComplete() { return !V9_IsCmdRunning(); }

// Function: V9_IsSecondField
// Check if render is in the second field period during interlace
//
// Return:
//   FALSE if not in the second field period during interlace
inline bool V9_IsSecondField() { return V9_GetStatus() & V9_P05_E0; }

//-----------------------------------------------------------------------------
// Group: Interrupt
//-----------------------------------------------------------------------------

#define V9_INT_VBLANK				V9_P06_VI
#define V9_INT_HBLANK				V9_P06_HI
#define V9_INT_CMDEND				V9_P06_CE

// Function: V9_SetInterrupt
// Set interruption flags
//
// Parameters:
//   flags - Can be a combination of V9_INT_VBLANK, V9_INT_HBLANK or V9_INT_CMDEND
inline void V9_SetInterrupt(u8 flags) { V9_SetRegister(9, flags); }

// Function: V9_DisableInterrupt
// Disable all interruptions
inline void V9_DisableInterrupt() { V9_SetRegister(9, 0); }

// Function: V9_SetVBlankInterrupt
// Set vertical blank interruption 
//
// Parameters:
//   enable - TRUE to enable or FALSE to disable
inline void V9_SetVBlankInterrupt(bool enable) { V9_SetFlag(9, V9_R08_IEV_ON, enable ? V9_R08_IEV_ON : 0); }

// Function: V9_SetHBlankInterrupt
// Set horizontal blank interruption 
//
// Parameters:
//   enable - TRUE to enable or FALSE to disable
inline void V9_SetHBlankInterrupt(bool enable) { V9_SetFlag(9, V9_R08_IEH_ON, enable ? V9_R08_IEH_ON : 0); }

// Function: V9_SetCmdEndInterrupt
// Set command end interruption 
//
// Parameters:
//   enable - TRUE to enable or FALSE to disable
inline void V9_SetCmdEndInterrupt(bool enable) { V9_SetFlag(9, V9_R08_IECE_ON, enable ? V9_R08_IECE_ON : 0); }

// Function: V9_SetInterruptLine
// Specification of vertical position where display position interrupt occurs
//
// Parameters:
//   line - Line where h-blank interruption will occur (Specified by means of line No. with the display start line as "0")
inline void V9_SetInterruptLine(u16 line) { V9_SetRegister16(10, line); }

// Function: V9_SetInterruptEveryLine
// Set line interrupt on every line
inline void V9_SetInterruptEveryLine() { V9_SetRegister(11, V9_R09_EVERYLINE); }

// Function: V9_SetInterruptX
// Specification of horizontal position where display position interrupt occurs
//
// Parameters:
//   x - Horizontal position where h-blank interruption will occur (Specified by unit of 64 master clock with the display start position as "0")
inline void V9_SetInterruptX(u8 x) { V9_SetRegister(12, x); }

//-----------------------------------------------------------------------------
// Group: Scrolling
//-----------------------------------------------------------------------------

// Function: V9_SetScrollingX
// Set horizontal scrolling value
//
// Parameters:
//   x - Scrolling value (between 0 and image space width-1)
void V9_SetScrollingX(u16 x);

// Function: V9_SetScrollingY
// Set vertical scrolling value
//
// Parameters:
//   y - Scrolling value (between 0 and image space height-1)
void V9_SetScrollingY(u16 y);

// Function: V9_SetScrolling
// Set horizontal and vertical scrolling values
//
// Parameters:
//   x - Scrolling value (between 0 and image space width-1)
//   y - Scrolling value (between 0 and image space height-1)
inline void V9_SetScrolling(u16 x, u16 y) { V9_SetScrollingX(x);  V9_SetScrollingY(y); }

// Function: V9_SetScrollingBX
// Set horizontal scrolling value for layer B (P1 mode only)
//
// Parameters:
//   x - Scrolling value (between 0 and image space width-1)
void V9_SetScrollingBX(u16 x);

// Function: V9_SetScrollingBY
// Set vertical scrolling value for layer B (P1 mode only)
//
// Parameters:
//   y - Scrolling value (between 0 and image space height-1)
void V9_SetScrollingBY(u16 y);

// Function: V9_SetScrollingB
// Set horizontal and vertical scrolling values for layer B (P1 mode only)
//
// Parameters:
//   x - Scrolling value (between 0 and image space width-1)
//   y - Scrolling value (between 0 and image space height-1)
inline void V9_SetScrollingB(u16 x, u16 y) { V9_SetScrollingBX(x);  V9_SetScrollingBY(y); }

//-----------------------------------------------------------------------------
// Group: Cursor
//-----------------------------------------------------------------------------

// Function: V9_SetCursorAttribute
//
inline void V9_SetCursorAttribute(u8 id, u16 x, u16 y, u8 color)
{
	V9_Poke(0x7FE00 + id * 8, y & 0xFF);
	V9_Poke(0x7FE02 + id * 8, y >> 8);
	V9_Poke(0x7FE04 + id * 8, x & 0xFF);
	V9_Poke(0x7FE06 + id * 8, ((x >> 8) & 0x3) + (color << 6));
}

#define V9_CURSOR_DISABLE			0b00010000

// Function: V9_SetCursorEnable
//
inline void V9_SetCursorEnable(u8 id, bool enable)
{
	u8 val = V9_Peek(0x7FE06 + id * 8) & ~V9_CURSOR_DISABLE;
	V9_Poke(0x7FE06 + id * 8, (enable) ? val : val | V9_CURSOR_DISABLE);
}

// Function: V9_SetCursorPattern
//
inline void V9_SetCursorPattern(u8 id, const u8* data) { V9_WriteVRAM(0x7FF00 + id * 0x80, data, 128); }

// Function: V9_SetCursorPalette
// Set cursor palette offset
inline void V9_SetCursorPalette(u8 offset) { V9_SetRegister(28, offset); }

//-----------------------------------------------------------------------------
// Group: Sprite
//-----------------------------------------------------------------------------

// Function: V9_SetSpriteEnable
//
inline void V9_SetSpriteEnable(bool enable) { V9_SetFlag(8, V9_R08_SPD_OFF, enable ? 0 : V9_R08_SPD_OFF); }

// Function: V9_SetSpritePatternAddr
// Set sprite patterns VRAM address
inline void V9_SetSpritePatternAddr(u8 addr) { V9_SetRegister(25, addr); }

// Function: V9_SetSpritePalette
// Set sprite palette offset
inline void V9_SetSpritePalette(u8 offset) { V9_SetRegister(28, offset); }

//
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

// Function: V9_SetSpriteP1
// Set sprite attribute for P1 mode
inline void V9_SetSpriteP1(u8 id, const struct V9_Sprite* attr) { V9_WriteVRAM(V9_P1_SPAT + 4 * id, (const u8*)attr, 4); }

// Function: V9_SetSpritePatternP1
// Set sprite pattern for P1 mode
inline void V9_SetSpritePatternP1(u8 id, u8 pat) { V9_Poke(V9_P1_SPAT + 4 * id + 1, pat); }

// Function: V9_SetSpritePositionP1
// Set sprite position for P1 mode
inline void V9_SetSpritePositionP1(u8 id, u8 x, u8 y) { V9_Poke(V9_P1_SPAT + (4 * id), y); V9_Poke(V9_P1_SPAT + (4 * id) + 2, x); }

// Function: V9_SetSpriteP2
// Set sprite attribute for P2 mode
inline void V9_SetSpriteP2(u8 id, const struct V9_Sprite* attr) { V9_WriteVRAM(V9_P2_SPAT + 4 * id, (const u8*)attr, 4); }

// Function: V9_SetSpritePatternP2
// Set sprite pattern for P2 mode
inline void V9_SetSpritePatternP2(u8 id, u8 pat) { V9_Poke(V9_P2_SPAT + 4 * id + 1, pat); }

// Function: V9_SetSpritePositionP2
// Set sprite position for P2 mode
inline void V9_SetSpritePositionP2(u8 id, u8 x, u8 y) { V9_Poke(V9_P2_SPAT + (4 * id), y); V9_Poke(V9_P2_SPAT + (4 * id) + 2, x); }

//-----------------------------------------------------------------------------
// Group: Palette
//-----------------------------------------------------------------------------

//
struct V9_Color
{
	u16 B  : 5;
	u16 R  : 5;
	u16 G  : 5;
	u16 Ys : 1;
};

#if ((V9_PALETTE_MODE == V9_PALETTE_GBR_16) || (V9_PALETTE_MODE == V9_PALETTE_YSGBR_16))

// Function: V9_SetPaletteEntry
// Set the color of a given palette entry.
//
// Parameters:
//   index - Index of the palette entry (0-63)
//   color - 16 bits color value
//           Format: [Ys:1|green:5|red:5|blue:5]
void V9_SetPaletteEntry(u8 index, u16 color) __PRESERVES(h, l, iyl, iyh);

// Function: V9_SetPalette
// Set the colors of a given palette entries.
//
// Parameters:
//   first - Index of the first entry to set (0-63)
//   num   - Numer of entries to set (1-64)
//   table - 16 bits color table
//           Format: [Ys:1|green:5|red:5|blue:5]
inline void V9_SetPalette(u8 first, u8 num, const u16* table) { for(u8 i = 0; i < num; ++i) V9_SetPaletteEntry(first++, *(table++)); }

// Function: V9_SetPaletteAll
// Set the colors of all the palette 64 entries.
//
// Parameters:
//   table - 16 bits color table
//           Format: [Ys:1|green:5|red:5|blue:5]
inline void V9_SetPaletteAll(const u16* table) { V9_SetPalette(0, 64, table); }

#elif (V9_PALETTE_MODE == V9_PALETTE_RGB_24)

// Set the color of a given palette entry.
void V9_SetPaletteEntry(u8 index, const u8* color);

// Set the colors of a given palette entries.
inline void V9_SetPalette(u8 first, u8 num, const u8* table) { for(u8 i = 0; i < num; ++i) { V9_SetPaletteEntry(first++, table); table += 3; } }

// Set the colors of all the palette 64 entries.
inline void V9_SetPaletteAll(const u8* table) { V9_SetPalette(0, 64, table); }

#endif

// Function: V9_SetLayerPalette
// Set the P1 and P2 layers palette offset
inline void V9_SetLayerPalette(u8 a, u8 b) { V9_SetRegister(13, ((b & 0x3) << 2) + (a & 0x3)); }

//-----------------------------------------------------------------------------
// Group: Command helper
//-----------------------------------------------------------------------------

// Function: V9_SetCommandSX
//
inline void V9_SetCommandSX(u16 sx) { V9_SetRegister16(32, sx); }

// Function: V9_SetCommandSY
//
inline void V9_SetCommandSY(u16 sy) { V9_SetRegister16(34, sy); }

// Function: V9_SetCommandSA
//
inline void V9_SetCommandSA(u32 sa) { V9_SetRegister(32, sa & 0xFF); V9_SetRegister(34, (sa >> 8) & 0xFF); V9_SetRegister(35, (sa >> 16) & 0xFF); }

// Function: V9_SetCommandDX
//
inline void V9_SetCommandDX(u16 dx) { V9_SetRegister16(36, dx); }

// Function: V9_SetCommandDY
//
inline void V9_SetCommandDY(u16 dy) { V9_SetRegister16(38, dy); }

// Function: V9_SetCommandDA
//
inline void V9_SetCommandDA(u32 da) { V9_SetRegister(36, da & 0xFF); V9_SetRegister(38, (da >> 8) & 0xFF); V9_SetRegister(39, (da >> 16) & 0xFF); }

// Function: V9_SetCommandNX
//
inline void V9_SetCommandNX(u16 nx) { V9_SetRegister16(40, nx); }

// Function: V9_SetCommandNY
//
inline void V9_SetCommandNY(u16 ny) { V9_SetRegister16(42, ny); }

// Function: V9_SetCommandMJ
//
inline void V9_SetCommandMJ(u16 mj) { V9_SetRegister16(40, mj); }

// Function: V9_SetCommandMI
//
inline void V9_SetCommandMI(u16 mi) { V9_SetRegister16(42, mi); }

// Function: V9_SetCommandNA
//
inline void V9_SetCommandNA(u32 na) { V9_SetRegister(40, na & 0xFF); V9_SetRegister(42, (na >> 8) & 0xFF); V9_SetRegister(43, (na >> 16) & 0xFF); }

// Function: V9_SetCommandArgument
//
inline void V9_SetCommandArgument(u8 arg) { V9_SetRegister(44, arg); }

// Function: V9_SetCommandLogicalOp
//
inline void V9_SetCommandLogicalOp(u8 lop) { V9_SetRegister(45, lop); }

// Function: V9_SetCommandWriteMask
//
inline void V9_SetCommandWriteMask(u16 wm) { V9_SetRegister16(46, wm); }

// Function: V9_SetCommandFC
//
inline void V9_SetCommandFC(u16 fc) { V9_SetRegister16(48, fc); }

// Function: V9_SetCommandBC
//
inline void V9_SetCommandBC(u16 bc) { V9_SetRegister16(50, bc); }

// Function: V9_ExecCommand
//
inline void V9_ExecCommand(u8 op) { V9_SetRegister(52, op); }

// Function: GetCommandBX
//
inline u16 GetCommandBX() { return V9_GetRegister(53) + (V9_GetRegister(54) << 8); }

//-----------------------------------------------------------------------------
// Group: Command
//-----------------------------------------------------------------------------

// Function: V9_CommandSTOP
// Command being executed is stopped.
inline void V9_CommandSTOP() { V9_ExecCommand(V9_CMD_STOP); }

// Function: V9_CommandLMMC
// Data is transferred from CPU to VRAM rectangle area.
inline void V9_CommandLMMC(const u8* src, u16 dx, u16 dy, u16 nx, u16 ny, u8 arg) { V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_SetCommandNX(nx); V9_SetCommandNY(ny); V9_SetCommandArgument(arg); V9_ExecCommand(V9_CMD_LMMC); }

// Function: V9_CommandLMMV
// VRAM rectangle area is painted out.
inline void V9_CommandLMMV(u16 dx, u16 dy, u16 nx, u16 ny, u8 arg, u16 fc) { V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_SetCommandNX(nx); V9_SetCommandNY(ny); V9_SetCommandArgument(arg); V9_SetCommandFC(fc); V9_ExecCommand(V9_CMD_LMMV); }

// Function: V9_CommandLMCM
// VRAM rectangle area data is transferred to CPU.
inline void V9_CommandLMCM(u16 sx, u16 sy, u16 nx, u16 ny, u8 arg, u8* dest) { V9_SetCommandSX(sx); V9_SetCommandSY(sy); V9_SetCommandNX(nx); V9_SetCommandNY(ny); V9_SetCommandArgument(arg); V9_ExecCommand(V9_CMD_LMCM); }

// Function: V9_CommandLMMM
// Rectangle area data is transferred from VRAM to VRAM.
inline void V9_CommandLMMM(u16 sx, u16 sy, u16 dx, u16 dy, u16 nx, u16 ny, u8 arg) { V9_SetCommandSX(sx); V9_SetCommandSY(sy); V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_SetCommandNX(nx); V9_SetCommandNY(ny); V9_SetCommandArgument(arg); V9_ExecCommand(V9_CMD_LMMM); }

// Function: V9_CommandCMMC
// CPU character data is color-developed and transferred to VRAM rectangle area.
inline void V9_CommandCMMC(u16 dx, u16 dy, u16 nx, u16 ny, u8 arg, u16 fc, u16 bc) { V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_SetCommandNX(nx); V9_SetCommandNY(ny); V9_SetCommandArgument(arg); V9_SetCommandFC(fc); V9_SetCommandBC(bc); V9_ExecCommand(V9_CMD_CMMC); }

// Function: V9_CommandCMMM
// VRAM character data is color-developed and transferred to VRAM rectangle area.
inline void V9_CommandCMMM(u32 sa, u16 dx, u16 dy, u16 nx, u16 ny, u8 arg, u16 fc, u16 bc) { V9_SetCommandSA(sa); V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_SetCommandNX(nx); V9_SetCommandNY(ny); V9_SetCommandArgument(arg); V9_SetCommandFC(fc); V9_SetCommandBC(bc); V9_ExecCommand(V9_CMD_CMMM); }

// Function: V9_CommandBMXL
// Data on VRAM linear address is transferred to VRAM rectangle area.
inline void V9_CommandBMXL(u32 sa, u16 dx, u16 dy, u16 nx, u16 ny, u8 arg) { V9_SetCommandSA(sa); V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_SetCommandNX(nx); V9_SetCommandNY(ny); V9_SetCommandArgument(arg); V9_ExecCommand(V9_CMD_BMXL); }

// Function: V9_CommandBMLX
// VRAM rectangle area data is transferred onto VRAM linear address.
inline void V9_CommandBMLX(u16 sx, u16 sy, u32 da, u16 nx, u16 ny, u8 arg) { V9_SetCommandSX(sx); V9_SetCommandSY(sy); V9_SetCommandDA(da); V9_SetCommandNX(nx); V9_SetCommandNY(ny); V9_SetCommandArgument(arg); V9_ExecCommand(V9_CMD_BMLX); }

// Function: V9_CommandBMLL
// Data on VRAM linear address is transferred onto VRAM linear address.
inline void V9_CommandBMLL(u32 sa, u32 da, u32 na, u8 arg) { V9_SetCommandSA(sa); V9_SetCommandDA(da); V9_SetCommandNA(na); V9_SetCommandArgument(arg); V9_ExecCommand(V9_CMD_BMLL); }

// Function: V9_CommandLINE
// Straight line is drawn on X/Y-coordinates.
inline void V9_CommandLINE(u16 dx, u16 dy, u16 mj, u16 mi, u8 arg, u16 fc) { V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_SetCommandMJ(mj); V9_SetCommandMI(mi); V9_SetCommandArgument(arg); V9_SetCommandFC(fc); V9_ExecCommand(V9_CMD_LINE); }

// Function: V9_CommandSEARCH
// Border color coordinates on X/Y space are detected.
inline void V9_CommandSEARCH(u16 sx, u16 sy, u8 arg, u16 fc) { V9_SetCommandSX(sx); V9_SetCommandSY(sy); V9_SetCommandArgument(arg); V9_SetCommandFC(fc); V9_ExecCommand(V9_CMD_SEARCH); }

// Function: V9_CommandPOINT
// Color code of specified point on X/Y-coordinates is read out.
inline void V9_CommandPOINT(u16 sx, u16 sy) { V9_SetCommandSX(sx); V9_SetCommandSY(sy); V9_ExecCommand(V9_CMD_POINT); }

// Function: V9_CommandPSET
// Drawing is executed at drawing point on X/Y-coordinates.
inline void V9_CommandPSET(u16 dx, u16 dy, u16 fc, u8 shift) { V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_SetCommandFC(fc); V9_ExecCommand(V9_CMD_PSET | shift); }

// Function: V9_CommandADVANCE
// Drawing point on X/Y-coordinates is shifted.
inline void V9_CommandADVANCE(u16 dx, u16 dy, u8 shift) { V9_SetCommandDX(dx); V9_SetCommandDY(dy); V9_ExecCommand(V9_CMD_ADVANCE | shift); }

//-----------------------------------------------------------------------------
// Group: Helper
//-----------------------------------------------------------------------------

// Function: V9_Detect
// Detecting the presence of a V9990 card
bool V9_Detect();

// Function: V9_ClearVRAM
// Clear the whole 512 KB of VRAM with zero
void V9_ClearVRAM();

// Function: V9_CellAddrP1A
//
inline u32 V9_CellAddrP1A(u8 x, u8 y) { return V9_P1_PNT_A + (((64 * y) + x) * 2); }

// Function: V9_CellAddrP1B
//
inline u32 V9_CellAddrP1B(u8 x, u8 y) { return V9_P1_PNT_B + (((64 * y) + x) * 2); }

// Function: V9_CellAddrP2
//
inline u32 V9_CellAddrP2(u8 x, u8 y) { return V9_P2_PNT + (((128 * y) + x) * 2); }

// Function: V9_WaitCmdEnd
// Wait for current command completion
inline void V9_WaitCmdEnd() { while(V9_IsCmdRunning()) {} }