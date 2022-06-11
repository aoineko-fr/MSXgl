:: ____________________________
:: ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
:: ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
:: █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
:: ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
::  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
::─────────────────────────────────────────────────────────────────────────────
@echo off

::─────────────────────────────────────────────────────────────────────────────
:: Build Tool configuration
::─────────────────────────────────────────────────────────────────────────────
call ..\default_config.cmd %0

::*****************************************************************************
:: PROJECT SELECTION
::*****************************************************************************
set DoPause=0
set Input=%~n1

:CheckInput
if not [%Input%]==[] goto :FoundInput
cls
set DoPause=1
echo No valide target selected...
echo Available targets:
set Formats=BIN,DOS1,DOS2,ROM_8K,ROM_8K_P2,ROM_16K,ROM_16K_P2,ROM_32K,ROM_48K,ROM_48K_ISR,ROM_64K,ROM_64K_ISR,ROM_ASCII8,ROM_ASCII16,ROM_KONAMI,ROM_KONAMI_SCC
for %%G in (%Formats%) do echo - %%G
set /p Name=Enter a target: 
for %%I in ("%Name%") do (set Input=%%~nI)
goto :CheckInput

:FoundInput
::*****************************************************************************
:: PROJECT SETTINGS
::*****************************************************************************

:: Project name (will be use for output filename)
set ProjName=s_target
if not "%2" == "" (
	set ProjName=%ProjName%_%2
)
if not "%3" == "" (
	set ProjName=%ProjName%_%3K
)

:: Project modules to build (use ProjName if not defined)
set ProjModules=s_target

:: Project segments base name
set ProjSegments=s_target

:: List of library modules to build
set LibModules=system,bios,vdp,print,input,memory

:: MSX version:
:: - 1		MSX 1
:: - 2		MSX 2
:: - 2P		MSX 2+
:: - TR		MSX TurboR
:: - 12		MSX 1 or 2
set Machine=1

:: Target:
:: - BIN			.bin	BASIC binary program (8000h~)
:: - ROM_8K			.rom	8KB ROM in page 1 (4000h ~ 5FFFh)
:: - ROM_8K_P2		.rom	8KB ROM in page 2 (8000h ~ 9FFFh)
:: - ROM_16K		.rom	16KB ROM in page 1 (4000h ~ 7FFFh)
:: - ROM_16K_P2		.rom	16KB ROM in page 2 (8000h ~ BFFFh)
:: - ROM_32K		.rom	32KB ROM in page 1-2 (4000h ~ BFFFh)
:: - ROM_48K		.rom	48KB ROM in page 0-2 (0000h ~ BFFFh). Pages 1-2 visible at start
:: - ROM_48K_ISR	.rom	48KB ROM in page 0-2 (0000h ~ BFFFh). Pages 0-2 visible at start
:: - ROM_64K		.rom	64KB ROM in page 0-3 (0000h ~ FFFFh). Pages 1-2 visible at start
:: - ROM_64K_ISR	.rom	64KB ROM in page 0-3 (0000h ~ FFFFh). Pages 0-2 visible at start
:: - ROM_ASCII8		.rom	128KB ROM using ASCII-8 mapper
:: - ROM_ASCII16	.rom	128KB ROM using ASCII-16 mapper
:: - ROM_KONAMI		.rom	128KB ROM using Konami mapper (8KB segments)
:: - ROM_KONAMI_SCC	.rom	128KB ROM using Konami SCC mapper (8KB segments)
:: - DOS1			.com	MSX-DOS 1 program (0100h~) No direct acces to Main-ROM
:: - DOS2			.com	MSX-DOS 2 program (0100h~) No direct acces to Main-ROM
:: - DOS2_ARG		.com	[WIP] MSX-DOS 2 program (using command line arguments ; 0100h~) No direct acces to Main-ROM. 
set Target=%Input%
:: Mapper size
set ROMSize=%3
:: Install BDOS driver for ROM program? (0=false, 1=true)
set InstallBDOS=1
:: Use banked call and add trampoline functions (0=false, 1=true)
set BankedCall=1
:: Overwrite RAM starting address
set ForceRamAddr=

:: Set debug flag (0=false, 1=true)
set Debug=1
:: Optim:
:: - Default
:: - Speed
:: - Size
set Optim=Size
:: Additionnal compilation flag
set CompileOpt=
:: Skip file if compile data is newer than the source (0=false, 1=true)
set CompileSkipOld=0
:: Verbose mode (0=false, 1=true)
set Verbose=0

::*****************************************************************************
:: EMULATOR SETING
::*****************************************************************************

:: Emulator options: 0 or 1
set EmulMachine=1
set Emul60Hz=0
set EmulFullScreen=0
set EmulMute=0
set EmulDebug=0
:: Emulator extra parameters to be add to command-line
set EmulExtraParam=

::*****************************************************************************
:: BUILD STEPS
::*****************************************************************************
set DoClean=0
set DoCompile=1
set DoMake=1
set DoPackage=1
set DoDeploy=1
set DoRun=0

::*****************************************************************************
:: START BUILD
::*****************************************************************************
call %LibDir%\script\build.cmd

if %DoPause%==1 pause
