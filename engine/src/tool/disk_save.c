// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄       ▄▄       ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ▄   ██▀ ██▄▀    ▀█▄  ▄▀██ █ ██ ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ██ ▄██  ██ █    ▄▄█▀ ▀▄██ ▀▄▀  ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Disk save helper module (DOS wrapper)
//
// Features to handle save data on disk using MSX-DOS commands.
// Based on Wolfgang "dotmos" Reichardt's work.
//─────────────────────────────────────────────────────────────────────────────

#include "msxgl.h"
#include "tool/disk_save.h"
#include "dos.h"
#include "bios_diskrom.h"
#include "bios_hook.h"
#include "memory.h"

//=============================================================================
// DEFINES
//=============================================================================

#if (defined(APPSIGN))
extern const u32 g_AppSignature;
#endif

//=============================================================================
// VARIABLES
//=============================================================================

// Base name of the save data file (must be 6 characters max).
const c8* g_DiskSave_Name = "GAME";

// Extension of the save data file (must be 3 characters max).
const c8* g_DiskSave_Ext = "SAV";

// struct for handling file operations
DOS_FCB g_DiskSave_FCB;

// Buffer for reading/writing a file in 128 byte chunks. 128 byte chunk "limitation" is set by the MSX and we can't change that behaviour.
char g_DiskSave_Buffer[128]; 

//=============================================================================
// CONSTANTS
//=============================================================================

