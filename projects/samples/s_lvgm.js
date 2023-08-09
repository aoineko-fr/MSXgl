//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- Target program format (string)
Target = "DOS2";

//-- List of library modules to build (array)
LibModules = [ "psg", "scc", "msx-music", "msx-audio", "vgm/lvgm_player", "dos", "dos_mapper", "system", "bios", "vdp", "print", "input", "memory" ];

//-- List of data files to copy to disk (array)
DiskFiles = [ "content/lvgm/ds4.lvm", "content/lvgm/galious.lvm", "content/lvgm/goemon.lvm", "content/lvgm/mg.lvm", "content/lvgm/honotori.lvm", "content/lvgm/penguin.lvm", "content/lvgm/yureikun.lvm", "content/lvgm/ff.lvm", "content/lvgm/laydock2.lvm", "content/lvgm/undeadli.lvm", "content/lvgm/f1spirit.lvm", "content/lvgm/salamand.lvm", "content/lvgm/manbow.lvm" , "content/lvgm/mg2.lvm" ];

//-- Add SCC extension (boolean)
EmulSCC = true;

//-- Add MSX-Music extension (boolean)
EmulMSXMusic = true;

//-- Add MSX-Audio extension (boolean)
EmulMSXAudio = true;

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "LV";
