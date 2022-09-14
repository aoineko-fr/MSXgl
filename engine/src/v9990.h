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

#define V9_PALETTE_YSGBR_16			0
#define V9_PALETTE_GBR_16			1
#define V9_PALETTE_RGB_24			2


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
void V9_SetPort(u8 port, u8 value) __PRESERVES(b, d, e, h, iyl, iyh);

// Function: V9_GetPort
// Get port value
u8 V9_GetPort(u8 port) __PRESERVES(b, d, e, h, l, iyl, iyh);

// Function: V9_SetRegister
// Set register value
void V9_SetRegister(u8 reg, u8 val) __PRESERVES(b, c, d, e, h, iyl, iyh);

// Function: V9_GetRegister
// Get register value
u8 V9_GetRegister(u8 reg) __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: 
//
inline void V9_SetFlag(u8 reg, u8 mask, u8 flag) { V9_SetRegister(reg, V9_GetRegister(reg) & (~mask) | flag); }

//-----------------------------------------------------------------------------
// Group: Setting
//-----------------------------------------------------------------------------

// Function: V9_SetMode
// Set the current screen mode
void V9_SetMode(u8 mode);

// Function: V9_SetBPP
// Set bit number per pixel
inline void V9_SetBPP(u8 bpp) { V9_SetRegister(6, V9_GetRegister(6) & (~V9_R06_BPP_MASK) | bpp); }

// Function: V9_GetBPP
// Get bit number per pixel
inline u8 V9_GetBPP() { return V9_GetRegister(6) & (~V9_R06_BPP_MASK); }

// Function: V9_SetImageSpaceWidth
// Set number of pixels in X direction of image space
inline void V9_SetImageSpaceWidth(u8 width) { V9_SetRegister(6, V9_GetRegister(6) & (~V9_R06_WIDH_MASK) | width); }

// Function: V9_GetImageSpaceWidth
// Get number of pixels in X direction of image space
inline u8 V9_GetImageSpaceWidth() { return V9_GetRegister(6) & (~V9_R06_WIDH_MASK); }

// Function: 
//
inline void V9_SetDisplayEnable(bool enable) { V9_SetFlag(8, V9_R08_DISP_ON, enable ? V9_R08_DISP_ON : 0); }

// Function: 
//
inline void V9_SetSpriteEnable(bool enable) { V9_SetFlag(8, V9_R08_SPD_OFF, enable ? 0 : V9_R08_SPD_OFF); }

// Function: V9_SetScrollingX
//
void V9_SetScrollingX(u16 x);

// Function: V9_SetScrollingY
//
void V9_SetScrollingY(u16 y);

// Function: V9_SetScrolling
//
inline void V9_SetScrolling(u16 x, u16 y) { V9_SetScrollingX(x);  V9_SetScrollingY(y); }

// Function: V9_SetScrollingBX
//
void V9_SetScrollingBX(u16 x);

// Function: V9_SetScrollingBY
//
void V9_SetScrollingBY(u16 y);

// Function: V9_SetScrollingB
//
inline void V9_SetScrollingB(u16 x, u16 y) { V9_SetScrollingBX(x);  V9_SetScrollingBY(y); }

//-----------------------------------------------------------------------------
// Group: VRAM access
//-----------------------------------------------------------------------------

// Function: V9_SetWriteAddress
// Initialize VRAM address for writing
void V9_SetWriteAddress(u32 addr) __PRESERVES(b, iyl, iyh);

// Function: V9_SetReadAddress
// Initialize VRAM address for reading
void V9_SetReadAddress(u32 addr) __PRESERVES(b, iyl, iyh);

// Function: V9_FillVRAM_CurrentAddr
// 
void V9_FillVRAM_CurrentAddr(u8 value, u16 count);

// Function: V9_WriteVRAM_CurrentAddr
//
void V9_WriteVRAM_CurrentAddr(const u8* src, u16 count);

