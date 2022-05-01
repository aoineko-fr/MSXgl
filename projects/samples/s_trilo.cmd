@echo off

rem ***************************************************************************
rem * PROJECT OVERWRITE                                                       *
rem ***************************************************************************
set Machine=1
set Target=ROM_ASCII8
set ROMSize=128
set LibModules=trilo\trilo_scc_player,system,bios,vdp,print,input,memory
set EmulExtraParam=-exta slotexpander -ext scc -ext fmpac
