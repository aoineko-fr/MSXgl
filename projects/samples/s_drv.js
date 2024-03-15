//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- Target program format (string)
Target = "RAW";

//-- List of library modules to build (array)
LibModules = [ "bios" ];

//-- Overwrite code starting address (number). For example. 0xE0000 for a driver in RAM
ForceCodeAddr = 0xE000;

//-- Overwrite RAM starting address (number). For example. 0xE0000 for 8K RAM machine
ForceRamAddr = 0xF000;

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "DV";
