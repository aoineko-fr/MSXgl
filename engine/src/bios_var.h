// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄▄▄▄  ▄▄   ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀  ██  ██ █ ▀█▄  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▄██▄ ▀█▄▀ ▄▄█▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// BIOS variables 
//
// References:
// - http://map.grauw.nl/resources/msxsystemvars.php
// - https://www.msx.org/wiki/System_variables_and_work_area
// - Pratiques du MSX2
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//-----------------------------------------------------------------------------
// System RAM Variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Subroutines For Inter-Slot Read/Write/Call

#define M_RDPRIM	0xF380	// 5	Routine that reads from a primary slot
const u8 __at(M_RDPRIM) g_RDPRIM[5];

#define M_WRPRIM	0xF385	// 7	Routine that writes to a primary slot
const u8 __at(M_WRPRIM) g_WRPRIM[7];

#define M_CLPRIM	0xF38C	// 14	Routine that calls a routine in a primary slot
const u8 __at(M_CLPRIM) g_CLPRIM[14];

//-----------------------------------------------------------------------------
// Addresses For Extra Subroutines In Language Machine

#define M_USRTAB	0xF39A	// 2	Address to call with Basic USR0 / USR1 / ... / USR9
const u16 __at(M_USRTAB) g_USRTAB[10];

//-----------------------------------------------------------------------------
// Screen Parameters

#define M_LINL40	0xF3AE	// 1	Width for SCREEN 0 (default 37)
u8 __at(M_LINL40) g_LINL40;

#define M_LINL32	0xF3AF	// 1	Width for SCREEN 1 (default 29)
u8 __at(M_LINL32) g_LINL32;

#define M_LINLEN	0xF3B0	// 1	Width for the current text mode
u8 __at(M_LINLEN) g_LINLEN;

#define M_CRTCNT	0xF3B1	// 1	Number of lines on screen
u8 __at(M_CRTCNT) g_CRTCNT;

#define M_CLMLST	0xF3B2	// 1	Column space. It’s uncertain what this address actually stores
u8 __at(M_CLMLST) g_CLMLST;

#define M_TXTNAM	0xF3B3	// 2	BASE(0) - SCREEN 0 name table
u16 __at(M_TXTNAM) g_TXTNAM;

#define M_TXTCOL	0xF3B5	// 2	BASE(1) - SCREEN 0 color table
u16 __at(M_TXTCOL) g_TXTCOL;

#define M_TXTCGP	0xF3B7	// 2	BASE(2) - SCREEN 0 character pattern table
u16 __at(M_TXTCGP) g_TXTCGP;

#define M_TXTATR	0xF3B9	// 2	BASE(3) - SCREEN 0 Sprite Attribute Table
u16 __at(M_TXTATR) g_TXTATR;

#define M_TXTPAT	0xF3BB	// 2	BASE(4) - SCREEN 0 Sprite Pattern Table
u16 __at(M_TXTPAT) g_TXTPAT;

#define M_T32NAM	0xF3BD	// 2	BASE(5) - SCREEN 1 name table
u16 __at(M_T32NAM) g_T32NAM;

#define M_T32COL	0xF3BF	// 2	BASE(6) - SCREEN 1 color table
u16 __at(M_T32COL) g_T32COL;

#define M_T32CGP	0xF3C1	// 2	BASE(7) - SCREEN 1 character pattern table
u16 __at(M_T32CGP) g_T32CGP;

#define M_T32ATR	0xF3C3	// 2	BASE(8) - SCREEN 1 sprite attribute table
u16 __at(M_T32ATR) g_T32ATR;

#define M_T32PAT	0xF3C5	// 2	BASE(9) - SCREEN 1 sprite pattern table
u16 __at(M_T32PAT) g_T32PAT;

#define M_GRPNAM	0xF3C7	// 2	BASE(10) - SCREEN 2 name table
u16 __at(M_GRPNAM) g_GRPNAM;

#define M_GRPCOL	0xF3C9	// 2	BASE(11) - SCREEN 2 color table
u16 __at(M_GRPCOL) g_GRPCOL;

#define M_GRPCGP	0xF3CB	// 2	BASE(12) - SCREEN 2 character pattern table
u16 __at(M_GRPCGP) g_GRPCGP;

#define M_GRPATR	0xF3CD	// 2	BASE(13) - SCREEN 2 sprite attribute table
u16 __at(M_GRPATR) g_GRPATR;

#define M_GRPPAT	0xF3CF	// 2	BASE(14) - SCREEN 2 sprite pattern table
u16 __at(M_GRPPAT) g_GRPPAT;

#define M_MLTNAM	0xF3D1	// 2	BASE(15) - SCREEN 3 name table
u16 __at(M_MLTNAM) g_MLTNAM;

#define M_MLTCOL	0xF3D3	// 2	BASE(16) - SCREEN 3 color table
u16 __at(M_MLTCOL) g_MLTCOL;

#define M_MLTCGP	0xF3D5	// 2	BASE(17) - SCREEN 3 character pattern table
u16 __at(M_MLTCGP) g_MLTCGP;

#define M_MLTATR	0xF3D7	// 2	BASE(18) - SCREEN 3 sprite attribute table
u16 __at(M_MLTATR) g_MLTATR;

#define M_MLTPAT	0xF3D9	// 2	BASE(19) - SCREEN 3 sprite pattern table
u16 __at(M_MLTPAT) g_MLTPAT;

//-----------------------------------------------------------------------------
// Cursor and function Key Parameters

#define M_CLIKSW	0xF3DB	// 1	Key click switch. (0 = Disabled / 1 = Enabled)
u8 __at(M_CLIKSW) g_CLIKSW;

#define M_CSRY	0xF3DC	// 1	Current row-position of the cursor
u8 __at(M_CSRY) g_CSRY;

#define M_CSRX	0xF3DD	// 1	Current column-position of the cursor
u8 __at(M_CSRX) g_CSRX;

#define M_CNSDFG	0xF3DE	// 1	=0 when function keys are not displayed
// =1 when function keys are displayed
const u8 __at(M_CNSDFG) g_CNSDFG;

//-----------------------------------------------------------------------------
// Area To Save The Last Writting Value In VDP Registers

#define M_RG0SAV	0xF3DF	// 1	Mirror of VDP register 0 (Basic: VDP(0))
const u8 __at(M_RG0SAV) g_RG0SAV;

#define M_RG1SAV	0xF3E0	// 1	Mirror of VDP register 1 (Basic: VDP(1))
const u8 __at(M_RG1SAV) g_RG1SAV;

#define M_RG2SAV	0xF3E1	// 1	Mirror of VDP register 2 (Basic: VDP(2))
const u8 __at(M_RG2SAV) g_RG2SAV;

