//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "2";

//-- Target program format (string)
Target = "DOS2";

//-- List of library modules to build (array)
LibModules = [ "dos", "dos_mapper", "string", ...LibModules];

//-- List of data files to copy to disk (array)
DiskFiles = [ "content/img/IMAGE01.SC5", "content/img/IMAGE04.SC5", "content/img/PALETTE.SC5" ];

//-- Emulator extra parameters to be add to command-line (string). Emulator sotfware specific
EmulExtraParam = "-ext ram1mb";
