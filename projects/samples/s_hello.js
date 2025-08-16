//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- List of library modules to build (array)
LibModules = [ "tool/disk_save", "dos", ...LibModules];

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "HE";

//-- Postpone the ROM startup to let the other ROMs initialize like Disk controller or Network cartridge (boolean)
ROMDelayBoot = true;