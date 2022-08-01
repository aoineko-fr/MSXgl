:: ____________________________
:: ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄       ▄  ▄▄    ▄▄   ▄▄▄▄           ▄▄ 
:: ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██ █ ▄  ██   ▄██    ██  ▄█▀▄ ▄█▀▄ ██ 
:: █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▀█▄█ ██ ▀█▄ ▀▄██    ██  ▀█▄▀ ▀█▄▀ ▀█▄
:: ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
::  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
::─────────────────────────────────────────────────────────────────────────────
@echo off

::***************************************************************************
:: CHECK BASE PARAMETERS
::***************************************************************************

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
	echo %RED%Error: Invalid project name [%ProjName%]%RESET%
	exit /B 20
)

:: Check project modules
if not defined ProjModules (
	echo Note: ProjModules not defined. Adding %ProjName% to build list.
	set ProjModules=%ProjName%
)

:: Project segments base name
if not %MapperSize%==0 (
	if not defined ProjSegments (
		echo Note: ProjSegments not defined. Using %ProjName%.
		set ProjSegments=%ProjName%
	)
)

if not defined InstallBDOS ( set InstallBDOS=0 )
if not defined BankedCall  ( set BankedCall=0 )

::***************************************************************************
:: CHECK TOOLS PATH
::***************************************************************************

:: Check binary tools
if not exist "%Compiler%" (
	echo %RED%Error: Invalid path to C Compiler [%Compiler%]%RESET%
	exit /B 30
)
if not exist "%Assembler%" (
	echo %RED%Error: Invalid path to Assembler [%Assembler%]%RESET%
	exit /B 35
)
if not exist "%Linker%" (
	echo %RED%Error: Invalid path to Linker [%Linker%]%RESET%
	exit /B 40
)
if not exist "%Hex2Bin%" (
	echo %RED%Error: Invalid path to Hex2Bin [%Hex2Bin%]%RESET%
	exit /B 50
)

:: BASIC/MSX-DOS specific tools
if /I not %Ext%==rom (
	if not exist "%DskTool%" (
		echo %YELLOW%Warning: Invalid path to DskTool [%DskTool%]%RESET%
		echo Only programs in ROM format will be testable with most emulators 
	)
)

:: MSX-DOS specific tools
if /I %Ext%==com (
	if not exist "%MSXDOS%" (
		echo %YELLOW%Warning: Invalid path to MSX-DOS system files [%MSXDOS%]%RESET%
		echo Program will not be testable with emulator
	)
)

:: Emulator specific tools
if %DoRun%==1 (
	if not exist "%Emulator%" (
		echo %YELLOW%Warning: Invalid path to Emulator [%Emulator%]%RESET%
		echo Disactivate DoRun option
		set DoRun=0 
	)
)

::***************************************************************************
:: CHECK BUILD STEPS
::***************************************************************************

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
