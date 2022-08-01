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

//=============================================================================
// DEFINES
//=============================================================================

// V9990 address structure
struct V9_Address
{
	u8 Lower;
	u8 Center;
	u8 Upper;
};

//
extern struct V9_Address g_V9_Address;

#define V9_REG(n)				n
#define V9_RII					0b01000000
#define V9_WII					0b10000000

//=============================================================================
// FUNCTIONS
//=============================================================================

//
bool V9_Detect();

//
void V9_SetMode(u8 mode);

//
void V9_ClearVRAM();

//
void V9_SetRegister(u8 reg, u8 val);

//
u8 V9_GetRegister(u8 reg);

//=============================================================================
//
// I/O PORTS
//
//=============================================================================

//-----------------------------------------------------------------------------
// P#0 - VRAM data port - Read/write
//-----------------------------------------------------------------------------
#define V9_P00						0x60
#define V9_PORT_VRAM				V9_P00
__sfr __at(V9_P00)					g_V9_VRAMPort;

//-----------------------------------------------------------------------------
// P#1 - Palette data port - Read/write
//-----------------------------------------------------------------------------
#define V9_P01						0x61
#define V9_PORT_PALETTE				V9_P01
__sfr __at(V9_P01)					g_V9_PalettePort;

// V9990 color trilet structure
struct V9_ColorTriplet
{
	u8 R		: 5;	// Red
	u8 unused1	: 2;
	u8 YS		: 1;
	u8 G		: 5;	// Green
	u8 unused2	: 3;
	u8 B		: 5;	// Blue
	u8 unused3	: 3;
};

#define V9_YS						0x80

//-----------------------------------------------------------------------------
// P#2 - Command data port - Read/write
//-----------------------------------------------------------------------------
#define V9_P02						0x62
#define V9_PORT_CMD_DATA			V9_P02
__sfr __at(V9_P02)					g_V9_CmdDataPort;

//-----------------------------------------------------------------------------
// P#3 - Register data port - Read/write
//-----------------------------------------------------------------------------
#define V9_P03						0x63
#define V9_PORT_REG_DATA			V9_P03
__sfr __at(V9_P03)					g_V9_RegDataPort;

//-----------------------------------------------------------------------------
// P#4 - Register select port - Write Only
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	WII	RII	R5	R4	R3	R2	R1	R0
//	│	│	└───┴───┴───┴───┴───┴── Register number (0-63)
//	│	└────────────────────────── Read increment inhibit (disable register automatic increment on read access)
//	└────────────────────────────── Write increment inhibit (disable register automatic increment on write access)
#define V9_P04						0x64
#define V9_PORT_REG_SELECT			V9_P04
__sfr __at(V9_P04)					g_V9_RegSelectPort;

#define V9_P04_RII					0b01000000
#define V9_P04_WII					0b10000000

// V9990 register select structure
struct V9_RegisterSelect
{
	u8 Number	: 6;
	u8 RII		: 1;
	u8 WII		: 1;
};

//-----------------------------------------------------------------------------
// P#5 - Status port - Read Only
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	TR	VR	HR	BD	0	MCS	E0	CE
//	│	│	│	│		│	│	└── Command being executed
//	│	│	│	│		│	└────── In the second field period during interlace
//	│	│	│	│		└────────── Content of P#7 MCS
//	│	│	│	└────────────────── Border color detect at completion of SRCH command (becomes "1" when detected)
//	│	│	└────────────────────── Horizontal non-display period
//	│	└────────────────────────── Vertical non-display period. It is set when the drawing of VRAM content has stopped.
//	└────────────────────────────── Command data transfer ready. It is reset through P#2 access.
#define V9_P05						0x65
#define V9_PORT_STATUS				V9_P05
__sfr __at(V9_P05)					g_V9_SatusPort;

#define V9_P05_CE					0b00000001
#define V9_P05_E0					0b00000010
#define V9_P05_MCS					0b00000100
#define V9_P05_BD					0b00010000
#define V9_P05_HR					0b00100000
#define V9_P05_VR					0b01000000
#define V9_P05_TR					0b10000000

