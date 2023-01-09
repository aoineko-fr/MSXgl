//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- List of library modules to build (array)
LibModules = [ "device/ninjatap", ...LibModules ];

//-- Plug a virtual device into the joystick port A (string)
EmulPortA = "NinjaTap";
