// ____________________________
// ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄▄▄▄  ▄▄   ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀  ██  ██ █ ▀█▄  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▄██▄ ▀█▄▀ ▄▄█▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// BIOS Sub-ROM routines handler 
//
// References:
// - MSX2 Technical Handbook
// - http://map.grauw.nl/resources/msxbios.php
//─────────────────────────────────────────────────────────────────────────────
#pragma once
#include "bios.h"

//-----------------------------------------------------------------------------
//  █▀▀ █▀█ █▄ █ █▀ ▀█▀ ▄▀█ █▄ █ ▀█▀ █▀
//  █▄▄ █▄█ █ ▀█ ▄█  █  █▀█ █ ▀█  █  ▄█
//-----------------------------------------------------------------------------

#define S_SUBRID	0x0000 // String "CD", identification of MSX Sub ROM
const u8 __at(S_SUBRID) g_SUBRID[2];

#define S_STRSRC	0x0002 // Execution address for the startup screen on MSX 2, MSX 2+ or MSX turbo R. This is unofficial and undocumented
const u16 __at(S_STRSRC) g_STRSRC;

//-----------------------------------------------------------------------------
//  █▀█ █▀█ █ █ ▀█▀ █ █▄ █ █▀▀ █▀
//  █▀▄ █▄█ █▄█  █  █ █ ▀█ ██▄ ▄█
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// GRPRT (0089h)
#define S_GRPRT	0x0089
// Function:	One character output to the graphic screen (active only in screen modes 5 to 8)
// Input:		A for the character code
// Output:		none
// Registers:	none
 
//-----------------------------------------------------------------------------
// NVBXLN (00C9h)
#define S_NVBXLN	0x00C9
// Function:	Draws a box
// Input:
// 				Start point: BC for X-coordinate, DE for Y-coordinate
// 				End point:
// 					GXPOS (FCB3h) for X-coordinate
// 					GYPOS (FCB5h) for Y-coordinate
// 				Colour: ATRBYT (F3F3h) for the attribute
// 				Logical operation code: LOGOPR (FB02h)
// Output:		none
// Registers:	all
 
//-----------------------------------------------------------------------------
// NVBXFL (00CDh)
#define S_NVBXFL	0x00CD
// Function:	Draws a painted box
// Input:		Start point: BC for X-coordinate, DE for Y-coordinate
// 				End point:
// 					GXPOS (FCB3h) for X-coordinate
// 					GYPOS (FCB5h) for Y-coordinate
// 				Colour: ATRBYT (F3F3h) for the attribute
// 				Logical operation code: LOGOPR (FB02h)
// Output:		none
// Registers: 	all
 
//-----------------------------------------------------------------------------
// CHGMOD (00D1h)
#define S_CHGMOD	0x00D1
// Function:	Changes the screen mode
// Input:		A for the screen mode (0 to 8)
// Output:		none
// Registers:	all
 
//-----------------------------------------------------------------------------
// INITXT (00D5h)
#define S_INITXT	0x00D5
// Function:	Initialises the screen to TEXT1 mode (40 x 24)
// Input:		TXTNAM (F3B3h) for the pattern name table
// 				TXTCGP (F3B7h) for the pattern generator table
// 				LINL40 (F3AEh) for the length of one line
// Output:		none
// Registers:	all
 
//-----------------------------------------------------------------------------
// INIT32 (00D9h)
#define S_INIT32	0x00D9
// Function:	Initialises the screen to GRAPHIC1 mode (32x24)
// Input:		T32NAM (F3BDh) for the pattern name table
// 				T32COL (F3BFh) for the colour table
// 				T32CGP (F3C1h) for the pattern generator table
// 				T32ATR (F3C3h) for the sprite attribute table
// 				T32PAT (F3C5h) for the sprite generator table
// Output: 		none
// Registers: 	all
 
//-----------------------------------------------------------------------------
// INIGRP (00DDh)
#define S_INIGRP	0x00DD
// Function: 	Initialises the screen to the high-resolution graphics mode
// Input:		GRPNAM (F3C7h) for the pattern name table
// 				GRPCOL (F3C9h) for the colour table
// 				GRPCGP (F3CBh) for the pattern generator table
// 				GRPATR (F3CDh) for the sprite attribute table
// 				GRPPAT (F3CFh) for the sprite generator table
// Output: 		none
// Registers: 	all
 
