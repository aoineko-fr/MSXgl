// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄                     
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▄███ ▄█▄█ ▄█▀▄ ██▄▀ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀█▄▄ ██ █ ▀█▄▀ ██    ▀██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                            ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#include "memory.h"

extern u16 g_HeapStartAddress;
u16 g_StackAddress;

//-----------------------------------------------------------------------------
// Get the current address of the stack top (lower address)
u16 Mem_GetStackAddress()
{
	__asm
		ld		(_g_StackAddress), sp
		ld		de, (_g_StackAddress)
	__endasm;
}

//-----------------------------------------------------------------------------
// Get the current address of the heap top (higher addresse)
u16 Mem_GetHeapAddress()
{
	return g_HeapStartAddress;
}

//-----------------------------------------------------------------------------
// Get the amount of free space in the heap
u16 Mem_GetHeapSize()
{ 
	return Mem_GetStackAddress() - Mem_GetHeapAddress();
}

//-----------------------------------------------------------------------------
// Allocate a part of the heap
void* Mem_HeapAlloc(u16 size)
{ 
	u16 addr = g_HeapStartAddress;
	g_HeapStartAddress += size;
	return (void*)addr;
}

//-----------------------------------------------------------------------------
// Free the last allocated area of the heap
void Mem_HeapFree(u16 size)
{
	g_HeapStartAddress -= size;	
}

//-----------------------------------------------------------------------------
// Copy a memory block from a source address to an other
void Mem_Copy(const void* src, void* dest, u16 size) __naked
{
	src;	// HL
	dest;	// DE
	size;	// SP+2
	__asm
		// Get parameters
		pop		iy							// 16 cc (return address)
		pop		bc							// 11 cc (retreive size)
#if (MEM_USE_VALIDATOR)
		// Skip if size == 0
		ld		a, b
		or		a, c
		jp		z, mem_copy_end
#endif
		// Do copy
		ldir								// 23/18 cc
	mem_copy_end:
		jp		(iy)						// 10 cc
	__endasm;
}

#if (MEM_USE_FASTCOPY)
//-----------------------------------------------------------------------------
// Fast copy a memory block from a source address to a destination
// This function use a unrolled LDI loop (18,75% faster ; break-even at 6 loops for multiple of 16 size or at 31 loops otherwise)
void Mem_FastCopy(const void* src, void* dest, u16 size) __naked
{
	src;	// HL
	dest;	// DE
	size;	// SP+2
	__asm
		// Get parameters
		pop		iy							// 16 cc (return address)
		pop		bc							// 11 cc (retreive size)
#if (MEM_USE_VALIDATOR)
		// Skip if size == 0
		ld		a, b
		or		a, c
		jp		z, mem_fastcopy_end
#endif
	mem_fastcopy_setup:
		// Setup fast LDIR loop
		xor		a							//  5 cc
		sub		c							//  5 cc
		and		#15							//  8 cc
		jp		z, mem_fastcopy_loop		// 11 cc - total 29 cc (break-even at 16 loops)
		add		a							//  5 cc
		exx									//  5 cc
		add		a, #mem_fastcopy_loop 		//  8 cc
		ld		l, a						//  5 cc
		ld		a, #0						//  8 cc
		adc		a, #mem_fastcopy_loop >> 8	//  8 cc
		ld		h, a						//  5 cc
		push	hl							// 12 cc
		exx									//  5 cc
		ret									// 11 cc - total 101 cc (break-even at 25 loops)
	mem_fastcopy_loop:
		// Fast LDIR (with 16x unrolled LDI)
		.rept 16
		ldi									// 18 cc
		.endm
		jp		pe, mem_fastcopy_loop		// 11 cc (0,6875 cc per ldi)
	mem_fastcopy_end:
		jp		(iy)						// 10 cc
	__endasm;
}
#endif // (MEM_USE_FASTCOPY)

//-----------------------------------------------------------------------------
// Fill a memory block with a given value (minimal size: 2)
void Mem_Set(u8 val, void* dest, u16 size) __naked
{
	val;	// A
	dest;	// DE
	size;	// SP+2
	__asm
		push	de
		pop		hl
		ld		(hl), a
		inc		de
		// Get parameters
		pop		iy							// 16 cc (return address)
		pop		bc							// 11 cc (retreive size)
#if (MEM_USE_VALIDATOR)
		// Skip if size < 2
		ld		a, b
		or		a, c
		jp		z, mem_fill_end
		dec		bc
		jp		z, mem_fill_end
#else
		dec		bc
#endif
		// Do fill
		ldir								// 23/18 cc
	mem_fill_end:
		jp		(iy)						// 10 cc
	__endasm;
}

#if (MEM_USE_FASTSET)
//-----------------------------------------------------------------------------
// Fast fill a memory block with a given value (minimal size of 2 bytes).
void Mem_FastSet(u8 val, void* dest, u16 size) __naked
{
	val;	// A
	dest;	// DE
	size;	// SP+2
	__asm
		push	de
		pop		hl
		ld		(hl), a
		inc		de
		// Get parameters
		pop		iy							// 16 cc (return address)
		pop		bc							// 11 cc (retreive size)
#if (MEM_USE_VALIDATOR)
		// Skip if size < 2
		ld		a, b
		or		a, c
		jp		z, mem_fill_end
		dec		bc
		jp		z, mem_fill_end
#else
		dec		bc
#endif
		// Note: could jump from here to 'mem_fastcopy_setup' label and removing the following code
	mem_fastfill_setup:
		// Setup fast LDIR loop
		xor		a							//  5 cc
		sub		c							//  5 cc
		and		#15							//  8 cc
		jp		z, mem_fastfill_loop		// 11 cc - total 29 cc (break-even at 16 loops)
		add		a							//  5 cc
		exx									//  5 cc
		add		a, #mem_fastfill_loop 		//  8 cc
		ld		l, a						//  5 cc
		ld		a, #0						//  8 cc
		adc		a, #mem_fastfill_loop >> 8	//  8 cc
		ld		h, a						//  5 cc
		push	hl							// 12 cc
		exx									//  5 cc
		ret									// 11 cc - total 101 cc (break-even at 25 loops)
	mem_fastfill_loop:
		// Fast LDIR (with 16x unrolled LDI)
		.rept 16
		ldi									// 18 cc
		.endm
		jp		pe, mem_fastfill_loop		// 11 cc (0,6875 cc per ldi)
	mem_fastfill_end:
		jp		(iy)						// 10 cc
	__endasm;
}
#endif // MEM_USE_FASTSET