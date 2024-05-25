//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄ ▄                                                      
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀ ▄  ▄█▄█ ▄▀██                                           
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █ ██ ██ █  ▀██                                           
//_______________________________▀▀____________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/MSXimg)
// under CC BY-SA license (https://creativecommons.org/licenses/by-sa/2.0/)

#pragma once

// FreeImage
#include "FreeImage.h"
// MSXtk
#include "MSXtk.h"

extern u32 PaletteMSX[16];

enum PaletteType
{
	PALETTE_None,
	PALETTE_MSX1,
	PALETTE_MSX2,
	PALETTE_Custom,  // Generate a custom palette 
	PALETTE_Input,   // Use input palette 
};

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
};