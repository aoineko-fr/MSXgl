//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- List of library modules to build (array)
LibModules = [ "sprite_fx", "fsm", ...LibModules ];

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "SF";

//-- Check for ROM boot skipping if a given key is pressed (string)
//   - SKIP_NONE        Don't skip the ROM boot
//   - SKIP_ALL         Skip the ROM boot and go directly to BASIC or MSX-DOS
//   - SKIP_DELAY		Skip the delay boot and start the ROM program immediately
ROMSkipBoot = "SKIP_ALL";

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