#define M_RG3SAV	0xF3E2	// 1	Mirror of VDP register 3 (Basic: VDP(3))
const u8 __at(M_RG3SAV) g_RG3SAV;

#define M_RG4SAV	0xF3E3	// 1	Mirror of VDP register 4 (Basic: VDP(4))
const u8 __at(M_RG4SAV) g_RG4SAV;

#define M_RG5SAV	0xF3E4	// 1	Mirror of VDP register 5 (Basic: VDP(5))
const u8 __at(M_RG5SAV) g_RG5SAV;

#define M_RG6SAV	0xF3E5	// 1	Mirror of VDP register 6 (Basic: VDP(6))
const u8 __at(M_RG6SAV) g_RG6SAV;

#define M_RG7SAV	0xF3E6	// 1	Mirror of VDP register 7 (Basic: VDP(7))
const u8 __at(M_RG7SAV) g_RG7SAV;

#define M_STATFL	0xF3E7	// 1	Mirror of VDP(8) status register (S#0)
const u8 __at(M_STATFL) g_STATFL;

//-----------------------------------------------------------------------------
// Space bar / Triggers

#define M_TRGFLG	0xF3E8	// 1	Information about trigger buttons and space bar state
// 7 6 5 4 3 2 1 0
// │ │ │ │       └── Space bar, trig(0) (0 = pressed)
// │ │ │ └────────── Stick 1, Trigger 1 (0 = pressed)
// │ │ └──────────── Stick 1, Trigger 2 (0 = pressed)
// │ └────────────── Stick 2, Trigger 1 (0 = pressed)
// └──────────────── Stick 2, Trigger 2 (0 = pressed)
const u8 __at(M_TRGFLG) g_TRGFLG;

//-----------------------------------------------------------------------------
// Color and drawing parameters

#define M_FORCLR	0xF3E9	// Foreground color
u8 __at(M_FORCLR) g_FORCLR;

#define M_BAKCLR	0xF3EA	// Background color
u8 __at(M_BAKCLR) g_BAKCLR;

#define M_BDRCLR	0xF3EB	// Border color
u8 __at(M_BDRCLR) g_BDRCLR;

#define M_MAXUPD	0xF3EC	// Jump instruction used by Basic LINE command. The routines used are: RIGHTC, LEFTC, UPC and DOWNC
const u8 __at(M_MAXUPD) g_MAXUPD[3];

#define M_MINUPD	0xF3EF	// Jump instruction used by Basic LINE command. The routines used are: RIGHTC, LEFTC, UPC and DOWNC
const u8 __at(M_MINUPD) g_MINUPD[3];

#define M_ATRBYT	0xF3F2	// Attribute byte (for graphical routines it’s used to read the color)
u8 __at(M_ATRBYT) g_ATRBYT;

//-----------------------------------------------------------------------------
// MML

#define M_QUEUES	0xF3F3	// Queue table address, see QUETAB (initial value)
const u16 __at(M_QUEUES) g_QUEUES;

#define M_FRCNEW	0xF3F5	// CLOAD flag / =0 when CLOAD / =255 when CLOAD?
const u8 __at(M_FRCNEW) g_FRCNEW;

//-----------------------------------------------------------------------------
// Key Input

#define M_SCNCNT	0xF3F6	// Key scan timing
// When it’s zero, the key scan routine will scan for pressed keys so characters can be written to the keyboard buffer
const u8 __at(M_SCNCNT) g_SCNCNT;

#define M_REPCNT	0xF3F7	// This is the key repeat delay counter
// When it reaches zero, the key will repeat
const u8 __at(M_REPCNT) g_REPCNT;

#define M_PUTPNT	0xF3F8	// Address in the keyboard buffer where a character will be written
const u16 __at(M_PUTPNT) g_PUTPNT;

#define M_GETPNT	0xF3FA	// Address in the keyboard buffer where the next character is read
const u16 __at(M_GETPNT) g_GETPNT;

//-----------------------------------------------------------------------------
// Parameters For Cassette

#define M_CS120	0xF3FC	// Cassette I/O parameters to use for 1200 baud
typedef struct tagCassetteIO
{
	u8 low0;	// LOW signal length of bit 0. (53h by default)
	u8 high0;	// HIGH signal length of bit 0. (5Ch by default)
	u8 low1;	// Length of LOW signal of bit 1. (26h by default)
	u8 high1;	// Length of HIGH signal of bit 1. (2Dh by default)
	u8 header;	// Length of the header signal calculated by HEADLEN * 2/256
} CassetteIO;
const CassetteIO __at(M_CS120) g_CS120;

#define M_CS240	0xF401	// Cassette I/O parameters to use for 2400 baud
const CassetteIO __at(M_CS240) g_CS240;

#define M_LOW		0xF406	// Signal delay when writing a 0 to tape
// LOW+0 = LOW signal length which represents bit 0 at the current transmission rate. (53h by default)
// LOW+1 = HIGH signal length that represents bit 0 at the current transmission rate.(5Ch by default)
const u8 __at(M_LOW) g_LOW[2];

#define M_HIGH	0xF408	// Signal delay when writing a 1 to tape
// HIGH+0 = Length of the LOW signal that represents bit 1 at the current transmission rate. (26h by default)
// HIGH+1 = Length of the HIGH signal that represents bit 1 at the current transmission rate. (2Dh by default)
const u8 __at(M_HIGH) g_HIGH[2];

#define M_HEADER	0xF40A	// Delay of tape header (sync.) block
// Length of the current header signal calculated by HEADLEN * 2/256 or HEADLEN * 4/256
const u8 __at(M_HEADER) g_HEADER;

//-----------------------------------------------------------------------------
// Aspect Ratio settings For CIRCLE

#define M_ASPCT1	0xF40B	// Horizontal / Vertical aspect for CIRCLE command
const u16 __at(M_ASPCT1) g_ASPCT1;

#define M_ASPCT2	0xF40D	// Horizontal / Vertical aspect for CIRCLE command
const u16 __at(M_ASPCT2) g_ASPCT2;

//-----------------------------------------------------------------------------
// Basic Interpreter

