// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  MSX-DOS sample
//─────────────────────────────────────────────────────────────────────────────

#include "msxgl.h"
#include "dos.h"
#include "dos_mapper.h"
#include "extbios.h"
#include "memory.h"
#include "string.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL			"\x01\x02\x03\x04\x05\x06"

#define TAB_X			(128+16)
#define TAB_Y			32

#define PAGE_X			320
#define PAGE_Y			8


//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"

// Character animation
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// MEMORY DATA
//=============================================================================

// String buffer
c8 g_StrBuffer[128];

// String buffer
c8 g_SlotBuffer[4];

// File I/O buffer
u8 g_FileBuffer[4096]; // 32 lines of 4-bits data

// Log line
u8 g_LogLine = 120;

// Last segment
DOS_Segment g_LastSeg;

// Page 2 segment number
u8 g_Page2Seg;

// Frame counter
u8 g_Frame = 0;

// Previous KB row #0 value
u8 g_PrevRow0 = 0x00;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Print Slot ID
void Print_DrawSlot(u8 slot)
{
	Print_DrawInt(Sys_SlotGetPrimary(slot));
	if(Sys_SlotIsExpended(slot))
	{
		Print_DrawChar('-');
		Print_DrawInt(Sys_SlotGetSecondary(slot));
	}
}

//-----------------------------------------------------------------------------
// Format slot ID
const c8* FormatSlot(u8 slot)
{
	c8* str = g_SlotBuffer;
	*str++ = '0' + Sys_SlotGetPrimary(slot);
	if(Sys_SlotIsExpended(slot))
	{
		*str++ = '-';
		*str++ = '0' + Sys_SlotGetSecondary(slot);
	}
	*str = 0;
	return g_SlotBuffer;
}

//-----------------------------------------------------------------------------
// Display program header
void DisplayDOSHeader()
{
	DOS_ClearScreen();
	DOS_StringOutput("+--------------------------+\n\r$");
	DOS_StringOutput("| MSXgl - MSX-DOS 2 SAMPLE |\n\r$");
	DOS_StringOutput("+--------------------------+\n\r$");
}

