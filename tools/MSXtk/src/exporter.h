//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄ ▄                                                      
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀ ▄  ▄█▄█ ▄▀██                                           
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █ ██ ██ █  ▀██                                           
//_______________________________▀▀____________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/MSXimg)
// under CC BY-SA license (https://creativecommons.org/licenses/by-sa/2.0/)

#pragma once

// std
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>
// MSXi
#include "MSXimg.h"
#include "color.h"
#include "MGLV.h"

#define BUFFER_SIZE 1024

/// Export mode
enum MSXi_Mode
{
	MODE_Bitmap,				///< Export each block as bitmap
	MODE_GM1,					///< Export name/pattern/color tables for Graphic 1 mode
	MODE_GM2,					///< Export name/pattern/color tables for Graphic 2 & 3 mode
	MODE_Sprite,				///< Export 16x16 sprites with specific block ordering (0,2,1,3)
	MODE_Text,					///< Export name/pattern tables for Text 1 & 2 mode
	MODE_MGLV,					///< Export movie data in MGLV format
};

/// Format of the data
enum TableFormat
{
	TABLE_U8,					///< 8-bits unsigned interger
	TABLE_U16,					///< 16-bits unsigned interger
	TABLE_U32,					///< 32-bits unsigned interger
	TABLE_S8,					///< 8-bits signed interger
	TABLE_S16,					///< 16-bits signed interger
	TABLE_S32,					///< 32-bits signed interger
	TABLE_Header,				///< Header structure (@see MSXi_Header)
};

/// Type of iùage table
enum TableType
{
	TABLETYPE_Raw,				///< Raw binary data (misc)
	TABLETYPE_Bitmap,			///< Bitmap table
	TABLETYPE_TilePattern,		///< Tiles pattern form
	TABLETYPE_TileColor,		///< Tiles pattern color
	TABLETYPE_TileLayout,		///< Tiles screen layout
	TABLETYPE_SpritePattern,	///< Sprites pattern form
	TABLETYPE_SpriteColor,		///< Sprites pattern color
	TABLETYPE_Palette,			///< Palette
};

///Sprit layer information
struct Layer
{
	i32 posX;					///< Start X position for the layer (relative to block coordante)
	i32 posY;					///< Start Y position for the layer (relative to block coordante)
	u32 numX;					///< Width of the layer (in sprite size unit)
	u32 numY;					///< Height of the layer (in sprite size unit)
	bool size16;				///< 
	bool include;				///< 
	std::vector<u32> colors;	///< Layer colors
};

// Struct: ConfigMGLV
// MGLV video player parameters
struct ConfigMGLV
{
	MGLV_HEADER_MODE headerMode;			///< Header mode
	u8   segmentSize;			///< Segment size in KB (usually 8 or 16 KB)
	u8   screenMode;			///< Screen mode of the video
	u8   frameSkip;				///< Number of frame to skip (depend of video FPS and display frequency)
	bool isLooping;				///< Is video looping when end is reached
	u8   freq;					///< Display frequency (50 or 60 Hz)
	u16  width;					///< Video width
	u16  height;				///< Video height
	u16  minSkip;				///< Video height
	u16  minFill;				///< Video height

	ConfigMGLV(): 
		headerMode(MGLV_HEADER_FULL),
		segmentSize(8),
		screenMode(5),
		frameSkip(5),
		isLooping(false),
		freq(50),
		width(256),
		height (144),
		minSkip (MGLV_SKIP_COUNT_MIN),
		minFill (MGLV_FILL_COUNT_MIN)
	{}
};

