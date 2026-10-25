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

	// Get 32-bit raw datas
	i32 imageX = FreeImage_GetWidth(dib32);
	i32 imageY = FreeImage_GetHeight(dib32);
	i32 scanWidth = FreeImage_GetPitch(dib32);
	std::vector<BYTE> rawData((size_t)scanWidth * imageY);
	FreeImage_ConvertToRawBits(rawData.data(), dib32, scanWidth, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
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
					u32 c24 = 0xFFFFFF & ((u32*)rawData.data())[idx];
					if(c24)
						pattern |= 1 << (7 - i);
				}
				chunk.Pattern[j] = pattern;
			}
			u16 patIdx = GetChunkIdSC0(chunkList, chunk, param);
			layoutTable.push_back(patIdx + param->offset);
		}
	}

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
