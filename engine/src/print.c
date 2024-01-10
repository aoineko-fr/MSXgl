// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄       ▄       ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██▄▀ ▄  ██▀▄ ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ██   ██ ██ █ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "print.h"
#include "bios_mainrom.h"

//-----------------------------------------------------------------------------
//
// DEFINE
//
//-----------------------------------------------------------------------------

#if (PRINT_WIDTH == PRINT_WIDTH_6)
	#define DATA_LEN 6
#else
	#define DATA_LEN 8
#endif

//-----------------------------------------------------------------------------
// PROTOTYPE

#if (PRINT_USE_BITMAP)
// Draw character from RAM - 8-bits colors
void DrawChar_8B(u8 chr);
// Draw character from RAM - 4-bits colors
void DrawChar_4B(u8 chr);
// Draw character from RAM - 2-bits colors
void DrawChar_2B(u8 chr);

// Draw character from RAM - Multi-mode transparent 
void DrawChar_Trans(u8 chr);
#endif

#if (PRINT_USE_VRAM)
// Draw character from VRAM - 256 pixel screen
void DrawChar_VRAM256(u8 chr);
// Draw character from VRAM - 512 pixel screen
void DrawChar_VRAM512(u8 chr);
#endif

#if (PRINT_USE_SPRITE)
// Draw character from Sprites
void DrawChar_Sprite(u8 chr);
#endif

#if (PRINT_USE_TEXT)
// Draw characters as pattern name
void DrawChar_Layout(u8 chr);
#endif


//-----------------------------------------------------------------------------
//
// DATA
//
//-----------------------------------------------------------------------------

// Address of the heap top
extern u16 g_HeapStartAddress;

// Allocate memory for Print module data structure
struct Print_Data g_PrintData;

