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

#if ((DOS_USE_VALIDATOR) || (DOS_USE_ERROR_HANDLER))
// Backup of the last error value
u8 g_DOS_LastError;
#endif

#if (DOS_USE_UTILITIES)
// Backup of the last file info block
DOS_FIB g_DOS_LastFIB;

// Current time
DOS_Time g_DOS_Time;
#endif

#if (DOS_USE_ERROR_HANDLER)
// Pointer to the error handler
void* g_DOS_ErrorHandler;

// Stack pointer for the error handler
void* g_DOS_ErrorStack;
#endif

// Macro to call BDOS with stack backup
#if (DOS_USE_ERROR_HANDLER)
	#define BACKUP_STACK			ld (_g_DOS_ErrorStack), sp
#else
	#define BACKUP_STACK
#endif

//=============================================================================
// MSX-DOS 1 FUNCTIONS
//=============================================================================


#if (DOS_USE_BIOSCALL)

//-----------------------------------------------------------------------------
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
u8 DOS_InterSlotRead(u8 slot, u16 addr) __NAKED
{
	slot; // A
	addr; // DE
	__asm
		ex		de, hl
		call	R_RDSLT
		ret
	__endasm;
}

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
void DOS_InterSlotWrite(u8 slot, u16 addr, u8 value)
{
	slot;  // A
	addr;  // DE
	value; // (SP+4)
	__asm
		ld		iy, #4
		add		iy, sp

		ld		l, e
		ld		h, d
		ld		e, 0(iy)	
		call	R_WRSLT
	__endasm;
}

//-----------------------------------------------------------------------------
// Function : Executes inter-slot call.
// Input    : IY - High byte with slot ID, see RDSLT
//            IX - The address that will be called
// Remark   : Can be call directly from MSX-DOS
//            Variables can never be given in alternative registers or IX and IY
void DOS_InterSlotCall(u8 slot, u16 addr)
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

#endif // (DOS_USE_BIOSCALL)






#if (DOS_USE_ERROR_HANDLER)

//-----------------------------------------------------------------------------
// Error handler
void DOS_ErrorHandler() __NAKED
{
__asm
	ld		a, c					;// Get error code
	or		a, #0x70				;// Add marker so 0 is no error
	ld		(_g_DOS_LastError), a	;// Save the error code
	// ld		c, #DOS1_ERROR_RET_ABORT ;// Return 'Abort' action
	ld		sp, (_g_DOS_ErrorStack)		;// Restore stack pointer
	ret
__endasm;
}


//-----------------------------------------------------------------------------
// Abort error andler
void DOS_AbortHandler() __NAKED
{
__asm
	ld		sp, (_g_DOS_ErrorStack)		;// Restore stack pointer
	ret
__endasm;
}

//-----------------------------------------------------------------------------
// Install the DOS error handler in RAM (page 3).
void DOS_InitErrorHandler()
{
	// Install error handler to memory
	void* ptr = 0xD000;//Mem_HeapAlloc(16);
	Mem_Copy((const void*)DOS_ErrorHandler, ptr, 16);
	g_DOS_ErrorHandler = ptr;
	Poke16(M_DISKVE, (u16)&g_DOS_ErrorHandler);

	// // Install abort handler to memory
	// ptr = 0xD010;//Mem_HeapAlloc(8);
	// Mem_Copy((const void*)DOS_AbortHandler, ptr, 8);
	// Poke16(0xF1E6, (u16)ptr);
}

#endif // (DOS_USE_ERROR_HANDLER)

//-----------------------------------------------------------------------------
// Call a BDOS function
void DOS_Call(u8 func) __NAKED
{
	func;	// A

__asm
	push	ix
	ld		c, a
	call	BDOS
	pop		ix
	ret
__endasm;
}

//-----------------------------------------------------------------------------
// Exit program and return to DOS
void DOS_Exit0() __NAKED
{
__asm
	ld		c, #DOS_FUNC_TERM0
	jp		BDOS
__endasm;
}

