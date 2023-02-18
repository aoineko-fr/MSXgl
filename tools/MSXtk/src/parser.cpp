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
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
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

/***/
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

/***/
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

///
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

///
void ExportPletter(ExportParameters* param, ExporterInterface* exp, const std::vector<u8>& data)
{
	std::vector<u8> out;

	Pletter::Export(data, out);

	ExportTable(param, exp, out, 16);
}

///
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
// EXPORT BITMAP
//-----------------------------------------------------------------------------

/***/
bool ExportBitmap(ExportParameters * param, ExporterInterface * exp)
{
	FIBITMAP *dib, *dib32;
	i32 i, j, nx, ny, bit, minX, maxX, minY, maxY;
	RGB24 c24;
	GRB8 c8;
	u8 c2, c4, byte = 0;
	char strData[BUFFER_SIZE];
	u32 transRGB = 0x00FFFFFF & param->transColor;
	u32 headAddr = 0, palAddr = 0;
	std::vector<u16> sprtAddr;

	dib = LoadImage(param->inFile.c_str()); // open and load the file using the default load option
	if (dib == NULL)
	{
		printf("Error: Fail to load %s\n", param->inFile.c_str());
		return false;
	}

	// Get 32 bits version
	dib32 = FreeImage_ConvertTo32Bits(dib);
	FreeImage_Unload(dib); // free the original dib
	i32 imageX = FreeImage_GetWidth(dib32);
	i32 imageY = FreeImage_GetHeight(dib32);
	i32 scanWidth = FreeImage_GetPitch(dib32);
	i32 bpp = FreeImage_GetBPP(dib32);
	BYTE* bits = new BYTE[scanWidth * imageY];
	FreeImage_ConvertToRawBits(bits, dib32, scanWidth, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);

	// Get custom palette for 16 colors mode
	u32 customPalette[16];
	/*RGBQUAD defaultPal[3] = {
		{ 0x00, 0x00, 0x00, 0 },
		{ 0x80, 0x80, 0x80, 0 },
		{ 0xFF, 0xFF, 0xFF, 0 },
	};*/
	if ((param->bpc == 4) && (param->palType == PALETTE_Custom))
	{
		if (param->bUseTrans)
		{
			u32 black = 0;
			i32 res = FreeImage_ApplyColorMapping(dib32, (RGBQUAD*)&transRGB, (RGBQUAD*)&black, 1, true, false); // @warning: must be call AFTER retreving raw data!
		}
		FIBITMAP* dib4 = FreeImage_ColorQuantizeEx(dib32, FIQ_LFPQUANT, param->palCount, 0, NULL /*3, defaultPal*/); // Try Lossless Fast Pseudo-Quantization algorithm (if there are 15 colors or less)
		if(dib4 == NULL)
			dib4 = FreeImage_ColorQuantizeEx(dib32, FIQ_WUQUANT, param->palCount, 0, NULL /*3, defaultPal*/); // Else, use Efficient Statistical Computations for Optimal Color Quantization
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
		FIBITMAP* dib2 = FreeImage_ColorQuantizeEx(dib32, FIQ_LFPQUANT, param->palCount, 0, NULL /*3, defaultPal*/); // Try Lossless Fast Pseudo-Quantization algorithm (if there are 3 colors or less)
		if (dib2 == NULL)
			dib2 = FreeImage_ColorQuantizeEx(dib32, FIQ_WUQUANT, param->palCount, 0, NULL /*3, defaultPal*/); // Else, use Efficient Statistical Computations for Optimal Color Quantization
		RGBQUAD* pal = FreeImage_GetPalette(dib2);
		for (i32 c = 0; c < param->palOffset; c++)
			customPalette[c] = 0;
		for (i32 c = 0; c < param->palCount; c++)
			customPalette[c + param->palOffset] = ((u32*)pal)[c];
		FreeImage_Unload(dib2);
	}
	// Apply dithering for 2 color mode
	else if ((param->bpc == 1) && (param->dither != DITHER_None))
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
		sprintf_s(strData, BUFFER_SIZE, "%s_header", param->tabName.c_str());
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
		sprintf_s(strData, BUFFER_SIZE, "First character ASCII code (%c)", param->fontFirst);
		exp->Write1ByteLine((u8)param->fontFirst, strData);
		sprintf_s(strData, BUFFER_SIZE, "Last character ASCII code (%c)", param->fontLast);
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
								if (param->bpc == 8) // 8-bits GBR color
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
	sprintf_s(strData, BUFFER_SIZE, "Total size : % i bytes", exp->GetTotalBytes());
	exp->WriteTableEnd(strData);

	delete[] bits;

	//-------------------------------------------------------------------------
	// INDEX TABLE

	if (param->bAddIndex)
	{
		sprintf_s(strData, BUFFER_SIZE, "%s_index", param->tabName.c_str());
		exp->WriteTableBegin(TABLE_U16, strData, "Images index");
		for (i32 i = 0; i < (i32)sprtAddr.size(); i++)
		{
			exp->Write1WordLine(sprtAddr[i], "");
		}
		exp->WriteTableEnd("");
	}

	//-------------------------------------------------------------------------
	// PALETTE TABLE

	if (((param->bpc == 2) || (param->bpc == 4)) && (param->palType == PALETTE_Custom))
	{
		sprintf_s(strData, BUFFER_SIZE, "%s_palette", param->tabName.c_str());
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
				sprintf_s(strData, BUFFER_SIZE, "[%2i] #%06X", i, customPalette[i]);
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
				sprintf_s(strData, BUFFER_SIZE, "[%2i] #%06X", i, customPalette[i]);
				exp->Write2BytesLine(u8(c1), u8(c2), strData);
			}
		}
		exp->WriteTableEnd("");
	}

	// Write file
	bool bSaved = exp->Export();

	return bSaved;
}

