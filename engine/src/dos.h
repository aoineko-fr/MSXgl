// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄   ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██ █ ▀█▄  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▀█▄▀ ▄▄█▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// MSX-DOS routines handler 
//
// References:
// - ...
//─────────────────────────────────────────────────────────────────────────────

// Bios calls that can be called directly from MSX-DOS
//
// RDSLT (000CH) - read value at specified address of specified slot
// WRSLT (0014H) - write value at specified address of specified slot
// CALSLT (001CH) - call specified address of specified slot
// ENASLT (0024H) - make specified slot available
// CALLF (0030H) - call specified address of specified slot

// Start BASIC from DOS : https://www.msx.org/wiki/Disk-ROM_BIOS#4022H