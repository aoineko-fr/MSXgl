// ____________________________
// ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄▄▄▄  ▄▄   ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀  ██  ██ █ ▀█▄  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▄██▄ ▀█▄▀ ▄▄█▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// BIOS routines handler 
//
// References:
// - MSX2 Technical Handbook
// - http://map.grauw.nl/resources/msxbios.php
// - https://www.msx.org/wiki/Main-ROM_BIOS
// - Pratique du MSX 2
//─────────────────────────────────────────────────────────────────────────────
#pragma once

// R_xxxx	Data/routines in Main-ROM
// S_xxxx	Data/routines in Sub-ROM
// M_xxxx	Data/routines in Memory
// H_xxxx	Hooks
// P_xxxx	Ports
// I_xxxx	Interrupts
#include "bios_var.h"
#include "bios_hook.h"
#include "bios_mainrom.h"
#include "bios_subrom.h"
#include "bios_diskrom.h"

//=============================================================================
// DEFINES
//=============================================================================

// Files
#define FILE(str) "\"" str "\""

// Input Macros
#define KEYBOARD_HOLD(key)    ((g_NEWKEY[KEY_ROW(key)] & KEY_FLAG(key)) == 0)
#define KEYBOARD_PRESS(key)   (((g_NEWKEY[KEY_ROW(key)] & KEY_FLAG(key)) == 0) && ((g_OLDKEY[KEY_ROW(key)] & KEY_FLAG(key)) != 0))
#define KEYBOARD_RELEASE(key) (((g_NEWKEY[KEY_ROW(key)] & KEY_FLAG(key)) != 0) && ((g_OLDKEY[KEY_ROW(key)] & KEY_FLAG(key)) == 0))


//-----------------------------------------------------------------------------
//  █ █ █▀▀ █   █▀█ █▀▀ █▀█   █▀▀ █ █ █▄ █ █▀▀ ▀█▀ █ █▀█ █▄ █ █▀
//  █▀█ ██▄ █▄▄ █▀▀ ██▄ █▀▄   █▀  █▄█ █ ▀█ █▄▄  █  █ █▄█ █ ▀█ ▄█
//-----------------------------------------------------------------------------

// Call a bios function
inline void Bios_MainCall(u16 addr);

/// Handle soft reboot
void Bios_Reboot();

/// Handle clean transition to Basic or MSX-DOS environment
void Bios_Exit(u8 ret) __FASTCALL;

/// Get the slot ID of a given page
// u8 Bios_GetSlot(u8 page) __FASTCALL;

