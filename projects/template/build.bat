@echo off
rem __________________________
rem ██▀█▀██▀▀▀█▀▀█▀█  ▄▄▄ ▄▄  │   ▄▄▄                ▄▄      
rem █  ▄ █▄ ▀██▄ ▀▄█ ██   ██  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
rem █  █ █▀▀ ▄█  █ █ ▀█▄█ ██▄▄│  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
rem ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀──────────┘                 ▀▀
rem  Build Tool configuration
rem ───────────────────────────────────────────────────────────────────────────
call ..\default_config.cmd

rem ***************************************************************************
rem * PROJECT SETTINGS                                                        *
rem ***************************************************************************

rem  Project name (will be use for output filename)
SET ProjName=template

rem  List of modules to link
SET LibModules=system,bios,vdp,print,input,memory,math,draw
rem  MSX version:
rem  - 1		MSX 1
rem  - 2		MSX 2
rem  - 2P		MSX 2+
rem  - TR		MSX TurboR
SET Machine=1
rem  Target:
rem  - BIN		.bin	BASIC binary program (8000h~)
rem  - ROM16	.rom	16KB ROM in page 1 (4000h ~ 7FFFh)
rem  - ROM16P0	.rom	16KB ROM in page 0 (0000h ~ 3FFFh)
rem  - ROM16P2	.rom	16KB ROM in page 2 (8000h ~ BFFFh)
rem  - ROM32	.rom	32KB ROM in page 1-2 (4000h ~ BFFFh)
rem  - ROM32P0	.rom	32KB ROM in page 0-1 (0000h ~ 7FFFh)
rem  - ROM48	.rom	48KB ROM in page 0-2 (0000h ~ BFFFh) No direct acces to Main-ROM
rem  - DOS		.com	MSX-DOS program (0100h~) No direct acces to Main-ROM
rem  - DOSARG	.com	MSX-DOS program (using command line arguments ; 0100h~) No direct acces to Main-ROM
set Target=ROM_32K

rem ***************************************************************************
rem * BUILD STEPS                                                             *
rem ***************************************************************************
set DoClean=0
set DoCompile=1
set DoMake=1
set DoPackage=1
set DoDeploy=1
set DoRun=1

rem ***************************************************************************
rem * START BUILD                                                             *
rem ***************************************************************************
call %LibDir%\script\build.cmd