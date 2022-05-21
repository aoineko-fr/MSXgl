@ECHO off

rem ***************************************************************************
rem * PROJECT OVERWRITE                                                       *
rem ***************************************************************************

:: MSX version:
SET Machine=2
:: Target:
set Target=DOS1
:: List of library modules to build
set LibModules=%LibModules%,dos,compress,compress/bitbuster,compress/zx0
:: Data to copy to disk
set DiskFiles=content\zip\data10.bin,content\zip\data10.rlep,content\zip\data10.zx0,content\zip\data10.pck