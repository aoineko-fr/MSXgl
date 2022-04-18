// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄▄▄▄  ▄▄   ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀  ██  ██ █ ▀█▄  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▄██▄ ▀█▄▀ ▄▄█▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// BIOS Main-ROM routines handler 
//
// References:
// - MSX2 Technical Handbook
// - http://map.grauw.nl/resources/msxbios.php
// - https://www.msx.org/wiki/Main-ROM_BIOS
// - Pratique du MSX 2
//─────────────────────────────────────────────────────────────────────────────
#pragma once
#include "bios.h"
#include "input.h"
#include "bios_var.h"

//-----------------------------------------------------------------------------
//  █▀▀ █▀█ █▄ █ █▀ ▀█▀ ▄▀█ █▄ █ ▀█▀ █▀
//  █▄▄ █▄█ █ ▀█ ▄█  █  █▀█ █ ▀█  █  ▄█
//-----------------------------------------------------------------------------

#define R_CGTABL	0x0004 // Character Generator TABLe - Base address of the MSX character set in ROM
const u16 __at(R_CGTABL) g_CGTABL;

#define R_VDP_DR	0x0006 // Base port address for VDP data read
const u8 __at(R_VDP_DR) g_VDP_DR;

#define R_VDP_DW	0x0007 // Base port address for VDP data write
const u8 __at(R_VDP_DW) g_VDP_DW;

#define R_BASRVN	0x002B // Basic ROM version
#define R_BASRVN1	0x002B // Basic ROM version [0]
// 7 6 5 4 3 2 1 0
// │ │ │ │ └─┴─┴─┴── Character set
// │ │ │ │           0 = Japanese, 1 = International, 2 = Korean / Russian
// │ └─┴─┴────────── Date format
// │                 0 = Y-M-D, 1 = M-D-Y, 2 = D-M-Y
// └──────────────── Default interrupt frequency
//                   0 = 60Hz, 1 = 50Hz
#define R_BASRVN2	0x002C // Basic ROM version [1]
// 7 6 5 4 3 2 1 0
// │ │ │ │ └─┴─┴─┴── Keyboard type
// │ │ │ │           0 = Japanese, 1 = International
// │ │ │ │           2 = French (AZERTY), 3 = UK, 4 = German (DIN)
// └─┴─┴─┴────────── Basic version
//                   0000 = Japanese, 1111 = International
const u8 __at(R_BASRVN) g_BASRVN[2];
struct BasicROMVersion
{
	u8 CharacterSet: 4; // Character Set
	// 0000 (0) = Japan;
	// 0001 (1) = International;
	// 0010 (2) = Russia.
	u8 DateFormat: 3; // Date Format
	// 000 (0) = Year/Month/Day (Japanese);
	// 001 (1) = Month/Day/Year (U.S.A.);
	// 010 (2) = Day/Month/Year (Latin countries).
	u8 VSF: 1; // Vertical Synchronism and interruption Frequency
	// 0 = 60 Hz (NTSC [Japan, U.S.A., U.S.S.R.], PAL-M [Brazil]);
	// 1 = 50 Hz (PAL-x [most European countries], SECAM [France]).
	
	u8 KeyboardType: 4; // Character Set
	// 0000 (0) = Japan;
	// 0001 (1) = International;
	// 0010 (2) = France;
	// 0011 (3) = United Kingdom;
	// 0100 (4) = Germany;
	// 0101 (5) = USSR;
	// 0110 (6) = Spain.
	u8 SSM: 1; // Start-up Screen Mode
	// 0 = SCREEN 1 (Japan);
	// 1 = SCREEN 0 (International).
	u8 SLS: 1; // String Length Specificator Character of PRINT USING
	// 0 = ampersand ("&") (Japan);
	// 1 = backslash ("\") (International).
	u8 SRC: 1; // String Replacement Character of PRINT USING
	// 0 = "at" ("@") (Japan);
	// 1 = ampersand ("&") (International).
	u8 Cur: 1; // Currency Symbol
	// 0 = yen (Japan), pound (U.K.);
	// 1 = dollar (U.S.A.).
};
const struct BasicROMVersion __at(R_BASRVN) g_ROMVersion;

#define R_MSXVER	0x002D // MSX version number
// 0 = MSX 1
// 1 = MSX 2
// 2 = MSX 2+
// 3 = MSX turbo R
const u8 __at(R_MSXVER) g_MSXVER;

#define R_MSXMID	0x002E // Bit 0: if 1 then MSX-MIDI is present internally (MSX turbo R only)
const u8 __at(R_MSXMID) g_MSXMID;

#define R_CHAR_16	0x0034 // Default KANJI range. Copied to KANJTABLE by the DiskBIOS on boot
const u16 __at(R_CHAR_16) g_CHAR_16[2];


