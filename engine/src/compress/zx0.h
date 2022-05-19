// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄ ▄▄ ▄  ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ▄█▀ ▀█▄▀ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▄ ██ █ ▀▄█▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// ZX0 decoder by Einar Saukas & Urusergi
//  "Standard" version (68 bytes only)
//  https://github.com/einar-saukas/ZX0/blob/main/z80/dzx0_standard.asm
//─────────────────────────────────────────────────────────────────────────────
#pragma once

// Function: ZX0_Unpack
// Unpack ZX0 compressed data to a RAM buffer
void ZX0_Unpack(u16 source, u16 dest);