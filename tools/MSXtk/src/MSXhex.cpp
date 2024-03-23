// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄            
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄  ▄███ ▀█▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀█▄▄ ▄▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
//  available on GitHub (https://github.com/aoineko-fr/MSXgl/tree/main/tools/MSXtk)
//  under CC-BY-SA free license (https://creativecommons.org/licenses/by-sa/2.0/)
//─────────────────────────────────────────────────────────────────────────────

// std
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <ctime>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <map>

// MSXtk
#include "MSXtk.h"

//=============================================================================
// DEFINES
//=============================================================================

const char* VERSION = "0.1.8";

#define BUFFER_SIZE 1024

// Record structure
struct Record
{
	u8  Count;
	u16 Address;
	u8  Type;
	std::vector<u8> Data;
	u8  Checksum;

	Record() : Count(0), Address(0), Type(0), Checksum(0) {}
};

// Record structure
struct Segment
{
	u16 Size;
	u16 Lower;
	u16 Higher;

	Segment() : Size(0), Lower(0xFFFF), Higher(0) {}
};

// RawData structure
struct RawData
{
	u32 Offset;
	std::vector<u8> Data;

	RawData() : Offset(0) {}
};

//
enum RecordType : u8
{
	Type_Data                   = 0,
	Type_EndOfFile              = 1,
	Type_ExtendedSegmentAddress = 2,
	Type_StartSegmentAddress    = 3,
	Type_ExtendedLinearAddress  = 4,
	Type_StartLinearAddress     = 5,
};

// Raw file parser step
enum ParserStep : u8
{
	ParserStep_Offset = 0,
	ParserStep_Filename,
};


//=============================================================================
// VARIABLES
//=============================================================================

// Configuration
std::string          g_InputFile;
std::string          g_OutputFile;
std::string          g_OutputExt;
bool                 g_Log = false;
bool                 g_Check = false;
bool                 g_Verbose = true;
u32                  g_StartAddress = 0;
u32                  g_DataSize = 0;
u32                  g_BankSize = 0;
std::vector<u8>      g_BinData;
std::vector<bool>    g_BinCheck;
u8                   g_Padding = 0xFF;
std::vector<Segment> g_SegmentInfo;
std::vector<RawData> g_RawData;

std::map<c8, u8> g_HexaMap = {
	{ '0', 0 },
	{ '1', 1 },
	{ '2', 2 },
	{ '3', 3 },
	{ '4', 4 },
	{ '5', 5 },
	{ '6', 6 },
	{ '7', 7 },
	{ '8', 8 },
	{ '9', 9 },
	{ 'A', 10 },
	{ 'B', 11 },
	{ 'C', 12 },
	{ 'D', 13 },
	{ 'E', 14 },
	{ 'F', 15 },
};

std::map<const c8*, u32> g_NamedValue = {
	{ "1K",   1024 * 1 },
	{ "2K",   1024 * 2 },
	{ "4K",   1024 * 4 },
	{ "8K",   1024 * 8 },
	{ "16K",  1024 * 16 },
	{ "24K",  1024 * 24 },
	{ "32K",  1024 * 32 },
	{ "48K",  1024 * 48 },
	{ "64K",  1024 * 64 },
	{ "128K", 1024 * 128 },
	{ "256K", 1024 * 256 },
	{ "512K", 1024 * 512 },
	{ "1M",   1024 * 1024 * 1 },
	{ "2M",   1024 * 1024 * 2 },
	{ "4M",   1024 * 1024 * 4 },
	{ "8M",   1024 * 1024 * 8 },
	{ "16M",  1024 * 1024 * 16 },
	{ "32M",  1024 * 1024 * 32 },
	{ "64M",  1024 * 1024 * 64 },
};

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// 
const c8* GetU8(const c8* ptr, u8& byte)
{
	byte =  (1 << 4) * g_HexaMap[*ptr++];
	byte += (1 << 0) * g_HexaMap[*ptr++];
	return ptr;
}

//-----------------------------------------------------------------------------
// 
const c8* GetU16(const c8* ptr, u16& word)
{
	word =  (1 << 12) * g_HexaMap[*ptr++];
	word += (1 << 8)  * g_HexaMap[*ptr++];
	word += (1 << 4)  * g_HexaMap[*ptr++];
	word += (1 << 0)  * g_HexaMap[*ptr++];
	return ptr;
}

//-----------------------------------------------------------------------------
// 
u32 GetValue(std::string name)
{
	// Named value
	for (std::map<const c8*, u32>::iterator it = g_NamedValue.begin(); it != g_NamedValue.end(); ++it)
		if (MSX::StrEqual(name.c_str(), it->first))
			return it->second;

	// Hexadecimal
	if ((name[0] == '0') && (name[1] == 'x'))	
		return strtol(name.c_str(), NULL, 16);

	// Decimal
	return atol(name.c_str());
}

