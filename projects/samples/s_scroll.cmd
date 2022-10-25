@echo off

::*****************************************************************************
:: PROJECT OVERWRITE
::*****************************************************************************

:: MSX version
set Machine=2

:: List of library modules to build
set LibModules=scroll,%LibModules%

:: Install BDOS driver for ROM program? (0=false, 1=true)
REM set InstallBDOS=1

:: Data to copy to disk
REM set DiskFiles=content\misc\ntap_usr.bas