/// Exporter parameters
struct ExportParameters
{
	std::string inFile;			///< Input filename
	std::string outFile;		///< Output filename
	std::string tabName;		///< Data table name
	MSXi_Mode mode;				///< Exporter mode
	i32 posX;					///< Start X position for data extracting
	i32 posY;					///< Start Y position for data extracting
	i32 sizeX;					///< Width of data block
	i32 sizeY;					///< Height of data block
	i32 gapX;					///< X gap between blocks
	i32 gapY;					///< Y gap between blocks
	i32 numX;					///< Number of columns of block to extract
	i32 numY;					///< Number of row of block to extract
	i32 bpc;					///< Bits Per Color (can be 1, 2, 4 or 8-bits)
	bool bUseTrans;				///< Use transparency color
	u32 transColor;				///< Transparency color (24-bits RGB)
	bool bUseOpacity;			///< Use opacity color
	u32 opacityColor;			///< Opacity color (24-bits RGB)
	PaletteType palType;		///< Palette type (@see PaletteType)
	i32 palCount;				///< Number of colors in the palette
	i32 palOffset;				///< Index offset of the palette
	bool pal24;					///< Use 24-bits palette (v9990)
	std::vector<u32> palInput;	///< Use 24-bits palette (v9990)
	MSX::Compressor comp;		///< Compressor to use (@see MSX::Compressor)
	MSX::DataFormat format;		///< Data format to use for text export (@see MSX::DataFormat)
	MSX::AsmSyntax syntax;		///< Assember syntax (@see MSX::AsmSyntax)
	bool bSkipEmpty;			///< Skip empty block (be aware this option change the block index)
	DitheringMethod dither;		///< The dithering method to use (only for 1-bit BPC)
	bool bAddCopy;				///< Add copyright information from file
	std::string copyFile;		///< Copyright filename
	bool bAddHeader;			///< Add export header table
	bool bAddIndex;				///< Add index table (should be necessary if bSkipEmpty is True)
	bool bAddFont;				///< Add font header information data (@see MSXi_Font)
	c8 fontFirst;				///< First character ASCII code
	c8 fontLast;				///< Last character ASCII code
	u8 fontX;					///< Font width (can be equal or greater than sizeX)
	u8 fontY;					///< Font height (can be equal or greater than sizeY)
	u8 offset;					///< Offset of layout index for GM1 et GM2 mode
	bool bStartAddr;			///< Add starting address in the data definition
	u32 startAddr;				///< Data starting adress
	bool bDefine;				///< Add define block for C file that allow to add directive to table definition (to place data at a given address for e.g.)
	bool bReturn0;				///< Return 0 instead of data size
	bool bTitle;				///< Display ASCII-art title on top of exported text file
	std::vector<Layer> layers;	///< Block layers
	bool bTilesCompressNames;	///< GM2 mode: Compress names/layout table
	bool bTilesUnique;			///< GM2 mode: Export all unique tiles
	bool bTilesName;			///< GM2 mode: Include name table
	bool bTilesPattern;			///< GM2 mode: Include pattern table
	bool bTilesColor;			///< GM2 mode: Include color table
	bool bBLOAD;				///< Add header for BLOAD image
	FilterMethod scaleFilter;
	u16 scaleX;
	u16 scaleY;
	bool flipH;
	bool flipV;
	f32 rotAngle;
	ConfigMGLV mglvConfig;

	ExportParameters()
	{
		inFile = "";
		outFile = "";
		tabName = "table";
		mode = MODE_Bitmap;
		posX = 0;
		posY = 0;
		sizeX = 0;
		sizeY = 0;
		gapX = 0;
		gapY = 0;
		numX = 1;
		numY = 1;
		bpc = 8;
		bUseTrans = FALSE;
		transColor = 0x000000;
		bUseOpacity = FALSE;
		opacityColor = 0x000000;
		palType = PALETTE_MSX1;
		palCount = -1;
		palOffset = 1;
		pal24 = FALSE;
		comp = MSX::COMPRESS_None;
		format = MSX::DATAFORMAT_Hexa;
		syntax = MSX::ASMSYNTAX_sdasz80;
		bSkipEmpty = FALSE;
		dither = DITHER_None;
		bAddCopy = FALSE;
		copyFile = "";
		bAddHeader = FALSE;
		bAddIndex = FALSE;
		bAddFont = FALSE;
		fontFirst = 0;
		fontLast = 0;
		fontX = 0;
		fontY = 0;
		offset = 0;
		bDefine = FALSE;
		bStartAddr = FALSE;
		startAddr = 0;
		bTitle = TRUE;
		bTilesCompressNames = FALSE;
		bTilesUnique = FALSE;
		bTilesName = TRUE;
		bTilesPattern = TRUE;
		bTilesColor = TRUE;
		bBLOAD = FALSE;
		scaleFilter = FILTER_Catmull;
		scaleX = 0;
		scaleY = 0;
		flipH = 0;
		flipV = 0;
		rotAngle = 0.f;
	}
};

