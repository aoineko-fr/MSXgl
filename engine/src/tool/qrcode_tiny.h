// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// QR Code generator module
//─────────────────────────────────────────────────────────────────────────────
/*
 * Copyright (c) Project Nayuki. (MIT License)
 * https://www.nayuki.io/page/qr-code-generator-library
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 */
#pragma once

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------
#include "core.h"

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

#define QRCODE_TINY_VERSION 3

/*
 * This library creates QR Code symbols, which is a type of two-dimension barcode.
 * Invented by Denso Wave and described in the ISO/IEC 18004 standard.
 * A QR Code structure is an immutable square grid of dark and light cells.
 * The library provides functions to create a QR Code from text or binary data.
 * The library covers the QR Code Model 2 specification, supporting all versions (sizes)
 * from 1 to 40, all 4 error correction levels, and 4 character encoding modes.
 * 
 * Ways to create a QR Code object:
 * - High level: Take the payload data and call QRCode_EncodeText() or QRCode_EncodeBinary().
 * - Low level: Custom-make the list of segments and call
 *   QRCode_EncodeSegments() or QRCode_EncodeSegmentsAdvanced().
 * (Note that all ways require supplying the desired error correction level and various byte buffers.)
 */

// Enum: QRCODE_ECC
// The error correction level in a QR Code symbol.
enum QRCODE_ECC
{
	// Must be declared in ascending order of error protection
	// so that an internal qrcodegen function works properly
	QRCODE_ECC_LOW = 0 ,  // The QR Code can tolerate about  7% erroneous codewords
	QRCODE_ECC_MEDIUM  ,  // The QR Code can tolerate about 15% erroneous codewords
	QRCODE_ECC_QUARTILE,  // The QR Code can tolerate about 25% erroneous codewords
	QRCODE_ECC_HIGH    ,  // The QR Code can tolerate about 30% erroneous codewords
	// 
	QRCODE_ECC_MAX,
};

// Enum: QRCODE_MASK
// The mask pattern used in a QR Code symbol.
enum QRCODE_MASK
{
	// A special value to tell the QR Code encoder to
	// automatically select an appropriate mask pattern
	QRCODE_MASK_AUTO = -1,
	// The eight actual mask patterns
	QRCODE_MASK_0 = 0, // (i + j) % 2 = 0
	QRCODE_MASK_1,     // i % 2 = 0
	QRCODE_MASK_2,     // j % 3 = 0
	QRCODE_MASK_3,     // (i + j) % 3 = 0
	QRCODE_MASK_4,     // (i / 2 + j / 3) % 2 = 0
	QRCODE_MASK_5,     // (i * j) % 2 + (i * j) % 3 = 0
	QRCODE_MASK_6,     // ((i * j) % 3 + i * j) % 2 = 0
	QRCODE_MASK_7,     // ((i * j) % 3 + i + j) % 2 = 0
	// 
	QRCODE_MASK_MAX,
};

// Enum: QRCODE_MODE
// Describes how a segment's data bits are interpreted.
enum QRCODE_MODE
{
	QRCODE_MODE_NUMERIC      = 0x1,
	QRCODE_MODE_ALPHANUMERIC = 0x2,
	QRCODE_MODE_BYTE         = 0x4,
	QRCODE_MODE_KANJI        = 0x8,
	QRCODE_MODE_ECI          = 0x7,
};

// A segment of character/binary/control data in a QR Code symbol.
// The mid-level way to create a segment is to take the payload data and call a factory function such as QRCode_MakeNumeric().
// The low-level way to create a segment is to custom-make the bit buffer and initialize a QRCode_Segment struct with appropriate values.
// Even in the most favorable conditions, a QR Code can only hold 7089 characters of data.
// Any segment longer than this is meaningless for the purpose of generating QR Codes.
// Moreover, the maximum allowed bit length is 32767 because the largest QR Code (version 40) has 31329 modules.
struct QRCode_Segment
{
	// The mode indicator of this segment.
	enum QRCODE_MODE mode;
	
	// The length of this segment's unencoded data. Measured in characters for
	// numeric/alphanumeric/kanji mode, bytes for byte mode, and 0 for ECI mode.
	// Always zero or positive. Not the same as the data's bit length.
	i16 numChars;
	
	// The data bits of this segment, packed in bitwise big endian.
	// Can be null if the bit length is zero.
	u8 *data;
	
	// The number of valid data bits used in the buffer. Requires
	// 0 <= bitLength <= 32767, and bitLength <= (capacity of data array) * 8.
	// The character count (numChars) must agree with the mode and the bit buffer length.
	i16 bitLength;
};

// Define: QRCODE_BUFFER_LEN_FOR_VERSION
// Calculates the number of bytes needed to store any QR Code up to and including the given version number,
// as a compile-time constant. For example, 'u8 buffer[QRCODE_BUFFER_LEN_FOR_VERSION(25)];'
// can store any single QR Code from version 1 to 25 (inclusive). The result fits in an i16 (or int16).
// Requires QRCODE_VERSION_MIN <= n <= QRCODE_VERSION_MAX.
#define QRCODE_BUFFER_LEN_FOR_VERSION(n)  ((((n) * 4 + 17) * ((n) * 4 + 17) + 7) / 8 + 1)

// Define: QRCODE_BUFFER_LEN_MAX
// The worst-case number of bytes needed to store one QR Code, up to and including
// version 40. This value equals 3918, which is just under 4 kilobytes.
// Use this more convenient value to avoid calculating tighter memory bounds for buffers.
#define QRCODE_BUFFER_LEN_MAX  QRCODE_BUFFER_LEN_FOR_VERSION(QRCODE_TINY_VERSION)

