// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▄███ ▄█▄█ ▄█▀▄ ██▄▀ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀█▄▄ ██ █ ▀█▄▀ ██    ▀██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                            ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Module to handle memory allocation and copy/fill operations
//─────────────────────────────────────────────────────────────────────────────
#include "memory.h"

//=============================================================================
// MEMORY DATA
//=============================================================================

u16 g_StackAddress;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Get the current address of the stack top (lower address).
u16 Mem_GetStackAddress() __NAKED
{
__asm
	ld		(_g_StackAddress), sp
	ld		de, (_g_StackAddress)
	ret
__endasm;
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

#if (!MEM_USE_BUILTIN)
//-----------------------------------------------------------------------------
// Copy a memory block from a source address to an other
void Mem_Copy(const void* src, void* dest, u16 size) __NAKED // Stack: 4 bytes
{
	src;	// HL
	dest;	// DE
	size;	// SP[2]

__asm
	// Get parameters
	pop		iy							// 16 cc (return address)
	pop		bc							// 11 cc (retreive size) | stack ajusted
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
#endif // (!MEM_USE_BUILTIN)

#if (MEM_USE_FASTCOPY)
//-----------------------------------------------------------------------------
// Fast copy a memory block from a source address to a destination
// This function use a unrolled LDI loop (18,75% faster ; break-even at 6 loops for multiple of 16 size or at 31 loops otherwise)
void Mem_FastCopy(const void* src, void* dest, u16 size) __NAKED // Stack: 4 bytes
{
	src;	// HL
	dest;	// DE
	size;	// SP[2]

__asm
	// Get parameters
	pop		iy							// 16 cc (return address)
	pop		bc							// 11 cc (retreive size) | stack ajusted
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

#if (!MEM_USE_BUILTIN)
//-----------------------------------------------------------------------------
// Fill a memory block with a given value (minimal size: 2)
void Mem_Set(u8 val, void* dest, u16 size) __NAKED // Stack: 4 bytes
{
	val;	// A
	dest;	// DE
	size;	// SP[2]

__asm
	push	de
	pop		hl
	ld		(hl), a
	inc		de
	// Get parameters
	pop		iy							// 16 cc (return address)
	pop		bc							// 11 cc (retreive size) | stack ajusted
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
#endif // (!MEM_USE_BUILTIN)

//-----------------------------------------------------------------------------
// Fill a memory block with a given 16-bits value (minimal size of 2 bytes).
void Mem_Set_16b(u16 val, void* dest, u16 size)
{
	val;	// HL
	dest;	// DE
	size;	// SP[2]

__asm
	push	de
	ex		de, hl
	ld		(hl), d
	inc		hl
	ld		(hl), e
	inc		hl
	ex		de, hl
	pop		hl
	// Get parameters
	pop		iy							// 16 cc (return address)
	pop		bc							// 11 cc (retreive size) | stack ajusted
#if 0//(MEM_USE_VALIDATOR)
	// Skip if size < 2
	ld		a, b
	or		a, c
	jp		z, mem_fill16_end
	dec		bc
	jp		z, mem_fill16_end
	dec		bc
	jp		z, mem_fill16_end
#else
	dec		bc
	dec		bc
#endif
	// Do fill
	ldir								// 23/18 cc
mem_fill16_end:
	jp		(iy)						// 10 cc
__endasm;
}

#if (MEM_USE_FASTSET)
//-----------------------------------------------------------------------------
// Fast fill a memory block with a given value (minimal size of 2 bytes).
void Mem_FastSet(u8 val, void* dest, u16 size) __NAKED // Stack: 4 bytes
{
	val;	// A
	dest;	// DE
	size;	// SP[2]

__asm
	push	de
	pop		hl
	ld		(hl), a
	inc		de
	// Get parameters
	pop		iy							// 16 cc (return address)
	pop		bc							// 11 cc (retreive size) | stack ajusted
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

#if (MEM_USE_DYNAMIC)

//=============================================================================
// DYNAMIC MEMORY ALLOCATOR
//=============================================================================

// Dynamic memory chunk flag
#define MEM_CHUNK_FREE				0x8000

// Dynamic memory chunk root
MemChunkHeader* g_MemChunkRoot = NULL;

//-----------------------------------------------------------------------------
// Allocates a static memory block which can then be used to allocate chunks dynimically.
void Mem_DynamicInitialize(void* base, u16 size)
{
	g_MemChunkRoot = (MemChunkHeader*)base;
	g_MemChunkRoot->Size = (size - sizeof(MemChunkHeader)) | MEM_CHUNK_FREE;
	g_MemChunkRoot->Next = NULL;
}

//-----------------------------------------------------------------------------
// Allocate a memory chunk from the dynamic memory buffer
void* Mem_DynamicAlloc(u16 size)
{
	#if (MEM_USE_VALIDATOR)
	if (size == 0)
		return NULL;
	#endif

	MemChunkHeader* chunk = g_MemChunkRoot;
	while (chunk)
	{
		u16 chunkSize = chunk->Size;
		if (chunkSize & MEM_CHUNK_FREE) // Free chunk
		{
			chunkSize &= ~MEM_CHUNK_FREE;
			if (chunkSize == size) // Re-use chunk
			{
				chunk->Size &= ~MEM_CHUNK_FREE;
				return (void*)((u16)chunk + sizeof(MemChunkHeader));
			}
			u16 needSize = size + sizeof(MemChunkHeader);
			if (chunkSize > needSize) // Create new sub-chunk
			{
				MemChunkHeader* newChunk = (MemChunkHeader*)((u16)chunk + needSize); // New free sub-chunk
				newChunk->Size = (chunkSize - needSize) | MEM_CHUNK_FREE;
				newChunk->Next = chunk->Next;

				chunk->Size = size; // Allocated sub-chunk
				chunk->Next = newChunk;

				return (void*)((u16)chunk + sizeof(MemChunkHeader));
			}
		}
		chunk = chunk->Next;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Merge contiguous empty memory blocks
void Mem_DynamicMerge()
{
	MemChunkHeader* chunk = g_MemChunkRoot;
	while (chunk)
	{
		MemChunkHeader* nextChunk = chunk->Next;
		if ((nextChunk != NULL) && (chunk->Size & MEM_CHUNK_FREE) && (nextChunk->Size & MEM_CHUNK_FREE))
		{
			chunk->Size += nextChunk->Size + sizeof(MemChunkHeader);
			chunk->Next = nextChunk->Next;
		}
		chunk = chunk->Next;
	}
}

//-----------------------------------------------------------------------------
// Free a memory chunk from the dynamic memory buffer
void Mem_DynamicFree(void* ptr)
{
	MemChunkHeader* chunk = (MemChunkHeader*)((u16)ptr - sizeof(MemChunkHeader));
	chunk->Size |= MEM_CHUNK_FREE;
	Mem_DynamicMerge();
}

#endif // (MEM_USE_DYNAMIC)
