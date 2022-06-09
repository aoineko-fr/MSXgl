::██▀▀█▀▀███▀▀▀▀▀▀▀███▀▀█▀▀▀▀▀▀▀▀█
::██  ▀  ██   ▄▄▄▄  ▀  ▄█ ▄▀▀ █  █
::█  ▄ ▄  ▀▀▀   █▀  ▄  ▀█ ▀▄█ █▄ █
::█▄▄█▄█▄▄▄▄▄▄▄██▄▄███▄▄█▄▄▄▄▄▄▄▄█
:: by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
::-----------------------------------------------------------------------------
@echo off

for %%I in ("%Emulator%") do (set EmulatorName=%%~nI)

if "%EmulatorName%"=="" (
	echo %YELLOW%No emulator configured
	exit /B 0
)

echo %BLUE%Starting %EmulatorName% emulator...%RESET%

echo EmulMachine=%EmulMachine%
echo Emul60Hz=%Emul60Hz%
echo EmulFullScreen=%EmulFullScreen%
echo EmulMute=%EmulMute%
echo EmulDebug=%EmulDebug%

set  EmulatorArgs=%EmulExtraParam%

::*****************************************************************************
:: OpenMSX
::*****************************************************************************
:: Doc: https://openmsx.org/manual/commands.html
if /I %EmulatorName%==openmsx (

	echo Note: You can only change frequency by setting the machine

	::---- Add launch options ----
	if %EmulMachine%==1 (
		if /I %Machine%==1 (
			if %Emul60Hz%==1 (
				set EmulatorArgs=!EmulatorArgs! -machine C-BIOS_MSX1_JP
			) else (
				set EmulatorArgs=!EmulatorArgs! -machine C-BIOS_MSX1_EU
			)
		) else if /I %Machine%==2 (
			if %Emul60Hz%==1 (
				set EmulatorArgs=!EmulatorArgs! -machine C-BIOS_MSX2_JP
			) else (
				set EmulatorArgs=!EmulatorArgs! -machine C-BIOS_MSX2_EU
			)
		) else if /I %Machine%==2P (
			if %Emul60Hz%==1 (
				set EmulatorArgs=!EmulatorArgs! -machine C-BIOS_MSX2+_JP
			) else (
				set EmulatorArgs=!EmulatorArgs! -machine C-BIOS_MSX2+_EU
			)
		) else if /I %Machine%==TR (
			echo %RED%Error: No MSX turbo R default machine in OpenMSX%RESET%
			exit /B 500
		) else if /I %Machine%==12 (
			if %Emul60Hz%==1 (
				set EmulatorArgs=!EmulatorArgs! -machine C-BIOS_MSX1_JP
			) else (
				set EmulatorArgs=!EmulatorArgs! -machine C-BIOS_MSX1_EU
			)
		)
	)
	if %EmulFullScreen%==1 ( set EmulatorArgs=!EmulatorArgs! -command "set fullscreen on" )
	if %EmulMute%==1       ( set EmulatorArgs=!EmulatorArgs! -command "set mute on" )

	::---- Add launch program ----
	if /I %Ext%==bin ( set EmulatorArgs=!EmulatorArgs! -diska %ProjDir%\emul\bin )
	if /I %Ext%==rom ( set EmulatorArgs=!EmulatorArgs! -cart %ProjDir%\emul\rom\%ProjName%.rom )
	if /I %Ext%==com ( set EmulatorArgs=!EmulatorArgs! -ext msxdos2 -diska %ProjDir%\emul\dos%DOS%)
	REM if /I %Target%==DOS1 ( 
		REM set EmulatorArgs=!EmulatorArgs! -exta slotexpander -ext Panasonic_FS-FD1A -ext ram64k -diska %ProjDir%\emul\dos%DOS%
	REM ) else 	(
		REM if /I %Ext%==com ( set EmulatorArgs=!EmulatorArgs! -exta slotexpander -ext Panasonic_FS-FD1A -ext msxdos2 -ext ram512k -diska %ProjDir%\emul\dos%DOS% )
	REM )
	REM if /I %Ext%==com ( set EmulatorArgs=!EmulatorArgs! -diska %ProjDir%\emul\dos%DOS% -ext msxdos2 )
	REM if /I %Ext%==bin ( set EmulatorArgs=!EmulatorArgs! -ext ide -hda %ProjDir%\emul\dsk\%ProjName%.dsk )
	REM if /I %Ext%==rom ( set EmulatorArgs=!EmulatorArgs! -cart %ProjDir%\emul\rom\%ProjName%.rom )
	REM if /I %Ext%==com ( set EmulatorArgs=!EmulatorArgs! -ext ide -hda %ProjDir%\emul\dsk\%ProjName%.dsk -ext msxdos2 )
	
	::---- Start emulator ----
	if %EmulDebug%==1 ( start /b %Debugger% )	
)

