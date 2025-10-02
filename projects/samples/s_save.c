// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "fsm.h"
#include "tool/disk_save.h"
#include "device/pac.h"


//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

//
#define PAGE_CURSOR 12

// State function prototypes
void InitMain();
void UpdateMain();
void InitPAC();
void UpdatePAC();
void InitDisk();
void UpdateDisk();
// void InitRTC();
// void UpdateRTC();

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Fake save data
const u8 g_DataWrite[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

// States
const FSM_State g_StateMain = { 0, InitMain, UpdateMain, NULL };
const FSM_State g_StatePAC  = { 0, InitPAC,  UpdatePAC,  NULL };
const FSM_State g_StateDisk = { 0, InitDisk, UpdateDisk, NULL };
// const FSM_State g_StateRTC  = { 0, InitRTC,  UpdateRTC,  NULL };

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_FrameCount = 0; // Frame counter

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// 
void InitPage(const c8* header, const c8* footer)
{
	VDP_ClearVRAM();

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample6, 1);

	// Header
	Print_DrawText(MSX_GL " ");
	Print_DrawText(header);
	Print_DrawLineH(0, 1, 40);

	// Footer
	Print_DrawLineH(0, 22, 40);
	Print_DrawTextAt(0, 23, footer);
}


//#############################################################################
//  ▄▄▄   ▄▄   ▄▄
//  ██▄▀ ██▄█ ██ ▀
//  ██   ██ █ ▀█▄▀
//#############################################################################

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_PACDevice = 0;
u8 g_PACPage = 0;
u8 g_PACData[8];

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Draw slot ID
void Print_DrawSlot(u8 slot)
{
	Print_DrawInt(Sys_SlotGetPrimary(slot));
	if (Sys_SlotIsExpended(slot))
	{
		Print_DrawChar('-');
		Print_DrawInt(Sys_SlotGetSecondary(slot));
	}
}

//-----------------------------------------------------------------------------
// Get the name of the status
const c8* PACGetStatName(u8 stat)
{
	switch (stat)
	{
	case PAC_CHECK_EMPTY: return "Empty";
	#if (PAC_USE_SIGNATURE)
	case PAC_CHECK_APP:   return "Appli";
	#endif
	}
	return "Undef";
}

//-----------------------------------------------------------------------------
//  Set the current page
void PACSetPage(u8 page)
{
	Print_SetPosition(0, PAGE_CURSOR + g_PACPage);
	Print_DrawChar(' ');

	Print_SetPosition(0, PAGE_CURSOR + page);
	Print_DrawChar(0x8A);
	g_PACPage = page;
}

//-----------------------------------------------------------------------------
// Draw the current page
void PACDrawPage(u8 page)
{
	u8 stat = PAC_Check(page);
	PAC_Read(page, g_PACData, 8);

	Print_SetPosition(2, PAGE_CURSOR + page);
	Print_DrawCharX(' ', 40);

	Print_SetPosition(2, PAGE_CURSOR + page);
	if (PAC_GetSlot(g_PACDevice) != SLOT_NOTFOUND)
	{
		Print_DrawFormat("%i | %s | %2x %2x %2x %2x %2x %2x %2x %2x", page, PACGetStatName(stat),
			g_PACData[0], g_PACData[1], g_PACData[2], g_PACData[3], g_PACData[4], g_PACData[5], g_PACData[6], g_PACData[7]);
	}
	else
		Print_DrawText("  |       |");
}

//-----------------------------------------------------------------------------
// Set the current device
void PACSetDevice(u8 dev)
{
	g_PACDevice = dev;
	PAC_Select(g_PACDevice);
	PAC_Activate(TRUE);

	Print_SetPosition(20, 4);
	Print_DrawCharX(' ', 20);

	Print_SetPosition(20, 4);
	Print_DrawFormat(" (%i) ", g_PACDevice);
	if (PAC_GetSlot(g_PACDevice) == SLOT_NOTFOUND)
		Print_DrawText("Not found");
	else
		Print_DrawSlot(PAC_GetSlot(g_PACDevice));

	PACSetPage(0);
	for (u8 i = 0; i < PAC_PAGE_MAX; ++i)
		PACDrawPage(i);

	Print_SetPosition(0, PAGE_CURSOR + PAC_PAGE_MAX);
	Print_DrawText("----+-------+-------------------------");
}