// Function: V9_ReadVRAM_CurrentAddr
//
void V9_ReadVRAM_CurrentAddr(const u8* dest, u16 count);

// Function: V9_Poke_CurrentAddr
//
void V9_Poke_CurrentAddr(u8 val) __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: V9_Peek_CurrentAddr
//
u8 V9_Peek_CurrentAddr() __PRESERVES(b, c, d, e, h, l, iyl, iyh);

// Function: V9_FillVRAM
//
inline void V9_FillVRAM(u32 addr, u8 value, u16 count) { V9_SetWriteAddress(addr); V9_FillVRAM_CurrentAddr(value, count); }

// Function: V9_WriteVRAM
//
inline void V9_WriteVRAM(u32 addr, const u8* src, u16 count) { V9_SetWriteAddress(addr); V9_WriteVRAM_CurrentAddr(src, count); }

// Function: V9_ReadVRAM
//
inline void V9_ReadVRAM(u32 addr, const u8* dest, u16 count) { V9_SetReadAddress(addr); V9_ReadVRAM_CurrentAddr(dest, count); }

// Function: V9_Poke
//
inline void V9_Poke(u32 addr, u8 val) { V9_SetWriteAddress(addr); V9_Poke_CurrentAddr(val); }

// Function: V9_Peek
//
inline u8 V9_Peek(u32 addr) { V9_SetWriteAddress(addr); return V9_Peek_CurrentAddr(); }

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
void V9_SetPaletteEntry(u8 index, u16 color);

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

//-----------------------------------------------------------------------------
// Group: Helper
//-----------------------------------------------------------------------------

// Function: V9_Detect
// Detecting the presence of a V9990 card
bool V9_Detect();

// Function: V9_ClearVRAM
// Clear the whole 512 KB of VRAM with zero
void V9_ClearVRAM();



		// ifndef G9K_DISABLE_DIRECT_EXPORT
// ;----------------------------------------------------------------------------;
// ; General Functions overview                                                 ;
// ;----------------------------------------------------------------------------;
		// EXPORT G9k.Reset    	   ; Reset and initialize the Gfx9000
		// EXPORT G9k.SetScreenMode   ; Set screen mode
		// EXPORT G9k.SetVramWrite    ; Set vram write address
		// EXPORT G9k.SetVramRead     ; Set vram read address
		// EXPORT G9k.Detect          ; Detect presence of the Gfx9000
		// EXPORT G9k.DisplayEnable   ; Enable display
		// EXPORT G9k.DisplayDisable  ; Disable display
		// EXPORT G9k.SpritesEnable   ; Enable sprites/mouse cursor
		// EXPORT G9k.SpritesDisable  ; Disable sprites/mouse cursor
		// EXPORT G9k.WritePalette    ; Write palette data to the Gfx9000
		// EXPORT G9k.ReadPalette     ; Read palette data from the Gfx9000
		// EXPORT G9k.SetAdjust       ; Adjust Gfx9000 display 
		// EXPORT G9k.SetBackDropColor; Set backdrop color
		// EXPORT G9k.SetScrollX      ; Set scroll X Layer A
		// EXPORT G9k.SetScrollY      ; Set scroll Y Layer A
		// IFNDEF G9K_DISABLE_PATTERN
		// EXPORT G9k.SetScrollXB	   ; Set scroll X Layer B
		// EXPORT G9k.SetScrollYB     ; Set scroll Y Layer B
		// ENDIF
		// EXPORT G9k.SetScrollMode   ; Set scroll mode
		// EXPORT G9k.Close           ; Closes a G9B or VFF file

