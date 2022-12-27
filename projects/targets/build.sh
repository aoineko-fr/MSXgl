#!/bin/bash
# ____________________________
# ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
# ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
# █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
# ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
#  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
#─────────────────────────────────────────────────────────────────────────────

#*****************************************************************************
# PROJECT SETTINGS
#*****************************************************************************
ARGS="target=$1 projname=s_target"

# Project name (will be use for output filename)
if [ -n "$2" ]; then ARGS+="_$2"; fi

# Mapper size
if [ -n "$3" ]; then ARGS+=" romsize=$3"; fi

# Install BDOS driver for ROM program? (0=false, 1=true)
if [ "$4" == "BDOS" ]; then ARGS+=' delay'; fi

# Set RAM in slot 0 and install ISR there (0=false, 1=true)
if [ "$4" == "RAMISR" ]; then ARGS+=' ramisr'; fi

# clear

# echo ${ARGS}
if type -P node; then
	node ../../engine/script/js/build.js ${ARGS}
else
	../../tools/build/Node/node ../../engine/script/js/build.js ${ARGS}
fi