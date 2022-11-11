// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄       ▄       ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██▄▀ ▄  ██▀▄ ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ██   ██ ██ █ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//-----------------------------------------------------------------------------
// Function: Print_SetFontEx
// Set the current font
//
// Parameters:
//   patternX	- Width of the character data (should be always 8)
//   patternY	- Height of the character data
//   sizeX		- Width of the chatacter to display (can be different than the one of the data)
//   sizeY		- Height of the chatacter to display (can be different than the one of the data)
//   firstChr	- ASCII code of the first character included in the font data
//   lastChr	- ASCII code of the last character included in the font data
//   patterns	- Address of the character data table
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

#if (PRINT_USE_TEXT)
//-----------------------------------------------------------------------------
// Function: Print_SelectTextFont
// Set cursor position
inline void Print_SelectTextFont(const u8* font, u8 offset)
{
	g_PrintData.PatternOffset = offset;
	Print_SetFontEx(8, 8, 1, 1, font[2], font[3], font+4);
	// Print_Initialize();
	// Print_SetMode(PRINT_MODE_TEXT);
}
#endif

//-----------------------------------------------------------------------------
// Function: Print_SetPosition
// Set cursor position
inline void Print_SetPosition(UX x, UY y)
{
	g_PrintData.CursorX = x;
	g_PrintData.CursorY = y;
}

//-----------------------------------------------------------------------------
// Function: Print_SetCharSize
// Set cursor position
inline void Print_SetCharSize(u8 x, u8 y)
{
	g_PrintData.UnitX = x;
	g_PrintData.UnitY = y;
}

//-----------------------------------------------------------------------------
// Function: Print_SetTabSize
// Set tabulation size in pixel
// @param		size		Size of the tabulation in pixel (must be a power of 2 like 16, 32, 64, ...)
inline void Print_SetTabSize(u8 size)
{
	g_PrintData.TabSize = size;
}

//-----------------------------------------------------------------------------
// Function: Print_Space
// Print space
inline void Print_Space()
{
	g_PrintData.CursorX += PRINT_W(g_PrintData.UnitX);
}

//-----------------------------------------------------------------------------
// Function: Print_Tab
// Print tabulation
inline void Print_Tab()
{
	g_PrintData.CursorX += PRINT_W(g_PrintData.UnitX) + g_PrintData.TabSize - 1;
	g_PrintData.CursorX &= ~(g_PrintData.TabSize - 1);
}

//-----------------------------------------------------------------------------
// Function: Print_Return
// Print return
inline void Print_Return()
{
	g_PrintData.CursorX = 0;
	g_PrintData.CursorY += PRINT_H(g_PrintData.UnitY);
}

//-----------------------------------------------------------------------------
// Function: Print_GetFontInfo
// Get first character code
inline const struct Print_Data* Print_GetFontInfo()
{
	return &g_PrintData;
}

//-----------------------------------------------------------------------------
// Function: Print_DrawTextAt
// Draw text at a given position on screen
inline void Print_DrawTextAt(u8 x, u8 y, const c8* string)
{
	Print_SetPosition(x, y);
	Print_DrawText(string);
}

//-----------------------------------------------------------------------------
// Function: Print_DrawTextAtV
// Draw a vertical text at a given position on screen
inline void Print_DrawTextAtV(u8 x, u8 y, const c8* str)
{
	while(*str)
	{
		Print_SetPosition(x, y++);
		Print_DrawChar(*str++);
	}
}

//-----------------------------------------------------------------------------
// Function: Print_DrawCharAt()
// Print a single character at a given position.
inline void Print_DrawCharAt(u8 x, u8 y, c8 chr)
{
	Print_SetPosition(x, y);
	Print_DrawChar(chr);
}

//-----------------------------------------------------------------------------
// Function: Print_DrawCharXAt
// Print the same character many times at a given position.
inline void Print_DrawCharXAt(u8 x, u8 y, c8 chr, u8 len)
{
	Print_SetPosition(x, y);
	Print_DrawCharX(chr, len);
}

//-----------------------------------------------------------------------------
// Function: Print_DrawCharYAt
// Print the same character many times at a given position.
inline void Print_DrawCharYAt(u8 x, u8 y, c8 chr, u8 len)
{
	for(u8 i = 0; i < len; ++i)
		Print_DrawCharAt(x, y++, chr);
}

//-----------------------------------------------------------------------------
// Function: Print_DrawIntAt()
// Print a 16-bits signed decimal value at a given position.
inline void Print_DrawIntAt(u8 x, u8 y, i16 val)
{
	Print_SetPosition(x, y);
	Print_DrawInt(val);
}