#define M_ENDPRG	0xF40F	// 5	Dummy program end for instructions RESUME ... NEXT. (contains DB ":",0,0,0,0 by default)
#define M_ERRFLG	0xF414	// 1	Number of the last error occurred in BASIC.
#define M_LPTPOS	0xF415	// 1	Position of printer head.
#define M_PRTFLG	0xF416	// 1	Flag whether to send to printer. (0 = Print to screen / other = Print to printer)
#define M_NTMSXP	0xF417	// 1	0 If MSX printer (This converts Hiragana to Katakana on Japanese MSX)
#define M_RAWPRT	0xF418	// 1	0 to convert TAB's and unknown characters to spaces.
#define M_VLZADR	0xF419	// 2	Address of character replaced by VAL.
#define M_VLZDAT	0xF41B	// 1	Character replaced with 0 by VAL.
#define M_CURLIN	0xF41C	// 2	Current execution line number of Basic.
#define M_KBFMIN	0xF41E	// 1	Used if direct statement error occures.
#define M_KBUF		0xF41F	// 318	Crunch buffer; translated into intermediate language from BUF.
#define M_BUFMIN	0xF55D	// 1	Used by INPUT routine.
#define M_BUF		0xF55E	// 258	Buffer to store characters displayed to screen under Basic in direct mode.
#define M_ENDBUF	0xF660	// 1	Flag to prevent overflow of BUF.
#define M_TTYPOS	0xF661	// 1	Virtual cursor location internally retained by BASIC.
#define M_DIMFLG	0xF662	// 1	DIM instruction flag.
#define M_VALTYP	0xF663	// 1	Contains the code of the type of variable that is currently in DAC
#define M_DORES		0xF664	// 1	Tempory flag for the BASIC interpreter that indicates if stored keyword can be tokenized or not
#define M_OPRTYP	0xF664	// 1	When an operator used with an instruction, its type is stored momentarily here
#define M_DONUM		0xF665	// 1	Flag for the number encoding (IEEE 754-1985)
#define M_CONTXT	0xF666	// 2	Temporary save of the text pointer
#define M_CONSAV	0xF668	// 1	Store token of constant after calling CHRGTR (00010h en Main-ROM)
#define M_CONTYP	0xF669	// 1	Type of stored constant, used by CHRGTR (00010h en Main-ROM)
#define M_CONLO		0xF66A	// 8	Value of stored constant
#define M_MEMSIZ	0xF672	// 2	Highest location in memory used by Basic
#define M_STKTOP	0xF674	// 2	Top location to be used for the stack
#define M_TXTTAB	0xF676	// 2	Start address of the Basic program to load
#define M_TEMPPT	0xF678	// 2	Starting address of unused area of temporary descriptor.
#define M_TEMPST	0xF67A	// 30	Temporary descriptors.
#define M_DSCTMP	0xF698	// 3	Contains the length and position of the "Ok" message of the BASIC. SET PROMPT modifies it.
#define M_FRETOP	0xF69B	// 2	Starting address of unused area of string area.
#define M_TEMP3		0xF69D	// 2	Used for garbage collection or by USR function.
#define M_TEMP8		0xF69F	// 2	Used for garbage collection.
#define M_ENDFOR	0xF6A1	// 2	Next address of the instruction FOR, used by NEXT.
#define M_DATLIN	0xF6A3	// 2	Line number of the instruction DATA read by READ.
#define M_SUBFLG	0xF6A5	// 1	Flag for array for USR fun.
#define M_FLGINP	0xF6A6	// 1	Flag for the instruction INPUT or READ.
#define M_TEMP		0xF6A7	// 2	Location for temporary reservation for st.code
#define M_PTRFLG	0xF6A9	// 1	Pointer line number conversion flag for the Basic interpreter.
#define M_AUTFLG	0xF6AA	// 1	Auto mode flag (0 = Yes / other = No)
#define M_AUTLIN	0xF6AB	// 2	Current line number for auto.
#define M_AUTINC	0xF6AD	// 2	Increment for auto.
#define M_SAVTXT	0xF6AF	// 2	Text pointer for resume.
#define M_SAVSTK	0xF6B1	// 2	Save stack when an error occurs.
#define M_ERRLIN	0xF6B3	// 2	Line number where last error in BASIC.
#define M_DOT		0xF6B5	// 2	Current line number displayed by LIST.
#define M_ERRTXT	0xF6B7	// 2	Pointer to the error message.
#define M_ONELIN	0xF6B9	// 2	Line number to go when error, Used by ON ERROR GOTO.
#define M_ONEFLG	0xF6BB	// 1	Error flag for the instruction ON ERROR GOTO.
#define M_TEMP2		0xF6BC	// 2	Work area for the formula evaluation routine.
#define M_OLDLIN	0xF6BE	// 2	Line number stored after the execution of the instruction STOP or END, or by pressing CTRL+STOP.
#define M_OLDTXT	0xF6C0	// 2	Old text pointer. The pointer is directed to the instruction following the one where the stop occurred.

//-----------------------------------------------------------------------------
// Data and Variables of Basic

#define M_VARTAB	0xF6C2	// 2	Pointer to the beginning of single variables area.
#define M_ARYTAB	0xF6C4	// 2	Pointer to the beginning of array variables area.
#define M_STREND	0xF6C6	// 2	Address of the end of the variable area.
#define M_DATPTR	0xF6C8	// 2	Pointer to next data to read from the instruction DATA. Modified by RESTORE.
#define M_DEFTBL	0xF6CA	// 26	Declaration table of variables defined by the instructions DEFINT, DEFSTR, DEFSNG and DEFDBL of the Basic for each alphabet letter.

//-----------------------------------------------------------------------------
// Function and parameters of Basic

#define M_PRMSTK	0xF6E4	// 2	Previous definition block on the stack. (To clean the waste.)
#define M_PRMLEN	0xF6E6	// 2	Number of bytes used in PARM1.
#define M_PARM1		0xF6E8	// 100	Area 1 for parameters block storage.
#define M_PRMPRV	0xF74C	// 2	Pointer to previous parameter block in PARM1.
#define M_PRMLN2	0xF74E	// 2	Number of bytes used in PARM2.
#define M_PARM2		0xF750	// 100	Area 2 for to store the blocks being created.
#define M_PRMFLG	0xF7B4	// 1	Flag to indicate whether PARM1 was searching.
#define M_ARYTA2	0xF7B5	// 2	End point of search.
#define M_NOFUNS	0xF7B7	// 1	0 if no function active.
#define M_TEMP9		0xF7B8	// 2	Location of temporary storage for garbage collection.
#define M_FUNACT	0xF7BA	// 1	Count of active functions.
#define M_SWPTMP	0xF7BC	// 8	Value of first variable in instruction SWAP.

//-----------------------------------------------------------------------------
// Graphic Tracing

#define M_TRCFLG	0xF7C4	// 1	Tracing flag. 0 = No tracing; Other = Tracing in progress.

//-----------------------------------------------------------------------------
// Math-Pack