// Get the short/long name of a given compressor
const char* GetCompressorName(MSX::Compressor comp, bool bShort = FALSE);

//
const char* GetModeName(MSXi_Mode mode);

// Get table format C text
std::string GetTableCText(TableFormat format, std::string name, s32 addr = -1);

// Check if a compressor if compatible with given import parameters
bool IsCompressorCompatible(MSX::Compressor comp, const ExportParameters& param);

//---------------------------------------------------------------------------------
// Exporter section parameters
struct ExportSection
{
	u32 ID;
	MSX::DataSize Size;
	MSX::DataFormat Format;
	std::vector<u8> Data;

	ExportSection(u32 id = 0, MSX::DataSize size = MSX::DATASIZE_8bits, MSX::DataFormat format = MSX::DATAFORMAT_Hexa) : ID(id), Size(size), Format(format) {}

	const std::vector<u8>& GetData() const { return Data; }
	u32 GetTotalSize() { return (u32)Data.size(); }
};

/**
 * Exporter interface
 */
class ExporterInterface
{
protected:
	MSX::DataFormat eFormat;
	ExportParameters* Param;
	std::vector<ExportSection> Sections;

public:
	ExporterInterface(MSX::DataFormat f, ExportParameters* p): eFormat(f), Param(p) {}
	virtual ~ExporterInterface() = default;
	virtual void WriteHeader() = 0;
	virtual void WriteSpriteHeader(i32 number) = 0;
	virtual void WriteTableBegin(TableFormat format, std::string name, std::string comment = "") = 0;
	virtual void WriteTableEnd(std::string comment = "") = 0;
	virtual void WriteLineBegin() = 0;
	virtual void WriteLineEnd(std::string comment = "") = 0;
	virtual void WriteCommentLine(std::string comment = "") = 0;

	virtual void Write1ByteData(u8 data) = 0;
	virtual void Write8BitsData(u8 data) = 0;
	virtual void Write1WordData(u16 data) = 0;

	virtual void Write1ByteLine(u8 a, std::string comment = "")
	{
		WriteLineBegin();
		Write1ByteData(a);
		WriteLineEnd(comment);
	}
	virtual void Write2BytesLine(u8 a, u8 b, std::string comment = "")
	{
		WriteLineBegin();
		Write1ByteData(a);
		Write1ByteData(b);
		WriteLineEnd(comment);
	}
	virtual void Write4BytesLine(u8 a, u8 b, u8 c, u8 d, std::string comment = "")
	{
		WriteLineBegin();
		Write1ByteData(a);
		Write1ByteData(b);
		Write1ByteData(c);
		Write1ByteData(d);
		WriteLineEnd(comment);
	}
	virtual void Write1WordLine(u16 a, std::string comment = "")
	{
		WriteLineBegin();
		Write1WordData(a);
		WriteLineEnd(comment);
	}
	virtual void Write2WordsLine(u16 a, u16 b, std::string comment = "")
	{
		WriteLineBegin();
		Write1WordData(a);
		Write1WordData(b);
		WriteLineEnd(comment);
	}
	virtual void WriteBytesList(std::vector<u8> data, std::string comment = "")
	{
		WriteLineBegin();
		for (u32 i = 0; i < data.size(); i++)
			Write1ByteData(data[i]);
		WriteLineEnd(comment);
	}

