//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- List of library modules to build (array)
LibModules = [ "sprite_fx", "fsm", ...LibModules ];

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "SF";

//-- Check for ROM boot skipping if a given key is pressed (boolean)
ROMSkipBoot = true;

//-- The key to be check for ROM boot skipping (string). Key must be from keyboard row #7.
//   - F4
//   - F5
//   - ESC
//   - TAB
//   - STOP
//   - BS
//   - SELECT
//   - RETURN
ROMSkipBootKey = "BS";