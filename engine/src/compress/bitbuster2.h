// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄  ▄▄  ▄▄             ▄▄           
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ▄  ██▀ ██▄  ██ █  ██▀ ██▀ ▄███ ██▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ██ ▀█▄ ██▄▀ ▀█▄█ ▄██  ▀█▄ ▀█▄▄ ██  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// Bitbuster 2 - Copyright (c) 2003-2004, 2022 Arjan Bakker
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

// Function: Bitbuster2_UnpackToRAM
// Unpack Bitbuster 2 compressed data to a RAM buffer
void Bitbuster2_UnpackToRAM(const void* source, void* dest);