//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- Target program format (string)
Target = "ROM_ASCII16";

//-- ROM mapper total size in KB (number). Must be a multiple of 8 or 16 depending on the mapper type (from 64 to 4096)
ROMSize = 256;

//-- List of library modules to build (array)
LibModules = [ "psg", "scc", "msx-music", "msx-audio", "vgm/vgm_player", "system", "bios", "vdp", "print", "input", "memory" ];

//-- List of raw data files to be added to final binary (array). Each entry must be in the following format: { offset:0x0000, file:"myfile.bin" }
RawFiles = [
	{ segment: 2, file:"content/vgm/psg_ds4_03.vgm"},
	{ segment: 3, file:"content/vgm/psg_goemon_07.vgm" },
	{ segment: 5, file:"content/vgm/psg_metalgear_05.vgm" },
	{ segment: 6, file:"content/vgm/psg_honotori_09.vgm" },
	{ segment: 7, file:"content/vgm/scc_f1spirit_01.vgm" },
	{ segment: 2, file:"content/vgm/mm_undeadline_03.vgm" },
	{ segment: 8, file:"content/vgm/mm_ff_03.vgm" },
	{ segment: 9, file:"content/vgm/ma_xevious_01.vgm" },
	{ segment:10, file:"content/vgm/ma_proyakyu_10.vgm" },
];

//-- Add SCC extension (boolean)
EmulSCC = true;

//-- Add MSX-Music extension (boolean)
EmulMSXMusic = true;

//-- Add MSX-Audio extension (boolean)
EmulMSXAudio = true;

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "VG";
