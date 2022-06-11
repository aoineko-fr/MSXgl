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