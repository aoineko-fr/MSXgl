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
#include <assert.h>
#include <cstdint>
#include <array>
#include <limits>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <memory>

// MSXi
#include "parser.h"

u32 CustomPalette[16];

//-----------------------------------------------------------------------------
// MSX interface
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Get palette
u32* GetPalette(ExportParameters* param)
{
	if (param->palType == PALETTE_MSX1)
		return PaletteMSX1;

	if (param->palType == PALETTE_MSX2)
		return PaletteMSX2;

	return CustomPalette;
}

//-----------------------------------------------------------------------------
// Get nearest color index
u8 GetNearestColorIndex(u32 color, u32* pal, i32 count, i32 offset)
{
	u8 bestIndex = 0;
	i32 bestWeight = 256 * 4;

	RGB24 c = RGB24(color);

	for (i32 i = offset; i < count + offset; i++)
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
FIBITMAP* GetPreparedImage(std::string filename, ExportParameters* param, bool bCrop)
{
	// Open and load the file using the default load option
	std::unique_ptr<FIBITMAP, decltype(&FreeImage_Unload)> dib(LoadImage(param->inFile.c_str()), FreeImage_Unload);
	if (!dib)
	{
		printf("Error: Fail to load %s!\n", param->inFile.c_str());
		return NULL;
	}

	// Apply transformations
	if (param->scaleX && param->scaleY)
	{
		std::unique_ptr<FIBITMAP, decltype(&FreeImage_Unload)> dibScale(
			FreeImage_Rescale(dib.get(), param->scaleX, param->scaleY, (FREE_IMAGE_FILTER)param->scaleFilter), FreeImage_Unload);
		if (!dibScale)
		{
			printf("Error: Fail to rescale image (x:%i, y:%i, filter:%i)!\n", param->scaleX, param->scaleY, param->scaleFilter);
			return NULL;
		}
		dib = std::move(dibScale);
	}
	if (param->flipH)
	{
		if (!FreeImage_FlipHorizontal(dib.get()))
		{
			printf("Error: Fail to apply horizontal flip!\n");
			return NULL;
		}
	}
	if (param->flipV)
	{
		if (!FreeImage_FlipVertical(dib.get()))
		{
			printf("Error: Fail to apply vertical flip!\n");
			return NULL;
		}
	}
	if (param->rotAngle != 0)
	{
		std::unique_ptr<FIBITMAP, decltype(&FreeImage_Unload)> dibRot(
			FreeImage_Rotate(dib.get(), param->rotAngle, NULL), FreeImage_Unload);
		if (!dibRot)
		{
			printf("Error: Fail to rotate image (angle:%i)!\n", (i32)param->rotAngle);
			return NULL;
		}
		dib = std::move(dibRot);
	}
	if (bCrop)
	{
		if ((param->sizeX == 0) || (param->sizeY == 0))
		{
			printf("Error: Can't crop image if size is not defined! Define size using '-size' parameter.\n");
			return NULL;
		}
		i32 imageX = FreeImage_GetWidth(dib.get());
		i32 imageY = FreeImage_GetHeight(dib.get());
		if ((param->posX < 0) || (param->posY < 0) || (param->sizeX < 0) || (param->sizeY < 0) ||
			((i64)param->posX + param->sizeX > imageX) || ((i64)param->posY + param->sizeY > imageY))
		{
			printf("Error: Crop rectangle is outside the image bounds.\n");
			return NULL;
		}
		std::unique_ptr<FIBITMAP, decltype(&FreeImage_Unload)> dibCrop(
			FreeImage_Copy(dib.get(), param->posX, param->posY, param->posX + param->sizeX, param->posY + param->sizeY), FreeImage_Unload);
		if (!dibCrop)
		{
			printf("Error: Fail to crop image!\n");
			return NULL;
		}
		dib = std::move(dibCrop);
	}

	// Convert to 32-bit image
	std::unique_ptr<FIBITMAP, decltype(&FreeImage_Unload)> dib32(FreeImage_ConvertTo32Bits(dib.get()), FreeImage_Unload);
	return dib32.release();
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
	case MODE_GM2i:		return ExportGM2i(param, exp);
	case MODE_Sprite:	return ExportSprite(param, exp);
	case MODE_Text:		return ExportText(param, exp);
	case MODE_MGLV:		return ExportMGLV(param, exp);
	};
}
