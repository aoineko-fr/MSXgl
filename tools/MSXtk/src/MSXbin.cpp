// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄   ▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄  ▄  ██▀▄ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ██ ██ █ 
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

// MSXtk
#include "MSXtk.h"

//-----------------------------------------------------------------------------
// DEFINES

const char* VERSION = "1.4.2";

#define BUFFER_SIZE 1024

// Data size enum
enum DATA_SIZE
{
	DATA_SIZE_8B,		//< Bytes data
	DATA_SIZE_16B,		//< Words data
	DATA_SIZE_32B,		//< DWords data
};

// Data format enum
enum DATA_FORMAT
{
	DATA_FORMAT_DEC,	//< Decimal number
	DATA_FORMAT_HEXA,	//< Hexadecimal number (0xFF)
	DATA_FORMAT_BIN,	//< Binary number (0b10101010)
};

// Data language enum (C or Assembler)
enum DATA_LANGUAGE
{
	DATA_LANG_C,		//< C language
	DATA_LANG_ASM,		//< Assembler language (asxxx style)
};

// Address offset
enum ADDR_OFFSET
{
	ADDR_NONE,			//< Don't display address offset
	ADDR_DEC,			//< Display address offset in decimal format
	ADDR_HEXA,			//< Display address offset in hexadecimal format
};

struct SkipData
{
	u32 From;
	u32 Size;
};

//-----------------------------------------------------------------------------
// VARIABLES

// Settings
std::string				g_InputFile;
std::string				g_OutputFile;
std::string				g_TableName;
DATA_SIZE				g_Size = DATA_SIZE_8B;
DATA_FORMAT				g_Format = DATA_FORMAT_HEXA;
DATA_LANGUAGE			g_Lang = DATA_LANG_C;
bool					g_AddStartAddr = false;
u32						g_StartAddr = 0;
i32						g_ValuePerLine = 16; // 0: not limit (only 1 line)
std::vector<SkipData>	g_Skip;
bool					g_ASCII = false; // Display ASCII code of each line (only for 8-bits data)
bool					g_PT3 = false;
bool					g_AddDefine = false;
bool					g_AddSize = false;
ADDR_OFFSET				g_Address = ADDR_NONE;
bool					g_Decoration = true;

// Globals
bool					g_NewLine;


//-----------------------------------------------------------------------------

inline std::string StringFormat(const char* fmt, ...)
{
	char buf[BUFFER_SIZE];

	va_list args;
	va_start(args, fmt);
	const auto r = std::vsnprintf(buf, sizeof buf, fmt, args);
	va_end(args);

	if (r < 0)
		// conversion failed
		return {};

	const size_t len = r;
	if (len < sizeof buf)
		// we fit in the buffer
		return { buf, len };

#if __cplusplus >= 201703L
	// C++17: Create a string and write to its underlying array
	std::string s(len, '\0');
	va_start(args, fmt);
	std::vsnprintf(s.data(), len + 1, fmt, args);
	va_end(args);

	return s;
#else
	// C++11 or C++14: We need to allocate scratch memory
	auto vbuf = std::unique_ptr<char[]>(new char[len + 1]);
	va_start(args, fmt);
	std::vsnprintf(vbuf.get(), len + 1, fmt, args);
	va_end(args);

	return { vbuf.get(), len };
#endif
}

// Remode all non-alphanumeric character 
std::string ConvertToAlphaNum(const std::string& str)
{
	std::string ret;
	for (u32 i = 0; i < str.length(); ++i)
	{
		if ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))
			ret += str[i];
		else
			ret += '_';
	}
	return ret;
}

