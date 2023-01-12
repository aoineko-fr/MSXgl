//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "2";

//-- Target program format (string)
Target = "DOS0";

//-- List of library modules to build (array)
LibModules = [ "dos", "string", ...LibModules ];

//-- List of data files to copy to disk (array)
DiskFiles = [ "content/img/IMAGE01.SC8", "content/img/IMAGE04.SC8", "content/img/PALETTE.SC8" ];
