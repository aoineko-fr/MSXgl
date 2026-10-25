//_____________________________________________________________________________
//   ▄ ▄  ▄▄▄ ▄▄ ▄ ▄                                                      
//  ██▀█ ▀█▄  ▀█▄▀ ▄  ▄█▄█ ▄▀██                                           
//  ██ █ ▄▄█▀ ██ █ ██ ██ █  ▀██                                           
//__________________________▀▀_________________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/MSXimg)
// under CC BY-SA license (https://creativecommons.org/licenses/by-sa/2.0/)

// std
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <array>

// MSXi
#include "parser.h"

struct RLEHash
{
	i32 length;
	u32 color;
	std::vector<u32> data;

	RLEHash() : length(0), color(0){}
};

//#############################################################################
//
// EXPORT BITMAP
// 
//#############################################################################

//-----------------------------------------------------------------------------
// 
u32 ColorRGB(u32 color, ExportParameters* param)
{
	if (!param->pal24)
	{
		color &= 0b00000000'11100000'11100000'11100000;
		color |= 0b00000000'00010000'00010000'00010000;
	}
	return color;
}

//-----------------------------------------------------------------------------
// 
bool IsColorInPalette(u32 col, u32* pal, i32 size)
{
	for (i32 i = 0; i < size; i++)
		if (pal[i] == col)
			return true;
	return false;
}

//-----------------------------------------------------------------------------
// 
void FillPalette(RGBQUAD* inPal, u32* outPal, ExportParameters* param)
{
	if (param->palType == PALETTE_Partial)
	{
		i32 idx = 0;
		for (i32 c = 0; c < param->palOffset; c++)
			outPal[idx++] = 0;
		for (i32 c = 0; c < param->palInCount; c++)
			outPal[idx++] = ColorRGB(param->palInput[c], param);
		for (i32 c = 0; c < param->palOutCount; c++)
		{
			u32 col = ((u32*)inPal)[c];
			if (!IsColorInPalette(col, outPal, idx))
				outPal[idx++] = col;
		}
	}
	else // if (param->palType == PALETTE_Custom)
	{
		i32 idx = 0;
		for (i32 c = 0; c < param->palOffset; c++)
			outPal[idx++] = 0;
		for (i32 c = 0; c < param->palOutCount; c++)
			outPal[idx++] = ((u32*)inPal)[c];
	}
}

//-----------------------------------------------------------------------------
//
u8 ConvertComponent(u8 val, const PaletteMod* mod)
{
	f32 c = 127.5f + ((f32)val - 127.5f) * ((f32)mod->contrast / 100.f);
	c = (u8)(c * ((f32)mod->scale / 100.f));
	if (c < 0.f)
		return 0;
	if (c > 255.f)
		return 255;
	return (u8)c;
}

