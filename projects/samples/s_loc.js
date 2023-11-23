//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- List of library modules to build (array)
LibModules = [ "localize", "vdp", "print", "input" ];

//-- List files to be localized (array)
LocFiles = [ "s_loc_src2.ini", "s_loc_src1.ini" ];

//-- Localization output filename (string)
LocOutput = "s_loc_data.h";

//-- Localization structure name (string)
LocStruct = "g_LocData";

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "LO";