//-----------------------------------------------------------------------------
// 
bool WriteBytesAtOffset(u32 offsetStart, const std::vector<u8>& data)
{
	// Get file offset
	u32 offsetEnd = offsetStart + (u32)data.size() - 1;

	// Check max size
	if ((g_DataSize != 0) && (offsetEnd > g_DataSize - 1))
	{
		printf("Error: Try to write value outside defined data length! (Max=%08Xh Destination=%08Xh)\n", g_DataSize - 1, offsetEnd);
		return false;
	}

	// Resize data if needed
	if (offsetEnd + 1 > (u32)g_BinData.size())
	{
		g_BinData.resize(offsetEnd + 1, g_Padding);
		g_BinCheck.resize(offsetEnd + 1, false);
	}

	// Write data
	u32 offset = offsetStart;
	for (u32 i = 0; i < data.size(); i++)
	{
		if (g_BinCheck[offset])
		{
			printf("Error: Data overwrite at offset %08Xh!\n", offset);
			return false;
		}

		g_BinData[offset] = data[i];
		g_BinCheck[offset] = true;

		offset++;
	}

	return true;
}

//-----------------------------------------------------------------------------
// 
bool WriteBytesAtAddress(u32 addr, const std::vector<u8>& data)
{
	// Check starting address
	if (addr < g_StartAddress)
	{
		printf("Error: Try to write value below start address! (Start=%08Xh Addr=%08Xh)\n", g_StartAddress, addr);
		return false;
	}

	// Check banked data segment
	u16 segNum = (addr >> 16);
	u16 segAddr = (addr & 0xFFFF);
	if ((g_BankSize != 0) && (segNum > 0))
	{	
		u16 segOffset = (u16)(addr & (g_BankSize - 1));
		addr = g_StartAddress + (segNum * g_BankSize) + segOffset;
	}

	// Update segment info
	if ((u16)g_SegmentInfo.size() < segNum + 1)
		g_SegmentInfo.resize(segNum + 1);
	g_SegmentInfo[segNum].Size += (u16)data.size();
	if (segAddr < g_SegmentInfo[segNum].Lower)
		g_SegmentInfo[segNum].Lower = segAddr;
	if (segAddr + data.size() - 1 > g_SegmentInfo[segNum].Higher)
		g_SegmentInfo[segNum].Higher = segAddr + (u16)data.size() - 1;

	return WriteBytesAtOffset(addr - g_StartAddress, data);
}

//-----------------------------------------------------------------------------
// Save binary file
int SaveBinary(std::string outFile)
{
	if (g_Verbose)
	{
		u32 size = 0;
		printf("Saving %s...\n", outFile.c_str());
		for (u16 i = 0; i < g_SegmentInfo.size(); i++)
		{
			if (g_SegmentInfo[i].Size > 0)
			{
				printf(" Seg[%04i]: Lower=%04Xh Higher=%04Xh Size=%i (Holes=%i)\n", i, g_SegmentInfo[i].Lower, g_SegmentInfo[i].Higher, g_SegmentInfo[i].Higher - g_SegmentInfo[i].Lower + 1, (g_SegmentInfo[i].Higher - g_SegmentInfo[i].Lower + 1) - g_SegmentInfo[i].Size);
				size += g_SegmentInfo[i].Higher - g_SegmentInfo[i].Lower + 1;
			}
		}
		if(g_SegmentInfo.size() > 1)
			printf(" Total size: %i\n", size);
	}

	// Pad to desired size
	if ((g_DataSize != 0) && (g_BinData.size() < g_DataSize))
		g_BinData.resize(g_DataSize, g_Padding);

	// Open binary file
	FILE* file = fopen(outFile.c_str(), "wb");
	if (file == NULL)
	{
		printf("Error: Fail to open output file %s\n", outFile.c_str());
		return 1;
	}
	// Write data
	if (fwrite(g_BinData.data(), sizeof(u8), g_BinData.size(), file) != g_BinData.size())
	{
		printf("Error: Fail to write %i bytes to file %s\n", (int)g_BinData.size(), outFile.c_str());
		return 1;
	}
	fclose(file);

	return 0;
}