//-----------------------------------------------------------------------------
// PAC SRAM state
void InitPAC()
{
	InitPage("SAVE SAMPLE - PAC", "\x8D:Device \x82:Page W:Write E:Erase A:All");
	
	bool bFound = PAC_Initialize();

	Print_DrawTextAt(0, 3, "Devices:");
	Print_DrawTextAt(20, 3, "Current:");

	for (u8 i = 0; i < PAC_DEVICE_MAX; ++i)
	{
		Print_SetPosition(0, 4 + i);
		Print_DrawFormat(" %i: ", i);
		if (PAC_GetSlot(i) == SLOT_NOTFOUND)
			Print_DrawText("Not found");
		else
			Print_DrawSlot(PAC_GetSlot(i));
	}

	Print_SetPosition(0, PAGE_CURSOR - 2);
	Print_DrawText("Page  Stat    Data");
	Print_SetPosition(0, PAGE_CURSOR - 1);
	Print_DrawText("----+-------+-------------------------");
	Print_SetPosition(0, PAGE_CURSOR + PAC_PAGE_MAX);
	Print_DrawText("----+-------+-------------------------");

	PACSetDevice(0);
}

//-----------------------------------------------------------------------------
// PAC SRAM state
void UpdatePAC()
{
	Print_DrawCharAt(39, 0, g_ChrAnim[g_FrameCount++ & 0x03]);

	if (Keyboard_IsKeyPressed(KEY_UP))
	{
		PACSetPage((g_PACPage + PAC_PAGE_MAX - 1) % PAC_PAGE_MAX);
		while (Keyboard_IsKeyPressed(KEY_UP)) {}
	}
	if (Keyboard_IsKeyPressed(KEY_DOWN))
	{
		PACSetPage((g_PACPage + 1) % PAC_PAGE_MAX);
		while (Keyboard_IsKeyPressed(KEY_DOWN)) {}
	}

	if (Keyboard_IsKeyPressed(KEY_RIGHT))
	{
		g_PACDevice = (g_PACDevice + 1) % PAC_DEVICE_MAX;
		PACSetDevice(g_PACDevice);
	}
	if (Keyboard_IsKeyPressed(KEY_LEFT))
	{
		g_PACDevice = (g_PACDevice + PAC_DEVICE_MAX - 1) % PAC_DEVICE_MAX;
		PACSetDevice(g_PACDevice);
	}
	if (Keyboard_IsKeyPressed(KEY_W))
	{
		PAC_Write(g_PACPage, g_DataWrite, sizeof(g_DataWrite));
		PACDrawPage(g_PACPage);
	}
	if (Keyboard_IsKeyPressed(KEY_E))
	{
		PAC_Format(g_PACPage);
		PACDrawPage(g_PACPage);
	}
	if (Keyboard_IsKeyPressed(KEY_A))
	{
		PAC_FormatAll();
		PACSetDevice(g_PACDevice);
	}

	if (Keyboard_IsKeyPressed(KEY_ESC))
		FSM_SetState(&g_StateMain);
}

//#############################################################################
//  ▄▄▄  ▄       ▄▄       ▄▄▄
//  ██ █ ▄   ██▀ ██▄▀    ▀█▄  ▄▀██ █ ██ ▄███
//  ██▄▀ ██ ▄██  ██ █    ▄▄█▀ ▀▄██ ▀▄▀  ▀█▄▄
//#############################################################################

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_DiskSaveEntry = 0;
u8 g_DiskSaveBuffer[128];
u8 g_DiskSaveLog = 8;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Get the string representation of the disk save state
const c8* DiskGetStateString(u8 state)
{
	switch (state)
	{
	case SAVEDATA_NODRIVE:  return "No drive";
	case SAVEDATA_NODISK:   return "No disk";
	case SAVEDATA_NOTFOUND: return "Not found";
	case SAVEDATA_ERROR:    return "Error";
	case SAVEDATA_UNSIGNED: return "Unsigned";
	case SAVEDATA_VALID:    return "Valid";
	}
	return "Unknown";
}

//-----------------------------------------------------------------------------
// Set the current disk save entry
void DiskSetEntry(u8 entry)
{
	// Set entry
	g_DiskSaveEntry = entry;
	Print_DrawCharXAt(6, 3, ' ', 24); // Clear line
	Print_DrawHex8At(7, 3, g_DiskSaveEntry);
	DiskSave_BuildFilename(g_DiskSaveEntry, g_DiskSaveBuffer, FALSE);
	Print_DrawTextAt(11, 3, g_DiskSaveBuffer);

	// Check entry state
	Print_DrawCharXAt(6, 4, ' ', 24); // Clear line
	u8 state = DiskSave_Check(g_DiskSaveEntry);
	Print_DrawHex8At(7, 4, state);
	Print_DrawTextAt(11, 4, DiskGetStateString(state));
}

//-----------------------------------------------------------------------------
// Log a message in the disk save log
void DiskSaveLog(const c8* text)
{
	Print_DrawCharXAt(0, g_DiskSaveLog, ' ', 40); // Clear line
	Print_DrawTextAt(0, g_DiskSaveLog++, text);
	if (g_DiskSaveLog > 20) // Reset log if it exceeds the screen height
		g_DiskSaveLog = 8;
}