// ;----------------------------------------------------------------------------;
// ; Blitter Function overview                                                  ;
// ;----------------------------------------------------------------------------;
		// EXPORT G9k.DrawFilledBox     ; Draw filled box
		// EXPORT G9k.DrawBox           ; Draw box
		// EXPORT G9k.DrawLine	     ; Draw line (simple)
		// EXPORT G9k.SetupCopyRamToXY  ; Setup parameters for Ram to XY copy
		// EXPORT G9k.CopyRamToXY       ; Copy data from Ram to XY
		// EXPORT G9k.SetupCopyXYToRam  ; Setup parameters for XY to Ram copy
		// EXPORT G9k.CopyXYToRam       ; Copy data from XY to Ram
		// EXPORT G9k.CopyXYToXY        ; Copy XY to XY
		// EXPORT G9k.CopyXYToRegisterXY; Copy XY(struct) to XY (registers)
		// EXPORT G9k.CopyVramToXY      ; Copy Linear vram address to XY
		// EXPORT G9k.CopyXYToVram      ; Copy XY to Linear vram address
		// EXPORT G9k.CopyVramToVram    ; Copy vram linear to vram linear
		// EXPORT G9k.SetCmdWriteMask   ; Set blitter command write mask
		// EXPORT G9k.SetCmdColor       ; Set blitter command color
		// EXPORT G9k.SetCmdBackColor   ; Set command back ground color
		// EXPORT G9k.CopyRamToVram     ; Copy data from ram to Linear vram address
// ;----------------------------------------------------------------------------;
// ; Font Function overview                                                     ;
// ; ---------------------------------------------------------------------------;
// ; DEFINE G9K_DISABLE_VFF to disable inclution of vff functions
		// IFNDEF G9K_DISABLE_VFF
		// EXPORT G9k.OpenVff            ; Open a VFF file
		// EXPORT G9k.LoadFont	      ; Loads a VFF(V9990 font format) file from disk
		// EXPORT G9k.SetFont            ; Set a font as default
		// EXPORT G9k.PrintString        ; Print a zero terminated string 
		// EXPORT G9k.PutChar	      ; Print a character
		// EXPORT G9k.Locate	      ; Set X and Y coordinates for putchar
		// ENDIF
// ;----------------------------------------------------------------------------;
// ; Gfx9000 bitmap functions                                                   ;
// ;----------------------------------------------------------------------------;
// ; DEFINE G9K_DISABLE_G9B to disable inclution of G9B functions
		// IFNDEF G9K_DISABLE_G9B
		// EXPORT G9k.OpenG9B            ; Open a G9B file
		// EXPORT G9k.ReadG9BPalette     ; Read palette data from disk to Gfx9000
		// EXPORT G9k.ReadG9B            ; Read data from disk to Gfx9000 VRAM X,Y
		// EXPORT G9k.ReadG9BLinear      ; Read data from disk to Gfx9000 Linear VRAM Address
		// EXPORT G9k.CalcG9BVramSize    ; Calculate vram size of A G9B file
		// ENDIF
		
// ;----------------------------------------------------------------------------;
// ; Gfx9000 pattern functions                                                  ;
// ;----------------------------------------------------------------------------;
// ; DEFINE G9K_DISABLE_PATTERN to disable inclution of pattern functions
		// IFNDEF G9K_DISABLE_PATTERN
		// EXPORT G9k.SetPatternData     ; Set pattern data
		// EXPORT G9k.GetPatternData     ; Get partern data
		// EXPORT G9k.SetPattern         ; Set pattern
		// EXPORT G9k.GetPattern         ; Get pattern
		// ENDIF
// ;----------------------------------------------------------------------------;
// ; Gfx9000 Interrupt functions                                                ;
// ;----------------------------------------------------------------------------;
		// EXPORT  G9k.SetIntLine
		
// ;----------------------------------------------------------------------------;
// ; Macro overview    	                                                     ;
// ;----------------------------------------------------------------------------;
// ; G9kCmdWait            - Wait for Blitter command completion
// ; G9kWriteReg           - Write Gfx9000 register
// ; G9kReadReg            - Read Gfx9000 register
// ; G9kWaitVsync          - Wait for Vertical Sync
		// ENDIF

// ;----------------------------------------------------------------------------;
// ; V9990 register and port defines                                            ;
// ;----------------------------------------------------------------------------;




