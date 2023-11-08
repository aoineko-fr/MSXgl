// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄       ▄       ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██▄▀ ▄  ██▀▄ ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ██   ██ ██ █ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"
#include "vdp.h"
#include "string.h"

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

// Character display sources
enum PRINT_MODE
{
	// Bitmap modes (from RAM)
#if ((PRINT_USE_BITMAP) || (PRINT_USE_VRAM))
	PRINT_MODE_BITMAP		= 0,	//< Draw characters from RAM (R-T unpack font data and draw it)
	PRINT_MODE_BITMAP_TRANS	= 1,	//< Draw characters from RAM with transparency (R-T unpack font data and draw it)
#endif
	// Bitmap mode (from VRAM)
#if (PRINT_USE_VRAM)
	PRINT_MODE_BITMAP_VRAM	= 2,	//< Draw characters from VRAM (font data is upack once in VRAM thne drawing is done by VRAM copy)
#endif
	// Sprite mode
#if (PRINT_USE_SPRITE)
	PRINT_MODE_SPRITE		= 3,	//< Draw characters from sprites (load font data as sprite pattern in VRAM then display characters using sprite system)
#endif
	// Text mode
#if (PRINT_USE_TEXT)
	PRINT_MODE_TEXT			= 4,	//< Draw characters as pattern names (text mode)
#endif
	PRINT_MODE_MAX,
};

// Handle fixed of variables character width
#if (PRINT_WIDTH == PRINT_WIDTH_1)
	#define PRINT_W(a) 1
#elif (PRINT_WIDTH == PRINT_WIDTH_6)
	#define PRINT_W(a) 6
#elif (PRINT_WIDTH == PRINT_WIDTH_8)
	#define PRINT_W(a) 8
#else // (PRINT_WIDTH == PRINT_WIDTH_X)
	#define PRINT_W(a) (a)
#endif

// Handle fixed of variables character height
#if (PRINT_HEIGHT == PRINT_HEIGHT_1)
	#define PRINT_H(a) 1
#elif (PRINT_HEIGHT == PRINT_HEIGHT_8)
	#define PRINT_H(a) 8
#else // (PRINT_HEIGHT == PRINT_HEIGHT_X)
	#define PRINT_H(a) a
#endif

#define PRINT_TAB_SIZE				24
#define PRINT_DEFAULT_FONT			NULL

// Functions
typedef void (*print_drawchar)(u8); //< Draw char callback signature
typedef void (*print_loadfont)(VADDR); //< Font load callback signature

extern struct Print_Data g_PrintData;

#define PRINT_FX_SHADOW				0b00000001
#define PRINT_FX_OUTLINE			0b00000010
#define PRINT_FX_ONLY				0b10000000

//-----------------------------------------------------------------------------
// STRUCTURES
//-----------------------------------------------------------------------------

