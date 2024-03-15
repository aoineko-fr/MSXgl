//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- Target program format (string)
Target = "BIN_USR";

//-- List of library modules to build (array)
LibModules = [ "device/ninjatap", "bios" ];

//-- Overwrite code starting address (number). For example. 0xE0000 for a driver in RAM
ForceCodeAddr = 0xC000;

//-- Plug a virtual device into the joystick port A (string)
EmulPortA = "NinjaTap";

//-- List of data files to copy to disk (array)
DiskFiles = [ "content/misc/ntap_usr.bas" ];

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "US";
