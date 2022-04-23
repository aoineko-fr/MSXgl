@echo off
:: ____________________________
:: ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
:: ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
:: █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
:: ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
::  Build all targets
::─────────────────────────────────────────────────────────────────────────────
setlocal EnableDelayedExpansion
	
set Mappers=ROM_ASCII8,ROM_ASCII16,ROM_KONAMI,ROM_KONAMI_SCC

::-----------------------------------------------------------------------------
:: BASIC and MSX-DOS
set Formats=BIN,DOS1,DOS2
set Unsupported=DOS2_ARG
for %%G in (%Formats%) do (
	call build %%G
)

REM ::-----------------------------------------------------------------------------
REM :: Plain ROM
REM set Formats=ROM_8K,ROM_8K_P2,ROM_16K,ROM_16K_P2,ROM_32K,ROM_48K,ROM_48K_ISR,ROM_64K,ROM_64K_ISR
REM set Unsupported=ROM_32K_P0
REM for %%G in (%Formats%) do (
	REM call build %%G %%G
REM )

REM ::-----------------------------------------------------------------------------
REM :: ASCII-8 mapped ROM
REM for %%G in (64,128,256,512,1024,2048) do (
	REM call build ROM_ASCII8 ROM_ASCII8 %%G
REM )

REM ::-----------------------------------------------------------------------------
REM :: ASCII-16 mapped ROM
REM for %%G in (64,128,256,512,1024,2048,4096) do (
	REM call build ROM_ASCII16 ROM_ASCII16 %%G
REM )

REM ::-----------------------------------------------------------------------------
REM :: Konami mapped ROM
REM for %%G in (64,128,256,512,1024,2048) do (
	REM call build ROM_KONAMI ROM_KONAMI %%G
REM )

REM ::-----------------------------------------------------------------------------
REM :: Konami-SCC mapped ROM
REM for %%G in (64,128,256,512,1024,2048) do (
	REM call build ROM_KONAMI_SCC ROM_KONAMI_SCC %%G
REM )
	
:EOF