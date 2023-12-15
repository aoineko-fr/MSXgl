//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄   ▄▄▄▄           ▄▄         ▄▄ ▄ ▄  ▄▄
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀    ██  ▄█▀▄ ▄█▀▄ ██   ██▀   ██▄▀ ▄  ██▀
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █    ██  ▀█▄▀ ▀█▄▀ ▀█▄ ▄██    ██ █ ██ ▀█▄
//_____________________________________________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/MSXtk)
// under CC-BY-SA license (https://creativecommons.org/licenses/by-sa/2.0/)
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

//---------------------------------------------------------------------------------
struct ExportConfig
{
	DataFormat Format;
	AsmSyntax  Asm;
	u32        Address;

	ExportConfig() : Format(DATAFORMAT_Hexa), Asm(ASMSYNTAX_Default), Address(ADDRESS_INVALID) {}
};

//---------------------------------------------------------------------------------
/**
 * Exporter interface
 */
class ExporterInterface
{
public:
	u32 TotalBytes;
	ExportConfig Config;

	ExporterInterface() : TotalBytes(0), Config() {}
	ExporterInterface(ExportConfig& cfg) : TotalBytes(0), Config(cfg) {}
	virtual ~ExporterInterface() = default;

	virtual void AddReturn() = 0;
	virtual void AddComment(std::string comment = "") = 0;

	virtual void StartSection(std::string name, u8 size = DATASIZE_8bits, std::string comment = "") = 0;
	virtual void EndSection(std::string comment = "") = 0;
		
	virtual void StartLine() = 0;
	virtual void AddByte(u8 data) = 0;
	virtual void AddWord(u16 data) = 0;
	virtual void AddDouble(u32 data) = 0;
	virtual void EndLine(std::string comment = "") = 0;

	virtual void AddByteLine(u8 data, std::string comment = "")
	{
		StartLine();
		AddByte(data);
		EndLine(comment);
	}
	virtual void AddWordLine(u16 data, std::string comment = "")
	{
		StartLine();
		AddWord(data);
		EndLine(comment);
	}
	virtual void AddDoubleLine(u32 data, std::string comment = "")
	{
		StartLine();
		AddDouble(data);
		EndLine(comment);
	}
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
	ExporterDummy(ExportConfig& cfg) : ExporterInterface(cfg) {}

	virtual void AddReturn() {}
	virtual void AddComment(std::string comment = "") {}

	virtual void StartSection(std::string name, u8 size = DATASIZE_8bits, std::string comment = "") {}
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
	ExporterBin(ExportConfig& cfg) : ExporterInterface(cfg) {}

	virtual void AddReturn() {}
	virtual void AddComment(std::string comment = "") {}

	virtual void StartSection(std::string name, u8 size = DATASIZE_8bits, std::string comment = "") {}
	virtual void EndSection(std::string comment = "") {}

	virtual void StartLine() {}
	virtual void AddByte(u8 data)
	{
		outData.push_back(data);
		TotalBytes += 1;
	}
	virtual void AddWord(u16 data) // Little endian
	{
		outData.push_back(data & 0x00FF);
		outData.push_back(data >> 8);
		TotalBytes += 2;
	}
	virtual void AddDouble(u32 data) // Little endian
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
		FILE* file = fopen(filename.c_str(), "wb");
		if (file == NULL)
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
	ExporterText(ExportConfig& cfg) : ExporterInterface(cfg) {}

	virtual void AddReturn() = 0;
	virtual void AddComment(std::string comment = "") = 0;

	virtual void StartSection(std::string name, u8 size = DATASIZE_8bits, std::string comment = "") = 0;
	virtual void EndSection(std::string comment = "") = 0;

	virtual void StartLine() = 0;
	virtual void AddByte(u8 data) = 0;
	virtual void AddWord(u16 data) = 0;
	virtual void AddDouble(u32 data) = 0;
	virtual void EndLine(std::string comment = "") = 0;

