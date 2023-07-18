// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  MSX-DOS sample
//─────────────────────────────────────────────────────────────────────────────

#include "msxgl.h"
#include "dos.h"
#include "memory.h"
#include "string.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"

// Images table
const const c8* g_Images[] = {
	"IMAGE01 SC8",
	"IMAGE04 SC8",
	"PALETTE SC8",
};

//=============================================================================
// MEMORY DATA
//=============================================================================

FCB g_File;
u8  g_StrBuffer[128];
u8  g_ImageIdx = 0;
u8  g_FileNum = 0;
c8  g_FileList[10][12];

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Load image using DOS file manager
void LoadImage(const c8* filename)
{
	// Init render
	VDP_SetMode(VDP_MODE_SCREEN8);
	VDP_SetColor(0x00);
	VDP_ClearVRAM();

	// Open File
	Mem_Set(0, &g_File, sizeof(FCB));
	Mem_Copy(filename, &g_File.Name, 11);
	DOS_OpenFCB(&g_File);
	
	// Read file and copy content into VRAM
	u16 dst = 0;
	for (u16 i = 0; i < g_File.Size; i+=128)
	{
		DOS_SetTransferAddr(g_StrBuffer);
		DOS_SequentialReadFCB(&g_File);
		
		const u8* src;
		u8 size;
		if(i == 0)
		{
			src = g_StrBuffer + 7;
			size = 128 - 7;
		}
		else
		{
			src = g_StrBuffer;
			size = 128;
		}
		
		VDP_WriteVRAM_128K(src, dst, 0, size);
		dst += size;
	}
	DOS_CloseFCB(&g_File);

	// Initialize font
	Print_SetBitmapFont(g_Font_MGL_Sample6);

	// Diplay header	
	Print_SetColor(0xFF, 0x00);
	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL "DOS SAMPLE (Screen Mode 8)");
	
	// Diplay footer	
	Print_SetColor(0xEE, 0x00);
	Print_SetPosition(0, (u8)(212-9));
	Print_DrawText("Esc:Exit");
}

//-----------------------------------------------------------------------------
// Add image to list
void AddFile()
{
	if(g_FileNum >= 10)
		return;
	
	Mem_Copy(((c8*)0x0081), g_FileList[g_FileNum], 11);
	
	((c8*)0x0081)[11] = '$';
	DOS_CharOutput('0' + g_FileNum);
	DOS_StringOutput(": $");
	DOS_StringOutput((const c8*)0x0081);
	DOS_StringOutput("\n\r$");
	
	g_FileNum++;
}

//=============================================================================
// MAIN LOOP
//=============================================================================

void VDP_InterruptHandler() {}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
StartProgram:

	// Program header
	DOS_ClearScreen();
	DOS_StringOutput("+--------------------+\n\r$");
	DOS_StringOutput("| MSXgl - DOS Sample |\n\r$");
	DOS_StringOutput("+--------------------+\n\r$");

/*
	// Search for compatible images
	const c8* wildcard = "????????SC8";
	Mem_Set(0, &g_File, sizeof(FCB));
	Mem_Copy(wildcard, &g_File.Name, 11);
	g_FileNum = 0;
	if(DOS_FindFirstFileFCB(&g_File) == DOS_SUCCESS)
	{
		AddFile();
		while(DOS_FindNextFileFCB() == DOS_SUCCESS)
			AddFile();
	}
	DOS_Close(&g_File);

	while(1) {}

	DOS_StringOutput("PRESS A KEY\n\r$");
	DOS_CharInput();
*/

	// Load and display image
	DOS_Beep();
	DOS_StringOutput("Loading image...\n\r$");
	LoadImage("IMAGE04 SC8");

	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
	}

	// DOS_Exit0();
	Bios_Exit(0);
}