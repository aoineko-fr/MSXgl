::██▀▀█▀▀███▀▀▀▀▀▀▀███▀▀█▀▀▀▀▀▀▀▀█
::██  ▀  ██   ▄▄▄▄  ▀  ▄█ ▄▀▀ █  █
::█  ▄ ▄  ▀▀▀   █▀  ▄  ▀█ ▀▄█ █▄ █
::█▄▄█▄█▄▄▄▄▄▄▄██▄▄███▄▄█▄▄▄▄▄▄▄▄█
:: by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
@echo off

rem ***************************************************************************
rem  CHECK BASE PARAMETERS
rem ***************************************************************************

:: Check MSX version
if /I %Machine%==1			( echo » Machine: MSX 1
) else if /I %Machine%==2	( echo » Machine: MSX 2
) else if /I %Machine%==2P	( echo » Machine: MSX 2+
) else if /I %Machine%==TR	( echo » Machine: MSX turbo R
) else if /I %Machine%==12	( echo » Machine: MSX 1/2
) else (
	echo %RED%Error: Unknow MSX Version%RESET%
	exit /B 10
)

:: Check project name
if not defined ProjName (
	echo %RED%Error: Invalide project name [%ProjName%]%RESET%
	exit /B 20
)

:: Check project modules
if not defined ProjModules (
	echo %YELLOW%Warning: ProjModules not defined. Adding %ProjName% to build list.%RESET%
	set ProjModules=%ProjName%
)

rem ***************************************************************************
rem  CHECK TOOLS PATH
rem ***************************************************************************

:: Check binary tools
if not exist %Compiler% (
	echo %RED%Error: Invalide path to C Compiler [%Compiler%]%RESET%
	exit /B 30
)
if not exist %Assembler% (
	echo %RED%Error: Invalide path to Assembler [%Assembler%]%RESET%
	exit /B 40
)
if not exist %Linker% (
	echo %RED%Error: Invalide path to Linker [%Linker%]%RESET%
	exit /B 40
)
if not exist %Hex2Bin% (
	echo %RED%Error: Invalide path to Hex2Bin [%Hex2Bin%]%RESET%
	exit /B 50
)

:: ROM specific tools
if /I %Ext%==rom (
	if not exist %FillFile% (
		echo %RED%Error: Invalide path to FillFile [%FillFile%]%RESET%
		exit /B 60
	)
)

:: BASIC/MSX-DOS specific tools
if /I not %Ext%==rom (
	if not exist %DskTool% (
		echo %YELLOW%Warning: Invalide path to DskTool [%DskTool%]%RESET%
		echo Only programs in ROM format will be testable with most emulators 
	)
)

:: MSX-DOS specific tools
if /I %Ext%==com (
	if not exist %MSXDOS% (
		echo %YELLOW%Warning: Invalide path to MSX-DOS system files [%MSXDOS%]%RESET%
		echo Program will not be testable with emualtor
	)
)

:: Emulator specific tools
if %DoRun%==1 (
	if not exist %Emulator% (
		echo %YELLOW%Warning: Invalide path to Emulator [%Emulator%]%RESET%
	)

	if %EmulDebug%==1 (
		if not exist %Debugger% (
			echo %YELLOW%Warning: Invalide path to Debugger [%Debugger%]%RESET%
		)
	)
)

rem ***************************************************************************
rem  CHECK BUILD STEPS
rem ***************************************************************************

if not defined DoClean (
	echo %YELLOW%Warning: 'DoClean' buils step not defined. This step will be disable. %RESET%
	set DoClean=0
)

if not defined DoCompile (
	echo %YELLOW%Warning: 'DoCompile' buils step not defined. This step will be disable. %RESET%
	set DoCompile=0
)

if not defined DoMake (
	echo %YELLOW%Warning: 'DoMake' buils step not defined. This step will be disable. %RESET%
	set DoMake=0
)

if not defined DoPackage (
	echo %YELLOW%Warning: 'DoPackage' buils step not defined. This step will be disable. %RESET%
	set DoPackage=0
)

if not defined DoDeploy (
	echo %YELLOW%Warning: 'DoDeploy' buils step not defined. This step will be disable. %RESET%
	set DoDeploy=0
)
if not defined DoRun (
	echo %YELLOW%Warning: 'DoRun' buils step not defined. This step will be disable. %RESET%
	set DoRun=0
)