//-----------------------------------------------------------------------------
//  █▀█ █▀█ █ █ ▀█▀ █ █▄ █ █▀▀ █▀
//  █▀▄ █▄█ █▄█  █  █ █ ▀█ ██▄ ▄█
//-----------------------------------------------------------------------------

//=============================================================================
// RST-and other routines
//=============================================================================

// Group: Core

//-----------------------------------------------------------------------------
// CHKRAM (STARTUP, RESET, BOOT)	#0000	Tests RAM and sets RAM slot for the system
#define R_CHKRAM	0x0000 // Tests RAM and sets RAM slot for the system
#define R_STARTUP	0x0000
#define R_RESET		0x0000
#define R_BOOT		0x0000

// Function: Bios_Startup
// Tests RAM and sets RAM slot for the system
inline void Bios_Startup();

//-----------------------------------------------------------------------------
// SYNCHR	#0008	Tests whether the character of (HL) is the specified character
#define R_SYNCHR	0x0008 // tests whether the character of [HL] is the specified character

//-----------------------------------------------------------------------------
// RDSLT	#000C	Reads the value of an address in another slot
#define R_RDSLT		0x000C // Reads the value of an address in another slot

// Function: Bios_InterSlotRead
// Reads the value of an address in another slot
u8 Bios_InterSlotRead(u8 slot, u16 addr);

// Function: Bios_MainROMRead
// Reads the value of an address in another slot
u8 Bios_MainROMRead(u16 addr) __FASTCALL;

//-----------------------------------------------------------------------------
// CHRGTR	#0010	Gets the next character (or token) of the Basic text
#define R_CHRGTR	0x0010 // Gets the next character (or token) of the Basic-text

//-----------------------------------------------------------------------------
// WRSLT	#0014	Writes a value to an address in another slot
#define R_WRSLT		0x0014 // Writes a value to an address in another slot

// Function: Bios_InterSlotWrite
// Writes a value to an address in another slot
void Bios_InterSlotWrite(u8 slot, u16 addr, u8 value) __sdcccall(0);

//-----------------------------------------------------------------------------
// OUTDO	#0018	Output to current output channel (printer, file, etc.)
#define R_OUTDO		0x0018 // Output to current outputchannel (printer, diskfile, etc.)

//-----------------------------------------------------------------------------
// CALSLT	#001C	Executes inter-slot call
#define R_CALSLT	0x001C // Executes inter-slot call

// Function: Bios_InterSlotCall
// Executes inter-slot call
void Bios_InterSlotCall(u8 slot, u16 addr) __sdcccall(0);

// Function: Bios_MainROMCall
// Executes inter-slot call
void Bios_MainROMCall(u16 addr) __FASTCALL;

//-----------------------------------------------------------------------------
// DCOMPR	#0020	Compares HL with DE
#define R_DCOMPR	0x0020 // Compares HL with DE

//-----------------------------------------------------------------------------
// ENASLT	#0024	Switches indicated slot at indicated page on perpetually
#define R_ENASLT	0x0024 // Switches indicated slot at indicated page on perpetual

// Function: Bios_SwitchSlot
// Switches indicated slot at indicated page on perpetual
void Bios_SwitchSlot(u8 page, u8 slot);

//-----------------------------------------------------------------------------
// GETYPR	#0028	Returns Type of DAC
#define R_GETYPR	0x0028 // Returns Type of DAC

//-----------------------------------------------------------------------------
// CALLF	#0030	Executes an interslot call
#define R_CALLF		0x0030 // Executes an interslot call
#define Bios_InterSlotCallF(_slot, _addr) \
	__asm                                 \
		rst		R_CALLF                   \
		db		_slot                     \
		dw		_addr                     \
	__endasm

//-----------------------------------------------------------------------------
// KEYINT	#0038	Executes the timer interrupt process routine
#define R_KEYINT	0x0038 // Executes the timer interrupt process routine

//-----------------------------------------------------------------------------
// NMI		#0066	Executes non-maskable interupt handling routine
#define R_NMI		0x0066 // Executes (non-maskable interupt) handling routine

//=============================================================================
// Initialization-routines
//=============================================================================

// Group: Initialization

//-----------------------------------------------------------------------------
// INITIO	#003B	Initialises the device
#define R_INITIO	0x003B // Initialises the device

//-----------------------------------------------------------------------------
// INIFNK	#003E	Initialises the contents of the function keys
#define R_INIFNK	0x003E // Initialises the contents of the function keys

//=============================================================================
// VDP routines
//=============================================================================

// Group: VDP
#if BIOS_USE_VDP

//-----------------------------------------------------------------------------
// DISSCR	#0041	Inhibits the screen display
#define R_DISSCR	0x0041 // Inhibits the screen display

// Function: Bios_DisableScreen
// Inhibits the screen display
inline void Bios_DisableScreen();

