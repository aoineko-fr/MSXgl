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

// Left-shift a memory buffer
void Mem_LShift(const u8* ptr, u16 size);

// Right-shift a memory buffer
void Mem_RShift(const u8* ptr, u16 size);

// Increment a memory buffer
void Mem_Inc(const u8* ptr, u16 size);

// Decrement a memory buffer
void Mem_Dec(const u8* ptr, u16 size);