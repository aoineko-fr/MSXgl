//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- List of library modules to build (array)
LibModules = [ "system", "vdp", "print", "input", "memory" ];

//-- Target program format (string)
Target = "ROM_NEO8";

//-- ROM mapper total size in KB (number). Must be a multiple of 8 or 16 depending on the mapper type (from 64 to 4096)
ROMSize = 8192;

//-- Use automatic banked call and trampoline functions (boolean). For mapped ROM
BankedCall = false;

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "NM";