//-----------------------------------------------------------------------------
// INIMLT (00E1h)
#define S_INIMLT	0x00E1
// Function:	Initialises the screen to MULTI colour mode
// Input:		MLTNAM (F3D1h) for the pattern name table
// 				MLTCOL (F3D3h) for the colour table
// 				MLTCGP (F3D5h) for the pattern generator table
// 				MLTATR (F3D7h) for the sprite attribute table
// 				MLTPAT (F3D9h) for the sprite generator table
// Output: 		none
// Registers: 	all
 
//-----------------------------------------------------------------------------
// SETTXT (00E5h)
#define S_SETTXT	0x00E5
// Function:	Sets VDP in the text mode (40x24)
// Input:		Same as INITXT
// Output:		none
// Registers: 	all
 
//-----------------------------------------------------------------------------
// SETT32 (00E9h)
#define S_SETT32	0x00E9
// Function:	Set VDP in the text mode (32x24)
// Input:		Same as INIT32
// Output:		none
// Registers: 	all
 
//-----------------------------------------------------------------------------
// SETGRP (00EDh)
#define S_SETGRP	0x00ED
// Function:	Sets VDP in the high-resolution mode
// Input:		Same as INIGRP
// Output:		none
// Registers:	all
 
//-----------------------------------------------------------------------------
// SETMLT (00F1h)
#define S_SETMLT	0x00F1
// Function:	Sets VDP in MULTI COLOUR mode
// Input:		Same as INIMLT
// Output:		none
// Registers:	all
 
//-----------------------------------------------------------------------------
// CLRSPR (00F5h)
#define S_CLRSPR	0x00F5
// Function:	Initialises all sprites. The sprite pattern is set to null, sprite number to sprite plane number, and sprite colour to the foreground colour. The vertical location of the sprite is set to 217.
// Input:		SCRMOD (FCAFh) for the screen mode
// Output: 		none
// Registers: 	all
 
//-----------------------------------------------------------------------------
// CALPAT (00F9h)
#define S_CALPAT	0x00F9
// Function:	Returns the address of the sprite generator table (this routine is the same as CALPAT in MAIN-ROM)
// Input:		A for the sprite number
// Output:		HL for the address
// Registers:	AF, DE, HL
 
//-----------------------------------------------------------------------------
// CALATR (00FDh)
#define S_CALATR	0x00FD
// Function:	Returns the address of the sprite attribute table (this routine is the same as CALATR in MAIN-ROM)
// Input:		A for the sprite number
// Output:		HL for the address
// Registers:	AF, DE, HL
 
//-----------------------------------------------------------------------------
// GSPSIZ (0101h)
#define S_GSPSIZ	0x0101
// Function:	Returns the current sprite size (this routine is the same as GSPSIZ in MAIN-ROM)
// Input: 		none
// Output:		A for the sprite size. The CY flag is set only for the size 16 x 16.
// Registers:	AF
 
//-----------------------------------------------------------------------------
// GETPAT (0105h)
#define S_GETPAT	0x0105
// Function:	Returns the character pattern
// Input:		A for the character code
// Output:		PATWRK (FC40h) for the character pattern
// Registers:	all
 
//-----------------------------------------------------------------------------
// WRTVRM (0109h)
#define S_WRTVRM	0x0109
// Function:	Writes data in VRAM
// Input:		HL for VRAM address (0 TO FFFFh), A for data
// Output:		none
// Registers:	AF
 
//-----------------------------------------------------------------------------
// RDVRM (010Dh)
#define S_RDVRM	0x010D
// Function:	Reads the contents of VRAM
// Input:		HL for VRAM address (0 TO FFFFh) to be read
// Output:		A for the value which was read
// Registers:	AF
 
//-----------------------------------------------------------------------------
// CHGCLR (0111h)
#define S_CHGCLR	0x0111
// Function:	Changes the screen colour
// Input:		A for the mode
// 				FORCLR (F3E9h) for the foreground color
// 				BAKCLR (F3EAh) for the background color
// 				BDRCLR (F3EBh) for the border colour
// Output: 		none
// Registers: 	all
 
//-----------------------------------------------------------------------------
// CLSSUB (0115h)
#define S_CLSSUB	0x0115
// Function: 	Clears the screen
// Input: 		none
// Output: 		none
// Registers: 	all
 
//-----------------------------------------------------------------------------
// DSPFNK (011Dh)
#define S_DSPFNK	0x011D
// Function: 	Displays the function keys
// Input: 		none
// Output: 		none
// Registers:	all
 
//-----------------------------------------------------------------------------
// WRTVDP (012Dh)
#define S_WRTVDP	0x012D
// Function: 	Writes data in the VDP register
// Input: 		C for the register number, B for data
// Output: 		none
// Registers: 	AF, BC
 
