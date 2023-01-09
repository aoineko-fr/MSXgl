//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- Target program format (string)
Target = "BIN_USR";

//-- List of library modules to build (array)
LibModules = [ "device/ninjatap", "bios" ];

//-- Plug a virtual device into the joystick port A (string)
EmulPortA = "NinjaTap";

//-- List of data files to copy to disk (array)
DiskFiles = [ "content/misc/ntap_usr.bas" ];