#define M_FBUFFR	0xF7C5	// 43	Buffer used by mathematical routines.
#define M_DECTMP	0xF7F0	// 2	Used to transform decimal integer to floating point number.
#define M_DECTM2	0xF7F2	// 2	Used by the divisions.
#define M_DECCNT	0xF7F4	// 2	Used by the divisions.
#define M_DAC		0xF7F6	// 16	Decimal accumulator.
#define M_HOLD8		0xF806	// 48	Work area for decimal multiplications.
#define M_HOLD2		0xF836	// 8	Work area in the execution of numerical operators.
#define M_HOLD		0xF83E	// 8	Work area in the execution of numerical operators.
#define M_ARG		0xF847	// 16	Argument. (Value used to be calculate with DAC.)
#define M_RNDX		0xF857	// 8	Last random number generated.

//-----------------------------------------------------------------------------
// Display

#define M_FNKSTR	0xF87F	// 160	Texts for function keys (16 bytes each).
c8 __at(M_FNKSTR) g_FNKSTR[10][16];

#define M_CGPNT		0xF91F	// 2	Location of the character font used to initialise screen.
#define M_NAMBAS	0xF922	// 2	Current pattern name table address.
#define M_CGPBAS	0xF924	// 2	Current pattern generator table address.
#define M_PATBAS	0xF926	// 2	Current sprite generator table address.
#define M_ATRBAS	0xF928	// 2	Current sprite attribute table address.
#define M_CLOC		0xF92A	// 2	Cursor location.
u16 __at(M_CLOC) g_CLOC;
#define M_CMASK		0xF92C	// 1	Graphic cursor mask (SCREEN 2 to 4) or ordinate (SCREEN 5 to 12).
u8 __at(M_CMASK) g_CMASK;
#define M_MINDEL	0xF92D	// 2	Work area used by instruction LINE of Basic.
#define M_MAXDEL	0xF92F	// 2	End of the work area used by instruction LINE of Basic.

//-----------------------------------------------------------------------------
// Data Area For The instruction CIRCLE Of The Basic

#define M_ASPECT	0xF931	// 2	Aspect ratio of the circle; set by <ratio> of CIRCLE.
#define M_CENCNT	0xF933	// 2	Counter used by CIRCLE.
#define M_CLINEF	0xF935	// 1	Flag to draw line to centre, Used set by CIRCLE.
#define M_CNPNTS	0xF936	// 2	Point to be plottted in a 45° segment, Used set by CIRCLE.
#define M_CPLOTF	0xF938	// 1	Plot polarity flag, Used set by CIRCLE.
#define M_CPCNT		0xF939	// 2	Number of points in 1/8 of circle, Used set by CIRCLE.
#define M_CPCNT8	0xF93B	// 2	Number of points in the circle. Used by CIRCLE.
#define M_CRCSUM	0xF93D	// 2	Cyclic redundancy check sum of the circle. Used by CIRCLE.
#define M_CSTCNT	0xF93F	// 2	Variable to maintain the number of points of the starting angle. Used by the instruction CIRCLE.
#define M_CSCLXY	0xF941	// 1	Scale of X & Y. Used by the instruction CIRCLE.

//-----------------------------------------------------------------------------
// The Instruction PAINT

#define M_CSAVEA	0xF942	// 2	Address of the first pixel of different color, Used by the instruction PAINT.
#define M_CSAVEM	0xF944	// 1	Mask of the first pixel of different color, Used by the instruction PAINT.
#define M_CXOFF		0xF945	// 2	X offset from center, Used by the instruction PAINT.
#define M_CYOFF		0xF947	// 2	Y offset from center, Used by the instruction PAINT.
#define M_LOHMSK	0xF949	// 1	Leftmost position of an LH excursion, Used by the instruction PAINT in Basic.
#define M_LOHDIR	0xF94A	// 1	New painting direction required by an LH excursion, Used by the instruction PAINT in Basic.
#define M_LOHADR	0xF94B	// 2	Leftmost position of an LH, Used by the instruction PAINT in Basic.
#define M_LOHCNT	0xF94D	// 2	Size of an LH excursion, Used by the instruction PAINT in Basic.
#define M_SKPCNT	0xF94F	// 2	Skip count, Used by the instruction PAINT in Basic.
#define M_MOVCNT	0xF951	// 2	Movement count, Used by the instruction PAINT in Basic.
#define M_PDIREC	0xF953	// 1	Direction of the paint, Used by the instruction PAINT in Basic.
#define M_LFPROG	0xF954	// 1	Set to 1 when moving to the left. Used by the instruction PAINT in Basic.
#define M_RTPROG	0xF955	// 1	Set to 1 when moving to the right. Used by the instruction PAINT in Basic.
#define M_MCLTAB	0xF956	// 2	Used by the instructions DRAW & PLAY in Basic.
#define M_MCLFLG	0xF958	// 1	Used by the instructions DRAW & PLAY in Basic.

//-----------------------------------------------------------------------------
// RS-232 and MML buffers

#define M_QUETAB	0xF959	// 24	Queue-table containing queue information. There are four queues in total, three PLAY queues and one RS232 queue, with 6-byte information blocks per queue:
typedef struct tagRS232Queue
{
	u8	head; // Queue head offset (for writing)
	u8	tail; // Queue tail offset (for reading)
	u8	flag; // Flag indicating whether a byte has been returned
	u8	size; // Queue size
	u16	addr; // Queue start address
} RS232Queue;
const RS232Queue __at(M_QUETAB) g_QUETAB[4];

#define M_QUEBAK	0xF971	// 1	Replacement characters table of queues.
// QUEBAK:
	// db 0	; Replacement characters (voice A)
	// db 0	; Replacement characters (voice B)
	// db 0	; Replacement characters (voice C)
	// db 0	; Replacement characters (RS-232C) (MSX1 only)
const u8 __at(M_QUEBAK) g_QUEBAK;

#define M_VOICAQ	0xF975	// 128	Voice A queue of instruction PLAY. (PSG)
// If MSX-Music is present area #F9C0-#F9F8 is used to store copy of YM2413 registers.
const u8 __at(M_VOICAQ) g_VOICAQ[128];

#define M_VOICBQ	0xF9F5	// 128	Voice B queue of instruction PLAY. (PSG)
const u8 __at(M_VOICBQ) g_VOICBQ[128];

#define M_VOICCQ	0xFA75	// 128	Voice C queue of instruction PLAY. (PSG)
const u8 __at(M_VOICCQ) g_VOICCQ[128];

#define M_RS2IQ	0xFAF5	// 64	RC-232C queue. (MSX1 only)
const u8 __at(M_RS2IQ) g_RS2IQ[64];

//-----------------------------------------------------------------------------
// Graphic pages

#define M_DPPAGE	0xFAF5	// 1	Displayed page number. (MSX2~)
// Modified by SETPAGE X.
const u8 __at(M_DPPAGE) g_DPPAGE;

#define M_ACPAGE	0xFAF6	// 1	Destination page number. (MSX2~)
// Modified by SETPAGE ,X.
const u8 __at(M_ACPAGE) g_ACPAGE;