// ; Bit defines G9K_SYS_CTRL
// G9K_SYS_CTRL_SRS		EQU	2	; Power on reset state
// G9K_SYS_CTRL_MCKIN	EQU	1	; Select MCKIN terminal
// G9K_SYS_CTRL_XTAL	EQU	0	; Select XTAL

// ; Register Select options
// G9K_DIS_INC_READ		EQU	64
// G9K_DIS_INC_WRITE	EQU	128

// ; Bit defines G9K_SCREEN_MODE0 (register 6)
// G9K_SCR0_STANDBY		EQU	192	; Stand by mode
// G9K_SCR0_BITMAP		EQU	128	; Select Bit map mode
// G9K_SCR0_P2			EQU	64	; Select P1 mode
// G9K_SCR0_P1			EQU	0	; Select P1 mode
// G9K_SCR0_DTCLK		EQU	32	; Master Dot clock not divided
// G9K_SCR0_DTCLK2		EQU	16	; Master Dot clock divided by 2
// G9K_SCR0_DTCLK4		EQU	0	; Master Dot clock divided by 4
// G9K_SCR0_XIM2048		EQU	12	; Image size = 2048
// G9K_SCR0_XIM1024		EQU	8	; Image size = 1024
// G9K_SCR0_XIM512		EQU	4	; Image size = 512
// G9K_SCR0_XIM256		EQU	0	; Image size = 256
// G9K_SCR0_16BIT		EQU	3	; 16 bits/dot
// G9K_SCR0_8BIT		EQU	2	; 8 bits/dot
// G9K_SCR0_4BIT		EQU	1	; 4 bits/dot
// G9K_SCR0_2BIT		EQU	0	; 2 bits/dot

// ; Bit defines G9K_SCREEN_MODE1 (register 7)
// G9K_SCR1_C25M		EQU	64	; Select 640*480 mode
// G9K_SCR1_SM1		EQU	32	; Selection of 263 lines during non interlace , else 262
// G9K_SCR1_SM		EQU	16	; Selection of horizontal frequency 1H=fsc/227.5
// G9K_SCR1_PAL		EQU	8	; Select PAL, else NTSC
// G9K_SCR1_EO		EQU	4	; Select of vertical resoltion of twice the non-interlace resolution
// G9K_SCR1_IL		EQU	2	; Select Interlace
// G9K_SCR1_HSCN		EQU	1	; Select High scan mode

// ; Bit defines G9K_CTRL    (Register 8)
// G9K_CTRL_DISP		EQU	128	; Display VRAM
// G9K_CTRL_DIS_SPD	EQU	64	; Disable display sprite (cursor)
// G9K_CTRL_YSE		EQU	32	; /YS Enable
// G9K_CTRL_VWTE		EQU	16	; VRAM Serial data bus control during digitization
// G9K_CTRL_VWM		EQU	8	; VRAM write control during digitization
// G9K_CTRL_DMAE		EQU	4	; Enable DMAQ output
// G9K_CTRL_VRAM512	EQU	2	; VRAM=512KB
// G9K_CTRL_VRAM256	EQU	1	; VRAM=256KB
// G9K_CTRL_VRAM128	EQU	0	; VRAM=128KB

// ; Bit defines G9K_INT_ENABLE (register 9)
// G9K_INT_IECE	        EQU     4       ; Command end interrupt enable control
// G9K_INT_IEH	        EQU     2       ; Display position interrupt enable
// G9K_INT_IEV	        EQU     1       ; Int. enable during vertical retrace line interval

// ; Bit Defines G9K_PALETTE_CTRL  (Register 13)
// G9K_PAL_CTRL_YUV	EQU	192	; YUV mode
// G9K_PAL_CTRL_YJK	EQU	128	; YJK mode
// G9K_PAL_CTRL_256	EQU	64	; 256 color mode
// G9K_PAL_CTRL_PAL	EQU	0	; Pallete mode
// G9K_PAL_CTRL_YAE	EQU	32	; Enable YUV/YJK RGB mixing mode

