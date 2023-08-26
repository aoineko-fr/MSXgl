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

//#############################################################################
// I/O PORTS
//#############################################################################

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
//							│	└── MCS: The internally used master clock is selected.
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


//#############################################################################
// REGISTERS
//#############################################################################

//=============================================================================
// VRAM WRITE ADDRESS
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

//=============================================================================
// VRAM READ ADDRESS
//=============================================================================

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

//=============================================================================
// SCREEN MODE
//=============================================================================

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
#define V9_R06_BPP_MASK				0b00000011

#define V9_R06_WIDH_256				0b00000000	// 256 pixels wide
#define V9_R06_WIDH_512				0b00000100	// 512 pixels wide
#define V9_R06_WIDH_1024			0b00001000	// 1024 pixels wide
#define V9_R06_WIDH_2048			0b00001100	// 2048 pixels wide
#define V9_R06_WIDH_MASK			0b00001100

//												   MCS=0	MCS=1
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

//=============================================================================
// CONTROL
//=============================================================================

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
#define V9_R08						8
#define V9_REG_CTRL					8

#define V9_R08_VWM_ON				0b00001000
#define V9_R08_VWM_OFF				0

#define V9_R08_VWTE_ON				0b00010000
#define V9_R08_VWTE_OFF				0

#define V9_R08_YSE_ON				0b00100000
#define V9_R08_YSE_OFF				0

#define V9_R08_SPD_ON				0
#define V9_R08_SPD_OFF				0b01000000

#define V9_R08_DISP_ON				0b10000000
#define V9_R08_DISP_OFF				0

//=============================================================================
// INTERRUPT
//=============================================================================

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
#define V9_R09						9
#define V9_REG_INT_ENABLE			9

#define V9_R08_IEV_ON				0b00000001 // Interrupt enable during vertical retrace line interval
#define V9_R08_IEV_OFF				0

#define V9_R08_IEH_ON				0b00000010 // Display position interrupt enable (Interrupt position is specified with Interrupt Line, Interrupt X and IEHM.)
#define V9_R08_IEH_OFF				0

#define V9_R08_IECE_ON				0b00000100 // Command end interrupt enable control
#define V9_R08_IECE_OFF				0

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

#define V9_R09_EVERYLINE			0x80 // Selection of vertical position for display position interrupt

//-----------------------------------------------------------------------------
// R#12 - X interrupt register - Read/write
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	0	0	0	0	IX3	IX2	IX1	IX0
//					└───┴───┴───┴── Specification of horizontal position where display position interrupt occurs (Specified by unit of 64 master clock with the display start position as "0".)
#define V9_REG_INT_H_LINE			12

//=============================================================================
// PALETTE CONTROL
//=============================================================================

//-----------------------------------------------------------------------------
// R#13 - Palette control register - Write only
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

#define V9_R13_PLTM_PAL				0b00000000
#define V9_R13_PLTM_256				0b01000000
#define V9_R13_PLTM_YJK				0b10000000
#define V9_R13_PLTM_YUV				0b11000000
#define V9_R13_PLTM_MASK			0b11000000

#define V9_R13_YAE					0b00100000

#define V9_R13_PLTAIH				0b00010000

#define V9_R13_PLTO_MASK			0b00001111

//=============================================================================
// PALETTE POINTER
//=============================================================================

//-----------------------------------------------------------------------------
// R#14 - Palette pointer register - Write only
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	P5	P4	P3	P2	P1	P0	R1	R0
//	│	│	│	│	│	│	└───┴── Specification of RGB (0: R, 1: G, 2: B)
//	└───┴───┴───┴───┴───┴────────── Palette number (0-63)
#define V9_REG_PALETTE_PTR			14

#define V9_R14_PLTP_MASK			0b00000011
#define V9_R14_PLTA_MASK			0b11111100

//=============================================================================
// BACK DROP COLOR
//=============================================================================

//-----------------------------------------------------------------------------
// R#15 - Back drop color register - Read/write
//-----------------------------------------------------------------------------
#define V9_REG_BACK_DROP_COLOR 		15 // 6-bits color (0-63)

//=============================================================================
// DISPLAY ADJUST
//=============================================================================

