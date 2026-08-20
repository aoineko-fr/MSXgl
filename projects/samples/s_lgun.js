//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- List of library modules to build (array)
LibModules = [ "device/lightgun", "game/menu", "fsm", "vdp", "print", "input" ];

//-- Target program format (string)
Target = 'ROM_48K_ISR';

//-- Type of custom ISR to install (string). ISR is install in RAM or ROM depending on Target and InstallRAMISR parameters
CustomISR = "VBLANK";

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "LG";

//-- Plug a virtual device into the joystick port A (string)
EmulPortA = "Joystick";