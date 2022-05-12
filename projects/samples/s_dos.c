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

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Load image using DOS file manager
void LoadImage(u8 idx)
{
	const struct ImageEntry* img = &g_Images[idx];
	
	// Init render
	VDP_SetMode(img->Mode);
	VDP_SetColor(0x00);
	VDP_ClearVRAM();

	// Open File
	Mem_Set(0, &g_File, sizeof(FCB));
	Mem_Copy(img->Name, &g_File.Name, 11);
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
	Print_DrawFormat(MSX_GL "DOS SAMPLE (%s)", img->Text);
	
	// Diplay footer	
	Print_SetColor(0xEE, 0x00);
	Print_SetPosition(0, (u8)(212-9));
	Print_DrawText("\x83:Next image");
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
StartProgram:

	DOS_ClearScreen();
	DOS_StringOutput("+--------------------+\n\r$");
	DOS_StringOutput("| MSXgl - DOS Sample |\n\r$");
	DOS_StringOutput("+--------------------+\n\r$");
	DOS_StringOutput("Which screen mode?\n\r$");
	DOS_StringOutput(" (can be: 5, 8)\n\r$");

	i8 scrMode = DOS_CharInput();
	switch(scrMode)
	{
	case '5':
		DOS_StringOutput("\n\rScreen mode 5 selected\n\r$");
		break;
	case '8':
		DOS_StringOutput("\n\rScreen mode 8 selected\n\r$");
		break;
	default:
		DOS_StringOutput("\n\rUnsupported screen mode\n\rPress a key\n\r$");
		DOS_CharInput();
		goto StartProgram;
		break;
	}



	Mem_Set(0, &g_File, sizeof(FCB));
	Mem_Copy("????????SC?", &g_File.Name, 11);
	g_File.Name[10] = scrMode;
	i8 idx = 0;
	if(DOS_FindFirstFile(&g_File) == 0)
	{
		// DOS_CharOutput('0' + idx++);
		// DOS_StringOutput(": ");
		((c8*)0x0080)[12] = '$';
		DOS_StringOutput((const c8*)0x0080);
		DOS_StringOutput("\n\r$");
		while(DOS_FindNextFile() == 0)
		{
			// DOS_CharOutput('0' + idx++);
			// DOS_StringOutput(": ");
			((c8*)0x0080)[12] = '$';
			DOS_StringOutput((const c8*)0x0080);
			DOS_StringOutput("\n\r$");
		}
	}
	DOS_Close(&g_File);

	DOS_CharInput();

	// Load first image
	LoadImage(g_ImageIdx);

	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		// Load next image
		if(Keyboard_IsKeyPressed(KEY_SPACE))
		{
			g_ImageIdx++;
			if(g_ImageIdx >= numberof(g_Images))
				g_ImageIdx = 0;
			LoadImage(g_ImageIdx);
		}
	}
	
	// VDP_SetMode(VDP_MODE_SCREEN0);
	// DOS_Call(DOS_FUNC_TERM0);
	__asm__("call 0x0000");
}