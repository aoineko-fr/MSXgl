//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- List of library modules to build (array)
LibModules = [ "tile", "debug", "game/pawn", "string", ...LibModules ];

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "ST";

//-- Emulator extra parameters to be add to command-line (string). Emulator sotfware specific
// EmulExtraParam = `-script ${ToolsDir}script/openMSX/debugger_pvm.tcl`; // uncomment for use Debug module with openMSX
// EmulExtraParam = `-script ${ToolsDir}script/openMSX/profiler_grauw.tcl`;
// EmulExtraParam = `-script ${ToolsDir}script/openMSX/profiler_salutte.tcl -command "profile::section_scope_bp frame 0xFD9F; profile_osd ms;"`;