// V9990 status structure
struct V9_Status
{
	u8 CE		: 1;
	u8 EO		: 1;
	u8 MCS		: 1;
	u8 unused	: 1;
	u8 BD		: 1;
	u8 HR		: 1;
	u8 VR		: 1;
	u8 TR		: 1;
};

//-----------------------------------------------------------------------------
// P#6 - Interrupt flag port - Read/write
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	0	0	0	0	0	CE	HI	VI
//						│	│	└── Vertical display period completion flag
//						│	└────── Display position flag
//						└────────── Command completion flag
#define V9_P06						0x66
#define V9_PORT_INT_FLAG			V9_P06
__sfr __at(V9_P06)					g_V9_IntFlagPort;

#define V9_P06_VI					0b00000001
#define V9_P06_HI					0b00000010
#define V9_P06_CE					0b00000100

// V9990 interrupt flag structure
struct V9_InterruptFlag
{
	u8 VI		: 1;
	u8 HI		: 1;
	u8 CE		: 1;
	u8 unused	: 5;
};

//-----------------------------------------------------------------------------
// P#7 - System control port - Write Only
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	0	0	0	0	0	0	SRS	MCS
//							│	└── MCS : The internally used master clock is selected.
//							│		 1: MCKIN terminal
//							│		 0: XTAL1 terminal
//							└────── Writing "1" will set all ports except this one in "power ON reset" state. "0" should be written to cancel it.
#define V9_P07						0x67
#define V9_PORT_SYS_CTRL			V9_P07
__sfr __at(V9_P07)					g_V9_SysCtrlPort;

#define V9_P07_MCS					0b00000001
#define V9_P07_SRS					0b00000010

#define V9_P07_MCKIN				0b00000001
#define V9_P07_XTAL1				0b00000000

// V9990 system control structure
struct V9_SystemControl
{
	u8 MCS		: 1;
	u8 SRS		: 1;
	u8 unused	: 6;
};

//-----------------------------------------------------------------------------
// P#8 - Primary standard Kanji ROM address port - Write Only
//-----------------------------------------------------------------------------
#define V9_P08						0x68	// not used in Gfx9000
#define V9_PORT_KANJI_ADDR_L		V9_P08
__sfr __at(V9_P08)					g_V9_KanjiAddrLPort;

//-----------------------------------------------------------------------------
// P#9 - Primary standard Kanji ROM port - Read/write
//-----------------------------------------------------------------------------
#define V9_P09						0x69	// not used in Gfx9000
// Write: Primary standard Kanji ROM address port
#define V9_PORT_KANJI_ADDR_H		V9_P09
__sfr __at(V9_P09)					g_V9_KanjiAddrHPort;
// Read: Primary standard Kanji ROM data port
#define V9_PORT_KANJI_DATA			V9_P09
__sfr __at(V9_P09)					g_V9_KanjiDataPort;

//-----------------------------------------------------------------------------
// Port 6Ah & 6Bh - Secondary standard Kanji ROM address port - Write Only
//-----------------------------------------------------------------------------
#define V9_P10						0x6A	// not used in Gfx9000
#define V9_PORT_KANJI2_ADDR_L		V9_P10
__sfr __at(V9_P10)					g_V9_Kanji2AddrLPort;

//-----------------------------------------------------------------------------
// P#11 - Secondary standard Kanji ROM port - Write Only
//-----------------------------------------------------------------------------
#define V9_P11						0x6B	// not used in Gfx9000
// Write: Secondary standard Kanji ROM address port
#define V9_PORT_KANJI2_ADDR_H		V9_P11
__sfr __at(V9_P11)					g_V9_Kanji2AddrHPort;
// Read: Secondary standard Kanji ROM data port
#define V9_PORT_KANJI2_DATA			V9_P11
__sfr __at(V9_P11)					g_V9_Kanji2DataPort;

// P#12 ~ P#14 - Reserved