//-----------------------------------------------------------------------------
// R#16 - Display adjust register - Read/write
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	V3	V2	V1	V0	H3	H2	H1	H0
//	│	│	│	│	└───┴───┴───┴── Horizontal display position adjustment (4 bits signed). Right << 8 9 ... 15 | 0 | 1 ... 6 7 >> Left
//	└───┴───┴───┴────────────────── Vertical display position adjustment (4 bits signed). Down << 8 9 ... 15 | 0 | 1 ... 6 7 >> Up
// Note: P1 and B1 by 1 pixel unit, P2, B2 and B3 by 2-pixel unit, B4, B5 and B6 by 4-pixel unit
#define V9_REG_DISPLAY_ADJUST		16

#define V9_R16_ADJV_MASK			0b11110000
#define V9_R16_ADJH_MASK			0b00001111

//=============================================================================
// SCROLL CONTROL
//=============================================================================

//-----------------------------------------------------------------------------
// R#17 - Layer A vertical scroll register (lower part) - Read/write
//-----------------------------------------------------------------------------
#define V9_REG_SCROLL_Y_LOW			17	// Scroll offset on Y axis (bits 0-7)

//-----------------------------------------------------------------------------
// R#18 - Layer A vertical scroll register (higher part) - Read/write
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	R1	R0	0	SY4	SY3	SY2	SY1	SY0
//	│	│		└───┴───┴───┴───┴── As display start coordinates for layer "A" of P1 mode and screens of other modes, X/Y-coordinates in the image space are specified by pixels. (When 16bpp is used in B2 and B3 modes, however, bit 0 is ignored and X-coordinate is specified by 2-pixel unit.)
//	└───┴────────────────────────── Number of roll page line in Y direction is specified.
//									R512  R256  Number of roll page line in Y direction is specified.
//									1     0     : Displayed page is rolled by 512 lines.
//									0     1     : Displayed page is rolled by 256 lines.
//									0     0     : Rolling by image space size takes place.
#define V9_REG_SCROLL_Y_HIGH		18

#define V9_R16_SCAY_MASK			0b00011111
#define V9_R16_R256					0b01000000
#define V9_R16_R512					0b10000000

//-----------------------------------------------------------------------------
// R#19 - Layer A horizontal scrolling register (lower part) - Read/write
//-----------------------------------------------------------------------------
#define V9_REG_SCROLL_X_LOW			19

//-----------------------------------------------------------------------------
// R#20 - Layer A horizontal scrolling register (higher part) - Read/write
//-----------------------------------------------------------------------------
#define V9_REG_SCROLL_X_HIGH		20

//-----------------------------------------------------------------------------
// R#21 - Layer B vertical scroll register (lower part) - Read/write
//-----------------------------------------------------------------------------
#define V9_REG_SCROLL_LOW_Y_B		21

//-----------------------------------------------------------------------------
// R#22 - Layer B vertical scroll register (higher part) - Read/write
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	SDA	SDB	0	0	0	0	SY1	SY0
//	│	│					└───┴── For layer "B" of P1, X/Y-coordinates are specified in the same way as for SCAX/SCAY.
//	│	└────────────────────────── Set to "1" to disable layer "B" and sprites.
//	└────────────────────────────── Set to "1" to disable layer "A" and sprites.
#define V9_REG_SCROLL_HIGH_Y_B		22

#define V9_R22_SDB					0b01000000
#define V9_R22_SDA					0b10000000

//-----------------------------------------------------------------------------
// R#23 - Layer B horizontal scrolling register (lower part) - Read/write
//-----------------------------------------------------------------------------
#define V9_REG_SCROLL_LOW_X_B		23

//-----------------------------------------------------------------------------
// R#24 - Layer B horizontal scrolling register (higher part) - Read/write
//-----------------------------------------------------------------------------
#define V9_REG_SCROLL_HIGH_X_B		24

//=============================================================================
// SPRITE GENERATOR BASE ADDRESS
//=============================================================================

//-----------------------------------------------------------------------------
// R#25 - Sprite generator table base address - Read/write
//-----------------------------------------------------------------------------
#define V9_REG_SRTP_GEN_TABLE		25

#define V9_P1_SGT_00000				0x00
#define V9_P1_SGT_08000				0x02
#define V9_P1_SGT_10000				0x04
#define V9_P1_SGT_18000				0x06
#define V9_P1_SGT_20000				0x08
#define V9_P1_SGT_28000				0x0A
#define V9_P1_SGT_30000				0x0C
#define V9_P1_SGT_38000				0x0E

