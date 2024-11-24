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

#define MSXi_VERSION "1.20.1"

/// Header structure
struct MSXi_Header
{
	unsigned short sizeX;			///< Width of each image
	unsigned short sizeY;			///< Height of each image
	unsigned short numX;			///< Number of columns of images
	unsigned short numY;			///< Number of rows of images
	unsigned char bpc;				///< Bits-per-colors
	unsigned char comp;				///< Compressor ID @see MSXi_Compressor
	unsigned char bSkipEmpty;		///< Tell is empty blocks have been skipped or not
};

/// Font structure
struct MSXi_Font
{
	unsigned char dataSize;			///< Data size [x|y]
	unsigned char fontSize;			///< Font size [x|y]
	unsigned char firstChr;			///< First character ASCII code
	unsigned char lastChe;			///< Last character ASCII code
};

/// No entry flag
#define MSXi_NO_ENTRY	0x8000