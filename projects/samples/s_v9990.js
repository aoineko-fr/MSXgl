//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- List of library modules to build (array)
LibModules = [ "v9990", ...LibModules];

//-- Target program format (string)
// Target = "ROM_48K_ISR";

//-- Type of custom ISR to install (string). ISR is install in RAM or ROM depending on Target and InstallRAMISR parameters
//   - NONE       No ISR
//   - ALL        Handle all interruptions
//   - VBLANK     V-blank handler
//   - VHBLANK    V-blank and h-blank handler (V9938 or V9958)
//   - V9990      V-blank, h-blank and command end handler (V9990)
// CustomISR = "V9990";

//-- Add V9990 video-chip extension (boolean)
EmulV9990 = true;

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "V9";
