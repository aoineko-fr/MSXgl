//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "2P";

//-- Target program format (string)
Target = "DOS1";

//-- List of library modules to build (array)
LibModules = [ "dos", "string", ...LibModules];

//-- List of data files to copy to disk (array)
DiskFiles = [ "content/img/IMAGE01.SC5", "content/img/IMAGE01.SC8", "content/img/IMAGE01.S12", "content/img/IMAGE04.SC5", "content/img/IMAGE04.SC8", "content/img/IMAGE04.S12", "content/img/PALETTE.SC5", "content/img/PALETTE.SC8", "content/img/PALETTE.S12" ];
