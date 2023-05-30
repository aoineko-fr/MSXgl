//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "2";

//-- Target program format (string)
Target = "ROM_48K_ISR";

//-- Type of custom ISR to install (string). ISR is install in RAM or ROM depending on Target and InstallRAMISR parameters
//   - VBLANK     V-blank handler
//   - VHBLANK    V-blank and h-blank handler (V9938 or V9958)
//   - V9990      v-blank, h-blank and command end handler (V9990)
CustomISR = "VHBLANK";

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "G3";
