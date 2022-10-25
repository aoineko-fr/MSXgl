@echo off

::*****************************************************************************
:: PROJECT SETTINGS
::*****************************************************************************

:: MSX version
set Machine=1
:: List of library modules to build
set LibModules=system,vdp,print,input,memory

:: Target
set Target=ROM_ASCII8
:: ROM mapper size (from 64 to 4096). Must be a multiple of 8 or 16 depending on the mapper type
set ROMSize=128
:: Use banked call (and trampoline functions)
set BankedCall=1