#define V9_P2_SGT_00000				0x00
#define V9_P2_SGT_08000				0x01
#define V9_P2_SGT_10000				0x02
#define V9_P2_SGT_18000				0x03
#define V9_P2_SGT_20000				0x04
#define V9_P2_SGT_28000				0x05
#define V9_P2_SGT_30000				0x06
#define V9_P2_SGT_38000				0x07
#define V9_P2_SGT_40000				0x08
#define V9_P2_SGT_48000				0x09
#define V9_P2_SGT_50000				0x0A
#define V9_P2_SGT_58000				0x0B
#define V9_P2_SGT_60000				0x0C
#define V9_P2_SGT_68000				0x0D
#define V9_P2_SGT_70000				0x0E
#define V9_P2_SGT_78000				0x0F

//=============================================================================
// LCD CONTROL
//=============================================================================

//-----------------------------------------------------------------------------
// R#26 - LCD control - Read/write
//-----------------------------------------------------------------------------
#define V9_REG_LCD_CTRL				26

//=============================================================================
// PRIORITY CONTROL
//=============================================================================

//-----------------------------------------------------------------------------
// R#27 - Priority control - Read/write
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	0	0	0	0	PY1	PY0	PX1	PX0
//					│	│	└───┴── When in P1 mode, the display X-coordinate to exchange display priority order between layers "A" and "B" is specified. A unit of 64 pixels is used and in the range to the right of the specified pixel (including that pixel), layer "B" has priority. At "0", layer "A" has priority throughout the range.
//					└───┴────────── When in P1 mode, the display Y-coordinate to exchange display priority order between layers "A" and "B" is specified. A unit of 64 lines is used and in the range lower than the specified line (including that line), layer "B" has priority. At "0", layer "A" has priority throughout the range.
#define V9_REG_PRIORITY_CTRL		27

#define V9_R27_PRX_MASK				0b00000011
#define V9_R27_PRY_MASK				0b00001100

//=============================================================================
// SPRITE/CURSOR PALETTE OFFSET
//=============================================================================

//-----------------------------------------------------------------------------
// R#28 - Sprite/cursor palette offset - Write only
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	0	0	0	0	PO5	PO4	PO3	PO2
//					└───┴───┴───┴── Palette No. offset
#define V9_REG_SPR_PAL_CTRL			28

//=============================================================================
// COMMAND SOURCE
//=============================================================================

//-----------------------------------------------------------------------------
// R#32 - Source X coordinate (lower part) - Write only
//        Source linear address (lower part)
//-----------------------------------------------------------------------------
#define V9_REG_SX					32 // Specified with the X-coordinate of the image coordinate space (by pixels) and rolling to "0" occurs when a larger value than the image size is used for setting. (When in P1 mode, layer "A" is selected at SX9=0 and layer "B" at SX9=1.)
#define V9_REG_SX_LOW				32 // bits 0-7
#define V9_REG_SA					32 // Specified with the address on the VRAM map (by bytes).
#define V9_REG_SA_LOW				32 // bits 0-7

//-----------------------------------------------------------------------------
// R#33 - Source X coordinate (higher part) - Write only
//-----------------------------------------------------------------------------
#define V9_REG_SX_HIGH				33 // bits 8-10

//-----------------------------------------------------------------------------
// R#34 - Source Y coordinate (lower part) - Write only
//        Source linear address (middle part)
//-----------------------------------------------------------------------------
#define V9_REG_SY					34 // Specified with the Y-coordinate of the image coordinate space (by pixels) and rolling to "0" occurs when a larger value than the image size is used for setting.
#define V9_REG_SY_LOW				34 // bits 0-7
#define V9_REG_SA_MID				34 // bits 8-15

//-----------------------------------------------------------------------------
// R#35 - Source Y coordinate (higher part) - Write only
//        Source linear address (higher part)
//-----------------------------------------------------------------------------
#define V9_REG_SY_HIGH				35 // bits 8-11
#define V9_REG_SA_HIGH				35 // bits 16-18

//=============================================================================
// COMMAND DESTINATION
//=============================================================================

