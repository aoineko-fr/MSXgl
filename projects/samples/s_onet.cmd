@echo off

::*****************************************************************************
:: PROJECT OVERWRITE
::*****************************************************************************
set Machine=1
set Target=ROM_16K_P2
set LibModules=network\obsonet,%LibModules%

:: Postpone the ROM startup to let the other ROMs initialize (BDOS for example) (0=false, 1=true)
set ROMDelayBoot=1
