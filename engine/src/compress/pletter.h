// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Pletter v0.5c - XL2S Entertainment 2008
// Copyright (c) 2002-2003 Team Bomba under MIT License
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

#define LENGTHINDATA 0

// Function: Pletter_UnpackToRAM
// Unpack Pletter compressed data to a RAM buffer
void Pletter_UnpackToRAM(const void* source, void* dest);