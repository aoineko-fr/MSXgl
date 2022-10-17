@echo off

::*****************************************************************************
:: PROJECT OVERWRITE
::*****************************************************************************

:: MSX machine version
set Machine=1

:: Program media target
set Target=BIN_USR

:: List of library modules to build
set LibModules=device\ninjatap,bios

:: Emulator port: joystick, mouse, keyboard (fake joystick)
set EmulPortA=NinjaTap

:: Data to copy to disk
set DiskFiles=s_usr.bas