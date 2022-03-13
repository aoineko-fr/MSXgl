::██▀▀█▀▀███▀▀▀▀▀▀▀███▀▀█▀▀▀▀▀▀▀▀█
::██  ▀  ██   ▄▄▄▄  ▀  ▄█ ▄▀▀ █  █
::█  ▄ ▄  ▀▀▀   █▀  ▄  ▀█ ▀▄█ █▄ █
::█▄▄█▄█▄▄▄▄▄▄▄██▄▄███▄▄█▄▄▄▄▄▄▄▄█
:: by Guillaume 'Aoineko' Blanchard under CC BY-AS license
@echo off

::─────────────────────────────────────────────────────────────────────────────
:: Build Tool default configuration
::─────────────────────────────────────────────────────────────────────────────

::*****************************************************************************
:: DIRECTORIES SETTINGS
::*****************************************************************************
set ProjDir=.
REM set ProjDir=%~d1%~p1
set OutDir=%ProjDir%\out
set LibDir=%ProjDir%\..\..\engine
set ToolsDir=%ProjDir%\..\..\tools

::*****************************************************************************
:: TOOLS SETTINGS
::*****************************************************************************
set Compiler=%ToolsDir%\sdcc\bin\sdcc.exe
set Assembler=%ToolsDir%\sdcc\bin\sdasz80.exe
set Linker=%ToolsDir%\sdcc\bin\sdcc.exe
set Hex2Bin=%ToolsDir%\build\Hex2bin\hex2bin.exe
set FillFile=%ToolsDir%\build\MakeROM\fillfile.exe
set MSXDOS=%ToolsDir%\build\MSXDOS
set DskTool=%ToolsDir%\build\DskTool\dsktool.exe
set Emulator=
REM set Emulator=%ToolsDir%\OpenMSX\openmsx.exe
REM set Emulator=%ToolsDir%\Emulicious\Emulicious.exe
REM set Emulator=%ToolsDir%\BlueMSX\blueMSX.exe
REM set Emulator=%ToolsDir%\MEISEI\meisei.exe
REM set Emulator=%ToolsDir%\fMSX\fMSX.exe
REM set Emulator=%ToolsDir%\RuMSX\MSX.exe
set Debugger=
REM set Debugger=%ToolsDir%\OpenMSX\Debugger\openmsx-debugger.exe

::*****************************************************************************
:: PROJECT SETTINGS
::*****************************************************************************

:: Project name (will be use for output filename)
set ProjName=
:: Project modules to build (use ProjName if not defined)
set ProjModules=
:: List of library modules to build
set LibModules=system,bios,vdp,print,input,memory
:: Additional sources
set AddSources=

:: MSX version:
:: - 1		MSX 1
:: - 2		MSX 2
:: - 2P		MSX 2+
:: - TR		MSX TurboR
:: - 12		MSX 1/2 (working on MSX1 but allow to use MSX2 feature)
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
set Target=
:: ROM mapper size (from 64 to 4096). Must be a multiple of 8 or 16 depending on the mapper type
set ROMSize=
:: Allow BDOS driver installation for ROM target
set InstallBDOS=0
:: Overwrite RAM starting address
set ForceRamAddr=

:: Set debug flag
set Debug=0
:: Assembler code optimizer
:: - None
:: - PeepHole	SDCC otpimizer
:: - MDL		MDL z80 otpimizer
set AsmOptim=PeepHole
:: Optim:
:: - Default
:: - Speed
:: - Size
set Optim=Speed
:: Additionnal compilation flag
set CompileOpt=
:: Skip file if compile data is newer than the source 
set CompileSkip=0
:: Verbose mode: 0 or 1
set Verbose=0

::*******************************************************************************
:: EMULATOR SETING
::*******************************************************************************

:: Emulator options: 0 or 1
set EmulMachine=1
set Emul60Hz=0
set EmulFullScreen=0
set EmulMute=0
set EmulDebug=0
set EmulSCC=0
set EmulMSXMusic=0
set EmulMSXAudio=0
:: Emulator extra parameters to be add to command-line
set EmulExtraParam=

::*******************************************************************************
:: BUILD STEPS
::*******************************************************************************
set DoClean=0
set DoCompile=1
set DoMake=1
set DoPackage=1
set DoDeploy=1
set DoRun=0