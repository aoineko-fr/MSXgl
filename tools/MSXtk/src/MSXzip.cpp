﻿// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │       ▄        
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀▀██ ▄  ██▀▄  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▄ ██ ██▀   
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘          ▀▀
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
#include "RLEp.h"
#include "lVGM.h"

//=============================================================================
// DEFINES
//=============================================================================

const char* VERSION = "1.4.3";

/// Compressor enum
enum COMPRESSOR
{
	COMPRESS_NONE,
	COMPRESS_RLEP, // RLEp compressor
	COMPRESS_LVGM, // lVGM compressor for VGM format
};

//=============================================================================
// VARIABLES
//=============================================================================

MSX::FileData			g_InputFile;
std::string				g_OutputFile;
std::string				g_TableName;
MSX::FileFormat			g_Format = MSX::FILEFORMAT_C;
COMPRESSOR				g_Compressor = COMPRESS_NONE;

// RLEp options
u8						g_RLEp_DefaultValue = 0;
bool					g_RLEp_DefaultValueAuto = false;
bool					g_RLEp_IncludeDefaultValue = false;
bool					g_RLEp_IncludeZeroTerminator = false;

// Named value
const std::map<const c8*, u32> g_NamedValue = {
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
};

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// 
u32 GetValue(std::string name)
{
	// Named value
	for (auto it : g_NamedValue)
		if (MSX::StrEqual(name.c_str(), it.first))
			return it.second;

	// Hexadecimal
	if ((name[0] == '0') && (name[1] == 'x'))
		return strtol(name.c_str(), NULL, 16);

	// Decimal
	return atoi(name.c_str());
}

//-----------------------------------------------------------------------------
/// Display help information
void PrintHelp()
{
	printf("MSXzip %s - Binary file compressor for MSX\n", VERSION);
	printf("Usage: cmsxzip <inputfile> [options]\n");
	printf("Base options:\n");
	printf(" -o filename    Filename of the output file (default: use input filename with .h/.asm/.bin extension)\n");
	printf(" -t name        Data table name (default: use input filename)\n");
	printf(" -c             C data format (default)\n");
	printf(" -asm           Assembler data format\n");
	printf(" -bin           Binary data format\n");
	printf(" -no            No compression (default)\n");
	printf(" -help          Display this help\n");
	printf("RLEp options:\n");
	printf(" -rlep          RLEp compression\n");
	printf(" --def auto|X   Default value for type 0 chunk (default: 0)\n");
	printf(" --incdef       Include default value (as first data)\n");
	printf(" --inczero      Include 0 terminator (as last data)\n");
	printf("lVGM options:\n");
	printf(" -lVGM          VGM to lVGM convertor\n");
	printf(" --freq 50|60   Synchronization frequency (default: 60 Hz)\n");
	printf(" --nohead       Don't include header\n");
	printf(" --reorder      Reorder register writes\n");
	printf(" --simplify     Reorder register writes and remove duplicate\n");
	printf(" --split X      Split data in X bytes chunk\n");
	printf("\n");
	printf(" All integers can be decimal or hexadecimal starting with '0x'.\n");
	printf(" One of the following named values can also be used:\n  ");
	for (auto it = g_NamedValue.begin(); it != g_NamedValue.end(); ++it)
	{
		printf("%s", it->first);
		if (std::next(it) != g_NamedValue.end())
			printf(", ");
	}
	printf("\n");
}

//=============================================================================
// MAIN
//=============================================================================

//const char* ARGV[] = { "", "../testcases/ma_xevious_01.vgm", "-c", "-lVGM", "--split", "16K" };
//const char* ARGV[] = { "", "../testcases/ma_proyakyu_10.vgm", "-c", "-lVGM", "--split", "16K" };

