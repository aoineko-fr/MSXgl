//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄      ▄        
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀ ▀▀██ ▄  ██▀▄  
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █ ██▄▄ ██ ██▀   
//______________________________▀▀  ___________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/MSXbin)
// under CC-BY-AS free license (https://creativecommons.org/licenses/by-sa/2.0/)

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <ctime>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdarg>

// MSXtk
#include "MSXtk.h"

//=============================================================================
// DEFINES
//=============================================================================

/// RLEp pattern type
enum RLEP_PATTERN_TYPE
{
	PATTERN_DEFAULT = 0,
	PATTERN_1BYTE = 1,
	PATTERN_2BYTES = 2,
	PATTERN_UNCOMPRESS = 3,
	PATTERN_INVALID = 0xFF,
};

/// Compressed data chunk
struct Chunk
{
	u8					Type = PATTERN_INVALID;
	u32					Size = 0;
	u32					Index = 0;
	std::vector<u8>		Data;
};


extern std::string		g_TableName;

// RLEp options
extern u8				g_RLEp_DefaultValue;
extern bool				g_RLEp_DefaultValueAuto;
extern bool				g_RLEp_IncludeDefaultValue;
extern bool				g_RLEp_IncludeZeroTerminator;

//=============================================================================
// VARIABLES
//=============================================================================



//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
///
void WriteChunk(MSX::ExporterInterface* exp, Chunk& chunk)
{
	if (chunk.Type == PATTERN_INVALID)
		return;

	exp->AddComment(MSX::Format("Chunk[%i] type:%i len:%i", chunk.Index, chunk.Type, chunk.Size));
	exp->StartLine();
	exp->AddByte((u8)(((chunk.Type & 0x03) << 6) | ((chunk.Size - 1) & 0x3F)));
	exp->EndLine();
	switch (chunk.Type)
	{
	case PATTERN_DEFAULT: break;
	case PATTERN_1BYTE:
	case PATTERN_2BYTES:
	case PATTERN_UNCOMPRESS:
		exp->AddByteList(chunk.Data);
		break;
	}

	chunk.Type = PATTERN_INVALID;
	chunk.Index++;
}

//-----------------------------------------------------------------------------
///
bool ExportRLEp(MSX::ExporterInterface* exp, const std::vector<u8>& data)
{
	Chunk chunk;

	// Found best default value
	if (g_RLEp_DefaultValueAuto)
	{
		std::vector<u8> counters(256);
		for (u32 i = 0; i < data.size() - 2; i++)
		{
			if ((data[i] == data[i + 1]) && (data[i] == data[i + 2]))
				counters[data[i]]++;
		}

		u32 max = 0;
		for (u32 i = 0; i < 256; i++)
		{
			if (counters[i] > max)
			{
				g_RLEp_DefaultValue = (u8)i;
				max = counters[i];
			}
		}
	}

	exp->AddComment(MSX::Format("Default value: %i", g_RLEp_DefaultValue));

	exp->StartSection(g_TableName);

	if (g_RLEp_IncludeDefaultValue)
	{
		exp->AddComment("Default value (used for type 0)");
		exp->StartLine();
		exp->AddByte(g_RLEp_DefaultValue);
		exp->EndLine();
	}

	// Go through data
	for (u32 i = 0; i < data.size(); i++)
	{
		bool bFound = false;

		// Try 2-bytes length pattern
		while (((data.size() - i) >= 4) && (data[i] != data[i + 1]) && (data[i] == data[i + 2]) && (data[i + 1] == data[i + 3]))
		{
			bFound = true;
			if ((chunk.Type != PATTERN_2BYTES) || (chunk.Size > 64))
			{
				if ((chunk.Size > 64))
					chunk.Size--;
				WriteChunk(exp, chunk);
				chunk.Type = PATTERN_2BYTES;
				chunk.Size = 2;
				chunk.Data.resize(2);
				chunk.Data[0] = data[i];
				chunk.Data[1] = data[i + 1];
			}
			else
				chunk.Size++;
			i += 2;
		}
		if (bFound)
		{
			i++;
			WriteChunk(exp, chunk);
			continue;
		}

		// Try default value repetition
		while (((data.size() - i) >= 2) && (data[i] == g_RLEp_DefaultValue) && (data[i + 1] == g_RLEp_DefaultValue))
		{
			bFound = true;
			if ((chunk.Type != PATTERN_DEFAULT) || (chunk.Size > 64))
			{
				if ((chunk.Size > 64))
					chunk.Size--;
				WriteChunk(exp, chunk);
				chunk.Type = PATTERN_DEFAULT;
				chunk.Size = 2;
				chunk.Data.resize(0);
			}
			else
				chunk.Size++;
			i++;
		}
		if (bFound)
		{
			WriteChunk(exp, chunk);
			continue;
		}

		// Try 1-byte value repetition
		while (((data.size() - i) >= 2) && (data[i] == data[i + 1]))
		{
			bFound = true;
			if ((chunk.Type != PATTERN_1BYTE) || (chunk.Size > 64))
			{
				if ((chunk.Size > 64))
					chunk.Size--;
				WriteChunk(exp, chunk);
				chunk.Type = PATTERN_1BYTE;
				chunk.Size = 2;
				chunk.Data.resize(1);
				chunk.Data[0] = data[i];
			}
			else
				chunk.Size++;
			i++;
		}
		if (bFound)
		{
			WriteChunk(exp, chunk);
			continue;
		}

		// Add uncompress data
		if ((chunk.Type != PATTERN_UNCOMPRESS) || (chunk.Size > 64))
		{
			WriteChunk(exp, chunk);
			chunk.Type = PATTERN_UNCOMPRESS;
			chunk.Size = 1;
			chunk.Data.resize(0);
			chunk.Data.push_back(data[i]);
		}
		else
		{
			chunk.Size++;
			chunk.Data.push_back(data[i]);
		}
	}

	WriteChunk(exp, chunk);

	if (g_RLEp_IncludeZeroTerminator)
	{
		exp->AddComment("Zero terminator");
		exp->StartLine();
		exp->AddByte(0);
		exp->EndLine();
	}

	exp->EndSection();

	return true;
}