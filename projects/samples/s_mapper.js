//*****************************************************************************
// PROJECT SETTINGS
//*****************************************************************************

// MSX version
Machine = "1";
// List of library modules to build
LibModules = [ "system", "vdp", "print", "input", "memory" ];

// Target
Target = "ROM_ASCII8";
// ROM mapper size (from 64 to 4096). Must be a multiple of 8 or 16 depending on the mapper type
ROMSize = 128;
// Use banked call (and trampoline functions)
BankedCall = true;
