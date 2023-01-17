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
#include <stdlib.h>

namespace MSX {

const u32 ADDRESS_INVALID = 0xFFFFFFFF;

/// Assembler syntax
enum AsmSyntax : u8
{
	ASMSYNTAX_sdasz80,				// SDCC assembler (default)
	ASMSYNTAX_tniASM,				// tniASM assembler
	ASMSYNTAX_asMSX,				// asMSX assembler
	ASMSYNTAX_Sjasm,				// Sjasm assembler
	ASMSYNTAX_Default				= ASMSYNTAX_sdasz80,
};

/// Format of the data
enum DataFormat : u8
{
	DATAFORMAT_Decimal,				// Default decimal type (unsigned)
	DATAFORMAT_DecimalSigned,		// 0, -46, -1
	DATAFORMAT_DecimalUnsigned,		// 0, 210, 65535

	DATAFORMAT_Hexa,				// Default hexadecimal type (C format)
	DATAFORMAT_HexaC,				// 0x00, 0xD2, 0xFFFF
	DATAFORMAT_HexaASM,				// 00h, 0D2h, 0FFFFh
	DATAFORMAT_HexaPascal,			// $00, $D2, $FFFF
	DATAFORMAT_HexaBasic,			// &H00, &HD2, &HFFFF
	DATAFORMAT_HexaAnd,				// &00, &D2, &FFFF
	DATAFORMAT_HexaSharp,			// #00, #D2, #FFFF
	DATAFORMAT_HexaRaw,				// 00, D2, FFFF
	
	DATAFORMAT_Binary,				// Default binary type (C format)
	DATAFORMAT_BinaryC,				// 0b00000000, 0b11010010, 0b1111111111111111
	DATAFORMAT_BinaryASM,			// 00000000b, 11010010b, 1111111111111111b

	DATAFORMAT_Float,				// IEEE 754 floating number
};

/// 
enum DataSeparator : u8
{
	SEPARATOR_Comma,				// 0x00, 0xD2, 0xFF (default)
	SEPARATOR_Space,				// 0x00 0xD2 0xFF
};

/// Size of the data in bytes number
enum DataSize : u8
{
	DATASIZE_8bits  = 1,
	DATASIZE_16bits = 2,
	DATASIZE_32bits = 4,
	DATASIZE_64bits = 8,
	// Alias
	DATASIZE_Byte  = DATASIZE_8bits,
	DATASIZE_Word  = DATASIZE_16bits,
	DATASIZE_DWord = DATASIZE_32bits,
	DATASIZE_QWord = DATASIZE_64bits,
};

/// Compression mode
enum Compressor : u16
{
	COMPRESS_None          = 0x0000,     ///< No compression

	// Crop compression
	COMPRESS_Crop16        = 0x0001, ///< Crop sprite to keep only the non-transparent area (max size 16x16)
	COMPRESS_Crop32        = 0x0002, ///< Crop sprite to keep only the non-transparent area (max size 32x32)
	COMPRESS_Crop256       = 0x0003, ///< Crop sprite to keep only the non-transparent area (max size 256x256)
	COMPRESS_CropLine_Mask = 0x0008, // Bits mask
	COMPRESS_CropLine16    = COMPRESS_Crop16 | COMPRESS_CropLine_Mask,  ///< Crop each sprite line (max size 16x16)
	COMPRESS_CropLine32    = COMPRESS_Crop32 | COMPRESS_CropLine_Mask,  ///< Crop each sprite line (max size 32x32)
	COMPRESS_CropLine256   = COMPRESS_Crop256 | COMPRESS_CropLine_Mask, ///< Crop each sprite line (max size 256x256)
	COMPRESS_Crop_Mask     = 0x000F, // Bits mask

	// RLE compression
	COMPRESS_RLE0          = 0x0010, ///< Run-length encoding of transparent blocs (7-bits for block length)
	COMPRESS_RLE4          = 0x0020, ///< Run-length encoding for all colors (4-bits for block length)
	COMPRESS_RLE8          = 0x0030, ///< Run-length encoding for all colors (8-bits for block length)
	COMPRESS_RLEp          = 0x0040, ///< Pattern based run-length encoding (6-bits for block length)
	COMPRESS_RLE_Mask      = 0x00F0, // Bits mask

