// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄ ▄▄▄  ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██ █ ██▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▀  ██▄▀ ██  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Features to manage the VDP
//
// References:
// - V9938 Technical Data Book Programmer's Guide
// - https://www.msx.org/wiki/Category:VDP_Registers
// - http://map.grauw.nl/articles/
//─────────────────────────────────────────────────────────────────────────────
 #pragma once

//-----------------------------------------------------------------------------
//
//   V D P   R E G I S T E R S
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// R#0								Mode Register 0
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	0	DG	IE2	IE1	M5	M4	M3	EV	
//  	│	│	│	│	│	│	└── EV is the bit to enable the external VDP input (Disabled by default)
//		│	│	│	└───┴───┴────── M3-5 are bits are used with M1-2 bits of register 1 to define the VDP screen mode
//		│	│	└────────────────── IE2 enables (1) or disables (0) the light Pen interrupts (Used only by Korean MSX2 Daewoo CPC-300 and CPC-400/400S, always reset it otherwise)
//		│	└────────────────────── IE1 enables (1) or disables (0) the horizontal retrace interrupts indicated by the register 19 (V9938 & V9958 only)
//		└────────────────────────── DG specify the direction of the color bus. Write 1 to set the color bus as input in order to recover data in VRAM on MSX equipped with a digitizer (V9938 & V9958 only)
#define R00_EV						(0b00000001)
#define R00_M3						(0b00000010)
#define R00_M4						(0b00000100)
#define R00_M5						(0b00001000)
#define R00_IE1						(0b00010000)
#define R00_IE2						(0b00100000)
#define R00_DG						(0b01000000)

//-----------------------------------------------------------------------------
// R#1								Mode Register 1
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
// 	K16	BL	IE0	M2	M1	0	SI	MAG
//  │	│	│	│	│		│	└── MAG enlarges the sprites when 1 is written. (0 by default)
//  │	│	│	│	│		└────── SI defines the sprite size. Write 1 to use 16x16 sprites, 0 to usinge 8x8 sprites
//  │	│	│	└───┴────────────── M1-2 are bits are used with M3-5 bits of register 0 to define the VDP screen mode
//  │	│	└────────────────────── IE0 enables (1) or disable (0) the vertical retrace interrupts that occur at just after each display of the screen (foreground)
//  │	└────────────────────────── BL disables the screen display when reseted. VDP's commands work a bit faster as well. Screen display is displayed by default
//  └────────────────────────────── 4/16K selects VRAM configuration. Write 1 if the VDP is not a V9938 nor V9958
#define R01_MAG						(0b00000001)
#define R01_ST						(0b00000010)
#define R01_M1						(0b00001000)
#define R01_M2						(0b00010000)
#define R01_IE0						(0b00100000)
#define R01_BL						(0b01000000)
#define R01_K16						(0b10000000)

//-----------------------------------------------------------------------------
// R#2								Patern Layout Table
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	0	N16	N15	N14	N13	N12	N11	N10
//		└───┴───┴───┴───┴───┴───┴──	In MSX1 screen and SCREEN 4 modes, N10-N13 bits correspond to the four most significant bits of the address that defines the start of the character table in VRAM. Other bits must be always reseted.
//									In other SCREEN modes N10-N14 bits must always be set, and N15-N16 bits determine the page number.
// 									In KANJI screens modes, it will vary according using of Bitmap screen 5 or 7.
#define R02_SET(_n)					((_n >> 10) & 0x7F)
#define R02_DEFAULT_S0W40			0b00000000	// 0x0000
#define R02_DEFAULT_S0W80			0b00000011	// 0x0C00
#define R02_DEFAULT_S1				0b00000110	// 0x1800
#define R02_DEFAULT_S2				0b00000110	// 0x1800
#define R02_DEFAULT_S3				0b00000010	// 0x0800
#define R02_DEFAULT_S4				0b00000110	// 0x1800
#define R02_DEFAULT_S5				0b00011111	// 0x7C00
#define R02_DEFAULT_S6				0b00011111	// 0x7C00
#define R02_DEFAULT_S7				0b00011111	// 0x7C00
#define R02_DEFAULT_S8				0b00011111	// 0x7C00
#define R02_DEFAULT_S9				0b00111111	// 0xFC00
#define R02_DEFAULT_S10				0b00011111	// 0x7C00
#define R02_DEFAULT_S11				0b00011111	// 0x7C00
#define R02_DEFAULT_S12				0b00011111	// 0x7C00

