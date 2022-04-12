// __________________________
// ██▀█▀██▀▀▀█▀▀█▀█  ▄▄▄ ▄▄  │   ▄▄       ▄▄            
// █  ▄ █▄ ▀██▄ ▀▄█ ██   ██  │  ██▄█ ██▄▀ ██▄▀ ▄█▀▄  ██▀
// █  █ █▀▀ ▄█  █ █ ▀█▄█ ██▄▄│  ██ █ ██   ██ █ ▀█▄▀ ▄██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀──────────┘  
//  Arkos AKM replayer
//─────────────────────────────────────────────────────────────────────────────
// Arkos Tracker and the Z80 players are coded and designed by Julien Névo a.k.a Targhan/Arkos.
// https://www.julien-nevo.com/arkostracker/index.php/credits/
//─────────────────────────────────────────────────────────────────────────────
#pragma once
#include "core.h"

//
void AKM_Init(const void* data, u16 sng);

//
void AKM_Stop();

//
void AKM_Decode();