// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄   ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██ █ ▀█▄  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▀█▄▀ ▄▄█▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// MSX-DOS routines handler 
//
// References:
// - http://map.grauw.nl/resources/dos2_environment.php
//─────────────────────────────────────────────────────────────────────────────
#include "dos.h"
#include "bios.h"

// Backup of the last error value
u8 g_DOS_LastError;

// Backup of the last file info block
DOS_FIB g_DOS_LastFIB;

//=============================================================================
// MSX-DOS 1
//=============================================================================

//-----------------------------------------------------------------------------
// Call a BDOS function
void DOS_Call(u8 func)
{
	func;	// A
__asm
	push	ix
	ld		c, a
	call	BDOS
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// Exit program and return to DOS
void DOS_Exit0()
{
__asm
	ld		c, #DOS_FUNC_TERM0
	jp		BDOS
__endasm;
}

//-----------------------------------------------------------------------------
// Output character
void DOS_CharOutput(c8 chr)
{
	chr;	// A
__asm
	push	ix
	ld		e, a
	ld		c, #DOS_FUNC_CONOUT
	call	BDOS
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// Input character
c8 DOS_CharInput()
{
__asm
	push	ix
	ld		c, #DOS_FUNC_CONIN
	call	BDOS
	pop		ix
	// return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// The characters of the string will be output. The string is terminated by "$" (ASCII 24h).
void DOS_StringOutput(const c8* str)
{
	str;	// HL
__asm
	push	ix
	ex		de, hl
	ld		c, #DOS_FUNC_STROUT
	call	BDOS
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// Open file
//  The unopened FCB must contain a drive which may be zero to indicate the current drive and a filename and extension which may be ambiguous.
//  The current directory of the specified drive will be searched for a matching file and if found it will be opened.
//  Matching entries which are sub-directories or system files will be ignored, and if the filename is ambiguous then the first suitable matching entry will be opened.
i8 DOS_Open(FCB* stream)
{
	stream;	// HL
__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// Open file
	ld		c, #DOS_FUNC_FOPEN
	call	BDOS
	ld		(_g_DOS_LastError), a
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// Create file
i8 DOS_Create(FCB* stream)
{
	stream;	// HL
__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// Create file
	ld		c, #DOS_FUNC_FMAKE
	call	BDOS
	ld		(_g_DOS_LastError), a
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// Close file
i8 DOS_Close(FCB* stream)
{
	stream;	// HL
__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// Close file
	ld		c, #DOS_FUNC_FCLOSE
	call	BDOS
	ld		(_g_DOS_LastError), a
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// Set transfer address
i8 DOS_SetTransferAddr(void* data)
{
	data;	// HL
__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// 
	ld		c, #DOS_FUNC_SETDTA
	call	BDOS
	ld		(_g_DOS_LastError),a
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// Sequential read
i8 DOS_SequentialRead(FCB* stream)
{
	stream;	// HL
__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// Sequential read
	ld		c, #DOS_FUNC_RDSEQ
	call	BDOS
	ld		(_g_DOS_LastError), a
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// Sequential write
i8 DOS_SequentialWrite(FCB* stream)
{
	stream;	// HL
__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// Sequential write
	ld		c, #DOS_FUNC_WRSEQ
	call	BDOS
	ld		(_g_DOS_LastError), a
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// Random block read
u16 DOS_RandomBlockRead(FCB* stream, u16 records)
{
	stream;		// HL
	records;	// DE
__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// Read random block
	ld		c, #DOS_FUNC_RDBLK
	call	BDOS
	ld		(_g_DOS_LastError), a
	ex		de, hl	// DE becomes actual number of records read
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// Random block write
u16 DOS_RandomBlockWrite(FCB* stream, u16 records)
{
	stream;		// HL
	records;	// DE
__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// Write random block
	ld		c, #DOS_FUNC_WRBLK
	call	BDOS
	ld		(_g_DOS_LastError), a
	ex		de, hl	// DE becomes actual number of records written
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// Search the first file matched with wildcard
i8 DOS_FindFirstFile(FCB* stream)
{
	stream; // HL
__asm
	push	ix
	// FCB pointer
	ex		de, hl
	ld		c, #DOS_FUNC_SFIRST
	call	BDOS
	ld		(_g_DOS_LastError), a
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// Search the second and after the second file matched wildcard
i8 DOS_FindNextFile()
{
__asm
	push	ix
	ld		c, #DOS_FUNC_SNEXT
	call	BDOS
	ld		(_g_DOS_LastError), a
	pop		ix
__endasm;
}

//=============================================================================
// MSX-DOS 2
//=============================================================================
#if (TARGET == TARGET_DOS2)

//-----------------------------------------------------------------------------
// Get disk parameters
u8 DOS_GetDiskParam(u8 drv, DOS_DiskParam* param)
{
	drv;	// A
	param;	// DE
__asm
	push	ix
	ld		l, a
	ld		c, #DOS_FUNC_DPARM		// DOS routine
	call	BDOS
	ld		(_g_DOS_LastError), a	// Store last error code
	pop		ix
__endasm;
	// return A
}

//-----------------------------------------------------------------------------
// Find first entry
DOS_FIB* DOS_FindFirstEntry(const c8* filename, u8 attr) __NAKED // Stack: 3 bytes
{
	filename;	// HL
	attr;		// SP[2]
__asm
	pop		iy						// Retreive return address
	// filename
	ld		e, l
	ld		d, h
	// attribute
	dec		sp						// Adjust Stack-pointer
	pop		bc
	// file info block
	push	ix						// Backup frame pointer
	ld		ix, #_g_DOS_LastFIB
	// call
	ld		c, #DOS_FUNC_FFIRST		// DOS routine
	call	BDOS
	// check error
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, find1st_ok
	ld		e, #0
	ld		d, e
	jp		find1st_end
find1st_ok:
	ld		de, #_g_DOS_LastFIB
find1st_end:
	pop		ix						// Restore frame pointer
	jp		(iy)
__endasm;
	// return DE
}

//-----------------------------------------------------------------------------
// Find next entry
DOS_FIB* DOS_FindNextEntry()
{
__asm
	// file info block
	push	ix						// Backup frame pointer
	ld		ix, #_g_DOS_LastFIB
	// call
	ld		c, #DOS_FUNC_FNEXT		// DOS routine
	call	BDOS
	// check error
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, findnxt_ok
	ld		e, #0
	ld		d, e
	jp		findnxt_end
findnxt_ok:
	ld		de, #_g_DOS_LastFIB
findnxt_end:
	pop		ix						// Restore frame pointer
__endasm;
	// return DE
}

//-----------------------------------------------------------------------------
// Open file handle
u8 DOS_FOpen(const c8* path, u8 mode) __NAKED // Stack: 3 bytes
{
	path;	// HL
	mode;	// SP[2]
__asm
	pop		iy						// Retreive return address
	// path
	ld		e, l
	ld		d, h
	// flags
	dec		sp						// Adjust Stack-pointer
	pop		af
	// call
	ld		c, #DOS_FUNC_OPEN		// DOS routine
	call	BDOS
	// check error
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, fopen_ok
	ld		a, #HANDLE_INVALID		// Return invalid file handle
	jp		fopen_end
fopen_ok:
	ld		a, b
fopen_end:
	jp		(iy)
__endasm;
	// return A
}

//-----------------------------------------------------------------------------
// Create file handle
u8 DOS_FCreate(const c8* path, u8 mode, u8 attr) __NAKED // Stack: 4 bytes
{
	path;	// LH
	mode;	// SP[2]
	attr;	// SP[3]
__asm
	pop		iy						// Retreive return address
	// path
	ld		e, l
	ld		d, h
	// attrib
	pop		bc
	// flags
	ld		a, c
	// call
	ld		c, #DOS_FUNC_CREATE		// DOS routine
	call	BDOS
	// check error
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, fcreat_ok
	ld		a, #HANDLE_INVALID		// Return invalid file handle
	jp		fcreat_end
fcreat_ok:
	ld		a, b
fcreat_end:
	jp		(iy)
__endasm;
	// return A
}

//-----------------------------------------------------------------------------
// Close file handle
u8 DOS_FClose(u8 file)
{
	file; // A
__asm
	ld		b, a					// File handle
	ld		c, #DOS_FUNC_CLOSE		// DOS routine
	call	BDOS
	ld		(_g_DOS_LastError), a	// Store last error code
__endasm;
	// return A
}

//-----------------------------------------------------------------------------
// Ensure file handle
u8 DOS_FEnsure(u8 file)
{
	file; // A
__asm
	ld		b, a					// File handle
	ld		c, #DOS_FUNC_ENSURE		// DOS routine
	call	BDOS
	ld		(_g_DOS_LastError), a	// Store last error code
__endasm;
	// return A
}

//-----------------------------------------------------------------------------
// Duplicate file handle
u8 DOS_FDuplicate(u8 file)
{
	file; // A
__asm
	ld		b, a					// File handle
	ld		c, #DOS_FUNC_DUP		// DOS routine
	call	BDOS
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, fdupl_ok
	ld		a, #HANDLE_INVALID		// Return invalid file handle
	jp		fdupl_end
fdupl_ok:
	ld		a, b
fdupl_end:
__endasm;
	// return A
}

//-----------------------------------------------------------------------------
// Read from file handle
u16 DOS_FRead(u8 file, void* buffer, u16 size) __NAKED // Stack: 4 bytes
{
	file;	// A
	buffer;	// DE
	size;	// SP[2:3]
__asm
	pop		iy						// Retreive return address
	// handle
	ld		b, a
	// buffer
	//		Already in DE
	// bytes
	pop		hl
	// call
	ld		c, #DOS_FUNC_READ				// DOS routine
	call	BDOS
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, fread_end
	ld		e, #0xFF
	ld		d, e
fread_end:
	jp		(iy)
__endasm;
	// return DE
}

//-----------------------------------------------------------------------------
// Write to file handle
u16 DOS_FWrite(u8 file, const void* buffer, u16 size) __NAKED // Stack: 4 bytes
{
	file;	// A
	buffer;	// DE
	size;	// SP[2:3]
__asm
	pop		iy						// Retreive return address
	// handle
	ld		b, a
	// buffer
	//		Already in DE
	// bytes
	pop		hl
	// call
	ld		c, #DOS_FUNC_WRITE		// DOS routine
	call	BDOS
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, fwrite_end
	ld		e, #0xFF
	ld		d, e
fwrite_end:
	jp		(iy)
__endasm;
	// return DE
}

//-----------------------------------------------------------------------------
// Move file handle pointer
u32 DOS_FSeek(u8 file, u32 offset, u8 mode) __CALLEE __NAKED // Stack: 7 bytes
{
	file;	// A
	offset;	// SP[2:5]
	mode;	// SP[6]
__asm
	pop		iy						// Retreive return address
	// handle
	ld		b, a
	// offset (32bit)
	pop		hl
	pop		de
	// mode
	dec		sp						// Adjust Stack-pointer
	pop		af
	// call
	ld		c, #DOS_FUNC_SEEK		// DOS routine
	call	BDOS
	// return
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, flseek_end
	ld		e, #0xFF
	ld		d, e
	ld		l, e
	ld		h, e
flseek_end:
	jp		(iy)
__endasm;
}

//-----------------------------------------------------------------------------
// Terminate with error code
void DOS_Exit(u8 err)
{
	err; // A
__asm
	ld		b, a
	ld		c, #DOS_FUNC_TERM
	jp		BDOS
__endasm;
}

//-----------------------------------------------------------------------------
// Explain error code
void DOS_Explain(u8 err, c8* str)
{
	err; // A
	str; // DE
__asm
	push	ix
	ld		b, a
	ld		c, #DOS_FUNC_EXPLAIN
	call	BDOS
	pop		ix
__endasm;
}

#if (DOS_USE_UTILITIES)

//-----------------------------------------------------------------------------
// Delete file or subdirectory
u8 DOS_Delete(const c8* path)
{
	path; // HL
__asm
	ex		de, hl
	ld		c, #DOS_FUNC_DELETE		// DOS routine
	call	BDOS
	ld		(_g_DOS_LastError), a	// Store last error code
__endasm;
	// return A
}

//-----------------------------------------------------------------------------
// Rename file or subdirectory
u8 DOS_Rename(const c8* path, const c8* newPath)
{
	path;		// HL
	newPath;	// DE
__asm
	ex		de, hl
	ld		c, #DOS_FUNC_RENAME		// DOS routine
	call	BDOS
	ld		(_g_DOS_LastError), a	// Store last error code
__endasm;
	// return A
}

//-----------------------------------------------------------------------------
// Move file or subdirectory
u8 DOS_Move(const c8* path, const c8* newPath)
{
	path;		// HL
	newPath;	// DE
__asm
	ex		de, hl
	ld		c, #DOS_FUNC_MOVE		// DOS routine
	call	BDOS
	ld		(_g_DOS_LastError), a	// Store last error code
__endasm;
	// return A
}

#endif // (DOS_USE_UTILITIES)

#endif // (TARGET == TARGET_DOS2)

//-----------------------------------------------------------------------------
// Get MSX-DOS version number
//
// Parameters:    C = 6FH (_DOSVER)
// Results:       A = Error (always zero)
//               BC = MSX-DOS kernel version
//               DE = MSXDOS2.SYS version number
u8 DOS_GetVersion(DOS_Version* ver)
{
	ver; // HL
__asm
	push	ix
	push	hl
	ld		c, #DOS_FUNC_DOSVER
	call	BDOS
	pop		hl
	pop		ix
	or		a
	ret		nz
#if (TARGET == TARGET_DOS2)
	ld		(hl), c
	inc		hl
	ld		(hl), b
	inc		hl
	ld		(hl), e
	inc		hl
	ld		(hl), d
#endif
	ld		a, b
__endasm;
	// return A
}
