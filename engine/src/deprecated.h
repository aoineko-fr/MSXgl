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
#define Joystick_GetTrigger			Joystick_IsButtonPressed

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

// Memory module
#define Mem_ChunkInitialize			Mem_DynamicInitialize
#define Mem_ChunkAlloc				Mem_DynamicAlloc
#define Mem_ChunkFree				Mem_DynamicFree
#define Mem_GetChunkSize			Mem_GetDynamicSize

// MSX-DOS module
#define DOS_NO_ERROR				DOS_ERR_NONE
#define FCB							DOS_FCB
#define DOS2_Open					DOS_FOpen
#define DOS2_Create					DOS_FCreate
#define DOS2_Close					DOS_FClose
#define DOS2_Duplicate				DOS_FDuplicate
#define DOS2_Read					DOS_FRead
#define DOS2_Write					DOS_FWrite
#define DOS2_LSeek					DOS_FSeek
#define DOS2_Exit					DOS_Exit