// Table use to quick decimal-to-hexadecimal conversion
const c8 g_HexChar[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

#if (PRINT_USE_VALIDATOR)
// Character use by character validator to show invalid character
u8 g_PrintInvalid[] =
{
	0xFF, /* ######## */ 
	0x81, /* #......# */ 
	0x81, /* #......# */ 
	0x81, /* #......# */ 
	0x81, /* #......# */ 
	0x81, /* #......# */ 
	0x81, /* #......# */ 
	0xFF, /* ######## */ 
};
#endif


//-----------------------------------------------------------------------------
//
// HELPER FUNCTIONS
//
//-----------------------------------------------------------------------------

#if ((PRINT_USE_BITMAP) || (PRINT_USE_SPRITE))

#if (PRINT_USE_VALIDATOR)
// Slit color from merged colors
u8 Print_SplitColor(u8 color)
{
	#if (PRINT_USE_SPRITE)
	if(g_PrintData.SourceMode == PRINT_MODE_SPRITE)
		return color;
	#endif

	switch(VDP_GetMode())
	{
	#if (VDP_USE_MODE_G4)
		case VDP_MODE_GRAPHIC4: return color & 0x0F;
	#endif
	#if (VDP_USE_MODE_G5)
		case VDP_MODE_GRAPHIC5: return color & 0x03;
	#endif
	#if (VDP_USE_MODE_G6)
		case VDP_MODE_GRAPHIC6: return color & 0x0F;
	#endif
	#if (VDP_USE_MODE_G7)
		case VDP_MODE_GRAPHIC7: return color;
	#endif
	}
	return color;
}
#endif // (PRINT_USE_VALIDATOR)

u8 Print_MergeColor(u8 color)
{
	switch(VDP_GetMode())
	{
	#if (VDP_USE_MODE_G4)
		case VDP_MODE_GRAPHIC4: return (color & 0x0F) << 4 | (color & 0x0F);
	#endif
	#if (VDP_USE_MODE_G5)
		case VDP_MODE_GRAPHIC5: return (color & 0x03) << 6 + (color & 0x03) << 4 + (color & 0x03) << 2 + (color & 0x03);
	#endif
	#if (VDP_USE_MODE_G6)
		case VDP_MODE_GRAPHIC6: return (color & 0x0F) << 4 | (color & 0x0F);
	#endif
	#if (VDP_USE_MODE_G7)
		case VDP_MODE_GRAPHIC7: return color;
	#endif
	}
	return color;
}
#endif // ((PRINT_USE_BITMAP) || (PRINT_USE_SPRITE))

//-----------------------------------------------------------------------------
//
// INITIALIZATION FUNCTIONS
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Initialize print module. Must be called after VDP_SetMode()
bool Print_Initialize()
{
	Print_SetColor(0xF, 0x0);
	Print_SetPosition(0, 0);
	#if (PRINT_USE_FX_SHADOW)
		Print_EnableShadow(FALSE);
	#endif
	#if (PRINT_USE_FX_OUTLINE)
		Print_EnableOutline(FALSE);
	#endif

	switch(VDP_GetMode()) // Screen mode specific initialization
	{
	#if (VDP_USE_MODE_T1)
		case VDP_MODE_TEXT1:		// 40 characters per line of text, one colour for each character
			Print_SetTabSize(3);
			g_PrintData.ScreenWidth = 40;
			break;
	#endif
	#if (VDP_USE_MODE_MC)
		case VDP_MODE_MULTICOLOR:	// pseudo-graphic, one character divided into four block
			break;
	#endif
	#if (VDP_USE_MODE_G1)
		case VDP_MODE_GRAPHIC1:		// 32 characters per one line of text, the COLOURed character available
			Print_SetTabSize(3);
			g_PrintData.ScreenWidth = 32;
			break;
	#endif
	#if (VDP_USE_MODE_G2)
		case VDP_MODE_GRAPHIC2:		// 256 x 192, the colour is specififed for each 8 dots
			Print_SetTabSize(3);
			g_PrintData.ScreenWidth = 32;
			break;
	#endif
	#if (VDP_USE_MODE_T2)
		case VDP_MODE_TEXT2:		// 80 characters per line of text, character blinkable selection
			Print_SetTabSize(3);
			g_PrintData.ScreenWidth = 80;
			break;
	#endif
	#if (VDP_USE_MODE_G3)
		#if (VDP_USE_UNDOCUMENTED)
		case VDP_MODE_GRAPHIC3_MIRROR_0:
		case VDP_MODE_GRAPHIC3_MIRROR_01:
		case VDP_MODE_GRAPHIC3_MIRROR_02:
		#endif
		case VDP_MODE_GRAPHIC3:		// GRAPHIC 2 which can use sprite mode 2
			Print_SetTabSize(3);
			g_PrintData.ScreenWidth = 32;
			break;
	#endif
	#if (VDP_USE_MODE_G4)
		case VDP_MODE_GRAPHIC4:		// 256 x 212; 16 colours are available for each dot
			Print_SetTabSize(PRINT_TAB_SIZE);
			g_PrintData.ScreenWidth = 256;
			break;
	#endif
	#if (VDP_USE_MODE_G5)
		case VDP_MODE_GRAPHIC5:		// 512 x 212; 4 colours are available for each dot
			Print_SetTabSize(PRINT_TAB_SIZE);
			Print_SetColor(0x03, 0x0);
			g_PrintData.ScreenWidth = 512;
			break;
	#endif
	#if (VDP_USE_MODE_G6)
		case VDP_MODE_GRAPHIC6:		// 512 x 212; 16 colours are available for each dot
			Print_SetTabSize(PRINT_TAB_SIZE);
			g_PrintData.ScreenWidth = 512;
			break;
	#endif
	#if (VDP_USE_MODE_G7)
		case VDP_MODE_GRAPHIC7:		// 256 x 212; 256 colours are available for each dot
			Print_SetTabSize(PRINT_TAB_SIZE);
			Print_SetColor(0xFF, 0x0);
			g_PrintData.ScreenWidth = 256;
			break;
	#endif
	default:
		// Screen mode not (yet) supported!
		return FALSE;
	}
	
	// g_PrintData.PatternsPerLine = g_PrintData.ScreenWidth / PRINT_W(g_PrintData.UnitX);

	return TRUE;
}

//-----------------------------------------------------------------------------
//
void Print_SetMode(u8 mode)
{
	g_PrintData.SourceMode = mode;

	switch(g_PrintData.SourceMode)
	{
	#if (PRINT_USE_BITMAP)
		case PRINT_MODE_BITMAP:
		{
			switch(VDP_GetMode()) // Screen mode specific initialization
			{
			#if (VDP_USE_MODE_G5)
				case VDP_MODE_GRAPHIC5:
					g_PrintData.DrawChar = DrawChar_2B;
					break;
			#endif
			#if (VDP_USE_MODE_G4 || VDP_USE_MODE_G6)
				case VDP_MODE_GRAPHIC4:
				case VDP_MODE_GRAPHIC6:
					g_PrintData.DrawChar = DrawChar_4B;
					break;
			#endif
			#if (VDP_USE_MODE_G7)
				case VDP_MODE_GRAPHIC7:
					g_PrintData.DrawChar = DrawChar_8B; 
					break;
			#endif
			}
			break;
		}
		#if (VDP_USE_MODE_G4 || VDP_USE_MODE_G5 || VDP_USE_MODE_G6 || VDP_USE_MODE_G7)
		case PRINT_MODE_BITMAP_TRANS:
		{
			g_PrintData.DrawChar = DrawChar_Trans;
			break;
		}
		#endif
	#endif
	#if (PRINT_USE_VRAM)
		case PRINT_MODE_BITMAP_VRAM:
		{
			switch(VDP_GetMode()) // Screen mode specific initialization
			{
			#if (VDP_USE_MODE_G4 || VDP_USE_MODE_G7)
				case VDP_MODE_GRAPHIC4:
				case VDP_MODE_GRAPHIC7:
					g_PrintData.DrawChar = DrawChar_VRAM256;
					break;
			#endif
			#if (VDP_USE_MODE_G5 || VDP_USE_MODE_G6)
				case VDP_MODE_GRAPHIC5:
				case VDP_MODE_GRAPHIC6:
					g_PrintData.DrawChar = DrawChar_VRAM512;
					break;
			#endif
			};
			break;
		}
	#endif
	#if (PRINT_USE_SPRITE)
		case PRINT_MODE_SPRITE:
		{
			g_PrintData.DrawChar = DrawChar_Sprite;
			break;
		}
	#endif
	#if (PRINT_USE_TEXT)
		case PRINT_MODE_TEXT:
		{
			g_PrintData.DrawChar = DrawChar_Layout;
			break;
		}
	#endif
	};
}

//-----------------------------------------------------------------------------
// Set the current font
// @param		font		Pointer to font data to use (NULL=use Main-ROM font)
void Print_SetFont(const u8* font)
{
	if(font == NULL) // Use Bios font (if any)
		Print_SetFontEx(8, 8, 6, 8, 1, 255, (const u8*)g_CGTABL + 8);
	else
		Print_SetFontEx(font[0] >> 4, font[0] & 0x0F, font[1] >> 4, font[1] & 0x0F, font[2], font[3], font+4);
}

#if ((PRINT_USE_BITMAP) || (PRINT_USE_SPRITE))
//-----------------------------------------------------------------------------
// Initialize color buffer
void Print_InitColorBuffer(u8 t, u8 b)
{
	// Pre-compute colors combinaison for 2-bits of a character pattern line (used to quick drawing in DrawChar_GX functions)
	switch(VDP_GetMode())
	{
	#if (VDP_USE_MODE_G4)
		case VDP_MODE_GRAPHIC4:
	#endif
	#if (VDP_USE_MODE_G6)
		case VDP_MODE_GRAPHIC6:
	#endif
	#if (VDP_USE_MODE_G4 || VDP_USE_MODE_G6)
			g_PrintData.Buffer[0] = (b << 4) | b;	// [ 0, 0 ]
			g_PrintData.Buffer[1] = (b << 4) | t;	// [ 0, 1 ]
			g_PrintData.Buffer[2] = (t << 4) | b;	// [ 1, 0 ]
			g_PrintData.Buffer[3] = (t << 4) | t;	// [ 1, 1 ]
			break;
	#endif
	#if (VDP_USE_MODE_G5)
		case VDP_MODE_GRAPHIC5:
			g_PrintData.Buffer[ 0] = (b << 6) | (b << 4) | (b << 2) | b;	// [ 0, 0, 0, 0 ]
			g_PrintData.Buffer[ 1] = (b << 6) | (b << 4) | (b << 2) | t;	// [ 0, 0, 0, 1 ]
			g_PrintData.Buffer[ 2] = (b << 6) | (b << 4) | (t << 2) | b;	// [ 0, 0, 1, 0 ]
			g_PrintData.Buffer[ 3] = (b << 6) | (b << 4) | (t << 2) | t;	// [ 0, 0, 1, 1 ]
			g_PrintData.Buffer[ 4] = (b << 6) | (t << 4) | (b << 2) | b;	// [ 0, 1, 0, 0 ]
			g_PrintData.Buffer[ 5] = (b << 6) | (t << 4) | (b << 2) | t;	// [ 0, 1, 0, 1 ]
			g_PrintData.Buffer[ 6] = (b << 6) | (t << 4) | (t << 2) | b;	// [ 0, 1, 1, 0 ]
			g_PrintData.Buffer[ 7] = (b << 6) | (t << 4) | (t << 2) | t;	// [ 0, 1, 1, 1 ]
			g_PrintData.Buffer[ 8] = (t << 6) | (b << 4) | (b << 2) | b;	// [ 1, 0, 0, 0 ]
			g_PrintData.Buffer[ 9] = (t << 6) | (b << 4) | (b << 2) | t;	// [ 1, 0, 0, 1 ]
			g_PrintData.Buffer[10] = (t << 6) | (b << 4) | (t << 2) | b;	// [ 1, 0, 1, 0 ]
			g_PrintData.Buffer[11] = (t << 6) | (b << 4) | (t << 2) | t;	// [ 1, 0, 1, 1 ]
			g_PrintData.Buffer[12] = (t << 6) | (t << 4) | (b << 2) | b;	// [ 1, 1, 0, 0 ]
			g_PrintData.Buffer[13] = (t << 6) | (t << 4) | (b << 2) | t;	// [ 1, 1, 0, 1 ]
			g_PrintData.Buffer[14] = (t << 6) | (t << 4) | (t << 2) | b;	// [ 1, 1, 1, 0 ]
			g_PrintData.Buffer[15] = (t << 6) | (t << 4) | (t << 2) | t;	// [ 1, 1, 1, 1 ]
			break;
	#endif
	#if (VDP_USE_MODE_G7)
		case VDP_MODE_GRAPHIC7:
			g_PrintData.Buffer[0] = b;	// [ 0, 0 ]
			g_PrintData.Buffer[1] = b;
			g_PrintData.Buffer[2] = b;	// [ 0, 1 ]
			g_PrintData.Buffer[3] = t;
			g_PrintData.Buffer[4] = t;	// [ 1, 0 ]
			g_PrintData.Buffer[5] = b;
			g_PrintData.Buffer[6] = t;	// [ 1, 1 ]
			g_PrintData.Buffer[7] = t;
			break;
	#endif
	}	
}
#endif // ((PRINT_USE_BITMAP) || (PRINT_USE_SPRITE))

//-----------------------------------------------------------------------------
// Set the draw color
// @param		text		Text color (format depend of current screen mode)
// @param		bg			Background color (format depend of current screen mode)
void Print_SetColor(u8 text, u8 bg)
{
	if(VDP_IsBitmapMode(VDP_GetMode())) // Bitmap mode
	{
		#if ((PRINT_USE_BITMAP) || (PRINT_USE_SPRITE))
			u8 t = text;
			#if (PRINT_USE_VALIDATOR)
				t = Print_SplitColor(t);
			#endif
			#if (PRINT_COLOR_NUM == 1)
				g_PrintData.TextColor = t;
			#else // if (PRINT_COLOR_NUM > 1)
				for(u8 i = 0; i < PRINT_COLOR_NUM; ++i)
				{
					g_PrintData.TextColor[i] = t;
				}
			#endif
			
			u8 b = bg;
			#if (PRINT_USE_VALIDATOR)
				b = Print_SplitColor(b);
			#endif
			g_PrintData.BGColor = b;

			Print_InitColorBuffer(t, b);
		#endif
	}
	else // Text mode
	{
		#if (PRINT_USE_TEXT)
			u8 col = text << 4 | bg;
			switch(VDP_GetMode())
			{
			#if (VDP_USE_MODE_T1)
				case VDP_MODE_TEXT1:		// 40 characters per line of text, one colour for each character
			#endif
			#if (VDP_USE_MODE_T2)
				case VDP_MODE_TEXT2:		// 80 characters per line of text, character blinkable selection
			#endif
			#if (VDP_USE_MODE_T1 || VDP_USE_MODE_T2)
					VDP_SetColor(col);
					break;
			#endif
			#if (VDP_USE_MODE_G1)
				case VDP_MODE_GRAPHIC1:		// 32 characters per one line of text, the COLOURed character available
					VDP_FillVRAM(col, g_ScreenColorLow, 0, 32);
					break;
			#endif
			#if (VDP_USE_MODE_G2)
				case VDP_MODE_GRAPHIC2:		// 256 x 192, the colour is specififed for each 8 dots
			#endif
			#if (VDP_USE_MODE_G3)
				case VDP_MODE_GRAPHIC3:		// GRAPHIC 2 which can use sprite mode 2
			#endif
			#if (VDP_USE_MODE_G2 || VDP_USE_MODE_G3)
				{
					u16 dst = (u16)g_ScreenColorLow + g_PrintData.PatternOffset * 8;
					VDP_FillVRAM(col, dst, 0, g_PrintData.CharCount * 8);
					dst += 256 * 8;
					VDP_FillVRAM(col, dst, 0, g_PrintData.CharCount * 8);
					dst += 256 * 8;
					VDP_FillVRAM(col, dst, 0, g_PrintData.CharCount * 8);
					break;
				}
			#endif
			};
		#endif
	}
}

#if (PRINT_COLOR_NUM > 1)
//-----------------------------------------------------------------------------
// Set color shade
void Print_SetColorShade(const u8* shade)
{
	if(VDP_IsBitmapMode(VDP_GetMode())) // Bitmap mode
	{
		#if ((PRINT_USE_BITMAP) || (PRINT_USE_SPRITE))
			for(u8 i = 0; i < PRINT_COLOR_NUM; ++i)
			{
				u8 t = shade[i];
				#if (PRINT_USE_VALIDATOR)
					t = Print_SplitColor(t);
				#endif
				g_PrintData.TextColor[i] = t;
			}
		#endif
	}
	else // Text mode
	{
		#if (PRINT_USE_TEXT)
			switch(VDP_GetMode())
			{
			#if (VDP_USE_MODE_G2)
				case VDP_MODE_GRAPHIC2:		// 256 x 192, the colour is specififed for each 8 dots
			#endif
			#if (VDP_USE_MODE_G3)
				case VDP_MODE_GRAPHIC3:		// GRAPHIC 2 which can use sprite mode 2
			#endif
			#if (VDP_USE_MODE_G2 || VDP_USE_MODE_G3)
				{
					u16 dst = (u16)g_ScreenColorLow + g_PrintData.PatternOffset * 8;
					for(u8 i = 0; i < g_PrintData.CharCount; ++i)
					{
						VDP_WriteVRAM(shade, dst,           0, 8);
						VDP_WriteVRAM(shade, dst + 256 * 8, 0, 8);
						VDP_WriteVRAM(shade, dst + 512 * 8, 0, 8);
						dst += 8;
					}
					break;
				}
			#endif
			};
		#endif
	}
}
#endif


//-----------------------------------------------------------------------------
//
// VALIDATOR
//
//-----------------------------------------------------------------------------

#if (PRINT_USE_VALIDATOR)
//-----------------------------------------------------------------------------
// Validate character. Try to convert invalid letter to their upper/lower case conterpart or use default invalid character
// @param		chr			Address of the character to check
u8 Print_ValidateChar(u8 chr)
{
	if((chr < g_PrintData.CharFirst) || (chr > g_PrintData.CharLast))
	{
		if((chr >= 'a') && (chr <= 'z') && (g_PrintData.CharFirst <= 'A') && (g_PrintData.CharLast >= 'Z')) // try to remap to upper case letter
		{
			chr = chr - 'a' + 'A';
		}
		else if((chr >= 'A') && (chr <= 'Z') && (g_PrintData.CharFirst <= 'a') && (g_PrintData.CharLast >= 'z')) // try to remap to lower case letter
		{
			chr = chr - 'A' + 'a';
		}
		else
			chr = g_PrintData.CharFirst;
	}
	return chr;
}
//-----------------------------------------------------------------------------
// Validate character. Try to convert invalid letter to their upper/lower case conterpart or use default invalid character
// @param		chr			Address of the character to check
// @param		patterns	Address of the font data to check
u8 Print_ValidatePattern(u8 chr, const c8** patterns)
{
	if((chr < g_PrintData.CharFirst) || (chr > g_PrintData.CharLast))
	{
		if((chr >= 'a') && (chr <= 'z') && (g_PrintData.CharFirst <= 'A') && (g_PrintData.CharLast >= 'Z')) // try to remap to upper case letter
		{
			chr = chr - 'a' + 'A';
			*patterns = g_PrintData.FontPatterns + g_PrintData.PatternY * (chr - g_PrintData.CharFirst);
		}
		else if((chr >= 'A') && (chr <= 'Z') && (g_PrintData.CharFirst <= 'a') && (g_PrintData.CharLast >= 'z')) // try to remap to lower case letter
		{
			chr = chr - 'A' + 'a';
			*patterns = g_PrintData.FontPatterns + g_PrintData.PatternY * (chr - g_PrintData.CharFirst);
		}
		else
			*patterns = g_PrintInvalid;
	}
	return chr;
}
#endif // PRINT_USE_VALIDATOR


//-----------------------------------------------------------------------------
//
// BITMAP FONT RAM
//
//-----------------------------------------------------------------------------

#if (PRINT_USE_BITMAP)
//-----------------------------------------------------------------------------
// Initialize print module. Must be called after VDP_SetMode()
// @param		font		Pointer to font data to use (NULL=use Main-ROM font)
bool Print_SetBitmapFont(const u8* font)
{
	Print_SetFont(font);
	Print_SetMode(PRINT_MODE_BITMAP);
	Print_Initialize();
}

#if (VDP_USE_MODE_G7)
//-----------------------------------------------------------------------------
// Graphic 7 (Screen mode 8) low-level function to draw a character in VRAM 
// @param		chr			The character to draw
void DrawChar_8B(u8 chr)
{
	const u8* patterns = g_PrintData.FontAddr + chr * PRINT_H(g_PrintData.PatternY); // Get character patterns' base address
	#if (PRINT_USE_VALIDATOR)
		chr = Print_ValidatePattern(chr, &patterns);
	#endif
	u16* l = (u16*)g_HeapStartAddress;
	for(u8 j = 0; j < PRINT_H(g_PrintData.PatternY); ++j) // Unpack each 6/8-bits line to buffer and send it to VRAM
	{
		#if (PRINT_COLOR_NUM > 1)
			Print_InitColorBuffer(g_PrintData.TextColor[j], g_PrintData.BGColor);
		#endif // (PRINT_COLOR_NUM > 1)
		u8 f = patterns[j];
		*l++ = ((u16*)g_PrintData.Buffer)[f >> 6];
		*l++ = ((u16*)g_PrintData.Buffer)[(f >> 4) & 0x03];
		*l++ = ((u16*)g_PrintData.Buffer)[(f >> 2) & 0x03];
		#if (PRINT_WIDTH != PRINT_WIDTH_6)
			*l++ = ((u16*)g_PrintData.Buffer)[f & 0x03];
		#endif
	}
	VDP_CommandHMMC((u8*)g_HeapStartAddress, g_PrintData.CursorX, g_PrintData.CursorY, DATA_LEN, PRINT_H(g_PrintData.PatternY));
}
#endif // VDP_USE_MODE_G7


#if (VDP_USE_MODE_G4 || VDP_USE_MODE_G6)
//-----------------------------------------------------------------------------
// Graphic 4/6 (Screen mode 5/7) low-level function to draw a character in VRAM 
// @param		chr			The character to draw
void DrawChar_4B(u8 chr)
{
	const u8* patterns = g_PrintData.FontAddr + chr * PRINT_H(g_PrintData.PatternY); // Get character patterns' base address
	#if (PRINT_USE_VALIDATOR)
		chr = Print_ValidatePattern(chr, &patterns);
	#endif
	u8* l = (u8*)g_HeapStartAddress;
	for(u8 j = 0; j < PRINT_H(g_PrintData.PatternY); ++j) // Unpack each 6/8-bits line to buffer and send it to VRAM
	{
		#if (PRINT_COLOR_NUM > 1)
			Print_InitColorBuffer(g_PrintData.TextColor[j], g_PrintData.BGColor);
		#endif // (PRINT_COLOR_NUM > 1)
		u8 f = patterns[j];
		*l++ = g_PrintData.Buffer[f >> 6];
		*l++ = g_PrintData.Buffer[(f >> 4) & 0x03];
		*l++ = g_PrintData.Buffer[(f >> 2) & 0x03];
		#if (PRINT_WIDTH != PRINT_WIDTH_6)
			*l++ = g_PrintData.Buffer[f & 0x03];
		#endif
	}
	VDP_CommandHMMC((u8*)g_HeapStartAddress, g_PrintData.CursorX, g_PrintData.CursorY, DATA_LEN, PRINT_H(g_PrintData.PatternY));
}
#endif // (VDP_USE_MODE_G4 || VDP_USE_MODE_G6)


#if (VDP_USE_MODE_G5)
//-----------------------------------------------------------------------------
// Graphic 5 (Screen mode 6) low-level function to draw a character in VRAM 
// @param		chr			The character to draw
void DrawChar_2B(u8 chr)
{
	const u8* patterns = g_PrintData.FontAddr + chr * PRINT_H(g_PrintData.PatternY); // Get character patterns' base address
	#if (PRINT_USE_VALIDATOR)
		chr = Print_ValidatePattern(chr, &patterns);
	#endif
	u8* l = (u8*)g_HeapStartAddress;
	for(u8 j = 0; j < PRINT_H(g_PrintData.PatternY); ++j) // Unpack each 6/8-bits line to buffer and send it to VRAM
	{
		#if (PRINT_COLOR_NUM > 1)
			Print_InitColorBuffer(g_PrintData.TextColor[j], g_PrintData.BGColor);
		#endif // (PRINT_COLOR_NUM > 1)
		u8 f = patterns[j];
		*l++ = g_PrintData.Buffer[f >> 4];
		*l++ = g_PrintData.Buffer[f & 0x0F];
	}
	VDP_CommandHMMC((u8*)g_HeapStartAddress, g_PrintData.CursorX, g_PrintData.CursorY, 8, PRINT_H(g_PrintData.PatternY));
}
#endif // VDP_USE_MODE_G5

#if (VDP_USE_MODE_G4 || VDP_USE_MODE_G5 || VDP_USE_MODE_G6 || VDP_USE_MODE_G7)
//-----------------------------------------------------------------------------
//
void DrawChar_Trans(u8 chr)
{
	const u8* patterns = g_PrintData.FontAddr + chr * PRINT_H(g_PrintData.PatternY); // Get character patterns' base address
	#if (PRINT_USE_VALIDATOR)
		chr = Print_ValidatePattern(chr, &patterns);
	#endif
	#if (PRINT_USE_FX_SHADOW)
		if(g_PrintData.FX & PRINT_FX_SHADOW)
		{
			g_VDP_Command.DY = g_PrintData.CursorY + g_PrintData.ShadowOffsetY - 3;
			g_VDP_Command.CLR = g_PrintData.ShadowColor;
			g_VDP_Command.ARG = 0;
			g_VDP_Command.CMD = VDP_CMD_PSET + 0;
			for(u8 j = 0; j < PRINT_H(g_PrintData.PatternY); ++j)
			{
				g_VDP_Command.DX = g_PrintData.CursorX + g_PrintData.ShadowOffsetX - 3;
				
				u8 f = patterns[j];
				if(f & BIT_7) VPD_CommandSetupR36(); g_VDP_Command.DX++;
				if(f & BIT_6) VPD_CommandSetupR36(); g_VDP_Command.DX++;
				if(f & BIT_5) VPD_CommandSetupR36(); g_VDP_Command.DX++;
				if(f & BIT_4) VPD_CommandSetupR36(); g_VDP_Command.DX++;
				if(f & BIT_3) VPD_CommandSetupR36(); g_VDP_Command.DX++;
				if(f & BIT_2) VPD_CommandSetupR36(); g_VDP_Command.DX++;
				if(f & BIT_1) VPD_CommandSetupR36(); g_VDP_Command.DX++;
				if(f & BIT_0) VPD_CommandSetupR36();
				g_VDP_Command.DY++;
			}
		}
	#endif
	#if (PRINT_USE_FX_OUTLINE)
		if(g_PrintData.FX & PRINT_FX_OUTLINE)
		{
			g_VDP_Command.DY = g_PrintData.CursorY - 1;
			g_VDP_Command.NX = 3;
			g_VDP_Command.NY = 3;
			g_VDP_Command.CLR = g_PrintData.OutlineColor;
			g_VDP_Command.ARG = 0;
			g_VDP_Command.CMD = (u8)(VDP_CMD_LMMV + 0);
			for(u8 j = 0; j < PRINT_H(g_PrintData.PatternY); ++j)
			{
				g_VDP_Command.DX = g_PrintData.CursorX - 1;
				
				u8 f = patterns[j];
				if(f & BIT_7) VPD_CommandSetupR36(); g_VDP_Command.DX++;
				if(f & BIT_6) VPD_CommandSetupR36(); g_VDP_Command.DX++;
				if(f & BIT_5) VPD_CommandSetupR36(); g_VDP_Command.DX++;
				if(f & BIT_4) VPD_CommandSetupR36(); g_VDP_Command.DX++;
				if(f & BIT_3) VPD_CommandSetupR36(); g_VDP_Command.DX++;
				if(f & BIT_2) VPD_CommandSetupR36(); g_VDP_Command.DX++;
				if(f & BIT_1) VPD_CommandSetupR36(); g_VDP_Command.DX++;
				if(f & BIT_0) VPD_CommandSetupR36();
				g_VDP_Command.DY++;
			}
		}
	#endif
	#if (PRINT_USE_2_PASS_FX)
	if(g_PrintData.FX & PRINT_FX_ONLY)
		return;
	#endif

	g_VDP_Command.DY = g_PrintData.CursorY;
	g_VDP_Command.ARG = 0;
	g_VDP_Command.CMD = VDP_CMD_PSET + 0;
	#if (PRINT_COLOR_NUM == 1)
		g_VDP_Command.CLR = g_PrintData.TextColor;
	#endif
	for(u8 j = 0; j < PRINT_H(g_PrintData.PatternY); ++j)
	{
		#if (PRINT_COLOR_NUM > 1)
			g_VDP_Command.CLR = g_PrintData.TextColor[j];
		#endif
		g_VDP_Command.DX = g_PrintData.CursorX;
		
		u8 f = patterns[j];
		if(f & BIT_7) VPD_CommandSetupR36(); g_VDP_Command.DX++;
		if(f & BIT_6) VPD_CommandSetupR36(); g_VDP_Command.DX++;
		if(f & BIT_5) VPD_CommandSetupR36(); g_VDP_Command.DX++;
		if(f & BIT_4) VPD_CommandSetupR36(); g_VDP_Command.DX++;
		if(f & BIT_3) VPD_CommandSetupR36(); g_VDP_Command.DX++;
		if(f & BIT_2) VPD_CommandSetupR36(); g_VDP_Command.DX++;
		if(f & BIT_1) VPD_CommandSetupR36(); g_VDP_Command.DX++;
		if(f & BIT_0) VPD_CommandSetupR36();
		g_VDP_Command.DY++;
	}
}
#endif // (VDP_USE_MODE_G4 || VDP_USE_MODE_G5 || VDP_USE_MODE_G6 || VDP_USE_MODE_G7)

#endif // (PRINT_USE_BITMAP)


//-----------------------------------------------------------------------------
//
// BITMAP FONT VRAM
//
//-----------------------------------------------------------------------------

#if (PRINT_USE_VRAM)
//-----------------------------------------------------------------------------
// Set the current font and upload it to VRAM
void Print_SetVRAMFont(const u8* font, UY y, u8 color)
{
	UX cx = g_PrintData.CursorX;
	UY cy = g_PrintData.CursorY;

	Print_SetFont(font);
	Print_Initialize();
	Print_SetColor(color, 0x0);
	
	// Load font to VRAM	
	Print_SetMode(PRINT_MODE_BITMAP_TRANS); // Activate default mode to write font data into VRAM
	g_PrintData.FontVRAMY = y;
	// @todo To optimize (pre-compute + fixed width/height cases
	u8 nx = g_PrintData.ScreenWidth / PRINT_W(g_PrintData.UnitX);
	for(u16 chr = g_PrintData.CharFirst; chr <= g_PrintData.CharLast; ++chr)
	{
		u16 idx = chr - g_PrintData.CharFirst;
		// @todo To optimize (pre-compute + fixed width/height cases
		g_PrintData.CursorX = (idx % nx) * PRINT_W(g_PrintData.UnitX);		
		g_PrintData.CursorY = (idx / nx) * PRINT_H(g_PrintData.PatternY) + y;
		VDP_CommandHMMV(g_PrintData.CursorX, g_PrintData.CursorY, PRINT_W(g_PrintData.UnitX), PRINT_H(g_PrintData.PatternY), 0);
		g_PrintData.DrawChar(chr);
	}

	g_PrintData.CursorX = cx;
	g_PrintData.CursorY = cy;
	
	g_PrintData.CharPerLine = g_PrintData.ScreenWidth / PRINT_W(g_PrintData.UnitX);
	
	Print_SetMode(PRINT_MODE_BITMAP_VRAM);
}

#if (VDP_USE_MODE_G4 || VDP_USE_MODE_G7)
//-----------------------------------------------------------------------------
// 
// @param		chr			The character to draw
void DrawChar_VRAM256(u8 chr)
{
	#if (PRINT_USE_VALIDATOR)
		chr = Print_ValidateChar(chr);
	#endif
	u8 idx = chr - g_PrintData.CharFirst;
	#if (PRINT_WIDTH == PRINT_WIDTH_6)
		u16 sx = (idx % 42) * PRINT_W(g_PrintData.UnitX);		
		u16 sy = (idx / 42) * PRINT_H(g_PrintData.PatternY) + g_PrintData.FontVRAMY;
	#elif (PRINT_WIDTH == PRINT_WIDTH_8)
		u16 sx = (idx % 32) * PRINT_W(g_PrintData.UnitX);		
		u16 sy = (idx / 32) * PRINT_H(g_PrintData.PatternY) + g_PrintData.FontVRAMY;
	#else
		u16 sx = (idx % g_PrintData.CharPerLine) * PRINT_W(g_PrintData.UnitX);		
		u16 sy = (idx / g_PrintData.CharPerLine) * PRINT_H(g_PrintData.PatternY) + g_PrintData.FontVRAMY;
	#endif

	VDP_CommandHMMM(sx, sy, g_PrintData.CursorX, g_PrintData.CursorY, PRINT_W(g_PrintData.UnitX), PRINT_H(g_PrintData.PatternY));
}
#endif // (VDP_USE_MODE_G4 || VDP_USE_MODE_G7)

#if (VDP_USE_MODE_G5 || VDP_USE_MODE_G6)
//-----------------------------------------------------------------------------
// 
// @param		chr			The character to draw
void DrawChar_VRAM512(u8 chr)
{
	#if (PRINT_USE_VALIDATOR)
		chr = Print_ValidateChar(chr);
	#endif
	u8 idx = chr - g_PrintData.CharFirst;
	#if (PRINT_WIDTH == PRINT_WIDTH_6)
		u16 sx = (idx % 85) * PRINT_W(g_PrintData.UnitX);		
		u16 sy = (idx / 85) * PRINT_H(g_PrintData.PatternY) + g_PrintData.FontVRAMY;
	#elif (PRINT_WIDTH == PRINT_WIDTH_8)
		u16 sx = (idx % 64) * PRINT_W(g_PrintData.UnitX);		
		u16 sy = (idx / 64) * PRINT_H(g_PrintData.PatternY) + g_PrintData.FontVRAMY;
	#else
		u16 sx = (idx % g_PrintData.CharPerLine) * PRINT_W(g_PrintData.UnitX);		
		u16 sy = (idx / g_PrintData.CharPerLine) * PRINT_H(g_PrintData.PatternY) + g_PrintData.FontVRAMY;
	#endif

	VDP_CommandHMMM(sx, sy, g_PrintData.CursorX, g_PrintData.CursorY, PRINT_W(g_PrintData.UnitX), PRINT_H(g_PrintData.PatternY));
}
#endif // (VDP_USE_MODE_G5 || VDP_USE_MODE_G6)

#endif // (PRINT_USE_VRAM)


//-----------------------------------------------------------------------------
//
// TEXT FONT
//
//-----------------------------------------------------------------------------

#if (PRINT_USE_TEXT)
#if (VDP_USE_MODE_T1 || VDP_USE_MODE_T2 || VDP_USE_MODE_G1 || VDP_USE_MODE_G2 || VDP_USE_MODE_G3)

#if (PRINT_USE_VALIDATOR)
void CopyNo8HeightFontData(const u8* src, u16 dst, u8 height)
{
	for(u8 i = 0; i < g_PrintData.CharCount; ++i)
	{
		VDP_FillVRAM(0, dst, 0, 8);
		VDP_WriteVRAM(src, dst, 0, height);
		src += height;
		dst += 8;
	}	
}
#endif

//-----------------------------------------------------------------------------
// Set the current font and upload it to VRAM
void Print_SetTextFont(const u8* fontData, u8 offset)
{
	g_PrintData.PatternOffset = offset;

	// Initialize font attributes
	if(fontData == NULL) // Use Bios font (if any)
		Print_SetFontEx(8, 8, 1, 1, 1, 255, (const u8*)g_CGTABL + 8); // @todo Should be [1, 255] to include all characters
	else
		Print_SetFontEx(8, 8, 1, 1, fontData[2], fontData[3], fontData+4);
	Print_Initialize();
	Print_SetMode(PRINT_MODE_TEXT);

	// Load font data to VRAM
	const u8* src = g_PrintData.FontPatterns;
	u16 dst = (u16)g_ScreenPatternLow + (offset * 8);
	#if (PRINT_USE_VALIDATOR)
		if(fontData != NULL)
			CopyNo8HeightFontData(src, dst, fontData[0] & 0x0F);
		else
	#endif
		VDP_WriteVRAM(src, dst, 0, g_PrintData.CharCount * 8);
	
	switch(VDP_GetMode())
	{
	#if (VDP_USE_MODE_G2)
		case VDP_MODE_GRAPHIC2:		// 256 x 192, the colour is specififed for each 8 dots
	#endif
	#if (VDP_USE_MODE_G3)
		case VDP_MODE_GRAPHIC3:		// GRAPHIC 2 which can use sprite mode 2
	#endif
	#if (VDP_USE_MODE_G2 || VDP_USE_MODE_G3)
		dst += 256 * 8;
		#if (PRINT_USE_VALIDATOR)
			if(fontData != NULL)
				CopyNo8HeightFontData(src, dst, fontData[0] & 0x0F);
			else
		#endif
			VDP_WriteVRAM(src, dst, 0, g_PrintData.CharCount * 8);
		dst += 256 * 8;
		#if (PRINT_USE_VALIDATOR)
			if(fontData != NULL)
				CopyNo8HeightFontData(src, dst, fontData[0] & 0x0F);
			else
		#endif
			VDP_WriteVRAM(src, dst, 0, g_PrintData.CharCount * 8);
		break;
	#endif
	};
}

//-----------------------------------------------------------------------------
// Draw characters as pattern name
// @param		chr			The character to draw
void DrawChar_Layout(u8 chr)
{
	#if (PRINT_USE_VALIDATOR)
		chr = Print_ValidateChar(chr);
	#endif
	u8 shape = chr - g_PrintData.CharFirst + g_PrintData.PatternOffset;
	u16 dst = (u16)g_ScreenLayoutLow + (g_PrintData.CursorY * g_PrintData.ScreenWidth) + g_PrintData.CursorX;
	VDP_Poke(shape, dst, g_ScreenLayoutHigh);
}

#endif
#endif


//-----------------------------------------------------------------------------
//
// SPRITE FONT
//
//-----------------------------------------------------------------------------

#if (PRINT_USE_SPRITE)

extern u16 g_SpritePatternLow;
extern u8  g_SpritePatternHigh;

//-----------------------------------------------------------------------------
// 
void Print_SetSpriteFont(const u8* font, u8 patIdx, u8 sprtIdx)
{
	g_PrintData.SpritePattern = patIdx;
	g_PrintData.SpriteID = sprtIdx;

	Print_SetFont(font);
	Print_Initialize();
	Print_SetMode(PRINT_MODE_SPRITE);

	#if (PRINT_HEIGHT == PRINT_HEIGHT_8)
		VDP_LoadSpritePattern(g_PrintData.FontPatterns, patIdx, g_PrintData.CharCount);
	#else // (PRINT_HEIGHT == PRINT_HEIGHT_X)
		u16 ram = (u16)g_PrintData.FontPatterns;
		u16 vram = g_SpritePatternLow;
		for(u16 chr = g_PrintData.CharFirst; chr <= g_PrintData.CharLast; ++chr)
		{
			u16 idx = chr - g_PrintData.CharFirst;
			if(PRINT_H(g_PrintData.PatternY) < 8)
			{
				VDP_WriteVRAM((u8*)ram, vram, g_SpritePatternHigh, PRINT_H(g_PrintData.PatternY));
				vram += PRINT_H(g_PrintData.PatternY);
				VDP_FillVRAM(0, vram, g_SpritePatternHigh, 8 - PRINT_H(g_PrintData.PatternY));
				vram += 8 - PRINT_H(g_PrintData.PatternY);
			}
			else
			{
				VDP_WriteVRAM((u8*)ram, vram, g_SpritePatternHigh, 8);
				vram += 8;
			}
			ram += PRINT_H(g_PrintData.PatternY);
		}
	#endif
}


//-----------------------------------------------------------------------------
// 
// @param		chr			The character to draw
void DrawChar_Sprite(u8 chr)
{
	u16 shape = chr - g_PrintData.CharFirst + g_PrintData.SpritePattern;
	#if (PRINT_COLOR_NUM == 1)
		VDP_SetSpriteExUniColor(g_PrintData.SpriteID++, g_PrintData.CursorX, g_PrintData.CursorY, shape, g_PrintData.TextColor);
	#else // (PRINT_COLOR_NUM > 1)
		VDP_SetSpriteExMultiColor(g_PrintData.SpriteID++, g_PrintData.CursorX, g_PrintData.CursorY, shape, g_PrintData.TextColor);
	#endif
}

#endif // (PRINT_USE_SPRITE)


//-----------------------------------------------------------------------------
//
// FX
//
//-----------------------------------------------------------------------------

#if (PRINT_USE_FX_SHADOW)
//-----------------------------------------------------------------------------
// Set shadow effect
// @param		activate	Activate/deactivate shadow
// @param		offsetX		Shadow offset on X axis (can be from -3 to +4)
// @param		offsetY		Shadow offset on Y axis (can be from -3 to +4)
// @param		color		Shadow color (depend of the screen mode)
void Print_SetShadow(bool enable, i8 offsetX, i8 offsetY, u8 color)
{
	Print_EnableShadow(enable);
	g_PrintData.ShadowOffsetX = 3 + offsetX;
	g_PrintData.ShadowOffsetY = 3 + offsetY;
	g_PrintData.ShadowColor   = color;
}

//-----------------------------------------------------------------------------
// Activate/desactivate shadow effect
// @param		activate	Activate/deactivate shadow
void Print_EnableShadow(bool enable)
{
	g_PrintData.FX = enable ? PRINT_FX_SHADOW : 0;
	Print_SetMode(enable ? PRINT_MODE_BITMAP_TRANS : PRINT_MODE_BITMAP); // enable default mode to write font data into VRAM
}

#if (PRINT_USE_2_PASS_FX)
//-----------------------------------------------------------------------------
// Print a character string using shadow FX.
void Print_DrawTextShadow(const c8* string, i8 offsetX, i8 offsetY, u8 color)
{
	// First pass
	UX x = g_PrintData.CursorX;
	UY y = g_PrintData.CursorY;
	Print_SetShadow(TRUE, offsetX, offsetY, color);
	g_PrintData.FX |= PRINT_FX_ONLY;
	Print_DrawText(string);
	// Second pass
	g_PrintData.FX = 0;
	Print_SetPosition(x, y);
	Print_DrawText(string);
	Print_EnableShadow(FALSE);
}
#endif // (PRINT_USE_2_PASS_FX)
#endif // (PRINT_USE_FX_SHADOW)

#if (PRINT_USE_FX_OUTLINE)
//-----------------------------------------------------------------------------
// Set shadow effect
void Print_SetOutline(bool enable, u8 color)
{
	Print_EnableOutline(enable);
	g_PrintData.OutlineColor = color;
}

//-----------------------------------------------------------------------------
// Activate/desactivate shadow effect
void Print_EnableOutline(bool enable)
{
	g_PrintData.FX = enable ? PRINT_FX_OUTLINE : 0;
	Print_SetMode(enable ? PRINT_MODE_BITMAP_TRANS : PRINT_MODE_BITMAP); // enable default mode to write font data into VRAM
}

#if (PRINT_USE_2_PASS_FX)
//-----------------------------------------------------------------------------
// Print a character string using outline FX.
void Print_DrawTextOutline(const c8* string, u8 color)
{
	// First pass
	UX x = g_PrintData.CursorX;
	UY y = g_PrintData.CursorY;
	Print_SetOutline(TRUE, color);
	g_PrintData.FX |= PRINT_FX_ONLY;
	Print_DrawText(string);
	// Second pass
	g_PrintData.FX = 0;
	Print_SetPosition(x, y);
	Print_DrawText(string);
	Print_EnableShadow(FALSE);
}
#endif // (PRINT_USE_2_PASS_FX)
#endif // (PRINT_USE_FX_OUTLINE)

//-----------------------------------------------------------------------------
//
// DRAW FUNCTION
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Clear screen on the current page
void Print_Clear()
{
	if(VDP_IsBitmapMode(VDP_GetMode())) // Bitmap mode
	{
		#if (PRINT_USE_BITMAP)
			u8 color = Print_MergeColor(g_PrintData.BGColor);
			VDP_CommandHMMV(0, 0, g_PrintData.ScreenWidth, 212, color); // @todo Check the 192/212 lines parameter
			VDP_CommandWait();
		#endif
	}
	#if (PRINT_USE_TEXT)
	else // Text mode
	{
		VDP_FillVRAM(0, g_ScreenLayoutLow, g_ScreenLayoutHigh, 24 * g_PrintData.ScreenWidth);
	}
	#endif
}

//-----------------------------------------------------------------------------
// Clear X character back from current cursor position
// @param		num			Number of characters to remove
void Print_Backspace(u8 num)
{
	if(VDP_IsBitmapMode(VDP_GetMode())) // Bitmap mode
	{
		#if (PRINT_USE_BITMAP)
			u16 x = PRINT_W(g_PrintData.UnitX) * num;
			if(x >  g_PrintData.CursorX)
				x = 0;
			else
				x = g_PrintData.CursorX - x;

			u8 color = Print_MergeColor(g_PrintData.BGColor);
			VDP_CommandHMMV(x, g_PrintData.CursorY, x - g_PrintData.CursorX, PRINT_H(g_PrintData.UnitY), color); // @todo Check the 192/212 lines parameter
			g_PrintData.CursorX = x;
			VDP_CommandWait();	
		#endif
	}
	else // Text mode
	{
		#if (PRINT_USE_TEXT)
			u16 dst = g_ScreenLayoutLow + (g_PrintData.CursorY * g_PrintData.ScreenWidth) + g_PrintData.CursorX - num;
			VDP_FillVRAM(0, dst, g_ScreenLayoutHigh, num);
			g_PrintData.CursorX -= num;
		#endif
	}
}

//-----------------------------------------------------------------------------
// Print a single character
// @param		chr			The character to draw
void Print_DrawChar(u8 chr)
{
	#if (PRINT_USE_VALIDATOR)
		if(g_PrintData.CursorX + PRINT_W(g_PrintData.UnitX) > g_PrintData.ScreenWidth) // Handle automatic new-line when 
			Print_Return();
		#if (MSX_VERSION >= MSX_2)
			VDP_CommandWait();
		#endif
	#endif
	g_PrintData.DrawChar(chr);

	// g_PrintData.DrawChar(chr);
	g_PrintData.CursorX += PRINT_W(g_PrintData.UnitX);
}

//-----------------------------------------------------------------------------
// Print the same character many times
// @param		chr			Character to draw
// @param		num			Number of drawing
void Print_DrawCharX(c8 chr, u8 num)
{
	for(u8 i = 0; i < num; ++i)
		Print_DrawChar(chr);
}

//-----------------------------------------------------------------------------
// Print a character string
// @param		chr			String to draw (must be NULL-terminated)
void Print_DrawText(const c8* str)
{
	while(*str != 0)
	{
		if(*str == '\t')
			Print_Tab();
		else if(*str == '\n')
			Print_Return();
	#if (PRINT_SKIP_SPACE)
		else if(*str == ' ')
			Print_Space();
	#endif
		else
			Print_DrawChar(*str);
		str++;
	}
}

//-----------------------------------------------------------------------------
// Print a 8-bits binary value
// @param		value		Value to print
void Print_DrawBin8(u8 value)
{
	u8 flag = (u8)(1 << 7);
	for(u8 i = 0; i < 8; ++i)
	{
		Print_DrawChar((value & flag) ? '1' : '0');
		flag >>= 1;
	}
	#if (PRINT_USE_UNIT)
		Print_DrawChar('b');
	#endif
}

//-----------------------------------------------------------------------------
// Print a 8-bits hexadecimal value
// @param		value		Value to print
void Print_DrawHex8(u8 value)
{
	Print_DrawChar(g_HexChar[(value >> 4) & 0x000F]);
	Print_DrawChar(g_HexChar[value & 0x000F]);
	#if (PRINT_USE_UNIT)
		Print_DrawChar('h');
	#endif
}

//-----------------------------------------------------------------------------
// Print a 16-bits hexadecimal value
// @param		value		Value to print
void Print_DrawHex16(u16 value)
{
	Print_DrawHex8(value >> 8);
	Print_DrawHex8((u8)value);
}

#if (PRINT_USE_32B)
//-----------------------------------------------------------------------------
// Print a 16-bits hexadecimal value
// @param		value		Value to print
void Print_DrawHex32(u32 value)
{
	Print_DrawHex16(value >> 16);
	Print_DrawHex16((u16)value);
}
#endif

//-----------------------------------------------------------------------------
#if (PRINT_USE_32B)
// Print a 32-bits signed decimal value
// @param		value		Value to print
void Print_DrawInt(i32 value)
#else
// Print a 16-bits signed decimal value
// @param		value		Value to print
void Print_DrawInt(i16 value)
#endif
{
	if(value < 0)
	{	
		Print_DrawChar('-');
		value = -value;
	}
	
	c8 str[8];
	c8* ptr = str;
	*ptr = 0;
	while(value >= 10)
	{
		*++ptr = '0' + (value % 10);
		value /= 10;
	}
	*++ptr = '0' + value;
	while(*ptr != 0)
		Print_DrawChar(*ptr--);	
}

//-----------------------------------------------------------------------------
// FORMAT FUNCTION
//-----------------------------------------------------------------------------
#if (PRINT_USE_FORMAT)

//-----------------------------------------------------------------------------
// Print a formated string with a variable number of parameters
void Print_DrawFormat(const c8* format, ...)
{
	// @todo To be replaced by String_Format() + Print_DrawText()

	va_list args;
	va_start(args, format);

	const c8* ptr = format;
	while(*ptr != 0)
	{
		if(*ptr == '%')
		{
			ptr++;
			
			// Parse length
			u8 len = 0;
			if((*ptr >= '0') && (*ptr <= '9'))
			{
				len = *ptr - '0';
				ptr++;
			}
			while((*ptr >= '0') && (*ptr <= '9'))
			{
				len *= 10;
				len += *ptr - '0';
				ptr++;
			}

			// Parse variable types
			if((*ptr == 'i') || (*ptr == 'd'))
			{
				i16 val = (i16)va_arg(args, i16);
				Print_DrawInt(val);
			}
			else if(*ptr == 'u')
			{
				u16 val = (u16)va_arg(args, u16);
				Print_DrawInt(val);
			}
			else if(*ptr == 'x')
			{
				if(len == 0)
					len = 4;
				u16 val = (u16)va_arg(args, u16);
				if(len > 3)
					Print_DrawChar(g_HexChar[(val >> 12) & 0x000F]);
				if(len > 2)
					Print_DrawChar(g_HexChar[(val >> 8) & 0x000F]);
				if(len > 1)
					Print_DrawChar(g_HexChar[(val >> 4) & 0x000F]);
				Print_DrawChar(g_HexChar[val & 0x000F]);
			}
			else if(*ptr == 'b')
			{
				if(len == 0)
					len = 16;
				u16 val = (u16)va_arg(args, u16);
				u16 bit = 1 << (len - 1);
				while(bit)
				{
					if(val & bit)
						Print_DrawChar('1');
					else
						Print_DrawChar('0');
					bit >>= 1;
				}
			}
		#if (PRINT_USE_32B)
			else if((*ptr == 'I') || (*ptr == 'D'))
			{
				i32 val = (i32)va_arg(args, i32);
				Print_DrawInt(val);
			}
			else if(*ptr == 'U')
			{
				u32 val = (u32)va_arg(args, u32);
				Print_DrawInt(val);
			}
			else if(*ptr == 'X')
			{
				if(len == 0)
					len = 8;
				u32 val = (u32)va_arg(args, u32);
				// Print_DrawHex32(val);
				if(len > 7)
					Print_DrawChar(g_HexChar[(val >> 28) & 0xF]);
				if(len > 6)
					Print_DrawChar(g_HexChar[(val >> 24) & 0xF]);
				if(len > 5)
					Print_DrawChar(g_HexChar[(val >> 20) & 0xF]);
				if(len > 4)
					Print_DrawChar(g_HexChar[(val >> 16) & 0xF]);
				if(len > 3)
					Print_DrawChar(g_HexChar[(val >> 12) & 0xF]);
				if(len > 2)
					Print_DrawChar(g_HexChar[(val >> 8) & 0xF]);
				if(len > 1)
					Print_DrawChar(g_HexChar[(val >> 4) & 0xF]);
				Print_DrawChar(g_HexChar[val & 0xF]);
			}
			else if(*ptr == 'B')
			{
				if(len == 0)
					len = 16;
				u32 val = (u32)va_arg(args, u32);
				u32 bit = 1 << (len - 1);
				while(bit)
				{
					if(val & bit)
						Print_DrawChar('1');
					else
						Print_DrawChar('0');
					bit >>= 1;
				}
			}
		#endif
			else if(*ptr == 'c')
			{
				c8 val = (c8)va_arg(args, u16);
				Print_DrawChar(val);
			}
			else if(*ptr == 's')
			{
				const c8* val = (const c8*)va_arg(args, const c8*);
				Print_DrawText(val);
			}
			else if(*ptr == '%')
			{
				Print_DrawChar('%');
			}
		}
		// Parse special character
		else if(*ptr == '\t')
			Print_Tab();
		else if(*ptr == '\n')
			Print_Return();
		else if(*ptr == ' ')
			Print_Space();
		else
			Print_DrawChar(*ptr);
		
		ptr++;
	}

	va_end(args);
}
#endif

//-----------------------------------------------------------------------------
// GRAPH FUNCTIONS
//-----------------------------------------------------------------------------
#if (PRINT_USE_GRAPH)

//-----------------------------------------------------------------------------
// Draw an horizontal line using characters
void Print_DrawLineH(u8 x, u8 y, u8 len)
{
	Print_SetPosition(x, y);
	Print_DrawCharX(0x17, len);
}

//-----------------------------------------------------------------------------
// Draw a vertical line using characters
void Print_DrawLineV(u8 x, u8 y, u8 len)
{
	for(u8 i = 0; i < len; i++)
	{
		Print_SetPosition(x, y + i);
		Print_DrawChar(0x16);
	}
}

//-----------------------------------------------------------------------------
// Draw a box using characters
void Print_DrawBox(u8 x, u8 y, u8 width, u8 height)
{
	// Draw corners
	Print_SetPosition(x, y);
	Print_DrawChar(0x18);
	Print_SetPosition(x + width - 1, y);
	Print_DrawChar(0x19);
	Print_SetPosition(x, y + height - 1);
	Print_DrawChar(0x1A);
	Print_SetPosition(x + width - 1, y + height - 1);
	Print_DrawChar(0x1B);

	// Draw horizontal lines
	Print_DrawLineH(x + 1, y,              width - 2);
	Print_DrawLineH(x + 1, y + height - 1, width - 2);
	
	// Draw vertical lines
	Print_DrawLineV(x,             y + 1, height - 2);
	Print_DrawLineV(x + width - 1, y + 1, height - 2);
}

#endif // (PRINT_USE_GRAPH)