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
#include <assert.h>

// MSXi
#include "parser.h"
#include <cmath>

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
						u32 c24 = 0xFFFFFF & ((u32*)rawData.data())[idx];
						u8 c4 = GetNearestColorIndex(c24, (param->palType == PALETTE_MSX1) ? PaletteMSX1 : PaletteMSX2, 15, 1);
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
						u32 c24 = 0xFFFFFF & ((u32*)rawData.data())[idx];
						u8 c4 = GetNearestColorIndex(c24, (param->palType == PALETTE_MSX1) ? PaletteMSX1 : PaletteMSX2, 15, 1);
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
// EXPORT GM2-i
// 
//#############################################################################


// Structure of the combination for 1 frame
struct GM2i_Combo
{
	u8 Pattern; // Bitfield of 8x1 pixels chunk (0:Color0, 1:Color1)
	u8 Color0;  // Color index [0:15]
	u8 Color1;  // Color index [0:15]
};

// Final structure of the Chunk
struct GM2i_Chunk
{
	std::array<RGB24, 8> SourcePixels; // Original 8 pixels RGB
	GM2i_Combo Frame1; // Combination for frame 1
	GM2i_Combo Frame2; // Combination for frame 2
};

//-----------------------------------------------------------------------------
// Square distance between 2 RGB colors
inline i32 ColorDistance(const RGB24& c1, const RGB24& c2)
{
	i32 dr = static_cast<i32>(c1.R) - static_cast<i32>(c2.R);
	i32 dg = static_cast<i32>(c1.G) - static_cast<i32>(c2.G);
	i32 db = static_cast<i32>(c1.B) - static_cast<i32>(c2.B);
	return dr * dr + dg * dg + db * db;
}

//-----------------------------------------------------------------------------
// Perceived luminance of an RGB color in the 0-255 range.
inline i32 ColorLuminance(const RGB24& color)
{
	return (77 * color.R + 150 * color.G + 29 * color.B + 128) >> 8;
}

//-----------------------------------------------------------------------------
// A 50/50 blend of two colors
inline RGB24 ColorBlend(const RGB24& c1, const RGB24& c2)
{
	return RGB24
	{
		static_cast<u8>((static_cast<i32>(c1.R) + c2.R) / 2),
		static_cast<u8>((static_cast<i32>(c1.G) + c2.G) / 2),
		static_cast<u8>((static_cast<i32>(c1.B) + c2.B) / 2)
	};
}

