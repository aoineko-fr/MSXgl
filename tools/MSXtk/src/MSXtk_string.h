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

// std
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdarg>
#include <string.h>

#define BUFFER_SIZE 1024

namespace MSX {

inline std::string Format(const char* fmt, ...)
{
	char buf[BUFFER_SIZE];

	va_list args;
	va_start(args, fmt);
	const auto r = std::vsnprintf(buf, sizeof buf, fmt, args);
	va_end(args);

	if (r < 0)
		// conversion failed
		return {};

	const size_t len = r;
	if (len < sizeof buf)
		// we fit in the buffer
		return { buf, len };

#if __cplusplus >= 201703L
	// C++17: Create a string and write to its underlying array
	std::string s(len, '\0');
	va_start(args, fmt);
	std::vsnprintf(s.data(), len + 1, fmt, args);
	va_end(args);

	return s;
#else
	// C++11 or C++14: We need to allocate scratch memory
	auto vbuf = std::unique_ptr<char[]>(new char[len + 1]);
	va_start(args, fmt);
	std::vsnprintf(vbuf.get(), len + 1, fmt, args);
	va_end(args);

	return { vbuf.get(), len };
#endif
}

/// Check if 2 string are equal
inline bool StrEqual(const c8* str1, const c8* str2, bool bCaseInsensitive = true)
{
	if(bCaseInsensitive)
		return (_stricmp(str1, str2) == 0);
	else
		return (strcmp(str1, str2) == 0);
}

/// Remove the filename extension (if any)
inline std::string RemoveExt(const std::string& str)
{
	size_t lastdot = str.find_last_of(".");
	if (lastdot == std::string::npos)
		return str;
	return str.substr(0, lastdot);
}

/// Remove the filename extension (if any)
inline std::string RemovePath(const std::string& str)
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

} // MSX