// ; Bit defines G9K_LOP           (Register 45)
// G9K_LOP_TP		EQU	16
// G9K_LOP_WCSC		EQU	12
// G9K_LOP_WCNOTSC		EQU	3
// G9K_LOP_WCANDSC		EQU	8
// G9K_LOP_WCORSC		EQU	14
// G9K_LOP_WCEORSC		EQU	6

// ; Bit defines G9K_ARG
// G9K_ARG_MAJ		EQU	1
// G9K_ARG_NEG		EQU	2
// G9K_ARG_DIX		EQU	4
// G9K_ARG_DIY		EQU	8

// ; Blitter Commands G9K_OPCODE    (Register 52)
// G9K_OPCODE_STOP		EQU	00h	; Command being excuted is stopped 
// G9K_OPCODE_LMMC		EQU	10h     ; Data is transferred from CPU to VRAM rectangle area
// G9K_OPCODE_LMMV		EQU	20h     ; VRAM rectangle area is painted out
// G9K_OPCODE_LMCM		EQU	30h     ; VRAM rectangle area is transferred to CPU
// G9K_OPCODE_LMMM		EQU	40h     ; Rectangle area os transferred from VRAM to VRAM
// G9K_OPCODE_CMMC		EQU	050h    ; CPU character data is color-developed and transferred to VRAM rectangle area
// G9K_OPCODE_CMMK		EQU	060h    ; Kanji ROM data is is color-developed and transferred to VRAM rectangle area
// G9K_OPCODE_CMMM		EQU	070h    ; VRAM character data is color-developed and transferred to VRAM rectangle area 
// G9K_OPCODE_BMXL		EQU	080h    ; Data on VRAM linear address is transferred to VRAM rectangle area
// G9K_OPCODE_BMLX		EQU	090h    ; VRAM rectangle area is transferred to VRAM linear address 
// G9K_OPCODE_BMLL		EQU	0A0h    ; Data on VRAM linear address is transferred to VRAM linear address 
// G9K_OPCODE_LINE		EQU	0B0h    ; Straight line is drawer on X-Y co-ordinates
// G9K_OPCODE_SRCH		EQU	0C0h    ; Border color co-ordinates on X-Y are detected
// G9K_OPCODE_POINT	EQU	0D0h    ; Color code on specified point on X-Y is read out
// G9K_OPCODE_PSET		EQU	0E0h    ; Drawing is executed at drawing point on X-Y co-ordinates
// G9K_OPCODE_ADVN		EQU	0F0h    ; Drawing point on X-Y co-ordinates is shifted

// ; Bit defines G9K_STATUS
// G9K_STATUS_TR           EQU     128
// G9K_STATUS_VR           EQU     64
// G9K_STATUS_HR           EQU     32
// G9K_STATUS_BD           EQU     16
// G9K_STATUS_MSC          EQU     4
// G9K_STATUS_EO           EQU     2
// G9K_STATUS_CE           EQU     1

// ; Fixed VRAM addresses
// G9K_SCRA_PAT_NAME_TABLE EQU     07C000h
// G9K_SCRB_PAT_NAME_TABLE EQU     07E000h
// G9K_P1_SPR_ATTRIB_TABLE EQU     03FE00h
// G9K_P2_SPR_ATTRIB_TABLE EQU     07BE00h

// G9K_CURSOR0_ATTRIB      EQU     07FE00h
// G9K_CURSOR1_ATTRIB      EQU     07FE08h

// G9K_CURSOR0_PAT_DATA    EQU     07FF00h
// G9K_CURSOR1_PAT_DATA    EQU     07FF80h

// G9K_RED                 EQU     32
// G9K_GREEN               EQU     1024
// G9K_BLUE                EQU     1


// G9K_WRITE_MASK_LAYER_A	EQU	000FFh
// G9K_WRITE_MASK_LAYER_B	EQU	0FF00h

