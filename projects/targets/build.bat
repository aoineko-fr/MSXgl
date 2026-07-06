:: ____________________________
:: ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
:: ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
:: █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
:: ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
::  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
::─────────────────────────────────────────────────────────────────────────────
@echo off

::*****************************************************************************
:: PROJECT SETTINGS
::*****************************************************************************

IF NOT DEFINED MSXGL_PATH (
    set MSXGL_PATH=..\..
)

set ARGS=target^=%1 projname^=s_target

:: Project name (will be use for output filename)
if not "%2" == "" set ARGS=%ARGS%_%2

:: Mapper size
if not "%3" == "" set ARGS=%ARGS% romsize^=%3

:: Install BDOS driver for ROM program? (0=false, 1=true)
if "%4" == "BDOS" set ARGS=%ARGS% delay

:: Set RAM in slot 0 and install ISR there (0=false, 1=true)
if "%4" == "RAMISR" set ARGS=%ARGS% ramisr

:: Set RAM in slot 0 and install ISR there (0=false, 1=true)
if "%4" == "RAMSEG" set ARGS=%ARGS% ramseg

cls
%MSXGL_PATH%\tools\build\Node\node.exe %MSXGL_PATH%\engine\script\js\build.js %ARGS% %5 %6 %7 %8 %9