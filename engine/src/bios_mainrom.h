// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄▄▄▄  ▄▄   ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀  ██  ██ █ ▀█▄  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▄██▄ ▀█▄▀ ▄▄█▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// BIOS Main-ROM routines handler 
//
// References:
// - MSX2 Technical Handbook
// - http://map.grauw.nl/resources/msxbios.php
// - https://www.msx.org/wiki/Main-ROM_BIOS
// - Pratique du MSX2
//─────────────────────────────────────────────────────────────────────────────
#pragma once

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
#define MSXVER_1	0 // MSX1
#define MSXVER_2	1 // MSX2
#define MSXVER_2P	2 // MSX2+
#define MSXVER_TR	3 // MSX turbo R
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

#define R_CHKRAM	0x0000 // Tests RAM and sets RAM slot for the system
#define R_STARTUP	0x0000
#define R_RESET		0x0000
#define R_BOOT		0x0000
#define R_SYNCHR	0x0008 // tests whether the character of [HL] is the specified character
#define R_RDSLT		0x000C // Reads the value of an address in another slot
#define R_CHRGTR	0x0010 // Gets the next character (or token) of the Basic-text
#define R_WRSLT		0x0014 // Writes a value to an address in another slot
#define R_OUTDO		0x0018 // Outputs to current outputchannel (printer, diskfile, etc.)
#define R_CALSLT	0x001C // Executes inter-slot call
#define R_DCOMPR	0x0020 // Compares HL with DE
#define R_ENASLT	0x0024 // Switches to specified slot and page definitively
#define R_GETYPR	0x0028 // Returns Type of DAC
#define R_CALLF		0x0030 // Executes an interslot call
#define R_KEYINT	0x0038 // Executes the timer interrupt process routine
#define R_NMI		0x0066 // Executes (non-maskable interupt) handling routine

//=============================================================================
// Initialization-routines
//=============================================================================

#define R_INITIO	0x003B // Initialises the device
#define R_INIFNK	0x003E // Initialises the contents of the function keys

//=============================================================================
// VDP routines
//=============================================================================

#define R_DISSCR	0x0041 // Inhibits the screen display
#define R_ENASCR	0x0044 // Displays the screen
#define R_WRTVDP	0x0047 // Writes data in the VDP-register
#define R_RDVRM		0x004A // Reads the content of VRAM
#define R_WRTVRM	0x004D // Writes data in VRAM
#define R_SETRD		0x0050 // Enables VDP to read
#define R_SETWRT	0x0053 // Enables VDP to write
#define R_FILVRM	0x0056 // Fills VRAM with value
#define R_LDIRMV	0x0059 // Block transfers to memory from VRAM
#define R_LDIRVM	0x005C // Block transfers to VRAM from memory
#define R_CHGMOD	0x005F // Switches to given screen mode
#define R_CHGCLR	0x0062 // Changes the screen colors
#define R_CLRSPR	0x0069 // Initialises all sprites
#define R_INITXT	0x006C // Switches to SCREEN 0 (text screen with 40*24 characters)
#define R_INIT32	0x006F // Switches to SCREEN 1 (text screen with 32*24 characters)
#define R_INIGRP	0x0072 // Switches to SCREEN 2 (high resolution screen with 256*192 pixels)
#define R_INIMLT	0x0075 // Switches to SCREEN 3 (multi-color screen 64*48 pixels)
#define R_SETTXT	0x0078 // Switches VDP in SCREEN 0 mode
#define R_SETT32	0x007B // Switches VDP in SCREEN 1 mode
#define R_SETGRP	0x007E // Switches VDP to SCREEN 2 mode
#define R_SETMLT	0x0081 // Switches VDP to SCREEN 3 mode
#define R_CALPAT	0x0084 // Returns the address of the sprite pattern table
#define R_CALATR	0x0087 // Returns the address of the sprite attribute table
#define R_GSPSIZ	0x008A // Returns current sprite size
#define R_GRPPRT	0x008D // Displays a character on the graphic screen

//=============================================================================
// PSG routines
//=============================================================================

#define R_GICINI	0x0090 // Initialises PSG and sets initial value for the PLAY statement
#define R_WRTPSG	0x0093 // Writes data to PSG-register
#define R_RDPSG		0x0096 // Reads value from PSG-register
#define R_STRTMS	0x0099 // Tests whether the PLAY statement is being executed as a background task

//=============================================================================
// Console routines
//=============================================================================

#define R_CHSNS		0x009C // Tests the status of the keyboard buffer
#define R_CHGET		0x009F // One character input (waiting)
#define R_CHPUT		0x00A2 // Displays one character
#define R_LPTOUT	0x00A5 // Sends one character to printer
#define R_LPTSTT	0x00A8 // Tests printer status
#define R_CNVCHR	0x00AB // Tests for the graphic header and transforms the code
#define R_PINLIN	0x00AE // Stores in the specified buffer the character codes input until the return
#define R_INLIN		0x00B1 // Same as PINLIN except that AUGFLG (#F6AA) is set
#define R_QINLIN	0x00B4 // Prints a questionmark andone space
#define R_BREAKX	0x00B7 // Tests status of CTRL-STOP
#define R_ISCNTC	0x00BA // Tests status of SHIFT-STOP
#define R_CKCNTC	0x00BD // Same as ISCNTC. used in Basic
#define R_BEEP		0x00C0 // Generates beep
#define R_CLS		0x00C3 // Clears the screen
#define R_POSIT		0x00C6 // Moves cursor to the specified position
#define R_FNKSB		0x00C9 // Tests whether the function key display is active (FNKFLG)
#define R_ERAFNK	0x00CC // Erases function key display
#define R_DSPFNK	0x00CF // Displays the function keys
#define R_TOTEXT	0x00D2 // Forces the screen to be in the text mode

