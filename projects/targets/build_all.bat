@echo off
rem __________________________
rem ██▀█▀██▀▀▀█▀▀█▀█  ▄▄▄ ▄▄  │   ▄▄▄                ▄▄      
rem █  ▄ █▄ ▀██▄ ▀▄█ ██   ██  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
rem █  █ █▀▀ ▄█  █ █ ▀█▄█ ██▄▄│  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
rem ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀──────────┘                 ▀▀
rem  Build all targets
rem ───────────────────────────────────────────────────────────────────────────
setlocal EnableDelayedExpansion
	
set Formats=BIN,DOS1,DOS2,ROM_8K,ROM_8K_P2,ROM_16K,ROM_16K_P2,ROM_32K,ROM_48K,ROM_48K_ISR,ROM_64K,ROM_64K_ISR
set Mappers=ROM_ASCII8,ROM_ASCII16,ROM_KONAMI,ROM_KONAMI_SCC
set Unsupported=ROM_32K_P0,DOS2_ARG

for %%G in (%Formats%) do (

	rem ---- Build target ----
	call build %%G

	rem ---- Rename output file to prevent overwriting ----
	call :RenameOutput %%G
)

REM rem ---------------------------------------------------------------------------
REM rem  ASCII-8 mapped ROM
REM for %%G in (64,128,256,512,1024,2048) do (
	REM rem ---- Build target ----
	REM call build ROM_ASCII8 %%G

	REM rem ---- Rename output file to prevent overwriting ----
	REM call :RenameOutput ROM_ASCII8_%%GK
REM )

REM rem ---------------------------------------------------------------------------
REM rem  ASCII-16 mapped ROM
REM for %%G in (64,128,256,512,1024,2048,4096) do (
	REM rem ---- Build target ----
	REM call build ROM_ASCII16 %%G

	REM rem ---- Rename output file to prevent overwriting ----
	REM call :RenameOutput ROM_ASCII16_%%GK
REM )

REM rem ---------------------------------------------------------------------------
REM rem  Konami mapped ROM
REM for %%G in (64,128,256,512,1024,2048) do (
	REM rem ---- Build target ----
	REM call build ROM_KONAMI %%G

	REM rem ---- Rename output file to prevent overwriting ----
	REM call :RenameOutput ROM_KONAMI_%%GK
REM )

REM rem ---------------------------------------------------------------------------
REM rem  Konami-SCC mapped ROM
REM for %%G in (64,128,256,512,1024,2048) do (
	REM rem ---- Build target ----
	REM call build ROM_KONAMI_SCC %%G

	REM rem ---- Rename output file to prevent overwriting ----
	REM call :RenameOutput ROM_KONAMI_SCC_%%GK
REM )

goto :EOF

:RenameOutput
	set Temp=%1
	set Type=%Temp:~0,3%
	if /I !Type!==bin (
		echo Rename %ProjDir%\emul\dsk\%ProjName%.dsk to %ProjName%_%1.dsk 
		if exist %ProjDir%\emul\dsk\%ProjName%_%1.dsk ( del /Q %ProjDir%\emul\dsk\%ProjName%_%1.dsk )
		rename %ProjDir%\emul\dsk\%ProjName%.dsk %ProjName%_%1.dsk 
	)
	if /I !Type!==rom (
		echo Rename %ProjDir%\emul\rom\%ProjName%.rom to %ProjName%_%1.rom 
		if exist %ProjDir%\emul\rom\%ProjName%_%1.rom ( del /Q %ProjDir%\emul\rom\%ProjName%_%1.rom )
		rename %ProjDir%\emul\rom\%ProjName%.rom %ProjName%_%1.rom
	)
	if /I !Type!==dos (
		echo Rename %ProjDir%\emul\dsk\%ProjName%.dsk to %ProjName%_%1.dsk 
		if exist %ProjDir%\emul\dsk\%ProjName%_%1.dsk ( del /Q %ProjDir%\emul\dsk\%ProjName%_%1.dsk )
		rename %ProjDir%\emul\dsk\%ProjName%.dsk %ProjName%_%1.dsk 
	)
	exit /B 0
	
:EOF
