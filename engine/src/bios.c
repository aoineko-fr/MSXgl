// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄▄▄▄  ▄▄   ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀  ██  ██ █ ▀█▄  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▄██▄ ▀█▄▀ ▄▄█▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// BIOS routines handler 
//
// References:
// - MSX2 Technical Handbook
// - http://map.grauw.nl/resources/msxbios.php
// - https://www.msx.org/wiki/Main-ROM_BIOS
// - Pratique du MSX 2
//─────────────────────────────────────────────────────────────────────────────
#include "bios.h"
#include "dos.h"

//-----------------------------------------------------------------------------
//  █ █ █▀▀ █   █▀█ █▀▀ █▀█
//  █▀█ ██▄ █▄▄ █▀▀ ██▄ █▀▄
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Handle clean transition to Basic or MSX-DOS environment
// For MSX-DOS, return value is in L
void Bios_Exit(u8 ret)
{
	ret;	// A
#if (TARGET_TYPE == TYPE_DOS)

	__asm
	#if BIOS_USE_VDP
		push	af
		// Set Screen mode to 2...
		ld		a, #2
		ld		ix, #R_CHGMOD
		ld		iy, (M_EXPTBL-1)
		call	R_CALSLT
		// ... to be able to call TOTEXT routine
		ld		ix, #R_TOTEXT
		ld		iy, (M_EXPTBL-1)
		call	R_CALSLT
		pop		af
	#endif
		ld		b, a
		ld		c, #DOS_FUNC_TERM
		call	BDOS
		ld		c, #DOS_FUNC_TERM0
		jp		BDOS
	__endasm;

#elif (TARGET_TYPE == TYPE_BIN)
	
	__asm
	#if BIOS_USE_VDP
		// Set Screen mode to 2...
		ld		a, #2
		call	R_CHGMOD
		// ... to be able to call TOTEXT routine
		call	R_TOTEXT
	#endif
		// 
		ld		ix, #0x409B // address of "warm boot" BASIC interpreter
		// this routine is called to reset the stack if basic is externally stopped and then restarted.
		call	R_CALBAS
	__endasm;

#else // if (TARGET_TYPE == TYPE_ROM)

	__asm
		call	0x0000				// Soft reset
	__endasm;

#endif
}


//-----------------------------------------------------------------------------
// Set a safe hook jump to given function
void Bios_SetHookCallback(u16 hook, callback cb)
{
	u8 slot = Sys_GetPageSlot((u16)cb >> 14);
	Bios_SetHookInterSlotCallback(hook, slot, cb);
}

//-----------------------------------------------------------------------------
//  █▀▄▀█ ▄▀█ █ █▄ █ ▄▄ █▀█ █▀█ █▀▄▀█
//  █ ▀ █ █▀█ █ █ ▀█    █▀▄ █▄█ █ ▀ █
//-----------------------------------------------------------------------------

//=============================================================================
//
// RST-and other routines
//
//=============================================================================

//-----------------------------------------------------------------------------
// CHKRAM (STARTUP, RESET, BOOT)
// Address  : #0000
// Function : Tests RAM and sets RAM slot for the system
// Registers: All
// Remark   : After this, a jump must be made to INIT, for further initialisation

// inline void Bios_Startup() { Call(R_CHKRAM); }

//-----------------------------------------------------------------------------
// SYNCHR
// Address  : #0008
// Function : Tests whether the character of (HL) is the specified character
//            if not, it generates SYNTAX ERROR, otherwise it goes to CHRGTR (#0010)
// Input    : Set the character to be tested in (HL) and the character to be
//            compared next to RST instruction which calls this routine (inline parameter)
// Output   : HL is increased by one and A receives (HL), When the tested character is
//            numerical, the carry flag is set the end of the statement (00h or 3Ah) causes
//            the zero flag to be set
// Registers: AF, HL

//@todo To be implemented

//-----------------------------------------------------------------------------
// RDSLT
// Address  : #000C
// Function : Reads the value of an address in another slot
// Input    : A  - ExxxSSPP  Slot-ID
//                 │   ││└┴─ Primary slot number (00-11)
//                 │   └┴─── Secondary slot number (00-11)
//                 └──────── Expanded slot (0 = no, 1 = yes)
//            HL - Address to read
// Output   : A  - Contains the value of the read address
// Registers: AF, C, DE
// Remark   : Can be call directly from MSX-DOS
//            This routine turns off the interupt, but won't turn it on again
u8 Bios_InterSlotRead(u8 slot, u16 addr)
{
	slot; // A
	addr; // DE

	__asm
		ld		l, e
		ld		h, d
		call	R_RDSLT
	__endasm;
}

//-----------------------------------------------------------------------------
// CHRGTR
// Address  : #0010
// Function : Gets the next character (or token) of the Basic text
// Input    : HL - Address last character
// Output   : HL - Points to the next character
//            A  - Contains the character
//            Carry flag set if it's a number
//            Zero flag set if it's the end of the statement
// Registers: AF, HL

//@todo To be implemented

//-----------------------------------------------------------------------------
// WRSLT
// Address  : #0014 
// Function : Writes a value to an address in another slot
// Input    : A  - ExxxSSPP  Slot-ID
//                 │   ││└┴─ Primary slot number (00-11)
//                 │   └┴─── Secondary slot number (00-11)
//                 └──────── Expanded slot (0 = no, 1 = yes)
//            HL - Address
//            E  - Value
// Registers: AF, BC, D
// Remark   : Can be call directly from MSX-DOS
//            This routine turns off the interupt, but won't turn it on again
void Bios_InterSlotWrite(u8 slot, u16 addr, u8 value)
{
	slot;  // A
	addr;  // DE
	value; // (SP+4)

	__asm
		push	iy
		ld		iy, #4
		add		iy, sp

		ld		l, e
		ld		h, d
		ld		e, 0(iy)	
		call	R_WRSLT
		pop		iy
	__endasm;
}