//-----------------------------------------------------------------------------
// R#3								Color Table Low
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	C13	C12	C11	C10	C9	C8	C7	C6
//	└───┴───┴───┴───┴───┴───┴───┴── In the SCREEN modes 0, 1 and 3, C6-C13 bits define the address of the colors table.
//									In SCREEN 2 and 4, the operation is different. Only C13 can be changed, the other bits must be always set.
// 									In KANJI screens modes, it will vary according using of Bitmap screen 5 or 7.
#define R03_SET(_n)					((_n >> 6) & 0xFF)
#define R03_DEFAULT_S0W80			0b00100111	// 0x09C0
#define R03_DEFAULT_S1				0b10000000	// 0x2000
#define R03_DEFAULT_S2				0b11111111	// 0x1800
#define R03_DEFAULT_S4				0b11111111	// 0x3FC0

//-----------------------------------------------------------------------------
// R#4								Pattern Generator Table
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	0	0	F16	F15	F14	F13	F12	F11
//			└───┴───┴───┴───┴───┴── In the SCREEN modes 0, 1 and 3, F11-F13 bits code the 3 most significant bits of the address of the VRAM pattern table.
//									In the SCREEN modes 2 and 4, only the F13 bit defines the pattern table address. Thus, it can only start at the address 0000h or 2000h.
//									In the other SCREEN modes, F11-F16 bits code the 6 most significant bits of the address at the start of the pattern table.
#define R04_SET(_n)					((_n >> 11) & 0x3F)
#define R04_DEFAULT_S0W40			0b00000001	// 0x0800
#define R04_DEFAULT_S0W80			0b00000010	// 0x1000
#define R04_DEFAULT_S1				0b00000000	// 0x0000
#define R04_DEFAULT_S2				0b00000011	// 0x1800
#define R04_DEFAULT_S3				0b00000000	// 0x0000
#define R04_DEFAULT_S4				0b00000011	// 0x1800

//-----------------------------------------------------------------------------
// R#5								Sprite Attribute Table Low
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	S14	S13	S12	S11	S10	S9	S8	S7
//	└───┴───┴───┴───┴───┴───┴───┴── In SCREEN 1 to 3, S7-S13 bits code the 7 most significant bits of address of the sprites attribute table.
//									In SCREEN 4 to 6, the S7-S8 bits are ignored and S9 must be always set. So the address is obtained by the formula ((Register 5 value) - 3) x 80h.
//									In SCREEN 7, 8, 10 to 12, S7-S14 are used with S15-S16 of the VDP register 11. 
#define R05_SET(_n)					((_n >> 7) & 0xFF)
#define R05_DEFAULT_S1				0b00110110	// 0x1B00
#define R05_DEFAULT_S2				0b00110110	// 0x1B00
#define R05_DEFAULT_S3				0b00110110	// 0x1B00
#define R05_DEFAULT_S4				0b00111111	// 0x1E00
#define R05_DEFAULT_S5				0b11101111	// 0x7600
#define R05_DEFAULT_S6				0b11101111	// 0x7600
#define R05_DEFAULT_S7				0b11110111	// 0xFA00
#define R05_DEFAULT_S8				0b11110111	// 0xFA00
#define R05_DEFAULT_S10				0b11110111	// 0xFA00
#define R05_DEFAULT_S11				0b11110111	// 0xFA00
#define R05_DEFAULT_S12				0b11110111	// 0xFA00

//-----------------------------------------------------------------------------
// R#6								Sprite Pattern Generator Table
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	0	0	P16	P15	P14	P13	P12	P11
//			└───┴───┴───┴───┴───┴── In MSX1 screen modes, P11-P13 encode the 3 most significant bits of the address of the Sprite Pattern in VRAM. Other bits must be reseted.
//									In other screen modes, P11-P16 code the 6 most significant bits of the Sprite Pattern address.
#define R06_SET(_n)					((_n >> 11) & 0x3F)
#define R06_DEFAULT_S1				0b00000111	// 0x3800
#define R06_DEFAULT_S2				0b00000111	// 0x3800
#define R06_DEFAULT_S3				0b00000111	// 0x3800
#define R06_DEFAULT_S4				0b00000111	// 0x3800
#define R06_DEFAULT_S5				0b00001111	// 0x7800
#define R06_DEFAULT_S6				0b00001111	// 0x7800
#define R06_DEFAULT_S7				0b00011110	// 0xF000
#define R06_DEFAULT_S8				0b00011110	// 0xF000
#define R06_DEFAULT_S10				0b00011110	// 0xF000
#define R06_DEFAULT_S11				0b00011110	// 0xF000
#define R06_DEFAULT_S12				0b00011110	// 0xF000