//-----------------------------------------------------------------------------
// Display machine information
void DisplayInfo()
{
	// Program header
	DisplayDOSHeader();

	// Display VDP version
	u8 vdp = VDP_GetVersion();
	switch(vdp)
	{
	case VDP_VERSION_TMS9918A:	DOS_StringOutput("VDP: MSX1\n\r$"); break;
	case VDP_VERSION_V9938:		DOS_StringOutput("VDP: MSX2\n\r$"); break;
	case VDP_VERSION_V9958:		DOS_StringOutput("VDP: MSX2+\n\r$"); break;
	default:					DOS_StringOutput("VDP: Unknow\n\r$");
	}

	// // Display MSX-DOS version
	DOS_Version ver;
	u8 v = DOS_GetVersion(&ver);
	String_Format(g_StrBuffer, "Version: %d (Kernel: %1x.%2x | System: %1x.%2x)\n\r$", v, ver.Kernel >> 8, ver.Kernel & 0x00FF, ver.System >> 8, ver.System & 0x00FF);
	DOS_StringOutput(g_StrBuffer);

	// Display error explanation
	DOS_StringOutput("Explain 'IBDOS': $");
	DOS_Explain(DOS_ERR_IBDOS, g_StrBuffer);
	c8* ptr = g_StrBuffer;
	while(*ptr++) {}
	*ptr = '$';
	DOS_StringOutput(g_StrBuffer);
	DOS_Return();

	// Display default disk parameters
	DOS_DiskParam param;
	u8 err = DOS_GetDiskParam(DOS_DRIVE_DEFAULT, &param);
	DOS_StringOutput("Default disk paramters:\n\r$");
	String_Format(g_StrBuffer, "- Physical drive number: %c\n\r$", 'A' - 1 + param.DriveNum);
	DOS_StringOutput(g_StrBuffer);
	String_Format(g_StrBuffer, "- Sector size: %d\n\r$", param.SectorSize);
	DOS_StringOutput(g_StrBuffer);
	String_Format(g_StrBuffer, "- Sectors per cluster: %d\n\r$", param.ClusterSectors);
	DOS_StringOutput(g_StrBuffer);
	String_Format(g_StrBuffer, "- Number of reserved sectors: %d\n\r$", param.ReservedSectors);
	DOS_StringOutput(g_StrBuffer);
	String_Format(g_StrBuffer, "- Number of copies of the FAT: %d\n\r$", param.FATNum);
	DOS_StringOutput(g_StrBuffer);
	String_Format(g_StrBuffer, "- Number of root directory entries: %d\n\r$", param.RootNum);
	DOS_StringOutput(g_StrBuffer);
	String_Format(g_StrBuffer, "- Total number of logical sectors: %d\n\r$", param.LogicalSectors);
	DOS_StringOutput(g_StrBuffer);
	String_Format(g_StrBuffer, "- Media descriptor byte: %d\n\r$", param.Media);
	DOS_StringOutput(g_StrBuffer);
	String_Format(g_StrBuffer, "- Number of sectors per FAT: %d\n\r$", param.FATSectors);
	DOS_StringOutput(g_StrBuffer);
	String_Format(g_StrBuffer, "- First root directory sector number: %d\n\r$", param.RootSector);
	DOS_StringOutput(g_StrBuffer);
	String_Format(g_StrBuffer, "- First data sector number: %d\n\r$", param.DataSector);
	DOS_StringOutput(g_StrBuffer);
	String_Format(g_StrBuffer, "- Maximum cluster number: %d\n\r$", param.MaxClusters);
	DOS_StringOutput(g_StrBuffer);
	String_Format(g_StrBuffer, "- Dirty disk flag: %d\n\r$", param.DirtyFlag);
	DOS_StringOutput(g_StrBuffer);
	String_Format(g_StrBuffer, "- Volume id.: %d\n\r$", param.VolumeID);
	DOS_StringOutput(g_StrBuffer);

	DOS_StringOutput("\n\rPRESS A KEY...$");
	DOS_CharInput();
}

//-----------------------------------------------------------------------------
// Wait for key press
void WaitKeyPress()
{
	Print_DrawText("\nPRESS ANY KEY...");
	DOS_CharInput();
}

#if (DOS_USE_VALIDATOR)
//-----------------------------------------------------------------------------
// Display last error
void DisplayLastError()
{
	u8 err = DOS_GetLastError();
	DOS_Explain(err, g_StrBuffer);
	Print_DrawFormat("Error!\n%2x: %s\n", err, g_StrBuffer);
	WaitKeyPress();
	Bios_Exit(0);
}
#endif

//-----------------------------------------------------------------------------
// Display the page header
void DisplayHeader()
{
	Print_Clear();
	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL " MSX-DOS 2 Sample");
}

