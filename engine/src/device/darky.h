// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄▄  ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ▀█▄  ██  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ▄▄█▀ ▀█▄█
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// DARKY handler module
//
// References:
//  - AY-3-8910 Programmable Sound Generator Data Manual
//     http://map.grauw.nl/resources/sound/generalinstrument_ay-3-8910.pdf

//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

// 40H W: Using 170 as ID to enable Darky switched I/O 41H..4FH
#define P_DARKY_ENABLE				0x40
__sfr __at(P_DARKY_ENABLE) g_Darky_EnablePort;

// 41H RW: CPLD control register
#define P_DARKY_CPLD				0x41
__sfr __at(P_DARKY_CPLD) g_Darky_ControlPort;

// 42H RW: Internal register (b7=’1’-address and b7=’0’-Data)
#define P_DARKY_REG					0x42
__sfr __at(P_DARKY_REG) g_Darky_RegisterPort;

// 44H W: PSG 1 Index
#define P_DARKY_PSG1_IDX			0x44
__sfr __at(P_DARKY_PSG1_IDX) g_Darky_PSG1_IndexPort;

// 45H W: PSG 1 Data write
#define P_DARKY_PSG1_DATA			0x45
__sfr __at(P_DARKY_PSG1_DATA) g_Darky_PSG1_DataPort;

// 46H R: PSG 1 Data read
#define P_DARKY_PSG1_READ			0x46
__sfr __at(P_DARKY_PSG1_READ) g_Darky_PSG1_ReadPort;

// 4CH W: PSG 2 Index
#define P_DARKY_PSG2_IDX			0x4C
__sfr __at(P_DARKY_PSG2_IDX) g_Darky_PSG2_IndexPort;

// 4DH W: PSG 2 Data write
#define P_DARKY_PSG2_DATA			0x4D
__sfr __at(P_DARKY_PSG2_DATA) g_Darky_PSG2_DataPort;

// 4EH R: PSG 2 Data read
#define P_DARKY_PSG2_READ			0x4E
__sfr __at(P_DARKY_PSG2_READ) g_Darky_PSG2_ReadPort;
