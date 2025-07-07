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
#include "memory.h"

//=============================================================================
// OPTIONS VALIDATION
//=============================================================================

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// VARIABLES
//=============================================================================

// struct for handling file operations
DOS_FCB g_FileFCB;

// Buffer for reading/writing a file in 128 byte chunks. 128 byte chunk "limitation" is set by the MSX and we can't change that behaviour.
static char g_FileBuffer[128]; 

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Check if a save data slot is valid
u8 DiskSave_Check(u8 slot)
{
	// Point g_FileFCB to the file that should be opened
	Mem_Set(0, &g_FileFCB, sizeof(DOS_FCB));
	Mem_Copy(saveSlots[slot], &g_FileFCB.Name, FILENAME_LENGTH);
	
	if( DOS_OpenFCB(&g_FileFCB) != DOS_ERR_NONE)
		return SAVEDATA_NOTFOUND;  // There was an error / file not found
	
	DOS_CloseFCB(&g_FileFCB);
	
	return SAVEDATA_VALID;
}

//-----------------------------------------------------------------------------
// Save data to a file
bool DiskSave_Save(u8 slot, const u8* data, u16 size)
{
	// Point g_FileFCB to the file that should be opened/created
	Mem_Set(0, &g_FileFCB, sizeof(DOS_FCB));
	Mem_Copy(saveSlots[slot], &g_FileFCB.Name, FILENAME_LENGTH);
	
	if(DOS_CreateFCB(&g_FileFCB) != DOS_ERR_NONE)
		return FALSE; // There was an error
	
	if(DOS_OpenFCB(&g_FileFCB) != DOS_ERR_NONE)
		return FALSE; // There was an error
	
	// Point to data that should be written to file
	DOS_SetTransferAddr(g_FileBuffer);
	
	i16 remains = length;
	for(u16 i = 0; i < length; i += 128)
	{
		//Copy from data to g_FileBuffer.
		remains -= 128;
		if(remains < 0){
			remains += 128;
			Mem_Copy(data, g_FileBuffer, remains);
		} else {
			Mem_Copy(data, g_FileBuffer, 128);
			data += 128;
		}
		
		//Write data, 128 bytes per write. 128 bytes is defined by MSX and we can't change it
		if(DOS_SequentialWriteFCB(&g_FileFCB) != DOS_ERR_NONE){
			//There was an error
			return false;
		}
	}
	
	DOS_CloseFCB(&g_FileFCB);
	
	return true;
}


// Load data from a slot
static bool load(const uint8_t slot, uint8_t* dst, const uint16_t length) {
	// Point g_FileFCB to the file that should be opened
	Mem_Set(0, &g_FileFCB, sizeof(DOS_FCB));
	Mem_Copy(saveSlots[slot], &g_FileFCB.Name, FILENAME_LENGTH);
	
	// Try to open the file
	if(DOS_OpenFCB(&g_FileFCB) != DOS_ERR_NONE) {
		//There was an error
		return false;
	}
	
	// Point to data that should be read from file
	DOS_SetTransferAddr(g_FileBuffer);
	int16_t remains = length;
	for(uint16_t i=0; i<length; i+=128) {
		//Read data, 128 bytes per read. 128 bytes is defined by MSX DOS and we can't change it
		if( DOS_SequentialReadFCB(&g_FileFCB) != DOS_ERR_NONE) {
			//There was an error
			return false;
		}
		//Copy from g_FileBuffer to dst.
		remains -= 128;
		if(remains < 0) {
			remains += 128;
			Mem_Copy(g_FileBuffer, dst, remains);
			break;
		} else {
			Mem_Copy(g_FileBuffer, dst, 128);
			dst += 128;
		}
	}
	
	//Close file
	DOS_CloseFCB(&g_FileFCB);
	
	return true;
}