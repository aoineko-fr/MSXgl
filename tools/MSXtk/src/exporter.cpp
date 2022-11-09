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
const char* GetCompressorName(MSXi_Compressor comp, bool bShort)
{
	switch (comp)
	{
	case COMPRESS_None:        return "None";
	case COMPRESS_Crop16:      return bShort ? "Crop16" :      "Crop16 (4-bits, max 16x16)";
	case COMPRESS_CropLine16:  return bShort ? "CropLine16" :  "CropLine16 (4-bits per line, max 16x16)";
	case COMPRESS_Crop32:      return bShort ? "Crop32" :      "Crop32 (5-bits, max 32x32)";
	case COMPRESS_CropLine32:  return bShort ? "CropLine32" :  "CropLine32 (5-bits per line, max 32x32)";
	case COMPRESS_Crop256:     return bShort ? "Crop256" :     "Crop256 (8-bits, max 256x256)";
	case COMPRESS_CropLine256: return bShort ? "CropLine256" : "CropLine256 (8-bits per line, max 256x256)";
	case COMPRESS_RLE0:        return bShort ? "RLE0" :        "RLE0 (7-bits Run-length encoding for transparency)";
	case COMPRESS_RLE4:        return bShort ? "RLE4" :        "RLE4 (4-bits Run-length encoding)";
	case COMPRESS_RLE8:        return bShort ? "RLE8" :        "RLE8 (8-bits Run-length encoding)";
	case COMPRESS_RLEp:        return bShort ? "RLEp" :        "RLEp (6-bits Pattern based RLE)";
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
std::string GetTableCText(TableFormat format, std::string name)
{
	switch (format)
	{
	case TABLE_U8: return "const unsigned char " + name + "[]";
	case TABLE_U16: return "const unsigned short " + name + "[]";
	case TABLE_U32: return "const unsigned long " + name + "[]";
	case TABLE_Header: return "const struct MSXi_Header " + name;
	}
	return "Unknow";
}

bool IsCompressorCompatible(MSXi_Compressor comp, const ExportParameters& param)
{
	if (comp == COMPRESS_None)
		return true;

	if ((param.bpc == 1) && (comp != COMPRESS_Crop16) && (comp != COMPRESS_Crop32) && (comp != COMPRESS_Crop256))
		return false;

	if ((param.bpc == 8) && (comp == COMPRESS_RLE4))
		return false;

	if (((comp == COMPRESS_Crop16) || (comp == COMPRESS_CropLine16)) && ((param.sizeX > 16) || (param.sizeY > 16)))
		return false;
	if (((comp == COMPRESS_Crop32) || (comp == COMPRESS_CropLine32)) && ((param.sizeX > 32) || (param.sizeY > 32)))
		return false;
	if (((comp == COMPRESS_Crop256) || (comp == COMPRESS_CropLine256)) && ((param.sizeX > 256) || (param.sizeY > 256)))
		return false;

	return true;
}