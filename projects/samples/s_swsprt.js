//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target program format (string)
Target = "ROM_ASCII8";

//-- Select RAM in slot 0 and install ISR there (boolean). For MSX with at least 64 KB of RAM
InstallRAMISR = "RAM0_SEGMENT";

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
HexBinOpt = "--segaddr 4 0x0000";