	virtual const c8* GetNumberFormat(u8 bytes = 1) = 0;
	virtual const c8* GetDataFormat(u8 bytes = 1) = 0;

	virtual u32 GetTotalBytes()
	{
		u32 totalSize = 0;
		loop(i, Sections.size())
			totalSize += (u32)Sections[i].Data.size();
		return totalSize;
	}
	virtual bool Export() = 0;
};

/**
 * Text exporter interface
 */
class ExporterText : public ExporterInterface
{
protected:
	char strFormat[BUFFER_SIZE];
	char strData[BUFFER_SIZE];
	std::string outData;
	u32 TotalBytes;

public:
	ExporterText(MSX::DataFormat f, ExportParameters* p) : ExporterInterface(f, p) {}

	virtual void WriteHeader()
	{
		// Add title
		if (Param->bTitle)
		{
			WriteCommentLine(u8"██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█ ▄");
			WriteCommentLine(u8"██  ▀  █▄  ▀██▄ ▀ ▄█ ▄  ▄█▄█ ▄▀██");
			WriteCommentLine(u8"█  █ █  ▀▀  ▄█  █  █ ██ ██ █  ▀██");
			WriteCommentLine(u8"▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀          ▀▀");
		}

		// Add version & date
		std::time_t result = std::time(nullptr);
		char* ltime = std::asctime(std::localtime(&result));
		ltime[strlen(ltime) - 1] = 0; // remove final '\n'
		sprintf(strData, "Data generated using MSXimg %s on %s", MSXi_VERSION, ltime);
		WriteCommentLine(strData);

		// Add author & license
		WriteCommentLine("by Guillaume \"Aoineko\" Blanchard (2022) under CC BY-SA free license");
		WriteCommentLine(u8"─────────────────────────────────────────────────────────────────────────────");

		// Add copyright information
		if (Param->bAddCopy)
		{
			std::ifstream file(Param->copyFile);
			std::string strLine;
			while (std::getline(file, strLine))
			{
				WriteCommentLine(strLine);
			}
			file.close();
			WriteCommentLine(u8"─────────────────────────────────────────────────────────────────────────────");
		}

		// Add generation parameters
		WriteCommentLine("Generation parameters:");
		WriteCommentLine(MSX::Format(" - Input file:     %s", Param->inFile.c_str()));
		WriteCommentLine(MSX::Format(" - Mode:           %s", GetModeName(Param->mode)));
		WriteCommentLine(MSX::Format(" - Start position: %i, %i", Param->posX, Param->posY));
		WriteCommentLine(MSX::Format(" - Sprite size:    %i, %i (gap: %i, %i)", Param->sizeX, Param->sizeY, Param->gapX, Param->gapY));
		WriteCommentLine(MSX::Format(" - Sprite count:   %i, %i", Param->numX, Param->numY));
		WriteCommentLine(MSX::Format(" - Color count:    %i (Transparent: #%04X)", 1 << Param->bpc, Param->transColor));
		WriteCommentLine(MSX::Format(" - Compressor:     %s", GetCompressorName(Param->comp)));
		WriteCommentLine(MSX::Format(" - Skip empty:     %s", Param->bSkipEmpty ? "TRUE" : "FALSE"));
		switch (Param->mode)
		{
		default:
		case MODE_Bitmap:
			break;
		case MODE_GM1:
		case MODE_GM2:
			WriteCommentLine(MSX::Format(" - Offset:         %i", Param->offset));
			break;
		case MODE_Sprite:
			break;
		};
	}

	virtual void WriteSpriteHeader(i32 number) = 0;
	virtual void WriteTableBegin(TableFormat format, std::string name, std::string comment = "") = 0;
	virtual void WriteTableEnd(std::string comment = "") = 0;
	virtual void WriteLineBegin() = 0;
	virtual void WriteLineEnd(std::string comment = "") = 0;
	virtual void WriteCommentLine(std::string comment = "") = 0;

