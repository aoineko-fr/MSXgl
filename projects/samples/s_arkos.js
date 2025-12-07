//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- Target program format (string)
Target = "ROM_ASCII8";

//-- ROM mapper total size in KB (number). Must be a multiple of 8 or 16 depending on the mapper type (from 64 to 4096 for legacy mappers; can be up to 65536 for NEO-16 mapper)
ROMSize = 256;

//-- Project segments base name (string). ProjName will be used if not defined
ProjSegments = "segment/s_arkos";

//-- List of library modules to build (array)
LibModules = [ "arkos/akg_player", "arkos/akm_player", "arkos/aky_player", "arkos/aky_6ch_player", "arkos/aky_darky_player", ...LibModules ];

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "AK";

//-- Add second PSG extension (boolean)
EmulPSG2 = true;