/** Build 256 colors palette */
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
	fopen_s(&file, filename, "wb");
	fwrite(ColorTable, sizeof(ColorTable), 1, file);
	fclose(file);
}

/** Build 16 colors palette */
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
	fopen_s(&file, filename, "wb");
	fwrite(ColorTable, sizeof(ColorTable), 1, file);
	fclose(file);
}

//-----------------------------------------------------------------------------
// EXPORT GRAPHIC 1
//-----------------------------------------------------------------------------

namespace GM1 {

///
struct Chunk
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

///
struct Group
{
	u8 Color0;
	u8 Color1 = 0xFF;
	std::vector<Chunk> Chunks;

	bool IsPlain() { return Color1 == 0xFF; }
};

///
u8 GetChunkId(std::vector<Group>& group, Chunk& chunk, ExportParameters* param)
{
	// Force color0 to be smaller than color1
	if (chunk.Color0 > chunk.Color1)
		chunk.Invert();

	// Search for existing chunk index
	for (u8 i = 0; i < group.size(); i++)
	{
		for (u8 j = 0; j < group[i].Chunks.size(); j++)
		{
			if(chunk.IsPlain() && group[i].Chunks[j].IsPlain() && (chunk.Color0 == group[i].Chunks[j].Color0)) // Don't check pattern
				return (i * 8) + j;

			if (memcmp(&group[i].Chunks[j], &chunk, sizeof(Chunk)) == 0)
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

	// Check for free group
	if (group.size() >= 32)
	{
		printf("Error: image have more than 32 color groups!\n");
		exit(1);
	}

	// Create new group
	Group grp;
	grp.Color0 = chunk.Color0;
	grp.Color1 = chunk.Color1;
	grp.Chunks.push_back(chunk);
	group.push_back(grp);
	return static_cast<u8>((group.size() - 1) * 8);
}

/***/
bool Export(ExportParameters* param, ExporterInterface* exp)
{
	std::vector<Group> chunkList;
	FIBITMAP* dib, * dib32;

	//-------------------------------------------------------------------------
	// Prepare image

	dib = LoadImage(param->inFile.c_str()); // open and load the file using the default load option
	if (dib == NULL)
	{
		printf("Error: Fail to load %s\n", param->inFile.c_str());
		return false;
	}

	// Get 32 bits raw datas
	dib32 = FreeImage_ConvertTo32Bits(dib);
	FreeImage_Unload(dib); // free the original dib
	i32 imageX = FreeImage_GetWidth(dib32);
	i32 imageY = FreeImage_GetHeight(dib32);
	i32 scanWidth = FreeImage_GetPitch(dib32);
	i32 bpp = FreeImage_GetBPP(dib32);
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

	// File header
	exp->WriteHeader();

	for (u32 l = 0; l < param->layers.size(); l++)
	{
		Layer* layer = &param->layers[l];

		u32 numX = layer->numX / 8;
		u32 numY = layer->numY / 8;

		// Parse image
		std::vector<u8> layoutBytes;
		for (u32 ny = 0; ny < numY; ny++)
		{
			for (u32 nx = 0; nx < numX; nx++)
			{
				Chunk chunk;

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
				u8 patIdx = GetChunkId(chunkList, chunk, param);
				layoutBytes.push_back(patIdx + param->offset);
			}
		}

		//-------------------------------------------------------------------------
		// LAYOUT TABLE

		if (!param->bGM2Unique)
		{
			if (param->layers.size() == 1) // Default
				exp->WriteTableBegin(TABLE_U8, param->tabName + "_Names", "Names Table");
			else
				exp->WriteTableBegin(TABLE_U8, MSX::Format("%sL%i_Names", param->tabName.c_str(), l), "Names Table");

			if (param->bGM2CompressNames && param->comp == MSX::COMPRESS_RLEp)
				ExportRLEp(param, exp, layoutBytes);
			else if (param->bGM2CompressNames && param->comp == MSX::COMPRESS_Pletter)
				ExportPletter(param, exp, layoutBytes);
			else
				ExportTable(param, exp, layoutBytes, numX);

			exp->WriteTableEnd("");
		}
	}
	i32 namesSize = exp->GetTotalBytes();
	exp->WriteCommentLine(MSX::Format("Names size: %i Bytes", namesSize));

	delete[] bits;

	//-------------------------------------------------------------------------
	// PATTERNS TABLE

	exp->WriteTableBegin(TABLE_U8, param->tabName + "_Patterns", "Patterns Table");
	if (param->comp != MSX::COMPRESS_None)
	{
		// Build data
		std::vector<u8> bytes;
		for (i32 i = 0; i < (i32)chunkList.size(); i++)
		{
			for (i32 j = 0; j < 8; j++)
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
		for (i32 i = 0; i < (i32)chunkList.size(); i++) // group
		{
			for (i32 j = 0; j < 8; j++) // chunk
			{
				if ((i == chunkList.size() - 1) && (j >= chunkList[i].Chunks.size()))
					continue;

				// Print sprite header
				exp->WriteSpriteHeader(i * 8 + j + param->offset);
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
	i32 patternsSize = exp->GetTotalBytes() - namesSize;
	exp->WriteTableEnd(MSX::Format("Patterns size: %i Bytes", patternsSize));

	//-------------------------------------------------------------------------
	// COLORS TABLE

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
	exp->WriteLineEnd();
	exp->WriteCommentLine(MSX::Format("Total size: %i Bytes", exp->GetTotalBytes()));

	//-------------------------------------------------------------------------
	// Write file
	bool bSaved = exp->Export();

	return bSaved;
}

}; // namespace GM1

//-----------------------------------------------------------------------------
// EXPORT GRAPHIC 2
//-----------------------------------------------------------------------------

///
struct ChunkGM2
{
	u8 Pattern[8];
	u8 Color[8];
};

///
u8 GetChunkId(std::vector<ChunkGM2>& list, const ChunkGM2& chunk, ExportParameters* param)
{
	if (!param->bGM2Unique)
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

/***/
bool ExportGM2(ExportParameters* param, ExporterInterface* exp)
{
	std::vector<ChunkGM2> chunkList;
	FIBITMAP* dib, * dib32;

	//-------------------------------------------------------------------------
	// Prepare image

	dib = LoadImage(param->inFile.c_str()); // open and load the file using the default load option
	if (dib == NULL)
	{
		printf("Error: Fail to load %s\n", param->inFile.c_str());
		return false;
	}

	// Get 32 bits raw datas
	dib32 = FreeImage_ConvertTo32Bits(dib);
	FreeImage_Unload(dib); // free the original dib
	i32 imageX = FreeImage_GetWidth(dib32);
	i32 imageY = FreeImage_GetHeight(dib32);
	i32 scanWidth = FreeImage_GetPitch(dib32);
	i32 bpp = FreeImage_GetBPP(dib32);
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

	// File header
	exp->WriteHeader();

	for (u32 l = 0; l < param->layers.size(); l++)
	{
		Layer* layer = &param->layers[l];

		u32 numX = layer->numX / 8;
		u32 numY = layer->numY / 8;

		// Parse image
		std::vector<u8> layoutBytes;
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

				u8 patIdx = GetChunkId(chunkList, chunk, param);
				layoutBytes.push_back(patIdx + param->offset);
			}
		}

		//-------------------------------------------------------------------------
		// LAYOUT TABLE

		if (!param->bGM2Unique)
		{
			if (param->layers.size() == 1) // Default
				exp->WriteTableBegin(TABLE_U8, param->tabName + "_Names", "Names Table");
			else
				exp->WriteTableBegin(TABLE_U8, MSX::Format("%sL%i_Names", param->tabName.c_str(), l), "Names Table");

			if (param->bGM2CompressNames && param->comp == MSX::COMPRESS_RLEp)
				ExportRLEp(param, exp, layoutBytes);
			else if (param->bGM2CompressNames && param->comp == MSX::COMPRESS_Pletter)
				ExportPletter(param, exp, layoutBytes);
			else
				ExportTable(param, exp, layoutBytes, numX);

			exp->WriteTableEnd("");
		}
	}
	i32 namesSize = exp->GetTotalBytes();
	exp->WriteCommentLine(MSX::Format("Names size: %i Bytes", namesSize));

	delete[] bits;

	//-------------------------------------------------------------------------
	// PATTERNS TABLE

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
	i32 patternsSize = exp->GetTotalBytes() - namesSize;
	exp->WriteTableEnd(MSX::Format("Patterns size: %i Bytes", patternsSize));

	//-------------------------------------------------------------------------
	// COLORS TABLE

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
	exp->WriteLineEnd();
	exp->WriteCommentLine(MSX::Format("Total size: %i Bytes", exp->GetTotalBytes()));

	//-------------------------------------------------------------------------
	// Write file
	bool bSaved = exp->Export();

	return bSaved;
}

//-----------------------------------------------------------------------------
// EXPORT SPRITES
//-----------------------------------------------------------------------------

/// Convert color to binary value according to layer configuration
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

/// Export a 8x8 sprite data (1-bit per point)
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

/***/
bool ExportSprite(ExportParameters* param, ExporterInterface* exp)
{
	FIBITMAP* dib, * dib32;
	u32 sid = 0; // sprite id
	std::vector<u8> rawData;

	//-------------------------------------------------------------------------
	// Prepare image

	dib = LoadImage(param->inFile.c_str()); // open and load the file using the default load option
	if (dib == NULL)
	{
		printf("Error: Fail to load %s\n", param->inFile.c_str());
		return false;
	}

	// Get 32 bits raw datas
	dib32 = FreeImage_ConvertTo32Bits(dib);
	FreeImage_Unload(dib); // free the original dib
	i32 imageX = FreeImage_GetWidth(dib32);
	i32 imageY = FreeImage_GetHeight(dib32);
	i32 scanWidth = FreeImage_GetPitch(dib32);
	i32 bpp = FreeImage_GetBPP(dib32);
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
			if (param->comp != MSX::COMPRESS_RLEp)
				exp->WriteCommentLine(MSX::Format("======== Frame[%i]", nx + ny * param->numX));

			for (i32 l = 0; l < (i32)param->layers.size(); l++)
			{
				Layer& layer = param->layers[l];

				if (param->comp != MSX::COMPRESS_RLEp)
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

//-----------------------------------------------------------------------------
// PARSE IMAGE
//-----------------------------------------------------------------------------

/***/
bool ParseImage(ExportParameters* param, ExporterInterface* exp)
{
	switch (param->mode)
	{
	default:
	case MODE_Bitmap:	return ExportBitmap(param, exp);
	case MODE_GM1:		return GM1::Export(param, exp);
	case MODE_GM2:		return ExportGM2(param, exp);
	case MODE_Sprite:	return ExportSprite(param, exp);
	};
}