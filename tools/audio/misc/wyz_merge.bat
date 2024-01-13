@echo off

:: Setup variables
set File=%~f1
set FilePath=%~d1%~p1
set FileName=%~n1
set FileExt=%~x1
set OutFilePath=%2
set OutFileName=%OutFilePath%\%FileName%.asm
set IncFilePath=%3
set Tools=%~d0%~p0\..\..
set MSXtk=%Tools%\MSXtk\bin
set AudioMisc=%Tools%\audio\misc

echo Merging '%FileName%' WYZ assembler data to %OutFilePath%...

:: Checks for needed files
if not exist %FilePath%%FileName%.mus goto Error
if not exist %FilePath%%FileName%.mus.asm goto Error

:: Convert binary to asm
%MSXtk%\MSXbin.exe %FilePath%%FileName%.mus -asm -nodeco -t _%FileName%_Data -o %OutFilePath%\%FileName%_asmsong.asm

:: Create output assembler file
echo ;-----------------------------------------------------------------------------> %OutFileName%
echo ; WYZ file merged for MSXgl on %DATE% %TIME% >> %OutFileName%
echo ;----------------------------------------------------------------------------->> %OutFileName%
echo .module wyz_%FileName% >> %OutFileName%
echo _%FileName%_Song:: DW SONG00 >> %OutFileName%
type %FilePath%%FileName%.mus.asm >> %OutFileName%

%AudioMisc%\fart.exe -C %OutFileName% "TABLA_PAUTAS:"  "_%FileName%_Inst::"
%AudioMisc%\fart.exe -C %OutFileName% "TABLA_SONIDOS:" "_%FileName%_FX::"
%AudioMisc%\fart.exe -C %OutFileName% "DATOS_NOTAS:"   "_%FileName%_Freq::"
%AudioMisc%\fart.exe -C %OutFileName% "DB"             ".DB"
%AudioMisc%\fart.exe -C %OutFileName% "DW"             ".DW"

echo ; Song data >> %OutFileName%
echo SONG00: >> %OutFileName%
REM echo .incbin "%IncFilePath%\%FileName%.mus" >> %OutFileName%
type %OutFilePath%\%FileName%_asmsong.asm >> %OutFileName%
echo ; End of file >> %OutFileName%
echo _%FileName%_End:: >> %OutFileName%


:: Copy MUS file
REM copy %FilePath%%FileName%.mus %OutFilePath%\%FileName%.mus
del %OutFilePath%\%FileName%_asmsong.asm /Q

echo Done
exit /b 0

:: Error handling
Error:
	echo Error!
	exit /b 1