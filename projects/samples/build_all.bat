@echo off
:: ____________________________
:: ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
:: ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
:: █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
:: ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
::  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
::─────────────────────────────────────────────────────────────────────────────
setlocal EnableDelayedExpansion

for %%G in (*.c) do call :BuildFilter %%~nG %1

exit /b 0

:BuildFilter
	:: Setup variables
	set name=%1
	set ext=%name:~-3%

	:: Skip ROM mapper segments
	if /I %ext%==_b0 exit /b
	if /I %ext%==_b1 exit /b
	if /I %ext%==_b2 exit /b
	if /I %ext%==_b3 exit /b
	
	:: Do build
	call build %1 %2
	REM echo %1
	exit /b
