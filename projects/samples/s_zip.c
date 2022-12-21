// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Data un-compressor sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"
#include "dos.h"
#include "compress/zx0.h"
#include "compress/bitbuster.h"
#include "compress/bitbuster2.h"
#include "compress/pletter.h"

//=============================================================================
// DEFINES
//=============================================================================

#define IMAGE_X		(128-8)
#define IMAGE_Y		24
#define IMAGE_W		128
#define IMAGE_H		128

#define MENU_X		(8+4)
#define MENU_Y		(24+8+4)

// Callback signature
typedef u16 (*loader)(const c8*);

// Compress data table
struct Entry
{
	const c8* Name;
	const u8* File;
	loader    Loader;
};

// Prototypes
u16 LoadRaw(const c8* filename);
u16 LoadRLEp(const c8* filename);
u16 LoadXZ0(const c8* filename);
u16 LoadBitbuster(const c8* filename);
u16 LoadBitbuster2(const c8* filename);
u16 LoadPletter(const c8* filename);

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample8.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Compressor name
const struct Entry g_Compressors[] =
{
	{ "Raw",        "DATA10  BIN", LoadRaw },
	{ "RLEp",       "DATA10  RLE", LoadRLEp },
	{ "ZX0",        "DATA10  ZX0", LoadXZ0 },
	{ "Bitbuster",  "DATA10  PCK", LoadBitbuster },
	{ "Bitbuster2", "DATA10  BB2", LoadBitbuster2 },
	{ "Pletter",    "DATA10  PL5", LoadPletter },
};

//
u16 g_Compression;

//=============================================================================
// 
//=============================================================================
u8 g_UnpackBuffer[8*1024];
u8 g_LoadBuffer[6*1024];

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
u16 Load(const c8* filename, u8* buffer)
{
	FCB file;

	// Open File
	Mem_Set(0, &file, sizeof(FCB));
	Mem_Copy(filename, &file.Name, 11);
	DOS_Open(&file);
	
	// Read file and copy content into VRAM
	for (u16 i = 0; i < file.Size; i += 128)
	{
		DOS_SetTransferAddr(buffer);
		DOS_SequentialRead(&file);
		buffer += 128;
	}
	DOS_Close(&file);
	return file.Size;
}

//-----------------------------------------------------------------------------
//
u16 LoadRaw(const c8* filename)
{
	u16 size = Load(filename, g_UnpackBuffer);
	g_Compression = 0;
	return 0;
}

//-----------------------------------------------------------------------------
//
u16 LoadRLEp(const c8* filename)
{
	u16 size = Load(filename, g_LoadBuffer);
	g_Compression = 100 - (size / 82);

	u16 time = g_JIFFY;
	RLEp_UnpackToRAM(g_LoadBuffer, g_UnpackBuffer);
	return g_JIFFY - time;
}

//-----------------------------------------------------------------------------
//
u16 LoadXZ0(const c8* filename)
{
	u16 size = Load(filename, g_LoadBuffer);
	g_Compression = 100 - (size / 82);

	u16 time = g_JIFFY;
	ZX0_UnpackToRAM(g_LoadBuffer, g_UnpackBuffer);
	return g_JIFFY - time;
}

//-----------------------------------------------------------------------------
//
u16 LoadBitbuster(const c8* filename)
{
	u16 size = Load(filename, g_LoadBuffer);
	g_Compression = 100 - (size / 82);

	u16 time = g_JIFFY;
	Bitbuster_UnpackToRAM(g_LoadBuffer, g_UnpackBuffer);
	return g_JIFFY - time;
}

//-----------------------------------------------------------------------------
//
u16 LoadBitbuster2(const c8* filename)
{
	u16 size = Load(filename, g_LoadBuffer);
	g_Compression = 100 - (size / 82);

	u16 time = g_JIFFY;
	Bitbuster2_UnpackToRAM(g_LoadBuffer, g_UnpackBuffer);
	return g_JIFFY - time;
}

//-----------------------------------------------------------------------------
//
u16 LoadPletter(const c8* filename)
{
	u16 size = Load(filename, g_LoadBuffer);
	g_Compression = 100 - (size / 82);

	u16 time = g_JIFFY;
	Pletter_UnpackToRAM(g_LoadBuffer, g_UnpackBuffer);
	return g_JIFFY - time;
}

