@echo off

::*****************************************************************************
:: PROJECT OVERWRITE
::*****************************************************************************
set Machine=1
set LibModules=psg,scc,msx-music,msx-audio,lvgm\lvgm_player,system,bios,vdp,print,input,memory
set EmulExtraParam=-exta slotexpander -ext scc -ext fmpac -ext audio