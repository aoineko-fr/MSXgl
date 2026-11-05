//_____________________________________________________________________________
//   ▄ ▄  ▄▄▄ ▄▄ ▄ ▄                                                      
//  ██▀█ ▀█▄  ▀█▄▀ ▄  ▄█▄█ ▄▀██                                           
//  ██ █ ▄▄█▀ ██ █ ██ ██ █  ▀██                                           
//__________________________▀▀_________________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/MSXimg)
// under CC BY-SA license (https://creativecommons.org/licenses/by-sa/2.0/)
#pragma once

// FreeImage
#include "FreeImage.h"
// MSXtk
#include "MSXtk.h"

// Default palettes
extern u32 PaletteMSX1[16];
extern u32 PaletteMSX2[16];

// Color mode
enum ColorMode
{
	COLORMODE_P1,     // 1-bit palette (2 colors)                     [MSX1]
	COLORMODE_P2,     // 2-bit palette (4 colors)                     [MSX2]  [V9990]
	COLORMODE_P4,     // 4-bit palette (16 colors)                    [MSX1]  [V9990]
	COLORMODE_P6,     // 6-bit palette (64 colors)                            [V9990]
	COLORMODE_RGB8,   // 8-bit RGB (256 colors)                       [MSX2]  [V9990]
	COLORMODE_RGB16,  // 16-bit RGB (32768 colors)                            [V9990]
	COLORMODE_YJK,    // 8-bit YJK decoder (19268 colors)             [MSX2+] [V9990]
	COLORMODE_YJKP,   // 8-bit YJK Decoder + palette (12615 colors)   [MSX2+] [V9990]
	COLORMODE_YUV,    // 8-bit YUV decoder (19268 colors)                     [V9990]
	COLORMODE_YUVP,   // 8-bit YUV Decoder + palette (12615 colors)           [V9990]
};

// Get color mode name
inline const c8* GetColorModeName(ColorMode mode)
{
	switch (mode)
	{
		case COLORMODE_P1:      return "P1 (1-bit black & white)";
		case COLORMODE_P2:      return "P2 (2-bit palette)";
		case COLORMODE_P4:      return "P4 (4-bit palette)";
		case COLORMODE_P6:      return "P6 (6-bit palette)";
		case COLORMODE_RGB8:    return "RGB8 (8-bit GRB)";
		case COLORMODE_RGB16:   return "RGB16 (16-bit GRB)";
		case COLORMODE_YJK:     return "YJK";
		case COLORMODE_YJKP:	return "YJK + palette";
		case COLORMODE_YUV:     return "YUV";
		case COLORMODE_YUVP:	return "YUV + palette";
		default:                break;
	};
	return "Unknown";
}

// Check if color mode uses a color palette
inline bool IsPaletteColorMode(ColorMode mode)
{
	return (mode == COLORMODE_P2) || (mode == COLORMODE_P4) || (mode == COLORMODE_P6) || (mode == COLORMODE_YJKP) || (mode == COLORMODE_YUVP);
}

// Check if color mode uses YJK/YUV encoding (4 pixels share the same chrominance)
inline bool IsYJKColorMode(ColorMode mode)
{
	return (mode == COLORMODE_YJK) || (mode == COLORMODE_YJKP) || (mode == COLORMODE_YUV) || (mode == COLORMODE_YUVP);
}

// Check if color mode is only available on V9990
inline bool IsV9990ColorMode(ColorMode mode)
{
	return (mode == COLORMODE_P6) || (mode == COLORMODE_RGB16) || (mode == COLORMODE_YUV) || (mode == COLORMODE_YUVP);
}

// Get the number of entries of the color palette for the given color mode
inline i32 GetColorModePaletteSize(ColorMode mode)
{
	switch (mode)
	{
		case COLORMODE_P2:      return 4;
		case COLORMODE_P4:      return 16;
		case COLORMODE_P6:      return 64;
		case COLORMODE_YJKP:	return 16;
		case COLORMODE_YUVP:	return 16;
		default:                return 0;
	};
}

