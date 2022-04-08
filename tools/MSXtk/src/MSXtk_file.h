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

// MSX Tool Kit
#include "MSXtk_string.h"

namespace MSX {

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
		if (fopen_s(&file.Interface, file.Filename.c_str(), "rb") != 0)
		{
			printf("Error: Fail to open file %s\n", file.Filename.c_str());
			return false;
		}
		fseek(file.Interface, 0, SEEK_END);
		file.Size = ftell(file.Interface);
		file.Data.resize(file.Size);
		fseek(file.Interface, 0, SEEK_SET);
		if (fread_s(file.Data.data(), file.Size, sizeof(u8), file.Size, file.Interface) != file.Size)
		{
			file.Data.resize(0);
			printf("Error: Fail to read file %s\n", file.Filename.c_str());
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
};

} // namespace MSX
