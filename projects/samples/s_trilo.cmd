@echo off

rem ***************************************************************************
rem * PROJECT OVERWRITE                                                       *
rem ***************************************************************************
set Machine=1
REM set Target=ROM_KONAMI
set Target=ROM_KONAMI_SCC
set ROMSize=128
REM set LibModules=trilo\trilo_scc_player,system,bios,vdp,print,input,memory
set LibModules=trilo\trilo_scc_player,system,vdp,print,input,memory
REM set EmulExtraParam=-exta slotexpander -ext scc -ext fmpac