//-----------------------------------------------------------------------------
// ENASCR	#0044	Displays the screen
#define R_ENASCR	0x0044 // Displays the screen

// Function: Bios_EnableScreen
// Displays the screen
inline void Bios_EnableScreen();

//-----------------------------------------------------------------------------
// WRTVDP	#0047	Write data in the VDP-register
#define R_WRTVDP	0x0047 // Write data in the VDP-register

// Function: Bios_WriteVDP
// Write data in the VDP-register
void Bios_WriteVDP(u8 reg, u8 value) __sdcccall(0);

//-----------------------------------------------------------------------------
// RDVRM	#004A	Reads the content of VRAM
#define R_RDVRM		0x004A // Reads the content of VRAM

// Function: Bios_ReadVRAM
// Reads the content of VRAM
u8 Bios_ReadVRAM(u16 addr) __FASTCALL;

//-----------------------------------------------------------------------------
// WRTVRM	#004D	Writes data in VRAM
#define R_WRTVRM	0x004D // Writes data in VRAM

// Function: Bios_WriteVRAM
// Writes data in VRAM
void Bios_WriteVRAM(u16 addr, u8 value) __sdcccall(0);

//-----------------------------------------------------------------------------
// SETRD	#0050	Sets VRAM address to read a byte or more
#define R_SETRD		0x0050 // Enable VDP to read

// Function: Bios_SetAddressForRead
// Enable VDP to read
void Bios_SetAddressForRead(u16 addr) __FASTCALL;

//-----------------------------------------------------------------------------
// SETWRT	#0053	Sets VRAM address to write or more
#define R_SETWRT	0x0053 // Enable VDP to write

// Function: Bios_SetAddressForWrite
// Enable VDP to write
void Bios_SetAddressForWrite(u16 addr) __FASTCALL;

//-----------------------------------------------------------------------------
// FILVRM	#0056	Fills the specified VRAM area with the same data
#define R_FILVRM	0x0056 // Fill VRAM with value

// Function: Bios_FillVRAM
// Fill VRAM with value
void Bios_FillVRAM(u16 addr, u16 length, u8 value) __sdcccall(0);

//-----------------------------------------------------------------------------
// LDIRMV	#0059	Block transfer from VRAM to memory
#define R_LDIRMV	0x0059 // Block transfer to memory from VRAM

// Function: Bios_TransfertVRAMtoRAM
// Block transfer to memory from VRAM
void Bios_TransfertVRAMtoRAM(u16 vram, u16 ram, u16 length) __sdcccall(0);

//-----------------------------------------------------------------------------
// LDIRVM	#005C	Block transfer from memory to VRAM
#define R_LDIRVM	0x005C // Block transfer to VRAM from memory

// Function: Bios_TransfertRAMtoVRAM
// Block transfer to VRAM from memory
void Bios_TransfertRAMtoVRAM(u16 ram, u16 vram, u16 length) __sdcccall(0);

//-----------------------------------------------------------------------------
// CHGMOD	#005F	Switches to given screen mode
#define R_CHGMOD	0x005F // Switches to given screenmode

// Function: Bios_ChangeMode
// Switches to given screenmode
void Bios_ChangeMode(u8 screen) __FASTCALL;

//-----------------------------------------------------------------------------
// CHGCLR	#0062	Changes the screen colors
#define R_CHGCLR	0x0062 // Changes the screencolors

// Function: Bios_ChangeColor
// Changes the screencolors
void Bios_ChangeColor(u8 text, u8 back, u8 border) __sdcccall(0);

//-----------------------------------------------------------------------------
// CLRSPR	#0069	Initialises all sprites
#define R_CLRSPR	0x0069 // Initialises all sprites

//-----------------------------------------------------------------------------
// INITXT	#006C	Switches to SCREEN 0 (text screen with 40×24 characters)
#define R_INITXT	0x006C // Switches to SCREEN 0 (text screen with 40*24 characters)

// Function: Bios_InitScreen0
// Switches to SCREEN 0 (text screen with 40*24 characters)
inline void Bios_InitScreen0();

// Function: Bios_InitScreen0Ex
// Switches to SCREEN 0 (text screen with 40*24 characters)
inline void Bios_InitScreen0Ex(u16 pnt, u16 pgt, u8 width, u8 text, u8 bg, u8 border);

#define Bios_InitTextMode   Bios_InitScreen0
#define Bios_InitTextModeEx Bios_InitScreen0Ex

//-----------------------------------------------------------------------------
// INIT32	#006F	Switches to SCREEN 1 (text screen with 32×24 characters)
#define R_INIT32	0x006F // Switches to SCREEN 1 (text screen with 32*24 characters)

// Function: Bios_InitScreen1
// Switches to SCREEN 1 (text screen with 32*24 characters)
inline void Bios_InitScreen1();

