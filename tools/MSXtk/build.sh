#!/bin/bash

[ -d "out" ] || mkdir out
[ -d "bin" ] || mkdir bin

echo ----------------------------------------------------------------------
echo Building MSXbin...
g++ -std=c++17 -o ./out/MSXbin -Isrc -Wall src/MSXbin.cpp 
if [ $? -eq 0 ]; then
	echo copy MSXbin to bin
	cp out/MSXbin bin/MSXbin
fi

echo ----------------------------------------------------------------------
echo Building MSXzip...
g++ -std=c++17 -o ./out/MSXzip -Isrc -Wall src/MSXzip.cpp src/lVGM.cpp src/RLEp.cpp
cp out/MSXzip bin/MSXzip

echo ----------------------------------------------------------------------
echo Building MSXmath...
g++ -std=c++17 -o out/MSXmath -Isrc -Wall src/MSXmath.cpp
if [ $? -eq 0 ]; then
	echo copy MSXmath to bin
	cp out/MSXmath bin/MSXmath
fi

echo ----------------------------------------------------------------------
echo Building MSXhex...
g++ -std=c++17 -o ./out/MSXhex -Isrc -Wall src/MSXhex.cpp
if [ $? -eq 0 ]; then
	echo copy MSXhex to bin
	cp out/MSXhex bin/MSXhex
fi

# echo ----------------------------------------------------------------------
# echo Building MSXimg...
# g++ -o out/MSXimg -Isrc -IFreeimage -Wall -std=c++17 src/MSXimg.cpp src/color.cpp src/exporter.cpp src/image.cpp src/parser.cpp src/Pletter.cpp Freeimage/libfreeimage.a
# if [ $? -eq 0 ]; then
#	echo copy MSXimg to bin
#	cp out/MSXimg bin/MSXimg
# fi

