// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// pletter v0.5c msx unpacker
// call unpack with hl pointing to some pletter5 data, and de pointing to the destination.
// changes all registers
// define lengthindata when the original size is written in the pletter data
// define LENGTHINDATA
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

#define LENGTHINDATA 0

// Function: Pletter_Unpack
// Unpack Pletter compressed data to a RAM buffer
void Pletter_Unpack(const void* source, void* dest);