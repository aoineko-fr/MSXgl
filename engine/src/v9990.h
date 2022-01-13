// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// References:
// - 
//─────────────────────────────────────────────────────────────────────────────
#pragma once
#include "core.h"

		
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

/// Port defines
#define V9_PORT_VRAM				0x60	// R/W
#define V9_PORT_PALETTE				0x61	// R/W
#define V9_PORT_CMD_DATA			0x62	// R/W
#define V9_PORT_REG_DATA			0x63	// R/W
#define V9_PORT_REG_SELECT			0x64	// W
#define V9_PORT_STATUS				0x65	// R
#define V9_PORT_INT_FLAG			0x66	// R/W
#define V9_PORT_SYS_CTRL			0x67	// W
#define V9_PORT_OUTPUT_CTRL     	0x6F	// R/W

// ; Bit defines G9K_SYS_CTRL
// G9K_SYS_CTRL_SRS		EQU	2	; Power on reset state
// G9K_SYS_CTRL_MCKIN	EQU	1	; Select MCKIN terminal
// G9K_SYS_CTRL_XTAL	EQU	0	; Select XTAL

/// Register defines
#define V9_REG_WRITE_ADDR			0	// W
#define V9_REG_READ_ADDR			3	// W
#define V9_REG_SCREEN_MODE0			6	// R/W
#define V9_REG_SCREEN_MODE1			7	// R/W
#define V9_REG_CTRL					8	// R/W
#define V9_REG_INT_ENABLE			9	// R/W
#define V9_REG_INT_V_LINE_LO		10	// R/W	
#define V9_REG_INT_V_LINE_HI		11	// R/W
#define V9_REG_INT_H_LINE			12	// R/W	
#define V9_REG_PALETTE_CTRL			13	// W
#define V9_REG_PALETTE_PTR			14	// W
#define V9_REG_BACK_DROP_COLOR 		15	// R/W
#define V9_REG_DISPLAY_ADJUST		16	// R/W
#define V9_REG_SCROLL_LOW_Y			17	// R/W
#define V9_REG_SCROLL_HIGH_Y		18	// R/W
#define V9_REG_SCROLL_LOW_X			19	// R/W
#define V9_REG_SCROLL_HIGH_X		20	// R/W
#define V9_REG_SCROLL_LOW_Y_B		21	// R/W
#define V9_REG_SCROLL_HIGH_Y_B		22	// R/W
#define V9_REG_SCROLL_LOW_X_B		23	// R/W
#define V9_REG_SCROLL_HIGH_X_B		24	// R/W
#define V9_REG_PAT_GEN_TABLE   		25	// R/W
#define V9_REG_LCD_CTRL        		26	// R/W
#define V9_REG_PRIORITY_CTRL  		27	// R/W
#define V9_REG_SPR_PAL_CTRL			28	// W
#define V9_REG_SC_X					32	// W
#define V9_REG_SC_Y					34	// W
#define V9_REG_DS_X					36	// W
#define V9_REG_DS_Y					38	// W
#define V9_REG_NX					40	// W
#define V9_REG_NY					42	// W
#define V9_REG_ARG					44	// W
#define V9_REG_LOP					45	// W
#define V9_REG_WRITE_MASK			46	// W
#define V9_REG_FC					48	// W
#define V9_REG_BC					50	// W
#define V9_REG_OPCODE				52	// W

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

// ; Mode select defines for SetScreenMode
// G9K_MODE_P1		EQU	0	; Pattern mode 0 256 212
// G9K_MODE_P2		EQU	1	; Pattern mode 1 512 212
// G9K_MODE_B1		EQU	2	; Bitmap mode 1 256 212
// G9K_MODE_B2		EQU	3	; Bitmap mode 2 384 240
// G9K_MODE_B3		EQU	4	; Bitmap mode 3 512 212
// G9K_MODE_B4		EQU	5	; Bitmap mode 4 768 240
// G9K_MODE_B5		EQU	6	; Bitmap mode 5 640 400 (VGA)
// G9K_MODE_B6		EQU	7	; Bitmap mode 6 640 480 (VGA)
// G9K_MODE_B7		EQU	8	; Bitmap mode 7 1024 212 (Undocumented v9990 mode)

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