//-----------------------------------------------------------------------------
// Display a file content
void DisplayFile()
{
	// Initialize display system
	VDP_SetMode(VDP_MODE_SCREEN5);
	VDP_EnableSprite(FALSE);
	VDP_SetColor(0x4);

	u8 err;

	// Start bitmap screen
	Print_SetBitmapFont(g_Font_MGL_Sample6);
	Print_SetColor(0xFF, 0x44);
	DisplayHeader();

	// Change current directory
	Print_SetPosition(0, 16);
	const DOS_Time* time = DOS_GetTime();
	String_Format(g_StrBuffer, "\x07""Time... %02d/%02d/%02d %02d:%02d:%02d\n", time->Year, time->Month, time->Date, time->Hour, time->Minute, time->Second);
	Print_DrawText(g_StrBuffer);

	// Change current directory
	String_Format(g_StrBuffer, "%02d%02d%02d%\0", time->Hour, time->Minute, time->Second);
	Print_DrawFormat("\x07""Create %s directory... ", g_StrBuffer);
	DOS_FCreate(g_StrBuffer, O_RDWR, ATTR_FOLDER);
#if (DOS_USE_VALIDATOR)
	err = DOS_GetLastError();
	if(err == DOS_ERR_NONE)
		Print_DrawText("OK\n");
	else if(err = DOS_ERR_DIRX)
		Print_DrawText(" Warning\n Directory already exists\n");
	else
	{
		DisplayLastError();
		return;
	}
#else
	Print_DrawText("OK\n");
#endif

	Print_DrawFormat("\x07""Change current directory to %s... ", g_StrBuffer);
	DOS_ChangeDirectory(g_StrBuffer);
#if (DOS_USE_VALIDATOR)
	err = DOS_GetLastError();
	if(err != DOS_ERR_NONE)
	{
		DisplayLastError();
		return;
	}
#endif
	Print_DrawText("OK\n");

	Print_DrawText("\x07""Get current directory... ");
	DOS_GetDirectory(DOS_DRIVE_DEFAULT, g_FileBuffer);
#if (DOS_USE_VALIDATOR)
	err = DOS_GetLastError();
	if(err != DOS_ERR_NONE)
	{
		DisplayLastError();
		return;
	}
#endif
	Print_DrawText("OK\n");
	Print_DrawFormat(" Current directory: %s\n", g_FileBuffer);

	Print_DrawText("\x07""Go back to parent (\"..\") directory ... ");
	DOS_ChangeDirectory("..");
	Print_DrawText("OK\n");

	// Search
	Print_DrawText("\x07""Searching for '*.sc5'... ");
	DOS_FIB* fib = DOS_FindFirstEntry("*.SC5", 0);
#if (DOS_USE_VALIDATOR)
	err = DOS_GetLastError();
	if(err != DOS_ERR_NONE)
	{
		DisplayLastError();
		return;
	}
#endif
	Print_DrawText("OK\n");
	while(fib)
	{
		Print_DrawFormat(" Found: %s\n", fib->Filename);
		fib = DOS_FindNextEntry();
#if (DOS_USE_VALIDATOR)
		err = DOS_GetLastError();
#endif
	}

	WaitKeyPress();

	// Start bitmap screen
	Print_SetBitmapFont(g_Font_MGL_Sample6);
	Print_SetColor(0xFF, 0x44);
	DisplayHeader();

	// Open
	Print_SetPosition(0, 16);
	Print_DrawText("\x07""Opening 'image01.sc5'... ");
	u8 file = DOS_FOpen("IMAGE01.SC5", O_RDONLY);
#if (DOS_USE_VALIDATOR)
	if(file == 0xFF)
	{
		DisplayLastError();
		return;
	}
#endif
	Print_DrawFormat("OK (%d)\n", file);

	// Seek
	Print_DrawText("\x07""Seeking 7 bytes (skip header)... ");
	u32 seek = DOS_FSeek(file, 7, SEEK_SET);
#if (DOS_USE_VALIDATOR)
	err = DOS_GetLastError();
	if(err != DOS_ERR_NONE)
	{
		DisplayLastError();
		return;
	}
#endif
	Print_DrawText("OK\n");

	// Seek
	Print_DrawText("\x07""Seeking 2 KB (16 lines)... ");
	seek = DOS_FSeek(file, 2048, SEEK_CUR);
#if (DOS_USE_VALIDATOR)
	err = DOS_GetLastError();
	if(err != DOS_ERR_NONE)
	{
		DisplayLastError();
		return;
	}
#endif
	Print_DrawText("OK\n");

	// Read
	Print_DrawText("\x07""Reading 4 KB (32 lines)... ");
	u16 read = DOS_FRead(file, g_FileBuffer, 4096);
#if (DOS_USE_VALIDATOR)
	err = DOS_GetLastError();
	if(err != DOS_ERR_NONE)
	{
		DisplayLastError();
		return;
	}
#endif
	Print_DrawText("OK\n");
	VDP_WriteVRAM_128K(g_FileBuffer, 128 * g_PrintData.CursorY, 0, 4096);

	// Close
	g_PrintData.CursorY += 32;
	Print_DrawText("\x07""Closing 'image01.sc5'... ");
	err = DOS_FClose(file);
#if (DOS_USE_VALIDATOR)
	if(err != DOS_ERR_NONE)
	{
		DisplayLastError();
		return;
	}
#endif
	Print_DrawText("OK\n");

	// Create folder
	Print_Return();
	Print_DrawText("\x07""Creating 'tmp' directory... ");
	file = DOS_FCreate("tmp", O_RDWR, ATTR_FOLDER);
#if (DOS_USE_VALIDATOR)
	err = DOS_GetLastError();
	if(err == DOS_ERR_NONE)
		Print_DrawText("OK\n");
	else if(err = DOS_ERR_DIRX)
		Print_DrawText(" Warning\n Directory already exists\n");
	else
	{
		DisplayLastError();
		return;
	}
#else
	Print_DrawText("OK\n");
#endif

	// Create file
	Print_DrawText("\x07""Creating 'tmp\\a.bin' file... ");
	file = DOS_FCreate("tmp\\a.bin", O_RDWR, 0);
#if (DOS_USE_VALIDATOR)
	if(file == 0xFF)
	{
		DisplayLastError();
		return;
	}
#endif
	Print_DrawFormat("OK (%d)\n", file);

	// Write
	Print_DrawText("\x07""Writing 4 KB (32 lines)... ");
	u16 write = DOS_FWrite(file, g_FileBuffer, 4096);
#if (DOS_USE_VALIDATOR)
	err = DOS_GetLastError();
	if(err != DOS_ERR_NONE)
	{
		DisplayLastError();
		return;
	}
#endif
	Print_DrawText("OK\n");

	// Seek
	Print_DrawText("\x07""Rewinding file... ");
	seek = DOS_FSeek(file, 0, SEEK_SET);
#if (DOS_USE_VALIDATOR)
	err = DOS_GetLastError();
	if(err != DOS_ERR_NONE)
	{
		DisplayLastError();
		return;
	}
#endif
	Print_DrawText("OK\n");

	// Read
	Print_DrawText("\x07""Reading 4 KB (32 lines)... ");
	read = DOS_FRead(file, g_FileBuffer, 4096);
#if (DOS_USE_VALIDATOR)
	err = DOS_GetLastError();
	if(err != DOS_ERR_NONE)
	{
		DisplayLastError();
		return;
	}
#endif
	Print_DrawText("OK\n");
	VDP_WriteVRAM_128K(g_FileBuffer, 128 * g_PrintData.CursorY, 0, 4096);

	// Close
	g_PrintData.CursorY += 32;
	Print_DrawText("\x07""Closing 'tmp\\a.bin'... ");
	err = DOS_FClose(file);
#if (DOS_USE_VALIDATOR)
	if(err != DOS_ERR_NONE)
	{
		DisplayLastError();
		return;
	}
#endif
	Print_DrawText("OK\n");

	WaitKeyPress();
	Bios_Exit(0);
}

