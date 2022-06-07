#!/bin/bash

[ -d "out" ] || mkdir out
[ -d "bin" ] || mkdir bin

echo ----------------------------------------------------------------------
echo Building MSXbin...
g++ -o ./bin/MSXbin_linux -Isrc -Wall MSXbin.cpp 

echo ----------------------------------------------------------------------
echo Building MSXzip...
g++ -o ./bin/MSXzip_linux -Isrc -Wall MSXzip.cpp src/ayVGM.cpp src/RLEp.cpp

# echo ----------------------------------------------------------------------
# echo Building MSXmath...
# g++ -o bin\MSXmath_linux -Isrc -Wall MSXmath.cpp

echo ----------------------------------------------------------------------
echo Building MSXhex...
g++ -o ./bin/MSXhex_linux -Isrc -Wall MSXhex.cpp