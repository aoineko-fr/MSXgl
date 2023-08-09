#!/bin/bash

[ -d "out" ] || mkdir out
[ -d "bin" ] || mkdir bin

echo ----------------------------------------------------------------------
echo Building MSXbin...
g++ -std=c++17 -o ./out/MSXbin -Isrc -Wall src/MSXbin.cpp 
cp out/MSXbin bin/MSXbin

echo ----------------------------------------------------------------------
echo Building MSXzip...
g++ -std=c++17 -o ./out/MSXzip -Isrc -Wall src/MSXzip.cpp src/lVGM.cpp src/RLEp.cpp
cp out/MSXzip bin/MSXzip

# echo ----------------------------------------------------------------------
# echo Building MSXmath...
# g++ -std=c++17 -o out/MSXmath -Isrc -Wall src/MSXmath.cpp
# cp out/MSXmath bin/MSXmath

echo ----------------------------------------------------------------------
echo Building MSXhex...
g++ -std=c++17 -o ./out/MSXhex -Isrc -Wall src/MSXhex.cpp
cp out/MSXhex bin/MSXhex
