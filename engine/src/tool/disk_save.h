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
	SAVEDATA_NODRIVE = 0,	// No disk dirve present
	SAVEDATA_NODISK,		// No disk present
	SAVEDATA_NOTFOUND,		// No save data found
	SAVEDATA_ERROR,			// Save data is present but invalid
	SAVEDATA_UNSIGNED,		// Save data is present but not signed
	SAVEDATA_VALID,			// Save data is valid
};

// Base name of the save data file (must be 6 characters max)
extern const c8* g_DiskSave_Name;

// Extension of the save data file (must be 3 characters max).
extern const c8* g_DiskSave_Ext;

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: DiskSave_Initialize
// Detect if a disk drive is present and valid disk is inserted
//
// Return:
//   SAVEDATA_NODRIVE if no disk drive is present
//   SAVEDATA_NODISK if no disk is present
//   SAVEDATA_VALID if a valid disk is present
u8 DiskSave_Initialize();

// Function: DiskSave_SetName
// Set the base name of the save data file
//
// Parameters:
//   name - Pointer to the ASCIIZ string with the name (must be 6 characters max)
//          The name will be used as the base for the save data file, e.g. "GAME" will create files like "GAME00.SAV", "GAME01.SAV", etc.
inline void DiskSave_SetName(const c8* name) { g_DiskSave_Name = name; }

// Function: DiskSave_SetExtension
// Set the extension of the save data file
//
// Parameters:
//   ext - Pointer to the ASCIIZ string with the extension (must be 3 characters max)
//         The extension will be used for the save data file, e.g. "SAV" will create files like "GAME00.SAV", "GAME01.SAV", etc.
inline void DiskSave_SetExtension(const c8* ext) { g_DiskSave_Ext = ext; }

// Function: DiskSave_BuildFilename
// Build the filename for a given save data slot
//
// Parameters:
//   slot   - Save data slot
//   buffer - Pointer to the buffer where the filename will be stored (must be at least 13 bytes long)
//   pad    - If TRUE, the filename will be padded with spaces to fill 8 characters
void DiskSave_BuildFilename(u8 slot, c8* buffer, bool pad);

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