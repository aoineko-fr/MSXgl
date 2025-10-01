//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄ ▄                                                      
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀ ▄  ▄█▄█ ▄▀██                                           
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █ ██ ██ █  ▀██                                           
//_______________________________▀▀____________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/MSXimg)
// under CC-BY-AS license (https://creativecommons.org/licenses/by-sa/2.0/)

// std
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <assert.h>
// FreeImage
#include "FreeImage.h"
// MSXi
#include "color.h"
#include "exporter.h"
#include "image.h"
#include "parser.h"
#include "MGLV.h"

struct RLEHash
{
	i32 length;
	u32 color;
	std::vector<u32> data;

	RLEHash() : length(0), color(0){}
};

//-----------------------------------------------------------------------------
// MSX interface
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Get nearest color index
u8 GetNearestColorIndex(u32 color, u32* pal, i32 count, i32 offset)
{
	u8 bestIndex = 0;
	i32 bestWeight = 256 * 4;

	RGB24 c = RGB24(color);

	for (i32 i = offset; i <= count + offset; i++)
	{
		RGB24 p = RGB24(pal[i]);

		i32 weight = abs(p.R - c.R) + abs(p.G - c.G) + abs(p.B - c.B);
		if (weight < bestWeight)
		{
			bestWeight = weight;
			bestIndex = (u8)i;
		}
	}

	return bestIndex;
}

//-----------------------------------------------------------------------------
// Get 8-bit GRB color
u8 GetGBR8(u32 color, bool bUseTrans, u32 transRGB)
{
	RGB24 c24 = RGB24(color);
	u8 c8 = GRB8(c24);

	if (bUseTrans)
	{
		if (color == transRGB) // force color 0 for transparent pixel
		{
			c8 = 0;
		}
		else if (c8 == 0) // prevent color 0 for non-transparent pixel
		{
			if (c24.G > c24.R)
				c8 = 0x20;
			else
				c8 = 0x04;
		}
	}
	return c8;
}

//-----------------------------------------------------------------------------
// Get 16-bit GBR color
u16 GetGBR16(u32 color, bool bUseTrans, u32 transRGB)
{
	RGB24 c24 = RGB24(color);
	u16 c16 = GRB16(c24);

	if (bUseTrans)
	{
		if (color == transRGB) // force color 0 for transparent pixel
		{
			c16 = 0;
		}
		else if (c16 == 0) // prevent color 0 for non-transparent pixel
		{
			c16 = GRB16(RGB24(1, 1, 1));
		}
	}
	return c16;
}

//-----------------------------------------------------------------------------
// Export a table
void ExportTable(ExportParameters* param, ExporterInterface* exp, const std::vector<u8>& data, u32 modX)
{
	u32 i = 0;
	for (; i < data.size(); )
	{
		if (i % modX == 0)
			exp->WriteLineBegin();

		exp->Write1ByteData(data[i++]);

		if (i % modX == 0)
			exp->WriteLineEnd();
	}
	if (i % modX != 0)
		exp->WriteLineEnd();
}

namespace Pletter { void Export(const std::vector<u8>& in, std::vector<u8>& out); }

//-----------------------------------------------------------------------------
// Export Pletter data
void ExportPletter(ExportParameters* param, ExporterInterface* exp, const std::vector<u8>& data)
{
	std::vector<u8> out;

	Pletter::Export(data, out);

	ExportTable(param, exp, out, 16);
}

//-----------------------------------------------------------------------------
// Export RLEp data
void ExportRLEp(ExportParameters* param, ExporterInterface* exp, const std::vector<u8>& data)
{
	u32 chunk = 0;
	for (u32 i = 0; i < data.size(); i++)
	{
		u8 key = data[i];
		u8 len = 1;
		while ((i + 1 < data.size()) && (data[i + 1] == key))
		{
			len++;
			i++;
		}
		if (len > 1)  // Repeating patterns
		{
			u8 type = (key == 0) ? 0 : 1;
			exp->WriteCommentLine(MSX::Format("Chunk[%i]", chunk++));
			exp->Write1ByteLine((type << 6) | len, MSX::Format("Type=%i, Length=%i", type, len));
			if (type == 1)
			{
				exp->WriteLineBegin();
				exp->Write8BitsData(key);
				exp->WriteLineEnd();
			}
		}
		else // Uncompressed data
		{
			u8 type = 3;
			std::vector<u8> block;
			block.push_back(key);
			while ((i + 2 < data.size()) && (data[i + 1] != key) && (data[i + 2] != data[i + 1]))
			{
				key = data[i + 1];
				block.push_back(key);
				len++;
				i++;
			}
			exp->WriteCommentLine(MSX::Format("Chunk[%i]", chunk++));
			exp->Write1ByteLine((type << 6) | len, MSX::Format("Type=%i, Length=%i", type, len));
			for (u32 j = 0; j < block.size(); j++)
			{
				exp->WriteLineBegin();
				exp->Write8BitsData(block[j]);
				exp->WriteLineEnd();
			}
		}

	}
	exp->WriteCommentLine("Zero terminator");
	exp->Write1ByteLine(0x00, "");
}

//-----------------------------------------------------------------------------
// Prepare image for export
FIBITMAP* GetPreparedImage(std::string filename, ExportParameters* param)
{
	FIBITMAP *dib, *dib32;
	dib = LoadImage(param->inFile.c_str()); // open and load the file using the default load option
	if (dib == NULL)
	{
		printf("Error: Fail to load %s\n", param->inFile.c_str());
		return NULL;
	}

	// Apply transformations
	if (param->scaleX && param->scaleY)
	{
		dib = FreeImage_Rescale(dib, param->scaleX, param->scaleY, (FREE_IMAGE_FILTER)param->scaleFilter);
		if (dib == NULL)
		{
			printf("Error: Fail to rescale image (x:%i, y:%i, filter:%i)\n", param->scaleX, param->scaleY, param->scaleFilter);
			return NULL;
		}
	}
	if (param->flipH)
	{
		if (!FreeImage_FlipHorizontal(dib))
		{
			printf("Error: Fail to apply horizontal flip\n");
			FreeImage_Unload(dib); // free the original dib
			return NULL;
		}
	}
	if (param->flipV)
	{
		if (!FreeImage_FlipVertical(dib))
		{
			printf("Error: Fail to apply vertical flip\n");
			FreeImage_Unload(dib); // free the original dib
			return NULL;
		}
	}
	if (param->rotAngle != 0)
	{
		dib = FreeImage_Rotate(dib, param->rotAngle, NULL);
		if (dib == NULL)
		{
			printf("Error: Fail to rotate image (angle:%i)\n", (i32)param->rotAngle);
			return NULL;
		}
	}

	dib32 = FreeImage_ConvertTo32Bits(dib);
	FreeImage_Unload(dib); // free the original dib

	return dib32;
}

//#############################################################################
//
// EXPORT BITMAP
// 
//#############################################################################

