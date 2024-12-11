// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Data encryption and decryption module.
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

// Crypt module version
#define CRYPT_VERSION VERSION(1, 1, 0)
// Version history
// - 1.0: Original version
// - 1.1: Cascaded translation of code bytes to the next

extern const c8*  g_CryptKey; // Zero-terminated string use as key for encrypt/decrypt
extern const c8*  g_CryptMap; // String containing the 32 valid characters 
extern const u16* g_CryptCode; // Bit-field coding table

//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

// Function: Crypt_SetKey
// Set the encryption key
// Call to this function is mandatory before calling <Crypt_Encode> or <Crypt_Decode> functions.
//
// Parameters:
//   key - Zero-terminated string that contain the key
inline void Crypt_SetKey(const c8* key) { g_CryptKey = key; }

// Function: Crypt_SetMap
// Set the encryption character mapping table
// Not mandatory (default one will be use instead).
//
// Parameters:
//   map - Zero-terminated string that contain the character mapping table (should be 16, 32 or 64 charater long depending on number of bit per character used)
inline void Crypt_SetMap(const c8* map) { g_CryptMap = map; }

// Function: Crypt_SetCode
// Set the encryption code bit field
// Not mandatory (default one will be use instead).
//
// Parameters:
//   code - Array of 8 entries of 16-bit number that map each bit into 2 characters  
inline void Crypt_SetCode(const u16* code) { g_CryptCode = code; }

// Function: Crypt_Encode
// Encrypt data
//
// Parameters:
//   data - Source data buffer  
//   size - Size of the data buffer
//   str - Destination string buffer (size must be: input data size x 2 + 1).
//
// Returns:
//   FALSE if encoding failed  
bool Crypt_Encode(const void* data, u8 size, c8* str);

// Function: Crypt_Decode
// Decrypt data
//
// Parameters:
//   str - Source string 
//   data - Destination data buffer  (size must be: length of the string / 2).
//
// Returns:
//   FALSE if decoding failed  
bool Crypt_Decode(const c8* str, void* data);
