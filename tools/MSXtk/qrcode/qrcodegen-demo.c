/* 
 * QR Code generator demo (C)
 * 
 * Run this command-line program with no arguments. The program
 * computes a demonstration QR Codes and print it to the console.
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

//#include <stdbool.h>
//#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qrcode.h"


// Function prototypes
static void doBasicDemo(void);
static void doVarietyDemo(void);
static void doSegmentDemo(void);
static void doMaskDemo(void);
static void printQr(const u8 qrcode[]);


// The main application program.
i16 main(void)
{
	doBasicDemo();
	doVarietyDemo();
	//doSegmentDemo();
	//doMaskDemo();
	return EXIT_SUCCESS;
}



/*---- Demo suite ----*/

// Creates a single QR Code, then prints it to the console.
static void doBasicDemo(void)
{
	// Make and print the QR Code symbol
	u8 qrcode[QRCODE_BUFFER_LEN_MAX];
	u8 tempBuffer[QRCODE_BUFFER_LEN_MAX];

#if (QRCODE_VERSION_CUSTOM)
	QRCode_SetVersion(5, 5);
#endif
	{
		bool ok = QRCode_EncodeText("MSXgl", tempBuffer, qrcode, QRCODE_ECC_LOW, QRCODE_MASK_AUTO, TRUE);
		if (ok)
			printQr(qrcode);
	}

	{
		bool ok = QRCode_EncodeText("https://aoineko.org/msxgl", tempBuffer, qrcode, QRCODE_ECC_LOW, QRCODE_MASK_AUTO, TRUE);
		if (ok)
			printQr(qrcode);
	}

	{
		bool ok = QRCode_EncodeText("https://aoineko.org/msxgl?g=012&c=0123456789ABCDEF", tempBuffer, qrcode, QRCODE_ECC_LOW, QRCODE_MASK_AUTO, TRUE);
		if (ok)
			printQr(qrcode);
	}
}


// Creates a variety of QR Codes that exercise different features of the library, and prints each one to the console.
static void doVarietyDemo(void)
{
	{  // Numeric mode encoding (3.33 bits per digit)
		u8 qrcode[QRCODE_BUFFER_LEN_MAX];
		u8 tempBuffer[QRCODE_BUFFER_LEN_MAX];
		bool ok = QRCode_EncodeText("314159265358979323846264338327950288419716939937510", tempBuffer, qrcode,
			QRCODE_ECC_MEDIUM, QRCODE_MASK_AUTO, TRUE);
		if (ok)
			printQr(qrcode);
	}
	
	{  // Alphanumeric mode encoding (5.5 bits per character)
		u8 qrcode[QRCODE_BUFFER_LEN_MAX];
		u8 tempBuffer[QRCODE_BUFFER_LEN_MAX];
		bool ok = QRCode_EncodeText("DOLLAR-AMOUNT:$39.87 PERCENTAGE:100.00% OPERATIONS:+-*/", tempBuffer, qrcode,
			QRCODE_ECC_HIGH, QRCODE_MASK_AUTO, TRUE);
		if (ok)
			printQr(qrcode);
	}
	
	{  // Unicode text as UTF-8
		const char *text = "\xE3\x81\x93\xE3\x82\x93\xE3\x81\xAB\xE3\x81\xA1wa\xE3\x80\x81"
			"\xE4\xB8\x96\xE7\x95\x8C\xEF\xBC\x81\x20\xCE\xB1\xCE\xB2\xCE\xB3\xCE\xB4";
		u8 qrcode[QRCODE_BUFFER_LEN_MAX];
		u8 tempBuffer[QRCODE_BUFFER_LEN_MAX];
		bool ok = QRCode_EncodeText(text, tempBuffer, qrcode,
			QRCODE_ECC_QUARTILE, QRCODE_MASK_AUTO, TRUE);
		if (ok)
			printQr(qrcode);
	}
	
	{  // Moderately large QR Code using longer text (from Lewis Carroll's Alice in Wonderland)
		const char *text =
			"Alice was beginning to get very tired of sitting by her sister on the bank, "
			"and of having nothing to do: once or twice she had peeped into the book her sister was reading, "
			"but it had no pictures or conversations in it, 'and what is the use of a book,' thought Alice "
			"'without pictures or conversations?' So she was considering in her own mind (as well as she could, "
			"for the hot day made her feel very sleepy and stupid), whether the pleasure of making a "
			"daisy-chain would be worth the trouble of getting up and picking the daisies, when suddenly "
			"a White Rabbit with pink eyes ran close by her.";
		u8 qrcode[QRCODE_BUFFER_LEN_MAX];
		u8 tempBuffer[QRCODE_BUFFER_LEN_MAX];
		bool ok = QRCode_EncodeText(text, tempBuffer, qrcode,
			QRCODE_ECC_HIGH, QRCODE_MASK_AUTO, TRUE);
		if (ok)
			printQr(qrcode);
	}
}

