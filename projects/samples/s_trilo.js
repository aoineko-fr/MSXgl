//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- Target program format (string)
Target = "ROM_KONAMI_SCC";

//-- ROM mapper total size in KB (number). Must be a multiple of 8 or 16 depending on the mapper type (from 64 to 4096)
ROMSize = 128;

//-- List of library modules to build (array)
LibModules = [ "trilo/trilo_scc_player", "system", "vdp", "print", "input", "memory" ];

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "TT";
