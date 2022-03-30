//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄   ▄▄▄▄           ▄▄         ▄▄ ▄ ▄  ▄▄
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀    ██  ▄█▀▄ ▄█▀▄ ██   ██▀   ██▄▀ ▄  ██▀
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █    ██  ▀█▄▀ ▀█▄▀ ▀█▄ ▄██    ██ █ ██ ▀█▄
//_____________________________________________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/MSXtk)
// under CC-BY-AS license (https://creativecommons.org/licenses/by-sa/2.0/)
//_____________________________________________________________________________
#pragma once

// std
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdarg>

// MSX Tool Kit
#include "MSXtk_string.h"

namespace MSX {

/// Data size
enum DATA_SIZE
{
	DATA_8B,			///< Bytes data (8-bits)
	DATA_16B,			///< Words data (16-bits)
	DATA_32B,			///< DWords data (32-bits)
	DATA_BYTE  = DATA_8B,
	DATA_WORD  = DATA_16B,
	DATA_DWORD = DATA_32B,
};

/// Data numeral system
enum NUMERAL_SYSTEM
{
	DATA_BINARY,		///< Binary numeral system (base 2)
	DATA_DECIMAL,		///< Decimal numeral system (base 10)
	DATA_HEXADECIMAL,	///< Hexadecimal numeral system (base 16)
};

//---------------------------------------------------------------------------------
struct ExportData
{
	DATA_SIZE      Size;
	NUMERAL_SYSTEM System;

	//
	static const c8* GetFormat(u8 size, u8 system)
	{
		switch (system)
		{
		case DATA_BINARY:
			switch (size)
			{
			default:
			case DATA_8B: return "0b%08X";
			case DATA_16B: return "0b%016X";
			case DATA_32B:	return "0b%032X";
			};
		case DATA_DECIMAL:
			switch (size)
			{
			default:
			case DATA_8B: return "%3u";
			case DATA_16B: return "%d";
			case DATA_32B:	return "%d";
			};
		default:
		case DATA_HEXADECIMAL:
			switch (size)
			{
			default:
			case DATA_8B: return "0x%02X";
			case DATA_16B: return "0x%04X";
			case DATA_32B:	return "0x%08X";
			};
		}
	}
};

//---------------------------------------------------------------------------------
/**
 * Exporter interface
 */
class ExporterInterface
{
public:
	u32 TotalBytes;
	ExportData Data;

	ExporterInterface(): TotalBytes(0) {}

	virtual void AddComment(std::string comment = "") = 0;

	virtual void StartSection(std::string name, std::string comment = "") = 0;
	virtual void EndSection(std::string comment = "") = 0;
		
	virtual void StartLine() = 0;
	virtual void AddByte(u8 data) = 0;
	virtual void AddWord(u16 data) = 0;
	virtual void AddDouble(u32 data) = 0;
	virtual void EndLine(std::string comment = "") = 0;

	virtual void AddByteList(std::vector<u8> data, std::string comment = "")
	{
		StartLine();
		for(u32 i = 0; i < data.size(); i++)
			AddByte(data[i]);
		EndLine(comment);
	}
	virtual void AddWordList(std::vector<u16> data, std::string comment = "")
	{
		StartLine();
		for (u32 i = 0; i < data.size(); i++)
			AddWord(data[i]);
		EndLine(comment);
	}
	virtual void AddDoubleList(std::vector<u32> data, std::string comment = "")
	{
		StartLine();
		for (u32 i = 0; i < data.size(); i++)
			AddDouble(data[i]);
		EndLine(comment);
	}

	virtual bool Export(std::string filename) const = 0;
	virtual u32 GetTotalSize() const { return TotalBytes; }
};

//---------------------------------------------------------------------------------
/**
 * Dummy exporter
 */
class ExporterDummy : public ExporterInterface
{
public:
	ExporterDummy(): ExporterInterface() {}
	virtual void AddComment(std::string comment = "") {}

	virtual void StartSection(std::string name, std::string comment = "") {}
	virtual void EndSection(std::string comment = "") {}

	virtual void StartLine() {}
	virtual void AddByte(u8 data) { TotalBytes++; }
	virtual void AddWord(u16 data) { TotalBytes += 2; }
	virtual void AddDouble(u32 data) { TotalBytes += 4; }
	virtual void EndLine(std::string comment = "") {}

