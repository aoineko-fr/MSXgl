@echo off

if not exist out ( md out )
if not exist bin ( md bin )

echo ----------------------------------------------------------------------
echo Building MSXbin...
g++ -o bin\MSXbin -Isrc -Wall -std=c++17 MSXbin.cpp

echo ----------------------------------------------------------------------
echo Building MSXzip...
g++ -o bin\MSXzip -Isrc -Wall -std=c++17 MSXzip.cpp src\ayVGM.cpp src\RLEp.cpp

REM echo Building MSXmath...
REM g++ -o bin\MSXmath -Isrc -Wall -std=c++17 MSXmath.cpp

echo ----------------------------------------------------------------------
echo Building MSXhex...
g++ -o bin\MSXhex -Isrc -Wall -std=c++17 MSXhex.cpp

pause