	virtual void Write1ByteData(u8 data) = 0;
	virtual void Write8BitsData(u8 data) = 0;
	virtual void Write1WordData(u16 data) = 0;

	virtual const c8* GetNumberFormat(u8 bytes = 1)
	{
		return MSX::GetDataFormat(eFormat, bytes);
	}

	virtual const c8* GetDataFormat(u8 bytes = 1)
	{
		switch (bytes)
		{
		case 1:
			if (Param->syntax == MSX::ASMSYNTAX_sdasz80)
				return ".db";
			else
				return "db";
		case 2:
			if (Param->syntax == MSX::ASMSYNTAX_sdasz80)
				return ".dw";
			else
				return "dw";
		};
		return NULL;
	}

	virtual bool Export()
	{
		// Write header file
		FILE* file = fopen(Param->outFile.c_str(), "wb");
		if (file == NULL)
		{
			printf("Error: Fail to create %s\n", Param->outFile.c_str());
			return FALSE;
		}
		fwrite(outData.c_str(), 1, outData.size(), file);
		fclose(file);
		return TRUE;
	}

	virtual u32 GetTotalBytes() { return TotalBytes; }
};
	
/**
 * C langage exporter
 */
class ExporterC: public ExporterText
{
public:
	ExporterC(MSX::DataFormat f, ExportParameters* p): ExporterText(f, p) {}

	virtual void WriteSpriteHeader(i32 number)
	{
		WriteCommentLine(MSX::Format("// Sprite[%i] (offset:%i)\n", number, GetTotalBytes()));
	}

	virtual void WriteTableBegin(TableFormat format, std::string name, std::string comment = "")
	{
		if (Param->bStartAddr)
		{
			sprintf(strData,
				"\n"
				"// %s\n"
				"%s =\n"
				"{\n",
				comment.c_str(), GetTableCText(format, name, Param->startAddr + GetTotalBytes()).c_str());
		}
		else if (Param->bDefine)
		{
			sprintf(strData,
				"\n"
				"#ifndef D_%s\n"
				"\t#define D_%s\n"
				"#endif\n"
				"// %s\n"
				"D_%s %s =\n"
				"{\n",
				name.c_str(), name.c_str(), comment.c_str(), name.c_str(), GetTableCText(format, name).c_str());
		}
		else
		{
			sprintf(strData,
				"\n"
				"// %s\n"
				"%s =\n"
				"{\n",
				comment.c_str(), GetTableCText(format, name).c_str());
		}
		outData += strData;
	}

	virtual void WriteTableEnd(std::string comment = "")
	{
		outData += "};\n";
		if (comment != "")
			outData += MSX::Format("// %s\n", comment.c_str());
	}

	virtual void WriteLineBegin()
	{
		outData += "\t";
	}

	virtual void WriteLineEnd(std::string comment = "")
	{
		if (comment != "")
			outData += MSX::Format("// %s", comment.c_str());
		outData += "\n";
	}

	virtual void WriteCommentLine(std::string comment = "")
	{
		outData += MSX::Format("// %s\n", comment.c_str());
	}

	virtual void Write1ByteData(u8 data)
	{
		sprintf(strFormat, "%s, ", GetNumberFormat(1));
		outData += MSX::Format(strFormat, data);
		TotalBytes += 1;
	}

	virtual void Write8BitsData(u8 data)
	{
		sprintf(strFormat,
			"%s, /* %%c%%c%%c%%c%%c%%c%%c%%c */ ", GetNumberFormat(1));
		sprintf(strData, strFormat, data, 
			data & 0x80 ? '#' : '.', 
			data & 0x40 ? '#' : '.', 
			data & 0x20 ? '#' : '.', 
			data & 0x10 ? '#' : '.', 
			data & 0x08 ? '#' : '.', 
			data & 0x04 ? '#' : '.', 
			data & 0x02 ? '#' : '.', 
			data & 0x01 ? '#' : '.');
		outData += strData;
		TotalBytes += 1;
	}