//-----------------------------------------------------------------------------
//
void ExportPalette(ExportParameters* param, ExporterInterface* exp, const PaletteMod* mod)
{
	char strData[BUFFER_SIZE];
	if (mod)
		sprintf(strData, "%s_palette_mod%i", param->tabName.c_str(), mod->id);
	else
		sprintf(strData, "%s_palette", param->tabName.c_str());
	if (param->pal24)
	{
		exp->WriteTableBegin(TABLE_U8, strData, "Custom palette | Format: [x:3|R:5] [x:3|G:5] [x:3|B:5] (v9990)");
		if (mod)
			exp->WriteCommentLine(MSX::Format("Palette modifier: scale=%i, contrast=%i", mod->scale, mod->contrast));
		for (i32 i = param->palOffset; i < param->palOffset + param->palOutCount; i++)
		{
			RGB24 color(CustomPalette[i]);
			if (mod)
			{
				color.R = ConvertComponent(color.R, mod);
				color.G = ConvertComponent(color.G, mod);
				color.B = ConvertComponent(color.B, mod);
			}
			exp->WriteLineBegin();
			exp->Write1ByteData(color.R >> 3);
			exp->Write1ByteData(color.G >> 3);
			exp->Write1ByteData(color.B >> 3);
			sprintf(strData, "[%2i] #%06X", i, CustomPalette[i]);
			exp->WriteCommentLine(strData);
		}
	}
	else
	{
		exp->WriteTableBegin(TABLE_U8, strData, "Custom palette | Format: [x|R:3|x|B:3] [x:5|G:3] (v9938)");
		if (mod)
			exp->WriteCommentLine(MSX::Format("Palette modifier: scale=%i, contrast=%i", mod->scale, mod->contrast));
		for (i32 i = param->palOffset; i < param->palOffset + param->palOutCount; i++)
		{
			RGB24 color(CustomPalette[i]);
			if (mod)
			{
				color.R = ConvertComponent(color.R, mod);
				color.G = ConvertComponent(color.G, mod);
				color.B = ConvertComponent(color.B, mod);
			}
			u8 c1 = ((color.R >> 5) << 4) + (color.B >> 5);
			u8 c2 = (color.G >> 5);
			sprintf(strData, "[%2i] #%06X", i, CustomPalette[i]);
			exp->Write2BytesLine(u8(c1), u8(c2), strData);
		}
	}
	exp->WriteTableEnd("");
}

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
	u32 transRGB = 0x00FFFFFF & ColorRGB(param->transColor, param);
	std::vector<u16> sprtAddr;

	dib32 = GetPreparedImage(param->inFile.c_str(), param); // open and load the file using the default load option
	if (dib32 == NULL)
	{
		printf("Error: Fail to prepare 32-bit image\n");
		return false;
	}

	// Get 32-bit version
	i32 imageX = FreeImage_GetWidth(dib32);
	i32 imageY = FreeImage_GetHeight(dib32);
	i32 scanWidth = FreeImage_GetPitch(dib32);
	std::vector<BYTE> rawData((size_t)scanWidth * imageY);
	FreeImage_ConvertToRawBits(rawData.data(), dib32, scanWidth, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);

	if (!param->pal24) // convert image for use with 9-bit palette
	{
		std::vector<u32> uniqueColor;
		u32* rgb = (u32*)rawData.data();
		for (i32 i = 0; i < imageX * imageY; i++)
		{
			rgb[i] = ColorRGB(rgb[i], param);
		}
		FIBITMAP* dibReduced = FreeImage_ConvertFromRawBits(rawData.data(), imageX, imageY, scanWidth, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
		if (dibReduced == NULL)
		{
			printf("Error: Fail to convert image colors!\n");
			FreeImage_Unload(dib32);
			return false;
		}
		FreeImage_Unload(dib32);
		dib32 = dibReduced;
		FreeImage_ConvertToRawBits(rawData.data(), dib32, scanWidth, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
	}

	// Get custom palette for 16 colors mode
	if ((param->palType == PALETTE_Input) || (param->palType == PALETTE_Partial))
	{
		for (i32 i = 0; i < param->palInCount; i++)
			CustomPalette[i] = ColorRGB(param->palInput[i], param);
	}
	
	// Custom palette for 2-bit and 4-bit modes
	if (((param->bpc == 2) || (param->bpc == 4)) && ((param->palType == PALETTE_Custom) || (param->palType == PALETTE_Partial)))
	{
		i32 inNum = (param->palType == PALETTE_Partial) ? param->palInCount : 0;
		RGBQUAD* inPal = (param->palType == PALETTE_Partial) ? (RGBQUAD*)CustomPalette : NULL;

		if (param->bUseTrans)
		{
			u32 black = 0;
			FreeImage_ApplyColorMapping(dib32, (RGBQUAD*)&transRGB, (RGBQUAD*)&black, 1, true, false); // @warning: must be call AFTER retreving raw data!
		}
		FIBITMAP* dibPal = FreeImage_ColorQuantizeEx(dib32, FIQ_LFPQUANT, param->palOutCount, inNum, inPal); // Try Lossless Fast Pseudo-Quantization algorithm (if there are 15 colors or less)
		if (dibPal == NULL)
		{
			FIBITMAP* dib24 = FreeImage_ConvertTo24Bits(dib32);
			if (dib24 == NULL)
			{
				printf("Error: Fail to convert image to 24-bit colors for palette generation!\n");
				FreeImage_Unload(dib32);
				return false;
			}
			FreeImage_Unload(dib32);
			dib32 = dib24;
			dibPal = FreeImage_ColorQuantizeEx(dib32, FIQ_NNQUANT, param->palOutCount, inNum, inPal); // Else, use Efficient Statistical Computations for Optimal Color Quantization
		}
		if (dibPal == NULL)
		{
			printf("Error: Fail to quantize image colors!\n");
			FreeImage_Unload(dib32);
			return false;
		}
		RGBQUAD* pal = FreeImage_GetPalette(dibPal);
		FillPalette(pal, CustomPalette, param);
		FreeImage_Unload(dibPal);
	}

	// Apply dithering for 2 color mode
	if ((param->bpc == 1) && (param->dither != DITHER_None))
	{
		FIBITMAP* dib1 = FreeImage_Dither(dib32, (FREE_IMAGE_DITHER)param->dither);
		if (dib1 == NULL)
		{
			printf("Error: Fail to dither image!\n");
			FreeImage_Unload(dib32);
			return false;
		}
		FreeImage_ConvertToRawBits(rawData.data(), dib1, scanWidth, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
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
	i64 right = (i64)param->posX + ((i64)param->numX - 1) * ((i64)param->sizeX + param->gapX) + param->sizeX;
	i64 bottom = (i64)param->posY + ((i64)param->numY - 1) * ((i64)param->sizeY + param->gapY) + param->sizeY;
	if ((param->posX < 0) || (param->posY < 0) || (param->sizeX <= 0) || (param->sizeY <= 0) ||
		(param->numX <= 0) || (param->numY <= 0) || (param->gapX < 0) || (param->gapY < 0) ||
		(right > imageX) || (bottom > imageY))
	{
		printf("Error: Bitmap extraction rectangle is outside the image bounds or has invalid dimensions.\n");
		return false;
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
		u64 payloadSize = 0;
		for (i32 blockY = 0; blockY < param->numY; blockY++)
		{
			for (i32 blockX = 0; blockX < param->numX; blockX++)
			{
				for (i32 row = 0; row < param->sizeY; row++)
				{
					i64 rowStart = param->posX + (i64)blockX * ((i64)param->sizeX + param->gapX);
					i64 rowPixel = rowStart + ((i64)param->posY + row + (i64)blockY * ((i64)param->sizeY + param->gapY)) * imageX;
					i64 rowEnd = rowPixel + param->sizeX - 1;
					if (param->bpc == 1)
						payloadSize += (u64)(rowEnd / 8 - rowPixel / 8 + 1);
					else
						payloadSize += ((u64)param->sizeX * param->bpc + 7) / 8;
				}
			}
		}
		if (param->comp != MSX::COMPRESS_None || param->bSkipEmpty || param->bAddHeader || param->bAddFont)
		{
			printf("Error: BLOAD header requires uncompressed data with no skipped blocks, font header, or data header.\n");
			return false;
		}
		if ((payloadSize == 0) || (payloadSize > 0x10000))
		{
			printf("Error: BLOAD data size must be between 1 and 65536 bytes.\n");
			return false;
		}
		u16 size = (u16)(payloadSize - 1);
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
						u32 rgb = 0xFFFFFF & ((u32*)rawData.data())[pixel];

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
							if (param->bpc == 4) // 4-bit index color palette
							{
								u8 byte;
								for (u32 l = 0; l < hashTable[k].data.size(); l++)
								{
									u32 rgb = hashTable[k].color;
									if (param->bUseTrans)
										c4 = (rgb == transRGB) ? 0x0 : GetNearestColorIndex(rgb, GetPalette(param), param->palOutCount, param->palOffset);
									else
										c4 = GetNearestColorIndex(rgb, GetPalette(param), param->palOutCount, param->palOffset);
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
							else if (param->bpc == 8) // 8-bit GBR color
							{
								for (u32 l = 0; l < hashTable[k].data.size(); l++)
								{
									c8 = GetGBR8(hashTable[k].data[l], param->bUseTrans, transRGB);
									exp->Write1ByteData(c8);
								}
							}
							else if (param->bpc == 16) // 16-bit GBR color
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
					else if (param->comp == MSX::COMPRESS_RLE4) // Full color 4-bit Run-length encoding
					{
						if (param->bpc == 4) // 4-bit index color palette
						{
							u32 rgb = hashTable[k].color;
							if (param->bUseTrans)
								c4 = (rgb == transRGB) ? 0x0 : GetNearestColorIndex(rgb, GetPalette(param), param->palOutCount, param->palOffset);
							else
								c4 = GetNearestColorIndex(rgb, GetPalette(param), param->palOutCount, param->palOffset);
							u8 byte = ((0x0F & hashTable[k].length) << 4) + c4;
							exp->Write1ByteData(byte);
						}
					}
					else if (param->comp == MSX::COMPRESS_RLE8) // Full color 8-bit Run-length encoding
					{
						if (param->bpc == 4) // 4-bit index color palette
						{
							exp->Write1ByteData((u8)hashTable[k].length);
							u32 rgb = hashTable[k].color;
							if (param->bUseTrans)
								c4 = (rgb == transRGB) ? 0x0 : GetNearestColorIndex(rgb, GetPalette(param), param->palOutCount, param->palOffset);
							else
								c4 = GetNearestColorIndex(rgb, GetPalette(param), param->palOutCount, param->palOffset);
							exp->Write1ByteData(c4);
						}
						else if (param->bpc == 8) // 8-bit GBR color
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
							u32 rgb = 0xFFFFFF & ((u32*)rawData.data())[pixel];
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
						else if (param->bpc == 2) // 2-bit index color palette
						{
							minX &= 0xFC;	 // Round down 4
							maxX |= 0x03;	 // Round up 4
						}
						else if (param->bpc == 4) // 4-bit index color palette
						{
							minX &= 0xFE;	 // Round down 2
							maxX |= 0x01;	 // Round up 2
						}

						if (param->comp == MSX::COMPRESS_Crop16)
						{
							minX &= 0x0F;	// Clamp to 4-bit (0-15)
							maxX &= 0x0F;	// Clamp to 4-bit (0-15)
							minY &= 0x0F;	// Clamp to 4-bit (0-15)
							maxY &= 0x0F;	// Clamp to 4-bit (0-15)
							exp->Write2BytesLine(u8((minX << 4) + maxX), u8(((minY) << 4) + maxY), "[minX:4|maxX:4] [minY:4|maxY:4]");
						}
						else if (param->comp == MSX::COMPRESS_CropLine16)
						{
							minY &= 0x0F;	// Clamp to 4-bit (0-15)
							maxY &= 0x0F;	// Clamp to 4-bit (0-15)
							exp->Write1ByteLine(u8((minY << 4) + maxY), "[minY:4|maxY:4]");
						}
						else if (param->comp == MSX::COMPRESS_Crop32)
						{
							minX &= 0x07;	// Clamp to 3-bit (0-7)
							maxX &= 0x1F;	// Clamp to 5-bit (0-31)
							minY &= 0x07;	// Clamp to 3-bit (0-7)
							maxY &= 0x1F;	// Clamp to 5-bit (0-31)
							exp->Write2BytesLine(u8((minX << 5) + maxX), u8(((minY) << 5) + maxY), "[minX:3|maxX:5] [minY:3|maxY:5]");
						}
						else if (param->comp == MSX::COMPRESS_CropLine32)
						{
							minY &= 0x07;	// Clamp to 3-bit (0-7)
							maxY &= 0x1F;	// Clamp to 5-bit (0-31)
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
								u32 rgb = 0xFFFFFF & ((u32*)rawData.data())[pixel];
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
							else if (param->bpc == 2) // 2-bit index color palette
							{
								minX &= 0xFC;	 // Round down 4
								maxX |= 0x03;	 // Round up 4
							}
							else if (param->bpc == 4) // 4-bit index color palette
							{
								minX &= 0xFE;	 // Round down 2
								maxX |= 0x01;	 // Round up 2
							}

							// Add row range info
							if (param->comp == MSX::COMPRESS_CropLine16)
							{
								minX &= 0x0F;	// Clamp to 4-bit (0-15)
								maxX &= 0x0F;	// Clamp to 4-bit (0-15)
								exp->Write1ByteLine(u8((minX << 4) + maxX), "[minX:4|maxX:4]");
							}
							else if (param->comp == MSX::COMPRESS_CropLine32)
							{
								minX &= 0x07;	// Clamp to 3-bit (0-7)
								maxX &= 0x1F;	// Clamp to 5-bit (0-31)
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
								u32 rgb = 0xFFFFFF & ((u32*)rawData.data())[pixel];
								//-----------------------------------------------------------------
								if (param->bpc == 16) // 16-bit GBR color
								{
									// convert to 16-bit GRB
									c16 = GetGBR16(rgb, param->bUseTrans, transRGB);
									exp->Write1ByteData(c16 & 0x00FF);
									exp->Write1ByteData(c16 >> 8);
								}
								//-----------------------------------------------------------------
								else if (param->bpc == 8) // 8-bit GBR color
								{
									// convert to 8-bit GRB
									c8 = GetGBR8(rgb, param->bUseTrans, transRGB);
									exp->Write1ByteData((u8)c8);
								}
								//-----------------------------------------------------------------
								else if (param->bpc == 4) // 4-bit index color palette
								{
									if (param->bUseTrans)
										c4 = (rgb == transRGB) ? 0x0 : GetNearestColorIndex(rgb, GetPalette(param), param->palOutCount, param->palOffset);
									else
										c4 = GetNearestColorIndex(rgb, GetPalette(param), param->palOutCount, param->palOffset);
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
								else if (param->bpc == 2) // 2-bit index color palette
								{
									if (param->bUseTrans)
										c2 = (rgb == transRGB) ? 0x0 : GetNearestColorIndex(rgb, GetPalette(param), param->palOutCount, param->palOffset);
									else
										c2 = GetNearestColorIndex(rgb, GetPalette(param), param->palOutCount, param->palOffset);
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

	if (((param->bpc == 2) || (param->bpc == 4)) && ((param->palType == PALETTE_Custom) || (param->palType == PALETTE_Partial) || (param->palType == PALETTE_Input)))
	{
		ExportPalette(param, exp, NULL);

		for (u32 i = 0; i < param->palMod.size(); i++) // Modified palette
			ExportPalette(param, exp, &param->palMod[i]);
	}

	// Write file
	bool bSaved = exp->Export();

	return bSaved;
}