//-----------------------------------------------------------------------------
// System

#define M_AVCSAV	0xFAF7	// 1	Copy of AV control port (#F7) content. (MSX2+~)
const u8 __at(M_AVCSAV) g_AVCSAV;

#define M_EXBRSA	0xFAF8	// 1	SUB-ROM Slot ID. (MSX2~)
const u8 __at(M_EXBRSA) g_EXBRSA;

#define M_CHRCNT	0xFAF9	// 1	Character counter in the buffer, used in KANA-ROM. (MSX2~)
const u8 __at(M_CHRCNT) g_CHRCNT;

#define M_ROMA	0xFAFA	// 2	Area to store KANA character (Japanese MSX2~ only)
const u16 __at(M_ROMA) g_ROMA;

#define M_MODE	0xFAFC	// 1	Flag for screen mode.
// bit 7: 1 = conversion to Katakana; 0 = conversion to Hiragana. (MSX2+~)
// bit 6: 1 if Kanji ROM level 2. (MSX2+~)
// bit 5: 0/1 to draw in RGB / YJK mode SCREEN 10 or 11. (MSX2+~)
// bit 4: 0/1 to limit the Y coordinate to 211/255. (MSX2+~)
// bit 3: 1 to apply the mask in SCREEN 0~3.
// bits 1-2: VRAM size
	   // 00 for 16kB
	   // 01 for 64kB
	   // 10 for 128kB
	   // 11 for 192kB
// bit 0: 1 if the conversion of Romaji to Kana is possible. (MSX2~)
const u8 __at(M_MODE) g_MODE;

#define M_NORUSE	0xFAFD	// 1	Used by KANJI-ROM for rendering KANJI fonts in graphic modes. (MSX2~)
// bit 7 Don't return to textmode
// bit 6 if 1 and F7F4h (DECCNT)=0, read SHIFT status ???
// bit 5 Disable some functinality
// bit 4 Not in use	
// bit 3 color 0 = Transparent
// bit 0-2: Logical operation on kanji font draw.
	  // 0 for PSET
	  // 1 for AND
	  // 2 for OR
	  // 3 for XOR
	  // 4 for NOT
const u8 __at(M_NORUSE) g_NORUSE;

#define M_XSAVE	0xFAFE	// 2	X-coordinate for Mouse/Trackball/Lightpen. (MSX2~)
// XSAVE = X-coordinate
// XSAVE+1 = P0000000 (bit 7 indicates a request to interrupt the optical pen)
const u8 __at(M_XSAVE) g_XSAVE[2];

#define M_YSAVE	0xFB00	// 2	Y-coordinate for Mouse/Trackball/Lightpen. (MSX2~)
// YSAVE = X-coordinate
// YSAVE+1 = *0000000 (bit 7 unused)
const u8 __at(M_YSAVE) g_YSAVE[2];

#define M_LOGOPR	0xFB02	// 1	Logical operation code. (MSX2~)
u8 __at(M_LOGOPR) g_LOGOPR;

//-----------------------------------------------------------------------------
// Data Area Used By RS-232C

#define M_RSTMP	0xFB03	// 1	Temporary data storage for RS232 Driver
const u8 __at(M_RSTMP) g_RSTMP;

#define M_TOCNT	0xFB03	// 1	Counter used by the RS-232C interface.
const u8 __at(M_TOCNT) g_TOCNT;

#define M_RSFCB	0xFB04	// 2	FCB ("File Control Block") address of the RS-232C.
const u16 __at(M_RSFCB) g_RSFCB;

#define M_RSIQLN	0xFB06	// 1	Byte DATA used by the RS-232C interface.
const u8 __at(M_RSIQLN) g_RSIQLN;

#define M_MEXBIh	0xFB07	// 5	Hook called by the RS-232C.
typedef struct tagRS232Hook
{
	u8	rst;		// RST 30h (0F7h)
	u8	slot;		// Slot ID
	u16	address;	// Address
	u8	ret;		// RET (0C9h)
} RS232Hook;
const RS232Hook __at(M_MEXBIh) g_MEXBIh;

#define M_OLDSTT	0xFB0C	// 5	Hook called by the RS-232C.
const RS232Hook __at(M_OLDSTT) g_OLDSTT;

#define M_OLDINT	0xFB0C	// 5	Hook called by the RS-232C.
const RS232Hook __at(M_OLDINT) g_OLDINT;

#define M_DEVNUM	0xFB16	// 1	Byte offset. (RS-232C)
const u8 __at(M_DEVNUM) g_DEVNUM;

#define M_DATCNT	0xFB17	// 3	DATA area. (RS-232C)
// DATCNT = Slot ID
// DATCNT+1 = Address
typedef struct tagRS232Area
{
	u8	slot; // Slot ID
	u16	address;
} RS232Area;
const RS232Area __at(M_DATCNT) g_DATCNT;

#define M_ERRORS	0xFB1A	// 1	RS-232C error code.
const u8 __at(M_ERRORS) g_ERRORS;

#define M_FLAGS	0xFB1B	// 1	RS-232C flags.
const u8 __at(M_FLAGS) g_FLAGS;

#define M_ESTBLS	0xFB1C	// 1	Bit boolean. (RS-232C)
const u8 __at(M_ESTBLS) g_ESTBLS;

#define M_COMMSK	0xFB1D	// 1	RS-232C mask.
const u8 __at(M_COMMSK) g_COMMSK;

#define M_LSTCOM	0xFB1E	// 1	Byte Data. (RS-232C)
const u8 __at(M_LSTCOM) g_LSTCOM;

#define M_LSTMOD	0xFB1F	// 1	Byte Data. (RS-232C)
const u8 __at(M_LSTMOD) g_LSTMOD;

//-----------------------------------------------------------------------------
// Variables For Instruction PLAY

#define M_PRSCNT	0xFB35	// 1	Command counter.
const u8 __at(M_PRSCNT) g_PRSCNT;

#define M_SAVSP	0xFB36	// 2	Stack pointer when instruction PLAY is used.
const u16 __at(M_SAVSP) g_SAVSP;

#define M_VOICEN	0xFB38	// 1	Number of voice played.
const u8 __at(M_VOICEN) g_VOICEN;

#define M_SAVVOL	0xFB39	// 2	The volume is saved here when Pause.
const u16 __at(M_SAVVOL) g_SAVVOL;

#define M_MCLLEN	0xFB3B	// 1	Length of the macro being analyzed.
const u8 __at(M_MCLLEN) g_MCLLEN;

#define M_MCLPTR	0xFB3C	// 2	Address of the macro being analyzed.
const u16 __at(M_MCLPTR) g_MCLPTR;

#define M_QUEUEN	0xFB3E	// 1	Number of the current queue.
const u8 __at(M_QUEUEN) g_QUEUEN;

