/* 
 * QR Code generator library (C)
 * 
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

#include "qrcode.h"

#if (QRCODE_VERSION_CUSTOM)
	u8 g_QRCode_VersionMin = QRCODE_VERSION_MIN;
	u8 g_QRCode_VersionMax = QRCODE_VERSION_MAX;
	#define QRCODE_VERSION_CURRENT_MIN	g_QRCode_VersionMin
	#define QRCODE_VERSION_CURRENT_MAX	g_QRCode_VersionMax
#else
	#define QRCODE_VERSION_CURRENT_MIN	QRCODE_VERSION_MIN
	#define QRCODE_VERSION_CURRENT_MAX	QRCODE_VERSION_MAX
#endif

//#if (QRCODE_MASK_DEF == QRCODE_MASK_CUSTOM)
//	u8 g_QRCode_Mask = QRCODE_MASK_AUTO;
//	#define QRCODE_MASK_CURRENT			g_QRCode_Mask
//#else
//	#define QRCODE_MASK_CURRENT			QRCODE_MASK
//#endif


#define NULL						 ((void *)0)
#define LONG_MAX					2147483647L // 0x7FFFFFFF

int abs(int a)
{
	return  (a > 0) ? a : -a;
}

long labs(long a)
{
	return  (a > 0) ? a : -a;
}


u16 strlen(const c8* str)
{
	u16 ret = 0;
	while (*str++)
		ret++;	
	return ret;
}

const c8* strchr(const c8* str, c8 chr)
{
	while (*str)
	{
		if (*str == chr)
			return str;
		str++;
	}
	return NULL;
}

void* memset(void* src, u8 val, u16 num)
{
	u8* d = (u8*)src;
	for (u16 i = 0; i < num; i++)
	{
		*d = val;
		d++;
	}
	return src;
}

static u8 g_Buffer[1024];

void* memmove(void* dst, const void* src, u16 num)
{
	u8* tmp = g_Buffer;
	const u8* s = (const u8*)src;
	u8* d = tmp;
	for (u16 i = 0; i < num; i++)
	{
		*d = *s;
		s++;
		d++;
	}

	s = tmp;
	d = (u8*)dst;
	for (u16 i = 0; i < num; i++)
	{
		*d = *s;
		s++;
		d++;
	}
	return dst;
}

void* memcpy(void* dst, const void* src, u16 num)
{
	const u8* s = (const u8*)src;
	u8* d = (u8*)dst;
	for (u16 i = 0; i < num; i++)
	{
		*d = *s;
		s++;
		d++;
	}
	return dst;
}

/*---- Forward declarations for private functions ----*/

// Regarding all public and private functions defined in this source file:
// - They require all pointer/array arguments to be not null unless the array length is zero.
// - They only read input scalar/array arguments, write to output pointer/array
//   arguments, and return scalar values; they are "pure" functions.
// - They don't read mutable global variables or write to any global variables.
// - They don't perform I/O, read the clock, print to console, etc.
// - They allocate a small and constant amount of stack memory.
// - They don't allocate or free any memory on the heap.
// - They don't recurse or mutually recurse. All the code
//   could be inlined into the top-level public functions.
// - They run in at most quadratic time with respect to input arguments.
//   Most functions run in linear time, and some in constant time.
//   There are no unbounded loops or non-obvious termination conditions.
// - They are completely thread-safe if the caller does not give the
//   same writable buffer to concurrent calls to these functions.

void appendBitsToBuffer(u16 val, i16 numBits, u8 buffer[], i16 *bitLen);

void addEccAndInterleave(u8 data[], i16 version, enum QRCODE_ECC ecl, u8 result[]);
i16 getNumDataCodewords(i16 version, enum QRCODE_ECC ecl);
i16 getNumRawDataModules(i16 ver);

void reedSolomonComputeDivisor(i16 degree, u8 result[]);
void reedSolomonComputeRemainder(const u8 data[], i16 dataLen, const u8 generator[], i16 degree, u8 result[]);
u8 reedSolomonMultiply(u8 x, u8 y);

void initializeFunctionModules(i16 version, u8 qrcode[]);
static void drawLightFunctionModules(u8 qrcode[], i16 version);
static void drawFormatBits(enum QRCODE_ECC ecl, enum QRCODE_MASK mask, u8 qrcode[]);
i16 getAlignmentPatternPositions(i16 version, u8 result[7]);
static void fillRectangle(i16 left, i16 top, i16 width, i16 height, u8 qrcode[]);

static void drawCodewords(const u8 data[], i16 dataLen, u8 qrcode[]);
static void applyMask(const u8 functionModules[], u8 qrcode[], enum QRCODE_MASK mask);
static long getPenaltyScore(const u8 qrcode[]);
static i16 finderPenaltyCountPatterns(const i16 runHistory[7], i16 qrsize);
static i16 finderPenaltyTerminateAndCount(bool currentRunColor, i16 currentRunLength, i16 runHistory[7], i16 qrsize);
static void finderPenaltyAddHistory(i16 currentRunLength, i16 runHistory[7], i16 qrsize);

bool getModuleBounded(const u8 qrcode[], i16 x, i16 y);
void setModuleBounded(u8 qrcode[], i16 x, i16 y, bool isDark);
void setModuleUnbounded(u8 qrcode[], i16 x, i16 y, bool isDark);
static bool getBit(i16 x, i16 i);

i16 calcSegmentBitLength(enum QRCODE_MODE mode, u16 numChars);
i16 getTotalBits(const struct QRCode_Segment segs[], u16 len, i16 version);
static i16 numCharCountBits(enum QRCODE_MODE mode, i16 version);



/*---- Private tables of constants ----*/

// The set of all legal characters in alphanumeric mode, where each character
// value maps to the index in the string. For checking text and encoding segments.
static const char *ALPHANUMERIC_CHARSET = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";

// Sentinel value for use in only some functions.
#define LENGTH_OVERFLOW -1

