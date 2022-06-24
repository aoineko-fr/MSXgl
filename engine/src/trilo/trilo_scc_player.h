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

//=============================================================================
// DEFINES
//=============================================================================

extern u16 g_TriloSCC_ToneTable;
extern u16 g_TriloSFX_Bank;
extern u16 g_TriloSFX_WaveTable;

//=============================================================================
// FUNCTIONS
//=============================================================================
// Group: SCC player

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
inline void TriloSCC_SetToneTable(u16 addr) { g_TriloSCC_ToneTable = addr; }

//
void TriloSCC_Silent();

//
void TriloSCC_Update();

//
void TriloSCC_Apply();

//=============================================================================
// Group: SFX player

//
void TriloSFX_Initialize();

//
inline void TriloSFX_Load(u16 bank, u16 wave) { g_TriloSFX_Bank = bank; g_TriloSFX_WaveTable = wave; }

//
inline u8 TriloSFX_GetNumber() { return *((u8*)g_TriloSFX_Bank); }

//
void TriloSFX_SetBalanceSCC(u8 vol);

//
void TriloSFX_SetBalancePSG(u8 vol);

//
void TriloSFX_Play(u8 sfx, u8 prio);

//
void TriloSFX_Update();