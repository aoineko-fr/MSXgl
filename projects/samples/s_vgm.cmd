@ECHO off

rem ***************************************************************************
rem * PROJECT OVERWRITE                                                       *
rem ***************************************************************************
set Machine=1
set Target=ROM_ASCII16
set ROMSize=256
set LibModules=psg,scc,msx-music,msx-audio,vgm\vgm_player,system,bios,vdp,print,input,memory
REM set EmulExtraParam=-exta slotexpander -ext scc -ext fmpac -ext audio
set EmulSCC=1
set EmulMSXMusic=1
set EmulMSXAudio=1