//-----------------------------------------------------------------------------
//
bool AddRawData(u32 offset, std::string inFile)
{
	//if (g_Log)
		printf("Log: Add file '%s' at offset %08Xh\n", inFile.c_str(), offset);

	RawData raw;
	raw.Offset = offset;

	// Read binary file
	FILE* file = fopen(inFile.c_str(), "rb");
	if (file == NULL)
	{
		printf("Error: Fail to open input file %s\n", inFile.c_str());
		return false;
	}
	fseek(file, 0, SEEK_END);
	u32 fileSize = ftell(file);
	raw.Data.resize(fileSize);
	fseek(file, 0, SEEK_SET);
	if (fread(&raw.Data[0], sizeof(u8), fileSize, file) != fileSize)
	{
		printf("Error: Fail to read file %s\n", inFile.c_str());
		return false;
	}
	fclose(file);

	g_RawData.push_back(raw);
	return true;
}

//-----------------------------------------------------------------------------
// 
bool AddRawDataList(std::string inFile)
{
	MSX::FileData fileData(inFile);
	if (!MSX::File::Load(fileData))
		return false;

	ParserStep step = ParserStep_Offset;
	std::string offset;
	std::string filename;
	for (u32 i = 0; i < fileData.Data.size(); i++)
	{
		u8 chr = fileData.Data[i];
		switch (chr)
		{
		case ':':
			if (step == ParserStep_Offset)
				step = ParserStep_Filename;
			break;
		
		case '\n':
		case '\r':
			if (step == ParserStep_Filename)
			{
				AddRawData(GetValue(offset), filename);
				offset = filename = "";
				step = ParserStep_Offset;
			}
			break;

		case '\t': // skip
		case ' ':
			break;

		default:
			if (step == ParserStep_Offset)
				offset += chr;
			else // if (step == ParserStep_Filename)
				filename += chr;
			break;
		}
	}

	if (step == ParserStep_Filename)
	{
		AddRawData(GetValue(offset), filename);
	}

	return true;
}

