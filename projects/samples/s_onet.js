//*****************************************************************************
// PROJECT OVERWRITE
//*****************************************************************************
Machine = "1";
Target = "ROM_16K_P2";
LibModules = [ ...LibModules, "network/obsonet" ];

// Postpone the ROM startup to let the other ROMs initialize (BDOS for example) (0 = false, 1 = true)
ROMDelayBoot = "1";