//-----------------------------------------------------------------------------
// R#7								Text and Screen Margin Color
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	TC3	TC2	TC1	TC0	BD3	BD2	BD1	BD0
//	│	│	│	│	└───┴───┴───┴── TC0-TC3 define the text color in screen 0. Other bits are ignored in the other screen modes except in SCREEN 8 and 12. In these two modes, all the bits of the register define the border color.
//	└───┴───┴───┴────────────────── BD0-BD3 defines the border color. In SCREEN 6, only the bits BD0 and BD1 define the border color and when the bit BD3 is 0, the even lines of the border take the background color.
#define R07_SET(_a, _b)				(((_a & 0x0F) << 4) + (_b & 0x0F))

#if (MSX_VERSION >= MSX_2)
//-----------------------------------------------------------------------------
// R#8								Mode Register 8
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	MS	LP	TP	CB	VR	0	SPD	BW	
//  │	│	│	│	│		│	└── BW defines the display to grayscale in 32 tones (1) or in colour. (0 by default)
//  │	│	│	│	│		└────── SPD allows us to disable the sprites. The VDP commands work a little faster when the sprites are disabled (1). (Sprites are enabled by default)
//  │	│	│	│	└────────────── VR defines the type of vram chips used. Write 1 if the VRAM is 64K x 1 bit or 64K x 4 bits, 0 if VRAM is 16K x 1 bit or 16K x 4 bits. This bit Affects how VDP performs refresh on DRAM chips.
//  │	│	│	└────────────────── CB define the direction of the color bus. 1 for input, 0 for output (Default value).
//  │	│	└────────────────────── TP must be 0 to make the color 0 transparent. On machines with video input, the image of the video signal received becomes visible instead of the color 0. When 1 these dots are black by default. The color can be changed by defining the palette. TP also affects sprites in the same way.
//  │	└────────────────────────── LP serves to enable the light pen. (Used only Korean MSX2 Daewoo CPC-300 and CPC-400 / 400S, set to 0 otherwise.) (Does not exist on V9958.)
//  └────────────────────────────── MS serves to enable the mouse mode. (Not used on MSX computers, always reset it) (Does not exist on V9958.)
#define R08_BW						(0b00000001)
#define R08_SPD						(0b00000010)
#define R08_VR						(0b00001000)
#define R08_CB						(0b00010000)
#define R08_TP						(0b00100000)
#define R08_PL						(0b01000000)
#define R08_MS						(0b10000000)

//-----------------------------------------------------------------------------
// R#9								Mode Register 9
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	LN	0	S1	S0	IL	EO	NT	DC
//   │		│	│	│	│	│	└── DC is to define the direction of the signal dot clock. 1 sets DLCLK pin as input, 0 as output.
//   │		│	│	│	│	└────── NT define the frames display frequency for the RGB output. Write 1 for PAL TV (313 lines, 50Hz), 0 for NTSC TV (262 lines, 60Hz).
//   │		│	│	│	└────────── EO bit is used to activate the display of an even and odd page alternately, 1 frame out of two, for the second period in SCREEN 5 to 12 modes. (For more details, see register 13.)
//   │		│	│	└────────────── IL is for displaying with interlaced lines. Write 1 to select the display with interlacing, 0 to select the display non-interlaced.
//   │		└───┴────────────────── S1-S0 bits select the mode used simultaneously (00 = Foreground (Default value), 01 = Digitizer or Superimposition device, 10 = external video).
//   └───────────────────────────── LN defines the height of the screen. 1 for 212 lines, 0 for 192 (Default value).
#define R09_DC						(0b00000001)
#define R09_NT						(0b00000010)
#define R09_EO						(0b00000100)
#define R09_IL						(0b00001000)
#define R09_S0						(0b00010000)
#define R09_S1						(0b00100000)
#define R09_LN						(0b10000000)