//-----------------------------------------------------------------------------
// Apply Memory Mapper
void ApplyExtBIOS()
{
	// Mapper information
	u16 TotalNumSeg = 0;
	u16 TotalFreeSeg = 0;
	u16 TotalSysSeg = 0;
	u16 TotalUserSeg = 0;

	VDP_CommandHMMV(TAB_X, TAB_Y, 512-TAB_X, 48, 0x44); // Clear
	DOS_VarTable* tab = DOSMapper_GetVarTable();
	u8 i = 0;
	u16 x = TAB_X;
	while(tab->Slot != 0)
	{
		Print_SetPosition(x, TAB_Y);
		Print_DrawFormat("#%i", i);

		Print_SetPosition(x, TAB_Y+8);
		Print_DrawSlot(tab->Slot);

		Print_SetPosition(x, TAB_Y+16);
		Print_DrawInt(tab->NumSeg);
		TotalNumSeg+= tab->NumSeg;

		Print_SetPosition(x, TAB_Y+24);
		Print_DrawInt(tab->FreeSeg);
		TotalFreeSeg+= tab->FreeSeg;

		Print_SetPosition(x, TAB_Y+32);
		Print_DrawInt(tab->SysSeg);
		TotalSysSeg+= tab->SysSeg;

		Print_SetPosition(x, TAB_Y+40);
		Print_DrawInt(tab->UserSeg);
		TotalUserSeg+= tab->UserSeg;

		x += 40;
		i++;
		tab++;
	}

	Print_SetPosition(x, TAB_Y);
	Print_DrawText("Total");

	Print_SetPosition(x, TAB_Y+16);
	Print_DrawInt(TotalNumSeg);
	if(TotalNumSeg < 64)
		Print_DrawFormat(" (%iK)", TotalNumSeg * 16);
	else if(TotalNumSeg % 64 == 0)
		Print_DrawFormat(" (%iM)", TotalNumSeg / 64);
	else
		Print_DrawFormat(" (%iM+%iK)", TotalNumSeg / 64, (TotalNumSeg % 64) * 16);

	Print_SetPosition(x, TAB_Y+24);
	Print_DrawInt(TotalFreeSeg);

	Print_SetPosition(x, TAB_Y+32);
	Print_DrawInt(TotalSysSeg);

	Print_SetPosition(x, TAB_Y+40);
	Print_DrawInt(TotalUserSeg);

	// Page information
	VDP_CommandHMMV(PAGE_X, PAGE_Y+8, 512-PAGE_X, 8, 0x44); // Clear
	x = PAGE_X;
	for(u8 i = 0; i < 4; ++i)
	{
		Print_SetPosition(x, 16);
		Print_DrawInt(DOSMapper_GetPage(i));
		x += 6*5;
	}
}