//-----------------------------------------------------------------------------
// Chunk optimization algorithm
GM2i_Chunk ConvertChunk(const std::array<RGB24, 8>& src, u32* pal, u8 first, u8 last, u8 flickerPenalty)
{
	GM2i_Chunk bestChunk;
	bestChunk.SourcePixels = src;

	std::array<RGB24, 16> colors;
	std::array<i32, 16> luminance;
	for (u8 i = first; i <= last; ++i)
	{
		colors[i] = RGB24(pal[i]);
		luminance[i] = ColorLuminance(colors[i]);
	}

	u64 minTotalError = std::numeric_limits<u64>::max();

	// Test all possible color pairs for Frame 1
	for (u8 f1_c1 = first; f1_c1 <= last; f1_c1++)
	{
		for (u8 f1_c0 = f1_c1; f1_c0 <= last; f1_c0++)
		{
			// Test all possible color pairs for Frame 2
			for (u8 f2_c1 = first; f2_c1 <= last; f2_c1++)
			{
				for (u8 f2_c0 = f2_c1; f2_c0 <= last; f2_c0++)
				{
					if ((flickerPenalty == 100) && ((f1_c0 != f2_c0) || (f1_c1 != f2_c1)))
						continue;

					// The 4 possible color choices for a pixel based on bits F1 and F2:
					// Bit F1=1, Bit F2=1 -> f1_c1 + f2_c1
					// Bit F1=1, Bit F2=0 -> f1_c1 + f2_c0
					// Bit F1=0, Bit F2=1 -> f1_c0 + f2_c1
					// Bit F1=0, Bit F2=0 -> f1_c0 + f2_c0
					RGB24 option11 = ColorBlend(colors[f1_c1], colors[f2_c1]);
					RGB24 option10 = ColorBlend(colors[f1_c1], colors[f2_c0]);
					RGB24 option01 = ColorBlend(colors[f1_c0], colors[f2_c1]);
					RGB24 option00 = ColorBlend(colors[f1_c0], colors[f2_c0]);

					u64 currentError = 0;
					u8 f1_pat = 0;
					u8 f2_pat = 0;

					// For each pixel in the chunk (0 through 7), choose the best combination of bits
					for (i32 p = 0; p < 8; p++)
					{
						const RGB24& target = src[p];

						i32 d11 = ColorDistance(target, option11);
						i32 d10 = ColorDistance(target, option10);
						i32 d01 = ColorDistance(target, option01);
						i32 d00 = ColorDistance(target, option00);

						i32 flicker11 = luminance[f1_c1] - luminance[f2_c1];
						i32 flicker10 = luminance[f1_c1] - luminance[f2_c0];
						i32 flicker01 = luminance[f1_c0] - luminance[f2_c1];
						i32 flicker00 = luminance[f1_c0] - luminance[f2_c0];
						u64 bestPixelError = (flickerPenalty == 100) ? (u64)d11 : (u64)d11 * (100 - flickerPenalty) + (u64)(flicker11 * flicker11) * flickerPenalty * 3;
						bool bestBit1 = true, bestBit2 = true;

						if (flickerPenalty < 100)
						{
							u64 error10 = (u64)d10 * (100 - flickerPenalty) + (u64)(flicker10 * flicker10) * flickerPenalty * 3;
							u64 error01 = (u64)d01 * (100 - flickerPenalty) + (u64)(flicker01 * flicker01) * flickerPenalty * 3;
							if (error10 < bestPixelError)
							{
								bestPixelError = error10;
								bestBit1 = true;
								bestBit2 = false;
							}
							if (error01 < bestPixelError)
							{
								bestPixelError = error01;
								bestBit1 = false;
								bestBit2 = true;
							}
						}

						u64 error00 = (flickerPenalty == 100) ? (u64)d00 : (u64)d00 * (100 - flickerPenalty) + (u64)(flicker00 * flicker00) * flickerPenalty * 3;
						if (error00 < bestPixelError)
						{
							bestPixelError = error00;
							bestBit1 = false;
							bestBit2 = false;
						}

						currentError += bestPixelError;

						// Updates the bitmasks (the first pixel corresponds to the MSB—bit 7)
						if (bestBit1) f1_pat |= (1 << (7 - p));
						if (bestBit2) f2_pat |= (1 << (7 - p));
					}

					// If this overall combination is the best, we record
					if (currentError < minTotalError)
					{
						minTotalError = currentError;
						bestChunk.Frame1 = { f1_pat, f1_c0, f1_c1 };
						bestChunk.Frame2 = { f2_pat, f2_c0, f2_c1 };
					}
				}
			}
		}
	}

	return bestChunk;
}

