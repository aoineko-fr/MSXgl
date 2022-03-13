@echo off

rem ***************************************************************************
rem * PROJECT OVERWRITE                                                       *
rem ***************************************************************************
set Machine=1
set LibModules=wyz\wyz_player2,psg,system,bios,vdp,print,input,memory
set AddSources=content\wyz\Nayade.asm,content\wyz\RA_PSG.asm,content\wyz\jinj_med.asm,content\wyz\gothic.asm,content\wyz\shampoo.asm,content\wyz\quezesto.asm
set EmulExtraParam=-exta slotexpander -ext MegaFlashROM_SCC+_SD