// Table use to quick decimal-to-hexadecimal conversion
const c8 g_HexChar2[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Error handler
void DiskSave_ErrorHandler() __NAKED
{
__asm
_disksave_errorhandler_start:
	// Backup error code
	ld		a, c					;// Get error code
	or		a, #0x70				;// Add marker so 0 is no error
	ld		(_g_DOS_LastError), a	;// Save the error code
	// Restore stack pointer and C frame counter
	ld		sp, (_g_DOS_ErrorStack)	;// Restore stack pointer
	pop		ix
	// Restore ROM slot in page 1
	ld		h, #0b01000000			;// Page 1
	ld		a, (#_g_ROMSlotID)
	call	R_ENASLT
	ret
_disksave_errorhandler_end:
_g_DiskSave_ErrorHandlerSize::
	.db		_disksave_errorhandler_end - _disksave_errorhandler_start
__endasm;
}

extern const u8 g_DiskSave_ErrorHandlerSize;

//-----------------------------------------------------------------------------
// Detect if a disk drive is present and valid disk is inserted
u8 DiskSave_Initialize()
{
	if (Peek(H_PHYD) == 0xC9) // Check if a disk drive is present ; H.PHYD is not 0xC9 ('ret') if a disk drive is present
		return SAVEDATA_NODRIVE;

	// Install the error handler
	DOS_InstallErrorHandler(DiskSave_ErrorHandler, g_DiskSave_ErrorHandlerSize);

	// Clear the FCB structure
	Mem_Set(0, &g_DiskSave_FCB, sizeof(DOS_FCB));

	// Build the filename for the given slot
	Mem_Copy("TESTDATABIN", g_DiskSave_FCB.Name, 11);

	if (DOS_CreateFCB(&g_DiskSave_FCB) != DOS_ERR_NONE)
		return SAVEDATA_NODISK; // There was an error
	
	if (DOS_OpenFCB(&g_DiskSave_FCB) != DOS_ERR_NONE)
		return SAVEDATA_NODISK; // There was an error

	// Point to data that should be written to file
	DOS_SetTransferAddr(g_DiskSave_Buffer);
	
	// Write data, 128 bytes per write
	if (DOS_SequentialWriteFCB(&g_DiskSave_FCB) != DOS_ERR_NONE)
		return SAVEDATA_NODISK; // There was an error
	
	// Try to delete the file
	if (DOS_DeleteFCB(&g_DiskSave_FCB) != DOS_ERR_NONE)
		return SAVEDATA_NODISK; // There was an error

	DOS_CloseFCB(&g_DiskSave_FCB);
	
	return SAVEDATA_VALID;
}


//-----------------------------------------------------------------------------
// Build the filename for a given save data slot
void DiskSave_BuildFilename(u8 slot, c8* buffer, bool pad)
{
	// Clear the FCB structure
	Mem_Set(0, buffer, 13);

	// Copy the base name to the FCB name field
	u8 i;
	for (i = 0; (i < 6) && (g_DiskSave_Name[i] != 0); ++i)
		buffer[i] = g_DiskSave_Name[i];

	// Add slot number as hexadecimal digits
	buffer[i++] = g_HexChar2[slot >> 4];
	buffer[i++] = g_HexChar2[slot & 0xF];

	if (pad)
	{
		// Fill the rest of the name with spaces
		for (; i < 8; ++i)
			buffer[i] = ' ';

		// Add the extension
		for (; i < 11; ++i)
			buffer[i] = g_DiskSave_Ext[i - 8];
	}
	else
	{
		buffer[i++] = '.';
		// Add the extension
		loop(j, 3)
			buffer[i++] = g_DiskSave_Ext[j];

		// buffer[i++] = g_DiskSave_Ext[0];
		// buffer[i++] = g_DiskSave_Ext[1];
		// buffer[i]   = g_DiskSave_Ext[2];
	}
}

//-----------------------------------------------------------------------------
// Initialize the FCB structure for a given save data slot
void DiskSave_InitFCB(u8 slot)
{
	// Clear the FCB structure
	Mem_Set(0, &g_DiskSave_FCB, sizeof(DOS_FCB));

	// Build the filename for the given slot
	DiskSave_BuildFilename(slot, g_DiskSave_FCB.Name, TRUE);
}

//-----------------------------------------------------------------------------
// Check if a save data slot is valid
u8 DiskSave_Check(u8 slot)
{
	// Point g_DiskSave_FCB to the file that should be opened
	DiskSave_InitFCB(slot);
	
	if (DOS_OpenFCB(&g_DiskSave_FCB) != DOS_ERR_NONE)
		return SAVEDATA_NOTFOUND;  // There was an error / file not found
	
#if (defined(APPSIGN))

	// Point to data that should be read from file
	DOS_SetTransferAddr(g_DiskSave_Buffer);

	//Read the first 128 bytes
	if ( DOS_SequentialReadFCB(&g_DiskSave_FCB) != DOS_ERR_NONE)
		return SAVEDATA_ERROR; // Invalid file

	// Check the save data signature
	const u8* sign = (const u8*)&g_AppSignature;
	loop(i, 4)
		if (g_DiskSave_Buffer[i] != *sign++)
			return SAVEDATA_UNSIGNED; // Invalid signature

#endif

	DOS_CloseFCB(&g_DiskSave_FCB);
	
	return SAVEDATA_VALID;
}

//-----------------------------------------------------------------------------
// Save data to a file
bool DiskSave_Save(u8 slot, const u8* data, u16 size)
{
	// Point g_DiskSave_FCB to the file that should be opened/created
	DiskSave_InitFCB(slot);
	
	if (DOS_CreateFCB(&g_DiskSave_FCB) != DOS_ERR_NONE)
		return FALSE; // There was an error
	
	if (DOS_OpenFCB(&g_DiskSave_FCB) != DOS_ERR_NONE)
		return FALSE; // There was an error
	
	// Point to data that should be written to file
	DOS_SetTransferAddr(g_DiskSave_Buffer);
	
	i16 remains = size;
	for (u16 i = 0; i < size; i += 128)
	{
		//Copy from data to g_DiskSave_Buffer.
		remains -= 128;
		if (remains < 0)
		{
			remains += 128;
			Mem_Copy(data, g_DiskSave_Buffer, remains);
		}
		else
		{
			Mem_Copy(data, g_DiskSave_Buffer, 128);
			data += 128;
		}
		
		//Write data, 128 bytes per write
		if (DOS_SequentialWriteFCB(&g_DiskSave_FCB) != DOS_ERR_NONE)
			return FALSE; // There was an error
	}
	
	DOS_CloseFCB(&g_DiskSave_FCB);
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// Load saved data from a file
bool DiskSave_Load(u8 slot, u8* dst, u16 size)
{
	// Point g_DiskSave_FCB to the file that should be opened
	DiskSave_InitFCB(slot);
	
	// Try to open the file
	if (DOS_OpenFCB(&g_DiskSave_FCB) != DOS_ERR_NONE)
		return FALSE; // There was an error
	
	// Point to data that should be read from file
	DOS_SetTransferAddr(g_DiskSave_Buffer);
	i16 remains = size;
	for (u16 i = 0; i < size; i += 128)
	{
		//Read data, 128 bytes per read
		if ( DOS_SequentialReadFCB(&g_DiskSave_FCB) != DOS_ERR_NONE)
			return FALSE; // There was an error

		//Copy from g_DiskSave_Buffer to dst.
		remains -= 128;
		if (remains < 0)
		{
			remains += 128;
			Mem_Copy(g_DiskSave_Buffer, dst, remains);
			break;
		}
		else
		{
			Mem_Copy(g_DiskSave_Buffer, dst, 128);
			dst += 128;
		}
	}
	
	// Close file
	DOS_CloseFCB(&g_DiskSave_FCB);
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// Delete saved data from a file
bool DiskSave_Delete(u8 slot)
{
	// Point g_DiskSave_FCB to the file that should be opened
	DiskSave_InitFCB(slot);
	
	// Try to open the file
	if (DOS_OpenFCB(&g_DiskSave_FCB) != DOS_ERR_NONE)
		return FALSE; // There was an error

	// Try to delete the file
	if (DOS_DeleteFCB(&g_DiskSave_FCB) != DOS_ERR_NONE)
		return FALSE; // There was an error

	// Close file
	DOS_CloseFCB(&g_DiskSave_FCB);

	return TRUE;
}