// Function: Bios_InitScreen1Ex
// Switches to SCREEN 1 (text screen with 32*24 characters)
inline void Bios_InitScreen1Ex(u16 pnt, u16 ct, u16 pgt, u16 sat, u16 sgt, u8 text, u8 bg, u8 border);

#define Bios_InitText32Mode   Bios_InitScreen1	
#define Bios_InitText32ModeEx Bios_InitScreen1Ex	

//-----------------------------------------------------------------------------
// INIGRP	#0072	Switches to SCREEN 2 (high resolution screen with 256×192 pixels)
#define R_INIGRP	0x0072 // Switches to SCREEN 2 (high resolution screen with 256*192 pixels)

// Function: Bios_InitScreen2
// Switches to SCREEN 2 (high resolution screen with 256*192 pixels)
inline void Bios_InitScreen2();

// Function: Bios_InitScreen2Ex
// Switches to SCREEN 2 (high resolution screen with 256*192 pixels)
inline void Bios_InitScreen2Ex(u16 pnt, u16 ct, u16 pgt, u16 sat, u16 sgt, u8 text, u8 bg, u8 border);

#define Bios_InitGraphicMode   Bios_InitScreen2	
#define Bios_InitGraphicModeEx Bios_InitScreen2Ex	

//-----------------------------------------------------------------------------
// INIMLT	#0075	Switches to SCREEN 3 (multi-color screen with 64×48 pixels)
#define R_INIMLT	0x0075 // Switches to SCREEN 3 (multi-color screen 64*48 pixels)

// Function: Bios_InitScreen3
// Switches to SCREEN 3 (multi-color screen 64*48 pixels)
inline void Bios_InitScreen3();

// Function: Bios_InitScreen3Ex
// Switches to SCREEN 3 (multi-color screen 64*48 pixels)
inline void Bios_InitScreen3Ex(u16 pnt, u16 ct, u16 pgt, u16 sat, u16 sgt, u8 text, u8 bg, u8 border);

#define Bios_InitMulticolorMode   Bios_InitScreen3
#define Bios_InitMulticolorModeEx Bios_InitScreen3Ex

//-----------------------------------------------------------------------------
// SETTXT	#0078	Switches VDP to SCREEN 0 mode
#define R_SETTXT	0x0078 // Switches VDP in SCREEN 0 mode

// Function: Bios_SetScreen0
// Switches VDP in SCREEN 0 mode
inline void Bios_SetScreen0();

#define Bios_SetTextMode Bios_SetScreen0

//-----------------------------------------------------------------------------
// SETT32	#007B	Switches VDP to SCREEN 1 mode
#define R_SETT32	0x007B // Switches VDP in SCREEN 1 mode

// Function: Bios_SetScreen1
// Switches VDP in SCREEN 1 mode
inline void Bios_SetScreen1();

#define Bios_SetText32Mode Bios_SetScreen1	

//-----------------------------------------------------------------------------
// SETGRP	#007E	Switches VDP to SCREEN 2 mode
#define R_SETGRP	0x007E // Switches VDP to SCREEN 2 mode

// Function: Bios_SetScreen2
// Switches VDP to SCREEN 2 mode
inline void Bios_SetScreen2();

#define Bios_SetGraphicMode Bios_SetScreen2	

//-----------------------------------------------------------------------------
// SETMLT	#0081	Switches VDP to SCREEN 3 mode
#define R_SETMLT	0x0081 // Switches VDP to SCREEN 3 mode

// Function: Bios_SetScreen3
// Switches VDP to SCREEN 3 mode
inline void Bios_SetScreen3();

#define Bios_SetMulticolorMode Bios_SetScreen3

//-----------------------------------------------------------------------------
// CALPAT	#0084	Returns the address of the sprite pattern table
#define R_CALPAT	0x0084 // Returns the address of the sprite pattern table

// Function: Bios_GetPatternTableAddress
// Returns the address of the sprite pattern table
u16 Bios_GetPatternTableAddress(u8 id) __FASTCALL;

//-----------------------------------------------------------------------------
// CALATR	#0087	Returns the address of the sprite attribute table
#define R_CALATR	0x0087 // Returns the address of the sprite attribute table

// Function: Bios_GetAttributeTableAddress
// Returns the address of the sprite attribute table
u16 Bios_GetAttributeTableAddress(u8 id) __FASTCALL;

//-----------------------------------------------------------------------------
// GSPSIZ	#008A	Returns current sprite size
#define R_GSPSIZ	0x008A // Returns current sprite size

// Function: Bios_GetSpriteSize
// Returns current sprite size
u8 Bios_GetSpriteSize();

//-----------------------------------------------------------------------------
// GRPPRT	#008D	Displays a character on the graphic screen
#define R_GRPPRT	0x008D // Displays a character on the graphic screen

// Function: Bios_GraphPrintChar
// Displays a character on the graphic screen
void Bios_GraphPrintChar(u8 chr) __FASTCALL;

