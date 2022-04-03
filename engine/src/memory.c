// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄                     
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▄███ ▄█▄█ ▄█▀▄ ██▄▀ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀█▄▄ ██ █ ▀█▄▀ ██    ▀██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                            ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#include "memory.h"

extern u16 g_HeapStartAddress;
u16 g_StackAddress;

//-----------------------------------------------------------------------------
/// Get the current address of the stack top (lower address)
u16 Mem_GetStackAddress()
{
	__asm
		ld		(_g_StackAddress), sp
		ld		de, (_g_StackAddress)
	__endasm;
}

//-----------------------------------------------------------------------------
/// Get the current address of the heap top (higher addresse)
u16 Mem_GetHeapAddress()
{
	return g_HeapStartAddress;
}

//-----------------------------------------------------------------------------
/// Get the amount of free space in the heap
u16 Mem_GetHeapSize()
{ 
	return Mem_GetStackAddress() - Mem_GetHeapAddress();
}

//-----------------------------------------------------------------------------
/// Allocate a part of the heap
void* Mem_HeapAlloc(u16 size)
{ 
	u16 addr = g_HeapStartAddress;
	g_HeapStartAddress += size;
	return (void*)addr;
}

//-----------------------------------------------------------------------------
/// Free the last allocated area of the heap
void Mem_HeapFree(u16 size)
{
	g_HeapStartAddress -= size;	
}

//-----------------------------------------------------------------------------
/// Copy a memory block from a source address to an other
void Mem_Copy(const void* src, void* dest, u16 size)
{
	src;	// HL
	dest;	// DE
	size;	// IY+5 IY+4
	__asm
		// Get parameters
		ld		iy, #2
		add		iy, sp
		ld		c, 4-4(iy)
		ld		b, 5-4(iy)
#if 1			
		// Skip if size == 0
		ld		a, b
		or		a, c
		ret		z
		// Do copy
		ldir
	_copy_end:

#else // Fast LDIR (with 16x unrolled LDI) ; only work on RAM
// Up to 19% faster alternative for large LDIRs (break-even at 21 loops)
// hl = source (“home location”)
// de = destination
// bc = byte count
	FastLDIR:
		xor		a
		sub		c
		and		#(16 - 1)
		add		a, a
		di
		ld		(FastLDIR_jumpOffset), a
		ei
		jr nz,$  ; self modifying code
	FastLDIR_jumpOffset: equ $ - 1
	FastLDIR_Loop:
		ldi  ; 16x LDI
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		jp		pe, FastLDIR_Loop
		ret
#endif

	__endasm;
}

//-----------------------------------------------------------------------------
/// Fill a memory block with a given value
void Mem_Set(u8 val, void* dest, u16 size)
{
	val;	// A
	dest;	// DE
	size;	// iy+5 iy+4
	__asm
		// Get parameters
		ld		h, d
		ld		l, e
		ld		e, a
		ld		iy, #2
		add		iy, sp
		ld		c, 4-4(iy)
		ld		b, 5-4(iy)
		// Skip if size == 0
		ld		a, b
		or		c
		ret		z
		// Set first parameter
		ld		(hl), e
		// Set DE to propagate change
		ld		e, l
		ld		d, h
		inc		de
		dec		bc
		// Skip if size == 0
		ld		a, b
		or		c
		ret		z
		// Do fill
		ldir
	__endasm;
}