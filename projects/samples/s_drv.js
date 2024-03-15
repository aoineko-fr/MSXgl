//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- Target program format (string)
Target = "RAW";

//-- List of library modules to build (array)
LibModules = [];

//-- Overwrite code starting address (number). For example. 0xE0000 for a driver in RAM
ForceCodeAddr = 0xE000;

//-- Overwrite RAM starting address (number). For example. 0xE0000 for 8K RAM machine
ForceRamAddr = 0; // 0: right after code area

//-- Command lines to be executed after the build process (array)
PostBuildScripts = [ "copy emul\\bin\\s_drv.bin content\\sample.drv /Y" ];

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "DV";
