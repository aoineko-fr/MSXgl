// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄                     
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▄███ ▄█▄█ ▄█▀▄ ██▄▀ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀█▄▄ ██ █ ▀█▄▀ ██    ▀██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                            ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

// Current heap top address
extern u16 g_HeapStartAddress;

#if (MEM_USE_DYNAMIC)
// Dynamic memro chunk structure
struct MemChunkHeader
{
	u16 Size;
	struct MemChunkHeader* Next;
};
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//.............................................................................
// Group: Heap handling

// Function: Mem_GetStackAddress
// Get the current address of the stack top (lower address).
//
// Return:
//   Current stack bottom address
inline u16 Mem_GetStackAddress()
{
	__asm
		ld		(_g_StackAddress), sp
		ld		de, (_g_StackAddress)
	__endasm;
}

// Function: Mem_GetHeapAddress
// Get the current address of the heap top (higher addresse).
//
// Return:
//   Current heap top address
inline u16 Mem_GetHeapAddress() { return g_HeapStartAddress; }

// Function: Mem_GetHeapSize
// Get the amount of free space in the heap.
//
// Return:
//   Get the current size of the heap (between heap top address and stack bottom one)
inline u16 Mem_GetHeapSize() { return Mem_GetStackAddress() - Mem_GetHeapAddress(); }

// Function: Mem_HeapAlloc
// Alloc a part of the heap.
//
// Parameters:
//   size - The size of data to allocate
//
// Return:
//   Pointer to allocated heap chunk
void* Mem_HeapAlloc(u16 size);

// Function: Mem_HeapFree
// Free the last allocated area of the heap.
//
// Parameters:
//   size - The size of data to allocate
void Mem_HeapFree(u16 size);

//.............................................................................
// Group: Memory content modification

// Function: Mem_Copy
// Copy a memory block from a source address to an other (minimal size of 1 byte).
//
// Parameters:
//   src - Source address
//   dst - Destination address
//   size - The size of data to copy
void Mem_Copy(const void* src, void* dest, u16 size);

#if (MEM_USE_FASTCOPY)
// Function: Mem_FastCopy
// Fast copy a memory block from a source address to a destination.
// Use 16 unrolled-LDI loop.
//
// Parameters:
//   src - Source address
//   dst - Destination address
//   size - The size of data to copy
void Mem_FastCopy(const void* src, void* dest, u16 size);
#endif

// Function: Mem_Set
// Fill a memory block with a given value (minimal size of 2 bytes).
//
// Parameters:
//   val - Value to write
//   dst - Destination address
//   size - The size of data to fill
void Mem_Set(u8 val, void* dest, u16 size);

#if (MEM_USE_FASTSET)
// Function: Mem_FastSet
// Fast fill a memory chunk with a given value (minimal size of 2 bytes).
// Use 16 unrolled-LDI loop.
//
// Parameters:
//   val - Value to write
//   dst - Destination address
//   size - The size of data to fill
void Mem_FastSet(u8 val, void* dest, u16 size);
#endif

//.............................................................................
// Group: Dynamic allocator
#if (MEM_USE_DYNAMIC)
// Function: Mem_DynamicInitialize
// Allocates a static memory chunk which can then be used to allocate chunks dynimically.
// Memory chunk can be in any pages but:
// - The memory space have to be continuous from a Z80 perspective.
// - You have to ensure that all memory segments are selected in their proper place when allocating or releasing memory.
//
// Parameters:
//   base - Base address of the dynamic allocator memory buffer
//   size - Size of the dynamic allocator buffer (4 bytes chunk headers will consume some of this space)
void Mem_DynamicInitialize(void* base, u16 size);

// Function: Mem_DynamicInitializeHeap
// Allocates a static memory chunk in the heap which can then be used to allocate chunks dynimically.
//
// Parameters:
//   size - Size of the dynamic allocator buffer (4 bytes chunk headers will consume some of this space)
inline void Mem_DynamicInitializeHeap(u16 size) { Mem_DynamicInitialize(Mem_HeapAlloc(size), size); }

// Function: Mem_DynamicAlloc
// Allocate a memory chunk from the dynamic memory buffer
//
// Parameters:
//   size - Size of the memory chunk to allocate
//
// Return:
//   Address of the allocated memory chunk (or NULL if no enough continuous space have been found)
void* Mem_DynamicAlloc(u16 size);

// Function: Mem_DynamicFree
// Free a memory chunk from the dynamic memory buffer
//
// Parameters:
//   ptr - Address of the allocated memory chunk
void Mem_DynamicFree(void* ptr);

// Function: Mem_GetDynamicSize
// Get the size of a dynamically allocated memory chunk
//
// Parameters:
//   ptr - Address of the allocated memory chunk
//
// Return:
//   Size of the allocated memory chunk
inline u16 Mem_GetDynamicSize(void* ptr) { struct MemChunkHeader* chunk = (struct MemChunkHeader*)((u16)ptr - sizeof(struct MemChunkHeader)); return chunk->Size; }
#endif
