@echo off
REM setlocal EnableDelayedExpansion

:: Path
set Tools=..\..\tools
set MSXtk=%Tools%\MSXtk\bin
set Dest=.\content

if not exist %Dest% md %Dest%

echo ----------------------------------------
echo  Building player data...
%MSXtk%\MSXimg.exe datasrc\img.png -copy datasrc\gk.txt -out %Dest%\data_sprt_layer.h -pos 0 64 -size 16 16 -num 16 1 -name g_DataSprtLayer -mode sprt ^
	-l i16 0 0 1 1 0xEB8A06 0xBE5340 ^
	-l i16 0 0 1 1 0x1B141E ^
	-l i16 0 0 1 1 0x1B141E 0x759DA9 0xB8D8D1 0xBE5340

echo ----------------------------------------
echo  Building ball data...
%MSXtk%\MSXimg.exe datasrc\img.png -copy datasrc\gk.txt -out %Dest%\data_sprt_ball.h -pos 0 128 -size 16 16 -num 3 1 -name g_DataSprtBall -mode sprt ^
	-l i16 0 0 1 1 0xCCC35E 0xDED087 ^
	-l i16 0 0 1 1 0xCCC35E

echo ----------------------------------------
echo  Building cloud data...
%MSXtk%\MSXimg.exe datasrc\img.png -out %Dest%\data_sprt_cloud.h -pos 128 128 -size 16 16 -num 7 1 -name g_DataSprtCloud -mode sprt -compress pletter ^
	-l i16 0 0 1 1 0xCCCCCC ^
	-l i16 0 0 1 1 0xFFFFFF 0xCCCCCC

echo ----------------------------------------
echo  Building background data...
%MSXtk%\MSXimg.exe datasrc\bg.png -out %Dest%\data_bg2.h -pos 0 0 -size 256 192 -name g_DataBackground -mode gm2 -compress pletter --tilesComp

echo ----------------------------------------
echo  Building font data...
%MSXtk%\MSXimg.exe datasrc\font.png -copy datasrc\gfx.txt -out %Dest%\data_font.h -pos 0 0 -size 8 8 -num 16 4 -trans 0x010101 -bpc 1 -name g_Font -font 8 8 0x20 _

echo ----------------------------------------
echo  Building font data...
%MSXtk%\MSXimg.exe datasrc\logo.png -out content\logo_tile.h -mode gm2 -compress pletter -name g_DataLogoTile -pos 0 0 -size 48 16 ^
	-l gm2 0 16 32 32

%MSXtk%\MSXimg.exe datasrc\logo.png -out content\logo_sprt.h -mode sprt -compress pletter -name g_DataLogoSprt -pos 0 48 -size 32 48 -num 1 1 ^
	-l i16 0  8 2 2 0xFF897D ^
	-l i16 0  0 2 1 0xB95E51 ^
	-l i16 0 16 2 1 0xFFFFFF ^
	-l i16 0 32 2 1 0xDED087

pause