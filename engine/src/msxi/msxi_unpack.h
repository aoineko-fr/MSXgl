//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄ ▄                                                      
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀ ▄  ▄█▄█ ▄▀██                                           
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █ ██ ██ █  ▀██                                           
//_______________________________▀▀____________________________________________
// MSXi UPACK FUNCTIONS
//
// References:
// - https://github.com/aoineko-fr/MSXImage
//-----------------------------------------------------------------------------
#pragma once

#include "msxi/msxi.h"

// Unpack destination enum
enum MSXi_Destination
{
	DEST_RAM = 0,
	DEST_VRAM,
};

// Unpack config structure
typedef struct
{
	void*	Source;
	u16		DestX;
	u16		DestY;
	u8		SizeX;
	u8		SizeY;
	u8		NumX;
	u8		NumY; 
	u8		Compressor;			//< The decompressor to use. @see MSXi_Compressor
	u8  	Destination : 1;	//< Destination 
	u8		SourceBPC   : 2;	//< Source bits-per-colors (0 = 1-bit, 1 = 2-bits, 2 = 4-bits, 3 = 8-bits)
	u8		DestBPC     : 2;	//< Destination bits-per-colors (0 = 1-bit, 1 = 2-bits, 2 = 4-bits, 3 = 8-bits)
	u8		_unused1    : 3;	
	u8*		ColorReplace;		//< Color replacement (format = { num, c1_from, c1_to, c2_from, c2_to, ... })
} MSXi_UnpackParam;


#define  MSXi_UnpackToVRAM(src, destX, destY, sizeX, sizeY, numX, numY, clrReplace, compressor, srcBPC, destBPC) MSXi_UnpackToVRAM_##compressor##_##srcBPC##_##destBPC(src, destX, destY, sizeX, sizeY, numX, numY, clrReplace)

#if MSXi_USE_COMP_NONE
// Function: MSXi_UnpackToVRAM_None_4_4
// Uncompress 16 colors NONE data to VRAM
void MSXi_UnpackToVRAM_None_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace);
#endif

#if MSXi_USE_COMP_CROP16		
// Function: MSXi_UnpackToVRAM_Crop16_4_4
// Uncompress 16 colors CROP16 data to VRAM
void MSXi_UnpackToVRAM_Crop16_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace);
#endif

#if MSXi_USE_COMP_CROP32		
// Function: MSXi_UnpackToVRAM_Crop32_4_4
// Uncompress 16 colors CROP32 data to VRAM
void MSXi_UnpackToVRAM_Crop32_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace);
#endif

#if MSXi_USE_COMP_CROP256		
// Function: MSXi_UnpackToVRAM_Crop256_4_4
// Uncompress 16 colors CROP256 data to VRAM
void MSXi_UnpackToVRAM_Crop256_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace);
#endif

#if MSXi_USE_COMP_CROPLINE16	
// Function: MSXi_UnpackToVRAM_CropLine16_4_4
// Uncompress 16 colors CROPLINE16 data to VRAM
void MSXi_UnpackToVRAM_CropLine16_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace);
#endif

#if MSXi_USE_COMP_CROPLINE32	
// Function: MSXi_UnpackToVRAM_CropLine32_4_4
// Uncompress 16 colors CROPLINE32 data to VRAM
void MSXi_UnpackToVRAM_CropLine32_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace);
#endif

#if MSXi_USE_COMP_CROPLINE256	
// Function: MSXi_UnpackToVRAM_CropLine256_4_4
// Uncompress 16 colors CROPLINE256	 data to VRAM
void MSXi_UnpackToVRAM_CropLine256_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace);
#endif

#if MSXi_USE_COMP_RLE0			
// Function: MSXi_UnpackToVRAM_RLE0_4_4
// Uncompress 16 colors RLE0 data to VRAM
void MSXi_UnpackToVRAM_RLE0_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace);
#endif

#if MSXi_USE_COMP_RLE4			
// Function: MSXi_UnpackToVRAM_RLE4_4_4
// Uncompress 16 colors RLE4 data to VRAM
void MSXi_UnpackToVRAM_RLE4_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace);
#endif

#if MSXi_USE_COMP_RLE8			
// Function: MSXi_UnpackToVRAM_RLE8_4_4
// Uncompress 16 colors RLE8 data to VRAM
void MSXi_UnpackToVRAM_RLE8_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace);
#endif
