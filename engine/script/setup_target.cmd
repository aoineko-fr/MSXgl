::██▀▀█▀▀███▀▀▀▀▀▀▀███▀▀█▀▀▀▀▀▀▀▀█
::██  ▀  ██   ▄▄▄▄  ▀  ▄█ ▄▀▀ █  █
::█  ▄ ▄  ▀▀▀   █▀  ▄  ▀█ ▀▄█ █▄ █
::█▄▄█▄█▄▄▄▄▄▄▄██▄▄███▄▄█▄▄▄▄▄▄▄▄█
:: by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
@echo off

set MapperSize=0
set DOS=0
if not defined ROMSize set ROMSize=128

if /I %Target%==DOS     set Target=DOS1
if /I %Target%==DOS_ARG set Target=DOS2_ARG
if /I %Target%==ROM     set Target=ROM_32K
if /I %Target%==BAS     set Target=BIN

::***************************************************************************
::* BASIC                                                                   *
::***************************************************************************

::---------------------------------------------------------------------------
if /I %Target%==BIN (

	set Ext=bin
	set Crt0=crt0_basic
	set StartAddr=8000
	set CodeAddr=8007
	set RamAddr=0
	set FillSize=0
	
	echo » Target: BASIC binary program ^(8000h~^)
	exit /B 0
)

::***************************************************************************
::* PLAIN ROM                                                               *
::***************************************************************************

::---------------------------------------------------------------------------
if /I %Target%==ROM_8K (

	set Ext=rom
	set Crt0=crt0_rom16
	set StartAddr=4000
	set CodeAddr=4010
	set RamAddr=8000
	set /A FillSize=8*1024

	echo » Target: 8KB ROM in page 1 ^(4000h ~ 5FFFh^)
	exit /B 0
)
::---------------------------------------------------------------------------
if /I %Target%==ROM_8K_P2 (

	set Ext=rom
	set Crt0=crt0_rom16p2
	set StartAddr=8000
	set CodeAddr=8010
	set RamAddr=C000
	set /A FillSize=8*1024

	echo » Target: 8KB ROM in page 2 ^(8000h ~ 9FFFh^)
	exit /B 0
)
::---------------------------------------------------------------------------
if /I %Target%==ROM_16K (

	set Ext=rom
	set Crt0=crt0_rom16
	set StartAddr=4000
	set CodeAddr=4010
	set RamAddr=8000
	set /A FillSize=16*1024

	echo » Target: 16KB ROM in page 1 ^(4000h ~ 7FFFh^)
	exit /B 0
)
::---------------------------------------------------------------------------
if /I %Target%==ROM_16K_P2 (

	set Ext=rom
	set Crt0=crt0_rom16p2
	set StartAddr=8000
	set CodeAddr=8010
	set RamAddr=C000
	set /A FillSize=16*1024

	echo » Target: 16KB ROM in page 2 ^(8000h ~ BFFFh^)
	exit /B 0
)
::---------------------------------------------------------------------------
if /I %Target%==ROM_32K (

	set Ext=rom
	set Crt0=crt0_rom32
	set StartAddr=4000
	set CodeAddr=4010
	set RamAddr=C000
	set /A FillSize=32*1024

	echo » Target: 32KB ROM in page 1^&2 ^(4000h ~ BFFFh^)
	exit /B 0
)
::---------------------------------------------------------------------------
if /I %Target%==ROM_48K (

	set Ext=rom
	set Crt0=crt0_rom48
	set StartAddr=0000
	set CodeAddr=4000
	set RamAddr=C000
	set /A FillSize=48*1024

	echo » Target: 48KB ROM in page 0-2 ^(0000h ~ BFFFh^)
	exit /B 0
)
::---------------------------------------------------------------------------
if /I %Target%==ROM_48K_ISR (

	set Ext=rom
	set Crt0=crt0_rom48_isr
	set StartAddr=0000
	set CodeAddr=4000
	set RamAddr=C000
	set /A FillSize=48*1024

	echo » Target: 48KB ROM in page 0-2 ^(0000h ~ BFFFh^) with ISR replacement
	exit /B 0
)
::---------------------------------------------------------------------------
if /I %Target%==ROM_64K (

	set Ext=rom
	set Crt0=crt0_rom48
	set StartAddr=0000
	set CodeAddr=4000
	set RamAddr=C000
	set /A FillSize=64*1024

	echo » Target: 64KB ROM in page 0-3 ^(0000h ~ FFFFh^)
	exit /B 0
)
::---------------------------------------------------------------------------
if /I %Target%==ROM_64K_ISR (

	set Ext=rom
	set Crt0=crt0_rom48_isr
	set StartAddr=0000
	set CodeAddr=4000
	set RamAddr=C000
	set /A FillSize=64*1024

	echo » Target: 64KB ROM in page 0-3 ^(0000h ~ FFFFh^) with ISR replacement
	exit /B 0
)