//-----------------------------------------------------------------------------
// Input character
c8 DOS_CharInput() __NAKED
{
__asm
	push	ix
	ld		c, #DOS_FUNC_CONIN
	call	BDOS
	pop		ix
	ret								// return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Output character
void DOS_CharOutput(c8 chr) __NAKED
{
	chr;	// A

__asm
	push	ix
	ld		e, a
	ld		c, #DOS_FUNC_CONOUT
	call	BDOS
	pop		ix
	ret
__endasm;
}

//-----------------------------------------------------------------------------
// The characters of the string will be output. The string is terminated by "$" (ASCII 24h).
void DOS_StringOutput(const c8* str) __NAKED
{
	str;	// HL

__asm
	push	ix
	ex		de, hl
	ld		c, #DOS_FUNC_STROUT
	call	BDOS
	pop		ix
	ret
__endasm;
}

#if (DOS_USE_FCB)

//-----------------------------------------------------------------------------
// Set transfer address
void DOS_SetTransferAddr(void* data) __NAKED
{
	data;	// HL

__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// 
	ld		c, #DOS_FUNC_SETDTA
	call	BDOS
	pop		ix
	ret
__endasm;
}

//-----------------------------------------------------------------------------
// Get free space on disk
u16 DOS_GetFreeSpace(u8 drive) __NAKED
{
	drive;	// HL

__asm
	push	ix
	ld		e, a
	ld		c, #DOS_FUNC_ALLOC
	call	BDOS
	pop		ix
	ex		de, hl
	ret
__endasm;
}

//-----------------------------------------------------------------------------
// Open file
//  The unopened FCB must contain a drive which may be zero to indicate the current drive and a filename and extension which may be ambiguous.
//  The current directory of the specified drive will be searched for a matching file and if found it will be opened.
//  Matching entries which are sub-directories or system files will be ignored, and if the filename is ambiguous then the first suitable matching entry will be opened.
u8 DOS_OpenFCB(DOS_FCB* stream) __NAKED
{
	stream;	// HL

__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// Open file
	ld		c, #DOS_FUNC_FOPEN
	BACKUP_STACK
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a
#endif
	pop		ix
	ret								// return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Create file
u8 DOS_CreateFCB(DOS_FCB* stream) __NAKED
{
	stream;	// HL

__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// Create file
	ld		c, #DOS_FUNC_FMAKE
	BACKUP_STACK
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a
#endif
	pop		ix
	ret								// return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Close file
u8 DOS_CloseFCB(DOS_FCB* stream) __NAKED
{
	stream;	// HL

__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// Close file
	ld		c, #DOS_FUNC_FCLOSE
	BACKUP_STACK
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a
#endif
	pop		ix
	ret								// return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Delete file
u8 DOS_DeleteFCB(DOS_FCB* stream) __NAKED
{
	stream;	// HL

__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// Sequential read
	ld		c, #DOS_FUNC_FDEL
	BACKUP_STACK
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a
#endif
	pop		ix
	ret								// return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Sequential read
u8 DOS_SequentialReadFCB(DOS_FCB* stream) __NAKED
{
	stream;	// HL

__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// Sequential read
	ld		c, #DOS_FUNC_RDSEQ
	BACKUP_STACK
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a
#endif
	pop		ix
	ret								// return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Sequential write
u8 DOS_SequentialWriteFCB(DOS_FCB* stream) __NAKED
{
	stream;	// HL

__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// Sequential write
	ld		c, #DOS_FUNC_WRSEQ
	BACKUP_STACK
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a
#endif
	pop		ix
	ret								// return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Random block write
u8 DOS_RandomBlockWriteFCB(DOS_FCB* stream, u16 records) __NAKED
{
	stream;		// HL
	records;	// DE

__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// Write random block
	ld		c, #DOS_FUNC_WRBLK
	BACKUP_STACK
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a
#endif
	pop		ix
	ret								// return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Random block read
u16 DOS_RandomBlockReadFCB(DOS_FCB* stream, u16 records) __NAKED
{
	stream;		// HL
	records;	// DE

__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// Read random block
	ld		c, #DOS_FUNC_RDBLK
	BACKUP_STACK
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a
#endif
	ex		de, hl					// DE becomes actual number of records read
	pop		ix
	ret								// return value in DE
__endasm;
}

//-----------------------------------------------------------------------------
// Search the first file matched with wildcard
u8 DOS_FindFirstFileFCB(DOS_FCB* stream) __NAKED
{
	stream; // HL

__asm
	push	ix
	// FCB pointer
	ex		de, hl
	ld		c, #DOS_FUNC_SFIRST
	BACKUP_STACK
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a
#endif
	pop		ix
	ret								// return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Search the second and after the second file matched wildcard
u8 DOS_FindNextFileFCB() __NAKED
{
__asm
	push	ix
	ld		c, #DOS_FUNC_SNEXT
	BACKUP_STACK
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a
#endif
	pop		ix
	ret								// return value in A
__endasm;
}

#endif // (DOS_USE_FCB)

//=============================================================================
// MSX-DOS 2 FUNCTIONS
//=============================================================================
#if ((TARGET == TARGET_DOS2) || (TARGET == TARGET_DOS2_MAPPER))

#if (DOS_USE_HANDLE)

//-----------------------------------------------------------------------------
// Open file handle
u8 DOS_OpenHandle(const c8* path, u8 mode) __NAKED // Stack: 3 bytes
{
	path;	// HL
	mode;	// SP[2]

__asm
	pop		iy						// Retreive return address
	// path
	ex		de, hl
	// flags
	dec		sp						// Adjust Stack-pointer
	pop		af
	// call
	ld		c, #DOS_FUNC_OPEN		// DOS routine
	call	BDOS
	// check error
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, fopen_ok
	ld		a, #HANDLE_INVALID		// Return invalid file handle
	jp		fopen_end
#endif
fopen_ok:
	ld		a, b
fopen_end:
	jp		(iy)					// Return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Create file handle
u8 DOS_CreateHandle(const c8* path, u8 mode, u8 attr) __NAKED // Stack: 4 bytes
{
	path;	// LH
	mode;	// SP[2]
	attr;	// SP[3]

__asm
	pop		iy						// Retreive return address
	// path
	ex		de, hl
	// attrib
	pop		bc
	// flags
	ld		a, c
	// call
	ld		c, #DOS_FUNC_CREATE		// DOS routine
	call	BDOS
	// check error
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, fcreat_ok
	ld		a, #HANDLE_INVALID		// Return invalid file handle
	jp		fcreat_end
#endif
fcreat_ok:
	ld		a, b
fcreat_end:
	jp		(iy)					// Return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Close file handle
u8 DOS_CloseHandle(u8 file) __NAKED
{
	file; // A

__asm
	ld		b, a					// File handle
	ld		c, #DOS_FUNC_CLOSE		// DOS routine
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
#endif
	ret								// Return value in A
	__endasm;
}

//-----------------------------------------------------------------------------
// Ensure file handle
u8 DOS_EnsureHandle(u8 file) __NAKED
{
	file; // A

__asm
	ld		b, a					// File handle
	ld		c, #DOS_FUNC_ENSURE		// DOS routine
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
#endif
	ret								// Return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Duplicate file handle
u8 DOS_DuplicateHandle(u8 file) __NAKED
{
	file; // A

__asm
	ld		b, a					// File handle
	ld		c, #DOS_FUNC_DUP		// DOS routine
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, fdupl_ok
	ld		a, #HANDLE_INVALID		// Return invalid file handle
	jp		fdupl_end
#endif
fdupl_ok:
	ld		a, b
fdupl_end:
	ret								// Return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Read from file handle
u16 DOS_ReadHandle(u8 file, void* buffer, u16 size) __NAKED // Stack: 4 bytes
{
	file;	// A
	buffer;	// DE
	size;	// SP[2:3]

__asm
	// Get file handle
	ld		b, a
	// Get buffer address
	//		Already in DE
	// Get bytes count
	pop		iy						// Retreive return address
	pop		hl
	// call
	ld		c, #DOS_FUNC_READ		// DOS routine
	call	BDOS
	ex		de, hl					// Put result in DE
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, fread_end
	ld		e, #0xFF
	ld		d, e
#endif
fread_end:
	jp		(iy)					// return DE
__endasm;
}

//-----------------------------------------------------------------------------
// Write to file handle
u16 DOS_WriteHandle(u8 file, const void* buffer, u16 size) __NAKED // Stack: 4 bytes
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
	ex		de, hl					// Put result in DE
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, fwrite_end
	ld		e, #0xFF
	ld		d, e
#endif
fwrite_end:
	jp		(iy)					// return DE
__endasm;
}

//-----------------------------------------------------------------------------
// Move file handle pointer
u32 DOS_SeekHandle(u8 file, i32 offset, u8 mode) __NAKED __CALLEE // Stack: 7 bytes
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
	ex		de, hl					// DE:HL => HL:DE
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, flseek_end
	ld		e, #0xFF
	ld		d, e
	ld		l, e
	ld		h, e
#endif
flseek_end:
	jp		(iy)					// return HL:DE
__endasm;
}

//-----------------------------------------------------------------------------
// Delete file or subdirectory
u8 DOS_DeleteHandle(u8 file) __NAKED
{
	file;	// A

__asm
	ld		b, a
	ld		c, #DOS_FUNC_HDELETE	// DOS routine
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
#endif
	ret								// Return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Rename file or subdirectory
u8 DOS_RenameHandle(u8 file, const c8* path) __NAKED
{
	file;	// A
	path;	// DE

__asm
	ex		de, hl
	ld		b, a
	ld		c, #DOS_FUNC_HRENAME	// DOS routine
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
#endif
	ret								// Return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Move file or subdirectory
u8 DOS_MoveHandle(u8 file, const c8* path) __NAKED
{
	file;	// A
	path;	// DE

__asm
	ex		de, hl
	ld		b, a
	ld		c, #DOS_FUNC_HMOVE		// DOS routine
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
#endif
	ret								// Return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Set file attributes
u8 DOS_SetAttributeHandle(u8 file, u8 attr) __NAKED
{
	file;	// A
	attr;	// L

__asm
	ld		b, a
	ld		a, #1					// A==1 => Set attributes
	ld		c, #DOS_FUNC_HATTR		// DOS routine
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, fsetattr_ok
	ld		a, #0xFF				// Return invalid attribute
	ret
#endif
fsetattr_ok:
	ld		a, l
	ret								// Return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Get file attributes
u8 DOS_GetAttributeHandle(u8 file) __NAKED
{
	file;	// A

__asm
	ld		b, a
	xor		a						// A==0 => Get attributes
	ld		c, #DOS_FUNC_HATTR		// DOS routine
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, fgetattr_ok
	ld		a, #0xFF				// Return invalid attribute
	ret
#endif
fgetattr_ok:
	ld		a, l
	ret								// Return value in A
__endasm;
}

#endif // (DOS_USE_HANDLE)

//-----------------------------------------------------------------------------
// Get disk parameters
u8 DOS_GetDiskParam(u8 drv, DOS_DiskParam* param) __NAKED
{
	drv;	// A
	param;	// DE

__asm
	push	ix
	ld		l, a
	ld		c, #DOS_FUNC_DPARM		// DOS routine
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
#endif
	pop		ix
	ret								// Return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Terminate with error code
void DOS_Exit(u8 err) __NAKED
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
void DOS_Explain(u8 err, c8* str) __NAKED
{
	err; // A
	str; // DE

__asm
	push	ix
	ld		b, a
	ld		c, #DOS_FUNC_EXPLAIN
	call	BDOS
	pop		ix
	ret
__endasm;
}

#if (DOS_USE_UTILITIES)

//-----------------------------------------------------------------------------
// Find first entry
DOS_FIB* DOS_FindFirstEntry(const c8* filename, u8 attr) __NAKED // Stack: 3 bytes
{
	filename;	// HL
	attr;		// SP[2]

__asm
	pop		iy						// Retreive return address
	// filename
	ex		de, hl
	// attribute
	dec		sp						// Adjust Stack-pointer
	pop		bc
	// file info block
	push	ix						// Backup frame pointer
	ld		ix, #_g_DOS_LastFIB
	// call
	ld		c, #DOS_FUNC_FFIRST		// DOS routine
	call	BDOS
#if (DOS_USE_VALIDATOR)
	// check error
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, find1st_ok
	ld		e, #0
	ld		d, e
	jp		find1st_end
#endif
find1st_ok:
	ld		de, #_g_DOS_LastFIB
find1st_end:
	pop		ix						// Restore frame pointer
	jp		(iy)					// return DE
__endasm;	
}

//-----------------------------------------------------------------------------
// Find next entry
DOS_FIB* DOS_FindNextEntry() __NAKED
{
__asm
	// file info block
	push	ix						// Backup frame pointer
	ld		ix, #_g_DOS_LastFIB
	// call
	ld		c, #DOS_FUNC_FNEXT		// DOS routine
	call	BDOS
	// check error
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
#endif
	or		a
	jp		z, findnxt_ok
	ld		e, #0
	ld		d, e
	jp		findnxt_end
findnxt_ok:
	ld		de, #_g_DOS_LastFIB
findnxt_end:
	pop		ix						// Restore frame pointer
	ret								// return DE
__endasm;
}

//-----------------------------------------------------------------------------
// Delete file or subdirectory
u8 DOS_Delete(const c8* path) __NAKED
{
	path; // HL

__asm
	ex		de, hl
	ld		c, #DOS_FUNC_DELETE		// DOS routine
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
#endif
	ret								// Return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Rename file or subdirectory
u8 DOS_Rename(const c8* path, const c8* newPath) __NAKED
{
	path;		// HL
	newPath;	// DE

__asm
	ex		de, hl
	ld		c, #DOS_FUNC_RENAME		// DOS routine
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
#endif
	ret								// Return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Move file or subdirectory
u8 DOS_Move(const c8* path, const c8* newPath) __NAKED
{
	path;		// HL
	newPath;	// DE

__asm
	ex		de, hl
	ld		c, #DOS_FUNC_MOVE		// DOS routine
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
#endif
	ret								// Return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Set file attributes
u8 DOS_SetAttribute(const c8* path, u8 attr) __NAKED
{
	path;	// HL
	attr;	// SP[2]

__asm
	pop		iy						// Retreive return address
	// path
	ex		de, hl
	// flags
	pop		hl
	dec		sp						// Adjust Stack-pointer
	// call
	ld		a, #1					// A==1 => Set attributes
	ld		c, #DOS_FUNC_ATTR		// DOS routine
	call	BDOS
#if (DOS_USE_VALIDATOR)
	// check error
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, setattr_ok
	ld		a, #0xFF				// Return invalid attribute
	jp		setattr_end
#endif
setattr_ok:
	ld		a, l
setattr_end:
	jp		(iy)					// Return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Get file attributes
u8 DOS_GetAttribute(const c8* path) __NAKED
{
	path;	// HL

__asm
	ex		de, hl
	xor		a						// A==0 => Get attributes
	ld		c, #DOS_FUNC_ATTR		// DOS routine
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
	or		a
	jp		z, getattr_ok
	ld		a, #0xFF				// Return invalid attribute
	ret
#endif
getattr_ok:
	ld		a, l
	ret								// Return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Get current directory
u8 DOS_GetDirectory(u8 drive, const c8* path) __NAKED
{
	drive;	// A
	path;	// DE

__asm
	ld		b, a
	ld		c, #DOS_FUNC_GETCD
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
#endif
	ret								// Return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Change current directory
u8 DOS_ChangeDirectory(const c8* path) __NAKED
{
	path;	// HL

__asm
	ex		de, hl
	ld		c, #DOS_FUNC_CHDIR
	call	BDOS
#if (DOS_USE_VALIDATOR)
	ld		(_g_DOS_LastError), a	// Store last error code
#endif
	ret								// Return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Get current date and time
const DOS_Time* DOS_GetTime() __NAKED
{
__asm
	// Get date
	ld		c, #DOS_FUNC_GDATE
	call	BDOS
	ld		(#_g_DOS_Time+0), hl
	ld		(#_g_DOS_Time+2), de
	ld		(#_g_DOS_Time+4), a
	// Get time
	ld		c, #DOS_FUNC_GTIME
	call	BDOS
	ld		(#_g_DOS_Time+5), hl
	ld		a, d
	ld		(#_g_DOS_Time+7), a
	// return
	ld		de, #_g_DOS_Time
	ret							// return DE
__endasm;
}

#endif // (DOS_USE_UTILITIES)

#endif // ((TARGET == TARGET_DOS2) || (TARGET == TARGET_DOS2_MAPPER))

//-----------------------------------------------------------------------------
// Get MSX-DOS version number
//
// Parameters:    C = 6FH (_DOSVER)
// Results:       A = Error (always zero)
//               BC = MSX-DOS kernel version
//               DE = MSXDOS2.SYS version number
u8 DOS_GetVersion(DOS_Version* ver) __NAKED
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
#if ((TARGET == TARGET_DOS2) || (TARGET == TARGET_DOS2_MAPPER))
	ld		(hl), c
	inc		hl
	ld		(hl), b
	inc		hl
	ld		(hl), e
	inc		hl
	ld		(hl), d
#endif
	ld		a, b
	ret								// Return value in A
__endasm;
}
