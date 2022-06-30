@ECHO off

@ECHO off

::*****************************************************************************
:: PROJECT CONFIGURATION OVERWRITE                                                       
::*****************************************************************************

:: Set RAM in slot 0 and install ISR there
:: - 0				Don't install
:: - 1 | VBLANK		Add ISR that support V-blank interruption
:: - HBLANK			Add ISR that support V-blank and H-blank interruption
set InstallRAMISR=VBLANK
