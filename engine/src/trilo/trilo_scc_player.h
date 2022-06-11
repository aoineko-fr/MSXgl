// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄      ▄  ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ██▄▀ ▄  ██  ▄█▀▄
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │   ██  ██   ██ ▀█▄ ▀█▄▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Trilo Tracker SCC replayer
//
// Trilo Tracker and the Z80 players are coded and designed by Cornelisser.
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//
void TriloSCC_Initialize();

//
void TriloSCC_Pause();

//
void TriloSCC_FadeOut(u8 speed);

//
void TriloSCC_SetBalanceSCC(u8 vol);

//
void TriloSCC_SetBalancePSG(u8 vol);

//
void TriloSCC_Equalization(bool enable);

//
void TriloSCC_LoadSong(u16 addr);

//
void TriloSCC_Silent();

//
void TriloSCC_Play();

//
void TriloSCC_Apply();