//-----------------------------------------------------------------------------
// P#15 - Video9000 control register - Write only
//        V7040 superimpose chip
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	0	S1	S2	GEN	TRN	0	MIX	YM
//  	│	│	│	│		│	└── Half-tone mode is switched on when this bit is 1 (only valid when GEN is 1)
//		│	│	│	│		└────── Mixed mode is switched on when this bit is 1 (only valid when GEN is 1) 
//		│	│	│	└────────────── Transparent bit: When 0 the V9990 screen is made transparent completely and only the external video signal is visible (e.g. the MSX V9938/58 screen). This bit is only valid when GEN is 1.
//		│	│	└────────────────── Genlock enable bit: When this bit is 1 the genlock is enabled. Superimposition and digitization are possible now. After an MSX hardware RESET this bit is made 1.
//		└───┴────────────────────── Video source select: 0-1=RGB input is used, 2=CVBS input is used, 3=S-VHS input is used
#define V9_P15						0x6F	// Gfx9000 and Video9000
#define V9_PORT_OUTPUT_CTRL			V9_P15
__sfr __at(V9_P15)					g_V9_OutputCtrPort;

#define V9_P15_YM					0b00000001
#define V9_P15_MIX					0b00000010
#define V9_P15_TRN					0b00001000
#define V9_P15_GEN					0b00010000
#define V9_P15_S2					0b00100000
#define V9_P15_S1					0b01000000

// V9990 superimpose control structure
struct V9_SuperimposeControl
{
	u8 YM		: 1;
	u8 MIX		: 1;
	u8 unused1	: 1;
	u8 TRN		: 1;
	u8 GEN		: 1;
	u8 S2		: 1;
	u8 S1		: 1;
	u8 unused2	: 1;
};


//=============================================================================
//
// REGISTERS
//
//=============================================================================

//-----------------------------------------------------------------------------
// R#0 - Write VRAM lower address register - Write only
//-----------------------------------------------------------------------------
#define V9_REG_WRITE_ADDR			0
#define V9_REG_WRITE_ADDR_L			0	// VRAM address lower part (bits 0-7)

//-----------------------------------------------------------------------------
// R#1 - Write VRAM center address register - Write only
//-----------------------------------------------------------------------------
#define V9_REG_WRITE_ADDR_C			1	// VRAM address center part (bits 8-15)

//-----------------------------------------------------------------------------
// R#2 - Write VRAM upper address register - Write only
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	AII	0	0	0	0	A18	A17	A16
//	│					└───┴───┴── VRAM address upper part (bits 16-18)
//	└────────────────────────────── Disable automatic VRAM address increment
#define V9_REG_WRITE_ADDR_U			2	// VRAM address bits 16-18 & increment

//-----------------------------------------------------------------------------
// R#3 - Read VRAM lower address register - Write only
//-----------------------------------------------------------------------------
#define V9_REG_READ_ADDR			3
#define V9_REG_READ_ADDR_L			3	// VRAM address lower part (bits 0-7)

//-----------------------------------------------------------------------------
// R#4 - Read VRAM center address register - Write only
//-----------------------------------------------------------------------------
#define V9_REG_READ_ADDR_C			4	// VRAM address center part (bits 8-15)

//-----------------------------------------------------------------------------
// R#5 - Read VRAM upper address register - Write only
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	AII	0	0	0	0	A18	A17	A16
//	│					└───┴───┴── VRAM address upper part (bits 16-19)
//	└────────────────────────────── Disable automatic VRAM address increment
#define V9_REG_READ_ADDR_U			5	// Address bits 16-19 & increment

//-----------------------------------------------------------------------------
// R#6 - Screen mode 0 register - Read/write
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	DSPM	DCKM	XIMM	CLRM
//	│	│	│	│	│	│	└───┴── Selection of bit number per pixel
//	│	│	│	│	│	│			 0: 2 bits/pixel
//	│	│	│	│	│	│			 1: 4 bits/pixel
//	│	│	│	│	│	│			 2: 8 bits/pixel
//	│	│	│	│	│	│			 3: 16 bits/pixel
//	│	│	│	│	└───┴────────── Selection of number of pixels in X direction of image space (Number of pixels in Y direction is automatically calculated from XIMM and CLRM)
//	│	│	│	│					 0: 256 pixels wide
//	│	│	│	│					 1: 512 pixels wide
//	│	│	│	│					 2: 1024 pixels wide
//	│	│	│	│					 3: 2048 pixels wide
//	│	│	└───┴────────────────── Pixel clock selection (used in combination with MCS of P#7)
//	│	│							 0: XTAL1/4	*MCKIN/4	(Number of division of master clock) *Undocumented setting
//	│	│							 1: XTAL1/2	MCKIN/2
//	│	│							 2: XTAL1	MCKIN
//	└───┴────────────────────────── Display mode selection
//									 0: P1 mode
//									 1: P2 mode
//									 2: Bit map mode (Bl-6)
//									 3: Stand-by mode (non-display, no VRAM refresh, kanji ROM readable)
#define V9_R06						6
#define V9_REG_SCREEN_MODE0			6

