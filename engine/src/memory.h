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

#define MEM_USE_DYNAMIC				TRUE

#if (MEM_USE_DYNAMIC)
// 
struct MemChunkHeader
{
	u16 Size;
	struct MemChunkHeader* Next;
};
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

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

#if (MEM_USE_FASTCOPY)
// Function: Mem_FastSet
// Fast fill a memory block with a given value (minimal size of 2 bytes).
// Use 16 unrolled-LDI loop.
//
// Parameters:
//   val - Value to write
//   dst - Destination address
//   size - The size of data to fill
void Mem_FastSet(u8 val, void* dest, u16 size);
#endif

#if (MEM_USE_DYNAMIC)

// Function: Mem_DynamicInitialize
// 
void Mem_DynamicInitialize(void* base, u16 size);

// Function: Mem_DynamicAlloc
// 
void* Mem_DynamicAlloc(u16 size);

// Function: Mem_DynamicFree
// 
void Mem_DynamicFree(void* ptr);

// Function: Mem_GetDynamicSize
// 
inline u16 Mem_GetDynamicSize(void* ptr) { struct MemChunkHeader* chunk = (struct MemChunkHeader*)((u16)ptr - sizeof(struct MemChunkHeader)); return chunk->Size; }

#endif // (MEM_USE_DYNAMIC)
