//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Clear all intermediate files and exit (boolean)
// DoClean = true;

//-- Target MSX machine version (string)
Machine = "2";

//-- List of library modules to build (array)
LibModules = [ "tool/qrcode_tiny", ...LibModules ];

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "QR";
