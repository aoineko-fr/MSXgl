// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄   ▄ ▄ ▄▄▄▄ ▄▄▄   ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██ ▀ ██▀█ ██▄  ██▀█ ██ ▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ▀█▄▀ ██ █ ██▄▄ ██ █ ▀█▄▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// PCM Player by Hitoshi Iwai (aburi6800)
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "psg.h"

// Function: PCM_Play
// Play a PCM sound at 8 or 11 KHz (synchronously).
// PCMPLAY_FREQ define must be PCMPLAY_8K or PCMPLAY_11K.
//
// Paramaters:
//   addr	- Address of source data
//   size	- Size of source data
void PCM_Play(void* addr, u16 size);