#define V9_R06_BPP_2				0b00000000	// 4 colors per pixel
#define V9_R06_BPP_4				0b00000001	// 16 colors per pixel
#define V9_R06_BPP_8				0b00000010	// 256 colors per pixel
#define V9_R06_BPP_16				0b00000011	// 65K colors per pixel

#define V9_R06_WIDH_256				0b00000000	// 256 pixels wide
#define V9_R06_WIDH_512				0b00000100	// 512 pixels wide
#define V9_R06_WIDH_1024			0b00001000	// 1024 pixels wide
#define V9_R06_WIDH_2048			0b00001100	// 2048 pixels wide

#define V9_R06_CLOCK_4				0b00000000	// XTAL1/4	MCKIN/4
#define V9_R06_CLOCK_2				0b00010000	// XTAL1/2	MCKIN/2
#define V9_R06_CLOCK_1				0b00100000	// XTAL1	MCKIN
		
#define V9_R06_MODE_P1				0b00000000	// P1 mode
#define V9_R06_MODE_P2				0b01000000	// P2 mode
#define V9_R06_MODE_BITMAP			0b10000000	// Bitmap mode (Bl-6)
#define V9_R06_MODE_STANDBY			0b11000000	// Stand-by mode (non-display, no VRAM refresh, kanji ROM readable)

//-----------------------------------------------------------------------------
// R#7 - Screen mode 1 register - Read/write
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	0	C25	SM1	SM	PAL	EO	IL	HSCN
//		│	│	│	│	│	│	└── Selection of horizontal high scan mode (1: B5 and B6 modes; 0: other modes)
//		│	│	│	│	│	└────── Activate interlace mode (invalid when in B5 and B6 modes)
//		│	│	│	│	└────────── Selection of vertical resolution during interlace (invalid when in B5 and B6 modes). 1: Twice the vertical resolution during non-interlace; 0: Same as during non-interlace.
//		│	│	│	└────────────── Selection of PAL mode (invalid when in B5 and B6 modes). 0: NTSC; 1: PAL
//		│	│	└────────────────── Selection of horizontal frequency (invalid when in B5 and B6 modes). 1: 1H: fsc/227.5 (the sub-carrier phase is inverted for each line.); 0: 1H: fsc/228
//		│	└────────────────────── Selection of total number of vertical lines during non-interlace, NTSC. 1: 263 lines (In combination with SM, the sub-carrier phase is inverted for each frame.); 0: 262 lines
//		└────────────────────────── Selection of 640X480 mode, valid when HSCN is "1". 1: B6 mode; 0: other modes
#define V9_R07						7
#define V9_REG_SCREEN_MODE1			7

#define V9_R07_HSCN					0b00000001
#define V9_R07_HIGHSCAN_OFF			0b00000000
#define V9_R07_HIGHSCAN_ON			0b00000001

#define V9_R07_IL					0b00000010
#define V9_R07_INTERLACE_OFF		0b00000000
#define V9_R07_INTERLACE_ON			0b00000010

#define V9_R07_EO					0b00000100
#define V9_R07_VRES_OFF				0b00000000
#define V9_R07_VRES_ON				0b00000100

#define V9_R07_PAL					0b00001000
#define V9_R07_NTSC					0b00000000

#define V9_R07_SM					0b00010000

#define V9_R07_SM1					0b00100000
#define V9_R07_LINE_262				0b00000000
#define V9_R07_LINE_263				0b00100000

