// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄           ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ▄█▀▄ ▄█▀▄ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │   ██  ▀█▄▀ ▀█▄▀ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Application (Konami style) signature detection
//   Search on all slot for a specific application signatures
//   https://aoineko.org/msxgl/index.php?title=AppSignature
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

// Signature structure
typedef c8* AppSign;

#define APPSIGN_NONE				0xFF	// Value returned when no signature is found

// List of known application signatures
#define APPSIGN_SNAKE_ESCAPE		"2NSE"   // 2NICE - Snake Escape
#define APPSIGN_SUPER_SOKOBAN		"2NSO"   // 2NICE - Super Sokoban
#define APPSIGN_CHRONO_RUNNER		"AMCR"   // Associazione MSX Italia - Chrono Runner
#define APPSIGN_EGGYS_MAZE			"B5EM"   // Bik5 - Eggy's Maze
#define APPSIGN_DELTA				"B5R3"   // Bik5 - DELTA
#define APPSIGN_PHENIX_CORRUPTA		"CCPC"   // Casper Croes - Phenix Corrupta
#define APPSIGN_SOUL_TRAPPED		"CCTR"   // Casper Croes - Soul Trapped
#define APPSIGN_A_LONE_WOLF			"FDLW"   // FutureDisk - A Lone Wolf
#define APPSIGN_SAM_PR_EXPLORE		"FPEX"   // Fausto Pracek - Sam PR Explore
#define APPSIGN_MARTIAN_WAR			"FPMW"   // Fausto Pracek - Martian War
#define APPSIGN_SOCCER_LEAGUE		"FPSL"   // Fausto Pracek - Soccer League
#define APPSIGN_SAM_PR				"FPSP"   // Fausto Pracek - Sam PR
#define APPSIGN_MUGI				"HNMG"   // Hando - MUGI
#define APPSIGN_ROOM5				"MOR5"   // MO5 Association - Room5
#define APPSIGN_CRAWLERS			"PH\1\0" // Pixel Phenix - Crawlers
#define APPSIGN_FINAL_SMASH			"PH\2\0" // Pixel Phenix - Final Smash
#define APPSIGN_PENGPONG			"PH\3\0" // Pixel Phenix - Pengpong
#define APPSIGN_WIZZL				"PH\4\0" // Pixel Phenix - Wizzl
#define APPSIGN_ASTROJUMP			"RGAJ"   // Astrojump
#define APPSIGN_TERMSX_MULTI		"RGTM"   // Rutagames - TermSX Multi
#define APPSIGN_PETSCII_ROBOTS		"RSPE"   // RoboSoft - Petscii Robots
#define APPSIGN_TINY_TETRIS_VS		"TT4P"   // Antxiko - Tiny Tetris VS
#define APPSIGN_DUAEL_DROP			"TTDD"   // Totta - Duael Drop
#define APPSIGN_DOOMLINGS			"TTDL"   // Totta - Doomlings
#define APPSIGN_TETPUZ				"TTTP"   // Totta - Tetpuz

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: AppSign_Check
// Check if the application signature is present in any slot
//
// Parameters:
//   sign - Application signature to search for
//
// Returns:
//   FALSE if the application signature is not found
bool AppSign_Check(const AppSign sign);

// Function: AppSign_CheckList
// Check all the application signatures in the list and return the first found application's index
//
// Parameters:
//   appNum - Number of applications in the list
//   appList - List of application signatures to check (8 max)
//
// Returns:
//   The index of the first found application, or APPSIGN_NONE if not found
u8 AppSign_CheckList(u8 appNum, const AppSign* appList);