	virtual void Write1WordData(u16 data)
	{
		sprintf(strFormat, "%s, ", GetNumberFormat(2));
		outData += MSX::Format(strFormat, data);
		TotalBytes += 2;
	}
};


/**
 * Assembler langage exporter
 */
class ExporterASM: public ExporterText
{
public:
	ExporterASM(MSX::DataFormat f, ExportParameters* p) : ExporterText(f, p) {}

	virtual void WriteSpriteHeader(i32 number)
	{
		sprintf(strData,
			"; Sprite[%i] (offset:%i)\n", number, GetTotalBytes());
		outData += strData;
	}

	virtual void WriteTableBegin(TableFormat format, std::string name, std::string comment = "")
	{
		sprintf(strData,
			"\n"
			"; %s\n"
			"%s:\n",
			comment.c_str(), name.c_str());
		outData += strData;
	}

	virtual void WriteTableEnd(std::string comment = "")
	{
		if (comment != "")
		{
			sprintf(strData,
				"; %s\n", comment.c_str());
			outData += strData;
		}
	}

	virtual void WriteLineBegin()
	{
		outData += "\t";
		outData += GetDataFormat(1);
		outData += " ";
	}

	virtual void WriteLineEnd(std::string comment = "")
	{
		if (comment != "")
			outData += MSX::Format("; %s", comment.c_str());
		outData += "\n";
	}

	virtual void WriteCommentLine(std::string comment = "")
	{
		outData += MSX::Format("; %s\n", comment.c_str());
	}

	virtual void Write1ByteData(u8 data)
	{
		sprintf(strFormat, "%s, ", GetNumberFormat(1));
		outData += MSX::Format(strFormat, data);
		TotalBytes += 1;
	}

	virtual void Write8BitsData(u8 data)
	{
		sprintf(strFormat, "%s, ", GetNumberFormat(1));
		outData += MSX::Format(strFormat, data);
		TotalBytes += 1;
	}

	virtual void Write1WordData(u16 data)
	{
		sprintf(strFormat, "%s, ", GetDataFormat(2));
		outData += MSX::Format(strFormat, data);
		TotalBytes += 2;
	}
};

/**
 * BASIC langage exporter
 */
class ExporterBASIC : public ExporterText
{
	u16 Line;
	bool bLineStart;
public:
	ExporterBASIC(MSX::DataFormat f, ExportParameters* p) : ExporterText(f, p)
	{
		Line = (p->bStartAddr) ? p->startAddr : 1000;
		bLineStart = TRUE;
	}

	virtual void WriteSpriteHeader(i32 number)
	{
		sprintf(strData,
			"%i 'Sprite[%i] (offset:%i)\n", Line++, number, GetTotalBytes());
		outData += strData;
	}

	virtual void WriteTableBegin(TableFormat format, std::string name, std::string comment = "")
	{
		sprintf(strData,
			"\n"
			"%i '%s:%s\n",
			Line++, name.c_str(), comment.c_str());
		outData += strData;
	}

	virtual void WriteTableEnd(std::string comment = "")
	{
		if (comment != "")
		{
			sprintf(strData,
				"' %s\n", comment.c_str());
			outData += strData;
		}
	}

	virtual void WriteLineBegin()
	{
		sprintf(strData, "%i DATA ", Line++);
		outData += strData;
		bLineStart = TRUE;
	}

	virtual void WriteLineEnd(std::string comment = "")
	{
		outData += "\n";
		bLineStart = FALSE;
	}

	virtual void WriteCommentLine(std::string comment = "")
	{
		sprintf(strData, "%i '%s\n", Line++, comment.c_str());
		outData += strData;
	}

	virtual void Write1ByteData(u8 data)
	{
		if (bLineStart)
			sprintf(strFormat, "%s", GetNumberFormat(1));
		else
			sprintf(strFormat, ",%s", GetNumberFormat(1));
		sprintf(strData, strFormat, data);
		outData += strData;
		TotalBytes += 1;
		bLineStart = FALSE;
	}

