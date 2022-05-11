@ECHO off

rem ***************************************************************************
rem * PROJECT OVERWRITE                                                       *
rem ***************************************************************************

:: MSX version:
SET Machine=2
:: Target:
set Target=DOS1
:: List of library modules to build
set LibModules=dos,%LibModules%
:: Data to copy to disk
set DiskFiles=content\img\IMAGE01.SC5,content\img\IMAGE01.SC8,content\img\IMAGE04.SC5,content\img\IMAGE04.SC8,content\img\PALETTE.SC5,content\img\PALETTE.SC8
