@echo off

::*****************************************************************************
:: PROJECT OVERWRITE
::*****************************************************************************

:: MSX version
set Machine=2

:: List of library modules to build
set LibModules=scroll,%LibModules%

:: Postpone the ROM startup to let the other ROMs initialize (BDOS for example) (0=false, 1=true)
REM set ROMDelayBoot=1

:: Data to copy to disk
REM set DiskFiles=content\misc\ntap_usr.bas