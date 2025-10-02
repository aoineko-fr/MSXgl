// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄ ▄▄       ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ██▀ ██▄▀ ▄  ██▀▄ ▄▀██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀█▄ ██   ██ ██ █  ▀██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                         ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// String handling module
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
//
// Return:
//   FALSE if the given character is not a numeric.
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
//
// Return:
//   FALSE if the given character is not a alphabetic.
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
//
// Return:
//   FALSE if the given character is not a alphabetic or numeric.
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
//
// Return:
//   String length.
inline u8 String_Length(const c8* str)
{
	u8 ret = 0;
	while (*str++)
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
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
}

#if (STRING_USE_FROM_UINT8)
// Function: String_FromUInt8ZT
// Create a zero-terminated string from a 8-bits unsigned integer
// String buffer must be at least 4 bytes length (will be padded with '0')
//
// Parameters:
//   value  - Integer value to convert
//   string - Destination string
void String_FromUInt8ZT(u8 value, c8* string);

// Function: String_FromUInt8
// Create a string from a 8-bits unsigned integer (string is not zero-terminated)
// String buffer must be at least 3 bytes length (will be padded with '0')
// Original version derived from Galandros work (https://wikiti.brandonw.net/index.php?title=Z80_Routines:Other:DispA)
//
// Parameters:
//   value  - Integer value to convert
//   string - Destination string
void String_FromUInt8(u8 value, c8* string);
#endif

#if (STRING_USE_FROM_UINT16)
// Function: String_FromUInt16ZT
// Create a zero-terminated string from a 16-bits unsigned integer
// String buffer must be at least 6 bytes length (will be padded with '0')
//
// Parameters:
//   value  - Integer value to convert
//   string - Destination string
void String_FromUInt16ZT(u16 value, c8* string);

// Function: String_FromUInt16
// Create a string from a 16-bits unsigned integer (string is not zero-terminated)
// String buffer must be at least 5 bytes length (will be padded with '0')
// Original version by Milos "baze" Bazelides (http://map.grauw.nl/sources/external/z80bits.html#5.1)
//
// Parameters:
//   value  - Integer value to convert
//   string - Destination string
void String_FromUInt16(u16 value, c8* string);
#endif

#if (STRING_USE_FORMAT)
// Function: String_Format
// Build a zero-terminated string
//
//> Data format: %[0]<padding><type>
//> Type:      i      16 bits signed integer
//>            d      (same)
//>            I      32 bits signed integer (need STRING_USE_INT32 flag)
//>            D      (same)
//>            x      16 bits hexadecimal
//>            X      32 bits hexadecimal (need STRING_USE_INT32 flag)
//>            c      8 bits character
//>            s      Zero-terminated string
//> Padding:   Padding for integer and hexadecimal
//>            [0]    Padding with 0 (otherwise padding with space)
//
// Parameters:
//   dest - Destination string buffer (must big enough to contain the whole string)
//   format - Formating string
//   ... - Variable number of parameter (must match the Formating string)
void String_Format(c8* dest, const c8* format, ...);
#endif