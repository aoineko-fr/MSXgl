@echo off

if not exist out ( md out )
if not exist bin ( md bin )

REM echo ----------------------------------------------------------------------
REM echo Building MSXbin...
REM g++ -o bin\MSXbin -Isrc -Wall -std=c++17 MSXbin.cpp

REM echo ----------------------------------------------------------------------
REM echo Building MSXzip...
REM g++ -o bin\MSXzip -Isrc -Wall -std=c++17 MSXzip.cpp src\ayVGM.cpp src\RLEp.cpp

REM REM echo Building MSXmath...
REM REM g++ -o bin\MSXmath -Isrc -Wall -std=c++17 MSXmath.cpp

REM echo ----------------------------------------------------------------------
REM echo Building MSXhex...
REM g++ -o bin\MSXhex -Isrc -Wall -std=c++17 MSXhex.cpp

echo ----------------------------------------------------------------------
echo Building MSXimg...
g++ -o bin\MSXimg -Isrc -IFreeimage -Wall -std=c++17 MSXimg.cpp src\color.cpp src\exporter.cpp src\format.cpp src\image.cpp src\parser.cpp Freeimage\FreeImageLib64.lib

pause