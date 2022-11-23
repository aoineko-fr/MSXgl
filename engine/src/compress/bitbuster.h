// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄  ▄▄  ▄▄             ▄▄           
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ▄  ██▀ ██▄  ██ █  ██▀ ██▀ ▄███ ██▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ██ ▀█▄ ██▄▀ ▀█▄█ ▄██  ▀█▄ ▀█▄▄ ██  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Bitbuster v1.2 - (c) 2002-2003  Team Bomba
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

// Function: Bitbuster_UnpackToRAM
// Unpack Bitbuster compressed data to a RAM buffer
void Bitbuster_UnpackToRAM(const void* source, void* dest);

// Function: Bitbuster_UnpackToVRAM
// Unpack Bitbuster compressed data directly to VRAM
// Only first 16 KB of VRAM can be use as destination (0000-3FFF)
void Bitbuster_UnpackToVRAM(const void* source, u16 dest);
