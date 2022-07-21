// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█		   │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//	by Guillaume 'Aoineko' Blanchard under CC BY-SA license
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
#define VADDR_Lo					VADDR_LO
#define VADDR_Hi					VADDR_HI

// Compress module
#define UnpackRLEpToRAM				RLEp_UnpackToRAM

// Input module
#define JOY_INPUT_DIR_UP_RIGTH		JOY_INPUT_DIR_UP_RIGHT
#define JOY_INPUT_DIR_DOWN_RIGTH	JOY_INPUT_DIR_DOWN_RIGHT

// Compress module
#define ZX0_Unpack					ZX0_UnpackToRAM
#define Pletter_Unpack				Pletter_UnpackToRAM
#define Bitbuster_Unpack			Bitbuster_UnpackToRAM

// BIOS module
#define Bios_InterSlotCallF			BIOS_CALLF
#define Bios_Reboot					Bios_Startup
#define Bios_MainCall				Call

// Math module
#define Abs8						ABS8
#define Abs16						ABS16
#define Abs32						ABS32
#define Invert						INVERT
#define Merge44						MERGE44
#define Merge88						MERGE88
#define ModuloPow2					MOD_POW2
#define Clamp8						CLAMP8
#define Clamp16						CLAMP16
#define Max							MAX
#define Min							MIN
