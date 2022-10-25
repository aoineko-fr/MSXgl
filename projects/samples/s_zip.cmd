@echo off

::*****************************************************************************
:: PROJECT OVERWRITE
::*****************************************************************************

:: MSX version:
set Machine=2

:: Target:
set Target=DOS1

:: List of library modules to build
set LibModules=%LibModules%,dos,compress,compress/bitbuster,compress/bitbuster2,compress/zx0,compress/pletter

:: Data to copy to disk
set DiskFiles=content\zip\data10.bin,content\zip\data10.rlep,content\zip\data10.zx0,content\zip\data10.pck,content\zip\data10.bb2,content\zip\data10.pl5