//-----------------------------------------------------------------------------
//
void LoadAndUnpack(u8 id)
{
	VDP_SetSpriteUniColor(0, COLOR_LIGHT_RED);

	// Clean
	VDP_CommandHMMV(IMAGE_X, IMAGE_Y, IMAGE_W, IMAGE_H, 0x11);

	// Load
	u16 time = g_Compressors[id].Loader(g_Compressors[id].File);

	// Draw
	u8* src = g_UnpackBuffer;
	u16 dest = g_ScreenLayoutLow + (IMAGE_X/2) + (IMAGE_Y*128);
	for(u8 y = 0; y < IMAGE_H; ++y)
	{
		VDP_WriteVRAM(src, dest, 0, IMAGE_W/2);
		src += IMAGE_W/2;
		dest += 256/2;
	}

	VDP_CommandHMMV(0, 128, 128-16, 16, 0x44);
	Print_SetPosition(0, 128);
	Print_DrawText("Time:");
	Print_DrawInt(time);
	Print_SetPosition(0, (u8)(128+8));
	Print_DrawText("Ratio:");
	Print_DrawInt(g_Compression);
	Print_DrawText("%");

	VDP_SetSpriteUniColor(0, COLOR_WHITE);
}

//-----------------------------------------------------------------------------
//
void DisplayPage()
{
	VDP_CommandHMMV(0, 0, 256, 212, 0x44);
	VDP_CommandHMMV(IMAGE_X, IMAGE_Y, IMAGE_W, IMAGE_H, 0x11);

	Print_SetPosition(0, 0);
	Print_DrawText("\x2\x3\x4\x5 COMPRESS SAMPLE");
	Draw_LineH(0, 255, 8+4, 0x0F, 0);
	
	Print_SetPosition(0, 24);
	Print_DrawText("Compressors:");

	for(u8 i = 0; i < numberof(g_Compressors); ++i)
	{
		Print_SetPosition(MENU_X, MENU_Y + (i * 8));
		Print_DrawText(g_Compressors[i].Name);
	}

	Draw_Box(IMAGE_X - 2, IMAGE_Y - 2, (u8)(IMAGE_X + IMAGE_W + 1), (u8)(IMAGE_Y + IMAGE_H + 1), 0x0F, 0);

	Draw_LineH(0, 255, (u8)(212-8-4), 0x0F, 0);
	Print_SetPosition(0, (u8)(212-8));
	Print_DrawText("\x82:Select \x83:Load");
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Init VDP
	VDP_SetMode(VDP_MODE_SCREEN5);
	VDP_SetColor(0x4);

	// Init font
	Print_SetBitmapFont(g_Font_MGL_Sample8);
	
	// Init sprite
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_8 + VDP_SPRITE_SCALE_1);
	VDP_LoadSpritePattern(g_Font_MGL_Sample8 + 4, 0, g_Font_MGL_Sample8[3] - g_Font_MGL_Sample8[2]);
	VDP_SetSpriteExUniColor(0, 2, MENU_Y - 1, '\x80', COLOR_WHITE);
	VDP_DisableSpritesFrom(1);

	DisplayPage();
	
	u8 compId = 0;
	u8 count = 0;
	u8 prevRow8 = 0xFF;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();

		VDP_SetSpritePositionY(0, MENU_Y - 1 + compId * 8);
		
		Print_SetPosition(256-8, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);

		// Handle input
		u8 row8 = Keyboard_Read(8);
		if(IS_KEY_PRESSED(row8, KEY_UP) && !IS_KEY_PRESSED(prevRow8, KEY_UP))
		{
			if(compId == 0)
				compId = numberof(g_Compressors) - 1;
			else
				compId--;
		}
		else if(IS_KEY_PRESSED(row8, KEY_DOWN) && !IS_KEY_PRESSED(prevRow8, KEY_DOWN))
		{
			if(compId == numberof(g_Compressors) - 1)
				compId = 0;
			else
				compId++;
		}
		if(IS_KEY_PRESSED(row8, KEY_SPACE) && !IS_KEY_PRESSED(prevRow8, KEY_SPACE))
		{
			LoadAndUnpack(compId);
		}

		prevRow8 = row8;	
	}

	Bios_Exit(0);
}