//-----------------------------------------------------------------------------
// Export bitmap image
bool ExportBitmap(ExportParameters * param, ExporterInterface * exp)
{
	FIBITMAP *dib32;
	i32 i, j, nx, ny, bit, minX, maxX, minY, maxY;
	RGB24 c24;
	GRB16 c16;
	GRB8 c8;
	u8 c2, c4, byte = 0;
	char strData[BUFFER_SIZE];
	u32 transRGB = 0x00FFFFFF & param->transColor;
	u32 headAddr = 0, palAddr = 0;
	std::vector<u16> sprtAddr;

	dib32 = GetPreparedImage(param->inFile.c_str(), param); // open and load the file using the default load option
	if (dib32 == NULL)
	{
		printf("Error: Fail to prepare 32-bit image\n");
		return false;
	}

	// Get 32 bits version
	i32 imageX = FreeImage_GetWidth(dib32);
	i32 imageY = FreeImage_GetHeight(dib32);
	i32 scanWidth = FreeImage_GetPitch(dib32);
	BYTE* bits = new BYTE[scanWidth * imageY];
	FreeImage_ConvertToRawBits(bits, dib32, scanWidth, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);

	// Get custom palette for 16 colors mode
	u32 customPalette[16];
	if (param->palType == PALETTE_Input)
	{
		for (i32 i = 0; i < param->palCount; i++)
			customPalette[i] = param->palInput[i];
	}
	else if ((param->bpc == 4) && (param->palType == PALETTE_Custom))
	{
		if (param->bUseTrans)
		{
			u32 black = 0;
			i32 res = FreeImage_ApplyColorMapping(dib32, (RGBQUAD*)&transRGB, (RGBQUAD*)&black, 1, true, false); // @warning: must be call AFTER retreving raw data!
		}
		FIBITMAP* dib4 = FreeImage_ColorQuantizeEx(dib32, FIQ_LFPQUANT, param->palCount, 0, NULL); // Try Lossless Fast Pseudo-Quantization algorithm (if there are 15 colors or less)
		if(dib4 == NULL)
			dib4 = FreeImage_ColorQuantizeEx(dib32, FIQ_WUQUANT, param->palCount, 0, NULL); // Else, use Efficient Statistical Computations for Optimal Color Quantization
		RGBQUAD* pal = FreeImage_GetPalette(dib4);
		
		for (i32 c = 0; c < param->palOffset; c++)
			customPalette[c] = 0;
		for (i32 c = 0; c < param->palCount; c++)
			customPalette[c + param->palOffset] = ((u32*)pal)[c];
		FreeImage_Unload(dib4);
	}
	else if ((param->bpc == 2) && (param->palType == PALETTE_Custom))
	{
		if (param->bUseTrans)
		{
			u32 black = 0;
			i32 res = FreeImage_ApplyColorMapping(dib32, (RGBQUAD*)&transRGB, (RGBQUAD*)&black, 1, true, false); // @warning: must be call AFTER retreving raw data!
		}
		FIBITMAP* dib2 = FreeImage_ColorQuantizeEx(dib32, FIQ_LFPQUANT, param->palCount, 0, NULL); // Try Lossless Fast Pseudo-Quantization algorithm (if there are 3 colors or less)
		if (dib2 == NULL)
			dib2 = FreeImage_ColorQuantizeEx(dib32, FIQ_WUQUANT, param->palCount, 0, NULL); // Else, use Efficient Statistical Computations for Optimal Color Quantization
		RGBQUAD* pal = FreeImage_GetPalette(dib2);
		for (i32 c = 0; c < param->palOffset; c++)
			customPalette[c] = 0;
		for (i32 c = 0; c < param->palCount; c++)
			customPalette[c + param->palOffset] = ((u32*)pal)[c];
		FreeImage_Unload(dib2);
	}
	// Apply dithering for 2 color mode
	if ((param->bpc == 1) && (param->dither != DITHER_None))
	{
		FIBITMAP* dib1 = FreeImage_Dither(dib32, (FREE_IMAGE_DITHER)param->dither);
		FreeImage_ConvertToRawBits(bits, dib1, scanWidth, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
		FreeImage_Unload(dib1);
	}

	FreeImage_Unload(dib32);

	// Handle whole image case
	if ((param->sizeX == 0) || (param->sizeY == 0))
	{
		param->posX = param->posY = 0;
		param->sizeX = imageX;
		param->sizeY = imageY;
		param->numX = param->numY = 1;
	}

	//-------------------------------------------------------------------------
	// File header
	
	exp->WriteHeader();

	//-------------------------------------------------------------------------
	// Header table

	if (param->bAddHeader)
	{
		sprintf(strData, "%s_header", param->tabName.c_str());
		exp->WriteTableBegin(TABLE_Header, strData, "Header table");

		exp->Write2WordsLine((u16)param->sizeX, (u16)param->sizeY, "Sprite size (X Y)");
		exp->Write2WordsLine((u16)param->numX, (u16)param->numY, "Sprite count (X Y)");
		exp->Write1ByteLine((u8)param->bpc, "Bits per color");
		exp->Write1ByteLine((u8)param->comp, "Compressor");
		exp->Write1ByteLine(param->bSkipEmpty ? 1 : 0, "Skip empty");

		exp->WriteTableEnd("");
	}

	//-------------------------------------------------------------------------
	// Sprite table

	sprtAddr.resize(param->numX * param->numY);
	exp->WriteTableBegin(TABLE_U8, param->tabName, "Data table");

	// Font header
	if (param->bAddFont)
	{
		exp->WriteCommentLine("Font header data");
		exp->Write1ByteLine((u8)((8 << 4) + (param->sizeY & 0x0F)), "Data size [x|y]");
		exp->Write1ByteLine((u8)(((param->fontX & 0x0F) << 4) + (param->fontY & 0x0F)), "Font size [x|y]");
		sprintf(strData, "First character ASCII code (%c)", param->fontFirst);
		exp->Write1ByteLine((u8)param->fontFirst, strData);
		sprintf(strData, "Last character ASCII code (%c)", param->fontLast);
		exp->Write1ByteLine((u8)param->fontLast, strData);
	}

	// BLOAD header
	if (param->bBLOAD)
	{
		exp->WriteCommentLine("BLOAD header data");
		exp->Write1ByteLine(0xFE, "");
		exp->Write1ByteLine(0, "");
		exp->Write1ByteLine(0, "");
		u16 size = (u16)(param->sizeX * param->sizeY * param->numX * param->numY * param->bpc / 8) - 1;
		exp->Write1ByteLine(size & 0xFF, "");
		exp->Write1ByteLine(size >> 8, "");
		exp->Write1ByteLine(0, "");
		exp->Write1ByteLine(0, "");
	}

	// Parse source image
	for(ny = 0; ny < param->numY; ny++)
	{
		for (nx = 0; nx < param->numX; nx++)
		{
			sprtAddr[nx + (ny * param->numX)] = (u16)exp->GetTotalBytes();

			// Print sprite header
			exp->WriteSpriteHeader(nx + (ny * param->numX));

			//-----------------------------------------------------------------
			//
			// RLE compression
			//
			//-----------------------------------------------------------------
			if (param->comp & MSX::COMPRESS_RLE_Mask)
			{
				i32 maxLength;
				switch (param->comp)
				{
				case MSX::COMPRESS_RLE0: maxLength = 0x7F; break;
				case MSX::COMPRESS_RLE4: maxLength = 0x0F; break;
				default:            maxLength = 0xFF; // COMPRESS_RLE8
				}

				// Hash sprite data
				std::vector<RLEHash> hashTable;
				for (j = 0; j < param->sizeY; j++)
				{
					for (i = 0; i < param->sizeX; i++)
					{
						i32 pixel = param->posX + i + (nx * (param->sizeX + param->gapX)) + ((param->posY + j + (ny * (param->sizeY + param->gapY))) * imageX);
						u32 rgb = 0xFFFFFF & ((u32*)bits)[pixel];

						if (param->comp == MSX::COMPRESS_RLE0) // Transparency color Run-length encoding
						{
							if ((hashTable.size() != 0) && (rgb == transRGB) && (hashTable.back().color == transRGB) && (hashTable.back().length < maxLength))
							{
								hashTable.back().length++;
							}
							else if ((hashTable.size() != 0) && (rgb != transRGB) && (hashTable.back().color != transRGB) && (hashTable.back().length < maxLength))
							{
								hashTable.back().length++;
								hashTable.back().data.push_back(rgb);
							}
							else
							{
								RLEHash hash;
								hash.color = rgb;
								hash.length = 1;
								hash.data.push_back(rgb);
								hashTable.push_back(hash);
							}
						}
						else if ((param->comp == MSX::COMPRESS_RLE4) || (param->comp == MSX::COMPRESS_RLE8)) // Full color Run-length encoding
						{
							if ((hashTable.size() != 0) && (rgb == hashTable.back().color) && (hashTable.back().length < maxLength))
							{
								hashTable.back().length++;
							}
							else
							{
								RLEHash hash;
								hash.color = rgb;
								hash.length = 1;
								hashTable.push_back(hash);
							}
						}
					}
				}

				// Write hash table
				for (u32 k = 0; k < hashTable.size(); k++)
				{
					exp->WriteLineBegin();
					if (param->comp == MSX::COMPRESS_RLE0) // Transparency color Run-length encoding
					{
						if (hashTable[k].color == transRGB)
						{
							exp->Write1ByteData(0x80 + (u8)hashTable[k].length);
						}
						else
						{
							exp->Write1ByteData((u8)hashTable[k].length);
							if (param->bpc == 4) // 4-bits index color palette
							{
								u8 byte;
								for (u32 l = 0; l < hashTable[k].data.size(); l++)
								{
									u32 rgb = hashTable[k].color;
									u32* pal = (param->palType == PALETTE_MSX1) ? PaletteMSX : customPalette;
									if (param->bUseTrans)
										c4 = (rgb == transRGB) ? 0x0 : GetNearestColorIndex(rgb, pal, param->palCount, param->palOffset);
									else
										c4 = GetNearestColorIndex(rgb, pal, param->palCount, param->palOffset);
									if (l & 0x1)
										byte |= c4; // Second pixel use lower bits
									else
										byte |= (c4 << 4); // First pixel use higher bits
									if ((l & 0x1) || (l == hashTable[k].data.size() - 1))
									{
										exp->Write1ByteData(byte);
										byte = 0;
									}
								}
							}
							else if (param->bpc == 8) // 8-bits GBR color
							{
								for (u32 l = 0; l < hashTable[k].data.size(); l++)
								{
									c8 = GetGBR8(hashTable[k].data[l], param->bUseTrans, transRGB);
									exp->Write1ByteData(c8);
								}
							}
							else if (param->bpc == 16) // 16-bits GBR color
							{
								for (u32 l = 0; l < hashTable[k].data.size(); l++)
								{
									c16 = GetGBR16(hashTable[k].data[l], param->bUseTrans, transRGB);
									exp->Write1ByteData(c16 & 0x00FF);
									exp->Write1ByteData(c16 >> 8);
								}
							}
							
						}
					}
					else if (param->comp == MSX::COMPRESS_RLE4) // Full color 4bits Run-length encoding
					{
						if (param->bpc == 4) // 4-bits index color palette
						{
							u32 rgb = hashTable[k].color;
							u32* pal = (param->palType == PALETTE_MSX1) ? PaletteMSX : customPalette;
							if (param->bUseTrans)
								c4 = (rgb == transRGB) ? 0x0 : GetNearestColorIndex(rgb, pal, param->palCount, param->palOffset);
							else
								c4 = GetNearestColorIndex(rgb, pal, param->palCount, param->palOffset);
							u8 byte = ((0x0F & hashTable[k].length) << 4) + c4;
							exp->Write1ByteData(byte);
						}
					}
					else if (param->comp == MSX::COMPRESS_RLE8) // Full color 8bits Run-length encoding
					{
						if (param->bpc == 4) // 4-bits index color palette
						{
							exp->Write1ByteData((u8)hashTable[k].length);
							u32 rgb = hashTable[k].color;
							u32* pal = (param->palType == PALETTE_MSX1) ? PaletteMSX : customPalette;
							if (param->bUseTrans)
								c4 = (rgb == transRGB) ? 0x0 : GetNearestColorIndex(rgb, pal, param->palCount, param->palOffset);
							else
								c4 = GetNearestColorIndex(rgb, pal, param->palCount, param->palOffset);
							exp->Write1ByteData(c4);
						}
						else if (param->bpc == 8) // 8-bits GBR color
						{
							exp->Write1ByteData((u8)hashTable[k].length);
							c8 = GetGBR8(hashTable[k].color, param->bUseTrans, transRGB);
							exp->Write1ByteData(c8);
						}
					}
					exp->WriteLineEnd();
				}
			}
			//-----------------------------------------------------------------
			//
			// Crop & No compression
			//
			//-----------------------------------------------------------------
			else
			{
				minX = 0;
				maxX = param->sizeX - 1;
				minY = 0;
				maxY = param->sizeY - 1;

				if (param->bUseTrans)
				{
					// Compute bound for crop compression and count non transparent pixels
					i32 count = 0;
					if (param->comp & MSX::COMPRESS_Crop_Mask)
					{
						minX = param->sizeX;
						maxX = 0;
						minY = param->sizeY;
						maxY = 0;
					}
					for (j = 0; j < param->sizeY; j++)
					{
						for (i = 0; i < param->sizeX; i++)
						{
							i32 pixel = param->posX + i + (nx * (param->sizeX + param->gapX)) + ((param->posY + j + (ny * (param->sizeY + param->gapY))) * imageX);
							u32 rgb = 0xFFFFFF & ((u32*)bits)[pixel];
							if (rgb != transRGB)
							{
								if (param->comp & MSX::COMPRESS_Crop_Mask)
								{
									if (i < minX)
										minX = i;
									if (i > maxX)
										maxX = i;
									if (j < minY)
										minY = j;
									if (j > maxY)
										maxY = j;
								}
								count++;
							}
						}
					}

					// Handle Empty
					if (count == 0)
					{
						if (param->bSkipEmpty)
						{
							sprtAddr[nx + (ny * param->numX)] = MSXi_NO_ENTRY;
							continue;
						}
						else if (param->comp & MSX::COMPRESS_Crop_Mask)
							minX = maxX = minY = maxY = 0;
					}

					// Sprite header
					if ((param->comp & MSX::COMPRESS_Crop_Mask))
					{
						if (param->bpc == 1) // 1-bit black & white
						{
							minX &= 0xF8;	 // Round down 8
							maxX |= 0x07;	 // Round up 8
						}
						else if (param->bpc == 2) // 2-bits index color palette
						{
							minX &= 0xFC;	 // Round down 4
							maxX |= 0x03;	 // Round up 4
						}
						else if (param->bpc == 4) // 4-bits index color palette
						{
							minX &= 0xFE;	 // Round down 2
							maxX |= 0x01;	 // Round up 2
						}

						if (param->comp == MSX::COMPRESS_Crop16)
						{
							minX &= 0x0F;	// Clamp to 4-bits (0-15)
							maxX &= 0x0F;	// Clamp to 4-bits (0-15)
							minY &= 0x0F;	// Clamp to 4-bits (0-15)
							maxY &= 0x0F;	// Clamp to 4-bits (0-15)
							exp->Write2BytesLine(u8((minX << 4) + maxX), u8(((minY) << 4) + maxY), "[minX:4|maxX:4] [minY:4|maxY:4]");
						}
						else if (param->comp == MSX::COMPRESS_CropLine16)
						{
							minY &= 0x0F;	// Clamp to 4-bits (0-15)
							maxY &= 0x0F;	// Clamp to 4-bits (0-15)
							exp->Write1ByteLine(u8((minY << 4) + maxY), "[minY:4|maxY:4]");
						}
						else if (param->comp == MSX::COMPRESS_Crop32)
						{
							minX &= 0x07;	// Clamp to 3-bits (0-7)
							maxX &= 0x1F;	// Clamp to 5-bits (0-31)
							minY &= 0x07;	// Clamp to 3-bits (0-7)
							maxY &= 0x1F;	// Clamp to 5-bits (0-31)
							exp->Write2BytesLine(u8((minX << 5) + maxX), u8(((minY) << 5) + maxY), "[minX:3|maxX:5] [minY:3|maxY:5]");
						}
						else if (param->comp == MSX::COMPRESS_CropLine32)
						{
							minY &= 0x07;	// Clamp to 3-bits (0-7)
							maxY &= 0x1F;	// Clamp to 5-bits (0-31)
							exp->Write1ByteLine(u8(((minY) << 5) + maxY), "[minY:3|maxY:5]");
						}
						else if (param->comp == MSX::COMPRESS_Crop256)
						{
							exp->Write4BytesLine(u8(minX), u8(maxX), u8(minY), u8(maxY), "[minX] [maxX] [minY] [maxY]");
						}
						else if (param->comp == MSX::COMPRESS_CropLine256)
						{
							exp->Write2BytesLine(u8(minY), u8(maxY), "[minY] [maxY]");
						}
					}
				}

				// Print sprite content
				for (j = 0; j < param->sizeY; j++)
				{
					if ((j >= minY) && (j <= maxY))
					{
						// for line-crop, we need to recompute minX&maxX for each line
						if (param->comp & MSX::COMPRESS_CropLine_Mask)
						{
							minX = param->sizeX;
							maxX = 0;
							for (i = 0; i < param->sizeX; i++)
							{
								i32 pixel = param->posX + i + (nx * (param->sizeX + param->gapX)) + ((param->posY + j + (ny * (param->sizeY + param->gapY))) * imageX);
								u32 rgb = 0xFFFFFF & ((u32*)bits)[pixel];
								if (rgb  != transRGB)
								{
									if (i < minX)
										minX = i;
									if (i > maxX)
										maxX = i;
								}
							}
							if (param->bpc == 1) // 1-bit black & white
							{
								minX &= 0xF8;	 // Round down 8
								maxX |= 0x07;	 // Round up 8
							}
							else if (param->bpc == 2) // 2-bits index color palette
							{
								minX &= 0xFC;	 // Round down 4
								maxX |= 0x03;	 // Round up 4
							}
							else if (param->bpc == 4) // 4-bits index color palette
							{
								minX &= 0xFE;	 // Round down 2
								maxX |= 0x01;	 // Round up 2
							}

							// Add row range info
							if (param->comp == MSX::COMPRESS_CropLine16)
							{
								minX &= 0x0F;	// Clamp to 4-bits (0-15)
								maxX &= 0x0F;	// Clamp to 4-bits (0-15)
								exp->Write1ByteLine(u8((minX << 4) + maxX), "[minX:4|maxX:4]");
							}
							else if (param->comp == MSX::COMPRESS_CropLine32)
							{
								minX &= 0x07;	// Clamp to 3-bits (0-7)
								maxX &= 0x1F;	// Clamp to 5-bits (0-31)
								exp->Write1ByteLine(u8(((minX) << 5) + maxX), "[minX:3|maxX:5]");
							}
							else if (param->comp == MSX::COMPRESS_CropLine256)
							{
								exp->Write2BytesLine(u8(minX), u8(maxX), "[minX] [maxX]");
							}
						}

						// Add sprinte data
						exp->WriteLineBegin();
						byte = 0;
						for (i = 0; i < param->sizeX; i++)
						{
							if ((i >= minX) && (i <= maxX))
							{
								i32 pixel = param->posX + i + (nx * (param->sizeX + param->gapX)) + ((param->posY + j + (ny * (param->sizeY + param->gapY))) * imageX);
								u32 rgb = 0xFFFFFF & ((u32*)bits)[pixel];
								//-----------------------------------------------------------------
								if (param->bpc == 16) // 16-bits GBR color
								{
									// convert to 16 bits GRB
									c16 = GetGBR16(rgb, param->bUseTrans, transRGB);
									exp->Write1ByteData(c16 & 0x00FF);
									exp->Write1ByteData(c16 >> 8);
								}
								//-----------------------------------------------------------------
								else if (param->bpc == 8) // 8-bits GBR color
								{
									// convert to 8 bits GRB
									c8 = GetGBR8(rgb, param->bUseTrans, transRGB);
									exp->Write1ByteData((u8)c8);
								}
								//-----------------------------------------------------------------
								else if (param->bpc == 4) // 4-bits index color palette
								{
									u32* pal = (param->palType == PALETTE_MSX1) ? PaletteMSX : customPalette;
									if (param->bUseTrans)
										c4 = (rgb == transRGB) ? 0x0 : GetNearestColorIndex(rgb, pal, param->palCount, param->palOffset);
									else
										c4 = GetNearestColorIndex(rgb, pal, param->palCount, param->palOffset);
									c4 &= 0x0F;

									if ((i & 0x1) == 0)
										byte |= (c4 << 4); // First pixel use higher bits
									else // ((i & 0x1) == 1)
										byte |= c4; // Second pixel use lower bits
									if (((i & 0x1) == 1) || (i == maxX))
									{
										exp->Write1ByteData(byte);
										byte = 0;
									}
								}
								//-----------------------------------------------------------------
								else if (param->bpc == 2) // 2-bits index color palette
								{
									u32* pal = (param->palType == PALETTE_MSX1) ? PaletteMSX : customPalette;
									if (param->bUseTrans)
										c2 = (rgb == transRGB) ? 0x0 : GetNearestColorIndex(rgb, pal, param->palCount, param->palOffset);
									else
										c2 = GetNearestColorIndex(rgb, pal, param->palCount, param->palOffset);
									c2 &= 0x03;

									if ((i & 0x3) == 0)
										byte |= (c2 << 6); // First pixel
									else if ((i & 0x3) == 1)
										byte |= (c2 << 4); // Second pixel
									else if ((i & 0x3) == 2)
										byte |= (c2 << 2); // Third  pixel
									else // ((i & 0x3) == 3)
										byte |= c2; // Fourth pixel

									if (((i & 0x3) == 3) || (i == maxX))
									{
										exp->Write1ByteData(byte);
										byte = 0;
									}
								}
								//-----------------------------------------------------------------
								else if (param->bpc == 1) // Black & white
								{
									bit = pixel & 0x7;
									if (param->bUseTrans)
									{
										if (rgb != transRGB) // All non-transparent color are 1
											byte |= 1 << (7 - bit);
									}
									else
									{
										if (rgb != 0) // All non-black color are 1
											byte |= 1 << (7 - bit);
									}
									if (((pixel & 0x7) == 0x7) || (i == maxX))
									{
										exp->Write8BitsData(byte);
										byte = 0;
									}
								}
							}
						}
						exp->WriteLineEnd();
					}
				}
			}
		}
	}
	sprintf(strData, "Total size : %i bytes", exp->GetTotalBytes());
	exp->WriteTableEnd(strData);

	delete[] bits;

	//-------------------------------------------------------------------------
	// INDEX TABLE

	if (param->bAddIndex)
	{
		sprintf(strData, "%s_index", param->tabName.c_str());
		exp->WriteTableBegin(TABLE_U16, strData, "Images index");
		for (i32 i = 0; i < (i32)sprtAddr.size(); i++)
		{
			exp->Write1WordLine(sprtAddr[i], "");
		}
		exp->WriteTableEnd("");
	}

	//-------------------------------------------------------------------------
	// PALETTE TABLE

	if (((param->bpc == 2) || (param->bpc == 4)) && ((param->palType == PALETTE_Custom) || (param->palType == PALETTE_Input)))
	{
		sprintf(strData, "%s_palette", param->tabName.c_str());
		if (param->pal24)
		{
			exp->WriteTableBegin(TABLE_U8, strData, "Custom palette | Format: [x:3|R:5] [x:3|G:5] [x:3|B:5] (v9990)");
			for (i32 i = param->palOffset; i < param->palOffset + param->palCount; i++)
			{
				exp->WriteLineBegin();
				RGB24 color(customPalette[i]);
				exp->Write1ByteData(color.R >> 3);
				exp->Write1ByteData(color.G >> 3);
				exp->Write1ByteData(color.B >> 3);
				sprintf(strData, "[%2i] #%06X", i, customPalette[i]);
				exp->WriteCommentLine(strData);
			}
		}
		else
		{
			exp->WriteTableBegin(TABLE_U8, strData, "Custom palette | Format: [x|R:3|x|B:3] [x:5|G:3] (v9938)");
			for (i32 i = param->palOffset; i < param->palOffset + param->palCount; i++)
			{
				RGB24 color(customPalette[i]);
				u8 c1 = ((color.R >> 5) << 4) + (color.B >> 5);
				u8 c2 = (color.G >> 5);
				sprintf(strData, "[%2i] #%06X", i, customPalette[i]);
				exp->Write2BytesLine(u8(c1), u8(c2), strData);
			}
		}
		exp->WriteTableEnd("");
	}

	// Write file
	bool bSaved = exp->Export();

	return bSaved;
}

//-----------------------------------------------------------------------------
// Build 256 colors palette
void Create256ColorsPalette(const char* filename)
{
	RGB24 ColorTable[256];
	FILE* file;
	// Create table
	for(int i=0; i<256; i++)
	{
		ColorTable[i] = RGB24(GRB8(i));
	}
	// Save
	file = fopen(filename, "wb");
	fwrite(ColorTable, sizeof(ColorTable), 1, file);
	fclose(file);
}

//-----------------------------------------------------------------------------
// Build 16 colors palette
void Create16ColorsPalette(const char* filename)
{
	RGB24 ColorTable[256];
	FILE* file;
	// Create table
	for(int i=0; i<256; i++)
	{
		switch(i)
		{
			case 2:  ColorTable[i] = RGB24(36,  218, 36); break;
			case 3:  ColorTable[i] = RGB24(109, 255, 109); break;
			case 4:  ColorTable[i] = RGB24(36,  255, 36); break;
			case 5:  ColorTable[i] = RGB24(72,  109, 255); break;
			case 6:  ColorTable[i] = RGB24(182, 36,  36); break;
			case 7:  ColorTable[i] = RGB24(72,  218, 255); break;
			case 8:  ColorTable[i] = RGB24(255, 36,  36); break;
			case 9:  ColorTable[i] = RGB24(255, 109, 109); break;
			case 10: ColorTable[i] = RGB24(218, 218, 36); break;
			case 11: ColorTable[i] = RGB24(218, 218, 145); break;
			case 12: ColorTable[i] = RGB24(36,  145, 36); break;
			case 13: ColorTable[i] = RGB24(218, 72,  182); break;
			case 14: ColorTable[i] = RGB24(182, 182, 182); break;
			case 15: ColorTable[i] = RGB24(255, 255, 255); break;
			default: ColorTable[i] = RGB24(0, 0, 0);
		}
	}
	// Save
	file = fopen(filename, "wb");
	fwrite(ColorTable, sizeof(ColorTable), 1, file);
	fclose(file);
}

//#############################################################################
//
// EXPORT GRAPHIC 1
// 
//#############################################################################

// GM1 chunk structure
struct ChunkGM1
{
	u8 Color0;
	u8 Color1 = 0xFF;
	u8 Pattern[8];

	bool IsPlain() { return Color1 == 0xFF;  }

	void InvertPattern()
	{
		for (i32 i = 0; i < 8; i++)
			Pattern[i] = ~Pattern[i];
	}

	void Invert()
	{
		assert(!IsPlain());
		u8 tmp = Color1;
		Color1 = Color0;
		Color0 = tmp;
		InvertPattern();
	}
};

// GM1 group structure
struct GroupGM1
{
	u8 Color0;
	u8 Color1 = 0xFF;
	std::vector<ChunkGM1> Chunks;

	bool IsPlain() { return Color1 == 0xFF; }

	bool Insert(ChunkGM1& chunk)
	{
		if (Chunks.size() >= 8)
			return false;

		if (!chunk.IsPlain() && !IsPlain()) // Chunk and group not plain
		{
			if ((chunk.Color0 != Color0) || (chunk.Color1 != Color1))
				return false;
		}
		else if (chunk.IsPlain() && IsPlain()) // Chunk and group plain
		{
			if (chunk.Color0 != Color0)
			{
				if (chunk.Color0 > Color0)
				{
					Color1 = chunk.Color0;
					chunk.InvertPattern();
				}
				else
				{
					Color1 = Color0;
					Color0 = chunk.Color0;
					for (u8 i = 0; i < Chunks.size(); i++)
						Chunks[i].InvertPattern();
				}
			}
		}
		else if (!chunk.IsPlain() && IsPlain()) // Chunk not plain and group plain
		{
			if (chunk.Color0 == Color0)
			{
				Color1 = chunk.Color1;
			}
			else if (chunk.Color1 == Color0)
			{
				Color0 = chunk.Color0;
				Color1 = chunk.Color1;
				for (u8 i = 0; i < Chunks.size(); i++)
					Chunks[i].InvertPattern();
			}
			else
				return false;
		}
		else if (chunk.IsPlain() && !IsPlain()) // Chunk plain and group not plain
		{
			if (chunk.Color0 == Color1)
			{
				chunk.InvertPattern();
			}
			else if (chunk.Color0 != Color0)
				return false;
		}

		Chunks.push_back(chunk);
		return true;
	}
};

//-----------------------------------------------------------------------------
// Get GM1 chunk ID
u8 GetChunkIdGM1(std::vector<GroupGM1>& group, ChunkGM1& chunk, ExportParameters* param)
{
	// Force color0 to be smaller than color1
	if (chunk.Color0 > chunk.Color1)
		chunk.Invert();

	// Search for existing chunk index
	if (param->bTilesUnique && group.size())
	{
		GroupGM1& grp = group.back();
		if (grp.Chunks.size() < 8)
		{
			if (!grp.Insert(chunk))
			{
				printf("Error: Invalid chunk colors for group %d!\n", (int)(group.size() - 1));
				exit(1);
			}
			return static_cast<u8>(((group.size() - 1) * 8) + grp.Chunks.size() - 1);
		}
	}
	else
	{
		for (u8 i = 0; i < group.size(); i++)
		{
			for (u8 j = 0; j < group[i].Chunks.size(); j++)
			{
				if (chunk.IsPlain() && group[i].Chunks[j].IsPlain() && (chunk.Color0 == group[i].Chunks[j].Color0)) // Don't check pattern
					return (i * 8) + j;

				if (memcmp(&group[i].Chunks[j], &chunk, sizeof(ChunkGM1)) == 0)
					return (i * 8) + j;
			}
		}

		// Check for matching color group
		for (u8 i = 0; i < group.size(); i++)
		{
			if (group[i].Chunks.size() == 8) // Is group full?
				continue;

			if (!chunk.IsPlain() && !group[i].IsPlain()) // Chunk and group not plain
			{
				if ((chunk.Color0 == group[i].Color0) && (chunk.Color1 == group[i].Color1))
				{
					group[i].Chunks.push_back(chunk);
					return static_cast<u8>((i * 8) + group[i].Chunks.size() - 1);
				}
			}
			else if (chunk.IsPlain() && group[i].IsPlain()) // Chunk and group plain
			{
				assert(chunk.Color0 != group[i].Color0);
				assert(group[i].Chunks.size() == 1);
				chunk.InvertPattern();
				group[i].Color1 = chunk.Color0;
				group[i].Chunks.push_back(chunk);
				return static_cast<u8>((i * 8) + group[i].Chunks.size() - 1);
			}
			else if (!chunk.IsPlain() && group[i].IsPlain()) // Chunk not plain and group plain
			{
				assert(group[i].Chunks.size() == 1);
				if (chunk.Color0 == group[i].Color0)
				{
					group[i].Color1 = chunk.Color1;
					group[i].Chunks.push_back(chunk);
					return static_cast<u8>((i * 8) + group[i].Chunks.size() - 1);
				}
				else if (chunk.Color1 == group[i].Color0)
				{
					group[i].Chunks[0].InvertPattern();
					group[i].Color0 = chunk.Color0;
					group[i].Color1 = chunk.Color1;
					group[i].Chunks.push_back(chunk);
					return static_cast<u8>((i * 8) + group[i].Chunks.size() - 1);
				}
			}
			else if (chunk.IsPlain() && !group[i].IsPlain())
			{
				if (chunk.Color0 == group[i].Color0)
				{
					group[i].Chunks.push_back(chunk);
					return static_cast<u8>((i * 8) + group[i].Chunks.size() - 1);
				}
				else if (chunk.Color0 == group[i].Color1)
				{
					chunk.InvertPattern();
					group[i].Chunks.push_back(chunk);
					return static_cast<u8>((i * 8) + group[i].Chunks.size() - 1);
				}
			}
		}
	}

	// Check for free group
	if (group.size() >= 32)
	{
		printf("Error: Image need more than 32 color groups!\n");
		exit(1);
	}

	// Create new group
	GroupGM1 grp;
	grp.Color0 = chunk.Color0;
	grp.Color1 = chunk.Color1;
	grp.Chunks.push_back(chunk);
	group.push_back(grp);
	return static_cast<u8>((group.size() - 1) * 8);
}

//-----------------------------------------------------------------------------
// Export GM1 image
bool ExportGM1(ExportParameters* param, ExporterInterface* exp)
{
	std::vector<GroupGM1> chunkList;
	FIBITMAP *dib32;

	//-------------------------------------------------------------------------
	// Prepare image

	dib32 = GetPreparedImage(param->inFile.c_str(), param); // open and load the file using the default load option
	if (dib32 == NULL)
	{
		printf("Error: Fail to prepare 32-bit image\n");
		return false;
	}

	// Get 32 bits raw datas
	i32 imageX = FreeImage_GetWidth(dib32);
	i32 imageY = FreeImage_GetHeight(dib32);
	i32 scanWidth = FreeImage_GetPitch(dib32);
	BYTE* bits = new BYTE[scanWidth * imageY];
	FreeImage_ConvertToRawBits(bits, dib32, scanWidth, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
	FreeImage_Unload(dib32);

	// Check image size
	if ((param->sizeX == 0) || (param->sizeY == 0))
	{
		param->sizeX = imageX - param->posX;
		param->sizeY = imageY - param->posY;
	}
	if ((param->posX + param->sizeX) > imageX)
		param->sizeX = imageX - param->posX;
	if ((param->posY + param->sizeY) > imageY)
		param->sizeY = imageY - param->posY;

	// Convert default extract param into a layer
	{
		Layer l;
		l.posX = param->posX;
		l.posY = param->posY;
		l.numX = param->sizeX;
		l.numY = param->sizeY;
		param->layers.insert(param->layers.begin(), l);
	}

	//if (param->bTilesUnique)
	//	param->bTilesName = false;

	// File header
	exp->WriteHeader();

	for (u32 l = 0; l < param->layers.size(); l++)
	{
		Layer* layer = &param->layers[l];

		u32 numX = layer->numX / 8;
		u32 numY = layer->numY / 8;

		// Parse image
		std::vector<u8> layoutTable;
		for (u32 ny = 0; ny < numY; ny++)
		{
			for (u32 nx = 0; nx < numX; nx++)
			{
				ChunkGM1 chunk;

				// Generate chunk
				std::vector<u8> colors;
				for (i32 j = 0; j < 8; j++) // Y
				{
					u8 pattern = 0;
					for (i32 i = 0; i < 8; i++) // X
					{
						i32 idx = layer->posX + i + (nx * 8) + ((layer->posY + j + (ny * 8)) * imageX);
						u32 c24 = 0xFFFFFF & ((u32*)bits)[idx];
						u8 c4 = GetNearestColorIndex(c24, PaletteMSX, 16, 1);
						if (colors.empty()) // special case: first color
						{
							colors.push_back(c4);
						}
						else if ((colors.size() == 1) && (c4 != colors[0])) // special case: second color
						{
							colors.push_back(c4);
						}

						if (c4 == colors[0])
							continue;
						else if (c4 == colors[1])
							pattern |= 1 << (7 - i);
						else
							printf("Warning: More than 2 colors on a 8x8 pixels square (%i, %i)\n", layer->posX + i + (nx * 8), layer->posY + j + (ny * 8));
					}
					chunk.Pattern[j] = pattern;
				}

				chunk.Color0 = colors[0];
				chunk.Color1 = (colors.size() == 2) ? colors[1] : 0xFF;
				u8 patIdx = GetChunkIdGM1(chunkList, chunk, param);
				layoutTable.push_back(patIdx + param->offset);
			}
		}

		//-------------------------------------------------------------------------
		// LAYOUT TABLE

		if (param->bTilesName)
		{
			if (param->layers.size() == 1) // Default
				exp->WriteTableBegin(TABLE_U8, param->tabName + "_Names", "Names Table");
			else
				exp->WriteTableBegin(TABLE_U8, MSX::Format("%sL%i_Names", param->tabName.c_str(), l), "Names Table");

			if (param->bTilesCompressNames && param->comp == MSX::COMPRESS_RLEp)
				ExportRLEp(param, exp, layoutTable);
			else if (param->bTilesCompressNames && param->comp == MSX::COMPRESS_Pletter)
				ExportPletter(param, exp, layoutTable);
			else
				ExportTable(param, exp, layoutTable, numX);

			exp->WriteTableEnd("");
		}
	}
	i32 namesSize = exp->GetTotalBytes();
	exp->WriteCommentLine(MSX::Format("Names size: %i Bytes", namesSize));

	delete[] bits;

	//-------------------------------------------------------------------------
	// PATTERNS TABLE

	i32 patternsSize = 0;
	if (param->bTilesPattern)
	{
		exp->WriteTableBegin(TABLE_U8, param->tabName + "_Patterns", "Patterns Table");
		if (param->comp != MSX::COMPRESS_None)
		{
			// Build data
			std::vector<u8> bytes;
			for (u64 i = 0; i < chunkList.size(); i++)
			{
				for (u64 j = 0; j < 8; j++)
				{
					if ((i == chunkList.size() - 1) && (j >= chunkList[i].Chunks.size())) // skip last empty chunk
						continue;

					for (i32 k = 0; k < 8; k++)
						if (j < chunkList[i].Chunks.size())
							bytes.push_back(chunkList[i].Chunks[j].Pattern[k]);
						else
							bytes.push_back(0);
				}
			}

			// Compress
			if (param->comp == MSX::COMPRESS_RLEp)
				ExportRLEp(param, exp, bytes);
			else if (param->comp == MSX::COMPRESS_Pletter)
				ExportPletter(param, exp, bytes);
		}
		else
		{
			for (u64 i = 0; i < chunkList.size(); i++) // group
			{
				for (u64 j = 0; j < 8; j++) // chunk
				{
					if ((i == chunkList.size() - 1) && (j >= chunkList[i].Chunks.size()))
						continue;

					// Print sprite header
					exp->WriteSpriteHeader((i32)(i * 8 + j + param->offset));
					for (i32 k = 0; k < 8; k++) // line
					{
						exp->WriteLineBegin();
						if (j < chunkList[i].Chunks.size())
							exp->Write8BitsData(chunkList[i].Chunks[j].Pattern[k]);
						else
							exp->Write8BitsData(0x55);
						exp->WriteLineEnd();
					}
				}
			}
		}
		patternsSize = exp->GetTotalBytes() - namesSize;
		exp->WriteTableEnd(MSX::Format("Patterns size: %i Bytes", patternsSize));
	}

	//-------------------------------------------------------------------------
	// COLORS TABLE

	if (param->bTilesColor)
	{
		exp->WriteTableBegin(TABLE_U8, param->tabName + "_Colors", "Colors Table");
		for (i32 i = 0; i < (i32)chunkList.size(); i++)
		{
			// Print sprite header
			exp->WriteSpriteHeader(i);
			exp->WriteLineBegin();
			exp->Write1ByteData(chunkList[i].Color0 + (chunkList[i].Color1 << 4));
			exp->WriteLineEnd();
		}
		i32 colorsSize = exp->GetTotalBytes() - namesSize - patternsSize;
		exp->WriteTableEnd(MSX::Format("Colors size: %i Bytes", colorsSize));
	}

	exp->WriteLineEnd();
	exp->WriteCommentLine(MSX::Format("Total size: %i Bytes", exp->GetTotalBytes()));

	//-------------------------------------------------------------------------
	// Write file
	bool bSaved = exp->Export();

	return bSaved;
}

//#############################################################################
//
// EXPORT GRAPHIC 2
// 
//#############################################################################

// GM2 chunk
struct ChunkGM2
{
	u8 Pattern[8];
	u8 Color[8];
};

//-----------------------------------------------------------------------------
// Get GM3 chunk ID
u8 GetChunkIdGM2(std::vector<ChunkGM2>& list, const ChunkGM2& chunk, ExportParameters* param)
{
	if (!param->bTilesUnique)
	{
		if (list.size() >= 256)
		{
			printf("Error: Image have more than 256 unique patterns!\n");
			exit(1);
		}
		for (u8 i = 0; i < list.size(); i++)
		{
			if (memcmp(&list[i], &chunk, sizeof(ChunkGM2)) == 0)
				return i;
		}
	}
	list.push_back(chunk);
	return (u8)(list.size() - 1);
}

//-----------------------------------------------------------------------------
// Export GM2
bool ExportGM2(ExportParameters* param, ExporterInterface* exp)
{
	std::vector<ChunkGM2> chunkList;
	FIBITMAP *dib32;

	//-------------------------------------------------------------------------
	// Prepare image

	dib32 = GetPreparedImage(param->inFile.c_str(), param); // open and load the file using the default load option
	if (dib32 == NULL)
	{
		printf("Error: Fail to prepare 32-bit image\n");
		return false;
	}

	// Get 32 bits raw datas
	i32 imageX = FreeImage_GetWidth(dib32);
	i32 imageY = FreeImage_GetHeight(dib32);
	i32 scanWidth = FreeImage_GetPitch(dib32);
	BYTE* bits = new BYTE[scanWidth * imageY];
	FreeImage_ConvertToRawBits(bits, dib32, scanWidth, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
	FreeImage_Unload(dib32);

	// Check image size
	if ((param->sizeX == 0) || (param->sizeY == 0))
	{
		param->sizeX = imageX - param->posX;
		param->sizeY = imageY - param->posY;
	}
	if ((param->posX + param->sizeX) > imageX)
		param->sizeX = imageX - param->posX;
	if ((param->posY + param->sizeY) > imageY)
		param->sizeY = imageY - param->posY;

	// Convert default extract param into a layer
	{
		Layer l;
		l.posX = param->posX;
		l.posY = param->posY;
		l.numX = param->sizeX;
		l.numY = param->sizeY;
		param->layers.insert(param->layers.begin(), l);
	}

	//if (param->bTilesUnique)
	//	param->bTilesName = false;

	// File header
	exp->WriteHeader();

	for (u32 l = 0; l < param->layers.size(); l++)
	{
		Layer* layer = &param->layers[l];

		u32 numX = layer->numX / 8;
		u32 numY = layer->numY / 8;

		// Parse image
		std::vector<u8> layoutTable;
		for (u32 ny = 0; ny < numY; ny++)
		{
			for (u32 nx = 0; nx < numX; nx++)
			{
				ChunkGM2 chunk;

				// Generate chunk
				for (i32 j = 0; j < 8; j++)
				{
					u8 pattern = 0;
					std::vector<u8> colors;
					for (i32 i = 0; i < 8; i++)
					{
						i32 idx = layer->posX + i + (nx * 8) + ((layer->posY + j + (ny * 8)) * imageX);
						u32 c24 = 0xFFFFFF & ((u32*)bits)[idx];
						u8 c4 = GetNearestColorIndex(c24, PaletteMSX, 16, 1);
						if (colors.empty()) // special case: first color
						{
							colors.push_back(c4);
						}
						else if ((colors.size() == 1) && (c4 != colors[0])) // special case: second color
						{
							colors.push_back(c4);
						}

						if (c4 == colors[0])
							continue;
						else if (c4 == colors[1])
							pattern |= 1 << (7 - i);
						else
							printf("Warning: More than 2 colors on a 8 pixels line (%i, %i)\n", layer->posX + i + (nx * 8), layer->posY + j + (ny * 8));
					}
					if (colors.size() == 1)
						colors.push_back(colors[0]);

					if (colors[0] > colors[1])
					{
						chunk.Pattern[j] = ~pattern;
						chunk.Color[j] = (colors[0] << 4) + colors[1];
					}
					else
					{
						chunk.Pattern[j] = pattern;
						chunk.Color[j] = (colors[1] << 4) + colors[0];
					}
				}

				u8 patIdx = GetChunkIdGM2(chunkList, chunk, param);
				layoutTable.push_back(patIdx + param->offset);
			}
		}

		//-------------------------------------------------------------------------
		// LAYOUT TABLE

		if (param->bTilesName)
		{
			if (param->layers.size() == 1) // Default
				exp->WriteTableBegin(TABLE_U8, param->tabName + "_Names", "Names Table");
			else
				exp->WriteTableBegin(TABLE_U8, MSX::Format("%sL%i_Names", param->tabName.c_str(), l), "Names Table");

			if (param->bTilesCompressNames && param->comp == MSX::COMPRESS_RLEp)
				ExportRLEp(param, exp, layoutTable);
			else if (param->bTilesCompressNames && param->comp == MSX::COMPRESS_Pletter)
				ExportPletter(param, exp, layoutTable);
			else
				ExportTable(param, exp, layoutTable, numX);

			exp->WriteTableEnd("");
		}
	}
	i32 namesSize = exp->GetTotalBytes();
	exp->WriteCommentLine(MSX::Format("Names size: %i Bytes", namesSize));

	delete[] bits;

	//-------------------------------------------------------------------------
	// PATTERNS TABLE

	i32 patternsSize = 0;
	if (param->bTilesPattern)
	{
		exp->WriteTableBegin(TABLE_U8, param->tabName + "_Patterns", "Patterns Table");
		if (param->comp != MSX::COMPRESS_None)
		{
			// Build data
			std::vector<u8> bytes;
			for (i32 i = 0; i < (i32)chunkList.size(); i++)
				for (i32 j = 0; j < 8; j++)
					bytes.push_back(chunkList[i].Pattern[j]);

			// Compress
			if (param->comp == MSX::COMPRESS_RLEp)
				ExportRLEp(param, exp, bytes);
			else if (param->comp == MSX::COMPRESS_Pletter)
				ExportPletter(param, exp, bytes);
		}
		else
		{
			for (i32 i = 0; i < (i32)chunkList.size(); i++)
			{
				// Print sprite header
				exp->WriteSpriteHeader(i + param->offset);
				for (i32 j = 0; j < 8; j++)
				{
					exp->WriteLineBegin();
					exp->Write8BitsData(chunkList[i].Pattern[j]);
					exp->WriteLineEnd();
				}
			}
		}
		patternsSize = exp->GetTotalBytes() - namesSize;
		exp->WriteTableEnd(MSX::Format("Patterns size: %i Bytes", patternsSize));
	}

	//-------------------------------------------------------------------------
	// COLORS TABLE

	if (param->bTilesColor)
	{
		exp->WriteTableBegin(TABLE_U8, param->tabName + "_Colors", "Colors Table");
		if (param->comp != MSX::COMPRESS_None)
		{
			// Build data
			std::vector<u8> bytes;
			for (i32 i = 0; i < (i32)chunkList.size(); i++)
				for (i32 j = 0; j < 8; j++)
					bytes.push_back(chunkList[i].Color[j]);

			// Compress
			if (param->comp == MSX::COMPRESS_RLEp)
				ExportRLEp(param, exp, bytes);
			else if (param->comp == MSX::COMPRESS_Pletter)
				ExportPletter(param, exp, bytes);
		}
		else
		{
			for (i32 i = 0; i < (i32)chunkList.size(); i++)
			{
				// Print sprite header
				exp->WriteSpriteHeader(i + param->offset);
				exp->WriteLineBegin();
				for (i32 j = 0; j < 8; j++)
				{
					exp->Write1ByteData(chunkList[i].Color[j]);
				}
				exp->WriteLineEnd();
			}
		}
		i32 colorsSize = exp->GetTotalBytes() - namesSize - patternsSize;
		exp->WriteTableEnd(MSX::Format("Colors size: %i Bytes", colorsSize));
	}

	exp->WriteLineEnd();
	exp->WriteCommentLine(MSX::Format("Total size: %i Bytes", exp->GetTotalBytes()));

	//-------------------------------------------------------------------------
	// Write file
	bool bSaved = exp->Export();

	return bSaved;
}

//#############################################################################
//
// EXPORT SPRITES
// 
//#############################################################################

//-----------------------------------------------------------------------------
// Convert color to binary value according to layer configuration
i32 ColorToBinary(Layer& layer, u32 c24)
{
	std::vector<u32>::iterator it;

	it = std::find(layer.colors.begin(), layer.colors.end(), c24);

	if (layer.include)
	{
		return (it != layer.colors.end()) ? 1 : 0;
	}
	else // if (layer.mode & LAYER_Exclude)
	{
		return (it == layer.colors.end()) ? 1 : 0;
	}
}

//-----------------------------------------------------------------------------
// Export a 8x8 sprite data (1-bit per point)
void ExportSpriteData(ExportParameters* param, ExporterInterface* exp, Layer& layer, i32 sid, i32 x, i32 y, BYTE* bits, i32 imageX, i32 imageY, std::vector<u8> &rawData)
{
	if (param->comp == MSX::COMPRESS_None)
	{
		exp->WriteSpriteHeader(sid);
	}

	for (i32 j = 0; j < 8; j++)
	{
		u8 byte = 0;
		if (((y + j) >= 0) || ((y + j) < imageY))
		{
			for (i32 i = 0; i < 8; i++)
			{
				if (((x + i) >= 0) || ((x + i) < imageX))
				{
					i32 idx = (x + i) + ((y + j) * imageX);
					u32 c24 = 0xFFFFFF & ((u32*)bits)[idx];
					if (ColorToBinary(layer, c24))
						byte |= 1 << (7 - i);
				}
			}
		}
		if (param->comp == MSX::COMPRESS_None)
		{
			exp->WriteLineBegin();
			exp->Write8BitsData(byte);
			exp->WriteLineEnd();
		}
		else
		{
			rawData.push_back(byte);
		}
	}
}

//-----------------------------------------------------------------------------
// Export sprite sheet
bool ExportSprite(ExportParameters* param, ExporterInterface* exp)
{
	FIBITMAP *dib32;
	u32 sid = 0; // sprite id
	std::vector<u8> rawData;

	//-------------------------------------------------------------------------
	// Prepare image

	dib32 = GetPreparedImage(param->inFile.c_str(), param); // open and load the file using the default load option
	if (dib32 == NULL)
	{
		printf("Error: Fail to prepare 32-bit image\n");
		return false;
	}

	// Get 32 bits raw datas
	i32 imageX = FreeImage_GetWidth(dib32);
	i32 imageY = FreeImage_GetHeight(dib32);
	i32 scanWidth = FreeImage_GetPitch(dib32);
	BYTE* bits = new BYTE[scanWidth * imageY];
	FreeImage_ConvertToRawBits(bits, dib32, scanWidth, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
	FreeImage_Unload(dib32);

	if (param->layers.size() == 0)
	{
		Layer l;
		l.size16 = ((param->sizeX == 16) && (param->sizeY == 16));
		l.include = false;
		l.posX = 0;
		l.posY = 0;
		i32 sprtSize = l.size16 ? 16 : 8;
		l.numX = (param->sizeX + sprtSize - 1) / sprtSize;
		l.numY = (param->sizeY + sprtSize - 1) / sprtSize;
		l.colors.push_back(param->transColor);
		param->layers.push_back(l);
	}

	// File header
	exp->WriteHeader();

	//-------------------------------------------------------------------------
	// NAMES TABLE

	exp->WriteTableBegin(TABLE_U8, param->tabName, "Sprites table");

	// Parse image
	for (i32 ny = 0; ny < param->numY; ny++)
	{
		for (i32 nx = 0; nx < param->numX; nx++)
		{
			if (param->comp == MSX::COMPRESS_None)
				exp->WriteCommentLine(MSX::Format("======== Frame[%i]", nx + ny * param->numX));

			for (i32 l = 0; l < (i32)param->layers.size(); l++)
			{
				Layer& layer = param->layers[l];

				if (param->comp == MSX::COMPRESS_None)
					exp->WriteCommentLine(MSX::Format("---- Layer[%i] (%s %i,%i %i,%i %s %i)", l, layer.size16 ? "16x16" : "8x8", layer.posX, layer.posY, layer.numX, layer.numY, layer.include ? "inc" : "dec", layer.colors.size()));

				for (u32 j = 0; j < layer.numY; j++)
				{
					for (u32 i = 0; i < layer.numX; i++)
					{
						if (layer.size16)
						{
							i32 x = param->posX + (nx * (param->sizeX + param->gapX)) + layer.posX + i * 16;
							i32 y = param->posY + (ny * (param->sizeY + param->gapY)) + layer.posY + j * 16;
							ExportSpriteData(param, exp, layer, sid++, x, y, bits, imageX, imageY, rawData);
							y += 8;
							ExportSpriteData(param, exp, layer, sid++, x, y, bits, imageX, imageY, rawData);
							y -= 8;
							x += 8;
							ExportSpriteData(param, exp, layer, sid++, x, y, bits, imageX, imageY, rawData);
							y += 8;
							ExportSpriteData(param, exp, layer, sid++, x, y, bits, imageX, imageY, rawData);
						}
						else // if (layer.mode & LAYER_8x8)
						{
							i32 x = param->posX + (nx * (param->sizeX + param->gapX)) + layer.posX + i * 8;
							i32 y = param->posY + (ny * (param->sizeY + param->gapY)) + layer.posY + j * 8;
							ExportSpriteData(param, exp, layer, sid++, x, y, bits, imageX, imageY, rawData);
						}
					}
				}

			}
		}
	}

	// Export compressed data
	if (param->comp == MSX::COMPRESS_RLEp)
		ExportRLEp(param, exp, rawData);
	else if (param->comp == MSX::COMPRESS_Pletter)
		ExportPletter(param, exp, rawData);

	i32 namesSize = exp->GetTotalBytes();
	exp->WriteTableEnd(MSX::Format("Patterns size: %i Bytes", namesSize));

	delete[] bits;

	//-------------------------------------------------------------------------
	// Write file
	bool bSaved = exp->Export();

	return bSaved;
}

//#############################################################################
//
// EXPORT TEXT
// 
//#############################################################################

// SC0 chunk structure
struct ChunkSC0
{
	u8 Pattern[8];
	u8 Coverage;
	u8 Usage;

	// Check if chunks are equals
	bool IsEqual(ChunkSC0& chunk)
	{
		loop(i, 8)
			if (chunk.Pattern[i] != Pattern[i])
				return false;
		return true;
	}

	// Compute chunk information
	void Compute()
	{
		Coverage = 0;
		loop(i, 8)
		{
			loop(j, 8)
			{
				if (Pattern[i] & (1 << j))
					Coverage++;
			}
		}
	}

	// Return empty chunk
	static ChunkSC0 Empty()
	{
		ChunkSC0 chunk;
		loop(i, 8)
			chunk.Pattern[i] = 0;
		return chunk;
	}

	// Return full chunk
	static ChunkSC0 Full()
	{
		ChunkSC0 chunk;
		loop(i, 8)
			chunk.Pattern[i] = 0xFC;
		return chunk;
	}
};

//-----------------------------------------------------------------------------
// Export TEXT image
u16 GetChunkIdSC0(std::vector<ChunkSC0>& list, ChunkSC0& chunk, ExportParameters* param)
{
	for (u32 i = 0; i < list.size(); i++)
	{
		if (list[i].IsEqual(chunk))
		{
			list[i].Usage++;
			return i;
		}
	}

	chunk.Usage = 1;
	chunk.Compute();
	list.push_back(chunk);
	
	return (u16)list.size() - 1;
}

//-----------------------------------------------------------------------------
// Export TEXT image
bool ExportText(ExportParameters* param, ExporterInterface* exp)
{
	//-------------------------------------------------------------------------
	// Prepare image

	FIBITMAP* dib32 = GetPreparedImage(param->inFile.c_str(), param); // open and load the file using the default load option
	if (dib32 == NULL)
	{
		printf("Error: Fail to prepare 32-bit image\n");
		return false;
	}
	// Get 2 colors image
	if (param->dither == DITHER_None)
	{
		FIBITMAP* dib1 = FreeImage_Threshold(dib32, 192);
		dib32 = FreeImage_ConvertTo32Bits(dib1);
	}
	else
	{
		FIBITMAP* dib1 = FreeImage_Dither(dib32, (FREE_IMAGE_DITHER)param->dither);
		dib32 = FreeImage_ConvertTo32Bits(dib1);
	}

	// Get 32 bits raw datas
	i32 imageX = FreeImage_GetWidth(dib32);
	i32 imageY = FreeImage_GetHeight(dib32);
	i32 scanWidth = FreeImage_GetPitch(dib32);
	BYTE* bits = new BYTE[scanWidth * imageY];
	FreeImage_ConvertToRawBits(bits, dib32, scanWidth, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
	FreeImage_Unload(dib32);

	// Check image size
	if ((param->sizeX == 0) || (param->sizeY == 0))
	{
		param->posX = param->posY = 0;
		param->sizeX = imageX;
		param->sizeY = imageY;
	}

	// File header
	exp->WriteHeader();

	std::vector<ChunkSC0> chunkList;
	std::vector<u16> layoutTable;
	u32 numX = param->sizeX / 6;
	u32 numY = param->sizeY / 8;

	//-------------------------------------------------------------------------
	// Parse data chunks
	for (u32 ny = 0; ny < numY; ny++)
	{
		for (u32 nx = 0; nx < numX; nx++)
		{
			// Generate chunk
			ChunkSC0 chunk;
			for (i32 j = 0; j < 8; j++) // Y
			{
				u8 pattern = 0;
				for (i32 i = 0; i < 6; i++) // X
				{
					i32 idx = param->posX + i + (nx * 6) + ((param->posY + j + (ny * 8)) * imageX);
					u32 c24 = 0xFFFFFF & ((u32*)bits)[idx];
					if(c24)
						pattern |= 1 << (7 - i);
				}
				chunk.Pattern[j] = pattern;
			}
			u16 patIdx = GetChunkIdSC0(chunkList, chunk, param);
			layoutTable.push_back(patIdx + param->offset);
		}
	}

	delete[] bits;

	//-------------------------------------------------------------------------
	// Analyze and prepare for export

	if (chunkList.size() > 256)
	{
		printf("Warning: Image needs more than 256 patterns (needed patterns: %i).\n", (i32)chunkList.size());
	}

	std::vector<u8> layoutTableU8;
	layoutTableU8.resize(layoutTable.size());
	loop(i, layoutTable.size())
		layoutTableU8[i] = (u8)layoutTable[i];

	//-------------------------------------------------------------------------
	// Layout table
	u32 namesSize = 0;
	if (param->bTilesName)
	{
		exp->WriteTableBegin(TABLE_U8, param->tabName + "_Names", "Names Table");

		if (param->bTilesCompressNames && param->comp == MSX::COMPRESS_RLEp)
			ExportRLEp(param, exp, layoutTableU8);
		else if (param->bTilesCompressNames && param->comp == MSX::COMPRESS_Pletter)
			ExportPletter(param, exp, layoutTableU8);
		else
			ExportTable(param, exp, layoutTableU8, numX);

		namesSize = exp->GetTotalBytes();
		exp->WriteTableEnd(MSX::Format("Names size: %i Bytes", namesSize));
	}

	//-------------------------------------------------------------------------
	// Patterns table
	i32 patternsSize = 0;
	if (param->bTilesPattern)
	{
		exp->WriteTableBegin(TABLE_U8, param->tabName + "_Patterns", "Patterns Table");
		if (param->comp != MSX::COMPRESS_None)
		{
			// Build data
			std::vector<u8> bytes;
			for (i32 i = 0; i < (i32)chunkList.size(); i++)
				for (i32 j = 0; j < 8; j++)
					bytes.push_back(chunkList[i].Pattern[j]);

			// Compress
			if (param->comp == MSX::COMPRESS_RLEp)
				ExportRLEp(param, exp, bytes);
			else if (param->comp == MSX::COMPRESS_Pletter)
				ExportPletter(param, exp, bytes);
		}
		else
		{
			for (i32 i = 0; i < (i32)chunkList.size(); i++)
			{
				// Print sprite header
				exp->WriteSpriteHeader(i + param->offset);
				for (i32 j = 0; j < 8; j++)
				{
					exp->WriteLineBegin();
					exp->Write8BitsData(chunkList[i].Pattern[j]);
					exp->WriteLineEnd();
				}
			}
		}
		patternsSize = exp->GetTotalBytes() - namesSize;
		exp->WriteTableEnd(MSX::Format("Patterns size: %i Bytes", patternsSize));
	}

	exp->WriteLineEnd();
	exp->WriteCommentLine(MSX::Format("Total size: %i Bytes", exp->GetTotalBytes()));

	//-------------------------------------------------------------------------
	// Write file
	bool bSaved = exp->Export();

	return bSaved;
}

//#############################################################################
//
// EXPORT MGLV
// 
//#############################################################################

// MGLV data chunk
struct MGLVChunk
{
	MGLV_COMAND Command;
	std::vector<u8> Value;
	u16 Number;
	u16 Size;

	MGLVChunk() : Command(MGLV_CMD_END_VIDEO), Number(0), Size(0) {}
};

//-----------------------------------------------------------------------------
// Count the number of unchanged digits
u32 MGLVGetSkipCount(const std::vector<u8>& curData, const std::vector<u8>& prevData, u32 it)
{
	u32 i = it;
	while ((i < (u32)curData.size()) && (i < (u32)prevData.size()) && (curData[i] == prevData[i]))
	{
		i++;
	}
	return i - it;
}

//-----------------------------------------------------------------------------
// Count the number of identical digits
u32 MGLVGetFillCount(const std::vector<u8>& curData, u32 it)
{
	u8 val = curData[it];
	u32 i = it;
	while ((i < (u32)curData.size()) && (curData[i] == val))
	{
		i++;
	}
	return i - it;
}

//-----------------------------------------------------------------------------
/** Convert multiple images into MGLV video data */
MGLVChunk MGLVGetNextChunk(ExportParameters* param, const std::vector<u8>& curData, const std::vector<u8>& prevData, u32& it, u32 remain)
{
	MGLVChunk chunk;

	//......................................................................... 
	// Check end of frame
	if (it == (u32)curData.size())
	{
		chunk.Command = MGLV_CMD_SKIP_FRAME;
		chunk.Size = 1; // cmd
		return chunk;
	}
		
	//......................................................................... 
	// Check skip chunks
	u32 skip = MGLVGetSkipCount(curData, prevData, it);
	if (skip >= param->mglvConfig.minSkip)
	{
		chunk.Number = skip;
		if (skip < 16)
		{
			chunk.Command = MGLV_CMD_SKIP_4B;
			chunk.Size = 1; // cmd|n
		}
		else if (skip < 256)
		{
			chunk.Command = MGLV_CMD_SKIP_8B;
			chunk.Size = 1 + 1; // cmd + nn
		}
		else if (skip != curData.size())
		{
			chunk.Command = MGLV_CMD_SKIP_16B;
			chunk.Size = 1 + 2; // cmd + nnnn
		}
		else
		{
			chunk.Command = MGLV_CMD_SKIP_FRAME;
			chunk.Size = 1; // cmd
		}

		// Check that chunk don't overflow the current segment
		if (chunk.Size < remain)
		{
			it += skip;
			return chunk;
		}
	}

	//......................................................................... 
	// Check fill chunks
	u32 fill = MGLVGetFillCount(curData, it);
	if (fill >= param->mglvConfig.minFill)
	{
		chunk.Value.resize(1);
		chunk.Value[0] = curData[it];
		chunk.Number = fill;

		if (fill < 16)
		{
			chunk.Command = MGLV_CMD_FILL_4B;
			chunk.Size = 1 + 1; // cmd|n + val
		}
		else if (fill < 256)
		{
			chunk.Command = MGLV_CMD_FILL_8B;
			chunk.Size = 1 + 1 + 1; // cmd + nn + val
		}
		else if (fill != curData.size())
		{
			chunk.Command = MGLV_CMD_FILL_16B;
			chunk.Size = 1 + 2 + 1; // cmd + nnnn + val
		}
		else
		{
			chunk.Command = MGLV_CMD_FILL_FRAME;
			chunk.Size = 1 + 1; // cmd + val
		}

		// Check that chunk don't overflow the current segment
		if (chunk.Size < remain)
		{
			it += fill;
			return chunk;
		}
	}

	//......................................................................... 
	// Check end of segment
	if (remain <= 2)
	{
		chunk.Command = MGLV_CMD_END_SEGMENT;
		chunk.Size = 1; // cmd
		return chunk;
	}

	//......................................................................... 
	// Check copy chunks
	std::vector<u8> data;
	for (u32 i = it; i < (u32)curData.size(); i++)
	{
		if ((i - it) >= remain - 3)
			break;

		if (MGLVGetSkipCount(curData, prevData, i) >= param->mglvConfig.minSkip)
			break;

		if (MGLVGetFillCount(curData, i) >= param->mglvConfig.minFill)
			break;

		data.push_back(curData[i]);
	}
	u32 copy = (u32)data.size();
	
	chunk.Number = copy;
	chunk.Value = data;

	if (copy < 16)
	{
		chunk.Command = MGLV_CMD_COPY_4B;
		chunk.Size = 1 + copy; // cmd|n + val[n]
	}
	else if (copy < 256)
	{
		chunk.Command = MGLV_CMD_COPY_8B;
		chunk.Size = 1 + 1 + copy; // cmd + nn + val[nn]
	}
	else if (copy != curData.size())
	{
		chunk.Command = MGLV_CMD_COPY_16B;
		chunk.Size = 1 + 2 + copy; // cmd + nnnn + val[nnnn]
	}
	else
	{
		chunk.Command = MGLV_CMD_COPY_FRAME;
		chunk.Size = 1 + copy; // cmd + val[]
	}

	it += (u32)data.size();
	return chunk;
}

//-----------------------------------------------------------------------------
// Convert multiple images into MGLV video data
bool ExportMGLV(ExportParameters* param, ExporterInterface* exp)
{
	// Individual image exporter parameters
	ExportParameters imgParam = *param;
	imgParam.mode = MODE_Bitmap;

	u32 segIdx = 0; // current segment index
	u32 segRemain = param->mglvConfig.segmentSize * 1024; // current segment index

	exp->WriteTableBegin(TABLE_U8, param->tabName, "MGLV commands buffer");

	// Write header
	MGLV_Header head;
	memcpy(&head.Sign, "MGV", 3);
	head.Flag = MGLV_VERSION;
	if (param->mglvConfig.segmentSize == 16)
		head.Flag |= MGLV_SEGMENT_16K;
	head.Image = MGLV_SCR_MODE_BITMAP | MGLV_SCR_WIDTH_256 | MGLV_SCR_BPP_4;
	head.Replay = param->mglvConfig.frameSkip;
	if (param->mglvConfig.isLooping)
		head.Replay |= MGLV_LOOP;
	if (param->mglvConfig.freq == 60)
		head.Replay |= MGLV_FREQ_60HZ;
	head.Width = (head.Image & MGLV_SCR_WIDTH_512) ? param->mglvConfig.width / 2 - 1 : param->mglvConfig.width - 1;
	head.Height = (u8)param->mglvConfig.height;
	switch (param->mglvConfig.headerMode)
	{
	case MGLV_HEADER_NONE:
		break;
	case MGLV_HEADER_SHORT:
		exp->WriteBytesList(std::vector<u8>({ 'M', 'G', 'V' }), "Signature");
		exp->Write1ByteLine(head.Flag, "Flag");
		segRemain -= 4;
		break;
	case MGLV_HEADER_FULL:
		exp->WriteBytesList(std::vector<u8>({ 'M', 'G', 'V' }), "Signature");
		exp->Write1ByteLine(head.Flag | MGLV_IMAGE_HEADER, "Flag");
		exp->Write1ByteLine(head.Image, "Image");
		exp->Write1ByteLine(head.Replay, "Replay");
		exp->Write1ByteLine(head.Width, "Width");
		exp->Write1ByteLine(head.Height, "Height");
		segRemain -= sizeof(MGLV_Header);
		break;
	};

	// Parse all video files
	bool bStart = false;
	u32 i = 0;
	std::vector<u8> prevData;
	while (true) // Parse all images
	{
		// Check if file exists
		std::string filename = MSX::Format("%s/%04d.png", param->inFile.c_str(), i);
		if (MSX::File::Exists(filename))
		{
			printf("Converting %s...\n", filename.c_str());
			exp->WriteCommentLine(MSX::Format("Image %i", i));

			if (!bStart) // First image
			{
				bStart = true;
			}
			else // 
			{
			}

			// Get data from source image
			imgParam.inFile = filename;
			ExporterBinNoSave curImg(param->format, &imgParam); // convert image to binary
			ExportBitmap(&imgParam, &curImg);
			std::vector<u8> curData = curImg.GetData();

			u32 imgIt = 0;

			// Export image to video data
			while (1)
			{
				MGLVChunk chunk = MGLVGetNextChunk(param, curData, prevData, imgIt, segRemain);
				switch (chunk.Command)
				{
				//---- Events ----//
				case MGLV_CMD_END_VIDEO: // End of data
					exp->Write1ByteLine(chunk.Command, "END_VIDEO");
					break;
				case MGLV_CMD_END_SEGMENT: // End of segment(increment segment index and reset data pointer)
					exp->Write1ByteLine(chunk.Command, "END_SEGMENT");
					break;
				case MGLV_CMD_END_LINE: // End of line
					exp->Write1ByteLine(chunk.Command, "END_LINE");
					break;
				case MGLV_CMD_END_TABLE: // End table and switch to next one
					exp->Write1ByteLine(chunk.Command + (u8)(chunk.Value[0] << 4), "END_TABLE");
					break;
				//---- Skip ----//
				case MGLV_CMD_SKIP_4B: // Skip n bytes(1 - 15)
					exp->Write1ByteLine(chunk.Command + (u8)(chunk.Number << 4), MSX::Format("SKIP_4B (%i)", chunk.Number));
					break;
				case MGLV_CMD_SKIP_8B: // Skip nn bytes(1 - 255)
					exp->Write1ByteLine(chunk.Command, MSX::Format("SKIP_8B (%i)", chunk.Number));
					exp->Write1ByteLine((u8)chunk.Number);
					break;
				case MGLV_CMD_SKIP_16B: // Skip nnnn bytes(1 - 65535)
					exp->Write1ByteLine(chunk.Command, MSX::Format("SKIP_16B (%i)", chunk.Number));
					exp->Write1WordLine((u16)chunk.Number);
					break;
				case MGLV_CMD_SKIP_FRAME: // Skip a frame / End of frame
					exp->Write1ByteLine(chunk.Command, "SKIP_FRAME");
					break;
				//---- Fill ----//
				case MGLV_CMD_FILL_4B: // Fill n bytes(1 - 15) with vv value
					exp->Write1ByteLine(chunk.Command + (u8)(chunk.Number << 4), MSX::Format("FILL_4B (%i)", chunk.Number));
					exp->Write1ByteLine((u8)chunk.Value[0]);
					break;
				case MGLV_CMD_FILL_8B: // Fill nn bytes(1 - 255) with vv value
					exp->Write1ByteLine(chunk.Command, MSX::Format("FILL_8B (%i)", chunk.Number));
					exp->Write1ByteLine((u8)chunk.Number);
					exp->Write1ByteLine((u8)chunk.Value[0]);
					break;
				case MGLV_CMD_FILL_16B: // Fill nnnn bytes(1 - 65535) with vv value
					exp->Write1ByteLine(chunk.Command, MSX::Format("FILL_16B (%i)", chunk.Number));
					exp->Write1WordLine((u16)chunk.Number);
					exp->Write1ByteLine((u8)chunk.Value[0]);
					break;
				case MGLV_CMD_FILL_FRAME: // Fill a full frame with vv value
					exp->Write1ByteLine(chunk.Command, "FILL_FRAME");
					exp->Write1ByteLine((u8)chunk.Value[0]);
					break;
				//---- Copy ----//
				case MGLV_CMD_COPY_4B: // Copy n bytes(1 - 15) from vv[n] data table
					exp->Write1ByteLine(chunk.Command + (u8)(chunk.Number << 4), MSX::Format("COPY_4B (%i)", chunk.Number));
					exp->WriteBytesList(chunk.Value);
					break;
				case MGLV_CMD_COPY_8B: // Copy nn bytes(1 - 255) from vv[nn] data table
					exp->Write1ByteLine(chunk.Command, MSX::Format("COPY_8B (%i)", chunk.Number));
					exp->Write1ByteLine((u8)chunk.Number);
					exp->WriteBytesList(chunk.Value);
					break;
				case MGLV_CMD_COPY_16B: // Copy nnnn bytes(1 - 65535) from vv[nnnn] data table
					exp->Write1ByteLine(chunk.Command, MSX::Format("COPY_16B (%i)", chunk.Number));
					exp->Write1WordLine((u16)chunk.Number);
					exp->WriteBytesList(chunk.Value);
					break;
				case MGLV_CMD_COPY_FRAME: // Copy a full frame from data table (raw frame)
					exp->Write1ByteLine(chunk.Command, "COPY_FRAME");
					exp->WriteBytesList(chunk.Value);
					break;
				}

				segRemain -= chunk.Size;
				assert(segRemain >= 0);
				if (segRemain == 0)
				{
					segRemain = param->mglvConfig.segmentSize * 1024;
					segIdx++;
					exp->WriteCommentLine(MSX::Format("Switch to segment +%i (%i)", segIdx, exp->GetTotalBytes()));
				}

				if (chunk.Command == MGLV_CMD_END_FRAME)
					break;
			}
			prevData = curImg.GetData();
		}
		else
		{
			if (bStart) // Reached last file
			{
				break;
			}
		}
		i++;
	}

	exp->Write1ByteLine(MGLV_CMD_END_VIDEO, "END_VIDEO");
	exp->WriteTableEnd();

	bool bSaved = exp->Export();
	return bSaved;
}

//-----------------------------------------------------------------------------
// PARSE IMAGE
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Invoke the selected mode's exporter
bool ParseImage(ExportParameters* param, ExporterInterface* exp)
{
	switch (param->mode)
	{
	default:
	case MODE_Bitmap:	return ExportBitmap(param, exp);
	case MODE_GM1:		return ExportGM1(param, exp);
	case MODE_GM2:		return ExportGM2(param, exp);
	case MODE_Sprite:	return ExportSprite(param, exp);
	case MODE_Text:		return ExportText(param, exp);
	case MODE_MGLV:		return ExportMGLV(param, exp);
	};
}
