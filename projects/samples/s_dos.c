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
#include "font/font_mgl_sample6.h"

// Images table
const struct ImageEntry g_Images[] =
{
	{ "IMAGE01 SC5", "Screen 5",  VDP_MODE_SCREEN5 },
	{ "IMAGE01 SC8", "Screen 8",  VDP_MODE_SCREEN8 },
	{ "IMAGE01 S12", "Screen 12", VDP_MODE_SCREEN12 },
	{ "IMAGE04 SC5", "Screen 5",  VDP_MODE_SCREEN5 },
	{ "IMAGE04 SC8", "Screen 8",  VDP_MODE_SCREEN8 },
	{ "IMAGE04 S12", "Screen 12", VDP_MODE_SCREEN12 },
	{ "PALETTE SC5", "Screen 5",  VDP_MODE_SCREEN5 },
	{ "PALETTE SC8", "Screen 8",  VDP_MODE_SCREEN8 },
	{ "PALETTE S12", "Screen 12", VDP_MODE_SCREEN12 },
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
void LoadImage(u8 srcMode, u8 imgIdx)
{
	// Init render
	VDP_SetMode(srcMode);
	VDP_SetColor(0x00);
	VDP_ClearVRAM();

	// Open File
	Mem_Set(0, &g_File, sizeof(FCB));
	Mem_Copy(g_FileList[imgIdx], &g_File.Name, 11);
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
void main(u8 argc, const c8** argv)
{
StartProgram:

	// Program header
	DOS_ClearScreen();
	DOS_StringOutput("+--------------------+\n\r$");
	DOS_StringOutput("| MSXgl - DOS Sample |\n\r$");
	DOS_StringOutput("+--------------------+\n\r$");

	// Check argument
	String_Format(g_StrBuffer, "Argument count: %d\n\r$", argc);
	DOS_StringOutput(g_StrBuffer);
	if(argc == 0)
	{
		DOS_StringOutput("No argument...\n\r$");
		DOS_StringOutput("Usage: s_dos <mode> <image>\n\r$");
		DOS_StringOutput(" mode       '5', '8', 'c'\n\r$");
		DOS_StringOutput(" image      Filename\n\r$");
	}

	// Display VDP version
	u8 vdp = VDP_GetVersion();
	switch(vdp)
	{
	case VDP_VERSION_TMS9918A:
		DOS_StringOutput("VDP: MSX1\n\r$");
		break;
	case VDP_VERSION_V9938:
		DOS_StringOutput("VDP: MSX2\n\r$");
		break;
	case VDP_VERSION_V9958:
		DOS_StringOutput("VDP: MSX2+\n\r$");
		break;
	default:
		DOS_StringOutput("VDP: Unknow\n\r$");
	}

	// Get screen mode
	u8 scrChr = 0xFF;
	if(argc >= 1) // Get from argument list
	{
		u8 chr = argv[0][0];
		switch(chr)
		{
		case '5':
		case '8':
			scrChr = chr;
			break;
		case 'c':
			if(vdp >= VDP_VERSION_V9958)
				scrChr = chr;
			else
				DOS_StringOutput("Error: Screen 12 need MSX2+ VDP!\n\r$");
			break;
		default:
			String_Format(g_StrBuffer, "Error: Unknow screen mode '%c'\n\r$", chr);
			DOS_StringOutput(g_StrBuffer);
			break;
		}
	}
	if(scrChr == 0xFF) // Get from console input
	{
		DOS_StringOutput("Which screen mode?\n\r$");
		DOS_StringOutput(" (can be: 5, 8, c)\n\r$");
		scrChr = (u8)DOS_CharInput();
	}

	// Setup screen mode
	u8 scrMode;
	DOS_Beep();
	DOS_Return();
	const c8* wildcard;
	switch(scrChr)
	{
	case '5':
		DOS_StringOutput("Screen mode 5 selected\n\r\n\r$");
		DOS_StringOutput("Searching for *.SC5 files...\n\r$");
		scrMode = VDP_MODE_SCREEN5;
		wildcard = "????????SC5";
		break;
	case '8':
		if(vdp < VDP_VERSION_V9938)
		{
			DOS_StringOutput("Error: Screen 8 need MSX2/2+ VDP!\n\r$");
			DOS_StringOutput("Press a key...\n\r$");
			DOS_CharInput();
			goto StartProgram;		
		}
		DOS_StringOutput("Screen mode 8 selected\n\r\n\r$");
		DOS_StringOutput("Searching for *.SC8 files...\n\r$");
		scrMode = VDP_MODE_SCREEN8;
		wildcard = "????????SC8";
		break;
	case 'c':
		if(vdp < VDP_VERSION_V9958)
		{
			DOS_StringOutput("Error: Screen 12 need MSX2+ VDP!\n\r$");
			DOS_StringOutput("Press a key...\n\r$");
			DOS_CharInput();
			goto StartProgram;
		}
		DOS_StringOutput("Screen mode 12 selected\n\r\n\r$");
		DOS_StringOutput("Searching for *.S12 files...\n\r$");
		scrMode = VDP_MODE_SCREEN12;
		wildcard = "????????S12";
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
	Mem_Copy(wildcard, &g_File.Name, 11);
	g_FileNum = 0;
	if(DOS_FindFirstFileFCB(&g_File) == DOS_SUCCESS)
	{
		AddFile();
		while(DOS_FindNextFileFCB() == DOS_SUCCESS)
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

	// DOS_Exit0();
	Bios_Exit(0);
}