// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  VDP command sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL6		"\x01\x02\x03\x04\x05\x06"
#define MSX_GL8		"\x02\x03\x04\x05"

// Screen setting
struct ScreenSetting
{
	// Setup
	const c8*  Name;
	u8         Mode;
	u16        Width;
	u8         BPC;
	// Colors
	u8         Text;
	u8         Background;
	u8         Red;
	u8         White;
	u8         Gray;
	u8         Black;
	// Data
	const u8*  Font;
	const u8*  Data;
	const u8*  DataLMMC;
	const u16* Palette;
};

//=============================================================================
// MEMORY DATA
//=============================================================================

// Screen mode setting index
u8 g_SrcModeIndex;
u8 g_VBlank = 0;
u8 g_Frame = 0;
u16 SX, SY;

// Memory buffer to unpack compressed data
u8 g_LMMC2b[16*16];
u8 g_LMMC4b[16*16];


//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font\font_mgl_sample6.h"
#include "font\font_mgl_sample8.h"

// Cursor
const u8 g_CursorForm[] = 
{
// Sprite[0] (offset:0)
	0xF0, /* ######## */ 
	0x80, /* #....... */ 
	0x80, /* #....... */ 
	0x80, /* #....... */ 
	0x00, /* #....... */ 
	0x00, /* #....... */ 
	0x00, /* #....... */ 
	0x00, /* #....... */ 
// Sprite[1] (offset:8)
	0x00, /* #....... */ 
	0x00, /* #....... */ 
	0x00, /* #....... */ 
	0x00, /* #....... */ 
	0x80, /* #....... */ 
	0x80, /* #....... */ 
	0x80, /* #....... */ 
	0xF0, /* ######## */ 
// Sprite[2] (offset:16)
	0x0F, /* ######## */ 
	0x01, /* .......# */ 
	0x01, /* .......# */ 
	0x01, /* .......# */ 
	0x00, /* .......# */ 
	0x00, /* .......# */ 
	0x00, /* .......# */ 
	0x00, /* .......# */ 
// Sprite[3] (offset:24)
	0x00, /* .......# */ 
	0x00, /* .......# */ 
	0x00, /* .......# */ 
	0x00, /* .......# */ 
	0x01, /* .......# */ 
	0x01, /* .......# */ 
	0x01, /* .......# */ 
	0x0F, /* ######## */ 
};

// Bitmaps by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "content\data_bmp_2b.h"
#include "content\data_bmp_4b.h"
#include "content\data_bmp_8b.h"

// Screen mode settings
const struct ScreenSetting g_Settings[] =
{ //  Name              Mode              Width BPC Txt   BG    Red   White Gray  Black Font                Data         DataLMMC     Palette 
	{ MSX_GL6 " VDP COMMAND SAMPLE (S5/G4)",	VDP_MODE_SCREEN5, 256,	4,	0xFF, 0x44, 0x88, 0xFF, 0x11, 0x11, g_Font_MGL_Sample6, g_DataBmp4b, g_LMMC4b,    NULL }, // 0
	{ MSX_GL8 " VDP COMMAND SAMPLE (S6/G5)",	VDP_MODE_SCREEN6, 512,	2,	0xFF, 0xAA, 0x55, 0xFF, 0xAA, 0x55, g_Font_MGL_Sample8, g_DataBmp2b, g_LMMC2b,    NULL }, // 1
	{ MSX_GL8 " VDP COMMAND SAMPLE (S7/G6)",	VDP_MODE_SCREEN7, 512,	4,	0xFF, 0x44, 0x88, 0xFF, 0x11, 0x11, g_Font_MGL_Sample8, g_DataBmp4b, g_LMMC4b,    NULL }, // 2
	{ MSX_GL6 " VDP COMMAND SAMPLE (S8/G7)",	VDP_MODE_SCREEN8, 256,	8,	0xFF, 0x47, 0x1C, 0xFF, 0x6D, 0x00, g_Font_MGL_Sample6, g_DataBmp8b, g_DataBmp8b, NULL }, // 3
};