#define V9_R07_C25					0b01000000
#define V9_R07_480					0b01000000
#define V9_R07_400					0b00000000

//-----------------------------------------------------------------------------
// R#8 - Control register - Read/write
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	DIS	SPD	YSE	VWT	VWM	0	1	0
//	│	│	│	│	└────────────── VRAM write control during digitization
//	│	│	│	│					 0: Write transfer is not executed.
//	│	│	│	│					 1: Write transfer is executed during horizontal retrace line interval. (The data input into the serial data bus of VRAM during display period undergoes write transfer.)
//	│	│	│	└────────────────── VRAM serial data bus input/output control during digitization
//	│	│	│						 0: Read transfer is executed during horizontal retrace line interval. (The serial data bus of VRAM becomes an output terminal.)
//	│	│	│						 1: Dummy write transfer is executed during horizontal retrace line interval. (The serial data bus of VRAM becomes an input terminal.)
//	│	│	└────────────────────── YS signal output enable. 1: YS signal is output; 0: YS signal is not output. (YS terminal constantly remains as low level.)
//	│	└────────────────────────── Sprite (cursor) disable
//	└────────────────────────────── Screen display enable. 1: Display appears on screen according to the VRAM content.; 0: Back drop color is displayed all over the screen.
#define V9_REG_CTRL					8

//-----------------------------------------------------------------------------
// R#9 - Interrupt enable register - Read/write
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	0	0	0	0	0	IEC	IEH	IEV
//						│	│	└── Interrupt enable during vertical retrace line interval.
//						│	│		 0: INT0 terminal does not change according to VI flag.
//						│	│		 1: INT0 terminal becomes low level when VI flag of P#6 is "1".
//						│	└────── Display position interrupt enable (Interrupt position is specified with Interrupt Line, Interrupt X and IEHM.)
//						│			 0: INT1 terminal does not change according to HI flag.
//						│			 1: INT1 terminal becomes low level when HI flag of P#6 is "1".
//						└────────── Command end interrupt enable control
//									 0: INT0 terminal does not change according to CE flag.
//									 1: INT0 terminal becomes low level when CE flag of P#6 is "1".
#define V9_REG_INT_ENABLE			9

//-----------------------------------------------------------------------------
// R#10 - Line interrupt register (lower part) - Read/write
//-----------------------------------------------------------------------------
#define V9_REG_INT_V_LINE_LO		10	// Specification of vertical position where display position interrupt occurs (bits 0-7)
										// (Specified by means of line No. with the display start line as "0".)

//-----------------------------------------------------------------------------
// R#11 - Line interrupt register (higher part) - Read/write
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	IEH	0	0	0	0	0	IL9	IL8
//	│						└───┴── Specification of vertical position where display position interrupt occurs (bits 8-9)
//	└────────────────────────────── Selection of vertical position for display position interrupt
//									 0: Specified line
//									 1: Every line (value of Interrupt Line is ignored)
#define V9_REG_INT_V_LINE_HI		11

//-----------------------------------------------------------------------------
// R#12 - X interrupt register - Read/write
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	0	0	0	0	IX3	IX2	IX1	IX0
//					└───┴───┴───┴── Specification of horizontal position where display position interrupt occurs (Specified by unit of 64 master clock with the display start position as "0".)
#define V9_REG_INT_H_LINE			12

//-----------------------------------------------------------------------------
// R#13 - Palettte control register - Write only
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	TM1	TM0	YAE	PIH	TO5	TO4	TO3	TO2
//	│	│	│	│	└───┴───┴───┴── Still screen palette No. offset
//	│	│	│	└────────────────── Palette No. increment control at color palette read-out
//	│	│	│						 0: After P#1 read-out, R#14 palette pointer undergoes an increment.
//	│	│	│						 1: R#14 palette pointer is not changed by P#1 read-out.
//	│	│	└────────────────────── Selection of YJK (YUV) and RGB mixing mode (valid when in YUV and YJK modes)
//	│	│							 0: Only YJK (YUV) image is displayed.
//	│	│							 1: YJK (YUV) and RGB images are displayed together.
//	└───┴────────────────────────── Selection of color palette mode
//									 0: Palette mode
//									 1: 256 color mode
//									 2: YJK mode
//									 3: YUV mode
#define V9_REG_PALETTE_CTRL			13