// Print module configuration structure
struct Print_Data
{
	u8 PatternX;				//< X size of a character in screen unit (0-15)
	u8 PatternY;				//< Y size of a character in screen unit (0-15)
	u8 UnitX;					//< X size of a character in screen unit (0-15)
	u8 UnitY;					//< Y size of a character in screen unit (0-15)
	u8 TabSize;					//< Tabulation size in pixel (must be a power of 2 like 16, 32, 64, ...)
	UX CursorX;					//< Current X position (screen coordinate)
	UY CursorY;					//< Current Y position (screen coordinate)
#if (PRINT_COLOR_NUM == 1)
	u8 TextColor;				//< Text color
#else // (PRINT_COLOR_NUM > 1)
	u8 TextColor[PRINT_COLOR_NUM]; //< Text colors (1 per line)
#endif
	u8 BGColor;					//< Background color
	u8 CharFirst;				//< ASCII code of the first character of the current font
	u8 CharLast;				//< ASCII code of the last character of the current font
	u8 CharCount;				//< Characters count (/!\ max is 255)
	print_drawchar DrawChar;	//< Default function to draw a character (depend of the current mode)
	u8 SourceMode       : 4;	//< Character display mode : RAM, VRAM or Sprite (@see PRINT_MODE)
	u16 ScreenWidth;			//< Screen width
	// Bitmap from RAM
// #if (PRINT_USE_)
	const u8* FontPatterns;		//< Forms of the font
	const u8* FontAddr;			//< Address of the virtual index 0 character (used to quick drawing in DrawChar_GX functions)
// #endif
#if (PRINT_USE_VRAM)
	// Bitmap from VRAM
	UY FontVRAMY;				//< Y position of the font in VRAM
	u8 CharPerLine;
#endif
#if (PRINT_USE_SPRITE)
	// Sprites
	u8 SpritePattern;			//< Pattern index of the 1st sprite character
	u8 SpriteID;				//< Index of the current sprite
#endif
#if (PRINT_USE_TEXT)
	// Text mode
	u8 PatternOffset;
#endif
#if ((PRINT_USE_FX_SHADOW) || (PRINT_USE_FX_OUTLINE))
	u8 FX;
#endif
#if (PRINT_USE_FX_SHADOW)
	u8 ShadowOffsetX	: 3;	//< Shadow X offset (0:7 => -3:+4)
	u8 ShadowOffsetY	: 3;	//< Shadow Y offset (0:7 => -3:+4)
	u8 ShadowColor;				//< Shadow color
#endif
#if (PRINT_USE_FX_OUTLINE)
	u8 OutlineColor;			//< Shadow color
#endif
	u8 Buffer[16];				//< Mode specifique buffer (used to pre-compute color combinations)
};

//-----------------------------------------------------------------------------
// Group: Initialization
// Font initialization functions
//-----------------------------------------------------------------------------

// Function: Print_Initialize
// Initialize print module.
// Must be called after VDP_SetMode().
bool Print_Initialize();

// Function: Print_SetMode
// Change current print mode.
//
// Parameters:
//   mode - Print mode to select. Can be:
// > PRINT_MODE_BITMAP          Draw characters from RAM (R-T unpack font data and draw it). Need PRINT_USE_BITMAP or PRINT_USE_VRAM compile option.
// > PRINT_MODE_BITMAP_TRANS    Draw characters from RAM with transparency (R-T unpack font data and draw it). Need PRINT_USE_BITMAP or PRINT_USE_VRAM compile option.
// > PRINT_MODE_BITMAP_VRAM     Draw characters from VRAM (font data is upack once in VRAM thne drawing is done by VRAM copy). Need PRINT_USE_VRAM compile option.
// > PRINT_MODE_SPRITE          Draw characters from sprites (load font data as sprite pattern in VRAM then display characters using sprite system). Need PRINT_USE_SPRITE compile option.
// > PRINT_MODE_TEXT            Draw characters as pattern names (text mode). Need PRINT_USE_TEXT compile option.
void Print_SetMode(u8 mode);

// Function: Print_SetFont
// Set the current font (and set mode to RAM).
//
// Parameters:
//   font - Pointer to font structure. Character patterns plus a 4 bytes headers (character width/height, spacing width/height, 1st character code, last character code).
void Print_SetFont(const u8* font);

// Function: Print_SetFontEx
// Set the current font
//
// Parameters:
//   patternX - Width of the character data (should be always 8)
//   patternY - Height of the character data
//   sizeX    - Width of the chatacter to display (can be different than the one of the data)
//   sizeY    - Height of the chatacter to display (can be different than the one of the data)
//   firstChr - ASCII code of the first character included in the font data
//   lastChr  - ASCII code of the last character included in the font data
//   patterns - Address of the character data table
inline void Print_SetFontEx(u8 patternX, u8 patternY, u8 sizeX, u8 sizeY, u8 firstChr, u8 lastChr, const u8* patterns)
{
	g_PrintData.PatternX     = patternX;
	g_PrintData.PatternY     = patternY;
	g_PrintData.UnitX        = sizeX;
	g_PrintData.UnitY        = sizeY;
	g_PrintData.CharFirst    = firstChr;
	g_PrintData.CharLast     = lastChr;
	g_PrintData.CharCount    = lastChr - firstChr + 1;
	g_PrintData.FontPatterns = patterns;
	g_PrintData.FontAddr     = g_PrintData.FontPatterns - (firstChr * g_PrintData.PatternY); // pre-compute address of the virtual index 0 character (used to quick drawing in PutChar_GX functions)
}