// For generating error correction codes.
const i8 ECC_CODEWORDS_PER_BLOCK[4][41] = 
{
	// Version: (note that index 0 is for padding, and is set to an illegal value)
	//0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
	{-1,  7, 10, 15, 20, 26, 18, 20, 24, 30, 18, 20, 24, 26, 30, 22, 24, 28, 30, 28, 28, 28, 28, 30, 30, 26, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // Low
	{-1, 10, 16, 26, 18, 24, 16, 18, 22, 22, 26, 30, 22, 22, 24, 24, 28, 28, 26, 26, 26, 26, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28},  // Medium
	{-1, 13, 22, 18, 26, 18, 24, 18, 22, 20, 24, 28, 26, 24, 20, 30, 24, 28, 28, 26, 30, 28, 30, 30, 30, 30, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // Quartile
	{-1, 17, 28, 22, 16, 22, 28, 26, 26, 24, 28, 24, 28, 22, 24, 24, 30, 28, 28, 26, 28, 30, 24, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // High
};

#define QRCode_REED_SOLOMON_DEGREE_MAX 30  // Based on the table above

// For generating error correction codes.
const i8 NUM_ERROR_CORRECTION_BLOCKS[4][41] = 
{
	// Version: (note that index 0 is for padding, and is set to an illegal value)
	//0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
	{-1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 4,  4,  4,  4,  4,  6,  6,  6,  6,  7,  8,  8,  9,  9, 10, 12, 12, 12, 13, 14, 15, 16, 17, 18, 19, 19, 20, 21, 22, 24, 25},  // Low
	{-1, 1, 1, 1, 2, 2, 4, 4, 4, 5, 5,  5,  8,  9,  9, 10, 10, 11, 13, 14, 16, 17, 17, 18, 20, 21, 23, 25, 26, 28, 29, 31, 33, 35, 37, 38, 40, 43, 45, 47, 49},  // Medium
	{-1, 1, 1, 2, 2, 4, 4, 6, 6, 8, 8,  8, 10, 12, 16, 12, 17, 16, 18, 21, 20, 23, 23, 25, 27, 29, 34, 34, 35, 38, 40, 43, 45, 48, 51, 53, 56, 59, 62, 65, 68},  // Quartile
	{-1, 1, 1, 2, 4, 4, 4, 5, 6, 8, 8, 11, 11, 16, 16, 18, 16, 19, 21, 25, 25, 25, 34, 30, 32, 35, 37, 40, 42, 45, 48, 51, 54, 57, 60, 63, 66, 70, 74, 77, 81},  // High
};

// For automatic mask pattern selection.
static const i16 PENALTY_N1 =  3;
static const i16 PENALTY_N2 =  3;
static const i16 PENALTY_N3 = 40;
static const i16 PENALTY_N4 = 10;



/*---- High-level QR Code encoding functions ----*/

// Public function - see documentation comment in header file.
bool QRCode_EncodeText(const char *text, u8 tempBuffer[], u8 qrcode[], enum QRCODE_ECC ecl, enum QRCODE_MASK mask, bool boostEcl)
{
	u16 textLen = (u16)strlen(text);
	if (textLen == 0)
		return QRCode_encodeSegmentsAdvanced(NULL, 0, ecl, mask, boostEcl, tempBuffer, qrcode);
	u16 bufLen = (u16)QRCODE_BUFFER_LEN_FOR_VERSION(QRCODE_VERSION_CURRENT_MAX);
	
	struct QRCode_Segment seg;
	if (QRCode_isNumeric(text))
	{
		if (QRCode_CalcSegmentBufferSize(QRCODE_MODE_NUMERIC, textLen) > bufLen)
			goto fail;
		QRCode_MakeNumeric(text, tempBuffer, &seg);
	}
	else if (QRCode_isAlphanumeric(text))
	{
		if (QRCode_CalcSegmentBufferSize(QRCODE_MODE_ALPHANUMERIC, textLen) > bufLen)
			goto fail;
		QRCode_MakeAlphanumeric(text, tempBuffer, &seg);
	}
	else
	{
		if (textLen > bufLen)
			goto fail;
		for (u16 i = 0; i < textLen; i++)
			tempBuffer[i] = (u8)text[i];
		seg.mode = QRCODE_MODE_BYTE;
		seg.bitLength = calcSegmentBitLength(seg.mode, textLen);
		if (seg.bitLength == LENGTH_OVERFLOW)
			goto fail;
		seg.numChars = (i16)textLen;
		seg.data = tempBuffer;
	}
	return QRCode_encodeSegmentsAdvanced(&seg, 1, ecl, mask, boostEcl, tempBuffer, qrcode);
	
fail:
	qrcode[0] = 0;  // Set size to invalid value for safety
	return FALSE;
}


#if (QRCODE_USE_EXTRA)
// Public function - see documentation comment in header file.
bool QRCode_EncodeBinary(u8 dataAndTemp[], u16 dataLen, u8 qrcode[], enum QRCODE_ECC ecl, enum QRCODE_MASK mask, bool boostEcl)
{	
	struct QRCode_Segment seg;
	seg.mode = QRCODE_MODE_BYTE;
	seg.bitLength = calcSegmentBitLength(seg.mode, dataLen);
	if (seg.bitLength == LENGTH_OVERFLOW)
	{
		qrcode[0] = 0;  // Set size to invalid value for safety
		return FALSE;
	}
	seg.numChars = (i16)dataLen;
	seg.data = dataAndTemp;
	return QRCode_encodeSegmentsAdvanced(&seg, 1, ecl, mask, boostEcl, dataAndTemp, qrcode);
}
#endif

// Appends the given number of low-order bits of the given value to the given byte-based
// bit buffer, increasing the bit length. Requires 0 <= numBits <= 16 and val < 2^numBits.
void appendBitsToBuffer(u16 val, i16 numBits, u8 buffer[], i16 *bitLen)
{
	assert(0 <= numBits && numBits <= 16 && (unsigned long)val >> numBits == 0);
	for (i16 i = numBits - 1; i >= 0; i--, (*bitLen)++)
		buffer[*bitLen >> 3] |= ((val >> i) & 1) << (7 - (*bitLen & 7));
}



/*---- Low-level QR Code encoding functions ----*/

#if (QRCODE_USE_EXTRA)
// Public function - see documentation comment in header file.
bool QRCode_encodeSegments(const struct QRCode_Segment segs[], u16 len, enum QRCODE_ECC ecl, u8 tempBuffer[], u8 qrcode[])
{
	return QRCode_encodeSegmentsAdvanced(segs, len, ecl, QRCODE_MASK_AUTO, TRUE, tempBuffer, qrcode);
}
#endif

// Public function - see documentation comment in header file.
bool QRCode_encodeSegmentsAdvanced(const struct QRCode_Segment segs[], u16 len, enum QRCODE_ECC ecl, enum QRCODE_MASK mask, bool boostEcl, u8 tempBuffer[], u8 qrcode[])
{
	assert(segs != NULL || len == 0);
	assert(QRCODE_VERSION_MIN <= QRCODE_VERSION_CURRENT_MIN && QRCODE_VERSION_CURRENT_MIN <= QRCODE_VERSION_CURRENT_MAX && QRCODE_VERSION_CURRENT_MAX <= QRCODE_VERSION_MAX);
	assert(0 <= (i16)ecl && (i16)ecl <= 3 && -1 <= (i16)mask && (i16)mask <= 7);
	
	// Find the minimal version number to use
	i16 version, dataUsedBits;
	for (version = QRCODE_VERSION_CURRENT_MIN; ; version++)
	{
		i16 dataCapacityBits = getNumDataCodewords(version, ecl) * 8;  // Number of data bits available
		dataUsedBits = getTotalBits(segs, len, version);
		if (dataUsedBits != LENGTH_OVERFLOW && dataUsedBits <= dataCapacityBits)
			break;  // This version number is found to be suitable
		if (version >= QRCODE_VERSION_CURRENT_MAX)
		{  // All versions in the range could not fit the given data
			qrcode[0] = 0;  // Set size to invalid value for safety
			return FALSE;
		}
	}
	assert(dataUsedBits != LENGTH_OVERFLOW);
	
	// Increase the error correction level while the data still fits in the current version number
	for (i16 i = (i16)QRCODE_ECC_MEDIUM; i <= (i16)QRCODE_ECC_HIGH; i++)
	{  // From low to high
		if (boostEcl && dataUsedBits <= getNumDataCodewords(version, (enum QRCODE_ECC)i) * 8)
			ecl = (enum QRCODE_ECC)i;
	}
	
	// Concatenate all segments to create the data bit string
	memset(qrcode, 0, (u16)QRCODE_BUFFER_LEN_FOR_VERSION(version) * sizeof(qrcode[0]));
	i16 bitLen = 0;
	for (u16 i = 0; i < len; i++)
	{
		const struct QRCode_Segment *seg = &segs[i];
		appendBitsToBuffer((u16)seg->mode, 4, qrcode, &bitLen);
		appendBitsToBuffer((u16)seg->numChars, numCharCountBits(seg->mode, version), qrcode, &bitLen);
		for (i16 j = 0; j < seg->bitLength; j++)
		{
			i16 bit = (seg->data[j >> 3] >> (7 - (j & 7))) & 1;
			appendBitsToBuffer((u16)bit, 1, qrcode, &bitLen);
		}
	}
	assert(bitLen == dataUsedBits);
	
	// Add terminator and pad up to a byte if applicable
	i16 dataCapacityBits = getNumDataCodewords(version, ecl) * 8;
	assert(bitLen <= dataCapacityBits);
	i16 terminatorBits = dataCapacityBits - bitLen;
	if (terminatorBits > 4)
		terminatorBits = 4;
	appendBitsToBuffer(0, terminatorBits, qrcode, &bitLen);
	appendBitsToBuffer(0, (8 - bitLen % 8) % 8, qrcode, &bitLen);
	assert(bitLen % 8 == 0);
	
	// Pad with alternating bytes until data capacity is reached
	for (u8 padByte = 0xEC; bitLen < dataCapacityBits; padByte ^= 0xEC ^ 0x11)
		appendBitsToBuffer(padByte, 8, qrcode, &bitLen);
	
	// Compute ECC, draw modules
	addEccAndInterleave(qrcode, version, ecl, tempBuffer);
	initializeFunctionModules(version, qrcode);
	drawCodewords(tempBuffer, getNumRawDataModules(version) / 8, qrcode);
	drawLightFunctionModules(qrcode, version);
	initializeFunctionModules(version, tempBuffer);
	
	// Do masking
	if (mask == QRCODE_MASK_AUTO)
	{  // Automatically choose best mask
		long minPenalty = LONG_MAX;
		for (i16 i = 0; i < 8; i++)
		{
			enum QRCODE_MASK msk = (enum QRCODE_MASK)i;
			applyMask(tempBuffer, qrcode, msk);
			drawFormatBits(ecl, msk, qrcode);
			long penalty = getPenaltyScore(qrcode);
			if (penalty < minPenalty)
			{
				mask = msk;
				minPenalty = penalty;
			}
			applyMask(tempBuffer, qrcode, msk);  // Undoes the mask due to XOR
		}
	}
	assert(0 <= (i16)mask && (i16)mask <= 7);
	applyMask(tempBuffer, qrcode, mask);  // Apply the final choice of mask
	drawFormatBits(ecl, mask, qrcode);  // Overwrite old format bits
	return TRUE;
}



/*---- Error correction code generation functions ----*/

// Appends error correction bytes to each block of the given data array, then interleaves
// bytes from the blocks and stores them in the result array. data[0 : dataLen] contains
// the input data. data[dataLen : rawCodewords] is used as a temporary work area and will
// be clobbered by this function. The final answer is stored in result[0 : rawCodewords].
void addEccAndInterleave(u8 data[], i16 version, enum QRCODE_ECC ecl, u8 result[])
{
	// Calculate parameter numbers
	assert(0 <= (i16)ecl && (i16)ecl < 4 && QRCODE_VERSION_MIN <= version && version <= QRCODE_VERSION_MAX);
	i16 numBlocks = NUM_ERROR_CORRECTION_BLOCKS[(i16)ecl][version];
	i16 blockEccLen = ECC_CODEWORDS_PER_BLOCK  [(i16)ecl][version];
	i16 rawCodewords = getNumRawDataModules(version) / 8;
	i16 dataLen = getNumDataCodewords(version, ecl);
	i16 numShortBlocks = numBlocks - rawCodewords % numBlocks;
	i16 shortBlockDataLen = rawCodewords / numBlocks - blockEccLen;
	
	// Split data into blocks, calculate ECC, and interleave
	// (not concatenate) the bytes into a single sequence
	u8 rsdiv[QRCode_REED_SOLOMON_DEGREE_MAX];
	reedSolomonComputeDivisor(blockEccLen, rsdiv);
	const u8 *dat = data;
	for (i16 i = 0; i < numBlocks; i++)
	{
		i16 datLen = shortBlockDataLen + (i < numShortBlocks ? 0 : 1);
		u8 *ecc = &data[dataLen];  // Temporary storage
		reedSolomonComputeRemainder(dat, datLen, rsdiv, blockEccLen, ecc);
		for (i16 j = 0, k = i; j < datLen; j++, k += numBlocks)
		{  // Copy data
			if (j == shortBlockDataLen)
				k -= numShortBlocks;
			result[k] = dat[j];
		}
		for (i16 j = 0, k = dataLen + i; j < blockEccLen; j++, k += numBlocks)  // Copy ECC
			result[k] = ecc[j];
		dat += datLen;
	}
}


// Returns the number of 8-bit codewords that can be used for storing data (not ECC),
// for the given version number and error correction level. The result is in the range [9, 2956].
i16 getNumDataCodewords(i16 version, enum QRCODE_ECC ecl)
{
	i16 v = version, e = (i16)ecl;
	assert(0 <= e && e < 4);
	return getNumRawDataModules(v) / 8 - ECC_CODEWORDS_PER_BLOCK[e][v] * NUM_ERROR_CORRECTION_BLOCKS[e][v];
}


// Returns the number of data bits that can be stored in a QR Code of the given version number, after
// all function modules are excluded. This includes remainder bits, so it might not be a multiple of 8.
// The result is in the range [208, 29648]. This could be implemented as a 40-entry lookup table.
i16 getNumRawDataModules(i16 ver)
{
	assert(QRCODE_VERSION_MIN <= ver && ver <= QRCODE_VERSION_MAX);
	i16 result = (16 * ver + 128) * ver + 64;
	if (ver >= 2)
	{
		i16 numAlign = ver / 7 + 2;
		result -= (25 * numAlign - 10) * numAlign - 55;
		if (ver >= 7)
			result -= 36;
	}
	assert(208 <= result && result <= 29648);
	return result;
}



/*---- Reed-Solomon ECC generator functions ----*/

// Computes a Reed-Solomon ECC generator polynomial for the given degree, storing in result[0 : degree].
// This could be implemented as a lookup table over all possible parameter values, instead of as an algorithm.
void reedSolomonComputeDivisor(i16 degree, u8 result[])
{
	assert(1 <= degree && degree <= QRCode_REED_SOLOMON_DEGREE_MAX);
	// Polynomial coefficients are stored from highest to lowest power, excluding the leading term which is always 1.
	// For example the polynomial x^3 + 255x^2 + 8x + 93 is stored as the uint8 array {255, 8, 93}.
	memset(result, 0, (u16)degree * sizeof(result[0]));
	result[degree - 1] = 1;  // Start off with the monomial x^0
	
	// Compute the product polynomial (x - r^0) * (x - r^1) * (x - r^2) * ... * (x - r^{degree-1}),
	// drop the highest monomial term which is always 1x^degree.
	// Note that r = 0x02, which is a generator element of this field GF(2^8/0x11D).
	u8 root = 1;
	for (i16 i = 0; i < degree; i++)
	{
		// Multiply the current product by (x - r^i)
		for (i16 j = 0; j < degree; j++)
		{
			result[j] = reedSolomonMultiply(result[j], root);
			if (j + 1 < degree)
				result[j] ^= result[j + 1];
		}
		root = reedSolomonMultiply(root, 0x02);
	}
}


// Computes the Reed-Solomon error correction codeword for the given data and divisor polynomials.
// The remainder when data[0 : dataLen] is divided by divisor[0 : degree] is stored in result[0 : degree].
// All polynomials are in big endian, and the generator has an implicit leading 1 term.
void reedSolomonComputeRemainder(const u8 data[], i16 dataLen, const u8 generator[], i16 degree, u8 result[])
{
	assert(1 <= degree && degree <= QRCode_REED_SOLOMON_DEGREE_MAX);
	memset(result, 0, (u16)degree * sizeof(result[0]));
	for (i16 i = 0; i < dataLen; i++)
	{  // Polynomial division
		u8 factor = data[i] ^ result[0];
		memmove(&result[0], &result[1], (u16)(degree - 1) * sizeof(result[0]));
		result[degree - 1] = 0;
		for (i16 j = 0; j < degree; j++)
			result[j] ^= reedSolomonMultiply(generator[j], factor);
	}
}

#undef QRCode_REED_SOLOMON_DEGREE_MAX


// Returns the product of the two given field elements modulo GF(2^8/0x11D).
// All inputs are valid. This could be implemented as a 256*256 lookup table.
u8 reedSolomonMultiply(u8 x, u8 y)
{
	// Russian peasant multiplication
	u8 z = 0;
	for (i16 i = 7; i >= 0; i--) {
		z = (u8)((z << 1) ^ ((z >> 7) * 0x11D));
		z ^= ((y >> i) & 1) * x;
	}
	return z;
}



/*---- Drawing function modules ----*/

// Clears the given QR Code grid with light modules for the given
// version's size, then marks every function module as dark.
void initializeFunctionModules(i16 version, u8 qrcode[])
{
	// Initialize QR Code
	i16 qrsize = version * 4 + 17;
	memset(qrcode, 0, (u16)((qrsize * qrsize + 7) / 8 + 1) * sizeof(qrcode[0]));
	qrcode[0] = (u8)qrsize;
	
	// Fill horizontal and vertical timing patterns
	fillRectangle(6, 0, 1, qrsize, qrcode);
	fillRectangle(0, 6, qrsize, 1, qrcode);
	
	// Fill 3 finder patterns (all corners except bottom right) and format bits
	fillRectangle(0, 0, 9, 9, qrcode);
	fillRectangle(qrsize - 8, 0, 8, 9, qrcode);
	fillRectangle(0, qrsize - 8, 9, 8, qrcode);
	
	// Fill numerous alignment patterns
	u8 alignPatPos[7];
	i16 numAlign = getAlignmentPatternPositions(version, alignPatPos);
	for (i16 i = 0; i < numAlign; i++)
	{
		for (i16 j = 0; j < numAlign; j++)
		{
			// Don't draw on the three finder corners
			if (!((i == 0 && j == 0) || (i == 0 && j == numAlign - 1) || (i == numAlign - 1 && j == 0)))
				fillRectangle(alignPatPos[i] - 2, alignPatPos[j] - 2, 5, 5, qrcode);
		}
	}
	
	// Fill version blocks
	if (version >= 7)
	{
		fillRectangle(qrsize - 11, 0, 3, 6, qrcode);
		fillRectangle(0, qrsize - 11, 6, 3, qrcode);
	}
}


// Draws light function modules and possibly some dark modules onto the given QR Code, without changing
// non-function modules. This does not draw the format bits. This requires all function modules to be previously
// marked dark (namely by initializeFunctionModules()), because this may skip redrawing dark function modules.
static void drawLightFunctionModules(u8 qrcode[], i16 version)
{
	// Draw horizontal and vertical timing patterns
	u8 qrsize = QRCode_GetSize(qrcode);
	for (i16 i = 7; i < qrsize - 7; i += 2)
	{
		setModuleBounded(qrcode, 6, i, FALSE);
		setModuleBounded(qrcode, i, 6, FALSE);
	}
	
	// Draw 3 finder patterns (all corners except bottom right; overwrites some timing modules)
	for (i16 dy = -4; dy <= 4; dy++)
	{
		for (i16 dx = -4; dx <= 4; dx++)
		{
			i16 dist = abs(dx);
			if (abs(dy) > dist)
				dist = abs(dy);
			if (dist == 2 || dist == 4)
			{
				setModuleUnbounded(qrcode, 3 + dx, 3 + dy, FALSE);
				setModuleUnbounded(qrcode, qrsize - 4 + dx, 3 + dy, FALSE);
				setModuleUnbounded(qrcode, 3 + dx, qrsize - 4 + dy, FALSE);
			}
		}
	}
	
	// Draw numerous alignment patterns
	u8 alignPatPos[7];
	i16 numAlign = getAlignmentPatternPositions(version, alignPatPos);
	for (i16 i = 0; i < numAlign; i++)
	{
		for (i16 j = 0; j < numAlign; j++)
		{
			if ((i == 0 && j == 0) || (i == 0 && j == numAlign - 1) || (i == numAlign - 1 && j == 0))
				continue;  // Don't draw on the three finder corners
			for (i16 dy = -1; dy <= 1; dy++)
			{
				for (i16 dx = -1; dx <= 1; dx++)
					setModuleBounded(qrcode, alignPatPos[i] + dx, alignPatPos[j] + dy, dx == 0 && dy == 0);
			}
		}
	}
	
	// Draw version blocks
	if (version >= 7)
	{
		// Calculate error correction code and pack bits
		i16 rem = version;  // version is uint6, in the range [7, 40]
		for (i16 i = 0; i < 12; i++)
			rem = (rem << 1) ^ ((rem >> 11) * 0x1F25);
		long bits = (long)version << 12 | rem;  // uint18
		assert(bits >> 18 == 0);
		
		// Draw two copies
		for (i16 i = 0; i < 6; i++)
		{
			for (i16 j = 0; j < 3; j++)
			{
				i16 k = qrsize - 11 + j;
				setModuleBounded(qrcode, k, i, (bits & 1) != 0);
				setModuleBounded(qrcode, i, k, (bits & 1) != 0);
				bits >>= 1;
			}
		}
	}
}


// Draws two copies of the format bits (with its own error correction code) based
// on the given mask and error correction level. This always draws all modules of
// the format bits, unlike drawLightFunctionModules() which might skip dark modules.
static void drawFormatBits(enum QRCODE_ECC ecl, enum QRCODE_MASK mask, u8 qrcode[])
{
	// Calculate error correction code and pack bits
	assert(0 <= (i16)mask && (i16)mask <= 7);
	static const i16 table[] = {1, 0, 3, 2};
	i16 data = table[(i16)ecl] << 3 | (i16)mask;  // errCorrLvl is uint2, mask is uint3
	i16 rem = data;
	for (i16 i = 0; i < 10; i++)
		rem = (rem << 1) ^ ((rem >> 9) * 0x537);
	i16 bits = (data << 10 | rem) ^ 0x5412;  // uint15
	assert(bits >> 15 == 0);
	
	// Draw first copy
	for (i16 i = 0; i <= 5; i++)
		setModuleBounded(qrcode, 8, i, getBit(bits, i));
	setModuleBounded(qrcode, 8, 7, getBit(bits, 6));
	setModuleBounded(qrcode, 8, 8, getBit(bits, 7));
	setModuleBounded(qrcode, 7, 8, getBit(bits, 8));
	for (i16 i = 9; i < 15; i++)
		setModuleBounded(qrcode, 14 - i, 8, getBit(bits, i));
	
	// Draw second copy
	u8 qrsize = QRCode_GetSize(qrcode);
	for (i16 i = 0; i < 8; i++)
		setModuleBounded(qrcode, qrsize - 1 - i, 8, getBit(bits, i));
	for (i16 i = 8; i < 15; i++)
		setModuleBounded(qrcode, 8, qrsize - 15 + i, getBit(bits, i));
	setModuleBounded(qrcode, 8, qrsize - 8, TRUE);  // Always dark
}


// Calculates and stores an ascending list of positions of alignment patterns
// for this version number, returning the length of the list (in the range [0,7]).
// Each position is in the range [0,177), and are used on both the x and y axes.
// This could be implemented as lookup table of 40 variable-length lists of unsigned bytes.
i16 getAlignmentPatternPositions(i16 version, u8 result[7]) 
{
	if (version == 1)
		return 0;
	i16 numAlign = version / 7 + 2;
	i16 step = (version == 32) ? 26 :
		(version * 4 + numAlign * 2 + 1) / (numAlign * 2 - 2) * 2;
	for (i16 i = numAlign - 1, pos = version * 4 + 10; i >= 1; i--, pos -= step)
		result[i] = (u8)pos;
	result[0] = 6;
	return numAlign;
}


// Sets every module in the range [left : left + width] * [top : top + height] to dark.
static void fillRectangle(i16 left, i16 top, i16 width, i16 height, u8 qrcode[]) 
{
	for (i16 dy = 0; dy < height; dy++) 
	{
		for (i16 dx = 0; dx < width; dx++)
			setModuleBounded(qrcode, left + dx, top + dy, TRUE);
	}
}



/*---- Drawing data modules and masking ----*/

// Draws the raw codewords (including data and ECC) onto the given QR Code. This requires the initial state of
// the QR Code to be dark at function modules and light at codeword modules (including unused remainder bits).
static void drawCodewords(const u8 data[], i16 dataLen, u8 qrcode[]) 
{
	u8 qrsize = QRCode_GetSize(qrcode);
	i16 i = 0;  // Bit index into the data
	// Do the funny zigzag scan
	for (i16 right = qrsize - 1; right >= 1; right -= 2) 
	{  // Index of right column in each column pair
		if (right == 6)
			right = 5;
		for (i16 vert = 0; vert < qrsize; vert++) 
		{  // Vertical counter
			for (i16 j = 0; j < 2; j++) 
			{
				i16 x = right - j;  // Actual x coordinate
				bool upward = ((right + 1) & 2) == 0;
				i16 y = upward ? qrsize - 1 - vert : vert;  // Actual y coordinate
				if (!getModuleBounded(qrcode, x, y) && i < dataLen * 8) 
				{
					bool dark = getBit(data[i >> 3], 7 - (i & 7));
					setModuleBounded(qrcode, x, y, dark);
					i++;
				}
				// If this QR Code has any remainder bits (0 to 7), they were assigned as
				// 0/FALSE/light by the constructor and are left unchanged by this method
			}
		}
	}
	assert(i == dataLen * 8);
}


// XORs the codeword modules in this QR Code with the given mask pattern
// and given pattern of function modules. The codeword bits must be drawn
// before masking. Due to the arithmetic of XOR, calling applyMask() with
// the same mask value a second time will undo the mask. A final well-formed
// QR Code needs exactly one (not zero, two, etc.) mask applied.
static void applyMask(const u8 functionModules[], u8 qrcode[], enum QRCODE_MASK mask) 
{
	assert(0 <= (i16)mask && (i16)mask <= 7);  // Disallows QRCODE_MASK_AUTO
	u8 qrsize = QRCode_GetSize(qrcode);
	for (i16 y = 0; y < qrsize; y++) 
	{
		for (i16 x = 0; x < qrsize; x++) 
		{
			if (getModuleBounded(functionModules, x, y))
				continue;
			bool invert;
			switch ((i16)mask) 
			{
				case 0:  invert = (x + y) % 2 == 0;                    break;
				case 1:  invert = y % 2 == 0;                          break;
				case 2:  invert = x % 3 == 0;                          break;
				case 3:  invert = (x + y) % 3 == 0;                    break;
				case 4:  invert = (x / 3 + y / 2) % 2 == 0;            break;
				case 5:  invert = x * y % 2 + x * y % 3 == 0;          break;
				case 6:  invert = (x * y % 2 + x * y % 3) % 2 == 0;    break;
				case 7:  invert = ((x + y) % 2 + x * y % 3) % 2 == 0;  break;
				default:  assert(FALSE);  return;
			}
			bool val = getModuleBounded(qrcode, x, y);
			setModuleBounded(qrcode, x, y, val ^ invert);
		}
	}
}


// Calculates and returns the penalty score based on state of the given QR Code's current modules.
// This is used by the automatic mask choice algorithm to find the mask pattern that yields the lowest score.
static long getPenaltyScore(const u8 qrcode[]) 
{
	u8 qrsize = QRCode_GetSize(qrcode);
	long result = 0;
	
	// Adjacent modules in row having same color, and finder-like patterns
	for (i16 y = 0; y < qrsize; y++) 
	{
		bool runColor = FALSE;
		i16 runX = 0;
		i16 runHistory[7] = {0};
		for (i16 x = 0; x < qrsize; x++) 
		{
			if (getModuleBounded(qrcode, x, y) == runColor) 
			{
				runX++;
				if (runX == 5)
					result += PENALTY_N1;
				else if (runX > 5)
					result++;
			}
			else 
			{
				finderPenaltyAddHistory(runX, runHistory, qrsize);
				if (!runColor)
					result += finderPenaltyCountPatterns(runHistory, qrsize) * PENALTY_N3;
				runColor = getModuleBounded(qrcode, x, y);
				runX = 1;
			}
		}
		result += finderPenaltyTerminateAndCount(runColor, runX, runHistory, qrsize) * PENALTY_N3;
	}
	// Adjacent modules in column having same color, and finder-like patterns
	for (i16 x = 0; x < qrsize; x++) 
	{
		bool runColor = FALSE;
		i16 runY = 0;
		i16 runHistory[7] = {0};
		for (i16 y = 0; y < qrsize; y++) 
		{
			if (getModuleBounded(qrcode, x, y) == runColor) 
			{
				runY++;
				if (runY == 5)
					result += PENALTY_N1;
				else if (runY > 5)
					result++;
			}
			else
			{
				finderPenaltyAddHistory(runY, runHistory, qrsize);
				if (!runColor)
					result += finderPenaltyCountPatterns(runHistory, qrsize) * PENALTY_N3;
				runColor = getModuleBounded(qrcode, x, y);
				runY = 1;
			}
		}
		result += finderPenaltyTerminateAndCount(runColor, runY, runHistory, qrsize) * PENALTY_N3;
	}
	
	// 2*2 blocks of modules having same color
	for (i16 y = 0; y < qrsize - 1; y++) 
	{
		for (i16 x = 0; x < qrsize - 1; x++) 
		{
			bool  color = getModuleBounded(qrcode, x, y);
			if (  color == getModuleBounded(qrcode, x + 1, y) &&
			      color == getModuleBounded(qrcode, x, y + 1) &&
			      color == getModuleBounded(qrcode, x + 1, y + 1))
				result += PENALTY_N2;
		}
	}
	
	// Balance of dark and light modules
	i16 dark = 0;
	for (i16 y = 0; y < qrsize; y++) 
	{
		for (i16 x = 0; x < qrsize; x++) 
		{
			if (getModuleBounded(qrcode, x, y))
				dark++;
		}
	}
	i16 total = qrsize * qrsize;  // Note that size is odd, so dark/total != 1/2
	// Compute the smallest integer k >= 0 such that (45-5k)% <= dark/total <= (55+5k)%
	i16 k = (i16)((labs(dark * 20L - total * 10L) + total - 1) / total) - 1;
	assert(0 <= k && k <= 9);
	result += k * PENALTY_N4;
	assert(0 <= result && result <= 2568888L);  // Non-tight upper bound based on default values of PENALTY_N1, ..., N4
	return result;
}


// Can only be called immediately after a light run is added, and
// returns either 0, 1, or 2. A helper function for getPenaltyScore().
static i16 finderPenaltyCountPatterns(const i16 runHistory[7], i16 qrsize) 
{
	i16 n = runHistory[1];
	assert(n <= qrsize * 3);  (void)qrsize;
	bool core = n > 0 && runHistory[2] == n && runHistory[3] == n * 3 && runHistory[4] == n && runHistory[5] == n;
	// The maximum QR Code size is 177, hence the dark run length n <= 177.
	// Arithmetic is promoted to i16, so n*4 will not overflow.
	return (core && runHistory[0] >= n * 4 && runHistory[6] >= n ? 1 : 0)
	     + (core && runHistory[6] >= n * 4 && runHistory[0] >= n ? 1 : 0);
}


// Must be called at the end of a line (row or column) of modules. A helper function for getPenaltyScore().
static i16 finderPenaltyTerminateAndCount(bool currentRunColor, i16 currentRunLength, i16 runHistory[7], i16 qrsize) 
{
	if (currentRunColor) 
	{  // Terminate dark run
		finderPenaltyAddHistory(currentRunLength, runHistory, qrsize);
		currentRunLength = 0;
	}
	currentRunLength += qrsize;  // Add light border to final run
	finderPenaltyAddHistory(currentRunLength, runHistory, qrsize);
	return finderPenaltyCountPatterns(runHistory, qrsize);
}


// Pushes the given value to the front and drops the last value. A helper function for getPenaltyScore().
static void finderPenaltyAddHistory(i16 currentRunLength, i16 runHistory[7], i16 qrsize) 
{
	if (runHistory[0] == 0)
		currentRunLength += qrsize;  // Add light border to initial run
	memmove(&runHistory[1], &runHistory[0], 6 * sizeof(runHistory[0]));
	runHistory[0] = currentRunLength;
}



/*---- Basic QR Code information ----*/

// Public function - see documentation comment in header file.
u8 QRCode_GetSize(const u8 qrcode[])
{
	assert(qrcode != NULL);
	u8 result = qrcode[0];
	assert((QRCODE_VERSION_MIN * 4 + 17) <= result
		&& result <= (QRCODE_VERSION_MAX * 4 + 17));
	return result;
}


// Public function - see documentation comment in header file.
bool QRCode_GetModule(const u8 qrcode[], i16 x, i16 y)
{
	assert(qrcode != NULL);
	i16 qrsize = qrcode[0];
	return (0 <= x && x < qrsize && 0 <= y && y < qrsize) && getModuleBounded(qrcode, x, y);
}


// Returns the color of the module at the given coordinates, which must be in bounds.
bool getModuleBounded(const u8 qrcode[], i16 x, i16 y)
{
	i16 qrsize = qrcode[0];
	assert(21 <= qrsize && qrsize <= 177 && 0 <= x && x < qrsize && 0 <= y && y < qrsize);
	i16 index = y * qrsize + x;
	return getBit(qrcode[(index >> 3) + 1], index & 7);
}


// Sets the color of the module at the given coordinates, which must be in bounds.
void setModuleBounded(u8 qrcode[], i16 x, i16 y, bool isDark)
{
	i16 qrsize = qrcode[0];
	assert(21 <= qrsize && qrsize <= 177 && 0 <= x && x < qrsize && 0 <= y && y < qrsize);
	i16 index = y * qrsize + x;
	i16 bitIndex = index & 7;
	i16 byteIndex = (index >> 3) + 1;
	if (isDark)
		qrcode[byteIndex] |= 1 << bitIndex;
	else
		qrcode[byteIndex] &= (1 << bitIndex) ^ 0xFF;
}


// Sets the color of the module at the given coordinates, doing nothing if out of bounds.
void setModuleUnbounded(u8 qrcode[], i16 x, i16 y, bool isDark) 
{
	i16 qrsize = qrcode[0];
	if (0 <= x && x < qrsize && 0 <= y && y < qrsize)
		setModuleBounded(qrcode, x, y, isDark);
}

//static const u16 g_Bits[16] = 
//{
//	0b0000000000000001,
//	0b0000000000000010,
//	0b0000000000000100,
//	0b0000000000001000,
//	0b0000000000010000,
//	0b0000000000100000,
//	0b0000000001000000,
//	0b0000000010000000,
//	0b0000000100000000,
//	0b0000001000000000,
//	0b0000010000000000,
//	0b0000100000000000,
//	0b0001000000000000,
//	0b0010000000000000,
//	0b0100000000000000,
//	0b1000000000000000,
//};

// Returns TRUE iff the i'th bit of x is set to 1. Requires x >= 0 and 0 <= i <= 14.
static bool getBit(i16 x, i16 i) 
{
	return ((x >> i) & 1) != 0;
	//return (g_Bits[i] & x) != 0;
}



/*---- Segment handling ----*/

// Public function - see documentation comment in header file.
bool QRCode_isNumeric(const char *text) 
{
	assert(text != NULL);
	for (; *text != '\0'; text++) 
	{
		if (*text < '0' || *text > '9')
			return FALSE;
	}
	return TRUE;
}


// Public function - see documentation comment in header file.
bool QRCode_isAlphanumeric(const char *text) 
{
	assert(text != NULL);
	for (; *text != '\0'; text++) 
	{
		if (strchr(ALPHANUMERIC_CHARSET, *text) == NULL)
			return FALSE;
	}
	return TRUE;
}


// Public function - see documentation comment in header file.
u16 QRCode_calcSegmentBufferSize(enum QRCODE_MODE mode, u16 numChars)
{
	i16 temp = calcSegmentBitLength(mode, numChars);
	if (temp == LENGTH_OVERFLOW)
		return 0xFFFF;// SIZE_MAX;
	assert(0 <= temp && temp <= INT16_MAX);
	return ((u16)temp + 7) / 8;
}


// Returns the number of data bits needed to represent a segment
// containing the given number of characters using the given mode. Notes:
// - Returns LENGTH_OVERFLOW on failure, i.e. numChars > INT16_MAX
//   or the number of needed bits exceeds INT16_MAX (i.e. 32767).
// - Otherwise, all valid results are in the range [0, INT16_MAX].
// - For byte mode, numChars measures the number of bytes, not Unicode code points.
// - For ECI mode, numChars must be 0, and the worst-case number of bits is returned.
//   An actual ECI segment can have shorter data. For non-ECI modes, the result is exact.
i16 calcSegmentBitLength(enum QRCODE_MODE mode, u16 numChars) 
{
	// All calculations are designed to avoid overflow on all platforms
	if (numChars > (u16)INT16_MAX)
		return LENGTH_OVERFLOW;
	long result = (long)numChars;
	if (mode == QRCODE_MODE_NUMERIC)
		result = (result * 10 + 2) / 3;  // ceil(10/3 * n)
	else if (mode == QRCODE_MODE_ALPHANUMERIC)
		result = (result * 11 + 1) / 2;  // ceil(11/2 * n)
	else if (mode == QRCODE_MODE_BYTE)
		result *= 8;
	else if (mode == QRCODE_MODE_KANJI)
		result *= 13;
	else if (mode == QRCODE_MODE_ECI && numChars == 0)
		result = 3 * 8;
	else 
	{  // Invalid argument
		assert(FALSE);
		return LENGTH_OVERFLOW;
	}
	assert(result >= 0);
	if (result > INT16_MAX)
		return LENGTH_OVERFLOW;
	return (i16)result;
}

#if (QRCODE_USE_EXTRA)
// Public function - see documentation comment in header file.
void QRCode_MakeBytes(const u8 data[], u16 len, u8 buf[], struct QRCode_Segment* seg)
{
	assert(data != NULL || len == 0);
	seg->mode = QRCODE_MODE_BYTE;
	seg->bitLength = calcSegmentBitLength(seg->mode, len);
	assert(seg->bitLength != LENGTH_OVERFLOW);
	seg->numChars = (i16)len;
	if (len > 0)
		memcpy(buf, data, len * sizeof(buf[0]));
	seg->data = buf;
}
#endif

// Public function - see documentation comment in header file.
void QRCode_MakeNumeric(const char *digits, u8 buf[], struct QRCode_Segment* seg)
{
	assert(digits != NULL);
	u16 len = (u16)strlen(digits);
	seg->mode = QRCODE_MODE_NUMERIC;
	i16 bitLen = calcSegmentBitLength(seg->mode, len);
	assert(bitLen != LENGTH_OVERFLOW);
	seg->numChars = (i16)len;
	if (bitLen > 0)
		memset(buf, 0, ((u16)bitLen + 7) / 8 * sizeof(buf[0]));
	seg->bitLength = 0;
	
	u16 accumData = 0;
	i16 accumCount = 0;
	for (; *digits != '\0'; digits++) 
	{
		char c = *digits;
		assert('0' <= c && c <= '9');
		accumData = accumData * 10 + (u16)(c - '0');
		accumCount++;
		if (accumCount == 3) 
		{
			appendBitsToBuffer(accumData, 10, buf, &seg->bitLength);
			accumData = 0;
			accumCount = 0;
		}
	}
	if (accumCount > 0)  // 1 or 2 digits remaining
		appendBitsToBuffer(accumData, accumCount * 3 + 1, buf, &seg->bitLength);
	assert(seg->bitLength == bitLen);
	seg->data = buf;
}


// Public function - see documentation comment in header file.
void QRCode_MakeAlphanumeric(const char *text, u8 buf[], struct QRCode_Segment* seg)
{
	assert(text != NULL);
	u16 len = (u16)strlen(text);
	seg->mode = QRCODE_MODE_ALPHANUMERIC;
	i16 bitLen = calcSegmentBitLength(seg->mode, len);
	assert(bitLen != LENGTH_OVERFLOW);
	seg->numChars = (i16)len;
	if (bitLen > 0)
		memset(buf, 0, ((u16)bitLen + 7) / 8 * sizeof(buf[0]));
	seg->bitLength = 0;
	
	u16 accumData = 0;
	i16 accumCount = 0;
	for (; *text != '\0'; text++) 
	{
		const char *temp = strchr(ALPHANUMERIC_CHARSET, *text);
		assert(temp != NULL);
		accumData = accumData * 45 + (u16)(temp - ALPHANUMERIC_CHARSET);
		accumCount++;
		if (accumCount == 2) 
		{
			appendBitsToBuffer(accumData, 11, buf, &seg->bitLength);
			accumData = 0;
			accumCount = 0;
		}
	}
	if (accumCount > 0)  // 1 character remaining
		appendBitsToBuffer(accumData, 6, buf, &seg->bitLength);
	assert(seg->bitLength == bitLen);
	seg->data = buf;
}

#if (QRCODE_USE_EXTRA)
// Public function - see documentation comment in header file.
void QRCode_MakeECI(long assignVal, u8 buf[], struct QRCode_Segment* seg)
{
	seg->mode = QRCODE_MODE_ECI;
	seg->numChars = 0;
	seg->bitLength = 0;
	if (assignVal < 0)
		assert(FALSE);
	else if (assignVal < (1 << 7)) 
	{
		memset(buf, 0, 1 * sizeof(buf[0]));
		appendBitsToBuffer((u16)assignVal, 8, buf, &seg->bitLength);
	}
	else if (assignVal < (1 << 14)) 
	{
		memset(buf, 0, 2 * sizeof(buf[0]));
		appendBitsToBuffer(2, 2, buf, &seg->bitLength);
		appendBitsToBuffer((u16)assignVal, 14, buf, &seg->bitLength);
	}
	else if (assignVal < 1000000L) 
	{
		memset(buf, 0, 3 * sizeof(buf[0]));
		appendBitsToBuffer(6, 3, buf, &seg->bitLength);
		appendBitsToBuffer((u16)(assignVal >> 10), 11, buf, &seg->bitLength);
		appendBitsToBuffer((u16)(assignVal & 0x3FF), 10, buf, &seg->bitLength);
	} else
		assert(FALSE);
	seg->data = buf;
}
#endif

// Calculates the number of bits needed to encode the given segments at the given version.
// Returns a non-negative number if successful. Otherwise returns LENGTH_OVERFLOW if a segment
// has too many characters to fit its length field, or the total bits exceeds INT16_MAX.
i16 getTotalBits(const struct QRCode_Segment segs[], u16 len, i16 version) 
{
	assert(segs != NULL || len == 0);
	long result = 0;
	for (u16 i = 0; i < len; i++) 
	{
		i16 numChars  = segs[i].numChars;
		i16 bitLength = segs[i].bitLength;
		assert(0 <= numChars  && numChars  <= INT16_MAX);
		assert(0 <= bitLength && bitLength <= INT16_MAX);
		i16 ccbits = numCharCountBits(segs[i].mode, version);
		assert(0 <= ccbits && ccbits <= 16);
		if (numChars >= (1L << ccbits))
			return LENGTH_OVERFLOW;  // The segment's length doesn't fit the field's bit width
		result += 4L + ccbits + bitLength;
		if (result > INT16_MAX)
			return LENGTH_OVERFLOW;  // The sum might overflow an i16 type
	}
	assert(0 <= result && result <= INT16_MAX);
	return (i16)result;
}


// Returns the bit width of the character count field for a segment in the given mode
// in a QR Code at the given version number. The result is in the range [0, 16].
static i16 numCharCountBits(enum QRCODE_MODE mode, i16 version)
{
	assert(QRCODE_VERSION_MIN <= version && version <= QRCODE_VERSION_MAX);
	i16 i = (version + 7) / 17;
	switch (mode)
	{
		case QRCODE_MODE_NUMERIC     : { static const i16 temp[] = {10, 12, 14}; return temp[i]; }
		case QRCODE_MODE_ALPHANUMERIC: { static const i16 temp[] = { 9, 11, 13}; return temp[i]; }
		case QRCODE_MODE_BYTE        : { static const i16 temp[] = { 8, 16, 16}; return temp[i]; }
		case QRCODE_MODE_KANJI       : { static const i16 temp[] = { 8, 10, 12}; return temp[i]; }
		case QRCODE_MODE_ECI         : return 0;
		default:  assert(FALSE);  return -1;  // Dummy value
	}
}


#undef LENGTH_OVERFLOW
