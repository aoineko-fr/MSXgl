// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄   ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██ █ ▀█▄  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▀█▄▀ ▄▄█▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// MSX-DOS routines handler 
//
// References:
// - http://map.grauw.nl/resources/dos2_environment.php
//─────────────────────────────────────────────────────────────────────────────
#include "dos.h"

// Backup of the last error value
i8 g_DOS_LastError;
	
//=============================================================================
// MSX-DOS 1
//=============================================================================

//-----------------------------------------------------------------------------
// Open file
//  The unopened FCB must contain a drive which may be zero to indicate the current drive and a filename and extension which may be ambiguous.
//  The current directory of the specified drive will be searched for a matching file and if found it will be opened.
//  Matching entries which are sub-directories or system files will be ignored, and if the filename is ambiguous then the first suitable matching entry will be opened.
i8 DOS_Open(fcb* stream)
{
	stream; // HL
__asm
	push	ix
	// FCB pointer
	ex		de, hl
	// Open file
	ld		c, #DOS_FUNC_FOPEN
	call	BDOS
	ld		(_g_DOS_LastError), a
	pop ix
__endasm;
}

//-----------------------------------------------------------------------------
// Create file
i8 DOS_Create(fcb* stream)
{
	stream; // HL
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
i8 DOS_Close(fcb* stream)
{
	stream; // HL
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
	data; // HL
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
i8 DOS_SequentialRead(fcb* stream)
{
	stream; // HL
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
i8 DOS_SequentialWrite(fcb* stream)
{
	stream; // HL
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
u16 DOS_RandomBlockRead(fcb* stream, u16 records)
{
	stream; // HL
	records: // DE
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
u16 DOS_RandomBlockWrite(fcb* stream, u16 records)
{
	stream; // HL
	records: // DE
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

//=============================================================================
// MSX-DOS 2
//=============================================================================
#if (TARGET == TARGET_DOS2)

//-----------------------------------------------------------------------------
//
i8 DOS2_Open(char *, ui8) __sdcccall(0)
{
__asm
	push iy
	ld iy,#0
	add iy,sp

	// path
	ld e,4(iy)
	ld d,5(iy)
	// flags
	ld a,6(iy)
	// call
	ld c,#0x43
	call BDOS
	// check error
	ld (_last_error),a
	add a,#0
	jp z,_open_ok
	ld l,#-1
	jp _open_end
_open_ok:
	ld l,b
_open_end:

	pop iy
__endasm;
}

//-----------------------------------------------------------------------------
//
i8 DOS2_Create(char *, u8, u8) __sdcccall(0)
{
__asm
	push iy
	ld iy,#0
	add iy,sp

	// path
	ld e,4(iy)
	ld d,5(iy)
	// flags
	ld a,6(iy)
	// attrib
	ld b,7(iy)
	// call
	ld c,#0x44
	call BDOS
	// check error
	ld (_last_error),a
	add a,#0
	jp z,_creat_ok
	ld l,#-1
	jp _creat_end
_creat_ok:
	ld l,b
_creat_end:

	pop iy
__endasm;
}

//-----------------------------------------------------------------------------
//
i8 DOS2_Close(i8) __sdcccall(0)
{
__asm
	push iy
	ld iy,#0
	add iy,sp

	// handle
	ld b,4(iy)
	// call
	ld c,#0x45
	call BDOS
	// return
	ld (_last_error),a
	ld l,a

	pop iy
__endasm;
}

//-----------------------------------------------------------------------------
//
i8 DOS2_Duplicate(i8) __sdcccall(0)
{
__asm
	push iy
	ld iy,#0
	add iy,sp

	// handle
	ld b,4(iy)
	// call
	ld c,#0x47
	call BDOS
	// return
	ld (_last_error),a
	add a,#0
	jp z,_dup_ok
	ld l,#-1
	jp _dup_end
_dup_ok:
	ld l,b
_dup_end:

	pop iy
__endasm;
}

//-----------------------------------------------------------------------------
//
i16 DOS2_Read(i8, void *, i16) __sdcccall(0)
{
__asm
	push iy
	ld iy,#0
	add iy,sp

	// handle
	ld b,4(iy)
	// buffer
	ld e,5(iy)
	ld d,6(iy)
	// bytes
	ld l,7(iy)
	ld h,8(iy)
	// call
	ld c,#0x48
	call BDOS
	// return
	ld (_last_error),a
	add a,#0
	jp z,_read_end
	ld h,#-1
	ld l,#-1
_read_end:

	pop iy
__endasm;
}

//-----------------------------------------------------------------------------
//
i16 DOS2_Write(i8, void *, i16) __sdcccall(0)
{
__asm
	push iy
	ld iy,#0
	add iy,sp

	// handle
	ld b,4(iy)
	// buffer
	ld e,5(iy)
	ld d,6(iy)
	// bytes
	ld l,7(iy)
	ld h,8(iy)
	// call
	ld c,#0x49
	call BDOS
	// return
	ld (_last_error),a
	add a,#0
	jp z,_write_end
	ld h,#-1
	ld l,#-1
_write_end:

	pop iy
__endasm;
}

//-----------------------------------------------------------------------------
//
u32 DOS2_LSeek(i8, u32, u8) __sdcccall(0)
{
__asm
	push iy
	ld iy,#0
	add iy,sp

	// handle
	ld b,4(iy)
	// offset (32bit)
	ld l,5(iy)
	ld h,6(iy)
	ld e,7(iy)
	ld d,8(iy)
	// method
	ld a,9(iy)
	// call
	ld c,#0x4A
	call BDOS
	// return
	ld (_last_error),a
	add a,#0
	jp z,_lseek_end
	ld h,#-1
	ld l,#-1
_lseek_end:

	pop iy
__endasm;
}

//-----------------------------------------------------------------------------
//
void DOS2_Exit(i8) __sdcccall(0)
{
__asm
	push iy
	ld iy,#0
	add iy,sp

	ld b,4(iy)
	ld c,#0x62
	call BDOS

	pop iy
__endasm;
}


#endif