	virtual bool Export(std::string filename) const { return true; }
};

//---------------------------------------------------------------------------------
/**
 * Binary exporter
 */
class ExporterBin : public ExporterInterface
{
public:
	std::vector<u8> outData;

	ExporterBin() : ExporterInterface() {}
	virtual void AddComment(std::string comment = "") {}

	virtual void StartSection(std::string name, std::string comment = "") {}
	virtual void EndSection(std::string comment = "") {}

	virtual void StartLine() {}
	virtual void AddByte(u8 data)
	{
		outData.push_back(data);
		TotalBytes += 1;
	}
	virtual void AddWord(u16 data)
	{
		outData.push_back(data & 0x00FF);
		outData.push_back(data >> 8);
		TotalBytes += 2;
	}
	virtual void AddDouble(u32 data)
	{
		outData.push_back(data & 0x000000FF);
		outData.push_back((data >> 8) & 0x000000FF);
		outData.push_back((data >> 16) &0x000000FF);
		outData.push_back(data >> 24);
		TotalBytes += 4;
	}
	virtual void EndLine(std::string comment = "") {}

	virtual bool Export(std::string filename) const
	{
		// Write header file
		FILE* file;
		if (fopen_s(&file, filename.c_str(), "wb") != 0)
		{
			printf("Error: Fail to create %s\n", filename.c_str());
			return false;
		}
		fwrite(outData.data(), 1, outData.size(), file);
		fclose(file);
		return true;
	}
};

//---------------------------------------------------------------------------------
/**
 * Text exporter
 */
class ExporterText : public ExporterInterface
{
public:
	std::string outData;

	ExporterText() : ExporterInterface() {}
	virtual void AddComment(std::string comment = "") = 0;

	virtual void StartSection(std::string name, std::string comment = "") = 0;
	virtual void EndSection(std::string comment = "") = 0;

	virtual void StartLine() = 0;
	virtual void AddByte(u8 data) = 0;
	virtual void AddWord(u16 data) = 0;
	virtual void AddDouble(u32 data) = 0;
	virtual void EndLine(std::string comment = "") = 0;

	virtual bool Export(std::string filename) const
	{
		// Write header file
		FILE* file;
		if (fopen_s(&file, filename.c_str(), "wb") != 0)
		{
			printf("Error: Fail to create %s\n", filename.c_str());
			return false;
		}
		fwrite(outData.c_str(), 1, outData.size(), file);
		fclose(file);
		return true;
	}
}; 

//---------------------------------------------------------------------------------
/**
 * C language exporter
 */
class ExporterC : public ExporterText
{
public:
	ExporterC() : ExporterText() {}
	virtual void AddComment(std::string comment = "") { outData += MSX::Format("// %s\n", comment.c_str()); }

	virtual void StartSection(std::string name, std::string comment = "")
	{
		outData += MSX::Format("const u8 %s[] = {", name.c_str()); 
		if (!comment.empty())
			outData += MSX::Format(" // %s", comment.c_str());
		outData += MSX::Format("\n");
	}
	virtual void EndSection(std::string comment = "")
	{
		if(outData.back() != '\n')
			outData += "\n";
		outData += MSX::Format("};\n");
	}

	virtual void StartLine() { outData += MSX::Format("\t"); }
	virtual void AddByte(u8 data)
	{
		std::string Dataformat = ExportData::GetFormat(Data.Size, Data.System) + std::string(", ");
		outData += MSX::Format(Dataformat.c_str(), data);
		TotalBytes += 1;
	}
	virtual void AddWord(u16 data)
	{
		std::string Dataformat = ExportData::GetFormat(Data.Size, Data.System) + std::string(", ");
		outData += MSX::Format(Dataformat.c_str(), data);
		TotalBytes += 2;
	}
	virtual void AddDouble(u32 data)
	{
		std::string Dataformat = ExportData::GetFormat(Data.Size, Data.System) + std::string(", ");
		outData += MSX::Format(Dataformat.c_str(), data);
		TotalBytes += 4;
	}
	virtual void EndLine(std::string comment = "")
	{
		if (!comment.empty())
			outData += MSX::Format("// %s", comment.c_str());
		outData += "\n";
	}
};


} // namespace MSX