//-----------------------------------------------------------------------------
// R#36 - Destination X coordinate (lower part) - Write only
//        Destination linear address (lower part)
//-----------------------------------------------------------------------------
#define V9_REG_DX					36 // Specified with the X-coordinate of the image coordinate space (by pixels) and rolling to "0" occurs when a larger value than the image size is used for setting. (When in P1 mode, layer "A" is selected at DX9=0 and layer "B" at DX9=1.)
#define V9_REG_DX_LOW				36 // bits 0-7
#define V9_REG_DA					36 // Specified with the address on the VRAM map (by bytes).
#define V9_REG_DA_LOW				36 // bits 0-7

//-----------------------------------------------------------------------------
// R#37 - Destination X coordinate (higher part) - Write only
//-----------------------------------------------------------------------------
#define V9_REG_DX_HIGH				37 // bits 8-10

//-----------------------------------------------------------------------------
// R#38 - Destination Y coordinate (lower part) - Write only
//        Destination linear address (middle part)
//-----------------------------------------------------------------------------
#define V9_REG_DY					38 // Specified with the Y-coordinate of the image coordinate space (by pixels) and rolling to "0" occurs when a larger value than the image size is used for setting.
#define V9_REG_DY_LOW				38 // bits 0-7
#define V9_REG_DA_MID				38 // bits 8-15

//-----------------------------------------------------------------------------
// R#39 - Destination Y coordinate (higher part) - Write only
//        Destination linear address (higher part)
//-----------------------------------------------------------------------------
#define V9_REG_DY_HIGH				39 // bits 8-11
#define V9_REG_DA_HIGH				39 // bits 16-18

//=============================================================================
// COMMAND NUMBER
//=============================================================================

//-----------------------------------------------------------------------------
// R#40 - Transfer pixel number X (lower part) - Write only
//        Transfer linear number (lower part)
//        Transfer line major (lower part)
//-----------------------------------------------------------------------------
#define V9_REG_NX					40 // The number of pixels in X direction of the image coordinate space is specified and rolling to "0" occurs when the specified range exceeds the image size. (Maximum value is 2048 pixels when all "0".)
#define V9_REG_NX_LOW				40 // bits 0-7
#define V9_REG_NA					40 // NA: The number of bytes on the VRAM map is specified and rolling to address "0" occurs when the specified range exceeds the VRAM capacity. (Maximum value is 512K bytes when all "0".) Note: 256 bytes when NA bit 7-0 are all "0".
#define V9_REG_NA_LOW				40 // bits 0-7
#define V9_REG_MJ					40 // The length of the longer side of the rectangle whose diagonal line is the same as the drawn line is specified by pixels. When the specified range exceeds the image size, rolling to "0" occurs.
#define V9_REG_MJ_LOW				40 // bits 0-7

//-----------------------------------------------------------------------------
// R#41 - Transfer pixel number X (higher part) - Write only
//        Transfer line major (higher part)
//-----------------------------------------------------------------------------
#define V9_REG_NX_HIGH				41 // bits 8-10
#define V9_REG_MJ_HIGH				41 // bits 8-11

//-----------------------------------------------------------------------------
// R#42 - Transfer pixel number Y (lower part) - Write only
//        Transfer linear number (middle part)
//        Transfer line minor (lower part)
//-----------------------------------------------------------------------------
#define V9_REG_NY					42 // The number of pixels in Y direction of the image coordinate space is specified and rolling to "0" occurs when the specified range exceeds the image size. (Maximum value is 4096 pixels when all "0".)
#define V9_REG_NY_LOW				42 // bits 0-7
#define V9_REG_NA_MID				42 // bits 8-15
#define V9_REG_MI					42 // The length of the shorter side of the rectangle whose diagonal line is the same as the drawn line is specified by pixels. When the specified range exceeds the image size, rolling to "0" occurs.
#define V9_REG_MI_LOW				42 // bits 0-7

//-----------------------------------------------------------------------------
// R#43 - Transfer pixel number Y (higher part) - Write only
//        Transfer linear number (higher part)
//        Transfer line minor (higher part)
//-----------------------------------------------------------------------------
#define V9_REG_NY_HIGH				43 // bits 8-11
#define V9_REG_NA_HIGH				43 // bits 16-18
#define V9_REG_MI_HIGH				43 // bits 8-11