	virtual void Write8BitsData(u8 data)
	{
		if (bLineStart)
			sprintf(strFormat, "%s", GetNumberFormat(1));
		else
			sprintf(strFormat, ",%s", GetNumberFormat(1));
		sprintf(strData, strFormat, data);
		outData += strData;
		TotalBytes += 1;
		bLineStart = FALSE;
	}


	virtual void Write1WordData(u16 data)
	{
		if (bLineStart)
			sprintf(strFormat, "%s", GetNumberFormat(2));
		else
			sprintf(strFormat, ",%s", GetNumberFormat(2));
		sprintf(strData, strFormat, data);
		outData += strData;
		TotalBytes += 2;
		bLineStart = FALSE;
	}
};

/**
 * Binary exporter
 */
class ExporterBin: public ExporterInterface
{
protected:

public:
	ExporterBin(MSX::DataFormat f, ExportParameters* p) : ExporterInterface(f, p) {}
	virtual void WriteHeader() {}
	virtual void WriteSpriteHeader(i32 number) {}

	virtual void WriteTableBegin(TableFormat format, std::string name, std::string comment = "") { Sections.push_back(ExportSection()); }
	virtual void WriteTableEnd(std::string comment = "") {}
	virtual void WriteLineBegin() {}
	virtual void WriteLineEnd(std::string comment = "") {}
	virtual void WriteCommentLine(std::string comment = "") {}

	virtual void Write1ByteData(u8 data) { Sections.back().Data.push_back(data); }
	virtual void Write8BitsData(u8 data) { Sections.back().Data.push_back(data); }
	virtual void Write1WordData(u16 data)
	{
		Sections.back().Data.push_back(data & 0xFF);
		Sections.back().Data.push_back(data >> 8);
	}

	virtual const c8* GetNumberFormat(u8 bytes = 1) { return NULL; }
	virtual const c8* GetDataFormat(u8 bytes = 1) { return NULL; }

	virtual bool Export()
	{
		FILE* file;
		file = fopen(Param->outFile.c_str(), "wb");
		if (file == NULL)
		{
			printf("Error: Fail to create %s\n", Param->outFile.c_str());
			return FALSE;
		}
		loop(i, Sections.size())
			fwrite(Sections[i].Data.data(), 1, Sections[i].Data.size(), file);
		fclose(file);
		return TRUE;
	}

	std::vector<u8>& GetData(u32 idx = 0) { return Sections[idx].Data; }
};

/**
 * Binary exporter
 */
class ExporterBinNoSave : public ExporterBin
{
protected:

public:
	ExporterBinNoSave(MSX::DataFormat f, ExportParameters* p) : ExporterBin(f, p) {}

	virtual bool Export() { return TRUE; }
};

/**
 * Dummy exporter
 */
class ExporterDummy : public ExporterInterface
{
protected:
	u32 TotalBytes;

public:
	ExporterDummy(MSX::DataFormat f, ExportParameters* p) : ExporterInterface(f, p), TotalBytes(0) {}
	virtual void WriteHeader() {}
	virtual void WriteSpriteHeader(i32 number) {}
	virtual void WriteTableBegin(TableFormat format, std::string name, std::string comment = "") {}
	virtual void WriteTableEnd(std::string comment = "") {}
	virtual void WriteLineBegin() {}
	virtual void WriteLineEnd(std::string comment = "") {}
	virtual void WriteCommentLine(std::string comment = "") {}

	virtual void Write1ByteData(u8 data) { TotalBytes += 1; }
	virtual void Write8BitsData(u8 data) { TotalBytes += 1;	}
	virtual void Write1WordData(u16 data) { TotalBytes += 2; }

	virtual const c8* GetNumberFormat(u8 bytes = 1) { return NULL; }
	virtual const c8* GetDataFormat(u8 bytes = 1) { return NULL; }
	virtual u32 GetTotalBytes() { return TotalBytes; }
	virtual bool Export() { return TRUE; }
};