	// 3rd party compressor
	COMPRESS_Pletter       = 0x0100, ///< Pletter v0.5c1 by XL2S Entertainment
	COMPRESS_Bitbuster     = 0x0200, ///< Bitbuster v1.2 by Team Bomba
	COMPRESS_Bitbuster2    = 0x0300, ///< Bitbuster v2 by Arjan Bakker
	COMPRESS_ZX0           = 0x0400, ///< ZX0 by Einar Saukas & Urusergi
	//COMPRESS_Ext_Mask      = 0xFF00, // Bits mask
};

/// 
inline const c8* GetDataFormat(DataFormat format, u8 size = DATASIZE_8bits)
{
	if (size == DATASIZE_64bits)
		return "%d";

	switch (format)
	{
	default:
	case DATAFORMAT_Float:
	case DATAFORMAT_Decimal:
	case DATAFORMAT_DecimalUnsigned:
		switch (size)
		{
		default:
		case DATASIZE_8bits:	return "%3u";
		case DATASIZE_16bits:	return "%5u";
		case DATASIZE_32bits:	return "%u";
		}
	case DATAFORMAT_DecimalSigned:
		switch (size)
		{
		default:
		case DATASIZE_8bits:	return "%d";
		case DATASIZE_16bits:	return "%d";
		case DATASIZE_32bits:	return "%d";
		}

	case DATAFORMAT_Hexa:
	case DATAFORMAT_HexaC:
		switch (size)
		{
		default:
		case DATASIZE_8bits:	return "0x%02X";
		case DATASIZE_16bits:	return "0x%04X";
		case DATASIZE_32bits:	return "0x%08X";
		}

	case DATAFORMAT_HexaASM:
		switch (size)
		{
		default:
		case DATASIZE_8bits:	return "0%02Xh";
		case DATASIZE_16bits:	return "0%04Xh";
		case DATASIZE_32bits:	return "0%08Xh";
		}
	case DATAFORMAT_HexaPascal:
		switch (size)
		{
		default:
		case DATASIZE_8bits:	return "$%02X";
		case DATASIZE_16bits:	return "$%04X";
		case DATASIZE_32bits:	return "$%08X";
		}
	case DATAFORMAT_HexaBasic:
		switch (size)
		{
		default:
		case DATASIZE_8bits:	return "&H%02X";
		case DATASIZE_16bits:	return "&H%04X";
		case DATASIZE_32bits:	return "&H%08X";
		}
	case DATAFORMAT_HexaAnd:
		switch (size)
		{
		default:
		case DATASIZE_8bits:	return "&%02X";
		case DATASIZE_16bits:	return "&%04X";
		case DATASIZE_32bits:	return "&%08X";
		}
	case DATAFORMAT_HexaSharp:
		switch (size)
		{
		default:
		case DATASIZE_8bits:	return "#%02X";
		case DATASIZE_16bits:	return "#%04X";
		case DATASIZE_32bits:	return "#%08X";
		}
	case DATAFORMAT_HexaRaw:
		switch (size)
		{
		default:
		case DATASIZE_8bits:	return "%02X";
		case DATASIZE_16bits:	return "%04X";
		case DATASIZE_32bits:	return "%08X";
		}

	case DATAFORMAT_Binary:
	case DATAFORMAT_BinaryC:
	case DATAFORMAT_BinaryASM:
		return "%u";
	}
	return NULL;
}

/// 
inline const c8* GetCTable(DataFormat format, u8 size = DATASIZE_8bits)
{
	switch (format)
	{
	default:
	case DATAFORMAT_Decimal:
	case DATAFORMAT_DecimalUnsigned:
	case DATAFORMAT_Hexa:
	case DATAFORMAT_HexaC:
	case DATAFORMAT_HexaASM:
	case DATAFORMAT_HexaPascal:
	case DATAFORMAT_HexaBasic:
	case DATAFORMAT_HexaAnd:
	case DATAFORMAT_HexaSharp:
	case DATAFORMAT_HexaRaw:
	case DATAFORMAT_Binary:
	case DATAFORMAT_BinaryC:
	case DATAFORMAT_BinaryASM:
		switch (size)
		{
		default:
		case DATASIZE_8bits:	return "const unsigned char";
		case DATASIZE_16bits:	return "const unsigned short";
		case DATASIZE_32bits:	return "const unsigned long";
		case DATASIZE_64bits:	return "const unsigned long long";
		}
	case DATAFORMAT_DecimalSigned:
		switch (size)
		{
		default:
		case DATASIZE_8bits:	return "const signed char";
		case DATASIZE_16bits:	return "const signed short";
		case DATASIZE_32bits:	return "const signed long";
		case DATASIZE_64bits:	return "const signed long long";
		}

	case DATAFORMAT_Float:
		switch (size)
		{
		default:
		case DATASIZE_8bits:	return "const unsigned char";
		case DATASIZE_16bits:	return "const unsigned short";
		case DATASIZE_32bits:	return "const float";
		case DATASIZE_64bits:	return "const double";
		}
	}
	return NULL;
 }

///
inline const c8* GetAsmDirective(u8 size, u8 syntax)
{
	switch (syntax)
	{
	case ASMSYNTAX_sdasz80:
		switch (size)
		{
		default:
		case DATASIZE_8bits:	return ".db";
		case DATASIZE_16bits:	return ".dw";
		case DATASIZE_32bits:	return ".quad";
		};
	default:
	case ASMSYNTAX_tniASM:
	case ASMSYNTAX_asMSX:
	case ASMSYNTAX_Sjasm:
		switch (size)
		{
		default:
		case DATASIZE_8bits:	return "db";
		case DATASIZE_16bits:	return "dw";
		case DATASIZE_32bits:	return "dd";
		};
	};

	return NULL;
}

} // namespace MSX