#define M_MUSICF	0xFB3F	// 1	Music interruption flags.
const u8 __at(M_MUSICF) g_MUSICF;

#define M_PLYCNT	0xFB40	// 1	Number of macro strings in the PLAY queue.
const u8 __at(M_PLYCNT) g_PLYCNT;

#define M_VCBA	0xFB41	// 37	Data for voice A.
const u8 __at(M_VCBA) g_VCBA[37];

#define M_VCBB	0xFB66	// 37	Data for voice B.
const u8 __at(M_VCBB) g_VCBB[37];

#define M_VCBC	0xFB8B	// 37	Data for voice C.
const u8 __at(M_VCBC) g_VCBC[37];

//-----------------------------------------------------------------------------
// Keyboard

#define M_ENSTOP	0xFBB0	// 1	<>0 when it is possible to resume execution of a BASIC program (CTRL+SHIFT+GRPH+KANA/CODE to resume)
const u8 __at(M_ENSTOP) g_ENSTOP;

#define M_BASROM	0xFBB1	// 1	<>0 if basic is in rom. (CTRL+STOP disabled)
const u8 __at(M_BASROM) g_BASROM;

#define M_LINTTB	0xFBB2	// 24	Table of 24 end-of-line flags for each physical line. (used by the Basic interpreter.)
// 0 = The corresponding line contains a program line that continues on the next physical line,
// Other value = The corresponding line contains a program line that ends here.
const u8 __at(M_LINTTB) g_LINTTB[24];

#define M_FSTPOS	0xFBCA	// 2	first position for inlin.
const u16 __at(M_FSTPOS) g_FSTPOS;

#define M_CODSAV	0xFBCC	// 1	ASCII code of character under the cursor
const u8 __at(M_CODSAV) g_CODSAV;

#define M_FNKSWI	0xFBCD	// 1	Indicate which function keys is displayed.
const u8 __at(M_FNKSWI) g_FNKSWI;

#define M_FNKFLG	0xFBCE	// 10	Function key which have subroutine.
const u8 __at(M_FNKFLG) g_FNKFLG[10];

#define M_ONGSBF	0xFBD8	// 1	Global event flag.
const u8 __at(M_ONGSBF) g_ONGSBF;

#define M_CLIKFL	0xFBD9	// 1	Flag to know if the key click has already occurred.
const u8 __at(M_CLIKFL) g_CLIKFL;

#define M_OLDKEY	0xFBDA	// 11	Previous status of each keyboard matrix row.
const u8 __at(M_OLDKEY) g_OLDKEY[11];

#define M_NEWKEY	0xFBE5	// 11	New status of each keyboard matrix row. The status is updated by the KEYINT interrupt routine.
// Bit correspondence for a European Keyboard Layout:
// NEWKEY+0  = 7      6      5      4      3      2      1      0
// NEWKEY+1  = ;      ]      [      \      =      -      9      8
// NEWKEY+2  = B      A    ACCENT   /      .      ,      `      '
// NEWKEY+3  = J      I      H      G      F      E      D      C
// NEWKEY+4  = R      Q      P      O      N      M      L      K
// NEWKEY+5  = Z      Y      X      W      V      U      T      S
// NEWKEY+6  = F3     F2     F1    CODE   CAPS   GRPH   CTRL  SHIFT
// NEWKEY+7  = RET    SEL    BS    STOP   TAB    ESC    F5     F4
// NEWKEY+8  = RIGHT  DOWN   UP    LEFT   DEL    INS    HOME  SPACE
// NEWKEY+9  = 4      3      2      1      0      /      +      *
// NEWKEY+10 = .      ,      -      9      8      7      6      5
// See here for other keyboard layouts.
const u8 __at(M_NEWKEY) g_NEWKEY[11];

#define M_KEYBUF	0xFBF0	// 40	Key code buffer.
const u8 __at(M_KEYBUF) g_KEYBUF[40];

//-----------------------------------------------------------------------------
// Buffers and work areas

#define M_LINWRK	0xFC18	// 40	Work area for screen management.
const u8 __at(M_LINWRK) g_LINWRK[40];

#define M_PATWRK	0xFC40	// 8	Work area for the name-to-form converter.
const u8 __at(M_PATWRK) g_PATWRK[8];

#define M_BOTTOM	0xFC48	// 2	Address of the beginning of the available RAM area.
const u16 __at(M_BOTTOM) g_BOTTOM;

#define M_HIMEM	0xFC4A	// 2	Address of the end of the available RAM area.
const u16 __at(M_HIMEM) g_HIMEM;

//-----------------------------------------------------------------------------
// Interruptions

#define M_TRPTBL	0xFC4C	// 78	Tables for each of the following instructions:
// TRPTBL (3×10 bytes) => ON KEY GOSUB
// TRPTBL+30 (3×1 byte) => ON STOP GOSUB
// TRPTBL+33 (3×1 byte) => ON SPRITE GOSUB
// TRPTBL+48 (3×5 bytes) => ON GOSUB STRIG
// TRPTBL+51 (3×1 byte) => ON INTERVAL GOSUB
// TRPTBL+54 => Reserved
// The first octet serves as an flag. 0 = OFF, 1 = ON, 2 = STOP, 3 = Call in progress, 7 = Call waiting. The other 2 bytes contain the address of the line number of the routine to be called by the GOSUB in the Basic program.
const u8 __at(M_TRPTBL) g_TRPTBL[78];

#define M_RTYCNT	0xFC9A	// 1	Interrupt control.
const u8 __at(M_RTYCNT) g_RTYCNT;

#define M_INTFLG	0xFC9B	// 1	This flag is set if STOP or CTRL+STOP is pressed.
// 0 = Not Pressed
// 3 = CTRL +STOP Pressed
// 4 = STOP Pressed
const u8 __at(M_INTFLG) g_INTFLG;

#define M_PADY	0xFC9C	// 1	Y-coordinate of a connected touch pad. (Until MSX2+)
const u8 __at(M_PADY) g_PADY;

#define M_PADX	0xFC9D	// 1	X-coordinate of a connected touch pad. (Until MSX2+)
const u8 __at(M_PADX) g_PADX;

#define M_JIFFY	0xFC9E	// 2	Contains value of the software clock, each interrupt of the VDP it is increased by 1.
// The contents can be read or changed by the function 'TIME' or instruction 'TIME'.
const u16 __at(M_JIFFY) g_JIFFY;

#define M_INTVAL	0xFCA0	// 2	Contains length of the interval when the ON INTERVAL routine was established.
const u16 __at(M_INTVAL) g_INTVAL;

#define M_INTCNT	0xFCA2	// 2	ON INTERVAL counter (counts backwards).
const u16 __at(M_INTCNT) g_INTCNT;