// Function: Print_GetFontInfo
// Get first character code
//
// Return:
//   Current font information structure
inline const struct Print_Data* Print_GetFontInfo()
{
	return &g_PrintData;
}

// Function: Print_SetPosition
// Set cursor position.
//
// Parameters:
//   X - Print cursor position X coordinate
//   Y - Print cursor position Y coordinate
inline void Print_SetPosition(UX x, UY y)
{
	g_PrintData.CursorX = x;
	g_PrintData.CursorY = y;
}

// Function: Print_SetCharSize
// Set character size.
//
// Parameters:
//   X - Character width
//   Y - Character height
inline void Print_SetCharSize(u8 x, u8 y)
{
	g_PrintData.UnitX = x;
	g_PrintData.UnitY = y;
}

// Function: Print_SetTabSize
// Set tabulation size in pixel.
//
// Parameters:
//   size - Size of the tabulation in pixel (must be a power of 2 like 16, 32, 64, ...)
inline void Print_SetTabSize(u8 size)
{
	g_PrintData.TabSize = size;
}

#if (PRINT_USE_BITMAP)
// Function: Print_SetBitmapFont
// Initialize print module and set a font in RAM.
// This function requires PRINT_USE_BITMAP compile option to be set to TRUE.
//
// Parameters:
//   font - Pointer to font structure. Character patterns plus a 4 bytes headers (character width/height, spacing width/height, 1st character code, last character code).
bool Print_SetBitmapFont(const u8* font);
#endif

#if (PRINT_USE_VRAM)
// Function: Print_SetVRAMFont
// Set the current font and upload data to VRAM .
// This function requires PRINT_USE_VRAM compile option to be set to TRUE.
//
// Parameters:
//   font  - Pointer to font structure. Character patterns plus a 4 bytes headers (character width/height, spacing width/height, 1st character code, last character code).
//   y     - Y position to store the font in VRAM
//   color - Color of the font
void Print_SetVRAMFont(const u8* font, UY y, u8 color);
#endif

#if (PRINT_USE_TEXT)
// Function: Print_SetTextFont
// Initialize print module and set a font in RAM.
// This function requires PRINT_USE_TEXT compile option to be set to TRUE.
//
// Parameters:
//   font   - Pointer to font structure. Character patterns plus a 4 bytes headers (character width/height, spacing width/height, 1st character code, last character code).
//   offset - Pattern index where to start to store the font
void Print_SetTextFont(const u8* font, u8 offset);

// Function: Print_SelectTextFont
// Set print mode to text mode.
// This function requires PRINT_USE_TEXT compile option to be set to TRUE.
//
// Parameters:
//   font   - Pointer to font structure. Character patterns plus a 4 bytes headers (character width/height, spacing width/height, 1st character code, last character code).
//   offset - Pattern index where to start to store the font
inline void Print_SelectTextFont(const u8* font, u8 offset)
{
	g_PrintData.PatternOffset = offset;
	Print_SetFontEx(8, 8, 1, 1, font[2], font[3], font+4);
	// Print_Initialize();
	// Print_SetMode(PRINT_MODE_TEXT);
}
#endif

#if (PRINT_USE_SPRITE)
// Function: Print_SetSpriteFont
// Set the current font and upload to Sprite Pattern Table.
// This function requires PRINT_USE_SPRITE compile option to be set to TRUE.
//
// Parameters:
//   font    - Pointer to font structure. Character patterns plus a 4 bytes headers (character width/height, spacing width/height, 1st character code, last character code).
//   patIdx  - Pattern index to put font character
//   sprtIdx - Sprite index of the first sprite the module can use to display text
void Print_SetSpriteFont(const u8* font, u8 patIdx, u8 sprtIdx);

