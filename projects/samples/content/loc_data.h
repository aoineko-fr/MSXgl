// Generated by MSXgl on 2023-11-30 23:46:22
#pragma once
#include "core.h"

// Language definition
#define LANG_EN 0
#define LANG_FR 1
#define LANG_JA 2
#define LANG_MAX 3

// Text ID definition
#define TEXT_CURRENTLANG 0
#define TEXT_MONDAY 1
#define TEXT_TUESDAY 2
#define TEXT_WEDNESDAY 3
#define TEXT_THURSDAY 4
#define TEXT_FRIDAY 5
#define TEXT_SATURDAY 6
#define TEXT_SUNDAY 7
#define TEXT_DATE 8
#define TEXT_TITLE 9
#define TEXT_LANGUAGE 10
#define TEXT_FONT 11
#define TEXT_SAMPLE 12
#define TEXT_EXAMPLE 13
#define TEXT_MAX 14

// Localization data table
const c8* const g_LocData[LANG_MAX][TEXT_MAX] =
{
	// EN
	{
		"English", // TEXT_CURRENTLANG
		"Monday", // TEXT_MONDAY
		"Tuesday", // TEXT_TUESDAY
		"Wednesday", // TEXT_WEDNESDAY
		"Thursday", // TEXT_THURSDAY
		"Friday", // TEXT_FRIDAY
		"Saturday", // TEXT_SATURDAY
		"Sunday", // TEXT_SUNDAY
		"Date", // TEXT_DATE
		"MSXgl - Localization sample", // TEXT_TITLE
		"Language", // TEXT_LANGUAGE
		"Font", // TEXT_FONT
		"Sample", // TEXT_SAMPLE
		"The quick brown fox jumps over the lazy dog.", // TEXT_EXAMPLE
	},
	// FR
	{
		"Fran\x87\x61is", // TEXT_CURRENTLANG
		"Lundi", // TEXT_MONDAY
		"Mardi", // TEXT_TUESDAY
		"Mercredi", // TEXT_WEDNESDAY
		"Jeudi", // TEXT_THURSDAY
		"Vendredi", // TEXT_FRIDAY
		"Samedi", // TEXT_SATURDAY
		"Dimanche", // TEXT_SUNDAY
		"Date", // TEXT_DATE
		"MSXgl - Traduction", // TEXT_TITLE
		"Langue", // TEXT_LANGUAGE
		"Police", // TEXT_FONT
		"Exemple", // TEXT_SAMPLE
		"Voix ambigu\x89 d'un c\x6F\x65ur qui, au z\x82phyr, pr\x82\x66\x8Are les jattes de kiwis.", // TEXT_EXAMPLE
	},
	// JA
	{
		"\xE6\xEB\xFD\x9A\xDE", // TEXT_CURRENTLANG
		"\x99\xDE\xE2\xF6\x93\xEB\xDE", // TEXT_MONDAY
		"\x96\xF6\x93\xEB\xDE", // TEXT_TUESDAY
		"\x9D\x92\xF6\x93\xEB\xDE", // TEXT_WEDNESDAY
		"\xF3\x98\xF6\x93\xEB\xDE", // TEXT_THURSDAY
		"\x97\xFD\xF6\x93\xEB\xDE", // TEXT_FRIDAY
		"\xE4\xDE\xF6\x93\xEB\xDE", // TEXT_SATURDAY
		"\xE6\xE1\xF6\x93\xEB\xDE", // TEXT_SUNDAY
		NULL, // TEXT_DATE
		"MSXgl - \xDB\xB0\xB6\xD8\xBE\xDE\xB0\xBC\xAE\xDD\xA5\xBB\xDD\xCC\xDF\xD9", // TEXT_TITLE
		"\x99\xDE\xFD\x9A\xDE", // TEXT_LANGUAGE
		"\xCC\xAB\xDD\xC4", // TEXT_FONT
		"\xBB\xDD\xCC\xDF\xD9", // TEXT_SAMPLE
		"\x92\xFB\xEA\xE6\xEE\xED\xE4\xE1\xF8\xE7\xF9\x86\xFC\x96\xF6\xE0\xFA\x9F\xE2\xE8\xE5\xF7\xF1\x93ゐ\xE9\x95\x98\xF4\xEF\x99\xEC\x9A\x94\xE3\x91\x9B\x97\xF5\xF2\xF0\x9Cゑ\xEB\xF3\x9E\x9D\xA1", // TEXT_EXAMPLE
	},
};