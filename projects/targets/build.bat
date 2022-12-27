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

set ARGS=target^=%1 projname^=s_target

:: Project name (will be use for output filename)
if not "%2" == "" set ARGS=%ARGS%_%2

:: Mapper size
if not "%3" == "" set ARGS=%ARGS% romsize^=%3

:: Install BDOS driver for ROM program? (0=false, 1=true)
if "%4" == "BDOS" set ARGS=%ARGS% delay

:: Set RAM in slot 0 and install ISR there (0=false, 1=true)
if "%4" == "RAMISR" set ARGS=%ARGS% ramisr

cls
..\..\tools\build\Node\node.exe ..\..\engine\script\js\build.js %ARGS%