:: ____________________________
:: ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
:: ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
:: █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
:: ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
::  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
::────────────────────────────────────────────────────────────────────
@echo off

IF NOT DEFINED MSXGL_PATH (
    set MSXGL_PATH=..\..
)

cls

%MSXGL_PATH%\tools\build\Node\node.exe %MSXGL_PATH%\engine\script\js\build.js %1 %2 %3 %4 %5 %6 %7 %8 %9