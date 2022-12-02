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
