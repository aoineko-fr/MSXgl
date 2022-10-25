@echo off

::*****************************************************************************
:: PROJECT OVERWRITE
::*****************************************************************************

:: MSX version
set Machine=2

:: List of library modules to build
set LibModules=scroll,%LibModules%

:: Install BDOS driver for ROM program? (0=false, 1=true)
set InstallBDOS=1