//-----------------------------------------------------------------------------
// Initialize the disk save state
void InitDisk()
{
	InitPage("SAVE SAMPLE - DISK", "\x8D:Entry S:Save L:Load D:Del ESC:Exit");

	DiskSave_Initialize();
	DiskSave_SetName("SAVE"); // Set the base name of the save data file
	DiskSave_SetExtension("BIN"); // Set the extension of the save data file

	Print_DrawTextAt(0, 3, "Entry:");
	Print_DrawTextAt(0, 4, "State:");
	Print_DrawTextAt(0, 6, "Log:");

	DiskSetEntry(0);

	DiskSaveLog("Drives: ");		Print_DrawBin8(DiskSave_AvailableDrives());
	DiskSaveLog("Current: ");		Print_DrawChar('A' + DiskSave_GetCurrentDrive());
	DiskSaveLog("Free entries: ");	Print_DrawInt(DiskSave_GetFreeEntries());
}

//-----------------------------------------------------------------------------
// Update the disk save state
void UpdateDisk()
{
	Print_DrawCharAt(39, 0, g_ChrAnim[g_FrameCount++ & 0x03]);

	// Change current entry
	if (Keyboard_IsKeyPressed(KEY_LEFT) && (g_DiskSaveEntry > 0))
		DiskSetEntry(g_DiskSaveEntry - 1);
	else if (Keyboard_IsKeyPressed(KEY_RIGHT) && (g_DiskSaveEntry < 0xFF))
		DiskSetEntry(g_DiskSaveEntry + 1);

	if (Keyboard_IsKeyPressed(KEY_S))
	{
		if (DiskSave_Save(g_DiskSaveEntry, g_DataWrite, sizeof(g_DataWrite)))
		{
			DiskSaveLog("Saved data [");
			loop(i, 8)
			{
				if (i) Print_DrawChar(',');
				Print_DrawHex8(g_DataWrite[i]);
			}
			Print_DrawChar(']');
			DiskSetEntry(g_DiskSaveEntry);
		}
		else
			DiskSaveLog("Failed to save data.");
	}
	else if (Keyboard_IsKeyPressed(KEY_L))
	{
		Mem_Set(0, &g_DiskSaveBuffer, 128); // Clear the buffer
		if (DiskSave_Load(g_DiskSaveEntry, g_DiskSaveBuffer, sizeof(g_DataWrite)))
		{
			DiskSaveLog("Loaded data [");
			loop(i, 8)
			{
				if (i) Print_DrawChar(',');
				Print_DrawHex8(g_DiskSaveBuffer[i]);
			}
			Print_DrawChar(']');
		}
		else
			DiskSaveLog("Failed to load data.");
	}
	else if (Keyboard_IsKeyPressed(KEY_D))
	{
		if (DiskSave_Delete(g_DiskSaveEntry))
		{
			DiskSaveLog("Data deleted!");
			DiskSetEntry(g_DiskSaveEntry);
		}
		else
			DiskSaveLog("Failed to delete data.");
	}

	if (Keyboard_IsKeyPressed(KEY_ESC))
		FSM_SetState(&g_StateMain);
}

//#############################################################################
//   ▄ ▄      ▄
//  ██▀█ ▄▀██ ▄  ██▀▄
//  ██ █ ▀▄██ ██ ██ █
//#############################################################################

//-----------------------------------------------------------------------------
// 
void InitMain()
{
	InitPage("SAVE SAMPLE", "1:PAC 2:Disk 3:RTC");

	// PAC
	Print_SetPosition(0, 3);
	Print_DrawFormat("1. PAC Cartridge (%s)", PAC_Initialize() ? "Found" : "Not found");

	// Disk
	Print_SetPosition(0, 4);
	Print_DrawFormat("2. Disk Save (%s)", DiskGetStateString(DiskSave_Initialize()));

	// RTC
	Print_SetPosition(0, 5);
	Print_DrawText("3. Realtime Clock");
}

//-----------------------------------------------------------------------------
// 
void UpdateMain()
{
	Print_DrawCharAt(39, 0, g_ChrAnim[g_FrameCount++ & 0x03]);

	if (Keyboard_IsKeyPressed(KEY_1))
		FSM_SetState(&g_StatePAC);
	else if (Keyboard_IsKeyPressed(KEY_2))
		FSM_SetState(&g_StateDisk);
	// else if (Keyboard_IsKeyPressed(KEY_3))
	// 	FSM_SetState(&g_StateRTC);
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	Bios_SetKeyClick(FALSE);

	// Initialize screen mode 0 (text)
	VDP_SetMode(VDP_MODE_SCREEN0);

	FSM_SetState(&g_StateMain);

	while (1)
	{
		Halt();

		FSM_Update();
	}


}