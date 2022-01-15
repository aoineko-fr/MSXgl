// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄       ▄       ▄▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██▄▀ ▄  ██▀▄ ██▀ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ██   ██ ██ █ ▀█▄ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#pragma once
#include "core.h"
#include "vdp.h"

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

// Character display sources
enum PRINT_MODE
{
	// Bitmap modes (from RAM)
#if ((USE_PRINT_BITMAP) || (USE_PRINT_VRAM))
	PRINT_MODE_BITMAP		= 0,	//< Draw characters from RAM (R-T unpack font data and draw it)
	PRINT_MODE_BITMAP_TRANS	= 1,	//< Draw characters from RAM with transparency (R-T unpack font data and draw it)
#endif
	// Bitmap mode (from VRAM)
#if (USE_PRINT_VRAM)
	PRINT_MODE_BITMAP_VRAM	= 2,	//< Draw characters from VRAM (font data is upack once in VRAM thne drawing is done by VRAM copy)
#endif
	// Sprite mode
#if (USE_PRINT_SPRITE)
	PRINT_MODE_SPRITE		= 3,	//< Draw characters from sprites (load font data as sprite pattern in VRAM then display characters using sprite system)
#endif
	// Text mode
#if (USE_PRINT_TEXT)
	PRINT_MODE_TEXT			= 4,	//< Draw characters as pattern names (text mode)
#endif
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

#define PRINT_TAB_SIZE		24
#define PRINT_DEFAULT_FONT	null

// Functions
typedef void (*print_drawchar)(u8); //< Draw char callback signature
typedef void (*print_loadfont)(VADDR); //< Font load callback signature

extern struct Print_Data g_PrintData;

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
// #if (USE_PRINT_)
	const u8* FontPatterns;		//< Forms of the font
	const u8* FontAddr;			//< Address of the virtual index 0 character (used to quick drawing in DrawChar_GX functions)
// #endif
#if (USE_PRINT_VRAM)
	// Bitmap from VRAM
	UY FontVRAMY;				//< Y position of the font in VRAM
	u8 CharPerLine;
#endif
#if (USE_PRINT_SPRITE)
	// Sprites
	u8 SpritePattern;			//< Pattern index of the 1st sprite character
	u8 SpriteID;				//< Index of the current sprite
#endif
#if (USE_PRINT_TEXT)
	// Text mode
	u8 PatternOffset;
#endif
#if (USE_PRINT_FX_SHADOW)
	u8 Shadow			: 1;	//< Is shadow render active
	u8 ShadowOffsetX	: 3;	//< Shadow X offset (0:7 => -3:+4)
	u8 ShadowOffsetY	: 3;	//< Shadow Y offset (0:7 => -3:+4)
	u8 ShadowColor;				//< Shadow color
#endif
#if (USE_PRINT_FX_OUTLINE)
	u8 Outline			: 1;	//< Is shadow render active
	u8 OutlineColor;			//< Shadow color
#endif
	u8 Buffer[16];				//< Mode specifique buffer (used to pre-compute color combinations)
};

//-----------------------------------------------------------------------------
// Group: Initialization
// Font initialization functions
//-----------------------------------------------------------------------------

// Function: Print_Initialize
// Initialize print module. Must be called after VDP_SetMode()
bool Print_Initialize();

// Function: Print_SetMode
// Change current print mode
void Print_SetMode(u8 src);

// Function: Print_SetFont
// Set the current font (and set mode to RAM)
void Print_SetFont(const u8* font);

#if (USE_PRINT_BITMAP)
// Function: Print_SetBitmapFont
// Initialize print module and set a font in RAM
bool Print_SetBitmapFont(const u8* font);
#endif

#if (USE_PRINT_VRAM)
// Function: Print_SetVRAMFont
// Set the current font and upload data to VRAM 
void Print_SetVRAMFont(const u8* font, UY y, u8 color);
#endif

#if (USE_PRINT_TEXT)
// Function: Print_SetTextFont
// Initialize print module and set a font in RAM
void Print_SetTextFont(const u8* font, u8 offset);
#endif

#if (USE_PRINT_SPRITE)
// Function: Print_SetSpriteFont
// Set the current font and upload to Sprite Pattern Table
void Print_SetSpriteFont(const u8* font, u8 patIdx, u8 sprtIdx);

// Function: Print_GetSpritePattern
// Get pattern index of the 1st sprite character
inline u8 Print_GetSpritePattern() { return g_PrintData.SpritePattern; }