//-----------------------------------------------------------------------------
// Cassette

#define M_LOWLIM	0xFCA4	// 1	Used by the Cassette system (minimal length of startbit).
const u8 __at(M_LOWLIM) g_LOWLIM;

#define M_WINWID	0xFCA5	// 1	Used by the Cassette system (store the difference between a low-and high-cycle).
const u8 __at(M_WINWID) g_WINWID;

//-----------------------------------------------------------------------------
// Display & System

#define M_GRPHED	0xFCA6	// 1	Heading for the output of graphic characters.
const u8 __at(M_GRPHED) g_GRPHED;

#define M_ESCCNT	0xFCA7	// 1	Escape sequence counter.
const u8 __at(M_ESCCNT) g_ESCCNT;

#define M_INSFLG	0xFCA8	// 1	Insert mode flag.
const u8 __at(M_INSFLG) g_INSFLG;

#define M_CSRSW		0xFCA9	// 1	Cursor display switch
const u8 __at(M_CSRSW) g_CSRSW;

#define M_CSTYLE	0xFCAA	// 1	Cursor style i.e. Used if INS Key is used. ( 0 = Full Cursor / other = Halve Cursor )
const u8 __at(M_CSTYLE) g_CSTYLE;

#define M_CAPST		0xFCAB	// 1	Capital status. ( 0 = Off / other = On )
const u8 __at(M_CAPST) g_CAPST;

#define M_KANAST	0xFCAC	// 1	Kana mode flags for MSX Japanese, Russian key status ou "Dead Key" status.
// 0 = No Dead Keys
// 1 = Dead Key > Accent Grave
// 2 = SHIFT + Dead Key > Accent Egu
// 3 = CODE + Dead Key > Accent Circumflex
// 4 = SHIFT + CODE + Dead Key > Trema
const u8 __at(M_KANAST) g_KANAST;

#define M_KANAMD	0xFCAD	// 1	Flag to know if the keyboard type is "KANA" (0) or "JIS" (other value). (Japanese MSX only)
const u8 __at(M_KANAMD) g_KANAMD;

#define M_FLBMEM	0xFCAE	// 1	0 if loading basic program.
const u8 __at(M_FLBMEM) g_FLBMEM;

#define M_SCRMOD	0xFCAF	// 1	Screen mode.
const u8 __at(M_SCRMOD) g_SCRMOD;

#define M_OLDSCR	0xFCB0	// 1	Old screen mode.
const u8 __at(M_OLDSCR) g_OLDSCR;

#define M_CASPRV	0xFCB1	// 1	Work area for the cassette (until MSX2+)
// On MSX turbo R, bit 0 indicates the status of the Pause LED and bit 7 indicates the status of the R800 LED.
const u8 __at(M_CASPRV) g_CASPRV;

#define M_BRDATR	0xFCB2	// 1	Border color for paint.
const u8 __at(M_BRDATR) g_BRDATR;

#define M_GXPOS		0xFCB3	// 1	X-position of graphic cursor.
const u8 __at(M_GXPOS) g_GXPOS;

#define M_GYPOS		0xFCB5	// 1	Y-position of graphic cursor.
const u8 __at(M_GYPOS) g_GYPOS;

#define M_GRPACX	0xFCB7	// 1	X Graphics Accumulator.
u8 __at(M_GRPACX) g_GRPACX;

#define M_GRPACY	0xFCB9	// 1	Y Graphics Accumulator.
u8 __at(M_GRPACY) g_GRPACY;

#define M_DRWFLG	0xFCBB	// 1	Used by the instruction DRAW (DrawFlag).
// Bit 7 = Draw Line 0 = No / 1 = Yes (,N)
// Bit 6 = Move Cursor 0 = Yes / 1 = Yes (,B)
// Bit 5 - 0 Unused
const u8 __at(M_DRWFLG) g_DRWFLG;

#define M_DRWSCL	0xFCBC	// 1	Used by the instruction DRAW (DrawScaling).
const u8 __at(M_DRWSCL) g_DRWSCL;

#define M_DRWANG	0xFCBD	// 1	Used by the instruction DRAW (DrawAngle).
// 0 = 0° rotation
// 1 = 90° rotation
// 2 = 180° rotation
// 3 = 270° rotation
const u8 __at(M_DRWANG) g_DRWANG;

#define M_MNROM		0xFCC1	// 1
const u8 __at(M_MNROM) g_MNROM;
#define M_EXPTBL	0xFCC1	// 4	Bit 7 of these variables is a flag for each primary slot to indicate if the slot is expanded or not. The other bits are reset.
//                                  The first variable (also called MNROM) indicates the slot ID of the Main-ROM. 
//                                  This implies that the Main-ROM of an MSX is always in primary slot 0, or secondary slot 0-0.
// EXPTBL+0 = Flag for slot 0 (also Slot ID of the Main-ROM!)
// EXPTBL+1 = Flag for slot 1
// EXPTBL+2 = Flag for slot 2
// EXPTBL+3 = Flag for slot 3
const u8 __at(M_EXPTBL) g_EXPTBL[4];

#define M_SLTTBL	0xFCC5	// 4	Saves the state of the 4 secondary slot registers of each extended Primary Slot.
// SLTTBL+0 = Value for slot 0
// SLTTBL+1 = Value for slot 1
// SLTTBL+2 = Value for slot 2
// SLTTBL+3 = Value for slot 3
//  Format:
//   Bits 1-0 = Extended slot on page 0000h~3FFFh
//   Bits 3-2 = Extended slot on page 4000h~7FFFh
//   Bits 5-4 = Extended slot on page 8000h~BFFFh
//   Bits 7-6 = Extended slot on page C000h~FFFFh
const u8 __at(M_SLTTBL) g_SLTTBL[4];

#define M_SLTATR	0xFCC9	// 64	Slot attributes given during MSX boot process.
// Bit 7 = 1 if Basic program, else 0
// Bit 6 = 1 if device extension, else 0
// Bit 5 = 1 if statement extension, else 0
// Bits 4~0 = Unused
const u8 __at(M_SLTATR) g_SLTATR[64];

#define M_SLTWRK	0xFD09	// 128	SLTWRK is a 128-byte variable array used to reserve a RAM work area in Main-RAM for ROM applications. This array consists of 8 bytes per slot (2 per memory page). Each of these 2 octets are provided to place an slot ID with flags on a byte (MSB) or an address on two bytes as follows.
// SLTWRK+0 = Work area for slot 0-0, page 0000h~3FFFh
// SLTWRK+2 = Work area for slot 0-0, page 4000h~7FFFh
// SLTWRK+4 = Work area for slot 0-0, page 8000h~BFFFh
// SLTWRK+6 = Work area for slot 0-0, page C000h~FFFFh
// SLTWRK+8 = Work area for slot 0-1, page 0000h~3FFFh
  // .
  // .
  // .
