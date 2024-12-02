// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▄███ ▄█▄█ ▄█▀▄ ██▄▀ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀█▄▄ ██ █ ▀█▄▀ ██    ▀██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                            ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Memory operations
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: Mem_LShift
// Left-shift all bytes in a memory buffer
//
// Paramters:
//   ptr - Buffer containing data
//   size - Size of the buffer
void Mem_LShift(const u8* ptr, u16 size);

// Function: Mem_RShift
// Right-shift all bytes in a memory buffer
//
// Paramters:
//   ptr - Buffer containing data
//   size - Size of the buffer
void Mem_RShift(const u8* ptr, u16 size);

// Function: Mem_Inc
// Increment all bytes in a memory buffer
//
// Paramters:
//   ptr - Buffer containing data
//   size - Size of the buffer
void Mem_Inc(const u8* ptr, u16 size);

// Function: Mem_Dec
// Decrement all bytes in a memory buffer
//
// Paramters:
//   ptr - Buffer containing data
//   size - Size of the buffer
void Mem_Dec(const u8* ptr, u16 size);