//-----------------------------------------------------------------------------
// Display Extended BIOS information
void DisplayExtBIOS()
{
	// Initialize display system
	VDP_SetMode(VDP_MODE_SCREEN7);
	VDP_EnableSprite(FALSE);
	VDP_SetColor(0x4);

	// Start bitmap screen
	Print_SetBitmapFont(g_Font_MGL_Sample6);
	Print_SetColor(0xFF, 0x44);
	Print_Clear();
	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL " MSX-DOS 2 Sample");

	Print_SetPosition(PAGE_X, PAGE_Y);
	Print_DrawText("P0   P1   P2   P3");

	// Check Extended BIOS
	Print_SetPosition(0, 16);
	if(ExtBIOS_Check())
		Print_DrawText("Extended BIOS: Detected\n");
	else
		Print_DrawText("Extended BIOS: Not detected\n");

	// Memory mapper variables
	Print_SetPosition(0, 24);
	u8 a = DOSMapper_Init();

	Print_DrawFormat("Jump table: %4xh\n", g_DOS_JumpTable);
	Print_DrawText("Memory Mapper table:\n");
	Print_DrawText("- Slot:\n");
	Print_DrawText("- Segments number:\n");
	Print_DrawText("- Free segments:\n");
	Print_DrawText("- System segments:\n");
	Print_DrawText("- User segments:\n\n");
	Print_DrawText("ALLOC: [1] User Prim  [2] User Not Prim  [3] Sys Prim 1st\n");
	Print_DrawText("FREE: [4] Last Seg   MAPPER: [5] Inc Page 2  [6] Dec Page 2\n\n");
	Print_DrawText("Log:\n");

	g_Page2Seg = DOSMapper_GetPage2();

	ApplyExtBIOS();
}

//-----------------------------------------------------------------------------
// Log a message
void Log(const c8* str)
{
	if(g_LogLine > 190)
	{
		VDP_CommandHMMV(0, 120, 512, 72, 0x44);
		g_LogLine = 120;
	}

	Print_SetPosition(0, g_LogLine);
	Print_DrawText(str);
	g_LogLine += 8;
}

