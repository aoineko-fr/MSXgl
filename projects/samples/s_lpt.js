
//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "LP";

//-- List of library modules to build (array)
LibModules = [ "device/printer", ...LibModules ];

//-- Plug printer device (boolean)
EmulPrinter  = true;