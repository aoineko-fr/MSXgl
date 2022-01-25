@ECHO off

rem ***************************************************************************
rem * PROJECT OVERWRITE                                                       *
rem ***************************************************************************
set Machine=1
set Target=ROM_ASCII16
set LibModules=psg,scc,msx-music,vgm\vgm_player,%LibModules%
set EmulExtraParam=-exta slotexpander -extb scc