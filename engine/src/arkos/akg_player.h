// __________________________
// ██▀█▀██▀▀▀█▀▀█▀█  ▄▄▄ ▄▄  │   ▄▄       ▄▄            
// █  ▄ █▄ ▀██▄ ▀▄█ ██   ██  │  ██▄█ ██▄▀ ██▄▀ ▄█▀▄  ██▀
// █  █ █▀▀ ▄█  █ █ ▀█▄█ ██▄▄│  ██ █ ██   ██ █ ▀█▄▀ ▄██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀──────────┘  
//  Arkos AKG replayer
//─────────────────────────────────────────────────────────────────────────────
// Arkos Tracker and the Z80 players are coded and designed by Julien Névo a.k.a Targhan/Arkos.
// https://www.julien-nevo.com/arkostracker/index.php/credits/
//─────────────────────────────────────────────────────────────────────────────
#pragma once
#include "core.h"

//
void AKG_Init(const void* data, u16 sng);

//
void AKG_Stop();

//
void AKG_Decode();