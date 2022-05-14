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

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

// Image entry
struct ImageEntry
{
	const c8* Name;
	const c8* Text;
	u8        Mode;
};

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font\font_mgl_sample6.h"

// Images table
const struct ImageEntry g_Images[] =
{
	{ "IMAGE01 SC5", "Screen 5", VDP_MODE_SCREEN5 },
	{ "IMAGE01 SC8", "Screen 8", VDP_MODE_SCREEN8 },
	{ "IMAGE04 SC5", "Screen 5", VDP_MODE_SCREEN5 },
	{ "IMAGE04 SC8", "Screen 8", VDP_MODE_SCREEN8 },
	{ "PALETTE SC5", "Screen 5", VDP_MODE_SCREEN5 },
	{ "PALETTE SC8", "Screen 8", VDP_MODE_SCREEN8 },
};

//=============================================================================
// MEMORY DATA
//=============================================================================

FCB g_File;
u8  g_Buffer[128];
u8  g_ImageIdx = 0;
u8  g_FileNum = 0;
c8  g_FileList[10][12];

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Load image using DOS file manager
void LoadImage(u8 srcMode, u8 imgIdx)
{
	// Init render
	VDP_SetMode(srcMode);
	VDP_SetColor(0x00);
	VDP_ClearVRAM();

	// Open File
	Mem_Set(0, &g_File, sizeof(FCB));
	Mem_Copy(g_FileList[imgIdx], &g_File.Name, 11);
	DOS_Open(&g_File);
	
	// Read file and copy content into VRAM
	u16 dst = 0;
	for (u16 i = 0; i < g_File.Size; i+=128)
	{
		DOS_SetTransferAddr(g_Buffer);
		DOS_SequentialRead(&g_File);
		
		const u8* src;
		u8 size;
		if(i == 0)
		{
			src = g_Buffer + 7;
			size = 128 - 7;
		}
		else
		{
			src = g_Buffer;
			size = 128;
		}
		
		VDP_WriteVRAM_128K(src, dst, 0, size);
		dst += size;
	}
	DOS_Close(&g_File);

	// Initialize font
	Print_SetBitmapFont(g_Font_MGL_Sample6);

	// Diplay header	
	Print_SetColor(0xFF, 0x00);
	Print_SetPosition(0, 0);
	Print_DrawFormat(MSX_GL "DOS SAMPLE (Screen Mode %s)", srcMode == VDP_MODE_SCREEN5 ? "5" : "8");
	
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
	DOS_StringOutput("Which screen mode?\n\r$");
	DOS_StringOutput(" (can be: 5, 8)\n\r$");

	// Setup screen mode
	u8 scrMode;
	i8 scrChr = DOS_CharInput();
	DOS_Beep();
	DOS_Return();
	switch(scrChr)
	{
	case '5':
		DOS_StringOutput("Screen mode 5 selected\n\r\n\r$");
		DOS_StringOutput("Searching for *.SC5 files...\n\r$");
		scrMode = VDP_MODE_SCREEN5;
		break;
	case '8':
		DOS_StringOutput("Screen mode 8 selected\n\r\n\r$");
		DOS_StringOutput("Searching for *.SC8 files...\n\r$");
		scrMode = VDP_MODE_SCREEN8;
		break;
	default:
		DOS_StringOutput("Error: Unsupported screen mode!\n\r$");
		DOS_StringOutput("Press a key...\n\r$");
		DOS_CharInput();
		goto StartProgram;
		break;
	}

	// Search for compatible images
	Mem_Set(0, &g_File, sizeof(FCB));
	Mem_Copy("????????SC?", &g_File.Name, 11);
	g_File.Name[10] = scrChr;
	g_FileNum = 0;
	if(DOS_FindFirstFile(&g_File) == DOS_NO_ERROR)
	{
		AddFile();
		while(DOS_FindNextFile() == DOS_NO_ERROR)
			AddFile();
	}
	DOS_Close(&g_File);

	// No image found
	if(g_FileNum == 0)
	{
		DOS_StringOutput("Error: No file found!\n\r$");
		DOS_StringOutput("Press a key...\n\r$");
		DOS_CharInput();
		goto StartProgram;		
	}

	// Select image index to display
	DOS_StringOutput("Which image to display?\n\r$");
	u8 imgIdx = DOS_CharInput() - '0';
	DOS_Return();

	// Invalid image index
	if(imgIdx >= g_FileNum)
	{
		DOS_StringOutput("Error: Invalide image index!\n\r$");
		DOS_StringOutput("Press a key...\n\r$");
		DOS_CharInput();
		goto StartProgram;		
	}

	// Load and display image
	DOS_Beep();
	DOS_StringOutput("Loading image...\n\r$");
	LoadImage(scrMode, imgIdx);

	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
	}

	DOS_Exit();
}