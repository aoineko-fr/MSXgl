//*****************************************************************************
// PROJECT OVERWRITE
//*****************************************************************************

// MSX version:
Machine = "2P";

// Target:
Target = "DOS1";

// List of library modules to build
LibModules = [ ...LibModules, "dos" ];

// Data to copy to disk
DiskFiles = [ "content/img/IMAGE01.SC5", "content/img/IMAGE01.SC8", "content/img/IMAGE01.S12", "content/img/IMAGE04.SC5", "content/img/IMAGE04.SC8", "content/img/IMAGE04.S12", "content/img/PALETTE.SC5", "content/img/PALETTE.SC8", "content/img/PALETTE.S12" ];
