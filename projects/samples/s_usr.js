//*****************************************************************************
// PROJECT OVERWRITE
//*****************************************************************************

// MSX machine version
Machine = "1";

// Program media target
Target = "BIN_USR";

// List of library modules to build
LibModules = [ "device/ninjatap", "bios" ];

// Emulator port: joystick, mouse, keyboard (fake joystick)
EmulPortA = "NinjaTap";

// Data to copy to disk
DiskFiles = [ "content/misc/ntap_usr.bas" ];