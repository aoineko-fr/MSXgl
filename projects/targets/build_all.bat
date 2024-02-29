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

::-----------------------------------------------------------------------------
:: BASIC and MSX-DOS
set Formats=BIN_DISK,BIN_TAPE,DOS0,DOS1,DOS2
for %%G in (%Formats%) do (
	call build %%G
)
call build BIN_USR USR 0

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
	call build ROM_ASCII8 ROM_ASCII8_%%GK %%G
)

::-----------------------------------------------------------------------------
:: ASCII-16 mapped ROM
for %%G in (64,128,256,512,1024,2048,4096) do (
	call build ROM_ASCII16 ROM_ASCII16_%%GK %%G
)

::-----------------------------------------------------------------------------
:: Konami mapped ROM
for %%G in (64,128,256,512,1024,2048) do (
	call build ROM_KONAMI ROM_KONAMI_%%GK %%G
)

::-----------------------------------------------------------------------------
:: Konami-SCC mapped ROM
for %%G in (64,128,256,512,1024,2048) do (
	call build ROM_KONAMI_SCC ROM_KONAMI_SCC_%%GK %%G
)

::-----------------------------------------------------------------------------
:: NEO-8 mapped ROM
for %%G in (512, 1024, 2048, 4096, 8192, 16384, 32768) do (
	call build ROM_NEO8 ROM_NEO8_%%GK %%G
)

::-----------------------------------------------------------------------------
:: NEO-16 mapped ROM
for %%G in (512, 1024, 2048, 4096, 8192, 16384, 32768, 65536) do (
	call build ROM_NEO16 ROM_NEO16_%%GK %%G
)

::-----------------------------------------------------------------------------
:: BDOS
call build ROM_32K ROM_32K_BDOS 0 BDOS
call build ROM_48K ROM_48K_BDOS 0 BDOS
call build ROM_ASCII8 ROM_ASCII8_128K_BDOS 128 BDOS

::-----------------------------------------------------------------------------
:: RAM ISR
call build ROM_32K ROM_32K_RAMISR 0 RAMISR
call build ROM_48K ROM_48K_RAMISR 0 RAMISR
call build ROM_ASCII8 ROM_ASCII8_128K_RAMISR 128 RAMISR

:EOF