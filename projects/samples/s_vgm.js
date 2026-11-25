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
LibModules = [ "audio/opl4", "audio/opna", "psg", "scc", "msx-music", "msx-audio", "vgm/vgm_player", "system", "bios", "vdp", "print", "input", "memory" ];

//-- List of raw data files to be added to final binary (array). Each entry must be in the following format: { offset:0x0000, file:"myfile.bin" }
RawFiles = [
	{ segment: 3, file:"content/vgm/psg_ds4_03.vgm"},
	{ segment: 4, file:"content/vgm/psg_goemon_07.vgm" },
	{ segment: 5, file:"content/vgm/psg_metalgear_05.vgm" },
	{ segment: 7, file:"content/vgm/scc_f1spirit_01.vgm" },
	{ segment: 3, file:"content/vgm/mm_undeadline_03.vgm" },
	{ segment: 8, file:"content/vgm/mm_ff_03.vgm" },
	{ segment: 9, file:"content/vgm/ma_xevious_01.vgm" },
	{ segment:10, file:"content/vgm/ma_proyakyu_10.vgm" },
	{ segment:11, file:"content/vgm/opl2_monkeyisland_11.vgm" },
	{ segment:12, file:"content/vgm/opl3_heretic_01.vgm" },
	{ segment:13, file:"content/vgm/opl4_sonyc_05.vgm" },
	{ segment: 6, file:"content/vgm/opn_konamic_31.vgm" },
	{ segment:14, file:"content/vgm/opna_valis2_23.vgm" },
	{ segment:15, file:"content/vgm/opna_mistyblue_05.vgm" },
];

//-- Add SCC extension (boolean)
EmulSCC = true;

//-- Add MSX-Music extension (boolean)
EmulMSXMusic = true;

//-- Add MSX-Audio extension (boolean)
EmulMSXAudio = true;

//-- Add OPL4 extension (boolean)
EmulOPL4 = true;

//-- Add OPNA extension (boolean)
EmulOPNA = true;

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "VG";
