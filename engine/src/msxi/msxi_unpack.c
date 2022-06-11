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
#include "core.h"
#include "vdp.h"
#include "memory.h"
#include "msxi/msxi_unpack.h"

//-----------------------------------------------------------------------------
/// Search for color replacement. Replacement table must be in the format : { num, c1_from, c1_to, c2_from, c2_to, ... }
bool SearchColorReplacement(const u8* clrReplace, u8* color)
{
	if((clrReplace == NULL) || (clrReplace[0] == 0))
		return FALSE;
	for(u8 i = 0; i < clrReplace[0]; ++i)
	{
		if(*color == clrReplace[i * 2 + 1])
		{
			*color = clrReplace[i * 2 + 2];
			return TRUE;
		}
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
#if MSXi_USE_COMP_NONE			
void MSXi_UnpackToVRAM_None_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace) {}
#endif // MSXi_USE_COMP_NONE

//-----------------------------------------------------------------------------
#if MSXi_USE_COMP_CROP16		
void MSXi_UnpackToVRAM_Crop16_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace)
{
	clrReplace;
	u8* ptr = (u8*)src;
	u8 pageY = 0;
	if(destY >= 512)
	{
		destY -= 512;
		pageY = 1;
	}
	
	for(u8 j = 0; j < numY; ++j)
	{
		for(u8 i = 0; i < numX; ++i)
		{
			u8 minX = *ptr >> 4;
			//minX &= 0xFE;			
			u8 maxX = *ptr & 0x0F;
			//maxX |= 0x01;
			u8 len = (maxX - minX + 1);
			len /= 2;
			++ptr;
			u8 minY = *ptr >> 4;
			u8 maxY = *ptr & 0x0F;
			++ptr;
			for(u8 y = minY; y <= maxY; ++y)
			{
				if(clrReplace != NULL)
				{
					u8* buffer = Mem_HeapAlloc(len);
					Mem_Copy(ptr, buffer, len);
					for(u8 k = 0; k < len; ++k)
					{
						u8 c1 = buffer[k] >> 4;
						if(SearchColorReplacement(clrReplace, &c1))
						{
							buffer[k] &= 0x0F;
							buffer[k] |= (c1 & 0x0F) << 4;
						}

						u8 c2 = buffer[k] & 0x0F;
						if(SearchColorReplacement(clrReplace, &c2))
						{
							buffer[k] &= 0xF0;
							buffer[k] |= c2 & 0x0F;
						}
					}					
					VDP_WriteVRAM(buffer, ((destY + y + (j * sizeY)) * 128) + ((destX + (i * sizeX) + minX) >> 1), pageY, len);
					Mem_HeapFree(len);
				}
				else
				{
					VDP_WriteVRAM(ptr, ((destY + y + (j * sizeY)) * 128) + ((destX + (i * sizeX) + minX) >> 1), pageY, len);
				}
				ptr += len;
			}
		}
	}
}
#endif // MSXi_USE_COMP_CROP16

//-----------------------------------------------------------------------------
#if MSXi_USE_COMP_CROP32		
void MSXi_UnpackToVRAM_Crop32_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace) {}
#endif // MSXi_USE_COMP_CROP32

//-----------------------------------------------------------------------------
#if MSXi_USE_COMP_CROP256		
void MSXi_UnpackToVRAM_Crop256_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace) {}
#endif // MSXi_USE_COMP_CROP256

//-----------------------------------------------------------------------------
#if MSXi_USE_COMP_CROPLINE16	
void MSXi_UnpackToVRAM_CropLine16_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace)
{	
	clrReplace;
	u8* ptr = (u8*)src;
	u8 pageY = 0;
	if(destY >= 512)
	{
		destY -= 512;
		pageY = 1;
	}
	
	for(u8 j = 0; j < numY; ++j)
	{
		for(u8 i = 0; i < numX; ++i)
		{
			u8 minY = *ptr >> 4;
			u8 maxY = *ptr & 0x0F;
			ptr++;
			for(u8 y = minY; y <= maxY; ++y)
			{
				u8 minX = *ptr >> 4;
				//minX &= 0xFE;			
				u8 maxX = *ptr & 0x0F;
				//maxX |= 0x01;
				u8 len = (maxX - minX + 1);
				len /= 2;
				++ptr;
				if(clrReplace != NULL)
				{
					u8* buffer = Mem_HeapAlloc(len);
					Mem_Copy(ptr, buffer, len);
					for(u8 k = 0; k < len; ++k)
					{
						u8 c1 = buffer[k] >> 4;
						if(SearchColorReplacement(clrReplace, &c1))
						{
							buffer[k] &= 0x0F;
							buffer[k] |= (c1 & 0x0F) << 4;
						}

						u8 c2 = buffer[k] & 0x0F;
						if(SearchColorReplacement(clrReplace, &c2))
						{
							buffer[k] &= 0xF0;
							buffer[k] |= c2 & 0x0F;
						}
					}					
					VDP_WriteVRAM(buffer, ((destY + y + (j * sizeY)) * 128) + ((destX + (i * sizeX) + minX) >> 1), pageY, len);
					Mem_HeapFree(len);
				}
				else
				{
					VDP_WriteVRAM(ptr, ((destY + y + (j * sizeY)) * 128) + ((destX + (i * sizeX) + minX) >> 1), pageY, len);
				}
				ptr += len;
			}
		}
	}
}
#endif // MSXi_USE_COMP_CROPLINE16

//-----------------------------------------------------------------------------
#if MSXi_USE_COMP_CROPLINE32	
void MSXi_UnpackToVRAM_CropLine32_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace) {}
#endif // MSXi_USE_COMP_CROPLINE32

//-----------------------------------------------------------------------------
#if MSXi_USE_COMP_CROPLINE256	
void MSXi_UnpackToVRAM_CropLine256_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace) {}
#endif // MSXi_USE_COMP_CROPLINE256

//-----------------------------------------------------------------------------
#if MSXi_USE_COMP_RLE0			
void MSXi_UnpackToVRAM_RLE0_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace) {}
#endif // MSXi_USE_COMP_RLE0

//-----------------------------------------------------------------------------
#if MSXi_USE_COMP_RLE4			
void MSXi_UnpackToVRAM_RLE4_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace) {}
#endif // MSXi_USE_COMP_RLE4

//-----------------------------------------------------------------------------
#if MSXi_USE_COMP_RLE8			
void MSXi_UnpackToVRAM_RLE8_4_4(void* src, u16 destX, u16 destY, u8 sizeX, u8 sizeY, u8 numX, u8 numY, u8* clrReplace) {}
#endif // MSXi_USE_COMP_RLE8
