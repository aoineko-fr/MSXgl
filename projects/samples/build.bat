::██▀▀█▀▀██▀▀▀▀▀▀▀███▀▀█▀▀▀▀▀▀▀▀█
::██  ▀  █▄  ▄▄▄▄  ▀  ▄█ ▄▀▀ █  █
::█  ▄ ▄  ▀▀  ▀█▀  ▄  ▀█ ▀▄█ █▄ █
::█▄▄█▄█▄▄▄▄▄▄██▄▄███▄▄█▄▄▄▄▄▄▄▄█
:: by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
::─────────────────────────────────────────────────────────────────────────────
@echo off

::─────────────────────────────────────────────────────────────────────────────
:: Build Tool configuration
::─────────────────────────────────────────────────────────────────────────────
call ..\default_config.cmd %0

::*****************************************************************************
:: PROJECT SELECTION MENU
::*****************************************************************************
set Pause=0
set Input=%~n1

:CheckInput
if exist %Input%.c goto :FoundInput
cls
set Pause=1
echo No valide sample selected...
echo Available samples:
for /R .\ %%G in (*.c) do call :DisplayFilter %%~nG
set /p Name=Enter a sample: 
for %%I in ("%Name%") do (set Input=%%~nI)
goto :CheckInput

:DisplayFilter
	:: Setup variables
	set name=%1
	set ext=%name:~-3%

	:: Skip ROM mapper segments
	if /I %ext%==_b0 exit /b
	if /I %ext%==_b1 exit /b
	if /I %ext%==_b2 exit /b
	if /I %ext%==_b3 exit /b
	
	:: Do display
	echo - %1
	exit /b
	
:FoundInput
::*****************************************************************************
:: TOOLS SETTINGS
::*****************************************************************************
REM set Emulator=%ToolsDir%\OpenMSX\openmsx.exe
REM set Emulator=%ToolsDir%\Emulicious\Emulicious.exe
REM set Emulator=%ToolsDir%\BlueMSX\blueMSX.exe
REM set Emulator=%ToolsDir%\MEISEI\meisei.exe
REM set Emulator=%ToolsDir%\fMSX\fMSX.exe
REM set Emulator=%ToolsDir%\RuMSX\MSX.exe

::*****************************************************************************
:: PROJECT SETTINGS
::*****************************************************************************

:: Project name (will be use for output filename)
set ProjName=%Input%
:: Project modules to build (use ProjName if not defined)
set ProjModules=%ProjName%
:: Project segments base name
set ProjSegments=%ProjName%
:: List of library modules to build
set LibModules=system,bios,vdp,print,input,memory,math,draw

:: MSX version:
:: - 1		MSX 1
:: - 2		MSX 2
:: - 2P		MSX 2+
:: - TR		MSX TurboR
:: - 12		MSX 1 or 2
set Machine=2
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
:: - ROM_ASCII8		.rom	Mega-ROM using ASCII-8 mapper
:: - ROM_ASCII16	.rom	Mega-ROM using ASCII-16 mapper
:: - ROM_KONAMI		.rom	Mega-ROM using Konami mapper (8KB segments)
:: - ROM_KONAMI_SCC	.rom	Mega-ROM using Konami SCC mapper (8KB segments)
:: - DOS1			.com	MSX-DOS 1 program (0100h~) No direct acces to Main-ROM
:: - DOS2			.com	MSX-DOS 2 program (0100h~) No direct acces to Main-ROM
:: - DOS2_ARG		.com	[WIP] MSX-DOS 2 program (using command line arguments ; 0100h~) No direct acces to Main-ROM. 
set Target=ROM_32K
if not "%2"=="" set Target=%2
:: ROM mapper size (from 64 to 4096). Must be a multiple of 8 or 16 depending on the mapper type
set ROMSize=
:: Install BDOS driver for ROM program? (0=false, 1=true)
set InstallBDOS=0
:: Use banked call (and trampoline functions)
set BankedCall=0
:: Overwrite RAM starting address
set ForceRamAddr=

:: Set debug flag (0=false, 1=true)
set Debug=1
:: Optim:
:: - Default
:: - Speed
:: - Size
set Optim=Speed
:: Additionnal compilation flag
set CompileOpt=
:: Skip file if compile data is newer than the source (0=false, 1=true)
set CompileSkipOld=0
:: Verbose mode (0=false, 1=true)
set Verbose=0

:: Emulator options (0=false, 1=true)
set EmulMachine=0
REM set Emul60Hz=0
REM set EmulFullScreen=0
REM set EmulMute=0
REM set EmulDebug=0
REM set EmulSCC=0
REM set EmulMSXMusic=0
REM set EmulMSXAudio=0
:: Emulator extra parameters to be add to command-line
REM set EmulExtraParam=

:: Check for sample specific parameters
if exist %ProjName%.cmd call %ProjName%.cmd

::*****************************************************************************
:: BUILD STEPS
::*****************************************************************************
REM set DoClean=0
REM set DoCompile=1
REM set DoMake=1
REM set DoPackage=1
REM set DoDeploy=1
REM set DoRun=0

::*****************************************************************************
:: START BUILD
::*****************************************************************************
call %LibDir%\script\build.cmd

if %Pause%==1 pause
