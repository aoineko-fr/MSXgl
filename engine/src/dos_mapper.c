// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄   ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██ █ ▀█▄  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▀█▄▀ ▄▄█▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// MSX-DOS 2 Memory Mapper extension BIOS routines
//
// References:
// - http://map.grauw.nl/resources/dos2_environment.php
// - https://www.msx.org/wiki/Extended_Bios#004h_.284.29_-_Memory_Mapper_.28MSX-DOS_2.29
//─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "extbios.h"
#include "dos_mapper.h"

//=============================================================================
// MEMORY DATA
//=============================================================================

// Memory Mappers variable table
DOS_VarTable* g_DOS_VarTable = NULL;

// Start address of jump table
u16 g_DOS_JumpTable = NULL;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize DOS extended BIOS
bool DOSMapper_Init()
{
__asm
	push	ix
	call	init_extbios
	pop		ix
	ret

init_extbios:
	ld		a, (M_HOKVLD)
	and		#0x01
	ret		z						// Extended BIOS not installed. Return FALSE

	xor		a
	ld		d, #EXTBIOS_MEMMAPPER	// DOS Mapper device
	ld		e, #DOS_FUNC_GET		// Device function 1
	call	M_EXTBIO
	or		a
	ret		z						// Mapper variable routines are not available. Return FALSE
	ld		(_g_DOS_VarTable), hl

	xor		a
	ld		d, #EXTBIOS_MEMMAPPER	// DOS Mapper device
	ld		e, #DOS_FUNC_CALL		// Device function 2
	call	M_EXTBIO
	or		a
	ret		z						// Mapper support routines are not available. Return FALSE
	ld		(_g_DOS_JumpTable), hl

	ld		a, #0xFF				// Return TRUE
__endasm;
	// return A
}

//-----------------------------------------------------------------------------
// Allocate a segment
bool DOSMapper_Alloc(u8 type, u8 slot, DOS_Segment* seg) // Stack: 4 bytes
{
	type;	// A
	slot;	// L
	seg;	// SP+2
__asm
	push	ix						// Backup the frame pointer
	ld		b, l
	ld		hl, (_g_DOS_JumpTable)
	// ld		de, #DOS_ALL_SEG
	// add		hl, de
	call	___sdcc_call_hl			// Call ALL_SEG
	pop		ix						// Restore the frame pointer
	jr		c, alloc_error

	pop		iy						// Return address
	pop		hl						// Segment sctruct address
	ld		(hl), a					// Store segment number
	inc		hl
	ld		a, b
	ld		(hl), a					// Store slot number
	ld		a, #0xFF				// Return TRUE
	jp		(iy)

alloc_error:
	xor		a						// Return FALSE
__endasm;
	// return A
}

//-----------------------------------------------------------------------------
// Free a segment
bool DOSMapper_Free(u8 seg, u8 slot)
{
	seg;	// A
	slot;	// L
__asm
	push	ix						// Backup the frame pointer
	ld		b, l
	ld		hl, (_g_DOS_JumpTable)
	ld		de, #DOS_FRE_SEG
	add		hl, de
	call	___sdcc_call_hl			// Call FRE_SEG
	ld		a, #0
	adc		#0xFF
	pop		ix						// Restore the frame pointer
__endasm;
	// return A
}

//-----------------------------------------------------------------------------
// Select a segment on the corresponding memory page at the specified address
void DOSMapper_SetPage(u8 page, u8 seg)
{
	page;	// A
	seg;	// L
__asm
	rrca
	rrca
	ld		h, a
	ld		a, l
	// ld		l, #0 // (seem not needed)
	ld		iy, (_g_DOS_JumpTable)
	ld		de, #DOS_PUT_PH
	add		iy, de
	call	___sdcc_call_iy			// Call PUT_PH
__endasm;
}

//-----------------------------------------------------------------------------
// Select a segment on page 0 (0000h~3FFFh)
void DOSMapper_SetPage0(u8 seg)
{
	seg;	// A
__asm
	ld		hl, (_g_DOS_JumpTable)
	ld		de, #DOS_PUT_P0
	add		hl, de
	call	___sdcc_call_hl			// Call PUT_P0
__endasm;
}

//-----------------------------------------------------------------------------
// Select a segment on page 1 (4000h~7FFFh)
void DOSMapper_SetPage1(u8 seg)
{
	seg;	// A
__asm
	ld		hl, (_g_DOS_JumpTable)
	ld		de, #DOS_PUT_P1
	add		hl, de
	call	___sdcc_call_hl			// Call PUT_P1
__endasm;
}

//-----------------------------------------------------------------------------
// Select a segment on page 2 (8000h~BFFFh)
void DOSMapper_SetPage2(u8 seg)
{
	seg;	// A
__asm
	ld		hl, (_g_DOS_JumpTable)
	ld		de, #DOS_PUT_P2
	add		hl, de
	call	___sdcc_call_hl			// Call PUT_P2
__endasm;
}

//-----------------------------------------------------------------------------
// Get the selected segment number on the corresponding memory page at the specified address
u8 DOSMapper_GetPage(u8 page)
{
	page;	// A
__asm
	rrca
	rrca
	ld		h, a
	// ld		l, #0 // (not needed)
	ld		iy, (_g_DOS_JumpTable)
	ld		de, #DOS_GET_PH
	add		iy, de
	call	___sdcc_call_iy			// Call GET_PH
__endasm;
	// return A
}

//-----------------------------------------------------------------------------
// Get the segment number on page 0 (0000h~3FFFh)
u8 DOSMapper_GetPage0()
{
__asm
	ld		hl, (_g_DOS_JumpTable)
	ld		de, #DOS_GET_P0
	add		hl, de
	call	___sdcc_call_hl			// Call GET_P0
__endasm;
	// return A
}

//-----------------------------------------------------------------------------
// Get the segment number on page 1 (4000h~7FFFh)
u8 DOSMapper_GetPage1()
{
__asm
	ld		hl, (_g_DOS_JumpTable)
	ld		de, #DOS_GET_P1
	add		hl, de
	call	___sdcc_call_hl			// Call GET_P1
__endasm;
	// return A
}

//-----------------------------------------------------------------------------
// Get the segment number on page 2 (8000h~BFFFh)
u8 DOSMapper_GetPage2()
{
__asm
	ld		hl, (_g_DOS_JumpTable)
	ld		de, #DOS_GET_P2
	add		hl, de
	call	___sdcc_call_hl			// Call GET_P2
__endasm;
	// return A
}

//-----------------------------------------------------------------------------
// Get the segment number on page 3 (C000h~FFFFh)
u8 DOSMapper_GetPage3()
{
__asm
	ld		hl, (_g_DOS_JumpTable)
	ld		de, #DOS_GET_P3
	add		hl, de
	call	___sdcc_call_hl			// Call GET_P3
__endasm;
	// return A
}