// Function: Bios_GraphPrintCharEx
// Displays a character on the graphic screen
void Bios_GraphPrintCharEx(u8 chr, u16 x, u8 y, u8 color, u8 op);

#endif // BIOS_USE_VDP

//=============================================================================
// PSG routines
//=============================================================================

// Group: PSG
#if BIOS_USE_PSG

//-----------------------------------------------------------------------------
// GICINI	#0090	Initialises PSG and sets initial value for the PLAY statement
#define R_GICINI	0x0090 // Initialises PSG and sets initial value for the PLAY statement

// Function: Bios_InitPSG
inline void Bios_InitPSG();

//-----------------------------------------------------------------------------
// WRTPSG	#0093	Writes data to PSG register
#define R_WRTPSG	0x0093 // Writes data to PSG-register

// Function: Bios_WritePSG
// Writes data to PSG-register
void Bios_WritePSG(u8 reg, u8 value) __sdcccall(0);

//-----------------------------------------------------------------------------
// RDPSG	#0096	Reads value from PSG register
#define R_RDPSG		0x0096 // Reads value from PSG-register

// Function: Bios_ReadPSG
// Reads value from PSG-register
u8 Bios_ReadPSG(u8 reg) __FASTCALL;

//-----------------------------------------------------------------------------
// STRTMS	#0099	Tests whether the PLAY statement is being executed as a background task. If not, begins to execute the PLAY statement
#define R_STRTMS	0x0099 // Tests whether the PLAY statement is being executed as a background task

// Function: Bios_PlayPSG
// Tests whether the PLAY statement is being executed as a background task
inline void Bios_PlayPSG();

#endif // BIOS_USE_PSG

//=============================================================================
// Console routines
//=============================================================================

// Group: Console

//-----------------------------------------------------------------------------
// CHSNS	#009C	Tests the status of the keyboard buffer
#define R_CHSNS		0x009C // Tests the status of the keyboard buffer

//-----------------------------------------------------------------------------
// CHGET	#009F	Waits for a character input on the keyboard
#define R_CHGET		0x009F // One character input (waiting)

// Function: Bios_GetCharacter
// One character input (waiting)
u8 Bios_GetCharacter();

//-----------------------------------------------------------------------------
// CHPUT	#00A2	Displays one character
#define R_CHPUT		0x00A2 // Displays one character

// Function: Bios_TextPrintChar
// Displays one character
void Bios_TextPrintChar(u8 chr) __FASTCALL;

//-----------------------------------------------------------------------------
// LPTOUT	#00A5	Sends one character to printer
#define R_LPTOUT	0x00A5 // Sends one character to printer

//-----------------------------------------------------------------------------
// LPTSTT	#00A8	Tests printer status
#define R_LPTSTT	0x00A8 // Tests printer status

//-----------------------------------------------------------------------------
// CNVCHR	#00AB	Tests for the graphic header and transforms the code
#define R_CNVCHR	0x00AB // Tests for the graphic header and transforms the code

//-----------------------------------------------------------------------------
// PINLIN	#00AE	Stores in the specified buffer the character codes input until the return key or STOP key is pressed
#define R_PINLIN	0x00AE // Stores in the specified buffer the character codes input until the return

//-----------------------------------------------------------------------------
// INLIN	#00B1	Same as PINLIN except that AUGFLG (#F6AA) is set
#define R_INLIN		0x00B1 // Same as PINLIN except that AUGFLG (#F6AA) is set

//-----------------------------------------------------------------------------
// QINLIN	#00B4	Prints a question mark and one space
#define R_QINLIN	0x00B4 // Prints a questionmark andone space

//-----------------------------------------------------------------------------
// BREAKX	#00B7	Tests status of CTRL-STOP
#define R_BREAKX	0x00B7 // Tests status of CTRL-STOP

//-----------------------------------------------------------------------------
// ISCNTC	#00BA	Tests status of SHIFT-STOP
#define R_ISCNTC	0x00BA // Tests status of SHIFT-STOP

//-----------------------------------------------------------------------------
// CKCNTC	#00BD	Same as ISCNTC. used in Basic
#define R_CKCNTC	0x00BD // Same as ISCNTC. used in Basic

//-----------------------------------------------------------------------------
// BEEP		#00C0	Generates beep
#define R_BEEP		0x00C0 // Generates beep

// Function: 
inline void Bios_Beep();

//-----------------------------------------------------------------------------
// CLS		#00C3	Clears the screen
#define R_CLS		0x00C3 // Clears the screen

// Function: Bios_ClearScreen
// Clears the screen
inline void Bios_ClearScreen();

//-----------------------------------------------------------------------------
// POSIT	#00C6	Moves cursor to the specified position
#define R_POSIT		0x00C6 // Moves cursor to the specified position

