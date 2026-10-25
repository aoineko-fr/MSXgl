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
void ExportSpriteData(ExportParameters* param, ExporterInterface* exp, Layer& layer, i32 sid, i32 x, i32 y, BYTE* rawData, i32 imageX, i32 imageY, std::vector<u8> &expData)
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
					u32 c24 = 0xFFFFFF & ((u32*)rawData)[idx];
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
			expData.push_back(byte);
		}
	}
}

//-----------------------------------------------------------------------------
// Export sprite sheet
bool ExportSprite(ExportParameters* param, ExporterInterface* exp)
{
	FIBITMAP *dib32;
	u32 sid = 0; // sprite id
	std::vector<u8> expData;

	//-------------------------------------------------------------------------
	// Prepare image

	dib32 = GetPreparedImage(param->inFile.c_str(), param); // open and load the file using the default load option
	if (dib32 == NULL)
	{
		printf("Error: Fail to prepare 32-bit image\n");
		return false;
	}

	// Get 32-bit raw datas
	i32 imageX = FreeImage_GetWidth(dib32);
	i32 imageY = FreeImage_GetHeight(dib32);
	i32 scanWidth = FreeImage_GetPitch(dib32);
	std::vector<BYTE> rawData((size_t)scanWidth * imageY);
	FreeImage_ConvertToRawBits(rawData.data(), dib32, scanWidth, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
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
							ExportSpriteData(param, exp, layer, sid++, x, y, rawData.data(), imageX, imageY, expData);
							y += 8;
							ExportSpriteData(param, exp, layer, sid++, x, y, rawData.data(), imageX, imageY, expData);
							y -= 8;
							x += 8;
							ExportSpriteData(param, exp, layer, sid++, x, y, rawData.data(), imageX, imageY, expData);
							y += 8;
							ExportSpriteData(param, exp, layer, sid++, x, y, rawData.data(), imageX, imageY, expData);
						}
						else // if (layer.mode & LAYER_8x8)
						{
							i32 x = param->posX + (nx * (param->sizeX + param->gapX)) + layer.posX + i * 8;
							i32 y = param->posY + (ny * (param->sizeY + param->gapY)) + layer.posY + j * 8;
							ExportSpriteData(param, exp, layer, sid++, x, y, rawData.data(), imageX, imageY, expData);
						}
					}
				}

			}
		}
	}

	// Export compressed data
	if (param->comp == MSX::COMPRESS_RLEp)
		ExportRLEp(param, exp, expData);
	else if (param->comp == MSX::COMPRESS_Pletter)
		ExportPletter(param, exp, expData);

	i32 namesSize = exp->GetTotalBytes();
	exp->WriteTableEnd(MSX::Format("Patterns size: %i Bytes", namesSize));

	//-------------------------------------------------------------------------
	// Write file
	bool bSaved = exp->Export();

	return bSaved;
}
