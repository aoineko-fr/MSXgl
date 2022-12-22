@echo off

if not exist out ( md out )
if not exist bin ( md bin )

echo ----------------------------------------------------------------------
echo Building MSXbin...
g++ -o out\MSXbin -Isrc -Wall -std=c++17 src\MSXbin.cpp
copy out\MSXbin.exe bin\MSXbin.exe

echo ----------------------------------------------------------------------
echo Building MSXzip...
g++ -o out\MSXzip -Isrc -Wall -std=c++17 src\MSXzip.cpp src\ayVGM.cpp src\RLEp.cpp
copy out\MSXzip.exe bin\MSXzip.exe

REM echo Building MSXmath...
REM g++ -o out\MSXmath -Isrc -Wall -std=c++17 src\MSXmath.cpp
REM copy out\MSXmath.exe bin\MSXmath.exe

echo ----------------------------------------------------------------------
echo Building MSXhex...
g++ -o out\MSXhex -Isrc -Wall -std=c++17 src\MSXhex.cpp
copy out\MSXhex.exe bin\MSXhex.exe

REM echo ----------------------------------------------------------------------
REM echo Building MSXimg...
REM g++ -o out\MSXimg -Isrc -IFreeimage -Wall -std=c++17 src\MSXimg.cpp src\color.cpp src\exporter.cpp src\format.cpp src\image.cpp src\parser.cpp Freeimage\FreeImageLib64.lib
REM copy out\MSXimg.exe bin\MSXimg.exe

pause