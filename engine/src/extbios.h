// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄   ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██ █ ▀█▄
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▀█▄▀ ▄▄█▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// MSX-DOS 2 extension BIOS routines
//
// References:
// - https://map.grauw.nl/resources/dos2_environment.php#c5
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// DEFINES
//=============================================================================

// Extented BIOS 
#define M_HOKVLD					0xFB20
const u8 __at(M_HOKVLD) g_HOKVLD;
#define M_EXTBIO					0xFFCA
const u8 __at(M_EXTBIO) g_EXTBIO;

#define EXTBIOS_BROADCAST			0x00 // Broad-cast
#define EXTBIOS_MEMMAPPER			0x04 // Memory Mapper (MSX-DOS 2)
#define EXTBIOS_MODEM				0x08 // MSX-Modem & RS-232C
#define EXTBIOS_MSXAUDIO			0x0A // MSX-AUDIO
#define EXTBIOS_MSXJE				0x10 // MSX-JE
#define EXTBIOS_KANJI				0x11 // Kanji driver
#define EXTBIOS_UNAPI				0x22 // MSX-UNAPI (Unofficial)
#define EXTBIOS_MEMMAN				0x4D // MemMan (Unofficial)
#define EXTBIOS_UDRIVER				0x84 // μ-driver by Yoshikazu Yamamoto (Unofficial)
#define EXTBIOS_MULTIMENTE			0xF1 // MultiMente (Unofficial)
#define EXTBIOS_SYSTEM				0xFF // System

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: ExtBIOS_Check
// Check id extended BIOS is installed
inline bool ExtBIOS_Check() { return g_HOKVLD & 0x01; }