//*****************************************************************************
// PROJECT OVERWRITE
//*****************************************************************************
Machine = "1";
LibModules = [ "wyz/wyz_player2", "psg", "system", "bios", "vdp", "print", "input", "memory" ];
AddSources = [ "content/wyz/Nayade.asm", "content/wyz/RA_PSG.asm", "content/wyz/jinj_med.asm", "content/wyz/gothic.asm", "content/wyz/shampoo.asm", "content/wyz/quezesto.asm" ];
EmulExtraParam = "-exta slotexpander -ext MegaFlashROM_SCC+_SD";