#define QRCODE_SIZE            (QRCODE_TINY_VERSION * 4 + 17)

//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

// Function: QRCode_EncodeText 
// Encodes the given text string to a QR Code, returning TRUE if successful.
// If the data is too long to fit in any version in the given range at the given ECC level, then FALSE is returned.
// 
// The input text must be encoded in UTF-8 and contain no NULs.
// Requires 1 <= minVersion <= maxVersion <= 40.
// 
// The smallest possible QR Code version within the given range is automatically chosen for the output. Iff boostEcl is TRUE, then the ECC level of the result
// may be higher than the ecl argument if it can be done without increasing the version. The mask is either between QRCODE_MASK_0 to 7 to force that mask, or
// QRCODE_MASK_AUTO to automatically choose an appropriate mask (which may be slow).
// 
// About the arrays, letting len = QRCODE_BUFFER_LEN_FOR_VERSION(maxVersion):
// - Before calling the function:
//   - The array ranges tempBuffer[0 : len] and qrcode[0 : len] must allow reading and writing; hence each array must have a length of at least len.
//   - The two ranges must not overlap (aliasing).
//   - The initial state of both ranges can be uninitialized because the function always writes before reading.
// - After the function returns:
//   - Both ranges have no guarantee on which elements are initialized and what values are stored.
//   - tempBuffer contains no useful data and should be treated as entirely uninitialized.
//   - If successful, qrcode can be passed into QRCode_GetSize() and QRCode_GetModule().
// 
// If successful, the resulting QR Code may use numeric, alphanumeric, or byte mode to encode the text.
// 
// In the most optimistic case, a QR Code at version 40 with low ECC can hold any UTF-8 string up to 2953 bytes, or any alphanumeric string
// up to 4296 characters, or any digit string up to 7089 characters.
// These numbers represent the hard upper limit of the QR Code standard.
// 
// Please consult the QR Code specification for information on data capacities per version, ECC level, and text encoding mode.
bool QRCode_EncodeText(const char *text, u8 tempBuffer[], u8 qrcode[], enum QRCODE_ECC ecl, enum QRCODE_MASK mask);

// Function: QRCode_EncodeSegmentsAdvanced
// Encodes the given segments to a QR Code, returning TRUE if successful.
// If the data is too long to fit in any version in the given range at the given ECC level, then FALSE is returned.
// 
// Requires 1 <= minVersion <= maxVersion <= 40.
// 
// The smallest possible QR Code version within the given range is automatically chosen for the output. Iff boostEcl is TRUE, then the ECC level of the result
// may be higher than the ecl argument if it can be done without increasing the version. The mask is either between QRCODE_MASK_0 to 7 to force that mask, or
// QRCODE_MASK_AUTO to automatically choose an appropriate mask (which may be slow).
// 
// About the byte arrays, letting len = QRCODE_BUFFER_LEN_FOR_VERSION(QRCODE_VERSION_MAX):
// - Before calling the function:
//   - The array ranges tempBuffer[0 : len] and qrcode[0 : len] must allow reading and writing; hence each array must have a length of at least len.
//   - The two ranges must not overlap (aliasing).
//   - The initial state of both ranges can be uninitialized because the function always writes before reading.
//   - The input array segs can contain segments whose data buffers overlap with tempBuffer.
// - After the function returns:
//   - Both ranges have no guarantee on which elements are initialized and what values are stored.
//   - tempBuffer contains no useful data and should be treated as entirely uninitialized.
//   - Any segment whose data buffer overlaps with tempBuffer[0 : len] must be treated as having invalid values in that array.
//   - If successful, qrcode can be passed into QRCode_GetSize() and QRCode_GetModule().
// 
// Please consult the QR Code specification for information on data capacities per version, ECC level, and text encoding mode.
// 
// This function allows the user to create a custom sequence of segments that switches between modes (such as alphanumeric and byte) to encode text in less space.
// This is a low-level API; the high-level API is QRCode_EncodeText() and QRCode_EncodeBinary().
bool QRCode_EncodeSegmentsAdvanced(const struct QRCode_Segment segs[], u16 len, enum QRCODE_ECC ecl, enum QRCODE_MASK mask, u8 tempBuffer[], u8 qrcode[]);

// Function: QRCode_GetSize
// Returns the side length of the given QR Code, assuming that encoding succeeded.
// The result is in the range [21, 177]. Note that the length of the array buffer is related to the side length - every 'u8 qrcode[]' must have length at least
// QRCODE_BUFFER_LEN_FOR_VERSION(version), which equals ceil(size^2 / 8 + 1).
inline u8 QRCode_GetSize(const u8 qrcode[]) { return qrcode[0]; }

// Function: QRCode_GetModule
// Returns the color of the module (pixel) at the given coordinates, which is FALSE for light or TRUE for dark.
// The top left corner has the coordinates (x=0, y=0).
// Coordinates must be in bounds.
bool QRCode_GetModule(const u8 qrcode[], u8 x, u8 y);

// Function: QRCode_GetByte
// Returns colors of 8 module (byte) at the given coordinates, which is 0 for light or 1 for dark.
// The top left corner has the coordinates (x=0, y=0).
u8 QRCode_GetByte(const u8 qrcode[], u8 x, u8 y);

// Function: QRCode_GetVersion
inline u8 QRCode_GetVersion(const u8 qrcode[]) { return (QRCode_GetSize(qrcode) - 17) / 4; }