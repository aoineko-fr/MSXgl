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
#include <string>

namespace MSX {

	//-----------------------------------------------------------------------------
	/// 
	inline u32 StringToInt(std::string name)
	{
		// Hexadecimal
		if ((name[0] == '0') && ((name[1] == 'x') || (name[1] == 'X'))) // Handle 0xFF and 0XFF hexadecimal format
			return strtol(name.c_str(), NULL, 16);
		if ((name.front() == '$') || (name.front() == '#')) // Handle $FF and #FF hexadecimal format
		{
			name = name.substr(1);
			return strtol(name.c_str(), NULL, 16);
		}
		if ((name.back() == 'h') || (name.back() == 'H')) // Handle 0FFh and 0FFH hexadecimal format
		{
			name = name.substr(0, name.length() - 1);
			return strtol(name.c_str(), NULL, 16);
		}

		// Decimal
		return atoi(name.c_str());
	}

} // namespace MSX