// Function: Print_GetSpritePattern
// Get pattern index of the 1st sprite character.
// This function requires PRINT_USE_SPRITE compile option to be set to TRUE.
//
// Return:
//   Pattern index where the font is stored
inline u8 Print_GetSpritePattern() { return g_PrintData.SpritePattern; }

// Function: Print_GetSpriteID
// Get the next sprite index.
// This function requires PRINT_USE_SPRITE compile option to be set to TRUE.
//
// Return:
//   Sprite index of the first sprite the module can use to display text
inline u8 Print_GetSpriteID() { return g_PrintData.SpriteID; }

// Function: Print_SetSpriteID
// Set the next sprite index.
// This function requires PRINT_USE_SPRITE compile option to be set to TRUE.
//
// Parameters:
//   id - Sprite index of the first sprite the module can use to display text
inline void Print_SetSpriteID(u8 id) { g_PrintData.SpriteID = id; }

#endif


//-----------------------------------------------------------------------------
// Group: Draw
// Text draw functions
//-----------------------------------------------------------------------------

// Function: Print_DrawChar
// Print a single character 
//
// Parameters:
//   chr - ASCII code of the character to draw
void Print_DrawChar(u8 chr);

// Function: Print_DrawCharX
// Print the same character many times
//
// Parameters:
//   chr - ASCII code of the character to draw
//   num - Number of the same character to display in a row
void Print_DrawCharX(c8 chr, u8 num);

// Function: Print_DrawText
// Print a character string
//
// Parameters:
//   string - Null-terminated string to draw
void Print_DrawText(const c8* string);

// Function: Print_DrawHex8
// Print a 8-bits binary value 
//
// Parameters:
//   value - 8 bits integer to draw in hexadecimal form
void Print_DrawHex8(u8 value);

// Function: Print_DrawHex16
// Print a 16-bits hexadecimal value
//
// Parameters:
//   value - 16 bits integer to draw in hexadecimal form
void Print_DrawHex16(u16 value);

// Function: Print_DrawBin8
// Print a 8-bits binary value
//
// Parameters:
//   value - 8 bits integer to draw in binary form (0 or 1)
void Print_DrawBin8(u8 value);

#if (PRINT_USE_32B)
// Function: Print_DrawHex32
// Print a 32-bits hexadecimal value
// This function requires PRINT_USE_32B compile option to be set to TRUE.
//
// Parameters:
//   value - 32 bits integer to draw in hexadecimal form
void Print_DrawHex32(u32 value);

// Function: Print_DrawInt
// Print a 32-bits signed decimal value.
// This function requires PRINT_USE_32B compile option to be set to TRUE.
//
// Parameters:
//   value - 32 bits signed integer to draw
void Print_DrawInt(i32 value);
#else
// Function: Print_DrawInt
// Print a 16-bits signed decimal value.
// This function requires PRINT_USE_32B compile option to be set to FALSE.
//
// Parameters:
//   value - 16 bits signed integer to draw
void Print_DrawInt(i16 value);
#endif

// Function: Print_Space
// Print space.
inline void Print_Space()
{
	g_PrintData.CursorX += PRINT_W(g_PrintData.UnitX);
}

// Function: Print_Tab
// Print tabulation.
inline void Print_Tab()
{
	g_PrintData.CursorX += PRINT_W(g_PrintData.UnitX) + g_PrintData.TabSize - 1;
	g_PrintData.CursorX &= ~(g_PrintData.TabSize - 1);
}

// Function: Print_Return
// Print return.
inline void Print_Return()
{
	g_PrintData.CursorX = 0;
	g_PrintData.CursorY += PRINT_H(g_PrintData.UnitY);
}