//const char* ARGV[] = { "", "../testcases/psg_metalgear_05.vgm", "-c", "-lVGM" };
//const char* ARGV[] = { "", "../testcases/psg_goemon07.vgm", "-c", "-lVGM" };
//const char* ARGV[] = { "", "../testcases/psg_honotori_09.vgm", "-c", "-lVGM" };
//const char* ARGV[] = { "", "../testcases/mm_ff_03.vgm", "-c", "-lVGM", "-freq", "60" };
//const char* ARGV[] = { "", "../testcases/mm_undeadline_03.vgm", "-c", "-lVGM", "--simplify" };
//const char* ARGV[] = { "", "../testcases/mm_psycho_03.vgm", "-c", "-lVGM" };
//const char* ARGV[] = { "", "../testcases/mm_laydock2_01.vgm", "-c", "-lVGM", "--split", "16K" };
//const char* ARGV[] = { "", "../testcases/mm_greatestdriver_01.vgm", "-c", "-lVGM", "--split", "16K" };
//const char* ARGV[] = { "", "../testcases/mm_feedback_03.vgm", "-c", "-lVGM" };
//const char* ARGV[] = { "", "../testcases/mm_f1spirit3d_01.vgm", "-c", "-lVGM" };
//const char* ARGV[] = { "", "../testcases/mm_deva_08.vgm", "-c", "-lVGM" };
//const char* ARGV[] = { "", "../testcases/mm_columns_03.vgm", "-c", "-lVGM" };
//const char* ARGV[] = { "", "../testcases/scc_f1spirit_01.vgm", "-c", "-lVGM" };
//const char* ARGV[] = { "", "../testcases/scc_nemesis2_03.vgm", "-c", "-lVGM", "--split", "16K" };
//const char* ARGV[] = { "", "../testcases/ma_xevious_01.vgm", "-c", "-lVGM", "--freq", "50" };
//const char* ARGV[] = { "", "../testcases/lvl5.dat.dts", "-c", "-rlep", "--def", "0", "--inczero" };
//#define DEBUG_ARGS

