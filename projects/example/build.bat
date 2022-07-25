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
:: TOOLS SETTINGS
::*****************************************************************************
set Emulator=%ToolsDir%\OpenMSX\openmsx.exe
REM set Emulator=%ToolsDir%\Emulicious\Emulicious.exe
REM set Emulator=%ToolsDir%\BlueMSX\blueMSX.exe
REM set Emulator=%ToolsDir%\MEISEI\meisei.exe
REM set Emulator=%ToolsDir%\fMSX\fMSX.exe
REM set Emulator=%ToolsDir%\RuMSX\MSX.exe

::*****************************************************************************
:: PROJECT SETTINGS
::*****************************************************************************

:: Project name (will be use for output filename)
set ProjName=example
:: Project modules to build (use ProjName if not defined)
set ProjModules=%ProjName%
:: List of modules to link
SET LibModules=system,bios,vdp,print,input,memory,game,game_pawn,math,string

:: MSX machine version:
:: - 1		MSX 1
:: - 2		MSX 2
:: - 12		MSX 1 or 2 (dual support)
:: - 2K		Korean MSX 2 (SC9 support)
:: - 2P		MSX 2+
:: - TR		MSX Turbo-R
:: - 3		MSX 3 (reserved)
set Machine=1
:: Program media target:
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
set Target=ROM_32K
:: ROM mapper size (from 64 to 4096). Must be a multiple of 8 or 16 depending on the mapper type
set ROMSize=

:: Install BDOS driver for ROM program? (0=false, 1=true)
REM set InstallBDOS=0
:: Set RAM in slot 0 and install ISR there (0=false, 1=true)
REM set InstallRAMISR=0
:: Use banked call and trampoline functions (0=false, 1=true)
REM set BankedCall=0
:: Overwrite RAM starting address (e.g. 0xE0000 for 8K RAM machine)
REM set ForceRamAddr=
:: Data to copy to disk (comma separated list)
REM set DiskFiles=

::*******************************************************************************
:: MAKE SETTINGS
::*******************************************************************************

:: Use static MSXgl library (0=false, 1=true)
REM set BuildLibrary=0
:: Set debug flag (0=false, 1=true)
REM set Debug=0
:: Assembler code optimizer
:: - None
:: - PeepHole	SDCC otpimizer
:: - MDL		MDL z80 otpimizer
REM set AsmOptim=None
:: Optim:
:: - Default
:: - Speed
:: - Size
REM set Optim=Speed
:: Additionnal compilation flag
REM set CompileOpt=
:: Skip file if compile data is newer than the (0=false, 1=true)
set CompileSkipOld=1
:: Compile verbose mode (0=false, 1=true)
REM set Verbose=0
:: Update build version header file
set BuildVersion=1

::*******************************************************************************
:: EMULATOR SETINGS
::*******************************************************************************

:: Emulator options: 0 or 1
set EmulMachine=0
REM set Emul60Hz=0
REM set EmulFullScreen=0
REM set EmulMute=0
REM set EmulDebug=0
:: Emulator extensions: 0 or 1
REM set EmulSCC=0
REM set EmulMSXMusic=0
REM set EmulMSXAudio=0
REM set EmulPSG2=0
REM set EmulV9990=0
:: Emulator extra parameters to be add to command-line (emulator sotfware specific)
REM set EmulExtraParam=

::*******************************************************************************
:: BUILD STEPS
::*******************************************************************************

set DoClean=0
set DoCompile=1
set DoMake=1
set DoPackage=1
set DoDeploy=1
set DoRun=1

::*****************************************************************************
:: START BUILD
::*****************************************************************************
call %LibDir%\script\build.cmd
