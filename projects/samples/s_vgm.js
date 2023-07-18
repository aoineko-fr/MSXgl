//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- Target program format (string)
Target = "ROM_ASCII16";

//-- ROM mapper total size in KB (number). Must be a multiple of 8 or 16 depending on the mapper type (from 64 to 4096)
ROMSize = 256;

//-- Project segments base name (string). ProjName will be used if not defined
ProjSegments = "segment/s_vgm";

//-- List of library modules to build (array)
LibModules = [ "psg", "scc", "msx-music", "msx-audio", "vgm/vgm_player", "system", "bios", "vdp", "print", "input", "memory" ];

//-- Add SCC extension (boolean)
EmulSCC = true;

//-- Add MSX-Music extension (boolean)
EmulMSXMusic = true;

//-- Add MSX-Audio extension (boolean)
EmulMSXAudio = true;

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "VG";