//.............................................................................
// Format
//.............................................................................
#if (PRINT_USE_FORMAT)
// Function: Print_DrawFormat
// Print a formated string with a variable number of parameters.
// This function requires PRINT_USE_FORMAT compile option to be set to TRUE.
//
// Parameters:
//   format - Printf style format
// > %i or %d                  16 bits signed integer
// > %u                        16 bits unsigned integer
// > %[N]x                     16 bits hexadecimal with N digits (default: 4)
// > %c                        ASCII coded character
// > %s                        Null-terminated string
// > %I or %D                  32 bits signed integer
// > %U                        32 bits unsigned integer
// > %[N]X                     32 bits hexadecimal with N digits (default: 4)
// > \t                        Tabulation
// > \n                        Return to new line
//   ... - Variadic parameters
void Print_DrawFormat(const c8* format, ...);

#endif

//-----------------------------------------------------------------------------
// Group: Helper
//
//-----------------------------------------------------------------------------

// Function: Print_Clear
// Clear screen
void Print_Clear();

// Function: Print_Backspace
// Clear X character back from current cursor position.
//
// Parameters:
//   num - Number of character to clear
void Print_Backspace(u8 num);

// Function: Print_DrawTextAt
// Draw text at a given position on screen.
//
// Parameters:
//   x   - Position X coordinate
//   y   - Position Y coordinate
//   str - Null-terminated string to draw
inline void Print_DrawTextAt(u8 x, u8 y, const c8* str)
{
	Print_SetPosition(x, y);
	Print_DrawText(str);
}

// Function: Print_DrawTextAtV
// Draw a vertical text at a given position on screen.
//
// Parameters:
//   x   - Position X coordinate
//   y   - Position Y coordinate
//   str - Null-terminated string to draw
inline void Print_DrawTextAtV(u8 x, u8 y, const c8* str)
{
	while(*str)
	{
		Print_SetPosition(x, y++);
		Print_DrawChar(*str++);
	}
}

// Function: Print_DrawCharAt
// Print a single character at a given position.
//
// Parameters:
//   x   - Position X coordinate
//   y   - Position Y coordinate
//   chr - Character to draw
inline void Print_DrawCharAt(u8 x, u8 y, c8 chr)
{
	Print_SetPosition(x, y);
	Print_DrawChar(chr);
}

// Function: Print_DrawCharXAt
// Print the same character many times at a given position.
//
// Parameters:
//   x   - Position X coordinate
//   y   - Position Y coordinate
//   chr - Character to draw
//   len - Number of character to draw
inline void Print_DrawCharXAt(u8 x, u8 y, c8 chr, u8 len)
{
	Print_SetPosition(x, y);
	Print_DrawCharX(chr, len);
}

// Function: Print_DrawCharYAt
// Print vertically the same character many times at a given position.
//
// Parameters:
//   x   - Position X coordinate
//   y   - Position Y coordinate
//   chr - Character to draw
//   len - Number of character to draw
inline void Print_DrawCharYAt(u8 x, u8 y, c8 chr, u8 len)
{
	for(u8 i = 0; i < len; ++i)
		Print_DrawCharAt(x, y++, chr);
}

// Function: Print_DrawIntAt
// Print a 16-bits signed decimal value at a given position.
//
// Parameters:
//   x   - Position X coordinate
//   y   - Position Y coordinate
//   val - 16 bits integer to display
inline void Print_DrawIntAt(u8 x, u8 y, i16 val)
{
	Print_SetPosition(x, y);
	Print_DrawInt(val);
}

//-----------------------------------------------------------------------------
// Group: FX
//
//-----------------------------------------------------------------------------

//.............................................................................
// Color
//.............................................................................

// Function: Print_SetColor
// Set the draw color.
//
// Parameters:
//   text - Text color (16 color index or 8 bits RGB)
//   bg   - Background color (16 color index or 8 bits RGB)
void Print_SetColor(u8 text, u8 bg);

#if (PRINT_COLOR_NUM > 1)
// Function: Print_SetColorShade
// Set colors shade.
// This function requires PRINT_COLOR_NUM compile option to be set to greater than 1 value.
//
// Parameters:
//   shade - Pointer to font color (1 color for each line)
void Print_SetColorShade(const u8* shade);
#endif

//.............................................................................
// Shadow
//.............................................................................