// Function: Bios_SetCursorPosition
// Moves cursor to the specified position
void Bios_SetCursorPosition(u8 X, u8 Y);

//-----------------------------------------------------------------------------
// FNKSB	#00C9	Tests whether the function key display is active (FNKFLG)
#define R_FNKSB		0x00C9 // Tests whether the function key display is active (FNKFLG)

//-----------------------------------------------------------------------------
// ERAFNK	#00CC	Erase functionkey display
#define R_ERAFNK	0x00CC // Erase functionkey display

//-----------------------------------------------------------------------------
// DSPFNK	#00CF	Displays the function keys
#define R_DSPFNK	0x00CF // Displays the function keys

//-----------------------------------------------------------------------------
// TOTEXT	#00D2	Forces the screen to be in the text mode
#define R_TOTEXT	0x00D2 // Forces the screen to be in the text mode

//=============================================================================
// Controller routines
//=============================================================================

// Group: Controller

//-----------------------------------------------------------------------------
// GTSTCK	#00D5	Returns the joystick status
#define R_GTSTCK	0x00D5 // Returns the joystick status

// Function: Bios_GetJoystickDirection
// Returns the joystick status
u8 Bios_GetJoystickDirection(u8 port) __FASTCALL;

//-----------------------------------------------------------------------------
// GTTRIG	#00D8	Returns current trigger status
#define R_GTTRIG	0x00D8 // Returns current trigger status

// Function: Bios_GetJoystickTrigger
// Returns current trigger status
u8 Bios_GetJoystickTrigger(u8 trigger) __FASTCALL;

//-----------------------------------------------------------------------------
// GTPAD	#00DB	Returns current touch pad status
#define R_GTPAD		0x00DB // Returns current touch pad status

//-----------------------------------------------------------------------------
// GTPDL	#00DE	Returns current value of paddle
#define R_GTPDL		0x00DE // Returns current value of paddle

//=============================================================================
// Tape device routines
//=============================================================================

// Group: Tape

//-----------------------------------------------------------------------------
// TAPION	#00E1	Reads the header block after turning the cassette motor on
#define R_TAPION	0x00E1 // Reads the header block after turning the cassette motor on

//-----------------------------------------------------------------------------
// TAPIN	#00E4	Read data from the tape
#define R_TAPIN		0x00E4 // Read data from the tape

//-----------------------------------------------------------------------------
// TAPIOF	#00E7	Stops reading from the tape
#define R_TAPIOF	0x00E7 // Stops reading from the tape

//-----------------------------------------------------------------------------
// TAPOON	#00EA	Turns on the cassette motor and writes the header
#define R_TAPOON	0x00EA // Turns on the cassette motor and writes the header

//-----------------------------------------------------------------------------
// TAPOUT	#00ED	Writes data on the tape
#define R_TAPOUT	0x00ED // Writes data on the tape

//-----------------------------------------------------------------------------
// TAPOOF	#00F0	Stops writing on the tape
#define R_TAPOOF	0x00F0 // Stops writing on the tape

//-----------------------------------------------------------------------------
// STMOTR	#00F3	Sets the cassette motor action
#define R_STMOTR	0x00F3 // Sets the cassette motor action

//=============================================================================
// Queue routines
//=============================================================================

// Group: Queue

//-----------------------------------------------------------------------------
// LFTQ		#00F6	Gives number of bytes in queue
#define R_LFTQ		0x00F6 // Gives number of bytes in queue

//-----------------------------------------------------------------------------
// PUTQ		#00F9	Put byte in queue
#define R_PUTQ		0x00F9 // Put byte in queue

//=============================================================================
// Graphic routines
//=============================================================================

// Group: Graphic

//-----------------------------------------------------------------------------
// RIGHTC	#00FC	Shifts screen pixel to the right
#define R_RIGHTC	0x00FC // Shifts screen pixel to the right

//-----------------------------------------------------------------------------
// LEFTC	#00FF	Shifts screen pixel to the left
#define R_LEFTC		0x00FF // Shifts screen pixel to the left

//-----------------------------------------------------------------------------
// UPC		#0102	Shifts screen pixel up
#define R_UPC		0x0102 // Shifts screen pixel up

//-----------------------------------------------------------------------------
// TUPC		#0105	Tests whether UPC is possible, if possible, execute UPC
#define R_TUPC		0x0105 // Tests whether UPC is possible, if possible, execute UPC

//-----------------------------------------------------------------------------
// DOWNC	#0108	Shifts screen pixel down
#define R_DOWNC		0x0108 // Shifts screen pixel down

//-----------------------------------------------------------------------------
// TDOWNC	#010B	Tests whether DOWNC is possible, if possible, execute DOWNC
#define R_TDOWNC	0x010B // Tests whether DOWNC is possible, if possible, execute DOWNC