//-----------------------------------------------------------------------------
// R#10								Color Table High
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	0	0	0	0	0	C16	C15	C14
//						└───┴───┴── Cf. R#3
#define R10_SET(_n)					((_n >> 14) & 0x07)
#define R10_DEFAULT_S0W80			0	// 0x09C0
#define R10_DEFAULT_S1				0	// 0x2000
#define R10_DEFAULT_S2				0	// 0x1800
#define R10_DEFAULT_S4				0	// 0x3FC0

//-----------------------------------------------------------------------------
// R#11								Sprite Attribute Table High
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	0	0	0	0	0	0	S16	S15
//							└───┴── Cf. R#5
#define R11_SET(_n)					((_n >> 15) & 0x03)
#define R11_DEFAULT_S1				0	// 0x1B00
#define R11_DEFAULT_S2				0	// 0x1B00
#define R11_DEFAULT_S3				0	// 0x1B00
#define R11_DEFAULT_S4				0	// 0x1E00
#define R11_DEFAULT_S5				1	// 0x7600
#define R11_DEFAULT_S6				1	// 0x7600
#define R11_DEFAULT_S7				1	// 0xFA00
#define R11_DEFAULT_S8				1	// 0xFA00
#define R11_DEFAULT_S10				1	// 0xFA00
#define R11_DEFAULT_S11				1	// 0xFA00
#define R11_DEFAULT_S12				1	// 0xFA00

//-----------------------------------------------------------------------------
// R#12								Text and Background Blink Color
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	T23	T22	T21	T20	BC3	BC2	BC1	BC0
//	│	│	│	│	└───┴───┴───┴── T20-T23 bits is text blink color in SCREEN 0 width 41-80
//	└───┴───┴───┴────────────────── BC0-BC3 bits is background blink color in SCREEN 0 width 41-80
#define R12_SET(_a, _b)				(((_a & 0x0F) << 4) + (_b & 0x0F))

//-----------------------------------------------------------------------------
// R#13								Blink Period Register
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	ON3	ON2	ON1	ON0	OF3	OF2	OF1	OF0
//	│	│	│	│	└───┴───┴───┴── ON0-ON3 bits set the time for the second period
//	└───┴───┴───┴────────────────── OF0-OF3 bits set the time for the first period and enables the periodic display when any of these bits are set to 1.
#define R13_SET(_a, _b)				(((_a & 0x0F) << 4) + (_b & 0x0F))

//-----------------------------------------------------------------------------
// R#14								VRAM Access Base Register
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	0	0	0	0	0	A16	A15	A14
//						└───┴───┴── A14-A16 contains three senior bits of VRAM access address.
//									In all modes, except GRAPHIC1, GRAPHIC2, MULTICOLOR and TEXT1, if there’s a carry flag from A13 the value in this register is automatically incremented.

//-----------------------------------------------------------------------------
// R#15								Status Register Pointer
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	0	0	0	0	S3	S2	S1	S0
//					└───┴───┴───┴── S0-S3 points to the respective status register (S#0...S#9) to be read.

//-----------------------------------------------------------------------------
// R#16								Color Palette Address Register
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	0	0	0	0	C3	C2	C1	C0
//					└───┴───┴───┴── C0-C3 points to the respective color palette register (P#0...P#15) to be accessed. 

//-----------------------------------------------------------------------------
// R#17								Control Register Pointer 
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	AII	0	RS5	RS4	RS3	RS2	RS1	RS0    
//	│		└───┴───┴───┴───┴───┴── RS0-RS5 define the number of the control register to write. (It can be 0 to 23 and 32 to 46 on MSX2 and higher, 25 to 27 on MSX2+ and higher)
//	└────────────────────────────── AII enables the auto-increment of the register number after each writing when 1. (0 initially)

//-----------------------------------------------------------------------------
// R#18								Display Adjust Register
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	V3	V2	V1	V0	H3	H2	H1	H0
//	│	│	│	│	└───┴───┴───┴── V0-V3 bits serve to adjust the screen vertical location. (0 at initialisation)
//	└───┴───┴───┴────────────────── H0-H3 bits serve to adjust the screen horizontal location. (0 at initialisation)
#define R18_DEFAULT					0

//-----------------------------------------------------------------------------
// R#19								Interrupt Line Register
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	IL7	IL6	IL5	IL4	IL3	IL2	IL1	IL0
//	└───┴───┴───┴───┴───┴───┴───┴── This register defines the line number where a programmed interrupt must happen when horizontal retrace interrupts are enabled (value 1 in bit 4 - IE1 - of control register 0).
#define R19_DEFAULT					0

