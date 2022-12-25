#!/bin/bash

[ -d "out" ] || mkdir out
[ -d "bin" ] || mkdir bin

echo ----------------------------------------------------------------------
echo Building MSXbin...
g++ -std=c++17 -o ./out/MSXbin -Isrc -Wall src/MSXbin.cpp 
cp out/MSXbin bin/MSXbin
chmod 755 bin/MSXbin

echo ----------------------------------------------------------------------
echo Building MSXzip...
g++ -std=c++17 -o ./out/MSXzip -Isrc -Wall src/MSXzip.cpp src/ayVGM.cpp src/RLEp.cpp
cp out/MSXzip bin/MSXzip
chmod 755 bin/MSXbin

# echo ----------------------------------------------------------------------
# echo Building MSXmath...
# g++ -std=c++17 -o out/MSXmath -Isrc -Wall src/MSXmath.cpp
# cp out/MSXmath bin/MSXmath
# chmod 755 bin/MSXbin

echo ----------------------------------------------------------------------
echo Building MSXhex...
g++ -std=c++17 -o ./out/MSXhex -Isrc -Wall src/MSXhex.cpp
cp out/MSXhex bin/MSXhex
chmod 755 bin/MSXbin
