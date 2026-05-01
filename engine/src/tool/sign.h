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

#define APPSIGN_SNAKE_ESCAPE		"2NSE"
#define APPSIGN_SUPER_SOKOBAN		"2NSO"
#define APPSIGN_DELTA				"5BR3"
#define APPSIGN_CHRONO_RUNNER		"AMCR"
#define APPSIGN_EGGYS_MAZE			"B5EM"
#define APPSIGN_PHENIX_CORRUPTA		"CCPC"
#define APPSIGN_SOUL_TRAPPED		"CCTR"
#define APPSIGN_A_LONE_WOLF			"FDLW"
#define APPSIGN_SAM_PR_EXPLORE		"FPEX"
#define APPSIGN_MARTIAN_WAR			"FPMW"
#define APPSIGN_SOCCER_LEAGUE		"FPSL"
#define APPSIGN_SAM_PR				"FPSP"
#define APPSIGN_MUGI				"HNMG"
#define APPSIGN_ROOM5				"MOR5"
#define APPSIGN_CRAWLERS			"PH\1\0"
#define APPSIGN_FINAL_SMASH			"PH\2\0"
#define APPSIGN_PENGPONG			"PH\3\0"
#define APPSIGN_WIZZL				"PH\4\0"
#define APPSIGN_ASTROJUMP			"RGAJ"
#define APPSIGN_TERMSX_MULTI		"RGTM"
#define APPSIGN_PETSCII_ROBOTS		"RSPE"
#define APPSIGN_TINY_TETRIS_VS		"TT4P"
#define APPSIGN_DUAEL_DROP			"TTDD"
#define APPSIGN_DOOMLINGS			"TTDL"
#define APPSIGN_TETPUZ				"TTTP"

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: AppSign_Check
// Check if the application signature is present in any slot
//
// Parameters:
//   creatorID - Creator ID of the application to search for
//   appID - Application ID of the application to search for
//
// Returns:
//   FALSE if the application signature is not found
bool AppSign_Check(const AppSign sign);

// Function: AppSign_CheckList
// Check all the application signatures in the list and return the first found application's index
//
// Parameters:
//   appNum - Number of applications in the list
//   signList - List of application signatures to check (8 max)
//
// Returns:
//   The index of the first found application, or APPSIGN_NONE if not found
u8 AppSign_CheckList(u8 appNum, const AppSign* appList);