// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄▄       ▄▄  ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██  ▄███ ██▀ ██▀ ▄███ ██▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ▀█▄ ▀█▄▄ ▀█▄ ▀█▄ ▀█▄▄ ██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Pletter v0.5c - XL2S Entertainment 2008
// Copyright (c) 2002-2003 Team Bomba under MIT License
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

// Function: Pletter_UnpackToRAM
// Unpack Pletter compressed data to a RAM buffer
//
// Paramaters:
//   source	- Address of the source data.
//   dest	- Address of unpack destination in RAM.
void Pletter_UnpackToRAM(const void* source, void* dest);

// Function: Pletter_UnpackToVRAM
// Unpack Pletter compressed data directly to VRAM
//
// Paramaters:
//   source	- Address of the source data.
//   dest	- Address of unpack destination in VRAM.
//            Only first 16 KB of VRAM can be use as destination (0000-3FFF)
void Pletter_UnpackToVRAM(const void* source, u16 dest);
