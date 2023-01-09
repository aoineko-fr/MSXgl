//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "2";

//-- Target program format (string)
Target = "DOS1";

//-- List of library modules to build (array)
LibModules = [ "dos", "compress", "compress/bitbuster", "compress/bitbuster2", "compress/zx0", "compress/pletter", ...LibModules ];

//-- List of data files to copy to disk (array)
DiskFiles = [ "content/zip/data10.bin", "content/zip/data10.rlep", "content/zip/data10.zx0", "content/zip/data10.pck", "content/zip/data10.bb2", "content/zip/data10.pl5" ];