#define V9_R13_COL_PAL				0b00000000
#define V9_R13_COL_256				0b01000000
#define V9_R13_COL_YJK				0b10000000
#define V9_R13_COL_YUV				0b11000000

//-----------------------------------------------------------------------------
// R#14 - Palette pointer register - Write only
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	P5	P4	P3	P2	P1	P0	R1	R0
//	│	│	│	│	│	│	└───┴── RGB (0: R, 1: G, 2: B)
//	└───┴───┴───┴───┴───┴────────── Palette number (0-63)
#define V9_REG_PALETTE_PTR			14

//-----------------------------------------------------------------------------
// R#15 - Back drop color register - Read/write
//-----------------------------------------------------------------------------
#define V9_REG_BACK_DROP_COLOR 		15 // 6-bits color (0-63)

//-----------------------------------------------------------------------------
// R#16 - Display adjust register - Read/write
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	V3	V2	V1	V0	H3	H2	H1	H0
//	│	│	│	│	└───┴───┴───┴── Horizontal display position adjustment (4 bits signed). Right << 8 9 ... 15 | 0 | 1 ... 6 7 >> Left
//	└───┴───┴───┴────────────────── Vertical display position adjustment (4 bits signed). Down << 8 9 ... 15 | 0 | 1 ... 6 7 >> Up
// Note: P1 and B1 by 1 pixel unit, P2, B2 and B3 by 2-pixel unit, B4, B5 and B6 by 4-pixel unit
#define V9_REG_DISPLAY_ADJUST		16


//-----------------------------------------------------------------------------
// R#17 - Layer A vertical scroll register (lower part) - Read/write
//-----------------------------------------------------------------------------
#define V9_REG_SCROLL_Y_LOW			17	// Scroll offset on Y axis (bits 0-7)

//-----------------------------------------------------------------------------
// R#18 - Layer A vertical scroll register (higher part) - Read/write
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	R1	R0	0	SY4	SY3	SY2	SY1	SY0
//	│	│		└───┴───┴───┴───┴── Horizontal display position adjustment (4 bits signed). Right << 8 9 ... 15 | 0 | 1 ... 6 7 >> Left
//	└───┴────────────────────────── Vertical display position adjustment (4 bits signed). Down << 8 9 ... 15 | 0 | 1 ... 6 7 >> Up
#define V9_REG_SCROLL_Y_HIGH		18

//-----------------------------------------------------------------------------
// R#19 - Layer A horizontal scrolling register (lower  part) - Read/write
//-----------------------------------------------------------------------------
#define V9_REG_SCROLL_X_LOW			19

//-----------------------------------------------------------------------------
// R#20 - Layer A horizontal scrolling register (higher part) - Read/write
//-----------------------------------------------------------------------------
#define V9_REG_SCROLL_X_HIGH		20

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_SCROLL_LOW_Y_B		21	// R/W

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_SCROLL_HIGH_Y_B		22	// R/W

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_SCROLL_LOW_X_B		23	// R/W

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_SCROLL_HIGH_X_B		24	// R/W

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_PAT_GEN_TABLE   		25	// R/W

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_LCD_CTRL        		26	// R/W

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_PRIORITY_CTRL  		27	// R/W

//-----------------------------------------------------------------------------
// R#18 - Sprite palettte register - Write only
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	0	0	0	0	PO5	PO4	PO3	PO2
//					└───┴───┴───┴── PLTO
#define V9_REG_SPR_PAL_CTRL			28

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_SC_X					32	// W

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_SC_Y					34	// W

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_DS_X					36	// W

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_DS_Y					38	// W

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_NX					40	// W

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_NY					42	// W

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_ARG					44	// W

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_LOP					45	// W

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_WRITE_MASK			46	// W

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_FC					48	// W

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_BC					50	// W

//-----------------------------------------------------------------------------
// R# -  - 
//-----------------------------------------------------------------------------
#define V9_REG_OPCODE				52	// W


//=============================================================================
//
// SCREEN MODE
//
//=============================================================================

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