#if (PRINT_USE_FX_SHADOW)
// Function: Print_SetShadow
// Set shadow effect.
// This function requires PRINT_USE_FX_SHADOW compile option to be set to TRUE.
//
// Parameters:
//   offsetX - Offset on X coordinate (can be negative).
//   offsetY - Offset on Y coordinate (can be negative).
//   color   - Shadow's color.
void Print_SetShadow(bool enable, i8 offsetX, i8 offsetY, u8 color);

// Function: Print_EnableShadow
// Activate/desactivate shadow effect.
// This function requires PRINT_USE_FX_SHADOW compile option to be set to TRUE.
//
// Parameters:
//   enable - TRUE to enable shadow effect, FALSE to disable.
void Print_EnableShadow(bool enable);

#if (PRINT_USE_2_PASS_FX)
// Function: Print_DrawTextShadow
// Print a character string with shadow FX using 2 pass rendering (to have clean shadow over all characters).
// This function requires PRINT_USE_2_PASS_FX and PRINT_USE_FX_SHADOW compile options to be set to TRUE.
//
// Parameters:
//   string  - Pointer to the null-terminated string to display.
//   offsetX - Shadow position offset (X coordinate).
//   offsetY - Shadow position offset (X coordinate).
//   color   - Shadow color.
void Print_DrawTextShadow(const c8* string, i8 offsetX, i8 offsetY, u8 color);
#endif // (PRINT_USE_2_PASS_FX)
#endif // (PRINT_USE_FX_SHADOW)

//.............................................................................
// Outline
//.............................................................................

#if (PRINT_USE_FX_OUTLINE)
// Function: Print_SetOutline
// Set shadow effect.
// This function requires PRINT_USE_FX_OUTLINE compile option to be set to TRUE.
//
// Parameters:
//   enable - TRUE to enable outline effect, FALSE to disable.
//   color  - Color of the outline effect
void Print_SetOutline(bool enable, u8 color);

// Function: Print_EnableOutline
// Activate/desactivate shadow effect
// This function requires PRINT_USE_FX_OUTLINE compile option to be set to TRUE.
//
// Parameters:
//   enable - TRUE to enable outline effect, FALSE to disable.
void Print_EnableOutline(bool enable);

#if (PRINT_USE_2_PASS_FX)
// Function: Print_DrawTextOutline
// Print a character string using outline FX.
// This function requires PRINT_USE_2_PASS_FX and PRINT_USE_FX_OUTLINE compile options to be set to TRUE.
//
// Parameters:
//   string  - Pointer to the null-terminated string to display
//   color   - Outline color
void Print_DrawTextOutline(const c8* string, u8 color);
#endif // (PRINT_USE_2_PASS_FX)
#endif // (PRINT_USE_FX_OUTLINE)

//-----------------------------------------------------------------------------
// Group: Graph
// Boxes draw functions
//-----------------------------------------------------------------------------
#if (PRINT_USE_GRAPH)

// Function: Print_DrawLineH
// Draw an horizontal line using characters
// This function requires PRINT_USE_GRAPH compile option to be set to TRUE.
//
// Parameters:
//   x   - Start position X coordinate
//   y   - Start position Y coordinate
//   len - Lenght of the horizontal line (in print mode unit)
void Print_DrawLineH(u8 x, u8 y, u8 len);

// Function: Print_DrawLineV
// Draw a vertical line using characters
// This function requires PRINT_USE_GRAPH compile option to be set to TRUE.
//
// Parameters:
//   x   - Start position X coordinate
//   y   - Start position Y coordinate
//   len - Lenght of the vertical line (in print mode unit)
void Print_DrawLineV(u8 x, u8 y, u8 len);

// Function: Print_DrawBox
// Draw a box using characters
// This function requires PRINT_USE_GRAPH compile option to be set to TRUE.
//
// Parameters:
//   x      - Start position X coordinate
//   y      - Start position Y coordinate
//   width  - Box width (in print mode unit)
//   height - Box height (in print mode unit)
void Print_DrawBox(u8 x, u8 y, u8 width, u8 height);

#endif // (PRINT_USE_GRAPH)