::***************************************************************************
::* MAPPED ROM                                                              *
::***************************************************************************

::---------------------------------------------------------------------------
if /I %Target%==ROM_ASCII8 (

	set Mapper=ROM_ASCII8
	set Target=ROM_ASCII8_%ROMSize%K
	set Ext=rom
	set Crt0=crt0_rom_mapper
	set StartAddr=4000
	set CodeAddr=4010
	set RamAddr=C000
	set /A FillSize=32*1024
	set /A MapperSize=%ROMSize%*1024
	set /A SegSize=8*1024
	set Bank0Addr=4000
	set Bank1Addr=6000
	set Bank2Addr=8000
	set Bank3Addr=A000

	echo » Target: %ROMSize%KB ROM using ASCII-8 mapper ^(starting at 4000h^)
	exit /B 0
)
::---------------------------------------------------------------------------
if /I %Target%==ROM_ASCII16 (

	set Mapper=ROM_ASCII16
	set Target=ROM_ASCII16_%ROMSize%K
	set Ext=rom
	set Crt0=crt0_rom_mapper
	set StartAddr=4000
	set CodeAddr=4010
	set RamAddr=C000
	set /A FillSize=32*1024
	set /A MapperSize=%ROMSize%*1024
	set /A SegSize=16*1024
	set Bank0Addr=4000
	set Bank1Addr=8000
	set Bank2Addr=0
	set Bank3Addr=0

	echo » Target: %ROMSize%KB ROM using ASCII-16 mapper ^(starting at 4000h^)
	exit /B 0
)
::---------------------------------------------------------------------------
if /I %Target%==ROM_KONAMI (

	set Mapper=ROM_KONAMI
	set Target=ROM_KONAMI_%ROMSize%K
	set Ext=rom
	set Crt0=crt0_rom_mapper
	set StartAddr=4000
	set CodeAddr=4010
	set RamAddr=C000
	set /A FillSize=32*1024
	set /A MapperSize=%ROMSize%*1024
	set /A SegSize=8*1024
	set Bank0Addr=0
	set Bank1Addr=6000
	set Bank2Addr=8000
	set Bank3Addr=A000

	echo » Target: %ROMSize%KB ROM using KONAMI mapper ^(starting at 4000h^)
	exit /B 0
)
::---------------------------------------------------------------------------
if /I %Target%==ROM_KONAMI_SCC (

	set Mapper=ROM_KONAMI_SCC
	set Target=ROM_KONAMI_SCC_%ROMSize%K
	set Ext=rom
	set Crt0=crt0_rom_mapper
	set StartAddr=4000
	set CodeAddr=4010
	set RamAddr=C000
	set /A FillSize=32*1024
	set /A MapperSize=%ROMSize%*1024
	set /A SegSize=8*1024
	set Bank0Addr=4000
	set Bank1Addr=6000
	set Bank2Addr=8000
	set Bank3Addr=A000

	echo » Target: %ROMSize%KB ROM using KONAMI SCC mapper ^(starting at 4000h^)
	exit /B 0
)

::***************************************************************************
::* MSX-DOS                                                                 *
::***************************************************************************

::---------------------------------------------------------------------------
if /I %Target%==DOS1 (

	set DOS=1
	set Ext=com
	set Crt0=crt0_dos
	set StartAddr=0100
	set CodeAddr=0100
	set RamAddr=0
	set FillSize=0

	echo » Target: MSX-DOS 1 program ^(starting at 0100h^)
	exit /B 0
)
::---------------------------------------------------------------------------
if /I %Target%==DOS2 (

	set DOS=2
	set Ext=com
	set Crt0=crt0_dos
	set StartAddr=0100
	set CodeAddr=0100
	set RamAddr=0
	set FillSize=0

	echo » Target: MSX-DOS 2 program ^(starting at 0100h^)
	exit /B 0
)
::---------------------------------------------------------------------------
if /I %Target%==DOS2_ARG (

	set DOS=2
	set Ext=com
	set Crt0=crt0_dosarg
	set StartAddr=0100
	set CodeAddr=0180
	set RamAddr=0
	set FillSize=0

	echo » Target: MSX-DOS 2 program with command line arguments ^(starting at 0100h^)
	exit /B 0
)

echo %RED% Error^! Unknow target %Target%%RESET%
exit /b 1
