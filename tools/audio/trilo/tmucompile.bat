@echo off 
REM %~1 - expands %1 removing any surrounding quotes (")
REM %~f1 - expands %1 to a fully qualified path name
REM %~d1 - expands %1 to a drive letter only
REM %~p1 - expands %1 to a path only
REM %~n1 - expands %1 to a file name only
REM %~x1 - expands %1 to a file extension only
REM %~s1 - expanded path contains short names only
REM %~a1 - expands %1 to file attributes
REM %~t1 - expands %1 to date/time of file
REM %~z1 - expands %1 to size of file

set File=%~f1
set FilePath=%~d1%~p1
set FileName=%~n1
set FileExt=%~x1

echo ---- Compiling TMU file %FileName%%FileExt%...
echo Source: %1
echo Target: %FilePath%tmu_%FileName%.asm
echo Tool:   %~d0%~p0tmucompile.exe
%~d0%~p0tmucompile.exe %1 %FilePath%tmu_%FileName%_tmp.asm
%~d0%~p0..\misc\rxrepl -f %FilePath%tmu_%FileName%_tmp.asm -o %FilePath%tmu_%FileName%.asm -s "\." -r "" -s "\$" -r "0x" -s "db" -r ".db" -s "0x.db" -r "0xdb" -s "dw" -r ".dw" -s "waveform_" -r "%FileName%_waveform_" -s "instrument_" -r "%FileName%_instrument_" -s "track_" -r "%FileName%_track_" -s "restart" -r "%FileName%_restart" -s "customvoice_" -r "%FileName%_customvoice_" -s "drummacro_" -r "%FileName%_drummacro_" 
echo.

pause