// Function: Print_GetSpriteID
// Get the next sprite index
inline u8 Print_GetSpriteID() { return g_PrintData.SpriteID; }

// Function: Print_SetSpriteID
// Set the next sprite index
inline void Print_SetSpriteID(u8 id) { g_PrintData.SpriteID = id; }

#endif

//-----------------------------------------------------------------------------
// Group: Setting
//
//-----------------------------------------------------------------------------

// Function: Print_Clear
// Clear screen
void Print_Clear();

// Function: Print_Backspace
// Clear X character back from current cursor position
void Print_Backspace(u8 num);

// Function: Print_SetColor
// Set the draw color
void Print_SetColor(u8 text, u8 bg);

#if (PRINT_COLOR_NUM > 1)
// Function: Print_SetColorShade
// Set color shade
void Print_SetColorShade(const u8* shade);
#endif

#if (USE_PRINT_FX_SHADOW)	
// Function: Print_SetShadow
// Set shadow effect
void Print_SetShadow(bool enable, i8 offsetX, i8 offsetY, u8 color);

// Function: Print_EnableShadow
// Activate/desactivate shadow effect
void Print_EnableShadow(bool enable);
#endif

#if (USE_PRINT_FX_OUTLINE)	
// Function: Print_SetOutline
// Set shadow effect
void Print_SetOutline(bool enable, u8 color);

// Function: Print_EnableOutline
// Activate/desactivate shadow effect
void Print_EnableOutline(bool enable);
#endif

//-----------------------------------------------------------------------------
// Group: Draw
// Text draw functions
//-----------------------------------------------------------------------------

// Function: Print_DrawChar
// Print a single character 
void Print_DrawChar(u8 chr);

// Function: Print_DrawCharX
// Print the same character many times
void Print_DrawCharX(c8 chr, u8 num);

// Function: Print_DrawText
// Print a character string
void Print_DrawText(const c8* string);

// Function: Print_DrawHex8
// Print a 8-bits binary value 
void Print_DrawHex8(u8 value);

// Function: Print_DrawHex16
// Print a 16-bits hexadecimal value
void Print_DrawHex16(u16 value);

// Function: Print_DrawBin8
// Print a 8-bits binary value
void Print_DrawBin8(u8 value);

#if (USE_PRINT_32B)
// Function: Print_DrawHex32
// Print a 32-bits hexadecimal value
void Print_DrawHex32(u32 value);

// Function: Print_DrawInt
// Print a 16/32-bits signed decimal value (depends on USE_PRINT_32B flag)
void Print_DrawInt(i32 value);

#else
// Print a 16-bits signed decimal value
void Print_DrawInt(i16 value);

#endif

//-----------------------------------------------------------------------------
#if (USE_PRINT_FORMAT)
// Function: Print_DrawFormat
// Print a formated string with a variable number of parameters
void Print_DrawFormat(const c8* format, ...);
#endif

//-----------------------------------------------------------------------------
// Group: Graph
// Boxes draw functions
//-----------------------------------------------------------------------------
#if (USE_PRINT_GRAPH)

// Function: Print_DrawLineH
// Draw an horizontal line using characters
void Print_DrawLineH(u8 x, u8 y, u8 len);

// Function: Print_DrawLineV
// Draw a vertical line using characters
void Print_DrawLineV(u8 x, u8 y, u8 len);

// Function: Print_DrawBox
// Draw a box using characters
void Print_DrawBox(u8 x, u8 y, u8 width, u8 height);

#endif // (USE_PRINT_GRAPH)

//-----------------------------------------------------------------------------
// INLINE FUNCTIONS
//-----------------------------------------------------------------------------

// inline void Print_SetFontEx(u8 patternX, u8 patternY, u8 sizeX, u8 sizeY, u8 firstChr, u8 lastChr, const u8* patterns); // Set the current font
// inline void Print_SetPosition(u8 x, u8 y); // Set cursor position
// inline void Print_SetCharSize(u8 x, u8 y); // Set cursor position
// inline void Print_SetTabSize(u8 size) // Set tabulation size in pixel (must be a power of 2 like 16, 32, 64, ...);
// inline void Print_Space(); // Print space
// inline void Print_Tab(); // Print tabulation
// inline void Print_Return(); // Print return
// inline void Print_DrawTextAt(u8 x, u8 y, const c8* string); // Print a character string
#include "print.inl"
