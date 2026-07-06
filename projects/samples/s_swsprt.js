//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target program format (string)
Target = "ROM_ASCII8";

//-- Install ISR in RAM; either in page 0 (with optional code/data copy there) or in page 3 to use with IM2 (string). For MSX with at least 64 KB of RAM
//   - RAMISR_NONE       Don't install ISR in RAM
//   - RAMISR_PAGE0      Install ISR in page 0
//   - RAMISR_SEGMENT0   Install ISR and segment data (for mapped-ROM) in page 0
//   - RAMISR_PAGE3      Install ISR in page 3 (to use with IM2)
InstallRAMISR = "RAMISR_SEGMENT0";

//-- Type of custom ISR to install (string). ISR is install in RAM or ROM depending on Target and InstallRAMISR parameters
//   - NONE       No ISR
//   - ALL        Handle all interruptions
//   - VBLANK     V-blank handler
//   - VHBLANK    V-blank and h-blank handler (V9938 or V9958)
//   - V9990      V-blank, h-blank and command end handler (V9990)
CustomISR = "VBLANK";

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "SW";

//-- Additionnal options of Hex to Binary convertor (string)
// HexBinOpt = "--segaddr 4 0x0000";