//-----------------------------------------------------------------------------
// Update Extended BIOS
void UpdateExtBIOS()
{
	// Print sign-of-life
	Print_SetPosition(512-8, 0);
	Print_DrawChar(g_ChrAnim[g_Frame >> 2 & 0x03]);

	u8 row0 = ~Keyboard_Read(0);
	u8 keys = (row0 ^ g_PrevRow0) & row0;
	g_PrevRow0 = row0;

	// Allocate user segment from primary Memory mapper's slot
	if(KEY_FLAG(KEY_1) & keys)
	{
		bool ok = DOSMapper_Alloc(DOS_ALLOC_USER, DOS_SEGSLOT_PRIM, &g_LastSeg);
		String_Format(g_StrBuffer, "- %s: Allocate user segment from primary slot [slot:%s, seg:%d]", ok ? "Succed" : "Error", FormatSlot(g_LastSeg.Slot), g_LastSeg.Number);
		Log(g_StrBuffer);
		for(u8 i = 0; i < 4; ++i)
			DOSMapper_WriteByte(g_LastSeg.Number, i, i);
		ApplyExtBIOS();
	}

	// Allocate user segment from Memory mapper's slot other than primary one
	if(KEY_FLAG(KEY_2) & keys)
	{
		bool ok = DOSMapper_Alloc(DOS_ALLOC_USER, g_DOS_VarTable->Slot | DOS_SEGSLOT_OTHER, &g_LastSeg);
		String_Format(g_StrBuffer, "- %s: Allocate user segment from not-primary slot [slot:%s, seg:%d]", ok ? "Succed" : "Error", FormatSlot(g_LastSeg.Slot), g_LastSeg.Number);
		Log(g_StrBuffer);
		ApplyExtBIOS();
	}

	// Allocate system segment from primary Memory mapper's slot in priority
	if(KEY_FLAG(KEY_3) & keys)
	{
		bool ok = DOSMapper_Alloc(DOS_ALLOC_SYS, g_DOS_VarTable->Slot | DOS_SEGSLOT_THISFIRST, &g_LastSeg);
		String_Format(g_StrBuffer, "- %s: Allocate system segment from primary slot in priority [slot:%s, seg:%d]", ok ? "Succed" : "Error", FormatSlot(g_LastSeg.Slot), g_LastSeg.Number);
		Log(g_StrBuffer);
		ApplyExtBIOS();
	}

	// Free last allocated segment
	if(KEY_FLAG(KEY_4) & keys)
	{
		if(g_LastSeg.Slot == SLOT_INVALID)
		{
			String_Format(g_StrBuffer, "- Error: Invalid last segment");
		}
		else
		{
			bool ok = DOSMapper_Free(g_LastSeg.Number, g_LastSeg.Slot);
			String_Format(g_StrBuffer, "- %s: Free a segment [slot:%s, seg:%d]", ok ? "Succed" : "Error", FormatSlot(g_LastSeg.Slot), g_LastSeg.Number);
			g_LastSeg.Slot = SLOT_INVALID;
		}
		Log(g_StrBuffer);
		ApplyExtBIOS();
	}

	// Set page to next segment on the primary slot
	if(KEY_FLAG(KEY_5) & keys)
	{
		DOSMapper_SetPage2(++g_Page2Seg);
		u8 tab[8];
		for(u8 i = 0; i < 8; ++i)
			tab[i] = DOSMapper_ReadByte(g_Page2Seg, i);
		String_Format(g_StrBuffer, "- Switch page 2 to segment %d. 8000h:[%2x %2x %2x %2x %2x %2x %2x %2x]", g_Page2Seg, tab[0], tab[1], tab[2], tab[3], tab[4], tab[5], tab[6], tab[7]);
		Log(g_StrBuffer);
		ApplyExtBIOS();
	}

	// Set page to previous segment on the primary slot
	if(KEY_FLAG(KEY_6) & keys)
	{
		DOSMapper_SetPage2(--g_Page2Seg);
		u8 tab[8];
		for(u8 i = 0; i < 8; ++i)
			tab[i] = DOSMapper_ReadByte(g_Page2Seg, i);
		String_Format(g_StrBuffer, "- Switch page 2 to segment %d. 8000h:[%2x %2x %2x %2x %2x %2x %2x %2x]", g_Page2Seg, tab[0], tab[1], tab[2], tab[3], tab[4], tab[5], tab[6], tab[7]);
		Log(g_StrBuffer);
		ApplyExtBIOS();
	}
}

