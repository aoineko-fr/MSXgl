// ____________________________
// ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄▄▄▄  ▄▄   ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀  ██  ██ █ ▀█▄  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▄██▄ ▀█▄▀ ▄▄█▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// BIOS Disk-ROM routines handler 
//
// References:
// - MSX2 Technical Handbook
// - http://map.grauw.nl/resources/msxbios.php
//─────────────────────────────────────────────────────────────────────────────

//-----------------------------------------------------------------------------
// DiskROM RAM Variables
//-----------------------------------------------------------------------------

#define M_KANJTABLE	0xF30F // Two pairs of limits for the first bytes of Shift-JIS characters
const u16 __at(M_KANJTABLE) g_KANJTABLE[2];

#define M_RAMAD0	0xF341 // Slot address of RAM in page 0 (DOS)
const u8 __at(M_RAMAD0) g_RAMAD0;

#define M_RAMAD1	0xF342 // Slot address of RAM in page 1 (DOS)
const u8 __at(M_RAMAD1) g_RAMAD1;

#define M_RAMAD2	0xF343 // Slot address of RAM in page 2 (DOS/BASIC)
const u8 __at(M_RAMAD2) g_RAMAD2;

#define M_RAMAD3	0xF344 // Slot address of RAM in page 3 (DOS/BASIC)
const u8 __at(M_RAMAD3) g_RAMAD3;

#define M_MASTER	0xF348 // Main DiskROM slot address
const u8 __at(M_MASTER) g_MASTER;