//
//// Creates QR Codes with manually specified segments for better compactness.
//static void doSegmentDemo(void) {
//	{  // Illustration "silver"
//		const char *silver0 = "THE SQUARE ROOT OF 2 IS 1.";
//		const char *silver1 = "41421356237309504880168872420969807856967187537694807317667973799";
//		u8 qrcode[QRCODE_BUFFER_LEN_MAX];
//		u8 tempBuffer[QRCODE_BUFFER_LEN_MAX];
//		bool ok;
//		{
//			char *concat = calloc(strlen(silver0) + strlen(silver1) + 1, sizeof(char));
//			if (concat == NULL) {
//				perror("calloc");
//				exit(EXIT_FAILURE);
//			}
//			strcat(concat, silver0);
//			strcat(concat, silver1);
//			ok = QRCode_EncodeText(concat, tempBuffer, qrcode, QRCODE_ECC_LOW,
//				QRCODE_MASK_AUTO, TRUE);
//			if (ok)
//				printQr(qrcode);
//			free(concat);
//		}
//		{
//			u8 *segBuf0 = malloc(QRCode_CalcSegmentBufferSize(QRCODE_MODE_ALPHANUMERIC, (u16)strlen(silver0)) * sizeof(u8));
//			u8 *segBuf1 = malloc(QRCode_CalcSegmentBufferSize(QRCODE_MODE_NUMERIC, (u16)strlen(silver1)) * sizeof(u8));
//			if (segBuf0 == NULL || segBuf1 == NULL) {
//				perror("malloc");
//				exit(EXIT_FAILURE);
//			}
//			struct QRCode_Segment segs[] = {
//				QRCode_MakeAlphanumeric(silver0, segBuf0),
//				QRCode_MakeNumeric(silver1, segBuf1),
//			};
//			ok = QRCode_encodeSegments(segs, sizeof(segs) / sizeof(segs[0]), QRCODE_ECC_LOW, tempBuffer, qrcode);
//			free(segBuf0);
//			free(segBuf1);
//			if (ok)
//				printQr(qrcode);
//		}
//	}
//	
//	{  // Illustration "golden"
//		const char *golden0 = "Golden ratio \xCF\x86 = 1.";
//		const char *golden1 = "6180339887498948482045868343656381177203091798057628621354486227052604628189024497072072041893911374";
//		const char *golden2 = "......";
//		u8 qrcode[QRCODE_BUFFER_LEN_MAX];
//		u8 tempBuffer[QRCODE_BUFFER_LEN_MAX];
//		bool ok;
//		{
//			char *concat = calloc(strlen(golden0) + strlen(golden1) + strlen(golden2) + 1, sizeof(char));
//			if (concat == NULL) {
//				perror("calloc");
//				exit(EXIT_FAILURE);
//			}
//			strcat(concat, golden0);
//			strcat(concat, golden1);
//			strcat(concat, golden2);
//			ok = QRCode_EncodeText(concat, tempBuffer, qrcode, QRCODE_ECC_LOW,
//				QRCODE_MASK_AUTO, TRUE);
//			if (ok)
//				printQr(qrcode);
//			free(concat);
//		}
//		{
//			u8 *bytes = malloc(strlen(golden0) * sizeof(u8));
//			if (bytes == NULL) {
//				perror("malloc");
//				exit(EXIT_FAILURE);
//			}
//			for (u16 i = 0, len = (u16)strlen(golden0); i < len; i++)
//				bytes[i] = (u8)golden0[i];
//			u8 *segBuf0 = malloc(QRCode_CalcSegmentBufferSize(QRCODE_MODE_BYTE, (u16)strlen(golden0)) * sizeof(u8));
//			u8 *segBuf1 = malloc(QRCode_CalcSegmentBufferSize(QRCODE_MODE_NUMERIC, (u16)strlen(golden1)) * sizeof(u8));
//			u8 *segBuf2 = malloc(QRCode_CalcSegmentBufferSize(QRCODE_MODE_ALPHANUMERIC, (u16)strlen(golden2)) * sizeof(u8));
//			if (segBuf0 == NULL || segBuf1 == NULL || segBuf2 == NULL) {
//				perror("malloc");
//				exit(EXIT_FAILURE);
//			}
//			struct QRCode_Segment segs[] = {
//				QRCode_MakeBytes(bytes, (u16)strlen(golden0), segBuf0),
//				QRCode_MakeNumeric(golden1, segBuf1),
//				QRCode_MakeAlphanumeric(golden2, segBuf2),
//			};
//			free(bytes);
//			ok = QRCode_encodeSegments(segs, sizeof(segs) / sizeof(segs[0]), QRCODE_ECC_LOW, tempBuffer, qrcode);
//			free(segBuf0);
//			free(segBuf1);
//			free(segBuf2);
//			if (ok)
//				printQr(qrcode);
//		}
//	}
//	
//	{  // Illustration "Madoka": kanji, kana, Cyrillic, full-width Latin, Greek characters
//		u8 qrcode[QRCODE_BUFFER_LEN_MAX];
//		u8 tempBuffer[QRCODE_BUFFER_LEN_MAX];
//		bool ok;
//		{
//			const char *madoka =  // Encoded in UTF-8
//				"\xE3\x80\x8C\xE9\xAD\x94\xE6\xB3\x95\xE5"
//				"\xB0\x91\xE5\xA5\xB3\xE3\x81\xBE\xE3\x81"
//				"\xA9\xE3\x81\x8B\xE2\x98\x86\xE3\x83\x9E"
//				"\xE3\x82\xAE\xE3\x82\xAB\xE3\x80\x8D\xE3"
//				"\x81\xA3\xE3\x81\xA6\xE3\x80\x81\xE3\x80"
//				"\x80\xD0\x98\xD0\x90\xD0\x98\xE3\x80\x80"
//				"\xEF\xBD\x84\xEF\xBD\x85\xEF\xBD\x93\xEF"
//				"\xBD\x95\xE3\x80\x80\xCE\xBA\xCE\xB1\xEF"
//				"\xBC\x9F";
//			ok = QRCode_EncodeText(madoka, tempBuffer, qrcode, QRCODE_ECC_LOW,
//				QRCODE_MASK_AUTO, TRUE);
//			if (ok)
//				printQr(qrcode);
//		}
//		{
//			const i16 kanjiChars[] = {  // Kanji mode encoding (13 bits per character)
//				0x0035, 0x1002, 0x0FC0, 0x0AED, 0x0AD7,
//				0x015C, 0x0147, 0x0129, 0x0059, 0x01BD,
//				0x018D, 0x018A, 0x0036, 0x0141, 0x0144,
//				0x0001, 0x0000, 0x0249, 0x0240, 0x0249,
//				0x0000, 0x0104, 0x0105, 0x0113, 0x0115,
//				0x0000, 0x0208, 0x01FF, 0x0008,
//			};
//			u16 len = sizeof(kanjiChars) / sizeof(kanjiChars[0]);
//			u8 *segBuf = calloc(QRCode_CalcSegmentBufferSize(QRCODE_MODE_KANJI, len), sizeof(u8));
//			if (segBuf == NULL) {
//				perror("calloc");
//				exit(EXIT_FAILURE);
//			}
//			struct QRCode_Segment seg;
//			seg.mode = QRCODE_MODE_KANJI;
//			seg.numChars = (i16)len;
//			seg.bitLength = 0;
//			for (u16 i = 0; i < len; i++) {
//				for (i16 j = 12; j >= 0; j--, seg.bitLength++)
//					segBuf[seg.bitLength >> 3] |= ((kanjiChars[i] >> j) & 1) << (7 - (seg.bitLength & 7));
//			}
//			seg.data = segBuf;
//			ok = QRCode_encodeSegments(&seg, 1, QRCODE_ECC_LOW, tempBuffer, qrcode);
//			free(segBuf);
//			if (ok)
//				printQr(qrcode);
//		}
//	}
//}
//
//
//// Creates QR Codes with the same size and contents but different mask patterns.
//static void doMaskDemo(void) {
//	{  // Project Nayuki URL
//		u8 qrcode[QRCODE_BUFFER_LEN_MAX];
//		u8 tempBuffer[QRCODE_BUFFER_LEN_MAX];
//		bool ok;
//		
//		ok = QRCode_EncodeText("https://www.nayuki.io/", tempBuffer, qrcode,
//			QRCODE_ECC_HIGH, QRCODE_MASK_AUTO, TRUE);
//		if (ok)
//			printQr(qrcode);
//		
//		ok = QRCode_EncodeText("https://www.nayuki.io/", tempBuffer, qrcode,
//			QRCODE_ECC_HIGH, QRCODE_MASK_3, TRUE);
//		if (ok)
//			printQr(qrcode);
//	}
//	
//	{  // Chinese text as UTF-8
//		const char *text =
//			"\xE7\xB6\xAD\xE5\x9F\xBA\xE7\x99\xBE\xE7\xA7\x91\xEF\xBC\x88\x57\x69\x6B\x69\x70"
//			"\x65\x64\x69\x61\xEF\xBC\x8C\xE8\x81\x86\xE8\x81\xBD\x69\x2F\xCB\x8C\x77\xC9\xAA"
//			"\x6B\xE1\xB5\xBB\xCB\x88\x70\x69\xCB\x90\x64\x69\x2E\xC9\x99\x2F\xEF\xBC\x89\xE6"
//			"\x98\xAF\xE4\xB8\x80\xE5\x80\x8B\xE8\x87\xAA\xE7\x94\xB1\xE5\x85\xA7\xE5\xAE\xB9"
//			"\xE3\x80\x81\xE5\x85\xAC\xE9\x96\x8B\xE7\xB7\xA8\xE8\xBC\xAF\xE4\xB8\x94\xE5\xA4"
//			"\x9A\xE8\xAA\x9E\xE8\xA8\x80\xE7\x9A\x84\xE7\xB6\xB2\xE8\xB7\xAF\xE7\x99\xBE\xE7"
//			"\xA7\x91\xE5\x85\xA8\xE6\x9B\xB8\xE5\x8D\x94\xE4\xBD\x9C\xE8\xA8\x88\xE7\x95\xAB";
//		u8 qrcode[QRCODE_BUFFER_LEN_MAX];
//		u8 tempBuffer[QRCODE_BUFFER_LEN_MAX];
//		bool ok;
//		
//		ok = QRCode_EncodeText(text, tempBuffer, qrcode,
//			QRCODE_ECC_MEDIUM, QRCODE_MASK_0, TRUE);
//		if (ok)
//			printQr(qrcode);
//		
//		ok = QRCode_EncodeText(text, tempBuffer, qrcode,
//			QRCODE_ECC_MEDIUM, QRCODE_MASK_1, TRUE);
//		if (ok)
//			printQr(qrcode);
//		
//		ok = QRCode_EncodeText(text, tempBuffer, qrcode,
//			QRCODE_ECC_MEDIUM, QRCODE_MASK_5, TRUE);
//		if (ok)
//			printQr(qrcode);
//		
//		ok = QRCode_EncodeText(text, tempBuffer, qrcode,
//			QRCODE_ECC_MEDIUM, QRCODE_MASK_7, TRUE);
//		if (ok)
//			printQr(qrcode);
//	}
//}



/*---- Utilities ----*/

// Prints the given QR Code to the console.
static void printQr(const u8 qrcode[])
{
	u8 size = QRCode_GetSize(qrcode);
	u8 ver = QRCode_GetVersion(qrcode);
	printf("Size: %i; Version:%i\n", size, ver);
	i16 border = 4;
	for (i16 y = -border; y < size + border; y++)
	{
		for (i16 x = -border; x < size + border; x++) 
		{
			char chr = QRCode_GetModule(qrcode, x, y) ? 219 : ' ';
			printf("%c%c", chr, chr);
		}
		fputs("\n", stdout);
	}
	fputs("\n", stdout);
}
