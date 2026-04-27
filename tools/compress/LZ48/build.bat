@echo off

REM if not exist out ( md out )
REM if not exist bin ( md bin )

echo ----------------------------------------------------------------------
echo Building MSXbin...
REM g++ -o lz48 -Isrc -Wall -std=c++17 lz48_v002.c
gcc -o lz48 -Isrc -Wall -std=c++17 lz48_v002.c