//-----------------------------------------------------------------------------
// SCALXY	#010E	Scales X and Y coordinates
#define R_SCALXY	0x010E // Scales X and Y coordinates

//-----------------------------------------------------------------------------
// MAPXY	#0111	Places cursor at current cursor address
#define R_MAPXY		0x0111 // Places cursor at current cursor address

//-----------------------------------------------------------------------------
// FETCHC	#0114	Gets current cursor addresses mask pattern
#define R_FETCHC	0x0114 // Gets current cursor addresses mask pattern

//-----------------------------------------------------------------------------
// STOREC	#0117	Record current cursor addresses mask pattern
#define R_STOREC	0x0117 // Record current cursor addresses mask pattern

//-----------------------------------------------------------------------------
// SETATR	#011A	Set attribute byte
#define R_SETATR	0x011A // Set attribute byte

//-----------------------------------------------------------------------------
// READC	#011D	Reads attribute byte of current screen pixel
#define R_READC		0x011D // Reads attribute byte of current screen pixel

//-----------------------------------------------------------------------------
// SETC		#0120	Returns current screen pixel of specified attribute byte
#define R_SETC		0x0120 // Returns current screen pixel of specified attribute byte

//-----------------------------------------------------------------------------
// NSETCX	#0123	Set horizontal screen pixels
#define R_NSETCX	0x0123 // Set horizontal screen pixels

//-----------------------------------------------------------------------------
// GTASPC	#0126	Gets screen relations
#define R_GTASPC	0x0126 // Gets screen relations

//-----------------------------------------------------------------------------
// PNTINI	#0129	Initalises the PAINT instruction
#define R_PNTINI	0x0129 // Initalises the PAINT instruction

//-----------------------------------------------------------------------------
// SCANR	#012C	Scans screen pixels to the right
#define R_SCANR		0x012C // Scans screen pixels to the right

//-----------------------------------------------------------------------------
// SCANL	#012F	Scans screen pixels to the left
#define R_SCANL		0x012F // Scans screen pixels to the left

//=============================================================================
// Misc routines
//=============================================================================

// Group: Misc

//-----------------------------------------------------------------------------
// CHGCAP	#0132	Alternates the CAPS lamp status
#define R_CHGCAP	0x0132 // Alternates the CAPS lamp status

//-----------------------------------------------------------------------------
// CHGSND	#0135	Alternates the 1-bit sound port status
#define R_CHGSND	0x0135 // Alternates the 1-bit sound port status

//-----------------------------------------------------------------------------
// RSLREG	#0138	Reads the primary slot register
#define R_RSLREG	0x0138 // Reads the primary slot register

//-----------------------------------------------------------------------------
// WSLREG	#013B	Writes value to the primary slot register
#define R_WSLREG	0x013B // Writes value to the primary slot register

//-----------------------------------------------------------------------------
// RDVDP	#013E	Reads VDP status register
#define R_RDVDP		0x013E // Reads VDP status register

//-----------------------------------------------------------------------------
// SNSMAT	#0141	Returns the value of the specified line from the keyboard matrix
#define R_SNSMAT	0x0141 // Returns the value of the specified line from the keyboard matrix

// Function: Bios_GetKeyboardMatrix
// Returns the value of the specified line from the keyboard matrix
u8 Bios_GetKeyboardMatrix(u8 line) __FASTCALL;

// Function: Bios_IsKeyPressed
// Check if the given key is pressed
inline bool Bios_IsKeyPressed(u8 key)
{
	return (g_NEWKEY[KEY_ROW(key)] & (1 << KEY_IDX(key))) == 0;
}

// Check if the given key is just pushed // don't work with standard system BIOS because g_OLDKEY is reset during ISR
/*inline bool Bios_IsKeyPushed(u8 key)
{
	u8 newKey = Bios_IsKeyPressed(key);
	u8 oldKey = (g_OLDKEY[KEY_ROW(key)] & (1 << KEY_IDX(key))) == 0;
	return newKey && !oldKey;
}*/

//-----------------------------------------------------------------------------
// PHYDIO	#0144	Executes I/O for mass-storage media like disks
#define R_PHYDIO	0x0144 // Executes I/O for mass-storage media like disks

//-----------------------------------------------------------------------------
// FORMAT	#0147	Initialises mass-storage media like formatting of disks
#define R_FORMAT	0x0147 // Initialises mass-storage media like formatting of disks

//-----------------------------------------------------------------------------
// ISFLIO	#014A	Tests if I/O to device is taking place
#define R_ISFLIO	0x014A // Tests if I/O to device is taking place

//-----------------------------------------------------------------------------
// OUTDLP	#014D	Printer output
#define R_OUTDLP	0x014D // Printer output

//-----------------------------------------------------------------------------
// GETVCP	#0150	Returns pointer to play queue
#define R_GETVCP	0x0150 // Returns pointer to play queue

