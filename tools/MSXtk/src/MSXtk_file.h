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

// MSX Tool Kit
#include "MSXtk_string.h"

namespace MSX {

/// Format of the data
enum FileFormat : u8
{
	FILEFORMAT_Auto,				// Auto-detection
	FILEFORMAT_C,					// C header file (text)
	FILEFORMAT_Asm,					// Assembler header file (text)
	FILEFORMAT_BASIC,				// BASIC listing file (text)
	FILEFORMAT_Bin,					// Binary data file
};
	
/// File structure
struct FileData
{
	std::string		Filename;
	FILE*			Interface = nullptr;
	u32				Size = 0;
	std::vector<u8>	Data;
};

class File
{
public:

	//----------------------------------------------------------------------------
	/// Read binary file
	static bool Load(FileData& file)
	{
		file.Interface = fopen(file.Filename.c_str(), "rb");
		if (file.Interface == NULL)
		{
			printf("Error: Fail to open file %s\n", file.Filename.c_str());
			return false;
		}
		fseek(file.Interface, 0, SEEK_END);
		file.Size = ftell(file.Interface);
		file.Data.resize(file.Size);
		fseek(file.Interface, 0, SEEK_SET);
		if (fread(file.Data.data(), sizeof(u8), file.Size, file.Interface) != file.Size)
		{
			file.Data.resize(0);
			printf("Error: Fail to read file %s\n", file.Filename.c_str());
			return false;
		}
		fclose(file.Interface);
		return true;
	}

	//----------------------------------------------------------------------------
	/// Read binary file
	static bool Save(FileData& file)
	{
		// Open binary file
		file.Interface = fopen(file.Filename.c_str(), "wb");
		if (file.Interface == NULL)
		{
			printf("Error: Fail to open output file %s\n", file.Filename.c_str());
			return false;
		}
		// Write data
		if (fwrite(file.Data.data(), sizeof(u8), file.Data.size(), file.Interface) != file.Data.size())
		{
			printf("Error: Fail to write %i bytes to file %s\n", (int)file.Data.size(), file.Filename.c_str());
			return false;
		}
		fclose(file.Interface);
		return true;
	}

	//----------------------------------------------------------------------------
	/// Remove the filename extension (if any)
	static std::string RemoveExt(const std::string& str)
	{
		size_t lastdot = str.find_last_of(".");
		if (lastdot == std::string::npos)
			return str;
		return str.substr(0, lastdot);
	}

	//----------------------------------------------------------------------------
	/// Remove the filename extension (if any)
	static std::string RemovePath(const std::string& str)
	{
		size_t lastpos;
		size_t last1 = str.find_last_of("\\");
		size_t last2 = str.find_last_of("/");
		if ((last1 == std::string::npos) && (last2 == std::string::npos))
			return str;
		else if (last1 == std::string::npos)
			lastpos = last2;
		else if (last2 == std::string::npos)
			lastpos = last1;
		else
			lastpos = std::max(last1, last2);

		return str.substr(lastpos + 1);
	}

	//----------------------------------------------------------------------------
	/// Check if filename contains the given extension
	static bool HaveExt(const std::string& str, const std::string& ext)
	{
		return str.find(ext) != std::string::npos;
	}

	//----------------------------------------------------------------------------
	/// Check if filename contains the given extension
	static FileFormat GetFormat(const std::string& filename)
	{
		if(HaveExt(filename, ".h") || HaveExt(filename, ".inc"))
			return MSX::FILEFORMAT_C;

		if (HaveExt(filename, ".s") || HaveExt(filename, ".asm"))
			return MSX::FILEFORMAT_Asm;

		if (HaveExt(filename, ".bin") || HaveExt(filename, ".raw"))
			return MSX::FILEFORMAT_Bin;

		return FILEFORMAT_Auto;
	}

};

} // namespace MSX