//-----------------------------------------------------------------------------
// OUTDO
// Address  : #0018
// Function : Output to current output channel (printer, file, etc.)
// Input    : A  - PRTFIL, PRTFLG
// Remark   : Used in basic, in ML it's pretty difficult

//@todo To be implemented

//-----------------------------------------------------------------------------
// CALSLT
// Address  : #001C
// Function : Executes inter-slot call.
// Input    : IY - High byte with slot ID, see RDSLT
//            IX - The address that will be called
// Remark   : Can be call directly from MSX-DOS
//            Variables can never be given in alternative registers or IX and IY
void Bios_InterSlotCall(u8 slot, u16 addr)
{
	slot; // A
	addr; // DE
	__asm
		push	ix

		ld		l, #0
		ld		h, a
		push	hl
		pop		iy

		push	de
		pop		ix

		call	R_CALSLT

		ei							// because CALSLT do DI
		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
// DCOMPR
// Address  : #0020
// Function : Compares HL with DE
// Input    : HL, DE
// Output   : Zero flag set if HL and DE are equal. Carry flag set if HL is less than DE
// Registers: AF

//@todo To be implemented

//-----------------------------------------------------------------------------
// ENASLT
// Address  : #0024
// Function : selects the slot corresponding to the value of A and enables the slot to be used.
//            When this routine is called, interrupts are inhibited and remain so even after execution ends.
// Input    : A - Slot ID, see RDSLT
//            H - Bit 6 and 7 must contain the page number (00-11)
		   
void Bios_SwitchSlot(u8 page, u8 slot)
{
	page; // A
	slot; // H

	__asm
		LShift(6)					// A << 6
		ld		b, h				// 
		ld		h, a
		ld		a, b
		call	R_ENASLT
		// ei							// because ENASLT do DI
	__endasm;
}

//-----------------------------------------------------------------------------
// GETYPR
// Address  : #0028
// Function : Returns Type of DAC
// Input    : DAC
// Output   : S,Z,P/V, CY
// Registers: AF
// Remark   : The Main-Rom must be selected in the page 4000h~7FFFh to call this routine

//@todo To be implemented

//-----------------------------------------------------------------------------
// CALLF
// Address  : #0030
// Function : Executes an interslot call
// Output   : Depends on the calling routine
// Registers: AF, and the other registers depending on the calling routine
// Remark   : The following is the calling sequence:
//            RST #30
//            DB destination slot ID, see RDSLT
//            DW destination address

//@see Bios_InterSlotCallF(_slot, _addr)
		   
//-----------------------------------------------------------------------------
// KEYINT
// Address  : #0038
// Function : Executes the timer interrupt process routine


//=============================================================================
//
// Initialization-routines
//
//=============================================================================

//-----------------------------------------------------------------------------
// INITIO
// Address  : #003B
// Function : Initialises the device
// Registers: All

//-----------------------------------------------------------------------------
// INIFNK
// Address  : #003E
// Function : Initialises the contents of the function keys
// Registers: All


//=============================================================================
//
// VDP routines
//
//=============================================================================
#if BIOS_USE_VDP

//-----------------------------------------------------------------------------
// DISSCR
// Address  : #0041
// Function : Inhibits the screen display
// Registers: AF, BC
// void Bios_DisableScreen() { Call(R_DISSCR); }

//-----------------------------------------------------------------------------
// ENASCR
// Address  : #0044
// Function : Displays the screen
// Registers: AF, BC
// void Bios_EnableScreen() { Call(R_ENASCR); }

//-----------------------------------------------------------------------------
// WRTVDP
// Address  : #0047
// Function : Write data in the VDP-register
// Input    : B  - Data to write
//            C  - Number of the register
// Registers: AF, BC
// Remark   : Call Sub-ROM on MSX2 or later if the bit EV of VDP register 0 is changed or if register number is greater than 7
void Bios_WriteVDP(u8 reg, u8 value)
{
	reg;   // A
	value; // L

	__asm
		ld		c, a
		ld		b, l
		call	R_WRTVDP
	__endasm;
}

//-----------------------------------------------------------------------------
// RDVRM
// Address  : #004A
// Function : Reads the content of VRAM
// Input    : HL - VRAM address (00000h~03FFFh)
// Output   : A  - Value which was read
// Registers: AF
// Remark   : Routine for MSX1, so only the 14 low order bits of the VRAM address are valid. To use all bits, call NRDVRM (00174h)

// inline u8 Bios_ReadVRAM(u16 addr) { return ((u8(*)(u16))R_RDVRM)(addr); }

//-----------------------------------------------------------------------------
// WRTVRM
// Address  : #004D
// Function : Writes data in VRAM
// Input    : HL - VRAM address (00000h~03FFFh)
//            A  - Value write
// Registers: AF
// Remark   : Routine for MSX1, so only the 14 low order bits of the VRAM address are valid. To use all bits, call NWRVRM (00177h)
void Bios_WriteVRAM(u16 addr, u8 value)
{
	addr;  // HL
	value; // (SP+4)

	__asm
		ld		iy, #4
		add		iy, sp

		ld		a, 0(iy)
		call	R_WRTVRM
	__endasm;
}

//-----------------------------------------------------------------------------
// SETRD
// Address  : #0050
// Function : Sets VRAM address to read a byte or more. This is used to read data from the sequential VRAM area by using the address auto-increment function of VDP.
//            This enables faster readout than using RDVRM in a loop.
// Input    : HL - VRAM address (00000h~03FFFh)
// Registers: AF
// Remark   : Routine for MSX1, so only the 14 low order bits of the VRAM address are valid. To use all bits, call NSETRD (0016Eh)

// inline void Bios_SetAddressForRead(u16 addr) { CallHL(R_SETRD, addr); }

//-----------------------------------------------------------------------------
// SETWRT
// Address  : #0053
// Function : Sets VRAM address to write or more. The purpose is the same as SETRD.
// Input    : HL - VRAM address (00000h~03FFFh)
// Registers: AF
// Remark   : Routine for MSX1, so only the 14 low order bits of the VRAM address are valid. To use all bits, call NSTWRT (00171h).

// inline void Bios_SetAddressForWrite(u16 addr) { CallHL(R_SETWRT, addr); }

//-----------------------------------------------------------------------------
// FILVRM
// Address  : #0056
// Function : Fills the specified VRAM area with the same data
// Input    : HL - VRAM address to begin writing (00000h~03FFFh)
//            BC - The length of the area to be written
//            A  - Data byte
// Registers: AF, BC
// Remark   : Routine for MSX1, so only the 14 low order bits of the VRAM address are valid. To use all bits, call BIGFIL (0016Bh)
void Bios_FillVRAM(u16 addr, u16 length, u8 value)
{
	addr;   // HL
	length; // DE
	value;  // (SP+4)

	__asm
		ld		iy, #4
		add		iy, sp

		ld		c, e
		ld		b, d
		ld		a, 0(iy)
		call	R_FILVRM
	__endasm;
}

//-----------------------------------------------------------------------------
// LDIRMV
// Address  : #0059
// Function : Block transfer from VRAM to memory 
// Input    : HL - Start address of VRAM
//            DE - Start address of memory
//            BC - Block length
// Registers: All
// Remark   : In screen modes other than MSX1. The variable ACPAGE (0FAF6h) indicates the page from which the transfer must take place
void Bios_TransfertVRAMtoRAM(u16 vram, u16 ram, u16 length)
{
	vram;   // LH
	ram;    // DE
	length; // (SP+4)

	__asm
		ld		iy, #4
		add		iy, sp

		ld		c, 0(iy)
		ld		b, 1(iy)
		call	R_LDIRMV
	__endasm;
}

//-----------------------------------------------------------------------------
// LDIRVM
// Address  : #005C
// Function : Block transfer from memory to VRAM 
// Input    : HL - Start address of memory
//            DE - Start address of VRAM
//            BC - Block length
// Registers: All
// Remark   : In screen modes other than MSX1. The variable ACPAGE (0FAF6h) indicates the page from which the transfer must take place
void Bios_TransfertRAMtoVRAM(u16 ram, u16 vram, u16 length)
{
	ram;    // HL
	vram;   // DE
	length; // (SP+4)
	
	__asm
		ld		iy, #4
		add		iy, sp

		ld		c, 0(iy)
		ld		b, 1(iy)
		call	R_LDIRVM
	__endasm;
}

//-----------------------------------------------------------------------------
// CHGMOD
// Address  : #005F
// Function : Switches to given screen mode
// Input    : A  - Screen mode
// Registers: All
// Remark   : Colors palette is not initialized by this routine, call the routine CHGMDP (001B5h in Sub-ROM) if you need to initialize the palette

// inline void Bios_ChangeMode(u8 screen) { Call(R_CHGMOD); }

//-----------------------------------------------------------------------------
// CHGCLR
// Address  : #0062
// Function : Changes the screen colors
// Input    : Foreground color in FORCLR
//            Background color in BAKCLR
//            Border color in BDRCLR
// Registers: All
// Remark   : Same function as CHGCLR (00111h in Sub-ROM)
void Bios_ChangeColor(u8 text, u8 back, u8 border)
{
	text;   // A
	back;   // L
	border; // (SP+4)
	__asm
		ld		iy, #4
		add		iy, sp

		ld  	(M_FORCLR), a
		ld  	a, l
		ld  	(M_BAKCLR), a
		ld  	a, 0(iy)
		ld  	(M_BDRCLR), a
		call	R_CHGCLR
	__endasm;
}

//-----------------------------------------------------------------------------
// NMI
// Address  : #0066
// Function : Executes non-maskable interupt handling routine

//-----------------------------------------------------------------------------
// CLRSPR
// Address  : #0069
// Function : Initialises all sprites
// Input    : SCRMOD
// Registers: All

//-----------------------------------------------------------------------------
// INITXT
// Address  : #006C)
// Function : Switches to SCREEN 0 (text screen with 40×24 characters)
// Input    : TXTNAM (#F3B3) = Address of pattern name table
//            TXTCGP (#F3B7) = Address of pattern generator table
//            LINL40 (#F3AE) = Character per line
//            FORCLR (#F3E9) = Text color
//            BAKCLR (#F3EA) = Background color
//            BDRCLR (#F3EB) = Border colour
// Registers: All
// void Bios_InitScreen0() { Call(R_INITXT); }
void Bios_InitScreen0Ex(u16 pnt, u16 pgt, u8 width, u8 text, u8 bg, u8 border)
{
	g_TXTNAM = pnt;    // Address of pattern name table
	g_TXTCGP = pgt;    // Address of pattern generator table
	g_LINL40 = width;  // Character per line
	g_FORCLR = text;   // Text color
	g_BAKCLR = bg;     // Background color
	g_BDRCLR = border; // Border colour
	Call(R_INITXT);
}

//-----------------------------------------------------------------------------
// INIT32
// Address  : #006F
// Function : Switches to SCREEN 1 (text screen with 32×24 characters)
// Input    : T32NAM (#F3BD) = Address of pattern name table
//            T32COL (#F3BF) = Address of colors table
//            T32CGP (#F3C1) = Address of pattern generator table
//            T32ATR (#F3C3) = Address of sprite attribute table
//            T32PAT (#F3C5) = Address of sprite generator table
//            FORCLR (#F3E9) = Text color
//            BAKCLR (#F3EA) = Background color
//            BDRCLR (#F3EB) = Border color
// Registers: All
// void Bios_InitScreen1() { Call(R_INIT32); }
void Bios_InitScreen1Ex(u16 pnt, u16 ct, u16 pgt, u16 sat, u16 sgt, u8 text, u8 bg, u8 border)
{
	g_T32NAM = pnt;    // Address of pattern name table
	g_T32COL = ct;     // Address of colors table
	g_T32CGP = pgt;    // Address of pattern generator table
	g_T32ATR = sat;    // Address of sprite attribute table
	g_T32PAT = sgt;    // Address of sprite generator table
	g_FORCLR = text;   // Text color
	g_BAKCLR = bg;     // Background color
	g_BDRCLR = border; // Border color
	Call(R_INIT32);
}

//-----------------------------------------------------------------------------
// INIGRP
// Address  : #0072
// Function : Switches to SCREEN 2 (high resolution screen with 256×192 pixels)
// Input    : GRPNAM (#F3C7) = Address of pattern name table
//            GRPCOL (#F3C9) = Address of colors table
//            GRPCGP (#F3CB) = Address of pattern generator table
//            GRPATR (#F3CD) = Address of sprite attribute table
//            GRPPAT (#F3CF) = Address of sprite generator table
//            FORCLR (#F3E9) = Text color
//            BAKCLR (#F3EA) = Background color
//            BDRCLR (#F3EB) = Border color
// Registers: All
// void Bios_InitScreen2() { Call(R_INIGRP); }
void Bios_InitScreen2Ex(u16 pnt, u16 ct, u16 pgt, u16 sat, u16 sgt, u8 text, u8 bg, u8 border)
{
	g_GRPNAM = pnt;    // Address of pattern name table
	g_GRPCOL = ct;     // Address of colors table
	g_GRPCGP = pgt;    // Address of pattern generator table
	g_GRPATR = sat;    // Address of sprite attribute table
	g_GRPPAT = sgt;    // Address of sprite generator table
	g_FORCLR = text;   // Text color
	g_BAKCLR = bg;     // Background color
	g_BDRCLR = border; // Border color
	Call(R_INIGRP);
}

//-----------------------------------------------------------------------------
// INIMLT
// Address  : #0075
// Function : Switches to SCREEN 3 (multi-color screen with 64×48 pixels)
// Input    : MLTNAM (#F3D1) = Address of pattern name table
//            MLTCOL (#F3D3) = Address of colors table
//            MLTCGP (#F3D5) = Address of pattern generator table
//            MLTATR (#F3D7) = Address of sprite attribute table
//            MLTPAT (#F3D9) = Address of sprite generator table
//            FORCLR (#F3E9) = Text color
//            BAKCLR (#F3EA) = Background color
//            BDRCLR (#F3EB) = Border color
// Registers: All
// void Bios_InitScreen3() { Call(R_INIMLT); }
void Bios_InitScreen3Ex(u16 pnt, u16 ct, u16 pgt, u16 sat, u16 sgt, u8 text, u8 bg, u8 border)
{
	g_MLTNAM = pnt;    // Address of pattern name table
	g_MLTCOL = ct;     // Address of colors table
	g_MLTCGP = pgt;    // Address of pattern generator table
	g_MLTATR = sat;    // Address of sprite attribute table
	g_MLTPAT = sgt;    // Address of sprite generator table
	g_FORCLR = text;   // Text color
	g_BAKCLR = bg;     // Background color
	g_BDRCLR = border; // Border color
	Call(R_INIMLT);
}

//-----------------------------------------------------------------------------
// SETTXT
// Address  : #0078
// Function : Switches VDP to SCREEN 0 mode
// Input    : See INITXT
// Registers: All
// void Bios_SetScreen0() { Call(R_SETTXT); }

//-----------------------------------------------------------------------------
// SETT32
// Address  : #007B
// Function : Switches VDP to SCREEN 1 mode
// Input    : See INIT32
// Registers: All
// void Bios_SetScreen1() { Call(R_SETT32); }

//-----------------------------------------------------------------------------
// SETGRP
// Address  : #007E
// Function : Switches VDP to SCREEN 2 mode
// Input    : See INIGRP
// Registers: All
// void Bios_SetScreen2() { Call(R_SETGRP); }

//-----------------------------------------------------------------------------
// SETMLT
// Address  : #0081
// Function : Switches VDP to SCREEN 3 mode
// Input    : See INIMLT
// Registers: All
// void Bios_SetScreen3() { Call(R_SETMLT); }

//-----------------------------------------------------------------------------
// CALPAT
// Address  : #0084
// Function : Returns the address of the sprite pattern table
// Input    : A  - Sprite ID
// Output   : HL - For the address
// Registers: AF, DE, HL
u16 Bios_GetPatternTableAddress(u8 id) __FASTCALL
{
	id; // L

	__asm
		ld		a, l
		call	R_CALPAT
	__endasm;
}

//-----------------------------------------------------------------------------
// CALATR
// Address  : #0087
// Function : Returns the address of the sprite attribute table
// Input    : A  - Sprite number
// Output   : HL - For the address
// Registers: AF, DE, HL
u16 Bios_GetAttributeTableAddress(u8 id) __FASTCALL
{
	id; // L

	__asm
		ld		a, l
		call	R_CALATR
	__endasm;
}

//-----------------------------------------------------------------------------
// GSPSIZ
// Address  : #008A
// Function : Returns current sprite size
// Output   : A  - Sprite size in bytes
//            Carry flag set when size is 16×16 sprites otherwise Carry flag is reset
// Registers: AF

// inline u8 Bios_GetSpriteSize() { return ((u8(*)(void))R_GSPSIZ)(); }

//-----------------------------------------------------------------------------
// GRPPRT
// Address  : #008D
// Function : Displays a character on the graphic screen
// Input    : A  - ASCII value of the character to print

// inline void Bios_GraphPrintChar(u8 chr) { ((void(*)(u8))R_GRPPRT)(chr); }

// Input    : A  - ASCII value of the character to print
//            CLOC (#F92A) = In SCREEN 2 to 4 and 10 to 12, address where is the graphical cursor in VRAM. In SCREEN 5 to 8, the current abscissa of the graphical cursor
//            CMASK (#F92C) = In SCREEN 2 to 4 and 10 to 12, mask to apply. In SCREEN 5 to 8, current ordinate of the graphical cursor
//            LOGOPR (#FB02) = Logical operation code (for graphic modes of SCREEN 5 to 12)
void Bios_GraphPrintCharEx(u8 chr, u16 x, u8 y, u8 color, u8 op)
{
	g_GRPACX = x;
	g_GRPACY = y;
	g_ATRBYT = color;
	g_LOGOPR = op;
	Bios_GraphPrintChar(chr);
}

#endif // BIOS_USE_VDP


//=============================================================================
//
// PSG routines
//
//=============================================================================
#if BIOS_USE_PSG

//-----------------------------------------------------------------------------
// GICINI
// Address  : #0090
// Function : Initialises PSG and sets initial value for the PLAY statement
// Registers: All
// Remarks  : Interrupts must be disabled to call this routine

// inline void Bios_InitPSG() { DisableInterrupt(); Call(R_GICINI); EnableInterrupt(); }

//-----------------------------------------------------------------------------
// WRTPSG
// Address  : #0093
// Function : Writes data to PSG register
// Input    : A  - PSG register number
//            E  - Data write
// Remarks  : See https://www.msx.org/wiki/SOUND#Registers_Description
void Bios_WritePSG(u8 reg, u8 value)
{
	reg;   // A
	value; // L

	__asm
		ld		e, l
		call	R_WRTPSG
	__endasm;	
}

//-----------------------------------------------------------------------------
// RDPSG
// Address  : #0096
// Function : Reads value from PSG register
// Input    : A  - PSG register read
// Output   : A  - Value read

// inline u8 Bios_ReadPSG(u8 reg) { return ((u8(*)(u8))R_RDPSG)(reg); }

//-----------------------------------------------------------------------------
// STRTMS
// Address  : #0099
// Function : Tests whether the PLAY statement is being executed as a background
//            task. If not, begins to execute the PLAY statement
// Registers: All

// void Bios_PlayPSG() { Call(R_STRTMS); }

#endif // BIOS_USE_PSG


//=============================================================================
//
// Console routines
//
//=============================================================================

//-----------------------------------------------------------------------------
// CHSNS
// Address  : #009C
// Function : Tests the status of the keyboard buffer
// Output   : Zero flag set if buffer is empty, otherwise not set
// Registers: AF

//-----------------------------------------------------------------------------
// CHGET
// Address  : #009F
// Function : Waits for a character input on the keyboard
// Output   : A  - ASCII code of the input character
// Registers: AF

// inline u8 Bios_GetCharacter() { return ((u8(*)(void))R_CHGET)(); }

//-----------------------------------------------------------------------------
// CHPUT
// Address  : #00A2
// Function : Displays a character in text mode
// Input    : A  - ASCII code of character to display

// inline void Bios_TextPrintChar(u8 chr) { ((void(*)(u8))R_CHPUT)(chr); }

//-----------------------------------------------------------------------------
// LPTOUT
// Address  : #00A5
// Function : Sends one character to printer
// Input    : A  - ASCII code of character to send
// Output   : Carry flag set if failed
// Registers: F

//-----------------------------------------------------------------------------
// LPTSTT
// Address  : #00A8
// Function : Tests printer status
// Output   : A  - #FF and zero flag reset if printer is ready
//                 #00 and zero flag set if not ready
// Registers: AF

//-----------------------------------------------------------------------------
// CNVCHR
// Address  : #00AB
// Function : Tests for the graphic header and transforms the code
// Input    : A  - Character code
// Output   : The carry flag is reset to not the graphic reader
//            The carry flag and zero flag are set to the transformed code is set in A
//            The carry flag is set and zero flag is reset to the untransformed code is set in A
// Registers: AF

//-----------------------------------------------------------------------------
// PINLIN
// Address  : #00AE
// Function : Stores in the specified buffer the character codes input until the return
//            key or STOP key is pressed
// Output   : HL - For the starting address of the buffer -1
//            Carry flag set when it ends with the STOP key
// Registers: All

//-----------------------------------------------------------------------------
// INLIN
// Address  : #00B1
// Function : Same as PINLIN except that AUGFLG (#F6AA) is set
// Output   : HL - For the starting address of the buffer -1
//            Carry flag set when it ends with the STOP key
// Registers: All

//-----------------------------------------------------------------------------
// QINLIN
// Address  : #00B4
// Function : Prints a question mark and one space
// Output   : HL - For the starting address of the buffer -1
//            Carry flag set when it ends with the STOP key
// Registers: All

//-----------------------------------------------------------------------------
// BREAKX
// Address  : #00B7
// Function : Tests status of CTRL-STOP
// Output   : Carry flag set when pressed
// Registers: AF
// Remark   : In this routine, interrupts are inhibited

//-----------------------------------------------------------------------------
// ISCNTC
// Address  : #00BA
// Function : Tests status of SHIFT-STOP

//-----------------------------------------------------------------------------
// CKCNTC
// Address  : #00BD
// Function : Same as ISCNTC. used in Basic

//-----------------------------------------------------------------------------
// BEEP
// Address  : #00C0
// Function : Generates beep
// Registers: All

//-----------------------------------------------------------------------------
// CLS
// Address  : #00C3
// Function : Clears the screen
// Registers: AF, BC, DE
// Remark   : Zero flag must be set to be able to run this routine
//            XOR A will do fine most of the time
void Bios_ClearScreen()
{
	__asm
		xor		a, a
		call	R_CLS
	__endasm;
}

//-----------------------------------------------------------------------------
// POSIT
// Address  : #00C6
// Function : Moves cursor to the specified position
// Input    : H  - X coordinate of cursor
//            L  - Y coordinate of cursor
// Registers: AF
void Bios_SetCursorPosition(u8 X, u8 Y)
{
	X; // A
	Y; // L

	__asm
		ld		h, a
		call	R_POSIT	
	__endasm;	
}

//-----------------------------------------------------------------------------
// FNKSB
// Address  : #00C9
// Function : Tests whether the function key display is active (FNKFLG)
//            If so, displays them, otherwise erase them
// Input    : FNKFLG (#FBCE)
// Registers: All

//-----------------------------------------------------------------------------
// ERAFNK
// Address  : #00CC
// Function : Erase functionkey display
// Registers: All

//-----------------------------------------------------------------------------
// DSPFNK
// Address  : #00CF
// Function : Displays the function keys
// Registers: All

//-----------------------------------------------------------------------------
// TOTEXT
// Address  : #00D2
// Function : Forces the screen to be in the text mode
// Registers: All


//=============================================================================
//
// Controller routines
//
//=============================================================================

//-----------------------------------------------------------------------------
// GTSTCK
// Address  : #00D5
// Function : Returns the joystick status
// Input    : A  - Joystick number to test (0 = cursors, 1 = port 1, 2 = port 2)
// Output   : A  - Direction
// Registers: All

// inline u8 Bios_GetJoystickDirection(u8 port) { return ((u8(*)(u8))R_GTSTCK)(port); }

//-----------------------------------------------------------------------------
// GTTRIG
// Address  : #00D8
// Function : Returns current trigger status
// Input    : A  - Trigger button to test
//            0 = space bar
//            1 = port 1, button A
//            2 = port 2, button A
//            3 = port 1, button B
//            4 = port 2, button B
// Output   : A  - #00 trigger button not pressed
//                 #FF trigger button pressed
// Registers: AF

// inline bool Bios_GetJoystickTrigger(u8 trigger) { return ((u8(*)(u8))R_GTTRIG)(trigger); }

//-----------------------------------------------------------------------------
// GTPAD
// Address  : #00DB
// Function : Returns current touch pad status
// Input    : A  - Function call number. Fetch device data first, then read.
//            [ 0]   Fetch touch pad data from port 1 (#FF if available)
//            [ 1]   Read X-position
//            [ 2]   Read Y-position
//            [ 3]   Read touchpad status from port 1 (#FF if pressed)
//            [ 4]   Fetch touch pad data from port 2 (#FF if available)
//            [ 5]   Read X-position
//            [ 6]   Read Y-position
//            [ 7]   Read touchpad status from port 2 (#FF if pressed)
// Output   : A  - Value
// Registers: All
// Remark   : On MSX2, function call numbers 8-23 are forwarded to NEWPAD in the SubROM.

//-----------------------------------------------------------------------------
// GTPDL
// Address  : #00DE
// Function : Returns current value of paddle
// Input    : A  - Paddle number
// Output   : A  - Value
// Registers: All


//=============================================================================
//
// Tape device routines
//
//=============================================================================

//-----------------------------------------------------------------------------
// TAPION
// Address  : #00E1
// Function : Reads the header block after turning the cassette motor on
// Output   : Carry flag set if failed
// Registers: All

//-----------------------------------------------------------------------------
// TAPIN
// Address  : #00E4
// Function : Read data from the tape
// Output   : A  - Read value
//            Carry flag set if failed
// Registers: All

//-----------------------------------------------------------------------------
// TAPIOF
// Address  : #00E7
// Function : Stops reading from the tape

//-----------------------------------------------------------------------------
// TAPOON
// Address  : #00EA
// Function : Turns on the cassette motor and writes the header
// Input    : A  - #00 short header
//             not #00 long header
// Output   : Carry flag set if failed
// Registers: All

//-----------------------------------------------------------------------------
// TAPOUT
// Address  : #00ED
// Function : Writes data on the tape
// Input    : A  - Data to write
// Output   : Carry flag set if failed
// Registers: All

//-----------------------------------------------------------------------------
// TAPOOF
// Address  : #00F0
// Function : Stops writing on the tape

//-----------------------------------------------------------------------------
// STMOTR
// Address  : #00F3
// Function : Sets the cassette motor action
// Input    : A  - #00 stop motor
//                 #01 start motor
//                 #FF reverse the current action
// Registers: AF


//=============================================================================
//
// Queue routines
//
//=============================================================================

//-----------------------------------------------------------------------------
// LFTQ
// Address  : #00F6
// Function : Gives number of bytes in queue
// Output   : A  - Length of queue in bytes
// Remark   : Internal use

//-----------------------------------------------------------------------------
// PUTQ
// Address  : #00F9
// Function : Put byte in queue
// Remark   : Internal use


//=============================================================================
//
// Graphic routines
//
//=============================================================================

//-----------------------------------------------------------------------------
// RIGHTC
// Address  : #00FC
// Function : Shifts screen pixel to the right
// Registers: AF

//-----------------------------------------------------------------------------
// LEFTC
// Address  : #00FF
// Function : Shifts screen pixel to the left
// Registers: AF

//-----------------------------------------------------------------------------
// UPC
// Address  : #0102
// Function : Shifts screen pixel up
// Registers: AF

//-----------------------------------------------------------------------------
// TUPC
// Address  : #0105
// Function : Tests whether UPC is possible, if possible, execute UPC
// Output   : Carry flag set if operation would end outside the screen
// Registers: AF

//-----------------------------------------------------------------------------
// DOWNC
// Address  : #0108
// Function : Shifts screen pixel down
// Registers: AF

//-----------------------------------------------------------------------------
// TDOWNC
// Address  : #010B
// Function : Tests whether DOWNC is possible, if possible, execute DOWNC
// Output   : Carry flag set if operation would end outside the screen
// Registers: AF

//-----------------------------------------------------------------------------
// SCALXY
// Address  : #010E
// Function : Scales X and Y coordinates

//-----------------------------------------------------------------------------
// MAPXY
// Address  : #0111
// Function : Places cursor at current cursor address

//-----------------------------------------------------------------------------
// FETCHC
// Address  : #0114
// Function : Gets current cursor addresses mask pattern
// Output   : HL - Cursor address
//            A  - Mask pattern

//-----------------------------------------------------------------------------
// STOREC
// Address  : #0117
// Function : Record current cursor addresses mask pattern
// Input    : HL - Cursor address
//            A  - Mask pattern

//-----------------------------------------------------------------------------
// SETATR
// Address  : #011A
// Function : Set attribute byte

//-----------------------------------------------------------------------------
// READC
// Address  : #011D
// Function : Reads attribute byte of current screen pixel

//-----------------------------------------------------------------------------
// SETC
// Address  : #0120
// Function : Returns current screen pixel of specified attribute byte

//-----------------------------------------------------------------------------
// NSETCX
// Address  : #0123
// Function : Set horizontal screen pixels

//-----------------------------------------------------------------------------
// GTASPC
// Address  : #0126
// Function : Gets screen relations
// Output   : DE, HL
// Registers: DE, HL

//-----------------------------------------------------------------------------
// PNTINI
// Address  : #0129
// Function : Initalises the PAINT instruction

//-----------------------------------------------------------------------------
// SCANR
// Address  : #012C
// Function : Scans screen pixels to the right

//-----------------------------------------------------------------------------
// SCANL
// Address  : #012F
// Function : Scans screen pixels to the left


//=============================================================================
//
// Misc routines
//
//=============================================================================

//-----------------------------------------------------------------------------
// CHGCAP
// Address  : #0132
// Function : Alternates the CAPS lamp status
// Input    : A  - #00 is lamp on
//             not #00 is lamp off
// Registers: AF

//-----------------------------------------------------------------------------
// CHGSND
// Address  : #0135
// Function : Alternates the 1-bit sound port status
// Input    : A  - #00 to turn off
//             not #00 to turn on
// Registers: AF

//-----------------------------------------------------------------------------
// RSLREG
// Address  : #0138
// Function : Reads the primary slot register
// Output   : A  - For the value which was read
//            33221100
//            ││││││└┴─ Page 0 (#0000-#3FFF)
//            ││││└┴─── Page 1 (#4000-#7FFF)
//            ││└┴───── Page 2 (#8000-#BFFF)
//            └┴─────── Page 3 (#C000-#FFFF)
// Registers: A

//-----------------------------------------------------------------------------
// WSLREG
// Address  : #013B
// Function : Writes value to the primary slot register
// Input    : A  - Value to write, see RSLREG

//-----------------------------------------------------------------------------
// RDVDP
// Address  : #013E
// Function : Reads VDP status register
// Output   : A  - Value which was read
// Registers: A

//-----------------------------------------------------------------------------
// SNSMAT
// Address  : #0141
// Function : Returns the value of the specified line from the keyboard matrix
// Input    : A  - For the specified line
// Output   : A  - For data (the bit corresponding to the pressed key will be 0)
// Registers: AF

// inline u8 Bios_GetKeyboardMatrix(u8 line) { return ((u8(*)(u8))R_SNSMAT)(line); }

//-----------------------------------------------------------------------------
// PHYDIO
// Address  : #0144
// Function : Executes I/O for mass-storage media like disks
// Input    : F  - Set carry to write, reset carry to read
//            A  - Drive number (0 = A:, 1 = B:, etc.)
//            B  - Number of sectors
//            C  - Media ID of the disk
//            DE - Begin sector
//            HL - Begin address in memory
// Output   : F  - Carry set on error
//            A  - Error code (only if carry set)
//                 0 = Write protected
//                 2 = Not ready
//                 4 = Data error
//                 6 = Seek error
//                 8 = Record not found
//                 10 = Write error
//                 12 = Bad parameter
//                 14 = Out of memory
//                 16 = Other error
//            B  - Number of sectors actually written or read
// Registers: All
// Remark   : Interrupts may be disabled afterwards. On some hard disk interfaces,
//            when bit 7 of register C is set, a 23-bit addressing scheme is used
//            and bits 0-6 of register C contain bits 23-16 of the sector number.

//-----------------------------------------------------------------------------
// FORMAT
// Address  : #0147
// Function : Initialises mass-storage media like formatting of disks
// Registers: All
// Remark   : In minimum configuration only a HOOK is available

//-----------------------------------------------------------------------------
// ISFLIO
// Address  : #014A
// Function : Tests if I/O to device is taking place
// Output   : A  - #00 if not taking place
//             not #00 if taking place
// Registers: AF

//-----------------------------------------------------------------------------
// OUTDLP
// Address  : #014D
// Function : Printer output
// Input    : A  - Code to print
// Registers: F
// Remark   : Differences with LPTOUT:
//            1. TAB is expanded to spaces
//            2. For non-MSX printers, Hiragana is transformed to katakana
//               and graphic characters are transformed to 1-byte characters
//            3. If failed, device I/O error occurs

//-----------------------------------------------------------------------------
// GETVCP
// Address  : #0150
// Function : Returns pointer to play queue
// Input    : A  - Channel number
// Output   : HL - Pointer
// Registers: AF
// Remark   : Only used to play music in background

//-----------------------------------------------------------------------------
// GETVC2
// Address  : #0153
// Function : Returns pointer to variable in queue number VOICEN (byte at #FB38)
// Input    : L  - Pointer in play buffer
// Output   : HL - Pointer
// Registers: AF

//-----------------------------------------------------------------------------
// KILBUF
// Address  : #0156
// Function : Clear keyboard buffer
// Registers: HL

//-----------------------------------------------------------------------------
// CALBAS
// Address  : #0159
// Function : Executes inter-slot call to the routine in BASIC interpreter
// Input    : IX - For the calling address
// Output   : Depends on the called routine
// Registers: Depends on the called routine
#define CallBASIC(_addr)				\
__asm									\
	ld		ix, _addr					\
	call	R_CALBAS					\
__endasm

//=============================================================================
//
// MSX 2
//
//=============================================================================
#if (MSX_VERSION >= MSX_2)

//-----------------------------------------------------------------------------
// SUBROM
// Address  : #015C
// Function : Calls a routine in SUB-ROM
// Input    : IX - Address of routine in SUB-ROM
// Output   : Depends on the routine
// Registers: Alternative registers, IY
// Remark   : Use of EXTROM or CALSLT is more convenient.
//            In IX an extra value to the routine can be given by first
//            pushing it to the stack.

//-----------------------------------------------------------------------------
// EXTROM
// Address  : #015F
// Function : Calls a routine in SUB-ROM. Most common way
// Input    : IX - Address of routine in SUB-ROM
// Output   : Depends on the routine
// Registers: Alternative registers, IY
// Remark   : Use: LD IX,address
//                 CALL EXTROM

//-----------------------------------------------------------------------------
// CHKSLZ
// Address  : #0162
// Function : Search slots for SUB-ROM
// Registers: All

//-----------------------------------------------------------------------------
// CHKNEW
// Address  : #0165
// Function : Tests screen mode
// Output   : Carry flag set if screenmode = 5, 6, 7 or 8 
// Registers: AF

//-----------------------------------------------------------------------------
// EOL
// Address  : #0168
// Function : Deletes to the end of the line
// Input    : H  - X coordinate of cursor
//            L  - Y coordinate of cursor
// Registers: All

//-----------------------------------------------------------------------------
// BIGFIL
// Address  : #016B
// Function : Same function as FILVRM (with 16-bit VRAM-address).
// Input    : HL - Address
//            BC - Length
//            A  - Data
// Registers: AF,BC

//-----------------------------------------------------------------------------
// NSETRD
// Address  : #016E
// Function : Same function as SETRD (with 16-bit VRAM-address).
// Input    : HL - VRAM address
// Registers: AF

//-----------------------------------------------------------------------------
// NSTWRT
// Address  : #0171
// Function : Same function as SETWRT (with 16-bit VRAM-address).
// Input    : HL - VRAM address
// Registers: AF

//-----------------------------------------------------------------------------
// NRDVRM
// Address  : #0174
// Function : Reads VRAM like in RDVRM (with 16-bit VRAM-address).
// Input    : HL - VRAM address
// Output   : A  - Read value
// Registers: F

//-----------------------------------------------------------------------------
// NWRVRM
// Address  : #0177
// Function : Writes to VRAM like in WRTVRM (with 16-bit VRAM-address).
// Input    : HL - VRAM address
//            A  - Value to write
// Registers: AF

#endif // (MSX_VERSION >= MSX_2)


//=============================================================================
//
// MSX 2+
//
//=============================================================================
#if (MSX_VERSION >= MSX_2P)

//-----------------------------------------------------------------------------
// RDRES
// Address  : #017A
// Function : Read value of I/O port #F4
// Input    : None
// Output   : A = value read
// Registers: AF

//-----------------------------------------------------------------------------
// WRRES
// Address  : #017D
// Function : Write value to I/O port #F4
// Input    : A = value to write
//            When bit 7 is reset it shows the MSX 2+ startup screen on boot,
//            and counts and initialises the RAM.
// Output   : None
// Registers: None

#endif // (MSX_VERSION >= MSX_2P)


//=============================================================================
//
// MSX turbo R
//
//=============================================================================
#if (MSX_VERSION == MSX_TR)

//-----------------------------------------------------------------------------
// CHGCPU
// Address  : #0180
// Function : Changes CPU mode
// Input    : A = LED 0 0 0 0 0 x x
//                 │            0 0 = Z80 (ROM) mode
//                 │            0 1 = R800 ROM  mode
//                 │            1 0 = R800 DRAM mode
//                LED indicates whether the Turbo LED is switched with the CPU
// Output   : None
// Registers: None

// inline void Bios_SetCPUMode(u8 mode) { ((void(*)(u8))R_CHGCPU)(mode); }

//-----------------------------------------------------------------------------
// GETCPU
// Address  : #0183
// Function : Returns current CPU mode
// Input    : None
// Output   : A = 0 0 0 0 0 0 x x
//                            0 0 = Z80 (ROM) mode
//                            0 1 = R800 ROM  mode
//                            1 0 = R800 DRAM mode
// Registers: AF

// inline u8 Bios_GetCPUMode() { return ((u8(*)(void))R_GETCPU)(); }

//-----------------------------------------------------------------------------
// PCMPLY
// Address  : #0186
// Function : Plays specified memory area through the PCM chip
// Input    : A = v 0 0 0 0 0 x x
//                │           │ │
//                │           └─┴── Quality parameter (Speed: 0 = Fast)
//                └──────────────── VRAM usage flag
//            HL= Start address in RAM or VRAM
//            BC= Length of area to play
//            D = Bit 0 = Bit 17 of area length when using VRAM
//            E = Bit 0 = Bit 17 os start address when using VRAM
// Output   : Carry flag set when aborted with CTRL-STOP
// Registers: All

//-----------------------------------------------------------------------------
// PCMREC
// Address  : #0189
// Function : Records audio using the PCM chip into the specified memory area
// Input    : A = v t t t t c x x
//                │ │ │ │ │ │ │ │
//                │ │ │ │ │ │ └─┴── Quality parameter (Speed: 0 = Fast)
//                │ │ │ │ │ └────── Zero-data compression
//                │ └─┴─┴─┴──────── Treshold
//                └──────────────── VRAM usage flag
//            HL= Start address in RAM or VRAM
//            BC= Length of area to play
//            D = Bit 0 = Bit 17 of area length when using VRAM
//            E = Bit 0 = Bit 17 os start address when using VRAM
// Output   : Carry flag set when aborted with CTRL-STOP
// Registers: All

#endif // (MSX_VERSION == MSX_TR)

























