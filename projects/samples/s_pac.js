//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- List of library modules to build (array)
LibModules = [ "device/pac", ...LibModules ];

//-- Emulator extra parameters to be add to command-line (string). Emulator sotfware specific
EmulExtraParam = "-exta slotexpander -ext pac -ext fmpac";

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "PA";