// SLTWRK+124 = Work area for slot 3-3, page 8000h~BFFFh
// SLTWRK+126 = Work area for slot 3-3, page C000h~FFFFh
// The pointer is used to reserve a work area from 8000h or higher to F37Fh.
// The slot ID is used to reserve a work area on the pages 0000h~3FFFh & 4000h~7FFFh).
// Slot ID format used in table SLTWRK:
// LSB = F RMD APP RES SS1 SS0 PS1 PS0
// MSB = 00h
// PS = Primary slot number
// SS = Secondary slot number
// RES = Reserved
// APP = Set if the RAM used by an application, 0 otherwise
// RMD = Set if the RAM is used by instruction CALL MEMINI, 0 otherwise
// F = Set if secondary slot, 0 if primary slot.
const u8 __at(M_SLTWRK) g_SLTWRK[128];

#define M_PROCNM	0xFD89	// 16	Work aera of the instructions CALL and OPEN. Contents the instruction name or device name.
const u8 __at(M_PROCNM) g_PROCNM[16];

#define M_HOOKS	0xFD9A	// 570	System hooks
// @see bios_hook.h

//-----------------------------------------------------------------------------
// Area Added For MSX2 To Save Each Value Written Into VDP Registers

#define M_RG08SAV	0xFFE7	// 1	System saves the byte written to the register R#08 here, Used by VDP(9) in Basic. (MSX2~)
const u8 __at(M_RG08SAV) g_RG08SAV;

#define M_RG09SAV	0xFFE8	// 1	System saves the byte written to the register R#09 here, Used by VDP(10) in Basic. (MSX2~)
const u8 __at(M_RG09SAV) g_RG09SAV;

#define M_RG10SAV	0xFFE9	// 1	System saves the byte written to the register R#10 here, Used by VDP(11) in Basic. (MSX2~)
const u8 __at(M_RG10SAV) g_RG10SAV;

#define M_RG11SAV	0xFFEA	// 1	System saves the byte written to the register R#11 here, Used by VDP(12) in Basic. (MSX2~)
const u8 __at(M_RG11SAV) g_RG11SAV;

#define M_RG12SAV	0xFFEB	// 1	System saves the byte written to the register R#12 here, Used by VDP(13) in Basic. (MSX2~)
const u8 __at(M_RG12SAV) g_RG12SAV;

#define M_RG13SAV	0xFFEC	// 1	System saves the byte written to the register R#13 here, Used by VDP(14) in Basic. (MSX2~)
const u8 __at(M_RG13SAV) g_RG13SAV;

#define M_RG14SAV	0xFFED	// 1	System saves the byte written to the register R#14 here, Used by VDP(15) in Basic. (MSX2~)
const u8 __at(M_RG14SAV) g_RG14SAV;

#define M_RG15SAV	0xFFEE	// 1	System saves the byte written to the register R#15 here, Used by VDP(16) in Basic. (MSX2~)
const u8 __at(M_RG15SAV) g_RG15SAV;

#define M_RG16SAV	0xFFEF	// 1	System saves the byte written to the register R#16 here, Used by VDP(17) in Basic. (MSX2~)
const u8 __at(M_RG16SAV) g_RG16SAV;

#define M_RG17SAV	0xFFF0	// 1	System saves the byte written to the register R#17 here, Used by VDP(18) in Basic. (MSX2~)
const u8 __at(M_RG17SAV) g_RG17SAV;

#define M_RG18SAV	0xFFF1	// 1	System saves the byte written to the register R#18 here, Used by VDP(19) in Basic. (MSX2~)
const u8 __at(M_RG18SAV) g_RG18SAV;

#define M_RG19SAV	0xFFF2	// 1	System saves the byte written to the register R#19 here, Used by VDP(20) in Basic. (MSX2~)
const u8 __at(M_RG19SAV) g_RG19SAV;

#define M_RG20SAV	0xFFF3	// 1	System saves the byte written to the register R#20 here, Used by VDP(21) in Basic. (MSX2~)
const u8 __at(M_RG20SAV) g_RG20SAV;

#define M_RG21SAV	0xFFF4	// 1	System saves the byte written to the register R#21 here, Used by VDP(22) in Basic. (MSX2~)
const u8 __at(M_RG21SAV) g_RG21SAV;

#define M_RG22SAV	0xFFF5	// 1	System saves the byte written to the register R#22 here, Used by VDP(23) in Basic. (MSX2~)
const u8 __at(M_RG22SAV) g_RG22SAV;

#define M_RG23SAV	0xFFF6	// 1	System saves the byte written to the register R#23 here, Used by VDP(24) in Basic. (MSX2~)
const u8 __at(M_RG23SAV) g_RG23SAV;

#define M_MINROM	0xFFF7	// 1	Slot address of Main-ROM. Use EXPTBL+0 to know slot ID of the internal Main-ROM. (MSX2~)
const u8 __at(M_MINROM) g_MINROM;

#define M_SVFFF8	0xFFF8	// 2	Reserved
const u8 __at(M_SVFFF8) g_SVFFF8[2];

//-----------------------------------------------------------------------------
// Area Added for MSX2+ to save each value written into VDP registers

#define M_RG25SAV	0xFFFA	// 1	System saves the byte written to the register R#25 here, Used by VDP(26) in Basic. (MSX2+~)
const u8 __at(M_RG25SAV) g_RG25SAV;

#define M_RG26SAV	0xFFFB	// 1	System saves the byte written to the register R#26 here, Used by VDP(27) in Basic. (MSX2+~)
const u8 __at(M_RG26SAV) g_RG26SAV;

#define M_RG27SAV	0xFFFC	// 1	System saves the byte written to the register R#27 here, Used by VDP(28) in Basic. (MSX2+~)
const u8 __at(M_RG27SAV) g_RG27SAV;

#define M_SVFFFD	0xFFFD	// 2	Reserved
const u8 __at(M_SVFFFD) g_SVFFFD[2];

//-----------------------------------------------------------------------------
// Secondary slots registers

#define M_SLTSL	0xFFFF	// 1	Access address to selection register of secondary slots. (Available in extended slots only.)
// Format:
//  Bits 1-0 = Extended slot on page 0 (0000h~3FFFh)
//  Bits 3-2 = Extended slot on page 1 (4000h~7FFFh)
//  Bits 5-4 = Extended slot on page 2 (8000h~BFFFh)
//  Bits 7-6 = Extended slot on page 3 (C000h~FFFFh)
// Note: Read value has the INVERTED bits of previous written value.
u8 __at(M_SLTSL) g_SLTSL;