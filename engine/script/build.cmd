::██▀▀█▀▀███▀▀▀▀▀▀▀███▀▀█▀▀▀▀▀▀▀▀█
::██  ▀  ██   ▄▄▄▄  ▀  ▄█ ▄▀▀ █  █
::█  ▄ ▄  ▀▀▀   █▀  ▄  ▀█ ▀▄█ █▄ █
::█▄▄█▄█▄▄▄▄▄▄▄██▄▄███▄▄█▄▄▄▄▄▄▄▄█
:: by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
::-----------------------------------------------------------------------------
@echo off
"%__APPDIR__%chcp.com" 65001 > nul
title <nul & title MSXgl Build Tool – %ProjName% – %Target%

setlocal EnableDelayedExpansion

::-- Setup text color
set RESET=[0m
set RED=[91m
set GREEN=[92m
set YELLOW=[93m
set BLUE=[94m
set MAGENTA=[95m
set CYAN=[96m
set BG=[44m

cls
echo %BG%
echo ╔═══════════════════════════════════════════════════════════════════════════╗
echo ║                                                                           ║
echo ║  ██▀▀█▀▀███▀▀▀▀▀▀▀███▀▀█▀▀▀▀▀▀▀▀█                                         ║
echo ║  ██  ▀  ██   ▄▄▄▄  ▀  ▄█ ▄▀▀ █  █                                         ║
echo ║  █  ▄ ▄  ▀▀▀   █▀  ▄  ▀█ ▀▄█ █▄ █                                         ║
echo ║  █▄▄█▄█▄▄▄▄▄▄▄██▄▄███▄▄█▄▄▄▄▄▄▄▄█                                         ║
echo ║                                                                           ║
echo ║   ▄▄▄       ▄  ▄▄    ▄▄   ▄▄▄▄           ▄▄                               ║
echo ║   ██▄▀ ██ █ ▄  ██   ▄██    ██  ▄█▀▄ ▄█▀▄ ██                               ║
echo ║   ██▄▀ ▀█▄█ ██ ▀█▄ ▀▄██    ██  ▀█▄▀ ▀█▄▀ ▀█▄                              ║
echo ║                                                                           ║
echo ╚═══════════════════════════════════════════════════════════════════════════╝
echo %RESET%

::=============================================================================
:: BUILD INITIALIZATION
::=============================================================================

::-- Target specific initialization
call %LibDir%\script\setup_target.cmd
if errorlevel 1 goto :Error

::-- Parmaters validation
call %LibDir%\script\check_config.cmd
if errorlevel 1 goto :Error

::_____________________________________________________________________________
::   ▄▄  ▄▄                
::  ██ ▀ ██  ▄███ ▄▀██ ██▀▄
::  ▀█▄▀ ▀█▄ ▀█▄▄ ▀▄██ ██ █
::_____________________________________________________________________________
if %DoClean%==0 goto :NoClean

echo.
echo ┌───────────────────────────────────────────────────────────────────────────┐
echo │ CLEAN                                                                     │
echo └───────────────────────────────────────────────────────────────────────────┘
echo %BLUE%Cleaning...%RESET%

if exist %OutDir% (
	echo Removing %OutDir%...
	rd /S /Q %OutDir% 
)
if exist %ProjDir%\emul (
	echo Removing \emul...
	rd /S /Q %ProjDir%\emul
)

:NoClean

::_____________________________________________________________________________
::   ▄▄                 ▄  ▄▄ 
::  ██ ▀ ▄█▀▄ ▄█▄█ ██▀▄ ▄  ██  ▄███
::  ▀█▄▀ ▀█▄▀ ██ █ ██▀  ██ ▀█▄ ▀█▄▄
::_________________▀▀__________________________________________________________
if %DoCompile%==0 goto :NoCompile

echo.
echo ┌───────────────────────────────────────────────────────────────────────────┐
echo │ COMPILE                                                                   │
echo └───────────────────────────────────────────────────────────────────────────┘
echo %BLUE%Compiling %ProjName% using SDCC...%RESET%

if not exist "%OutDir%" ( md %OutDir% )

::=============================================================================
:: CREATE CTR0 CONFIG FILE
::=============================================================================

echo ; MSX-GL ^| Crt0 config file generated by the Build tool	>  %OutDir%\crt0_config.asm
if defined Mapper (
	echo ROM_MAPPER=%Mapper%		>> %OutDir%\crt0_config.asm
	echo ADDR_BOOT=0x%StartAddr%	>> %OutDir%\crt0_config.asm
)
if %InstallBDOS%==1 ( echo ROM_BDOS=1  >> %OutDir%\crt0_config.asm )
if %BankedCall%==1  ( echo ROM_BCALL=1 >> %OutDir%\crt0_config.asm )

::=============================================================================
:: GENERATE MODULES LIST
::=============================================================================

:: Add crt0 source to build list (it must be the first in the list)
set SrcList=%LibDir%\src\crt0\%Crt0%.asm
set LibList=%OutDir%\%Crt0%.rel 

:: Add project sources to build list
for %%G in (%ProjModules%) do (
	if not exist "%%G.c" (
		echo %RED%Error: Source file %%G.c don't exist%RESET%
		exit /B 100
	)
	set SrcList=!SrcList!,%%G.c
	set LibList=!LibList! %OutDir%\%%~nG.rel
)

:: Add modules sources to build list
echo » Modules: %LibModules%
for %%G in (%LibModules%) do (
	if not exist "%LibDir%\src\%%G.c" (
		echo %RED%Error: Module %%G don't exist%RESET%
		exit /B 110
	)
	set SrcList=!SrcList!,%LibDir%\src\%%G.c
	set LibList=!LibList! %OutDir%\%%~nG.rel
)

:: Additional source files to build
if not defined AddSources goto NoAddSources
for %%G in (%AddSources%) do (
	if not exist "%%G" (
		echo %RED%Error: Additionnal source %%G don't exist%RESET%
		exit /B 120
	)
	set SrcList=!SrcList!,%%G
	set LibList=!LibList! %OutDir%\%%~nG.rel
)
:NoAddSources

::-- Overwrite RAM start address
if defined ForceRamAddr (
	echo » Force RAM address to %ForceRamAddr%
	set RamAddr=%ForceRamAddr%
)

::=============================================================================
:: COMPILE ALL MODULES
::=============================================================================
call %LibDir%\script\compile_all.cmd
if errorlevel 1 goto :Error

::=============================================================================
:: COMPILE MAPPER SEGMENT
::=============================================================================
if %MapperSize%==0 goto :NoMapperCompile

set /A FirstSeg=%FillSize% / %SegSize%
set /A LastSeg=(%MapperSize% / %SegSize%) - 1
set MapperBanks=

echo %BLUE%Search for extra mapper segments to compile [%FirstSeg%-%LastSeg%]...%RESET%
for /L %%I in (%FirstSeg%,1,%LastSeg%) do (
	for %%K in (c,s,asm) do (
		call :toHex %%I hex
		if not %Bank0Addr%==0 (
			if exist %ProjSegments%_s%%I_b0.%%K (
				echo Segment found: %ProjSegments%_s%%I_b0.%%K ^(addr: !hex!%Bank0Addr%^)
				set MapperBanks=!MapperBanks! -Wl-b_SEG%%I^=0x!hex!%Bank0Addr%
				call %LibDir%\script\compile.cmd %ProjSegments%_s%%I_b0.%%K %SegSize% %%I
				set LibList=!LibList! %OutDir%\%ProjSegments%_s%%I_b0.rel
			)
		)
		if not %Bank1Addr%==0 (
			if exist %ProjSegments%_s%%I_b1.%%K (
				echo Segment found: %ProjSegments%_s%%I_b1.%%K ^(addr: !hex!%Bank1Addr%^)
				set MapperBanks=!MapperBanks! -Wl-b_SEG%%I^=0x!hex!%Bank1Addr%
				call %LibDir%\script\compile.cmd %ProjSegments%_s%%I_b1.%%K %SegSize% %%I
				set LibList=!LibList! %OutDir%\%ProjSegments%_s%%I_b1.rel
			)
		)
		if not %Bank2Addr%==0 (
			if exist %ProjSegments%_s%%I_b2.%%K (
				echo Segment found: %ProjSegments%_s%%I_b2.%%K ^(addr: !hex!%Bank2Addr%^)
				set MapperBanks=!MapperBanks! -Wl-b_SEG%%I^=0x!hex!%Bank2Addr%
				call %LibDir%\script\compile.cmd %ProjSegments%_s%%I_b2.%%K %SegSize% %%I
				set LibList=!LibList! %OutDir%\%ProjSegments%_s%%I_b2.rel
			)
		)
		if not %Bank3Addr%==0 (
			if exist %ProjSegments%_s%%I_b3.%%K (
				echo Segment found: %ProjSegments%_s%%I_b3.%%K ^(addr: !hex!%Bank3Addr%^)
				set MapperBanks=!MapperBanks! -Wl-b_SEG%%I^=0x!hex!%Bank3Addr%
				call %LibDir%\script\compile.cmd %ProjSegments%_s%%I_b3.%%K %SegSize% %%I
				set LibList=!LibList! %OutDir%\%ProjSegments%_s%%I_b3.rel
			)
		)
	)
)

:NoMapperCompile
:NoCompile

::_____________________________________________________________________________
::  ▄▄   ▄       ▄▄   
::  ██   ▄  ██▀▄ ██▄▀
::  ██▄▄ ██ ██ █ ██ █
::_____________________________________________________________________________
if %DoMake%==0 goto :NoMake

echo.
echo ┌───────────────────────────────────────────────────────────────────────────┐
echo │ LINK                                                                      │
echo └───────────────────────────────────────────────────────────────────────────┘
echo %BLUE%Making %ProjName% using SDCC...%RESET%

%Linker% --version

if %Optim%==Speed ( set LinkOpt=%LinkOpt% --opt-code-speed )
if %Optim%==Size ( set LinkOpt=%LinkOpt% --opt-code-size )
if %Debug%==1 ( set LinkOpt=%LinkOpt% --debug )

set SDCCParam=-mz80 --vc --no-std-crt0 --code-loc 0x%CodeAddr% --data-loc 0x%RamAddr% %LinkOpt% %MapperBanks% %LibList% -o %OutDir%\
echo SDCC %SDCCParam%
%Linker% %SDCCParam%
if errorlevel 1 goto :Error
echo %GREEN%Succeed%RESET%

:NoMake

::_____________________________________________________________________________
::  ▄▄▄            ▄▄                 
::  ██▄▀ ▄▀██ ▄█▀▀ ██▄▀ ▄▀██ ▄▀██ ▄███
::  ██   ▀▄██ ▀█▄▄ ██ █ ▀▄██  ▀██ ▀█▄▄
::____________________________▀▀_______________________________________________
if %DoPackage%==0 goto :NoPackage

echo.
echo ┌───────────────────────────────────────────────────────────────────────────┐
echo │ PACKAGE                                                                   │
echo └───────────────────────────────────────────────────────────────────────────┘
echo %BLUE%Packaging binary...%RESET%

::=============================================================================
:: MSXhex
::=============================================================================

if %MapperSize%==0 (
	set H2BParam=%OutDir%\%Crt0%.ihx -e %Ext% -s 0x%StartAddr% -l %FillSize%
) else (	
	set H2BParam=%OutDir%\%Crt0%.ihx -e %Ext% -s 0x%StartAddr% -l %MapperSize% -b %SegSize%
)

echo HEX2BIN %H2BParam%
%Hex2Bin% %H2BParam% 
if errorlevel 1 goto :Error
echo %GREEN%Succeed%RESET%

:NoPackage

::_____________________________________________________________________________
::  ▄▄▄            ▄▄           
::  ██ █ ▄███ ██▀▄ ██  ▄█▀▄ █ ██
::  ██▄▀ ▀█▄▄ ██▀  ▀█▄ ▀█▄▀  ▀██
::____________▀▀_____________▀▀________________________________________________
if %DoDeploy%==0 goto :NoDeploy

echo.
echo ┌───────────────────────────────────────────────────────────────────────────┐
echo │ DEPLOY                                                                    │
echo └───────────────────────────────────────────────────────────────────────────┘
echo %BLUE%Deploying %Target%...%RESET%

::=============================================================================
:: CREATE OUTPUT DIRECTORY
::=============================================================================
if not exist "%ProjDir%\emul" ( md %ProjDir%\emul )
if /I %Ext%==rom (
	if not exist "%ProjDir%\emul\rom" ( md %ProjDir%\emul\rom )
)
if /I %Ext%==bin (
	if not exist "%ProjDir%\emul\bin" ( md %ProjDir%\emul\bin )
	if not exist "%ProjDir%\emul\dsk" ( md %ProjDir%\emul\dsk )
)
if /I %Ext%==com (
	if not exist "%ProjDir%\emul\dos%DOS%" ( md %ProjDir%\emul\dos%DOS% )
	if /I %DOS%==1 (
		if not exist "%ProjDir%\emul\dos%DOS%\COMMAND.COM"  ( copy /Y %MSXDOS%\COMMAND.COM %ProjDir%\emul\dos%DOS% )
		if not exist "%ProjDir%\emul\dos%DOS%\MSXDOS.SYS"   ( copy /Y %MSXDOS%\MSXDOS.SYS %ProjDir%\emul\dos%DOS% )
	)
	if /I %DOS%==2 (
		if not exist "%ProjDir%\emul\dos%DOS%\COMMAND2.COM" ( copy /Y %MSXDOS%\COMMAND2.COM %ProjDir%\emul\dos%DOS% )
		if not exist "%ProjDir%\emul\dos%DOS%\MSXDOS2.SYS"  ( copy /Y %MSXDOS%\MSXDOS2.SYS %ProjDir%\emul\dos%DOS% )
	)
	if not exist "%ProjDir%\emul\dsk" ( md %ProjDir%\emul\dsk )
)

for %%I in ("%DskTool%") do set DskToolPath=%%~dI%%~pI
for %%J in ("%DskTool%") do set DskToolName=%%~nJ%%~xJ

::-----------------------------------------------------------------------------
if /I %Ext%==bin (
	echo -- Copy %OutDir%\%Crt0%.%Ext% to emul\bin\%ProjName%.%Ext%
	copy /Y %OutDir%\%Crt0%.%Ext% %ProjDir%\emul\bin\%ProjName%.%Ext%
	if errorlevel 1 goto :Error
	echo -- Create emul\bin\autoexec.bas
	echo 10 print"Loading..." > %ProjDir%\emul\bin\autoexec.bas
	echo 20 bload"%ProjName:~0,8%.%Ext%",r >> %ProjDir%\emul\bin\autoexec.bas
	::---- Generate DSK file ----
	if exist %DskTool% (
	
		echo %GREEN%Succeed%RESET%
		echo %BLUE%Generating DSK file...%RESET%
	
		echo -- Temporary copy files to DskTool directory
		copy /Y %ProjDir%\emul\bin\autoexec.bas %DskToolPath%
		copy /Y %ProjDir%\emul\bin\%ProjName%.%Ext% %DskToolPath%

		echo -- Generate .DSK file from autoexec.bas %ProjName%.%Ext%
		echo %DskToolName% a temp.dsk autoexec.bas %ProjName%.%Ext%
		set PrevCD=%cd%
		cd %DskToolPath%
		%DskToolName% a temp.dsk autoexec.bas %ProjName%.%Ext%
		cd !PrevCD!
		
		echo -- Copy DSK file to %ProjDir%\emul\dsk\%Target%_%ProjName%.dsk
		copy /Y %DskToolPath%\temp.dsk %ProjDir%\emul\dsk\%Target%_%ProjName%.dsk

		echo -- Clean temporary files
		del /Q %DskToolPath%\autoexec.bas %DskToolPath%\%ProjName%.%Ext% %DskToolPath%\temp.dsk
	)
)

::-----------------------------------------------------------------------------
if /I %Ext%==rom (
	echo Copy %OutDir%\%Crt0%.%Ext% to emul\rom\%ProjName%.%Ext%
	copy /Y %OutDir%\%Crt0%.%Ext% %ProjDir%\emul\rom\%ProjName%.%Ext%
	if errorlevel 1 goto :Error

	if %Debug%==1 (
		echo Copy symbols files to destination directory
		if exist %OutDir%\%Crt0%.map ( copy /Y %OutDir%\%Crt0%.map %ProjDir%\emul\rom\%ProjName%.map )
		if exist %OutDir%\%Crt0%.noi ( copy /Y %OutDir%\%Crt0%.noi %ProjDir%\emul\rom\%ProjName%.noi )
	)
)

::-----------------------------------------------------------------------------
if /I %Ext%==com (
	echo Copy %OutDir%\%Crt0%.%Ext% to emul\dos%DOS%\%ProjName%.%Ext%
	copy /Y %OutDir%\%Crt0%.%Ext% %ProjDir%\emul\dos%DOS%\%ProjName%.%Ext%
	if errorlevel 1 goto :Error
	echo Create emul\dos%DOS%\autoexec.bat
	if /I %DOS%==1 (
		echo %ProjName% > %ProjDir%\emul\dos%DOS%\autoexec.bat
	)
	if /I %DOS%==2 (
		echo echo Loading... > %ProjDir%\emul\dos%DOS%\autoexec.bat
		echo %ProjName%.%Ext% >> %ProjDir%\emul\dos%DOS%\autoexec.bat
		echo %ProjName% >> %ProjDir%\emul\dos%DOS%\autoexec.bat
	)
	
	if errorlevel 1 goto :Error
	::---- Generate DSK file ----
	if exist %DskTool% (

		echo %GREEN%Succeed%RESET%
		echo %BLUE%Generating DSK file...%RESET%

		if /I %DOS%==1 (
			set FilesList=COMMAND.COM MSXDOS.SYS autoexec.bat %ProjName%.%Ext%
		)
		if /I %DOS%==2 (
			set FilesList=COMMAND2.COM MSXDOS2.SYS autoexec.bat %ProjName%.%Ext%
		)
		
		echo -- Temporary copy files to DskTool directory
		for %%K in (!FilesList!) do ( copy /Y %ProjDir%\emul\dos%DOS%\%%K %DskToolPath% )

		echo -- Generate .DSK file
		echo %DskToolName% a temp.dsk !FilesList!
		set PrevCD=%cd%
		cd %DskToolPath%
		%DskToolName% a temp.dsk !FilesList!
		cd !PrevCD!

		echo -- Copy DSK file to %ProjDir%\emul\dsk\%Target%_%ProjName%.dsk
		copy /Y %DskToolPath%\temp.dsk %ProjDir%\emul\dsk\%Target%_%ProjName%.dsk

		echo -- Clean temporary files
		del /Q %DskToolPath%\temp.dsk
		for %%K in (!FilesList!) do ( del /Q %DskToolPath%\%%K )
	)
)

echo %GREEN%Succeed%RESET%

:NoDeploy

::_____________________________________________________________________________
::  ▄▄▄           
::  ██▄▀ ██ █ ██▀▄
::  ██ █ ▀█▄█ ██ █
::_____________________________________________________________________________
if %DoRun%==0 goto :NoRun

echo.
echo ┌───────────────────────────────────────────────────────────────────────────┐
echo │ RUN                                                                       │
echo └───────────────────────────────────────────────────────────────────────────┘
echo %BLUE%Runing...%RESET%

::=============================================================================
:: EMULATOR
::=============================================================================
call %LibDir%\script\setup_emulator.cmd

:NoRun

echo %GREEN%Build Succeed%RESET%
exit /B 0

:Error

echo %RED%Error: Build Failed with error number %errorlevel%%RESET%
exit /B 666





::_____________________________________________________________________________
::   ▄▄▄      ▄       ▄▄ 
::  ▀█▄  ██▄▀ ▄  ██▀▄ ██▀
::  ▄▄█▀ ██   ██ ██▀  ▀█▄
::_______________▀▀____________________________________________________________

:toHex dec hex -- convert a decimal number to hexadecimal, i.e. -20 to FFFFFFEC or 26 to 0000001A
::             -- dec [in]      - decimal number to convert
::             -- hex [out,opt] - variable to store the converted hexadecimal number in
::Thanks to 'dbenham' dostips forum users who inspired to improve this function
:$created 20091203 :$changed 20110330 :$categories Arithmetic,Encoding
:$source https://www.dostips.com
setlocal ENABLEDELAYEDEXPANSION
set /a dec=%~1
set "hex="
set "map=0123456789ABCDEF"
for /L %%N in (1,1,8) do (
    set /a "d=dec&15,dec>>=4"
    for %%D in (!d!) do set "hex=!map:~%%D,1!!hex!"
)
rem !!!! REMOVE LEADING ZEROS by activating the next line, e.g. will return 1A instead of 0000001A
for /f "tokens=* delims=0" %%A in ("%hex%") do set "hex=%%A"&if not defined hex set "hex=0"
( ENDLOCAL & REM RETURN VALUES
    if "%~2" NEQ "" (set %~2=%hex%) else echo.%hex%
)
exit /b