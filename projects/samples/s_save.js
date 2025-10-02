//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- List of library modules to build (array)
LibModules = [ "tool/disk_save", "dos", "device/pac", "fsm", ...LibModules];

//-- Emulator extra parameters to be add to command-line (string). Emulator sotfware specific
EmulExtraParam = "-exta slotexpander -ext pac -ext fmpac";

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "SV";

//-- Postpone the ROM startup to let the other ROMs initialize like Disk controller or Network cartridge (boolean)
ROMDelayBoot = true;