// Character animation
const u8 chrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void DisplayPage()
{
	const struct ScreenSetting* src = &g_Settings[g_SrcModeIndex];
	
	u8* buffer = Mem_HeapAlloc(256);
	u16 blockWidth = src->Width / 16;
	u16 blockBytes = 16 / 8 * src->BPC;
	u16 lineBytes = src->Width / 8 * src->BPC;
	u16 X, Y;
	u8 scale = 8 / src->BPC;
	
	//-------------------------------------------------------------------------
	// Init
	
	VDP_SetMode(src->Mode);
	VDP_SetColor(src->Background);
	VDP_CommandHMMV(0, 0, src->Width, 212, src->Background);
	
	VDP_SetPaletteEntry(1, RGB16(0, 0, 0));
	VDP_SetPaletteEntry(2, RGB16(1, 1, 4));
	VDP_SetPaletteEntry(3, RGB16(2, 2, 7));

	Print_SetBitmapFont(src->Font);
	Print_SetColor(src->Text, src->Background);
	
	Print_SetPosition(4, 2);
	Print_DrawText(src->Name);
	Draw_LineH(0, src->Width - 1, 12, src->Text, 0);

	//-------------------------------------------------------------------------
	// WriteVRAM(src, destLow, destHigh, count) - Write data from RAM to VRAM
	X = blockWidth / 2;
	Y = 24;
	Print_SetPosition(X, Y);
	Print_DrawText("Write");
	for(u16 i = 0; i < 16; ++i)
		VDP_WriteVRAM(src->Data + (blockBytes * i), (Y + 8 + i) * lineBytes + (X / scale), 0, blockBytes);

	// HMMC(addr, dx, dy, nx, ny) - High speed move CPU to VRAM
	Y += 32;
	Print_SetPosition(X, Y);
	Print_DrawText("HMMC");
	VDP_CommandHMMC(src->Data, X, Y + 8, 16, 16);

	// YMMM(sy, dx, dy, ny, dir) - High speed move VRAM to VRAM, Y coordinate only
	Y += 32;
	Print_SetPosition(X, Y);
	Print_DrawText("YMMM");
	VDP_CommandYMMM(SY, X, Y + 8, 16, 0);
	
	// HMMM(sx, sy, dx, dy, nx, ny) - High speed move VRAM to VRAM
	Y += 32;
	Print_SetPosition(X, Y);
	Print_DrawText("HMMM");
	VDP_CommandHMMM(SX, SY, X, Y + 8, 16, 16);

	// HMMV(dx, dy, nx, ny, col) - High speed move VDP to VRAM
	Y += 32;
	Print_SetPosition(X, Y);
	Print_DrawText("HMMV");
	VDP_CommandHMMV(X, Y + 8, blockWidth, 16, src->Red);

	//-----------------------------------------------------------------------------
	// ReadVRAM(srcLow, srcHigh, dest, count) - Read data from VRAM to RAM
	X += src->Width / 3;
	Y = 24;
	Print_SetPosition(X, Y);
	Print_DrawText("Read>W");
	for(u16 i = 0; i < 16; ++i)
	{
		VDP_ReadVRAM((SY + i) * lineBytes + (SX / scale), 0, buffer + (blockBytes * i), blockBytes);
		VDP_WriteVRAM(buffer + (blockBytes * i), (Y + 8 + i) * lineBytes + (X / scale), 0, blockBytes);
	}

	// LMMC(addr, dx, dy, nx, ny, op) - Logical move CPU to VRAM
	Y += 32;
	Print_SetPosition(X, Y);
	Print_DrawText("LMMC(TI)");
	VDP_CommandLMMC(src->DataLMMC, X, Y + 8, 16, 16, VDP_OP_TIMP);

	// LMCM() - Logical move VRAM to CPU
	Y += 32;
	Print_SetPosition(X, Y);
	Print_DrawText("LMCM(TI)");
	Print_SetPosition(X, Y + 8);
	Print_DrawText("xxx");

	// LMMM(sx, sy, dx, dy, nx, ny, op) - Logical move VRAM to VRAM
	Y += 32;
	Print_SetPosition(X, Y);
	Print_DrawText("LMMM(TI)");
	VDP_CommandLMMM(SX, SY, X, Y + 8, 16, 16, VDP_OP_TIMP);

	// LMMV(dx, dy, nx, ny, col, op) - Logical move VDP to VRAM
	Y += 32;
	Print_SetPosition(X, Y);
	Print_DrawText("LMMV(OR)");
	VDP_CommandLMMV(X - blockWidth/4, Y + 8 + 4, blockWidth + blockWidth/2, 16 - 8, src->Red, VDP_OP_IMP);
	VDP_CommandLMMV(X, Y + 8, blockWidth, 16, src->Gray, VDP_OP_OR);

	//-----------------------------------------------------------------------------
	// FillVRAM(value, destLow, destHigh, count) - Fill VRAM area with a given value
	X += src->Width / 3;
	Y = 24;
	Print_SetPosition(X, Y);
	Print_DrawText("Fill");
	for(u16 i = 0; i < 16; ++i)
		VDP_FillVRAM(src->Red, (Y + 8 + i) * lineBytes + (X / scale), 0, blockBytes * blockWidth/16);

	// LINE - Draw straight line in VRAM
	Y += 32;
	Print_SetPosition(X, Y);
	Print_DrawText("LINE(OR)");
	VDP_CommandHMMV(X - blockWidth/4, Y + 8 + 4,  blockWidth + blockWidth/2, 8, src->Red);
	VDP_CommandLINE(X, Y + 8,      blockWidth, 16, src->Gray, VDP_ARG_DIY_DOWN + VDP_ARG_DIX_RIGHT, VDP_OP_OR);
	VDP_CommandLINE(X, Y + 16 + 8, blockWidth, 16, src->Gray, VDP_ARG_DIY_UP + VDP_ARG_DIX_RIGHT, VDP_OP_OR);
	VDP_CommandLINE(X, Y + 8 + 8,  blockWidth, 0,  src->Gray, VDP_ARG_DIY_DOWN + VDP_ARG_DIX_RIGHT, VDP_OP_OR);
	VDP_CommandLINE(X + blockWidth/2, Y + 8, 16, 0,  src->Gray, VDP_ARG_DIY_DOWN + VDP_ARG_MAJ_V, VDP_OP_OR);

	// SRCH - Search for the specific color in VRAM to the right or left of the starting point
	Y += 32;
	Print_SetPosition(X, Y);
	Print_DrawText("SRCH");
	Print_SetPosition(X, Y + 8);
	Print_DrawText("xxx");
	
	// PSET - Draw a dot in VRAM 
	Y += 32;
	Print_SetPosition(X, Y);
	Print_DrawText("PSET(I)");
	for(u16 i = 0; i < 32; ++i)
	{
		u16 rnd = Math_GetRandom16();
		VDP_CommandPSET(X + rnd % blockWidth, Y + 8 + (rnd >> 4) % 16, (rnd >> 8), VDP_OP_IMP);
	}
	
	// POINT(sx, sy) - Read the color of the specified dot located in VRAM 
	Y += 32;
	Print_SetPosition(X, Y);
	Print_DrawText("POINT(I)");
	u8 clr = VDP_CommandPOINT(SX, SY);
	VDP_CommandLMMV(X, Y + 8, blockWidth, 16, clr, VDP_OP_IMP);

	Print_SetPosition(4, 200);
	Print_DrawText("\x81\x82\x80:Chg mode  Space+\x81\x82\x80:Move cursor");

	// Init sprite
	VDP_EnableSprite(TRUE);
	VDP_SetSpritePatternTable(0x0E000);
	VDP_SetSpriteAttributeTable(0x0EA00);
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);
	// Load and setup sprite	
	VDP_LoadSpritePattern(g_CursorForm, 0, 4);
	VDP_SetSpriteExUniColor(0, SX, SY, 0, 0xF);
	VDP_HideSpriteFrom(1);
}

