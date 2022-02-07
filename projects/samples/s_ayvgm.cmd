@ECHO off

::*****************************************************************************
:: PROJECT OVERWRITE
::*****************************************************************************
set Machine=1
set LibModules=psg,scc,ayvgm\ayvgm_player,%LibModules%
set EmulExtraParam=-exta slotexpander -ext scc -ext MegaFlashROM_SCC+_SD