//-----------------------------------------------------------------------------
// GETVC2	#0153	Returns pointer to variable in queue number VOICEN (byte at #FB38)
#define R_GETVC2	0x0153 // Returns pointer to variable in queue number VOICEN (byte at #FB38)

//-----------------------------------------------------------------------------
// KILBUF	#0156	Clear keyboard buffer
#define R_KILBUF	0x0156 // Clear keyboard buffer

//-----------------------------------------------------------------------------
// CALBAS	#0159	Executes inter-slot call to the routine in BASIC interpreter
#define R_CALBAS	0x0159 // Executes inter-slot call to the routine in BASIC interpreter

//=============================================================================
// MSX 2
//=============================================================================

// Group: MSX2
#if (MSX_VERSION >= MSX_2)

//-----------------------------------------------------------------------------
// SUBROM	#015C	Calls a routine in SUB-ROM
#define R_SUBROM	0x015C // Calls a routine in SUB-ROM

//-----------------------------------------------------------------------------
// EXTROM	#015F	Calls a routine in SUB-ROM. Most common way
#define R_EXTROM	0x015F // Calls a routine in SUB-ROM. Most common way

//-----------------------------------------------------------------------------
// CHKSLZ	#0162	Search slots for SUB-ROM
#define R_CHKSLZ	0x0162 // Search slots for SUB-ROM

//-----------------------------------------------------------------------------
// CHKNEW	#0165	Tests screen mode
#define R_CHKNEW	0x0165 // Tests screen mode

//-----------------------------------------------------------------------------
// EOL		#0168	Deletes to the end of the line
#define R_EOL		0x0168 // Deletes to the end of the line

//-----------------------------------------------------------------------------
// BIGFIL	#016B	Same function as FILVRM (with 16-bit VRAM-address)
#define R_BIGFIL	0x016B // Same function as FILVRM (with 16-bit VRAM-address)

//-----------------------------------------------------------------------------
// NSETRD	#016E	Same function as SETRD (with 16-bit VRAM-address)
#define R_NSETRD	0x016E // Same function as SETRD (with 16-bit VRAM-address)

//-----------------------------------------------------------------------------
// NSTWRT	#0171	Same function as SETWRT (with 16-bit VRAM-address)
#define R_NSTWRT	0x0171 // Same function as SETWRT (with 16-bit VRAM-address)

//-----------------------------------------------------------------------------
// NRDVRM	#0174	Reads VRAM like in RDVRM (with 16-bit VRAM-address)
#define R_NRDVRM	0x0174 // Reads VRAM like in RDVRM (with 16-bit VRAM-address)

//-----------------------------------------------------------------------------
// NWRVRM	#0177	Writes to VRAM like in WRTVRM (with 16-bit VRAM-address)
#define R_NWRVRM	0x0177 // Writes to VRAM like in WRTVRM (with 16-bit VRAM-address)

#endif // (MSX_VERSION >= MSX_2)

//=============================================================================
// MSX 2+
//=============================================================================

// Group: MSX2+
#if (MSX_VERSION >= MSX_2P)

//-----------------------------------------------------------------------------
// RDRES	#017A	Read value of I/O port #F4
#define R_RDRES		0x017A // Read value of I/O port #F4

//-----------------------------------------------------------------------------
// WRRES	#017D	Write value to I/O port #F4
#define R_WRRES		0x017D // Write value to I/O port #F4

#endif // (MSX_VERSION >= MSX_2P)

//=============================================================================
// MSX turbo R
//=============================================================================

// Group: MSX turbo R
#if (MSX_VERSION == MSX_TR)

#define CPU_MODE_Z80		0x00
#define CPU_MODE_R800_ROM	0x01
#define CPU_MODE_R800_DRAM	0x02
#define CPU_TURBO_LED		0x80

//-----------------------------------------------------------------------------
// CHGCPU	#0180	Changes CPU mode
#define R_CHGCPU	0x0180 // Changes CPU mode

// Function: Bios_SetCPUMode
// Changes CPU mode
void Bios_SetCPUMode(u8 mode) __FASTCALL;

//-----------------------------------------------------------------------------
// GETCPU	#0183	Returns current CPU mode
#define R_GETCPU	0x0183 // Returns current CPU mode

// Function: Bios_GetCPUMode
// Returns current CPU mode
u8 Bios_GetCPUMode();

//-----------------------------------------------------------------------------
// PCMPLY	#0186	Plays specified memory area through the PCM chip
#define R_PCMPLY	0x0186 // Plays specified memory area through the PCM chip

//-----------------------------------------------------------------------------
// PCMREC	#0189	Records audio using the PCM chip into the specified memory area
#define R_PCMREC	0x0189 // Records audio using the PCM chip into the specified memory area

#endif // (MSX_VERSION == MSX_TR)

