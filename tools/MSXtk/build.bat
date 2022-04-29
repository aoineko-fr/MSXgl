@echo off

if not exist out ( md out )
if not exist bin ( md bin )

echo Building MSXbin...
g++ -o bin\MSXbin -Isrc -Wall MSXbin.cpp

echo Building MSXzip...
g++ -o bin\MSXzip -Isrc -Wall MSXzip.cpp src\ayVGM.cpp src\RLEp.cpp

REM echo Building MSXmath...
REM g++ -o bin\MSXmath -Isrc -Wall MSXmath.cpp

echo Building MSXhex...
g++ -o bin\MSXhex -Isrc -Wall MSXhex.cpp

pause