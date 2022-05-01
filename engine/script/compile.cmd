::██▀▀█▀▀███▀▀▀▀▀▀▀███▀▀█▀▀▀▀▀▀▀▀█
::██  ▀  ██   ▄▄▄▄  ▀  ▄█ ▄▀▀ █  █
::█  ▄ ▄  ▀▀▀   █▀  ▄  ▀█ ▀▄█ █▄ █
::█▄▄█▄█▄▄▄▄▄▄▄██▄▄███▄▄█▄▄▄▄▄▄▄▄█
:: by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
::-----------------------------------------------------------------------------
@echo off
REM %~1 - expands %1 removing any surrounding quotes (")
REM %~f1 - expands %1 to a fully qualified path name
REM %~d1 - expands %1 to a drive letter only
REM %~p1 - expands %1 to a path only
REM %~n1 - expands %1 to a file name only
REM %~x1 - expands %1 to a file extension only
REM %~s1 - expanded path contains short names only
REM %~a1 - expands %1 to file attributes
REM %~t1 - expands %1 to date/time of file
REM %~z1 - expands %1 to size of file

set File=%~f1
set FilePath=%~d1%~p1
set FileName=%~n1
set FileExt=%~x1

if not exist %OutDir% ( md %OutDir% )

::-----------------------------------------------------------------------------
:: Skip file if compile data is newer than the source 
if %CompileSkipOld% == 0 ( goto :NoSkip )

copy %File% %OutDir% /Y >NUL
for /F %%i in ('dir /B /O:D %OutDir%\%FileName%.*') do set NewestFile=%%i
if /I %NewestFile% NEQ %FileName%%FileExt% ( goto :Skip )

:NoSkip

::-----------------------------------------------------------------------------
:: Switch compiler according to file extension
if /I %FileExt%==.c goto :CompileC
if /I %FileExt%==.s goto :CompileASM
if /I %FileExt%==.asm goto :CompileASM
echo %RED%Error: Compile failed with error number %errorlevel%%RESET%
goto :Error

::*****************************************************************************
::* COMPILE C SOURCE                                                          *
::*****************************************************************************
:CompileC

set AddOpt=%CompileOpt%
if /I %Optim%==Speed ( set AddOpt=!AddOpt! --opt-code-speed )
if /I %Optim%==Size ( set AddOpt=!AddOpt! --opt-code-size )
if not "%2"=="" ( set AddOpt=!AddOpt! --codeseg SEG%2 )

REM set AddOpt=!AddOpt! --max-allocs-per-node 100000
REM set AddOpt=!AddOpt! --constseg RODATA

set SDCCParam=-c -mz80 --vc -DTARGET=TARGET_%Target% -DMSX_VERSION=MSX_%Machine% -I%ProjDir% -I%LibDir%\src -I%LibDir%\content !AddOpt! %File% -o %OutDir%\

echo %BLUE%Compiling %1 using SDCC C compiler...%RESET%

echo SDCC !SDCCParam!
%Compiler% !SDCCParam!
if errorlevel 1 ( goto :Error )
goto :Succeed

::*****************************************************************************
::* COMPILE ASSEMBLER SOURCE                                                  *
::*****************************************************************************
:CompileASM

set ASMParam=-o -l -s -I%ProjDir% -I%OutDir% -I%LibDir%\src %File%

echo %BLUE%Compiling %1 using SDASZ80 ASM compiler...%RESET%

echo SDASZ80 !ASMParam!
%Assembler% !ASMParam!
if errorlevel 1 ( goto :Error )
move %FilePath%%FileName%.rel %OutDir%
move %FilePath%%FileName%.lst %OutDir%
move %FilePath%%FileName%.sym %OutDir%
goto :Succeed

::*****************************************************************************
:: FINISH
::*****************************************************************************

:Succeed

echo %GREEN%Succeed%RESET%
exit /B 0

:Skip

echo %YELLOW%Skip compiling %FileName%%FileExt%%RESET% (outdated source)
exit /B 0

:Error

echo %RED%Error: Compile failed with error number %errorlevel%%RESET%
exit /B 1