//
const c8* GetNumberFormat()
{
	switch (g_Format)
	{
	case DATA_FORMAT_DEC:
		switch (g_Size)
		{
		default:
		case DATA_SIZE_8B: return "%3u";
		case DATA_SIZE_16B: return "%d";
		case DATA_SIZE_32B:	return "%d";
		};
	case DATA_FORMAT_BIN:
		switch (g_Size)
		{
		default:
		case DATA_SIZE_8B: return "0b%08lX";
		case DATA_SIZE_16B: return "0b%016X";
		case DATA_SIZE_32B:	return "0b%032X";
		};
	default:
	case DATA_FORMAT_HEXA:
		switch (g_Size)
		{
		default:
		case DATA_SIZE_8B: return "0x%02X";
		case DATA_SIZE_16B: return "0x%04X";
		case DATA_SIZE_32B:	return "0x%08lX";
		};
	}
}

//
void AddComment(std::string& data, const std::string& comment, bool bNewLine = true) 
{
	switch (g_Lang)
	{
	default:
	case DATA_LANG_C: data += "// "; break;
	case DATA_LANG_ASM: data += "; "; break;
	};
	data += comment;
	if (bNewLine)
		data += "\n";
}

//
void StartTable(std::string& data, const std::string& name)
{
	switch (g_Lang)
	{
	default:
	case DATA_LANG_C:
		if (g_AddDefine)
		{
			data += StringFormat("\n"
				"#ifndef D_%s\n"
				"\t#define D_%s\n"
				"#endif\n"
				"D_%s ",
				name.c_str(), name.c_str(), name.c_str());
		}
		switch (g_Size)
		{
		default:
		case DATA_SIZE_8B:	data += "const unsigned char "; break;
		case DATA_SIZE_16B:	data += "const unsigned short "; break;
		case DATA_SIZE_32B: data += "const unsigned long "; break;
		};
		if (g_AddStartAddr)
			data += StringFormat("__at(0x%X) ", g_StartAddr);
		data += name + "[] = {";
		break;
	case DATA_LANG_ASM:
		if (g_AddStartAddr)
		{
			data += StringFormat(".org 0x%X\n ", g_StartAddr);
		}
		data += name + "::";
		break;
	};
}

//
void EndTable(std::string& data, const std::string& name, u32 size)
{
	switch (g_Lang)
	{
	default:
	case DATA_LANG_C:
		data += "\n};\n";
		if (g_AddSize)
		{
			data += StringFormat("#define %s_SIZE %i\n", MSX::ToUpper(name).c_str(), size);
		}
		break;
	case DATA_LANG_ASM: data += "\n"; break;
	};
}

//
void AddNewLine(std::string& data)
{
	data += "\n";
	switch (g_Lang)
	{
	default:
	case DATA_LANG_C:		data += "\t"; break;
	case DATA_LANG_ASM:
		switch (g_Size)
		{
		default:
		case DATA_SIZE_8B:	data += ".db "; break;
		case DATA_SIZE_16B:	data += ".dw "; break;
		case DATA_SIZE_32B:	data += ".dd "; break;
		};
		break;
	};
	g_NewLine = true;
}

//
void AddByte(std::string& data, u8 value)
{
	if ((g_Lang == DATA_LANG_ASM) && !g_NewLine)
		data += ",";

	data += StringFormat(GetNumberFormat(), value);

	if (g_Lang == DATA_LANG_C)
		data += ", ";
	g_NewLine = false;
}

//
void AddWord(std::string& data, u16 value)
{
	if ((g_Lang == DATA_LANG_ASM) && !g_NewLine)
		data += ",";

	data += StringFormat(GetNumberFormat(), value);

	if (g_Lang == DATA_LANG_C)
		data += ", ";
	g_NewLine = false;
}

//
void AddDWord(std::string& data, u32 value)
{
	if ((g_Lang == DATA_LANG_ASM) && !g_NewLine)
		data += ",";

	data += StringFormat(GetNumberFormat(), value);

	if (g_Lang == DATA_LANG_C)
		data += ", ";
	g_NewLine = false;
}

