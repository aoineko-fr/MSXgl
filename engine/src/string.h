// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

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