//-----------------------------------------------------------------------------
// VDPSTA (0131h)
#define S_VDPSTA	0x0131
// Function: 	Reads the VDP register
// Input: 		A for the register number (0 to 9)
// Output: 		A for data
// Registers: 	F
 
//-----------------------------------------------------------------------------
// SETPAG (013Dh)
#define S_SETPAG	0x013D
// Function: 	Switches the page
// Input:		DPPAGE (FAF5h) for the display page number
// 				ACPAGE (FAF6h) for the active page number
// Output: 		none
// Registers: 	AF
 
//-----------------------------------------------------------------------------
// INIPLT (0141h)
#define S_INIPLT	0x0141
// Function: 	Initialises the palette (the current palette is saved in VRAM)
// Input: 		none
// Output: 		none
// Registers:	AF, BC, DE
 
//-----------------------------------------------------------------------------
// RSTPLT (0145h)
#define S_RSTPLT	0x0145
// Function:	Restores the palette from VRAM
// Input: 		none
// Output: 		none
// Registers:	AF, BC, DE
 
//-----------------------------------------------------------------------------
// GETPLT (0149h)
#define S_GETPLT	0x0149
// Function:	Obtains the colour code from the palette
// Input: 		D for the palette number (0 to 15)
// Output:		4 high order bits of B for red code
// 				4 low order bits of B for blue code
// 				4 low order bits of C for green code
// Registers:	AF, DE
 
//-----------------------------------------------------------------------------
// SETPLT (014Dh)
#define S_SETPLT	0x014D
// Function:	Sets the colour code to the palette
// Input:		D for the palette number (0 to 15)
// 				4 high order bits of A for red code
// 				4 low order bits of A for blue code
// 				4 low order bits of E for green code
// Output: 		none
// Registers: 	AF
 
//-----------------------------------------------------------------------------
// BEEP (017Dh)
#define S_BEEP	0x017D
// Function: 	Generates BEEP
// Input: 		none
// Output: 		none
// Registers: 	all
 
//-----------------------------------------------------------------------------
// PROMPT (0181h)
#define S_PROMPT	0x0181
// Function: 	Displays the prompt
// Input: 		none
// Output: 		none
// Registers: 	all
 
//-----------------------------------------------------------------------------
// NEWPAD (01ADh)
#define S_NEWPAD	0x01AD
// Function:	Reads the status of mouse or light pen
// Input:		Call with setting the following data in A; descriptions in parenthesis are return values.
// 				8: light pen check (valid at 0FFh)
// 				9: returns X-coordinate
// 				10: returns Y-coordinate
// 				11: returns the light pen switch status (0FFh, when pressed)
// 				12: whether the mouse is connected to the port 1 (valid at 0FFh)
// 				13: returns the offset in X direction
// 				14: returns the offset in Y direction
// 				15: (always 0)
// 				16: whether the mouse is connected to the port 2 (valid at 0FFh)
// 				17: returns the offset in X direction
// 				18: returns the offset in Y direction
// 				19: (always 0)
// Output: 		A
// Registers: 	all
 
//-----------------------------------------------------------------------------
// CHGMDP (01B5h)
#define S_CHGMDP	0x01B5
// Function:	Changes VDP mode. The palette is initialised.
// Input: 		A for the screen mode (0 to 8)
// Output: 		none
// Registers: 	all
 
//-----------------------------------------------------------------------------
// KNJPRT (01BDh)
#define S_KNJPRT	0x01BD
// Function:	Sends a kanki to the graphic screen (modes 5 to 8)
// Input:		BC for JIS kanji code, A for the display mode. The display mode has the following meaning, similar to the PUT KANJI command of BASIC.
// 				0: display in 16 x 16 dot
// 				1: display even dots
// 				2: display odd dots
 
//-----------------------------------------------------------------------------
// REDCLK (01F5h)
#define S_REDCLK	0x01F5
// Function:	Reads the clock data
// Input: 		C for RAM address of the clock
// 				7 6 5 4 3 2 1 0
// 				    │ │ └─┴─┴─┴── Address (0 to 15)
// 				    └─┴────────── Mode (0 to 3)
// Output:		A for the data which were read (only 4 low order bits are valid)
// Registers: 	F
 
//-----------------------------------------------------------------------------
// WRTCLK (01F9h)
#define S_WRTCLK	0x01F9
// Function:	Writes the clock data
// Input:		A for the data to be written, C for RAM address of the clock
// Output: 		none
// Registers: 	F
 