//
i32 Export()
{
	std::string strData;
	u8* binData;
	char strBuf[BUFFER_SIZE];
	u8 byte;
	//u16 word;
	//u32 dword;

	// Read binary file
	FILE* file = fopen(g_InputFile.c_str(), "rb");
	if (file == NULL)
	{
		printf("Error: Fail to open file %s\n", g_InputFile.c_str());
		return 0;
	}
	fseek(file, 0, SEEK_END);
	u32 fileSize = ftell(file);
	binData = (u8*)malloc(fileSize);
	fseek(file, 0, SEEK_SET);
	if (fread(binData, sizeof(u8), fileSize, file) != fileSize)
	{
		free(binData);
		printf("Error: Fail to read file %s\n", g_InputFile.c_str());
		return 0;
	}
	fclose(file);
	// Deco
	if (g_Decoration)
	{
		AddComment(strData, u8"██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█ ▄▄   ▄");
		AddComment(strData, u8"██  ▀  █▄  ▀██▄ ▀ ▄█ ██▄  ▄  ██▀▄");
		AddComment(strData, u8"█  █ █  ▀▀  ▄█  █  █ ██▄▀ ██ ██ █");
		AddComment(strData, u8"▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀");
	}
	// License
	AddComment(strData, StringFormat("MSXbin %s by Guillaume \"Aoineko\" Blanchard (2022) under CC BY-SA free license", VERSION));
	// Date
	std::time_t result = std::time(nullptr);
	char* ltime = std::asctime(std::localtime(&result));
	ltime[strlen(ltime)-1] = 0; // remove final '\n'
	AddComment(strData, StringFormat("File generated on %s", ltime));
	// Source
	AddComment(strData, StringFormat("Soure file: %s", g_InputFile.c_str()));
	for (u32 i = 0; i < g_Skip.size(); i++)
	{
		if(i == 0)
			AddComment(strData, StringFormat("Skip areas: from=%i size=%i", g_Skip[i].From, g_Skip[i].Size));
		else
			AddComment(strData, StringFormat("          : from=%i size=%i", g_Skip[i].From, g_Skip[i].Size));
	}

	if (g_PT3)
	{
		memcpy(strBuf, binData + 0, 14);
		strBuf[14] = 0;
		AddComment(strData, StringFormat("Version:    %s", strBuf));

		memcpy(strBuf, binData + 30, 32);
		strBuf[32] = 0;
		AddComment(strData, StringFormat("Module:     %s", strBuf));

		memcpy(strBuf, binData + 66, 32);
		strBuf[32] = 0;
		AddComment(strData, StringFormat("Author:     %s", strBuf));

		switch (*(binData + 99))
		{
		case 0:		AddComment(strData, "Freq type:  (0) Pro Tracker");	break;
		case 1:		AddComment(strData, "Freq type:  (1) Sound Tracker");	break;
		case 2:		AddComment(strData, "Freq type:  (2) ASM/PSC");		break;
		case 3:		AddComment(strData, "Freq type:  (3) RealSound");		break;
		default:	AddComment(strData, StringFormat("Freq type:  (%i) Unknow", *(binData + 99)));		break;
		};
	}

	StartTable(strData, g_TableName);
	AddNewLine(strData);

	char strLine[BUFFER_SIZE];
	char* sl = strLine;

	// Parse binary data
	u32 bytes = 0;
	u32 count = 0;
	u32 total = 0;
	u32 offset = 0;
	while(bytes < fileSize)
	{
		bool bSkip = false;
		for (u32 i = 0; i < g_Skip.size(); i++)
		{
			if (bytes == g_Skip[i].From)
			{
				bytes += g_Skip[i].Size;
				bSkip = true;
			}
		}
		if (bSkip)
			continue;

		switch (g_Size)
		{
		default:
		case DATA_SIZE_8B:
			byte = *(u8*)(binData + bytes);
			AddByte(strData, byte);
			if (g_ASCII)
				*sl++ = byte >= 32 ? byte : ' ';
			bytes++;
			total++;
			break;
		case DATA_SIZE_16B:
			AddWord(strData, *(u16*)(binData + bytes));
			bytes += 2;
			total += 2;
			break;
		case DATA_SIZE_32B:
			AddDWord(strData, *(u32*)(binData + bytes));
			bytes += 4;
			total += 4;
			break;
		};

		if ((g_ValuePerLine > 0) && ((((count + 1) % g_ValuePerLine) == 0) || (bytes >= fileSize)))
		{
			if (g_ASCII || (g_Address != ADDR_NONE))
			{
				if(g_ASCII && (g_Size == DATA_SIZE_8B))
				{
					*sl++ = ' ';
					*sl++ = 0;
					sl = strLine;
				}
				if (g_Address != ADDR_NONE)
				{
					if (g_ASCII && (g_Size == DATA_SIZE_8B))
						sprintf(strBuf, (g_Address == ADDR_HEXA) ? "%08X | %s" : "%6d | %s", offset, strLine);
					else
						sprintf(strBuf, (g_Address == ADDR_HEXA) ? "%08X" : "%6d", offset);

					strcpy(strLine, strBuf);
					offset = total;
				}
				AddComment(strData, strLine, false);
			}
			if((bytes != fileSize))
				AddNewLine(strData);
		}
			
		count++;
	}

	EndTable(strData, g_TableName, total);
	AddComment(strData, StringFormat("Total bytes: %d", total));

	// Write header file
	file = fopen(g_OutputFile.c_str(), "wb");
	if (file == NULL)
	{
		printf("Error: Fail to create file %s\n", g_OutputFile.c_str());
		return 0;
	}
	fwrite(strData.c_str(), 1, strData.size(), file);
	fclose(file);
	return total;
}