//=============================================================================
// COMMAND ARGUMENT
//=============================================================================

//-----------------------------------------------------------------------------
// R#44 - Argument - Write only
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	0	0	0	0	DIY	DIX	NEQ	MAJ
//					│	│	│	└── In the line inclination specification for LINE, X is the longer side and Y the shorter side at "0" while Y is the longer side and X the shorter side at "1".
//					│	│	└────── In the border color specification for SEARCH, "0" is for specified color detection and "1" is for non-specified color detection.
//					└───┴────────── The transfer direction (plus direction at "0" and minus direction at "1") from the base coordinates is specified. With BMXL and BMLX, however, the linear address specified side is fixed to "plus" and with BMLL, both X and Y are specified to the same direction.
#define V9_REG_ARG					44

#define V9_R44_MAJ					0b00000001
#define V9_R44_NEQ					0b00000010
#define V9_R44_DIX					0b00000100
#define V9_R44_DIY					0b00001000

//=============================================================================
// COMMAND LOGICAL OPERATION
//=============================================================================

//-----------------------------------------------------------------------------
// R#45 - Logical operation - Write only
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	0	0	0	TP	L11	L10	L01	L00
//				│	│	│	│	└── WC value for the bit with SC=0 and DC=0.
//				│	│	│	└────── WC value for the bit with SC=0 and DC=1.
//				│	│	└────────── WC value for the bit with SC=1 and DC=0.
//				│	└────────────── WC value for the bit with SC=1 and DC=1.
//				└────────────────── When this bit is "1", the data with SC as all "0" (by pixel for the X/Y-coordinates and by byte for the linear address) is not transferred.
#define V9_REG_LOP					45

#define V9_R45_LOP_MASK				0b00001111
#define V9_R45_LOP_SET				0b00001100
#define V9_R45_LOP_NOT				0b00000011
#define V9_R45_LOP_AND				0b00001000
#define V9_R45_LOP_OR				0b00001110
#define V9_R45_LOP_XOR				0b00000110

#define V9_R45_TP					0b00010000

//=============================================================================
// COMMAND WRITE MASK
//=============================================================================

//-----------------------------------------------------------------------------
// R#46 - Write mask (lower part) - Write only
//-----------------------------------------------------------------------------
#define V9_REG_WRITE_MASK			46
#define V9_REG_WRITE_MASK_LOW		46 // bits 0-7

//-----------------------------------------------------------------------------
// R#47 - Write mask (higher part) - Write only
//-----------------------------------------------------------------------------
#define V9_REG_WRITE_MASK_HIGH		47 // bits 8-15

//=============================================================================
// COMMAND FONT COLOR
//=============================================================================

//-----------------------------------------------------------------------------
// R#48 - Font color (lower part) - Write only
//-----------------------------------------------------------------------------
#define V9_REG_FC					48 // Color code of font data "1".
#define V9_REG_FC_LOW				48 // bits 0-7

//-----------------------------------------------------------------------------
// R#49 - Font color (higher part) - Write only
//-----------------------------------------------------------------------------
#define V9_REG_FC_HIGH				49 // bits 8-15

//-----------------------------------------------------------------------------
// R#48 - Font background color (lower part) - Write only
//-----------------------------------------------------------------------------
#define V9_REG_BC					50 // Color code of font data "0"
#define V9_REG_BC_LOW				50 // bits 0-7

//-----------------------------------------------------------------------------
// R#49 - Font background color (higher part) - Write only
//-----------------------------------------------------------------------------
#define V9_REG_BC_HIGH				51 // bits 8-15

//=============================================================================
// COMMAND OPERATION CODE
//=============================================================================

