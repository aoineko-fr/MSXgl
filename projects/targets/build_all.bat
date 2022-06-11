:: ____________________________
:: ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
:: ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
:: █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
:: ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
::  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
::─────────────────────────────────────────────────────────────────────────────
::  Build all targets
::─────────────────────────────────────────────────────────────────────────────
@echo off

setlocal EnableDelayedExpansion
	
set Mappers=ROM_ASCII8,ROM_ASCII16,ROM_KONAMI,ROM_KONAMI_SCC

::-----------------------------------------------------------------------------
:: BASIC and MSX-DOS
set Formats=BIN,DOS1,DOS2
set Unsupported=DOS2_ARG
for %%G in (%Formats%) do (
	call build %%G
)

::-----------------------------------------------------------------------------
:: Plain ROM
set Formats=ROM_8K,ROM_8K_P2,ROM_16K,ROM_16K_P2,ROM_32K,ROM_48K,ROM_48K_ISR,ROM_64K,ROM_64K_ISR
set Unsupported=ROM_32K_P0
for %%G in (%Formats%) do (
	call build %%G %%G
)

::-----------------------------------------------------------------------------
:: ASCII-8 mapped ROM
for %%G in (64,128,256,512,1024,2048) do (
	call build ROM_ASCII8 ROM_ASCII8 %%G
)

::-----------------------------------------------------------------------------
:: ASCII-16 mapped ROM
for %%G in (64,128,256,512,1024,2048,4096) do (
	call build ROM_ASCII16 ROM_ASCII16 %%G
)

::-----------------------------------------------------------------------------
:: Konami mapped ROM
for %%G in (64,128,256,512,1024,2048) do (
	call build ROM_KONAMI ROM_KONAMI %%G
)

::-----------------------------------------------------------------------------
:: Konami-SCC mapped ROM
for %%G in (64,128,256,512,1024,2048) do (
	call build ROM_KONAMI_SCC ROM_KONAMI_SCC %%G
)
	
:EOF