//-----------------------------------------------------------------------------
// Display help information
void PrintHelp()
{
	printf("MSXbin %s - Convert binary to text file\n", VERSION);
	printf("Usage: msxbin <inputfile> [options]\n");
	printf("Options:\n");
	printf("  -o output     Filename of the output file (default: use input filename with .h/.asm extension)\n");
	printf("  -t name       Data table name (default: use input filename)\n");
	printf("  -8b           8-bits data size (default)\n");
	printf("  -16b          16-bits data size\n");
	printf("  -32b          32-bits data size\n");
	printf("  -dec          Decimal data format\n");
	printf("  -hex          Hexadecimal data format (default)\n");
	printf("  -bin          Binary data format\n");
	printf("  -c            C language data (default)\n");
	printf("  -asm          Assembler language data\n");
	printf("  -n X          Number of data per line (default: 16)\n");
	printf("                (0: means only one line)\n");
	printf("  -skip F S     Skip binary data from 'F' address to a 'S' size\n");
	printf("                Value can be hexadecimal (format: 0xFFFF) or decimal\n");
	printf("                Can be invoked multiple times to skip several sections\n");
	printf("  -ascii        Display ASCII code of each line (only for 8-bits data. default: false)\n");
	printf("  -ad           Display decimal address offset of the first element of each line\n");
	printf("  -ax           Display hexadecimal address offset of the first element of each line\n");
	printf("  -pt3			Extract PT3 header information and add it as comment (default: false)\n");
	printf("  -at X         Data starting address (can be decimal or hexadecimal starting with '0x')\n");
	printf("  -def			Add define before data structure (only for C language. default: false)\n");
	printf("  -size			Add size define after data structure (only for C language. default: false)\n");
	printf("  -nodeco       Don't display header decoration (default: false)\n");
	printf("  -help         Display this help\n");
}

