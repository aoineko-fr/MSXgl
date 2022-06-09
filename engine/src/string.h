// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//-----------------------------------------------------------------------------
// stdarg.h macros
typedef u8* va_list;
#define va_start(marker, last)  { marker = (va_list)&last + sizeof(last); }
#define va_arg(marker, type)    *((type *)((marker += sizeof(type)) - sizeof(type)))
#define va_copy(dest, src)      { dest = src; }
#define va_end(marker)          { marker = (va_list) 0; };

//-----------------------------------------------------------------------------
// Group: Character

//-----------------------------------------------------------------------------
// Function: Char_IsNum
// Check if a given character is numeric (0-9)
//
// Parameters:
//   chr - The character to check
inline bool Char_IsNum(c8 chr)
{
	return (chr >= '0') && (chr <= '9');
}

//-----------------------------------------------------------------------------
// Function: Char_IsAlpha
// Check if a given character is alphabetic (A-Za-z)
//
// Parameters:
//   chr - The character to check
inline bool Char_IsAlpha(c8 chr)
{
	return ((chr >= 'A') && (chr <= 'Z')) || ((chr >= 'a') && (chr <= 'z'));
}

//-----------------------------------------------------------------------------
// Function: Char_IsAlphaNum
// Check if a given character is alphanumeric (alphabet letter and numbers)
//
// Parameters:
//   chr - The character to check
inline bool Char_IsAlphaNum(c8 chr)
{
	return Char_IsNum(chr) || Char_IsAlpha(chr);
}

//-----------------------------------------------------------------------------
// Group: String

//-----------------------------------------------------------------------------
// Function: String_Length
// Get the length of a given string
//
// Parameters:
//   str - The string to check
inline u8 String_Length(const c8* str)
{
	u8 ret = 0;
	while(*str++)
		ret++;		
	return ret;
}

//-----------------------------------------------------------------------------
// Function: String_Copy
// Copy a source string to a destination buffer
//
// Parameters:
//   dst - Destination string (buffer must be large enough to fit the source string size)
//   src - Source string
inline void String_Copy(c8* dst, const c8* src)
{
	while(*src)
		*dst++ = *src++;
	*dst = '\0';
}

//-----------------------------------------------------------------------------
// Function: String_Format
// Build a zero-terminated string
//
// Parameters:
//   dest - Destination string buffer (must big enough to contain the whole string)
//   format - Formating string
//   ... - Variable number of parameter (depends on the Formating string)
void String_Format(c8* dest, const c8* format, ...);