//-----------------------------------------------------------------------------
// MAIN LOOP 
int main(int argc, const char* argv[])
{
#ifdef DEBUG_ARGS
	argc = sizeof(ARGV) / sizeof(ARGV[0]); argv = ARGV;
#endif

	// Search for -help option
	for (i32 i = 1; i < argc; ++i)
	{
		if (MSX::StrEqual(argv[i], "-help"))
		{
			PrintHelp();
			return 0;
		}
	}

	// Error check
	if (argc < 2)
	{
		printf("Error: No enough parameters!\n");
		PrintHelp();
		return 0;
	}

	g_InputFile.Filename = argv[1];

	// lVGM options
	g_lVGM_Frequency = LVGM_FREQ_60HZ;
	g_lVGM_AddHeader = true;
	g_lVGM_Simplify = LVGM_SIMPLIFY_NONE;
	g_lVGM_Split = 0;

	// Parse command line parameters
	for (i32 i = 2; i < argc; ++i)
	{
		//.....................................................................
		// Output filename
		if (MSX::StrEqual(argv[i], "-o"))
			g_OutputFile = argv[++i];
		// Table name
		else if (MSX::StrEqual(argv[i], "-t"))
			g_TableName = argv[++i];		
		// Data format
		else if (MSX::StrEqual(argv[i], "-c"))
			g_Format = MSX::FILEFORMAT_C;
		else if (MSX::StrEqual(argv[i], "-asm"))
			g_Format = MSX::FILEFORMAT_Asm;
		else if (MSX::StrEqual(argv[i], "-bin"))
			g_Format = MSX::FILEFORMAT_Bin;
		// Compressor
		else if (MSX::StrEqual(argv[i], "-no"))
			g_Compressor = COMPRESS_NONE;
		else if (MSX::StrEqual(argv[i], "-rlep"))
			g_Compressor = COMPRESS_RLEP;
		else if (MSX::StrEqual(argv[i], "-lVGM") || MSX::StrEqual(argv[i], "-ayVGM"))
			g_Compressor = COMPRESS_LVGM;
		//.....................................................................
		// RLEp options
		else if (MSX::StrEqual(argv[i], "--def") || MSX::StrEqual(argv[i], "-def"))
		{ 
			i++;
			if (MSX::StrEqual(argv[i], "auto"))
			{
				g_RLEp_DefaultValueAuto = true;
			}
			else
			{
				g_RLEp_DefaultValue = GetValue(argv[i]);
				g_RLEp_DefaultValueAuto = false;
			}
		}
		else if (MSX::StrEqual(argv[i], "--incdef") || MSX::StrEqual(argv[i], "-incdef"))
			g_RLEp_IncludeDefaultValue = true;
		else if (MSX::StrEqual(argv[i], "--inczero") || MSX::StrEqual(argv[i], "-inczero"))
			g_RLEp_IncludeZeroTerminator = true;
		//.....................................................................
		// lVGM options
		else if (MSX::StrEqual(argv[i], "--freq") || MSX::StrEqual(argv[i], "-freq"))
		{
			i++;
			if (MSX::StrEqual(argv[i], "50"))
				g_lVGM_Frequency = LVGM_FREQ_50HZ;
			else if (MSX::StrEqual(argv[i], "60"))
				g_lVGM_Frequency = LVGM_FREQ_60HZ;
		}
		else if (MSX::StrEqual(argv[i], "--nohead"))
		{
			g_lVGM_AddHeader = false;
		}
		else if (MSX::StrEqual(argv[i], "--reorder"))
		{
			g_lVGM_Simplify = LVGM_SIMPLIFY_ORDER;
		}
		else if (MSX::StrEqual(argv[i], "--simplify"))
		{
			g_lVGM_Simplify = LVGM_SIMPLIFY_DUPLICATE;
		}
		else if (MSX::StrEqual(argv[i], "--split"))
		{
			i++;
			g_lVGM_Split = GetValue(argv[i]);
		}
	}

	// Validate parameters
	if (g_TableName.empty())
	{
		g_TableName = MSX::File::RemovePath(MSX::File::RemoveExt(g_InputFile.Filename));
		for(u32 i = 0; i < g_TableName.size(); i++)
			if(!(((g_TableName[i] >= 'a') && (g_TableName[i] <= 'z')) || ((g_TableName[i] >= 'A') && (g_TableName[i] <= 'Z')) || ((g_TableName[i] >= '0') && (g_TableName[i] <= '9'))))
				g_TableName[i] = '_';
	}

	if (g_OutputFile.empty())
	{
		g_OutputFile = MSX::File::RemoveExt(g_InputFile.Filename);
		switch (g_Format)
		{
		default:
		case MSX::FILEFORMAT_C:   g_OutputFile += ".h";   break;
		case MSX::FILEFORMAT_Asm: g_OutputFile += ".asm"; break;
		case MSX::FILEFORMAT_Bin: g_OutputFile += ".bin"; break;
		}
	}

	// Load input data
	if (!MSX::File::Load(g_InputFile))
		return 1;

	MSX::ExporterInterface* exp;
	switch (g_Format)
	{
	default:
	case MSX::FILEFORMAT_C:	  exp = new MSX::ExporterC;   break;
	case MSX::FILEFORMAT_Asm: exp = new MSX::ExporterAsm; break;
	case MSX::FILEFORMAT_Bin: exp = new MSX::ExporterBin; break;
	}

	// Deco
	exp->AddComment(u8"██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█      ▄");
	exp->AddComment(u8"██  ▀  █▄  ▀██▄ ▀ ▄█ ▀▀██ ▄  ██▀▄");
	exp->AddComment(u8"█  █ █  ▀▀  ▄█  █  █ ██▄▄ ██ ██▀");
	exp->AddComment(u8"▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀         ▀▀");
	// License
	exp->AddComment(MSX::Format("MSXzip %s by Guillaume 'Aoineko' Blanchard (2022) under CC-BY-SA free license", VERSION));
	// Date
	std::time_t result = std::time(nullptr);
	char* ltime = std::asctime(std::localtime(&result));
	ltime[strlen(ltime) - 1] = 0; // remove final '\n'
	exp->AddComment(MSX::Format("File generated on %s", ltime));
	// Source
	exp->AddComment(MSX::Format("Source file: %s (%d bytes)", g_InputFile.Filename.c_str(), g_InputFile.Data.size()));

	switch (g_Compressor)
	{
	case COMPRESS_RLEP: 
		exp->AddComment("Compressor: RLEp");
		if(!ExportRLEp(exp, g_InputFile.Data))
			return 1;
		break;
	case COMPRESS_LVGM:
		exp->AddComment("Compressor: lVGM");
		if(!ExportlVGM(g_TableName, exp, g_InputFile.Data))
			return 1;
		break;
	default:
	case COMPRESS_NONE: 
		exp->AddComment("Compressor: None");
		break;
	};

	// License
	exp->AddComment(MSX::Format("Total size: %i bytes", exp->GetTotalSize()));

	exp->Export(g_OutputFile);

	delete exp;

	return 0;
}
