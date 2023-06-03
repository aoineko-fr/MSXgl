// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄   ▄ ▄ ▄▄▄▄ ▄▄▄   ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██ ▀ ██▀█ ██▄  ██▀█ ██ ▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ▀█▄▀ ██ █ ██▄▄ ██ █ ▀█▄▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// PCM-Encoder replayer
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "psg.h"

// Unique PCM-Encoder player
#if (PCMENC_FREQ == PCMENC_8K)
	#define PCM_Play PCM_Play_8K
#elif (PCMENC_FREQ == PCMENC_11K)
	#define PCM_Play PCM_Play_11K
#elif (PCMENC_FREQ == PCMENC_22K)
	#define PCM_Play PCM_Play_22K
#elif (PCMENC_FREQ == PCMENC_44K)
	#define PCM_Play PCM_Play_44K
#endif


#if (PCMENC_FREQ & PCMENC_8K)
// Function: PCM_Play_8K
// Play a pcmenc sound at 8 KHz (synchronously).
// Available only if (PCMENC_FREQ & PCMENC_8K).
//
// Paramaters:
//   addr	- Address of source data
void PCM_Play_8K(u16 addr);
#endif

#if (PCMENC_FREQ & PCMENC_11K)
// Function: PCM_Play_11K
// Play a pcmenc sound at 11 KHz (synchronously).
// Available only if (PCMENC_FREQ & PCMENC_11K).
//
// Paramaters:
//   addr	- Address of source data
void PCM_Play_11K(u16 addr);
#endif

#if (PCMENC_FREQ & PCMENC_22K)
// Function: PCM_Play_22K
// Play a pcmenc sound at 22 KHz (synchronously).
// Available only if (PCMENC_FREQ & PCMENC_22K).
//
// Paramaters:
//   addr	- Address of source data
void PCM_Play_22K(u16 addr);
#endif

#if (PCMENC_FREQ & PCMENC_44K)
// Function: PCM_Play_44K
// Play a pcmenc sound at 44 KHz (synchronously).
// Available only if (PCMENC_FREQ & PCMENC_44K).
//
// Paramaters:
//   addr	- Address of source data
void PCM_Play_44K(u16 addr);
#endif