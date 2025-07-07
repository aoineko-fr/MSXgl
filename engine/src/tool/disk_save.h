// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄       ▄▄       ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ▄   ██▀ ██▄▀    ▀█▄  ▄▀██ █ ██ ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ██ ▄██  ██ █    ▄▄█▀ ▀▄██ ▀▄▀  ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Module: Disk save helper module (DOS wrapper)
//
// Features to handle save data on disk using MSX-DOS commands.
// Based on Wolfgang "dotmos" Reichardt's work.
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "msxgl.h"

//=============================================================================
// OPTIONS VALIDATION
//=============================================================================

//=============================================================================
// DEFINES
//=============================================================================

// Save data slot state
enum SAVEDATA_STATE
{
	SAVEDATA_NODISK = 0,	// No disk present
	SAVEDATA_NOTFOUND,		// No save data found
	SAVEDATA_INVALID,		// Save data is present but invalid
	SAVEDATA_VALID,			// Save data is valid
};

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: DiskSave_Check
// Check if a save data slot is valid
//
// Parameters:
//   slot - Save data slot
//
// Return:
// * SAVEDATA_NODISK if no disk is present
// * SAVEDATA_NOTFOUND if no save data is found
// * SAVEDATA_INVALID if save data is present but invalid
// * SAVEDATA_VALID if save data is valid
u8 DiskSave_Check(u8 slot);

// Function: DiskSave_Save
// Save data to a file
//
// Parameters:
//   slot - Save data slot
//   data - Pointer to the data to save
//   size - Size of the data to save in bytes
//
// Return:
//   FALSE if the save data could not be saved
bool DiskSave_Save(u8 slot, const u8* data, u16 size);

// Function: DiskSave_Load
// Load saved data from a file
//
// Parameters:
//   slot - Save data slot
//   data - Pointer to the buffer where the data will be loaded
//   size - Size of the data to load in bytes
//
// Return:
//   FALSE if the save data could not be loaded
bool DiskSave_Load(u8 slot, u8* data, u16 size);

// Function: DiskSave_Delete
// Delete saved data from a file
//
// Parameters:
//   slot - Save data slot
//
// Return:
//   FALSE if the save data could not be deleted
bool DiskSave_Delete(u8 slot);