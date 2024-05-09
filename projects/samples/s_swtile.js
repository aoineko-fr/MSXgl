//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- List of library modules to build (array)
LibModules = [ "tile", "debug", "game_pawn", ...LibModules ];

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "ST";

//-- Emulator extra parameters to be add to command-line (string). Emulator sotfware specific
// EmulExtraParam = `-script ${ToolsDir}script/openMSX/debugger_pvm.tcl`; // uncomment for use Debug module with openMSX