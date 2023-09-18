//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "2P";

//-- Target program format (string)
Target = "DOS1";

//-- Parse MSX-DOS command-line arguments
DOSParseArg = true;

//-- List of library modules to build (array)
LibModules = [ "dos", "string", "tool/reg", ...LibModules];

//-- List of data files to copy to disk (array)
DiskFiles = [ "content/img/IMAGE01.SC5", "content/img/IMAGE01.SC8", "content/img/IMAGE01.S12", "content/img/IMAGE04.SC5", "content/img/IMAGE04.SC8", "content/img/IMAGE04.S12", "content/img/PALETTE.SC5", "content/img/PALETTE.SC8", "content/img/PALETTE.S12" ];

//-- Size of the final disk (.DSK file). Can be "360K" or "720K" (string)
DiskSize = "360K";

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "DO";