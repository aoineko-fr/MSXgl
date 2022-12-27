#!/bin/bash
# ____________________________
# ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
# ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
# █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
# ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
#  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
#─────────────────────────────────────────────────────────────────────────────
#  Build all targets
#─────────────────────────────────────────────────────────────────────────────

#-----------------------------------------------------------------------------
# BASIC and MSX-DOS
Formats=("BIN" "DOS1" "DOS2")
Unsupported=("DOS2_ARG")
for g in ${Formats[@]}; do
	./build.sh $g
done
./build.sh BIN_USR USR 0

#-----------------------------------------------------------------------------
# Plain ROM
Formats=("ROM_8K" "ROM_8K_P2" "ROM_16K" "ROM_16K_P2" "ROM_32K" "ROM_48K" "ROM_48K_ISR" "ROM_64K" "ROM_64K_ISR")
Unsupported=("ROM_32K_P0")
for g in ${Formats[@]}; do
	./build.sh $g $g
done

#-----------------------------------------------------------------------------
# ASCII-8 mapped ROM
Formats=("64" "128" "256" "512" "1024" "2048")
for g in ${Formats[@]}; do
	./build.sh ROM_ASCII8 "ROM_ASCII8_${g}K" $g
done

#-----------------------------------------------------------------------------
# ASCII-16 mapped ROM
Formats=("64" "128" "256" "512" "1024" "2048" "4096")
for g in ${Formats[@]}; do
	./build.sh ROM_ASCII16 "ROM_ASCII16_${g}K" $g
done

#-----------------------------------------------------------------------------
# Konami mapped ROM
Formats=("64" "128" "256" "512" "1024" "2048")
for g in ${Formats[@]}; do
	./build.sh ROM_KONAMI "ROM_KONAMI_${g}K" $g
done

#-----------------------------------------------------------------------------
# Konami-SCC mapped ROM
Formats=("64" "128" "256" "512" "1024" "2048")
for g in ${Formats[@]}; do
	./build.sh ROM_KONAMI_SCC "ROM_KONAMI_SCC_${g}K" $g
done

#-----------------------------------------------------------------------------
# BDOS
./build.sh ROM_32K ROM_32K_BDOS 0 BDOS
./build.sh ROM_48K ROM_48K_BDOS 0 BDOS
./build.sh ROM_ASCII8 ROM_ASCII8_128K_BDOS 128 BDOS

#-----------------------------------------------------------------------------
# RAM ISR
./build.sh ROM_32K ROM_32K_RAMISR 0 RAMISR
./build.sh ROM_48K ROM_48K_RAMISR 0 RAMISR
./build.sh ROM_ASCII8 ROM_ASCII8_128K_RAMISR 128 RAMISR
