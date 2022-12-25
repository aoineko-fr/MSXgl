//*****************************************************************************
// PROJECT OVERWRITE
//*****************************************************************************

// MSX version:
Machine = "2";

// Target:
Target = "DOS1";

// List of library modules to build
LibModules = [ ...LibModules, "dos", "compress", "compress/bitbuster", "compress/bitbuster2", "compress/zx0", "compress/pletter" ];

// Data to copy to disk
DiskFiles = [ "content/zip/data10.bin", "content/zip/data10.rlep", "content/zip/data10.zx0", "content/zip/data10.pck", "content/zip/data10.bb2", "content/zip/data10.pl5" ];