//-----------------------------------------------------------------------------
// R#20								Color Burst Register 1
//-----------------------------------------------------------------------------
#define R20_DEFAULT					0b00000000

//-----------------------------------------------------------------------------
// R#21								Color Burst Register 2
//-----------------------------------------------------------------------------
#define R21_DEFAULT					0b00111011

//-----------------------------------------------------------------------------
// R#22								Color Burst Register 3
//-----------------------------------------------------------------------------
#define R22_DEFAULT					0b00000101

//-----------------------------------------------------------------------------
// R#23								Vertical Offset Register
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	DO7	DO6	DO5	DO4	DO3	DO2	DO1	DO0
//	└───┴───┴───┴───┴───┴───┴───┴── This register sets the value of the first line to display on the screen
#define R23_DEFAULT					0

//-----------------------------------------------------------------------------
// R#24

#endif // (MSX_VERSION >= MSX_2)

#if (MSX_VERSION >= MSX_2P)
//-----------------------------------------------------------------------------
// R#25
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	0	CMD	VDS	YAE	YJK	WTE	MAK	SP2
//  	│	│	│	│	│	│	└── SP2 sets the horizontal scrolling of the screen on two pages when 0 (Default value) When 1, the horizontal scrolling occurs on a single page.
//  	│	│	│	│	│	└────── MAK allows to hide the first 8 vertical lines at left of screen (8 lines in screens 5, 8, 10 to 12 - 16 lines in screens 6 and 7) when set. (0 by default)
//  	│	│	│	│	└────────── WTE enables the WAIT function when set. (0 by default) (not used on MSX computers)
//  	│	│	│	└────────────── YJK must set and the YJK bit reset to get the screen 12 mode instead of screen 8. (0 by default)
//  	│	│	└────────────────── YAE must set with the YJK bit to get the screen 10/11 mode instead of screen 8. (0 by default)
//  	│	└────────────────────── VDS should always be to 0 because it determines the function of the VDP pin 8 which clocks the Z80A to 3.579545 MHz. If this bit is set the pin 8 becomes a VDS output.
//  	└────────────────────────── CMD enables the VDP commands for screens 0 to 4 when 1. Coordinates work like in SCREEN 8. This bit is reseted when the screen mode is changed. (0 by default)
#define R25_SP2						(0b00000001)
#define R25_MAK						(0b00000010)
#define R25_WTE						(0b00000100)
#define R25_YJK						(0b00001000)
#define R25_YAE						(0b00010000)
#define R25_VDS						(0b00100000)
#define R25_CMD						(0b01000000)

//-----------------------------------------------------------------------------
// R#26
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	0	0	HO8	HO7	HO6	HO5	HO4	HO3
//			└───┴───┴───┴───┴───┴──	cf. R#27

//-----------------------------------------------------------------------------
// R#27
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	WRITE ONLY
//	0	0	0	0	0	HO2	HO1	HO0
//						└───┴───┴──	HO0-HO8 bits specify the vertical line number of the screen to place the left most (HO8 bit is only useful if SP2 of register 25 is 1.
//									In screen 6 and 7, the scrolling will be every two lines.) (These two registers are initially reseted.)

#endif // (MSX_VERSION >= MSX_2P)

//-----------------------------------------------------------------------------
// R#32								Source X low register 

//-----------------------------------------------------------------------------
// R#33								Source X high register 

//-----------------------------------------------------------------------------
// R#34								Source Y low register 

//-----------------------------------------------------------------------------
// R#35								Source Y high register 

//-----------------------------------------------------------------------------
// R#36								Destination X low register 

//-----------------------------------------------------------------------------
// R#37								Destination X high register 

//-----------------------------------------------------------------------------
// R#38								Destination Y low register 

//-----------------------------------------------------------------------------
// R#39								Destination Y high register

//-----------------------------------------------------------------------------
// R#40								Number of dots X low register 

//-----------------------------------------------------------------------------
// R#41								Number of dots X high register 

//-----------------------------------------------------------------------------
// R#42								Number of dots Y low register 

//-----------------------------------------------------------------------------
// R#43								Number of dots Y high register  

//-----------------------------------------------------------------------------
// R#44								Color register  