//-----------------------------------------------------------------------------
// Display standard input
void DisplayStdIn()
{
	// Program header
	DisplayDOSHeader();

	DOS_StringOutput("\n\r$");
	DOS_StringOutput("Press keys:\n\r$");
	DOS_StringOutput("> $");
}

//-----------------------------------------------------------------------------
// Get a character from standard input
u8 DOS_StandardInput()
{
__asm
	push	ix
	ld		e, #0xFF
	ld		c, #DOS_FUNC_DIRIO
	call	BDOS
	pop		ix
__endasm;
	// return A
}

//-----------------------------------------------------------------------------
// Put a character to standard output 
void DOS_StandardOutput(u8 chr)
{
	chr;	// A
__asm
	push	ix
	ld		e, a
	ld		c, #DOS_FUNC_DIRIO
	call	BDOS
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// Update standard input
void UpdateStdIn()
{
	u8 chr = DOS_StandardInput();
	if(chr)
		DOS_StandardOutput(chr);
}

//-----------------------------------------------------------------------------
// Call driver and display result
void DisplayDriver()
{
	// Program header
	DisplayDOSHeader();
	DOS_StringOutput("== START DRIVER AT 0xE000 ==\n\r$");

	// Load driver to memory
	DOS_StringOutput("Opening 'sample.drv'...\n\r$");
	u8 file = DOS_FOpen("SAMPLE.DRV", O_RDONLY); // open
	u32 size = DOS_FSeek(file, 0, SEEK_END); // get size
	DOS_FSeek(file, 0, SEEK_SET); // rewind
	DOS_FRead(file, (void*)0xE000, size); // load
	DOS_FClose(file); // close

	// Starting driver
	DOS_StringOutput("Starting driver...\n\r$");
	u8 val = CallDriver(0xE000, 12);

	DOS_StringOutput("\n\rPRESS A KEY...$");
	DOS_CharInput();
}

//-----------------------------------------------------------------------------
// Program entry point
u8 main(u8 argc, const c8** argv)
{
	c8 chr = 0;
	if(argc > 0)
		chr = argv[0][0];

DisplayMenu:

	// Program header
	DisplayDOSHeader();

	DOS_StringOutput(" [1] System information\n\r$");
	DOS_StringOutput(" [2] File handling (Screen 5)\n\r$");
	DOS_StringOutput(" [3] Memory mapper (Screen 7)\n\r$");
	DOS_StringOutput(" [4] Standard input\n\r$");
	DOS_StringOutput(" [5] Load and execute driver\n\r$");
	DOS_StringOutput(" [Q] Quit\n\r$");

	callback cbUpdate = NULL;
	if(chr == 0)
		chr = DOS_CharInput();
	switch(chr)
	{
	case '1':
		DisplayInfo();
		chr = 0;
		goto DisplayMenu;

	case '2':
		DisplayFile();
		return;

	case '3':
		DisplayExtBIOS();
		cbUpdate = UpdateExtBIOS;
		break;

	case '4':
		DisplayStdIn();
		cbUpdate = UpdateStdIn;
		break;

	case '5':
		DisplayDriver();
		chr = 0;
		goto DisplayMenu;
		break;

	case 'q':
	case 'Q':
		DOS_ClearScreen();
		DOS_Exit(0);

	default:
		chr = 0;
		goto DisplayMenu;
	}

	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		// Naive V-Synch method
		Halt();

		// Update current page
		if(cbUpdate)
			cbUpdate();

		// Increment frame counter
		g_Frame++;
	}

	Bios_Exit(0);
	// return 0;   // Can be used only if you don't use VDP module
}