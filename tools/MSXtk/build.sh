#!/bin/bash

# @echo off

# if not exist out ( md out )
# if not exist bin ( md bin )

echo ----------------------------------------------------------------------
echo Building MSXbin...
g++ -o ./bin/MSXbin_linux.exe -Isrc -Wall MSXbin.cpp 

echo ----------------------------------------------------------------------
echo Building MSXzip...
g++ -o ./bin/MSXzip_linux.exe -Isrc -Wall MSXzip.cpp src/ayVGM.cpp src/RLEp.cpp

# echo ----------------------------------------------------------------------
# echo Building MSXmath...
# g++ -o bin\MSXmath -Isrc -Wall -std=c++17 MSXmath.cpp

echo ----------------------------------------------------------------------
echo Building MSXhex...
g++ -o ./bin/MSXhex_linux.exe -Isrc -Wall MSXhex.cpp