//=============================================================================
// Controller routines
//=============================================================================

#define R_GTSTCK	0x00D5 // Returns the joystick status
#define R_GTTRIG	0x00D8 // Returns current trigger status
#define R_GTPAD		0x00DB // Returns current touch pad status
#define R_GTPDL		0x00DE // Returns current value of paddle

//=============================================================================
// Tape device routines
//=============================================================================

#define R_TAPION	0x00E1 // Reads the header block after turning the cassette motor on
#define R_TAPIN		0x00E4 // Reads data from the tape
#define R_TAPIOF	0x00E7 // Stops reading from the tape
#define R_TAPOON	0x00EA // Turns on the cassette motor and writes the header
#define R_TAPOUT	0x00ED // Writes data on the tape
#define R_TAPOOF	0x00F0 // Stops writing on the tape
#define R_STMOTR	0x00F3 // Sets the cassette motor action

//=============================================================================
// Queue routines
//=============================================================================

#define R_LFTQ		0x00F6 // Gives number of bytes in queue
#define R_PUTQ		0x00F9 // Puts byte in queue

//=============================================================================
// Graphic routines
//=============================================================================

#define R_RIGHTC	0x00FC // Shifts screen pixel to the right
#define R_LEFTC		0x00FF // Shifts screen pixel to the left
#define R_UPC		0x0102 // Shifts screen pixel up
#define R_TUPC		0x0105 // Tests whether UPC is possible, if possible, execute UPC
#define R_DOWNC		0x0108 // Shifts screen pixel down
#define R_TDOWNC	0x010B // Tests whether DOWNC is possible, if possible, execute DOWNC
#define R_SCALXY	0x010E // Scales X and Y coordinates
#define R_MAPXY		0x0111 // Places cursor at current cursor address
#define R_FETCHC	0x0114 // Gets current cursor addresses mask pattern
#define R_STOREC	0x0117 // Records current cursor addresses mask pattern
#define R_SETATR	0x011A // Sets attribute byte
#define R_READC		0x011D // Reads attribute byte of current screen pixel
#define R_SETC		0x0120 // Returns current screen pixel of specified attribute byte
#define R_NSETCX	0x0123 // Sets horizontal screen pixels
#define R_GTASPC	0x0126 // Gets screen aspect ratio
#define R_PNTINI	0x0129 // Initalises the PAINT instruction
#define R_SCANR		0x012C // Scans screen pixels to the right
#define R_SCANL		0x012F // Scans screen pixels to the left

//=============================================================================
// Misc routines
//=============================================================================

#define R_CHGCAP	0x0132 // Alternates the CAPS lamp status
#define R_CHGSND	0x0135 // Alternates the 1-bit sound port status
#define R_RSLREG	0x0138 // Reads the primary slot register
#define R_WSLREG	0x013B // Writes value to the primary slot register
#define R_RDVDP		0x013E // Reads VDP status register
#define R_SNSMAT	0x0141 // Returns the value of the specified line from the keyboard matrix
#define R_PHYDIO	0x0144 // Executes I/O for mass-storage media like disks
#define R_FORMAT	0x0147 // Initialises mass-storage media like formatting of disks
#define R_ISFLIO	0x014A // Tests if I/O to device is taking place
#define R_OUTDLP	0x014D // Printer output
#define R_GETVCP	0x0150 // Returns pointer to play queue
#define R_GETVC2	0x0153 // Returns pointer to variable in queue number VOICEN (byte at #FB38)
#define R_KILBUF	0x0156 // Clears keyboard buffer
#define R_CALBAS	0x0159 // Executes inter-slot call to the routine in BASIC interpreter

//=============================================================================
// MSX2
//=============================================================================
#if (MSX_VERSION >= MSX_2)

#define R_SUBROM	0x015C // Calls a routine in SUB-ROM
#define R_EXTROM	0x015F // Calls a routine in SUB-ROM. Most common way
#define R_CHKSLZ	0x0162 // Searches slots for SUB-ROM
#define R_CHKNEW	0x0165 // Tests screen mode
#define R_EOL		0x0168 // Deletes to the end of the line
#define R_BIGFIL	0x016B // Same function as FILVRM (with 16-bit VRAM-address)
#define R_NSETRD	0x016E // Same function as SETRD (with 16-bit VRAM-address)
#define R_NSTWRT	0x0171 // Same function as SETWRT (with 16-bit VRAM-address)
#define R_NRDVRM	0x0174 // Reads VRAM like in RDVRM (with 16-bit VRAM-address)
#define R_NWRVRM	0x0177 // Writes to VRAM like in WRTVRM (with 16-bit VRAM-address)

#endif // (MSX_VERSION >= MSX_2)

//=============================================================================
// MSX2+
//=============================================================================
#if (MSX_VERSION >= MSX_2P)

#define R_RDRES		0x017A // Reads value of I/O port #F4
#define R_WRRES		0x017D // Writes value to I/O port #F4

#endif // (MSX_VERSION >= MSX_2P)

//=============================================================================
// MSX turbo R
//=============================================================================
#if (MSX_VERSION == MSX_TR)

#define R_CHGCPU	0x0180 // Changes CPU mode
#define R_GETCPU	0x0183 // Returns current CPU mode
#define R_PCMPLY	0x0186 // Plays specified memory area through the PCM chip
#define R_PCMREC	0x0189 // Records audio using the PCM chip into the specified memory area

#endif // (MSX_VERSION == MSX_TR)

