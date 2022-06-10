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

namespace MSX {

/// Format of the data
enum FileFormat
{
	FILEFORMAT_Auto,				// Auto-detection
	FILEFORMAT_C,					// C header file (text)
	FILEFORMAT_Asm,					// Assembler header file (text)
	FILEFORMAT_Bin,					// Binary data file
};

/// Format of the data
enum DataFormat
{
	DATAFORMAT_Decimal,				// 0, 210, 65535
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
};

/// 
enum DataSeparator
{
	SEPARATOR_Comma,				// 0x00, 0xD2, 0xFF
	SEPARATOR_Space,				// 0x00 0xD2 0xFF
};

/// 
enum DataType
{
	DATATYPE_U8,					// 8-bits unsigned interger
	DATATYPE_U16,					// 16-bits unsigned interger
	DATATYPE_U32,					// 32-bits unsigned interger
	DATATYPE_U64,					// 64-bits unsigned interger
	DATATYPE_S8,					// 8-bits signed interger
	DATATYPE_S16,					// 16-bits signed interger
	DATATYPE_S32,					// 32-bits signed interger
	DATATYPE_S64,					// 64-bits signed interger
	DATATYPE_F32,					// 32-bits floating-point
	DATATYPE_F64,					// 64-bits floating-point
};

/// Size of the data in bytes number
enum DataSize
{
	DATASIZE_8bits = 1,
	DATASIZE_16bits = 2,
	DATASIZE_32bits = 4,
	DATASIZE_64bits = 8,
};

/// Return the data type size in bytes number
inline u32 GetDataSize(DataType type)
{
	switch (type)
	{
	case DATATYPE_U8:
	case DATATYPE_S8:
		return 1;

	case DATATYPE_U16:
	case DATATYPE_S16:
		return 2;

	case DATATYPE_U32:
	case DATATYPE_S32:
	case DATATYPE_F32:
		return 4;

	case DATATYPE_U64:
	case DATATYPE_S64:
	case DATATYPE_F64:
		return 8;
	}
	return 0;
}

/// 
inline const c8* GetDataFormat(DataFormat format, u32 size = 1)
{
	if (size == DATASIZE_64bits)
		format = DATAFORMAT_Decimal;

	switch (format)
	{
	default:
	case DATAFORMAT_Decimal:
		return "%d";

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
}

/// 
inline const c8* GetCTableType(DataType type)
{
	switch(type)
	{
	default:
	case DATATYPE_U8:  return "const unsigned char";
	case DATATYPE_U16: return "const unsigned short";
	case DATATYPE_U32: return "const unsigned long";
	case DATATYPE_U64: return "const unsigned _int64";
	case DATATYPE_S8:  return "const signed char";
	case DATATYPE_S16: return "const signed short";
	case DATATYPE_S32: return "const signed long";
	case DATATYPE_S64: return "const signed _int64";
	case DATATYPE_F32: return "const float";
	case DATATYPE_F64: return "const double";
	}
 }

} // namespace MSX
