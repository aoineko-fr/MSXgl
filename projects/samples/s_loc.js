//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- List of library modules to build (array)
LibModules = [ "localize", "vdp", "print", "input" ];

//-- List files to be localized (array)
LocFiles = [ /*"datasrc/loc_src2.ini",*/ "datasrc/loc_src1.ini" ];

//-- Localization output filename (string)
LocOutput = "content/loc_data.h";

//-- Localization structure name (string)
LocStruct = "g_LocData";

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "LO";