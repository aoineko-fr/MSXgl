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

extern const c8*  g_CryptKey; // Zero-terminated string use as key for encrypt/decrypt
extern const c8*  g_CryptMap; // String containing the 32 valid characters 
extern const u16* g_CryptCode; // Bit-field coding table

//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

// Function: Crypt_SetKey
// Set the encryption key
//
// Call to this function is mandatory before calling <Crypt_Encode> or <Crypt_Decode> functions.
inline void Crypt_SetKey(const c8* key) { g_CryptKey = key; }

// Function: Crypt_SetMap
// Set the encryption character mapping table
//
// Not mandatory (default one will be use instead).
inline void Crypt_SetMap(const c8* map) { g_CryptMap = map; }

// Function: Crypt_SetCode
// Set the encryption code bit field
//
// Not mandatory (default one will be use instead).
inline void Crypt_SetCode(const u16* code) { g_CryptCode = code; }

// Function: Crypt_Encode
// Encrypt data
//
// Note: Destination string buffer size must be: input data size x 2 + 1.
bool Crypt_Encode(const void* data, u8 size, c8* str);

// Function: Crypt_Decode
// Decrypt data
//
// Note: Destination buffer size must be: length of the string / 2.
bool Crypt_Decode(const c8* str, void* data);
