//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄ ▄                                                      
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀ ▄  ▄█▄█ ▄▀██                                           
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █ ██ ██ █  ▀██                                           
//_______________________________▀▀____________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/MSXimg)
// under CC-BY-AS license (https://creativecommons.org/licenses/by-sa/2.0/)

// MSXi
#include "exporter.h"

//
const char* GetCompressorName(MSX::Compressor comp, bool bShort)
{
	switch (comp)
	{
	case MSX::COMPRESS_None:        return "None";
	case MSX::COMPRESS_Crop16:      return bShort ? "Crop16" :      "Crop16 (4-bits, max 16x16)";
	case MSX::COMPRESS_CropLine16:  return bShort ? "CropLine16" :  "CropLine16 (4-bits per line, max 16x16)";
	case MSX::COMPRESS_Crop32:      return bShort ? "Crop32" :      "Crop32 (5-bits, max 32x32)";
	case MSX::COMPRESS_CropLine32:  return bShort ? "CropLine32" :  "CropLine32 (5-bits per line, max 32x32)";
	case MSX::COMPRESS_Crop256:     return bShort ? "Crop256" :     "Crop256 (8-bits, max 256x256)";
	case MSX::COMPRESS_CropLine256: return bShort ? "CropLine256" : "CropLine256 (8-bits per line, max 256x256)";
	case MSX::COMPRESS_RLE0:        return bShort ? "RLE0" :        "RLE0 (7-bits Run-length encoding for transparency)";
	case MSX::COMPRESS_RLE4:        return bShort ? "RLE4" :        "RLE4 (4-bits Run-length encoding)";
	case MSX::COMPRESS_RLE8:        return bShort ? "RLE8" :        "RLE8 (8-bits Run-length encoding)";
	case MSX::COMPRESS_RLEp:        return bShort ? "RLEp" :        "RLEp (6-bits Pattern based RLE)";
	case MSX::COMPRESS_Pletter:     return bShort ? "Pletter" :     "Pletter v0.5c1 by XL2S Entertainment";
	case MSX::COMPRESS_Bitbuster:   return bShort ? "Bitbuster" :   "Bitbuster v1.2 by Team Bomba";
	case MSX::COMPRESS_Bitbuster2:  return bShort ? "Bitbuster2" :  "Bitbuster v2 by Arjan Bakker";
	case MSX::COMPRESS_ZX0:         return bShort ? "ZX0" :         "ZX0 by Einar Saukas & Urusergi";
	}
	return "Unknow";
}

//
const char* GetModeName(MSXi_Mode mode)
{
	switch (mode)
	{
	case MODE_Bitmap:	return "Bitmap";
	case MODE_GM1:		return "Graphic Mode 1";
	case MODE_GM2:		return "Graphic Mode 2";
	case MODE_Sprite:	return "Sprite";
	};
	return "Unknow";
}

//
std::string GetTableCText(TableFormat format, std::string name, s32 addr)
{
	if (format == TABLE_Header)
		return "const struct MSXi_Header " + name;

	std::string ret = "";
	switch (format)
	{
	case TABLE_U8:	ret = "const unsigned char "; break;
	case TABLE_U16: ret = "const unsigned short "; break;
	case TABLE_U32: ret = "const unsigned long "; break;
	}

	if (addr >= 0)
		ret += MSX::Format("__at(0x%4X) ", addr);
	ret += name + "[]";
	return ret;
}

bool IsCompressorCompatible(MSX::Compressor comp, const ExportParameters& param)
{
	if (comp == MSX::COMPRESS_None)
		return true;

	if ((param.bpc == 1) && (comp != MSX::COMPRESS_Crop16) && (comp != MSX::COMPRESS_Crop32) && (comp != MSX::COMPRESS_Crop256))
		return false;

	if ((param.bpc == 8) && (comp == MSX::COMPRESS_RLE4))
		return false;

	if (((comp == MSX::COMPRESS_Crop16) || (comp == MSX::COMPRESS_CropLine16)) && ((param.sizeX > 16) || (param.sizeY > 16)))
		return false;
	if (((comp == MSX::COMPRESS_Crop32) || (comp == MSX::COMPRESS_CropLine32)) && ((param.sizeX > 32) || (param.sizeY > 32)))
		return false;
	if (((comp == MSX::COMPRESS_Crop256) || (comp == MSX::COMPRESS_CropLine256)) && ((param.sizeX > 256) || (param.sizeY > 256)))
		return false;

	return true;
}
