// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// Deprecated symbols for backward compatibility
//─────────────────────────────────────────────────────────────────────────────
#pragma once

// Core
#define true						TRUE	
#define false						FALSE	
#define null						NULL	

// VDP module
#define VDP_HideSpriteFrom			VDP_DisableSpritesFrom

// Compress Module
#define UnpackRLEpToRAM				RLEp_UnpackToRAM

// Input Module
#define JOY_INPUT_DIR_UP_RIGTH		JOY_INPUT_DIR_UP_RIGHT
#define JOY_INPUT_DIR_DOWN_RIGTH	JOY_INPUT_DIR_DOWN_RIGHT
