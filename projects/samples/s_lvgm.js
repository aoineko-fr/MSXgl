//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- Program media target:
Target = "ROM_48K_ISR";

//-- List of library modules to build (array)
LibModules = [ "psg", "scc", "msx-music", "msx-audio", "vgm/lvgm_player", "system", "bios", "vdp", "print", "input", "memory" ];

//-- Add SCC extension (boolean)
EmulSCC = true;

//-- Add MSX-Music extension (boolean)
EmulMSXMusic = true;

//-- Add MSX-Audio extension (boolean)
EmulMSXAudio = true;

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "LV";