//-----------------------------------------------------------------------------
// R#52 - Operation code - Write only
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	OP3	OP2	OP1	OP0	AYM	AYE	AXM	AXE
//  │	│	│	│	│	│	└───┴── With PSET and ADVANCE, the drawing point can be shifted after executing the command.
//	│	│	│	│	│	│			AXE	AXM
//	│	│	│	│	│	│			 0	 0	: (DX,DY) is used as a pointer.
//	│	│	│	│	│	│			 0	 1	: The pointer is not shifted.
//	│	│	│	│	│	│			 1	 0	: The pointer is shifted to the right by 1 pixel.
//	│	│	│	│	│	│			 1	 1	: The pointer is shifted to the left by 1 pixel.
//	│	│	│	│	└───┴────────── AYE	AYM	
//	│	│	│	│					 0	 0	: The pointer is not shifted.
//	│	│	│	│					 0	 1	: The pointer is not shifted.
//	│	│	│	│					 1	 0	: The pointer is shifted down by 1 pixel.
//	│	│	│	│					 1	 1	: The pointer is shifted up by 1 pixel.
//	└───┴───┴───┴────────────────── Command code:
//									 0	STOP	: Command being executed is stopped.
//									 1	LMMC	: Data is transferred from CPU to VRAM rectangle area.
//									 2	LMMV	: VRAM rectangle area is painted out.
//									 3	LMCM	: VRAM rectangle area data is transferred to CPU.
//									 4	LMMM	: Rectangle area data is transferred from VRAM to VRAM.
//									 5	CMMC	: CPU character data is color-developed and transferred to VRAM rectangle area.
//									 7	CMMM	: VRAM character data is color-developed and transferred to VRAM rectangle area.
//									 8	BMXL	: Data on VRAM linear address is transferred to VRAM rectangle area.
//									 9	BMLX	: VRAM rectangle area data is transferred onto VRAM linear address.
//									 10	BMLL	: Data on VRAM linear address is transferred onto VRAM linear address.
//									 11	LINE	: Straight line is drawn on X/Y-coordinates.
//									 12	SEARCH	: Border color coordinates on X/Y space are detected.
//									 13	POINT	: Color code of specified point on X/Y-coordinates is read out.
//									 14	PSET	: Drawing is executed at drawing point on X/Y-coordinates.
//									 15	ADVANCE	: Drawing point on X/Y-coordinates is shifted.
#define V9_REG_OPCODE				52

#define V9_R52_AXE					0b00000001
#define V9_R52_AXM					0b00000010
#define V9_R52_AYE					0b00000100
#define V9_R52_AYM					0b00001000

#define V9_R52_CMD_MASK				0b11110000

#define V9_CMD_STOP					(0  << 4)	// Command being executed is stopped.
#define V9_CMD_LMMC					(1  << 4)	// Data is transferred from CPU to VRAM rectangle area.
#define V9_CMD_LMMV					(2  << 4)	// VRAM rectangle area is painted out.
#define V9_CMD_LMCM					(3  << 4)	// VRAM rectangle area data is transferred to CPU.
#define V9_CMD_LMMM					(4  << 4)	// Rectangle area data is transferred from VRAM to VRAM.
#define V9_CMD_CMMC					(5  << 4)	// CPU character data is color-developed and transferred to VRAM rectangle area.
#define V9_CMD_CMMM					(7  << 4)	// VRAM character data is color-developed and transferred to VRAM rectangle area.
#define V9_CMD_BMXL					(8  << 4)	// Data on VRAM linear address is transferred to VRAM rectangle area.
#define V9_CMD_BMLX					(9  << 4)	// VRAM rectangle area data is transferred onto VRAM linear address.
#define V9_CMD_BMLL					(10 << 4)	// Data on VRAM linear address is transferred onto VRAM linear address.
#define V9_CMD_LINE					(11 << 4)	// Straight line is drawn on X/Y-coordinates.
#define V9_CMD_SEARCH				(12 << 4)	// Border color coordinates on X/Y space are detected.
#define V9_CMD_POINT				(13 << 4)	// Color code of specified point on X/Y-coordinates is read out.
#define V9_CMD_PSET					(14 << 4)	// Drawing is executed at drawing point on X/Y-coordinates.
#define V9_CMD_ADVANCE				(15 << 4)	// Drawing point on X/Y-coordinates is shifted.

//=============================================================================
// COMMAND READ
//=============================================================================

//-----------------------------------------------------------------------------
// R#48 - Search command border coordinate (lower part) - Read only
//-----------------------------------------------------------------------------
#define V9_REG_BX					53 // When the border color is detected after the SEARCH command has been executed, the X-coordinate then can be read.
#define V9_REG_BX_LOW				53 // bits 0-7

//-----------------------------------------------------------------------------
// R#49 - Search command border coordinate (higher part) - Read only
//-----------------------------------------------------------------------------
#define V9_REG_BX_HIGH				54 // bits 8-10
