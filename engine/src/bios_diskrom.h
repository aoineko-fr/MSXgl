// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄▄▄▄  ▄▄   ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀  ██  ██ █ ▀█▄  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▄██▄ ▀█▄▀ ▄▄█▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// BIOS Disk-ROM routines handler 
//
// References:
// - MSX2 Technical Handbook
// - http://map.grauw.nl/resources/msxbios.php
// - http://www.kameli.net/lt/bdos1var.txt
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// DiskROM RAM Variables
//=============================================================================

// Current drive
#define M_DRIVE		0xF2E1

// Two pairs of limits for the first bytes of Shift-JIS characters
#define M_KANJTABLE	0xF30F
const u16 __at(M_KANJTABLE) g_KANJTABLE[2];

// F323 DISKVE: DEFS 2 
#define M_DISKVE	0xF323
const u16 __at(M_DISKVE) g_DISKVE;

// F325 BREAKV: DEFS 2 
#define M_BREAKV	0xF325
const u16 __at(M_BREAKV) g_BREAKV;

// Slot address of RAM in page 0 (DOS)
#define M_RAMAD0	0xF341
const u8 __at(M_RAMAD0) g_RAMAD0;

// Slot address of RAM in page 1 (DOS)
#define M_RAMAD1	0xF342
const u8 __at(M_RAMAD1) g_RAMAD1;

// Slot address of RAM in page 2 (DOS/BASIC)
#define M_RAMAD2	0xF343
const u8 __at(M_RAMAD2) g_RAMAD2;

// Slot address of RAM in page 3 (DOS/BASIC)
#define M_RAMAD3	0xF344
const u8 __at(M_RAMAD3) g_RAMAD3;

// Total number of drivies
#define M_NMBDRV	0xF347

// Main DiskROM slot address
#define M_MASTER	0xF348
const u8 __at(M_MASTER) g_MASTER;

// List of DRIVE PARAMETERS BLOCK
#define M_DPBLST	0xF355

// Set DISK-BASIC ROM
#define M_SETROM	0xF368

// Set system RAM configuration
#define M_SETRAM	0xF36B

// Inter slot move
#define M_SLTMOV	0xF36E

// F37D BDOS: DEFS 2 ; MSX-Disk BIOS Function Call.
#define M_BDOS		0xF37D
const u8 __at(M_BDOS) g_BDOS;

//=============================================================================
// DiskROM Functions
//=============================================================================

//-----------------------------------------------------------------------------
// MSX-DOS 1

// 00h - Program terminate
// 01h - Console input
// 02h - Console output
// 03h - Auxiliary input
// 04h - Auxiliary output
// 05h - Printer output
// 06h - Direct console I/O
// 07h - Direct console input
// 08h - Console input without echo
// 09h - String output
// 0Ah - Buffered line input
// 0Bh - Console status
// 0Ch - Return version number
// 0Dh - Disk reset
// 0Eh - Select disk
// 0Fh - Open file (FCB)
// 10h - Close file (FCB)
// 11h - Search for first entry (FCB)
// 12h - Search for next entry (FCB)
// 13h - Delete file (FCB)
// 14h - Sequential read (FCB)
// 15h - Sequential write FCB)
// 16h - Create file (FCB)
// 17h - Rename file (FCB)
// 18h - Get login vector
// 19h - Get current drive
// 1Ah - Set disk transfer address
// 1Bh - Get allocation information
// 1Ch -
// ...  } Unused
// 20h - 
// 21h - Random read (FCB)
// 22h - Random write(FCB)
// 23h - Get file size (FCB)
// 24h - Set random record (FCB)
// 25h - Unused
// 26h - Random block write (FCB)
// 27h - Random block read (FCB)
// 28h - Random write with zero fill (FCB)
// 29h - Unused
// 2Ah - Get date
// 2Bh - Set date
// 2Ch - Get time
// 2Dh - Set time
// 2Eh - Set/reset verify flag 
// 2Fh - Absolute sector read
// 30h - Absolute sector write

//-----------------------------------------------------------------------------
// MSX-DOS 2

// 31h - Get disk parameters
// 32h -
// ...  } Unused
// 3Fh - 
// 40h - Find first entry
// 41h - Find next entry
// 42h - Find new entry
// 43h - Open file handle
// 44h - Create file handle
// 45h - Close file handle
// 46h - Ensure file handle
// 47h - Duplicate file handle
// 48h - Read from file handle
// 49h - Write to file handle
// 4Ah - Move file handle pointer 
// 4Bh - I/O control for devices
// 4Ch - Test file handle 
// 4Dh - Delete file or subdirectory
// 4Eh - Rename file or subdirectory
// 4Fh - Move file or subdirectory
// 50h - Get/set file attributes
// 51h - Get/set file date and time
// 52h - Delete file handle
// 53h - Rename file handle
// 54h - Move file handle
// 55h - Get/set file handle attributes
// 56h - Get/set file handle date and time
// 57h - Get disk transfer address
// 58h - Get verify flag setting
// 59h - Get current directory
// 5Ah - Change current directory 
// 5Bh - Parse pathname
// 5Ch - Parse filename
// 5Dh - Check character
// 5Eh - Get whole path string
// 5Fh - Flush disk buffers 
// 60h - Fork a child process
// 61h - Rejoin parent process
// 62h - Terminate with error code
// 63h - Define abort exit routine
// 64h - Define disk error handler routine
// 65h - Get previous error code
// 66h - Explain error code 
// 67h - Format a disk
// 68h - Create or destroy RAM disk
// 69h - Allocate sector buffers
// 6Ah - Logical drive assignment
// 6Bh - Get environment item
// 6Ch - Set environment item
// 6Dh - Find environment item
// 6Eh - Get/set disk check status
// 6Fh - Get MSX-DOS version number
// 70h - Get/set redirection status