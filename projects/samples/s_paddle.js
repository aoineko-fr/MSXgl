//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- List of library modules to build (array)
LibModules = [ "device/paddle", ...LibModules ];

//-- Target program format (string)
Target = "ROM_48K_ISR";

//-- Plug a virtual device into the joystick port A (string)
EmulPortA = "paddle";

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "PD";