//-----------------------------------------------------------------------------
// Export GM2i (interlace mode with 2 images)
bool ExportGM2i(ExportParameters* param, ExporterInterface* exp)
{
	if ((param->sizeX <= 0) || (param->sizeY <= 0) || ((param->sizeX % 8) != 0))
	{
		printf("Error: GM2i requires a positive image size and a width divisible by 8 pixels.\n");
		return false;
	}

	//-------------------------------------------------------------------------
	// Prepare image
	FIBITMAP* dib32 = GetPreparedImage(param->inFile.c_str(), param, true); // open and load the file using the default load option
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
	u32* rgb = (u32*)rawData.data();

	if ((imageX <= 0) || (imageY <= 0) || ((imageX % 8) != 0))
	{
		printf("Error: GM2i image width must be a positive multiple of 8 pixels.\n");
		return false;
	}

	i32 chunkNum = (imageX / 8) * imageY;
	std::vector<u8> patternFrame1(chunkNum);
	std::vector<u8> colorFrame1(chunkNum);
	std::vector<u8> patternFrame2(chunkNum);
	std::vector<u8> colorFrame2(chunkNum);

	for (i32 y = 0; y < param->sizeY; y++)
	{
		for (i32 x = 0; x < param->sizeX; x += 8)
		{
			std::array<RGB24, 8> pixels;
			loop(i, 8)
				pixels[i] = RGB24(rgb[y * imageX + x + i]);
			GM2i_Chunk chunk = ConvertChunk(pixels, PaletteMSX1, 1, 15, param->gm2iFlickerPenalty);
			
			i32 idx = x + y % 8 + ((y / 8) * imageX);

			//printf("%04i | F1:[%02x][%02x] F2:[%02x][%02x] Idx:%i\n", cnt++,
			//	chunk.Frame1.Pattern, (chunk.Frame1.Color1 << 4) + chunk.Frame1.Color0, 
			//	chunk.Frame2.Pattern, (chunk.Frame2.Color1 << 4) + chunk.Frame2.Color0, idx);

			patternFrame1[idx] = chunk.Frame1.Pattern;
			colorFrame1[idx]   = (chunk.Frame1.Color1 << 4) + chunk.Frame1.Color0;
			patternFrame2[idx] = chunk.Frame2.Pattern;
			colorFrame2[idx]   = (chunk.Frame2.Color1 << 4) + chunk.Frame2.Color0;
		}
	}

	// Frame 1 pattern data
	exp->WriteTableBegin(TABLE_U8, param->tabName + "_pattern1", "Frame 1 pattern data");
	loop (i, patternFrame1.size())
	{
		if ((i % 8) == 0)
			exp->WriteCommentLine(MSX::Format("Tile[%i] (offset:%i)", i / 8, i));
		exp->WriteLineBegin();
		exp->Write8BitsData(patternFrame1[i]);
		exp->WriteLineEnd();
	}
	exp->WriteTableEnd();

	// Frame 1 color data
	exp->WriteTableBegin(TABLE_U8, param->tabName + "_color1",   "Frame 1 color data");
	loop(i, colorFrame1.size())
	{
		if ((i % 8) == 0)
		{
			exp->WriteCommentLine(MSX::Format("Tile[%i] (offset:%i)", i / 8, i));
			exp->WriteLineBegin();
		}
		exp->Write1ByteData(colorFrame1[i]);
		if ((i % 8) == 7)
			exp->WriteLineEnd();
	}
	exp->WriteTableEnd();

	// Frame 2 pattern data
	exp->WriteTableBegin(TABLE_U8, param->tabName + "_pattern2", "Frame 2 pattern data");
	loop(i, patternFrame2.size())
	{
		if ((i % 8) == 0)
			exp->WriteCommentLine(MSX::Format("Tile[%i] (offset:%i)", i / 8, i));
		exp->WriteLineBegin();
		exp->Write8BitsData(patternFrame2[i]);
		exp->WriteLineEnd();
	}
	exp->WriteTableEnd();

	// Frame 2 color data
	exp->WriteTableBegin(TABLE_U8, param->tabName + "_color2",   "Frame 2 color data");
	loop(i, colorFrame2.size())
	{
		if ((i % 8) == 0)
		{
			exp->WriteCommentLine(MSX::Format("Tile[%i] (offset:%i)", i / 8, i));
			exp->WriteLineBegin();
		}
		exp->Write1ByteData(colorFrame2[i]);
		if ((i % 8) == 7)
			exp->WriteLineEnd();
	}
	exp->WriteTableEnd();

	bool bSaved = exp->Export();
	return bSaved;
}