// Palette type
enum PaletteType
{
	PALETTE_None,
	PALETTE_MSX1,    // Default MSX1 palette
	PALETTE_MSX2,    // Default MSX2 palette
	PALETTE_Custom,  // Generate a custom palette 
	PALETTE_Input,   // Use input palette 
	PALETTE_Partial, // Combine partial input palette and custom palette
};

// Get palette type name
inline const c8* GetPaletteTypeName(PaletteType pal)
{
	switch (pal)
	{
		case PALETTE_MSX1:    // Default MSX1 palette
			return "MSX1";
		case PALETTE_MSX2:    // Default MSX2 palette
			return "MSX2";
		case PALETTE_Custom:  // Generate a custom palette 
			return "Custom";
		case PALETTE_Input:   // Use input palette 
			return "Input";
		case PALETTE_Partial: // Combine partial input palette and custom palette
			return "Partial";
		default:
			break;
	};
	return "None";
}

// Ditherig method
enum DitheringMethod
{
	DITHER_None      = -1,
	DITHER_Floyd     = FID_FS,           // Floyd & Steinberg error diffusion algorithm
	DITHER_Bayer4    = FID_BAYER4x4,     // Bayer ordered dispersed dot dithering(order 2 – 4x4 - dithering matrix)
	DITHER_Bayer8    = FID_BAYER8x8,     // Bayer ordered dispersed dot dithering(order 3 – 8x8 - dithering matrix)
	DITHER_Bayer16   = FID_BAYER16x16,   // Bayer ordered dispersed dot dithering(order 4 – 16x16 dithering matrix)
	DITHER_Cluster6  = FID_CLUSTER6x6,   // Ordered clustered dot dithering(order 3 - 6x6 matrix)
	DITHER_Cluster8  = FID_CLUSTER8x8,   // Ordered clustered dot dithering(order 4 - 8x8 matrix)
	DITHER_Cluster16 = FID_CLUSTER16x16, // Ordered clustered dot dithering(order 8 - 16x16 matrix)
};

// Filter method
enum FilterMethod
{
	FILTER_None     = -1,
	FILTER_Box      = FILTER_BOX,        // Box, pulse, Fourier window, 1st order (constant) B-Spline
	FILTER_Bilinear = FILTER_BILINEAR,   // Bilinear filter
	FILTER_BSpline  = FILTER_BSPLINE,    // 4th order(cubic) B - Spline
	FILTER_Bicubic  = FILTER_BICUBIC,    // Mitchell and Netravali's two-param cubic filter
	FILTER_Catmull  = FILTER_CATMULLROM, // Catmull - Rom spline, Overhauser spline
	FILTER_Lanczos  = FILTER_LANCZOS3,   // Lanczos - windowed sinc filter
};

// color types
struct GRB8;
struct RGB24;

/** G3 R3 B2 color type */
struct GRB8
{
	u8 RGB;

	GRB8(): RGB(0) {}
	GRB8(u8 a): RGB(a) {}
	GRB8(RGB24 color);

	operator u8() { return RGB; }
};

/** G5 R5 B5 color type */
struct GRB16
{
	u16 RGB;

	GRB16() : RGB(0) {}
	GRB16(u16 a) : RGB(a) {}
	GRB16(RGB24 color);

	operator u16() { return RGB; }
};

/** R8 G8 B8 color type */
struct RGB24
{
	u8 R, G, B;

	RGB24(): R(0), G(0), B(0) {}
	RGB24(u8 a, u8 b, u8 c): R(a), G(b), B(c) {}
	RGB24(u32 RGBA)
	{
		R = (RGBA >> 16) & 0xFF;
		G = (RGBA >> 8) & 0xFF;
		B = (RGBA >> 0) & 0xFF;
	}
	RGB24(GRB8 color);

	static RGB24 Blend(RGB24 c1, RGB24 c2)
	{
		RGB24 ret;
		ret.R = (u8)(((u16)c1.R + c2.R) / 2);
		ret.G = (u8)(((u16)c1.G + c2.G) / 2);
		ret.B = (u8)(((u16)c1.B + c2.B) / 2);
		return ret;
	}
};