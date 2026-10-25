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

#define MSXi_VERSION "1.23.0"

/// Header structure
struct MSXi_Header
{
	unsigned short SizeX;			///< Width of each image
	unsigned short SizeY;			///< Height of each image
	unsigned short NumX;			///< Number of columns of images
	unsigned short NumY;			///< Number of rows of images
	unsigned char BPC;				///< Bits-per-colors
	unsigned char Comp;				///< Compressor ID @see MSXi_Compressor
	unsigned char bSkipEmpty;		///< Tell is empty blocks have been skipped or not
};

/// Font structure
struct MSXi_Font
{
	unsigned char DataSize;			///< Data size [x|y]
	unsigned char FontSize;			///< Font size [x|y]
	unsigned char FirstChar;		///< First character ASCII code
	unsigned char LastChar;			///< Last character ASCII code
};

/// No entry flag
#define MSXi_NO_ENTRY	0x8000