	virtual bool Export(std::string filename) const
	{
		// Write header file
		FILE* file = fopen(filename.c_str(), "wb");
		if (file == NULL)
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
	ExporterC(ExportConfig& cfg) : ExporterText(cfg) {}

	virtual void AddReturn() { outData += "\n"; }
	virtual void AddComment(std::string comment = "") { outData += MSX::Format("// %s\n", comment.c_str()); }

	virtual void StartSection(std::string name, u8 size = DATASIZE_8bits, std::string comment = "")
	{
		outData += MSX::Format("%s %s[] = {", MSX::GetCTable(Config.Format, size), name.c_str());
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
		std::string Dataformat = MSX::GetDataFormat(Config.Format, DATASIZE_8bits) + std::string(", ");
		outData += MSX::Format(Dataformat.c_str(), data);
		TotalBytes += 1;
	}
	virtual void AddWord(u16 data)
	{
		std::string Dataformat = MSX::GetDataFormat(Config.Format, DATASIZE_16bits) + std::string(", ");
		outData += MSX::Format(Dataformat.c_str(), data);
		TotalBytes += 2;
	}
	virtual void AddDouble(u32 data)
	{
		std::string Dataformat = MSX::GetDataFormat(Config.Format, DATASIZE_32bits) + std::string(", ");
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

//---------------------------------------------------------------------------------
/**
 * Assembler language exporter
 */
class ExporterAsm : public ExporterText
{
	u8 CurrentSectionSize;
	u16 DataCount;

public:
	ExporterAsm() : ExporterText(), CurrentSectionSize(DATASIZE_8bits), DataCount(0) {}
	ExporterAsm(ExportConfig& cfg) : ExporterText(cfg), CurrentSectionSize(DATASIZE_8bits), DataCount(0) {}

	virtual void AddReturn() { outData += "\n"; }
	virtual void AddComment(std::string comment = "") { outData += MSX::Format(";%s\n", comment.c_str()); }

	virtual void StartSection(std::string name, u8 size = DATASIZE_8bits, std::string comment = "")
	{
		CurrentSectionSize = size;
		outData += MSX::Format("%s:", name.c_str());
		if (!comment.empty())
			outData += MSX::Format(" ; %s", comment.c_str());
		outData += MSX::Format("\n");
	}
	virtual void EndSection(std::string comment = "")
	{
		outData += MSX::Format("\n");
	}

	virtual void StartLine()
	{
		outData += MSX::Format("\t%s ", MSX::GetAsmDirective(CurrentSectionSize, Config.Asm));
		DataCount = 0;
	}
	virtual void AddByte(u8 data)
	{
		if (DataCount > 0)
			outData += std::string(", ");
		std::string Dataformat = MSX::GetDataFormat(Config.Format, DATASIZE_8bits);
		outData += MSX::Format(Dataformat.c_str(), data);
		TotalBytes += 1;
		DataCount++;
	}
	virtual void AddWord(u16 data)
	{
		if (DataCount > 0)
			outData += std::string(", ");
		std::string Dataformat = MSX::GetDataFormat(Config.Format, DATASIZE_16bits);
		outData += MSX::Format(Dataformat.c_str(), data);
		TotalBytes += 2;
		DataCount++;
	}
	virtual void AddDouble(u32 data)
	{
		if (DataCount > 0)
			outData += std::string(", ");
		std::string Dataformat = MSX::GetDataFormat(Config.Format, DATASIZE_32bits);
		outData += MSX::Format(Dataformat.c_str(), data);
		TotalBytes += 4;
		DataCount++;
	}
	virtual void EndLine(std::string comment = "")
	{
		if (!comment.empty())
			outData += MSX::Format(" ; %s", comment.c_str());
		outData += "\n";
	}
};

//---------------------------------------------------------------------------------
/**
 * Assembler language exporter
 */
class ExporterBASIC : public ExporterText
{
	u16 CurrentSectionSize;
	u16 DataCount;
	u16 LineNumber;

public:
	ExporterBASIC() : ExporterText(), CurrentSectionSize(DATASIZE_8bits), DataCount(0), LineNumber(1000) {}
	ExporterBASIC(ExportConfig& cfg) : ExporterText(cfg), CurrentSectionSize(DATASIZE_8bits), DataCount(0), LineNumber(1000) {}

	virtual void AddReturn() { outData += "\n"; }
	virtual void AddComment(std::string comment = "") { outData += MSX::Format("%i '%s\n", LineNumber++, comment.c_str()); }

	virtual void StartSection(std::string name, u8 size = DATASIZE_8bits, std::string comment = "")
	{
		CurrentSectionSize = size;
		outData += MSX::Format("%i '%s", LineNumber++, name.c_str());
		if (!comment.empty())
			outData += MSX::Format(" - %s", comment.c_str());
		outData += MSX::Format("\n");
	}
	virtual void EndSection(std::string comment = "")
	{
		outData += MSX::Format("\n");
	}

	virtual void StartLine()
	{
		outData += MSX::Format("%i ");
		DataCount = 0;
	}
	virtual void AddByte(u8 data)
	{
		if (DataCount > 0)
			outData += std::string(", ");
		std::string Dataformat = MSX::GetDataFormat(Config.Format, DATASIZE_8bits);
		outData += MSX::Format(Dataformat.c_str(), data);
		TotalBytes += 1;
		DataCount++;
	}
	virtual void AddWord(u16 data)
	{
		if (DataCount > 0)
			outData += std::string(", ");
		std::string Dataformat = MSX::GetDataFormat(Config.Format, DATASIZE_16bits);
		outData += MSX::Format(Dataformat.c_str(), data);
		TotalBytes += 2;
		DataCount++;
	}
	virtual void AddDouble(u32 data)
	{
		if (DataCount > 0)
			outData += std::string(", ");
		std::string Dataformat = MSX::GetDataFormat(Config.Format, DATASIZE_32bits);
		outData += MSX::Format(Dataformat.c_str(), data);
		TotalBytes += 4;
		DataCount++;
	}
	virtual void EndLine(std::string comment = "")
	{
		if (!comment.empty())
			outData += MSX::Format(" '%s", comment.c_str());
		outData += "\n";
	}
}; 

} // namespace MSX