//-----------------------------------------------------------------------------
// H_TIMI interrupt hook
void VBlankHook()
{
	g_VBlank = 1;
}

//-----------------------------------------------------------------------------
// Wait for V-Blank period
void WaitVBlank()
{
	while(g_VBlank == 0) {}
	g_VBlank = 0;
	g_Frame++;
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Precalc
	for(u16 i = 0; i < 256; ++i)
	{
		u8 c, b = g_DataBmp4b[i >> 1];
		if((i & 0x1) == 0)
			c = b >> 4;
		else // if((i & 0x1) == 1)
			c = b;		
		g_LMMC4b[i] = c & 0x0F;
	}
	for(u16 i = 0; i < 256; ++i)
	{
		u8 c, b = g_DataBmp2b[i >> 2];
		if((i & 0x3) == 0)
			c = b >> 6;
		else if((i & 0x3) == 1)
			c = b >> 4;
		else if((i & 0x3) == 2)
			c = b >> 2;
		else // if((i & 0x3) == 3)
			c = b;
		g_LMMC2b[i] = c  & 0x03;
	}

	// Init	
	Bios_SetHookCallback(H_TIMI, VBlankHook);
	g_SrcModeIndex = 3;
	SX = 8;
	SY = 32;
	DisplayPage();

	bool bContinue = TRUE;
	bool bEditing = FALSE;
	while(bContinue)
	{
		const struct ScreenSetting* src = &g_Settings[g_SrcModeIndex];

		Print_SetPosition(src->Width - 8, 2);
		Print_DrawChar(chrAnim[g_Frame & 0x03]);

		u8 row = Keyboard_Read(KEY_ROW(KEY_DOWN));
		if((row & KEY_FLAG(KEY_SPACE)) == 0)
		{
			bEditing = TRUE;
			if((row & KEY_FLAG(KEY_UP)) == 0)
			{
				if(SY > 0)
					--SY;
			}
			if((row & KEY_FLAG(KEY_DOWN)) == 0)
			{
				if(SY < 212)
					++SY;
			}
			if((row & KEY_FLAG(KEY_LEFT)) == 0)
			{
				if(SX > 0)
					--SX;
			}
			if((row & KEY_FLAG(KEY_RIGHT)) == 0)
			{
				++SX;
				if(SX >= src->Width)
					SX = src->Width - 1;
			}
		}
		else
		{
			if(((row & KEY_FLAG(KEY_UP)) == 0) || ((row & KEY_FLAG(KEY_LEFT)) == 0))
			{
				if(g_SrcModeIndex > 0)
					g_SrcModeIndex--;
				else
					g_SrcModeIndex = numberof(g_Settings) - 1;
				DisplayPage();
			}
			else if(((row & KEY_FLAG(KEY_DOWN)) == 0) || ((row & KEY_FLAG(KEY_RIGHT)) == 0))
			{
				if(g_SrcModeIndex < numberof(g_Settings) - 1)
					g_SrcModeIndex++;
				else
					g_SrcModeIndex = 0;
				DisplayPage();
			}

			if (bEditing)
			{
				DisplayPage();			
				bEditing = FALSE;
			}
			
			if(Keyboard_IsKeyPressed(KEY_ESC))
				bContinue = FALSE;
		}
		
		if(bEditing)
			VDP_SetSpriteExUniColor(0, SX, SY-1, 0, 0x8);
		else
			VDP_SetSpriteExUniColor(0, SX, (((g_Frame >> 3) & 1) == 0) ? SY-1 : 212, 0, 0xF);

		WaitVBlank();
	}

	Bios_ClearHook(H_TIMI);
	Bios_Exit(0);
}