::*****************************************************************************
:: Emulicious
::*****************************************************************************
:: See ReadMe.txt and Emulicious.ini
if /I %EmulatorName%==emulicious (

	set EmulatorArgs=!EmulatorArgs! -set System=MSX

	::---- Open/close debugger
	if %EmulDebug%==1 (
		set EmulatorArgs=!EmulatorArgs! -set WindowDebuggerOpen=true -set DebuggerSuspendOnOpen=false
	) else (
		set EmulatorArgs=!EmulatorArgs! -set WindowDebuggerOpen=false 
	)

	::---- Add launch options ----
	if %EmulMachine%==1    ( echo %YELLOW%Warning: EmulMachine can't be use with Emulicious%RESET% )
	if %Emul60Hz%==1       ( 
		set EmulatorArgs=!EmulatorArgs! -set MSXPAL=false 
	) else (
		set EmulatorArgs=!EmulatorArgs! -set MSXPAL=true 
	)
	if %EmulFullScreen%==1 ( set EmulatorArgs=!EmulatorArgs! -fullscreen )
	if %EmulMute%==1       ( set EmulatorArgs=!EmulatorArgs! -muted )

	::---- Add launch program ----
	if /I %Ext%==rom ( 
		set EmulatorArgs=!EmulatorArgs! %ProjDir%\emul\rom\%ProjName%.rom 
	) else (
		echo %RED%Error: Emulicious only support ROM format%RESET%
		exit /B 530
	)
)

::*****************************************************************************
:: BlueMSX
::*****************************************************************************
:: Doc: http://www.msxblue.com/manual/commandlineargs_c.htm
if /I %EmulatorName%==bluemsx (

	::---- Add launch options ----
	if %EmulMachine%==1 (
		if /I %Machine%==1 (
			if %Emul60Hz%==1 (
				set EmulatorArgs=!EmulatorArgs! /machine "MSX - Japanese"
			) else (
				set EmulatorArgs=!EmulatorArgs! /machine "MSX"
			)
		) else if /I %Machine%==2 (
			if %Emul60Hz%==1 (
				set EmulatorArgs=!EmulatorArgs! /machine "MSX2 - Japanese"
			) else (
				set EmulatorArgs=!EmulatorArgs! /machine "MSX2"
			)
		) else if /I %Machine%==2P (
			if %Emul60Hz%==1 (
				set EmulatorArgs=!EmulatorArgs! /machine "MSX2+"
			) else (
				set EmulatorArgs=!EmulatorArgs! /machine "MSX2+ - C-BIOS"
				echo Note: 50Hz MSX2+ is C-BIOS based with BlueMSX
			)
		) else if /I %Machine%==TR (
			if %Emul60Hz%==1 (
				set EmulatorArgs=!EmulatorArgs! /machine "MSXturboR"
			) else (
				set EmulatorArgs=!EmulatorArgs! /machine "MSXturboR"
				echo %YELLOW%Warning: MSXturboR is only 60Hz with BlueMSX%RESET%
			)
		) else if /I %Machine%==12 (
			if %Emul60Hz%==1 (
				set EmulatorArgs=!EmulatorArgs! /machine "MSX"
			) else (
				set EmulatorArgs=!EmulatorArgs! /machine "MSX - Japanese"
			)
		)
	)
	if %EmulFullScreen%==1 ( set EmulatorArgs=!EmulatorArgs! /fullscreen )
	if %EmulMute%==1       ( echo %YELLOW%Warning: EmulMute can't be use with BlueMSX%RESET% )

	::---- Add launch program ----
	if /I %Ext%==bin ( set EmulatorArgs=!EmulatorArgs! /diskA %ProjDir%\emul\dsk\%ProjName%.dsk )
	if /I %Ext%==rom ( set EmulatorArgs=!EmulatorArgs! /rom1 %ProjDir%\emul\rom\%ProjName%.rom )
	if /I %Ext%==com ( set EmulatorArgs=!EmulatorArgs! /diskA %ProjDir%\emul\dsk\%ProjName%.dsk )
)

::*****************************************************************************
:: fMSX
::*****************************************************************************
:: Doc: https://fms.komkon.org/fMSX/fMSX.html#LABI
if /I %EmulatorName%==fmsx (
	echo Note: Command line parameters are only fonctionnal since fMSX 6.0

	::---- Add launch options ----
	if %EmulMachine%==1 (
		if /I %Machine%==1 (
			set EmulatorArgs=!EmulatorArgs! -msx1
		) else if /I %Machine%==2 (
			set EmulatorArgs=!EmulatorArgs! -msx2
		) else if /I %Machine%==2P (
			set EmulatorArgs=!EmulatorArgs! -msx2+
		) else if /I %Machine%==TR (
			echo %RED%Error: No MSX turbo R support in fMSX%RESET%
			exit /B 510
		) else if /I %Machine%==12 (
			set EmulatorArgs=!EmulatorArgs! -msx1
		)
	)
	if %Emul60Hz%==1 (
		set EmulatorArgs=!EmulatorArgs! -ntsc
	) else (
		set EmulatorArgs=!EmulatorArgs! -pal
	)
	if %EmulFullScreen%==1 ( echo %YELLOW%Warning: EmulFullScreen can't be use with fMSX%RESET% )
	if %EmulMute%==1       ( set EmulatorArgs=!EmulatorArgs! -nosound )

	::---- Add launch program ----
	if /I %Ext%==bin ( set EmulatorArgs=!EmulatorArgs! -diska %ProjDir%\emul\dsk\%ProjName%.dsk )
	if /I %Ext%==rom ( set EmulatorArgs=!EmulatorArgs! %ProjDir%\emul\rom\%ProjName%.rom )
	if /I %Ext%==com ( set EmulatorArgs=!EmulatorArgs! -diska %ProjDir%\emul\dsk\%ProjName%.dsk )
)

::*****************************************************************************
:: Mesei
::*****************************************************************************
if /I %EmulatorName%==meisei (
	::---- Add launch options ----
	if %EmulMachine%==1    ( echo %YELLOW%Warning: EmulMachine can't be use with Mesei%RESET% )
	if %Emul60Hz%==1       ( echo %YELLOW%Warning: Emul60Hz can't be use with Mesei%RESET% )
	if %EmulFullScreen%==1 ( echo %YELLOW%Warning: EmulFullScreen can't be use with Mesei%RESET% )
	if %EmulMute%==1       ( echo %YELLOW%Warning: EmulMute can't be use with Mesei%RESET% )

	::---- Add launch program ----
	if /I %Ext%==rom ( 
		set EmulatorArgs=!EmulatorArgs! %ProjDir%\emul\rom\%ProjName%.rom 
	) else (
		echo %RED%Error: Mesei only support ROM format%RESET%
		exit /B 520
	)
)

::*****************************************************************************
:: RuMSX
::*****************************************************************************
if /I %EmulatorName%==msxw (
	set EmulatorName=msx
)

if /I %EmulatorName%==msx (
	::---- Add launch options ----
	if %EmulMachine%==1    ( echo %YELLOW%Warning: EmulMachine can't be use with RuMSX%RESET% )
	if %Emul60Hz%==1       ( echo %YELLOW%Warning: Emul60Hz can't be use with RuMSX%RESET% )
	if %EmulFullScreen%==1 ( echo %YELLOW%Warning: EmulFullScreen can't be use with RuMSX%RESET% )
	if %EmulMute%==1       ( set EmulatorArgs=!EmulatorArgs! -NoSoundOut )

	::---- Add launch program ----
	if /I %Ext%==bin ( set EmulatorArgs=!EmulatorArgs! -dirAsDisk -disk %ProjDir%\emul\bin )
	if /I %Ext%==rom ( set EmulatorArgs=!EmulatorArgs! -rom %ProjDir%\emul\rom\%ProjName%.rom )
	if /I %Ext%==com ( set EmulatorArgs=!EmulatorArgs! -dirAsDisk -disk %ProjDir%\emul\dos%DOS% )
)

::*****************************************************************************
:: START EMULATOR
::*****************************************************************************
echo %EmulatorName% %EmulatorArgs%
start /b %Emulator% %EmulatorArgs%