//-----------------------------------------------------------------------------
// R#45								Argument register 
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	0	MXC	MXD	MXS	DIY	DIX	EQ	MAJ
//  	│	│	│	│	│	│	└── MAJ must specify the longest side. 0 for horizontal, 1 for vertical. (Used only by the LINE command.) 
//  	│	│	│	│	│	└────── EQ = 0 to stop the search when the color is found, 1 to stop the search when the color is different from that set. 
//  	│	│	│	│	└────────── TEN = When this bit is at 1, the horizontal displacement value (register 40 and 41) comes negative. The displacement goes to the reverse direction.
//  	│	│	│	└────────────── DIY = When this bit is at 1, the vertical displacement value (register 42 and 43) is considered negative. The movement is in the other direction.
//  	│	│	└────────────────── MXS = Source video memory. 0 for main VRAM; 1 for extended VRAM (for MSX with 192 KB of VRAM).
//  	│	└────────────────────── MXD = Destination video memory. 0 for main VRAM; 1 for extended VRAM (for MSX with 192 KB of VRAM). This functionality has not been integrated into the MSX standard.
//  	└────────────────────────── MXC = RAM / VRAM switching. 1 for RAM extension; 0 for VRAM. Unused on MSX, reset this bit always. 

//-----------------------------------------------------------------------------
// R#46								Command register  
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	CM3	CM2	CM1	CM0	LO3	LO2	LO1	LO0
//	│	│	│	│	└───┴───┴───┴── Logical operator 
//	└───┴───┴───┴────────────────── VDP command to execute


//-----------------------------------------------------------------------------
//
//   V D P   C O M M A N D
//
//-----------------------------------------------------------------------------

#define VDP_CMD_HMMC	0xF0 // 1111
#define VDP_CMD_YMMM	0xE0 // 1110
#define VDP_CMD_HMMM	0xD0 // 1101
#define VDP_CMD_HMMV	0xC0 // 1100
#define VDP_CMD_LMMC	0xB0 // 1011
#define VDP_CMD_LMCM	0xA0 // 1010
#define VDP_CMD_LMMM	0x90 // 1001
#define VDP_CMD_LMMV	0x80 // 1000
#define VDP_CMD_LINE	0x70 // 0111
#define VDP_CMD_SRCH	0x60 // 0110
#define VDP_CMD_PSET	0x50 // 0101
#define VDP_CMD_POINT	0x40 // 0100
#define VDP_CMD_STOP	0x00 // 0000

#define VDP_OP_IMP		0x00 // 0000
#define VDP_OP_AND		0x01 // 0001
#define VDP_OP_OR		0x02 // 0010
#define VDP_OP_XOR		0x03 // 0011
#define VDP_OP_NOT		0x04 // 0100
#define VDP_OP_TIMP		0x08 // 1000
#define VDP_OP_TAND		0x09 // 1001
#define VDP_OP_TOR		0x0A // 1010
#define VDP_OP_TXOR		0x0B // 1011
#define VDP_OP_TNOT		0x0C // 1100

#define VDP_ARG_DIY_DOWN	0
#define VDP_ARG_DIY_UP		8
#define VDP_ARG_DIX_RIGHT	0
#define VDP_ARG_DIX_LEFT	4
#define VDP_ARG_MAJ_H		0 // horizontal
#define VDP_ARG_MAJ_V		1 // vertical

//-----------------------------------------------------------------------------
//
//   V D P   S T A T U S   R E G I S T E R S
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// S#0								Status register 0
//	7	6	5	4	3	2	1	0	READ ONLY
//	F	5S	C	SN4	SN3	SN2	SN1	SN0
//	│	│	│	└───┴───┴───┴───┴──	The number of 5th (or 9th in G3...G7 modes) sprite 
//  │	│	└────────────────────── Collision flag. Two sprites have collided 
//  │	└────────────────────────── Flag for 5th sprite. Five (or nine in G3...G7 modes) sprites are aligned on the same horizontal line 
// 	└────────────────────────────── Vertical scan interrupt flag. When S#0 is read, this flag is reset 
#define S00_F						0x80
#define S00_5S						0x40
#define S00_C						0x20
#define S00_GET_SN(_a)				(_a & 0x1F)

