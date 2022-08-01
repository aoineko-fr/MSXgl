@echo off
REM setlocal EnableDelayedExpansion

:: Path
set Tools=..\..\tools
set MSXtk=%Tools%\MSXtk\bin
set Dest=.\content

if not exist %Dest% md %Dest%

echo Building image data...

%MSXtk%\MSXimg.exe datasrc\img.png -copy datasrc\gk.txt -out %Dest%\data_sprt_layer.h -pos 0 64 -size 16 16 -num 13 1 -name g_DataSprtLayer -mode sprt ^
	-l i16 0 0 1 1 0x1B141E ^
	-l i16 0 0 1 1 0xFDF5F1 0xB8D8D1 0x759DA9 ^
	-l i16 0 0 1 1 0xEB8A06 0xBE5340 ^
	-l i16 0 0 1 1 0x1B141E 0x759DA9 0xB8D8D1 0xBE5340

%MSXtk%\MSXimg.exe datasrc\img.png -copy datasrc\gk.txt -out %Dest%\data_sprt_ball.h -pos 0 128 -size 16 16 -num 3 1 -name g_DataSprtBall -mode sprt ^
	-l i16 0 0 1 1 0xCCC35E 0xDED087 ^
	-l i16 0 0 1 1 0xCCC35E

%MSXtk%\MSXimg.exe datasrc\img.png -out %Dest%\data_bg.h -pos 0 192 -size 8 8 -num 8 3 -name g_DataBackground -mode sprt -l i8 0 0 1 1 0xFFFFFF

pause