//const char* ARGV[] = { "", "testcases/pt3/Beg!nsum.pt3", "-n", "16", "-skip", "0", "100", "-ascii", "-ax" };
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
		printf("Error: No enough parameters!\n");
		PrintHelp();
		return 0;
	}

	g_InputFile = argv[1];

	// Parse command line parameters
	for (i32 i = 1; i < argc; ++i)
	{
		// Display help
		if (MSX::StrEqual(argv[i], "-help"))
		{
			PrintHelp();
			return 0;
		}
		// Output filename
		else if (MSX::StrEqual(argv[i], "-o"))
			g_OutputFile = argv[++i];
		// Data table name
		else if (MSX::StrEqual(argv[i], "-t"))
		{
			g_TableName = ConvertToAlphaNum(argv[++i]);
			if ((g_TableName[0] >= '0') && (g_TableName[0] <= '9'))
				g_TableName = "g_" + g_TableName;
		}
		// Data size
		else if (MSX::StrEqual(argv[i], "-8b"))
			g_Size = DATA_SIZE_8B;
		else if (MSX::StrEqual(argv[i], "-16b"))
			g_Size = DATA_SIZE_16B;
		else if (MSX::StrEqual(argv[i], "-32b"))
			g_Size = DATA_SIZE_32B;
		// Data format
		else if (MSX::StrEqual(argv[i], "-dec"))
			g_Format = DATA_FORMAT_DEC;
		else if (MSX::StrEqual(argv[i], "-hex"))
			g_Format = DATA_FORMAT_HEXA;
		else if (MSX::StrEqual(argv[i], "-bin"))
			g_Format = DATA_FORMAT_BIN;
		// Starting address
		else if (MSX::StrEqual(argv[i], "-at"))
		{
			g_AddStartAddr = true;
			i++;
			sscanf(argv[i], "%i", &g_StartAddr);
		}
		// Data language
		else if (MSX::StrEqual(argv[i], "-c"))
			g_Lang = DATA_LANG_C;
		else if (MSX::StrEqual(argv[i], "-asm"))
			g_Lang = DATA_LANG_ASM;
		// Value per line
		else if (MSX::StrEqual(argv[i], "-n"))
			g_ValuePerLine = atoi(argv[++i]);
		// Display ASCII
		else if (MSX::StrEqual(argv[i], "-ascii"))
			g_ASCII = true;
		// Extract PT3 info
		else if (MSX::StrEqual(argv[i], "-pt3"))
			g_PT3 = true;
		// Add C define 
		else if (MSX::StrEqual(argv[i], "-def"))
			g_AddDefine = true;
		// Add size define 
		else if (MSX::StrEqual(argv[i], "-size"))
			g_AddSize = true;
		// Display address
		else if (MSX::StrEqual(argv[i], "-ad"))
			g_Address = ADDR_DEC;
		else if (MSX::StrEqual(argv[i], "-ax"))
			g_Address = ADDR_HEXA;
		// Skip area
		else if (MSX::StrEqual(argv[i], "-skip"))
		{
			SkipData s;
			i++;
			if ((argv[i][0] == '0') && (argv[i][1] == 'x'))	// hexadecimal
				s.From = strtol(argv[i], NULL, 16);
			else											// decimal
				s.From = atoi(argv[i]);
			i++;
			if ((argv[i][0] == '0') && (argv[i][1] == 'x'))	// hexadecimal
				s.Size = strtol(argv[i], NULL, 16);
			else											// decimal
				s.Size = atoi(argv[i]);
			g_Skip.push_back(s);
		}
		// No decoraction
		else if (MSX::StrEqual(argv[i], "-nodeco"))
			g_Decoration = false;
	}

	// Validate parameters
	if (g_OutputFile.empty())
	{
		g_OutputFile = MSX::RemoveExt(g_InputFile);
		switch (g_Lang)
		{
		default:
		case DATA_LANG_C: g_OutputFile += ".h"; break;
		case DATA_LANG_ASM: g_OutputFile += ".asm"; break;
		}
	}
	if (g_TableName.empty())
	{
		g_TableName = MSX::RemovePath(g_InputFile);
		g_TableName = MSX::RemoveExt(g_TableName);
		g_TableName = "g_" + ConvertToAlphaNum(g_TableName);
	}

	return g_StartAddr + Export();
}