//-----------------------------------------------------------------------------
// S#1								Status register 1
//	7	6	5	4	3	2	1	0	READ ONLY
//	FL	LPS	ID4	ID3	ID2	ID1	ID0	FH
//	│	│	│	│	│	│	│	└── Horizontal scan interrupt flag. Is set if VDP scans line specified in register R#19. If IE1 is set, interrupt is generated. FH is reset when S#1 is read 
//	│	│	└───┴───┴───┴───┴──────	The identification number of the VDP chip 
//  │	└────────────────────────── Light pen button. Is set when light pen button is pressed  
//	│								Mouse 1. Is set if first button of mouse was pressed This flag is not reset when reading status register S#1 in both set-ups 
// 	└────────────────────────────── Light pen. Is set if light pen detects light. If IE2 is set, interrupt is generated. Reset when S#1 is read.
//									Mouse 2. Is set if second button of mouse was pressed. This flag is not reset when reading status register S#1 
#define S01_FH						0x01
#define S01_GET_ID(_a)				((_a >> 1) & 0x1F)
#define S01_IPS						0x40
#define S01_FL						0x80

//-----------------------------------------------------------------------------
// S#2								Status register 2
//	7	6	5	4	3	2	1	0	READ ONLY
//	TR	VR	HR	BD	1	1	EO	CE 
//	│	│	│	│	 	 	│	└── Command execution flag. If set to 1, indicates that VDP is busy executing a command 
//	│	│	│	│			└──────	Display field flag. If set to 0, indicates the first field. If set to 1, indicated the second field 
//  │	│	│	└────────────────── Color detect flag. When the search command is executed, this flag is set if specified color was detected 
//  │	│	└────────────────────── Horizontal retrace flag. Is set during scanning of HBLANK area of the screen, i.e. when right and left borders of the screen are drawn 
//  │	└────────────────────────── Vertical retrace flag. Is set during scanning of VBLANK area of the screen, i.e. during vertical retrace plus while lower and upper borders of the screen is drawn 
//  └────────────────────────────── Transfer ready flag. If set to 1, indicates to the CPU that VDP is ready for next transfer. Value of 0 means that VDP is not ready 
#define S02_CE						0x01
#define S02_EO                      0x02
#define S02_BD                      0x10
#define S02_HR                      0x20
#define S02_VR                      0x40
#define S02_TR                      0x80

//-----------------------------------------------------------------------------
// S#3								Column register low 
//	7	6	5	4	3	2	1	0	READ ONLY
//	X7	X6	X5	X4	X3	X2	X1	X0
//	└───┴───┴───┴───┴───┴───┴───┴── Coordinate information about collision location of the sprites, or location of light pen, or relative movement of the mouse.

//-----------------------------------------------------------------------------
// S#4								Column register high 
//	7	6	5	4	3	2	1	0	READ ONLY
//	1	1	1	1	1	1	1	X8
//								└── Coordinate information about collision location of the sprites, or location of light pen, or relative movement of the mouse.

//-----------------------------------------------------------------------------
// S#5								Row register low 
//	7	6	5	4	3	2	1	0	READ ONLY
//	Y7	Y6	Y5	Y4	Y3	Y2	Y1	Y0
//	└───┴───┴───┴───┴───┴───┴───┴── Coordinate information about collision location of the sprites, or location of light pen, or relative movement of the mouse.

//-----------------------------------------------------------------------------
// S#6								Row register high  
//	7	6	5	4	3	2	1	0	READ ONLY
//	1	1	1	1	1	1	1	Y8
//								└── Coordinate information about collision location of the sprites, or location of light pen, or relative movement of the mouse.

//-----------------------------------------------------------------------------
// S#7								Color      registe 
//	7	6	5	4	3	2	1	0	READ ONLY
//	C7	C6	C5	C4	C3	C2	C1	C0
//	└───┴───┴───┴───┴───┴───┴───┴── This  color  register  is  used  when  executing  commands  “POINT”  and  “VRAM  to  CPU”  and contains VRAM data. 

//-----------------------------------------------------------------------------
// S#8								Coded color X register low 
//	7	6	5	4	3	2	1	0	READ ONLY
//	BX7	BX6	BX5	BX4	BX3	BX2	BX1	BX0
//	└───┴───┴───┴───┴───┴───┴───┴── When the search command is executed and coded color had been detected (see R#2), this register contains its X coordinate. 

//-----------------------------------------------------------------------------
// S#9								Coded color X register high 
//	7	6	5	4	3	2	1	0	READ ONLY
//	1	1	1	1	1	1	1	BX8
//								└── When the search command is executed and coded color had been detected (see R#2), this register contains its X coordinate. 