//-----------------------------------------------------------------------------
// Load and parse Intel HEX file
bool ParseHex(std::string inFile)
{
	std::string strData;
	u8* binData;
	u32 baseAddr = 0x00000000, lowAddr = 0xFFFFFFFF;

	// Display header
	printf("MSXhex %s - Convert Intel HEX file to binary\n", VERSION);
	if (g_Verbose)
		printf(" Start=%08Xh Size=%08Xh Bank=%08Xh Pad=%02Xh\n", g_StartAddress, g_DataSize, g_BankSize, g_Padding);

	// Read binary file
	FILE* file = fopen(inFile.c_str(), "rb");
	if (file == NULL)
	{
		printf("Error: Fail to open input file %s\n", inFile.c_str());
		return false;
	}
	fseek(file, 0, SEEK_END);
	u32 fileSize = ftell(file);
	binData = (u8*)malloc(fileSize);
	fseek(file, 0, SEEK_SET);
	if (fread(binData, sizeof(u8), fileSize, file) != fileSize)
	{
		free(binData);
		printf("Error: Fail to read file %s\n", inFile.c_str());
		return false;
	}
	fclose(file);

	// Parse data
	const c8* ptr = (const c8*)binData;
	u32 index = 0;
	while (1)
	{
		// Skip to next record
		while (*ptr != ':')
			ptr++;
		ptr++;

		// Read record
		Record rec;
		ptr = GetU8(ptr, rec.Count);
		ptr = GetU16(ptr, rec.Address);
		ptr = GetU8(ptr, rec.Type);
		for (u8 i = 0; i < rec.Count; i++)
		{
			u8 byte;
			ptr = GetU8(ptr, byte);
			rec.Data.push_back(byte);
		}
		ptr = GetU8(ptr, rec.Checksum);

		if (g_Check)
		{
			u8 sum = rec.Count + (rec.Address >> 8) + (rec.Address & 0xFF) + rec.Type + rec.Checksum;
			for (u8 i = 0; i < rec.Data.size(); i++)
				sum += rec.Data[i];
			if (sum != 0)
			{
				printf("Error: Record[%i] checksum error!\n", index);
				return false;
			}
		}

		// Compute record global address
		u32 addr = baseAddr + rec.Address;
		if (addr < lowAddr)
			lowAddr = addr;

		// Parse record
		switch (rec.Type)
		{
		case Type_Data:
			if (g_Log)
				printf("Log: Record[%i] Addr=%08Xh Size=%i\n", index, addr, rec.Count);
			if (!WriteBytesAtAddress(addr, rec.Data))
				return false;
			break;
		case Type_EndOfFile:
			if (g_Log)
				printf("Log: End of file\n");
			free(binData);
			return true;
		case Type_ExtendedSegmentAddress:
			baseAddr = (rec.Data[0] << 12) + (rec.Data[1] << 4);
			if (g_Log)
				printf("Log: Set base Addr=%08Xh\n", baseAddr);
			break;
		case Type_StartSegmentAddress:
			break;
		case Type_ExtendedLinearAddress:
			baseAddr = (rec.Data[0] << 24) + (rec.Data[1] << 16);
			if (g_Log)
				printf("Log: Set base Addr=%08Xh\n", baseAddr);
			break;
		case Type_StartLinearAddress:
			break;
		};

		index++;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Display help information
void PrintHelp()
{
	printf("MSXhex %s - Convert Intel HEX file to binary\n", VERSION);
	printf("--------------------------------------------------------------------------------\n");
	printf("Usage: msxhex <inputfile> [options]\n\n");
	printf("Options:\n");
	printf(" -o filename      Output filename (default: use input filename with '.bin')\n");
	printf(" -e ext           Output filename extension (can't be use with -o)\n");
	printf(" -s addr          Starting address (default: 0)\n");
	printf(" -l length        Total data length (default: 0, means autodetect)\n");
	printf(" -b length        Bank size (default: 0, means don't use)\n");
	printf(" -p value         Pading value (default: 0xFF)\n");
	printf(" -r offset file   Raw data 'file' to add at a given 'offset'\n");
	printf(" -rl filename     Raw data files list in format: offset:filename\\n\n");
	printf(" -check           Validate records\n");
	printf(" -log             Log records\n");
	printf("\n");
	printf(" All integers can be decimal or hexadecimal starting with '0x'.\n");
	printf(" One of the following named values can also be used:\n  ");
	for (std::map<const c8*, u32>::iterator it = g_NamedValue.begin(); it != g_NamedValue.end(); ++it)
	{
		printf("%s", it->first);
		if(std::next(it) != g_NamedValue.end())
			printf(", ");
	}
	printf("\n");
}

//-----------------------------------------------------------------------------
//const char* ARGV[] = { "", "D:/Dev/Private/MSX/MSXgl/projects/htd005/out/htd005.ihx", "-e", "rom", "-s", "0x4000", "-l", "131072", "-b", "8192", "-log", "-rl" , "list.txt" };
//#define DEBUG_ARGS

//-----------------------------------------------------------------------------
// MAIN LOOP 
int main(int argc, const char* argv[])
{
#ifdef DEBUG_ARGS
	argc = sizeof(ARGV) / sizeof(ARGV[0]); argv = ARGV;
#endif

	// Error check
	if (argc < 2)
	{
		//printf("Error: Not enough parameters!\n");
		PrintHelp();
		return 0;
	}

	g_InputFile = argv[1];

	// Parse command line parameters
	for (i32 i = 2; i < argc; ++i)
	{
		// Output filename
		if (MSX::StrEqual(argv[i], "-o"))
		{
			g_OutputFile = argv[++i];
		}
		// Output filename extension
		else if (MSX::StrEqual(argv[i], "-e"))
		{
			g_OutputExt = argv[++i];
		}
		// Starting address
		else if (MSX::StrEqual(argv[i], "-s"))
		{
			g_StartAddress = GetValue(argv[++i]);
		}
		// Total data length (size)
		else if (MSX::StrEqual(argv[i], "-l"))
		{
			g_DataSize = GetValue(argv[++i]);
		}
		// Bank size
		else if (MSX::StrEqual(argv[i], "-b"))
		{
			g_BankSize = GetValue(argv[++i]);
		}
		// Padding value
		else if (MSX::StrEqual(argv[i], "-p"))
		{
			g_Padding = (u8)GetValue(argv[++i]);
		}
		// Raw data file
		else if (MSX::StrEqual(argv[i], "-r"))
		{
			u32 offset = GetValue(argv[++i]);
			std::string file = argv[++i];
			AddRawData(offset, file);
		}
		// Raw data files list
		else if (MSX::StrEqual(argv[i], "-rl"))
		{
			std::string file = argv[++i];
			AddRawDataList(file);
		}

		// Activate record logging
		else if (MSX::StrEqual(argv[i], "-log"))
		{
			g_Log = true;
		}
		// Activate record validation check
		else if (MSX::StrEqual(argv[i], "-check"))
		{
			g_Check = true;
		}
	}

	// Generate output filename
	if (g_OutputFile.empty())
	{
		g_OutputFile = MSX::RemoveExt(g_InputFile);
		if(g_OutputExt.empty())
			g_OutputFile += ".bin";
		else
			g_OutputFile += "." + g_OutputExt;
	}

	// Parse IHX file
	if (!ParseHex(g_InputFile))
		return 1;

	// Insert raw data
	for (u8 i = 0; i < g_RawData.size(); i++)
	{
		if (!WriteBytesAtOffset(g_RawData[i].Offset, g_RawData[i].Data))
			return 1;
	}

	// Save final binary file
	return SaveBinary(g_OutputFile);
}
