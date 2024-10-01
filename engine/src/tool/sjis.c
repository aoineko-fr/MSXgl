//_____________________________________________________________________________
//
//  SJIS converter module
//_____________________________________________________________________________
// http://www.rikai.com/library/kanjitables/kanji_codes.sjis.shtml

#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

#define SJIS_DAKU		0xDE00 // Daku ten
#define SJIS_HAN		0xDF00 // Handaku ten

// 
typedef struct
{
	u8 SJIS;
	u8 MSX;
} SJIS_MapToMSX;

// 
typedef struct
{
	u16 SJIS;
	u8 MSX;
} SJIS_MapToMSX16;


//=============================================================================
// READ-ONLY DATA
//=============================================================================

// 
const SJIS_MapToMSX g_SJIS_MarkToMSX[] =
{
	{ 0x41, 0xA4 }, // [A]
	{ 0x42, 0xA1 }, // [B]
	{ 0x45, 0xA5 }, // [E]
	{ 0x48, 0x3F }, // [H]
	{ 0x49, 0x21 }, // [I]
	{ 0x4A, 0xDE }, // [J]
	{ 0x4B, 0xDF }, // [K]
	{ 0x75, 0xA5 }, // [u]
	{ 0x76, 0xA5 }, // [v]
	{ 0x5B, 0xB0 }, // [[]
};

// 
const u16 g_SJIS_HiraToMSX[] =
{
	0x87,				// 82 9F - HIRAGANA SMALL A
	0x91,				// 82 A0 - HIRAGANA A
	0x88,				// 82 A1 - HIRAGANA SMALL I
	0x92,				// 82 A2 - HIRAGANA I
	0x89,				// 82 A3 - HIRAGANA SMALL U
	0x93,				// 82 A4 - HIRAGANA U
	0x8A,				// 82 A5 - HIRAGANA SMALL E
	0x94,				// 82 A6 - HIRAGANA E
	0x8B,				// 82 A7 - HIRAGANA SMALL O
	0x95,				// 82 A8 - HIRAGANA O
	0x96,				// 82 A9 - HIRAGANA KA
	0x96 + SJIS_DAKU,	// 82 AA - HIRAGANA GA
	0x97,				// 82 AB - HIRAGANA KI
	0x97 + SJIS_DAKU,	// 82 AC - HIRAGANA GI
	0x98,				// 82 AD - HIRAGANA KU
	0x98 + SJIS_DAKU,	// 82 AE - HIRAGANA GU
	0x99,				// 82 AF - HIRAGANA KE
	0x99 + SJIS_DAKU,	// 82 B0 - HIRAGANA GE
	0x9A,				// 82 B1 - HIRAGANA KO
	0x9A + SJIS_DAKU,	// 82 B2 - HIRAGANA GO
	0x9B,				// 82 B3 - HIRAGANA SA
	0x9B + SJIS_DAKU,	// 82 B4 - HIRAGANA ZA
	0x9C,				// 82 B5 - HIRAGANA SHI
	0x9C + SJIS_DAKU,	// 82 B6 - HIRAGANA JI
	0x9D,				// 82 B7 - HIRAGANA SU
	0x9D + SJIS_DAKU,	// 82 B8 - HIRAGANA ZU
	0x9E,				// 82 B9 - HIRAGANA SE
	0x9E + SJIS_DAKU,	// 82 BA - HIRAGANA ZE
	0x9F,				// 82 BB - HIRAGANA SO
	0x9F + SJIS_DAKU,	// 82 BC - HIRAGANA ZO
	0xE0,				// 82 BD - HIRAGANA TA
	0xE0 + SJIS_DAKU,	// 82 BE - HIRAGANA DA
	0xE1,				// 82 BF - HIRAGANA CHI
	0xE1 + SJIS_DAKU,	// 82 C0 - HIRAGANA ZI
	0x8F,				// 82 C1 - HIRAGANA SMALL TSU
	0xE2,				// 82 C2 - HIRAGANA TSU
	0xE2 + SJIS_DAKU,	// 82 C3 - HIRAGANA DZU
	0xE3,				// 82 C4 - HIRAGANA TE
	0xE3 + SJIS_DAKU,	// 82 C5 - HIRAGANA DE
	0xE4,				// 82 C6 - HIRAGANA TO
	0xE4 + SJIS_DAKU,	// 82 C7 - HIRAGANA DO
	0xE5,				// 82 C8 - HIRAGANA NA
	0xE6,				// 82 C9 - HIRAGANA NI
	0xE7,				// 82 CA - HIRAGANA NU
	0xE8,				// 82 CB - HIRAGANA NE
	0xE9,				// 82 CC - HIRAGANA NO
	0xEA,				// 82 CD - HIRAGANA HA
	0xEA + SJIS_DAKU,	// 82 CE - HIRAGANA BA
	0xEA + SJIS_HAN,	// 82 CF - HIRAGANA PA
	0xEB,				// 82 D0 - HIRAGANA HI
	0xEB + SJIS_DAKU,	// 82 D1 - HIRAGANA BI
	0xEB + SJIS_HAN,	// 82 D2 - HIRAGANA PI
	0xEC,				// 82 D3 - HIRAGANA FU
	0xEC + SJIS_DAKU,	// 82 D4 - HIRAGANA BU
	0xEC + SJIS_HAN,	// 82 D5 - HIRAGANA PU
	0xED,				// 82 D6 - HIRAGANA HE
	0xED + SJIS_DAKU,	// 82 D7 - HIRAGANA BE
	0xED + SJIS_HAN,	// 82 D8 - HIRAGANA PE
	0xEE,				// 82 D9 - HIRAGANA HO
	0xEE + SJIS_DAKU,	// 82 DA - HIRAGANA BO
	0xEE + SJIS_HAN,	// 82 DB - HIRAGANA PO
	0xEF,				// 82 DC - HIRAGANA MA
	0xF0,				// 82 DD - HIRAGANA MI
	0xF1,				// 82 DE - HIRAGANA MU
	0xF2,				// 82 DF - HIRAGANA ME
	0xF3,				// 82 E0 - HIRAGANA MO
	0x8C,				// 82 E1 - HIRAGANA SMALL YA
	0xF4,				// 82 E2 - HIRAGANA YA
	0x8D,				// 82 E3 - HIRAGANA SMALL YU
	0xF5,				// 82 E4 - HIRAGANA YU
	0x8E,				// 82 E5 - HIRAGANA SMALL YO
	0xF6,				// 82 E6 - HIRAGANA YO
	0xF7,				// 82 E7 - HIRAGANA RA
	0xF8,				// 82 E8 - HIRAGANA RI
	0xF9,				// 82 E9 - HIRAGANA RU
	0xFA,				// 82 EA - HIRAGANA RE
	0xFB,				// 82 EB - HIRAGANA RO
	'?',				// 82 EC - HIRAGANA SMALL WA
	0xFC,				// 82 ED - HIRAGANA WA
	'?',				// 82 EE - HIRAGANA WI
	'?',				// 82 EF - HIRAGANA WE
	0x86,				// 82 F0 - HIRAGANA WO
	0xFD,				// 82 F1 - HIRAGANA N
};

// 
const u16 g_SJIS_KataToMSX[] =
{
	0xA7,				// 83 40 - KATAKAGA SMALL A
	0xB1,				// 83 41 - KATAKAGA A
	0xA8,				// 83 42 - KATAKAGA SMALL I
	0xB2,				// 83 43 - KATAKAGA I
	0xA9,				// 83 44 - KATAKAGA SMALL U
	0xB3,				// 83 45 - KATAKAGA U
	0xAA,				// 83 46 - KATAKAGA SMALL E
	0xB4,				// 83 47 - KATAKAGA E
	0xAB,				// 83 48 - KATAKAGA SMALL O
	0xB5,				// 83 49 - KATAKAGA O
	0xB6,				// 83 4A - KATAKAGA KA
	0xB6 + SJIS_DAKU,	// 83 4B - KATAKAGA GA
	0xB7,				// 83 4C - KATAKAGA KI
	0xB7 + SJIS_DAKU,	// 83 4D - KATAKAGA GI
	0xB8,				// 83 4E - KATAKAGA KU
	0xB8 + SJIS_DAKU,	// 83 4F - KATAKAGA GU
	0xB9,				// 83 50 - KATAKAGA KE
	0xB9 + SJIS_DAKU,	// 83 51 - KATAKAGA GE
	0xBA,				// 83 52 - KATAKAGA KO
	0xBA + SJIS_DAKU,	// 83 53 - KATAKAGA GO
	0xBB,				// 83 54 - KATAKAGA SA
	0xBB + SJIS_DAKU,	// 83 55 - KATAKAGA ZA
	0xBC,				// 83 56 - KATAKAGA SHI
	0xBC + SJIS_DAKU,	// 83 57 - KATAKAGA JI
	0xBD,				// 83 58 - KATAKAGA SU
	0xBD + SJIS_DAKU,	// 83 59 - KATAKAGA ZU
	0xBE,				// 83 5A - KATAKAGA SE
	0xBE + SJIS_DAKU,	// 83 5B - KATAKAGA ZE
	0xBF,				// 83 5C - KATAKAGA SO ---------- Must be: 'ƒ\\'
	0xBF + SJIS_DAKU,	// 83 5D - KATAKAGA ZO
	0xC0,				// 83 5E - KATAKAGA TA
	0xC0 + SJIS_DAKU,	// 83 5F - KATAKAGA DA
	0xC1,				// 83 60 - KATAKAGA CHI
	0xC1 + SJIS_DAKU,	// 83 61 - KATAKAGA ZI
	0xAF,				// 83 62 - KATAKAGA SMALL TSU
	0xC2,				// 83 63 - KATAKAGA TSU
	0xC2 + SJIS_DAKU,	// 83 64 - KATAKAGA DZU
	0xC3,				// 83 65 - KATAKAGA TE
	0xC3 + SJIS_DAKU,	// 83 66 - KATAKAGA DE
	0xC4,				// 83 67 - KATAKAGA TO
	0xC4 + SJIS_DAKU,	// 83 68 - KATAKAGA DO
	0xC5,				// 83 69 - KATAKAGA NA
	0xC6,				// 83 6A - KATAKAGA NI
	0xC7,				// 83 6B - KATAKAGA NU
	0xC8,				// 83 6C - KATAKAGA NE
	0xC9,				// 83 6D - KATAKAGA NO
	0xCA,				// 83 6E - KATAKAGA HA
	0xCA + SJIS_DAKU,	// 83 6F - KATAKAGA BA
	0xCA + SJIS_HAN,	// 83 70 - KATAKAGA PA
	0xCB,				// 83 71 - KATAKAGA HI
	0xCB + SJIS_DAKU,	// 83 72 - KATAKAGA BI
	0xCB + SJIS_HAN,	// 83 73 - KATAKAGA PI
	0xCC,				// 83 74 - KATAKAGA FU
	0xCC + SJIS_DAKU,	// 83 75 - KATAKAGA BU
	0xCC + SJIS_HAN,	// 83 76 - KATAKAGA PU
	0xCD,				// 83 77 - KATAKAGA HE
	0xCD + SJIS_DAKU,	// 83 78 - KATAKAGA BE
	0xCD + SJIS_HAN,	// 83 79 - KATAKAGA PE
	0xCE,				// 83 7A - KATAKAGA HO
	0xCE + SJIS_DAKU,	// 83 7B - KATAKAGA BO
	0xCE + SJIS_HAN,	// 83 7C - KATAKAGA PO
	0xCF,				// 83 7D - KATAKAGA MA
	0xD0,				// 83 7E - KATAKAGA MI
	'?',				// 83 7F
	0xD1,				// 83 80 - KATAKAGA MU
	0xD2,				// 83 81 - KATAKAGA ME
	0xD3,				// 83 82 - KATAKAGA MO
	0xAC,				// 83 83 - KATAKAGA SMALL YA
	0xD4,				// 83 84 - KATAKAGA YA
	0xAD,				// 83 85 - KATAKAGA SMALL YU
	0xD5,				// 83 86 - KATAKAGA YU
	0xAE,				// 83 87 - KATAKAGA SMALL YO
	0xD6,				// 83 88 - KATAKAGA YO
	0xD7,				// 83 89 - KATAKAGA RA
	0xD8,				// 83 8A - KATAKAGA RI
	0xD9,				// 83 8B - KATAKAGA RU
	0xDA,				// 83 8C - KATAKAGA RE
	0xDB,				// 83 8D - KATAKAGA RO
	'?',				// 83 8E - KATAKAGA SMALL WA
	0xDC,				// 83 8F - KATAKAGA WA
	'?',				// 83 90 - KATAKAGA WI
	'?',				// 83 91 - KATAKAGA WE
	0xA6,				// 83 92 - KATAKAGA WO
	0xDD,				// 83 93 - KATAKAGA N
	'?',				// 83 94 - KATAKAGA VI
	'?',				// 83 95 - KATAKAGA SMALL KA
	'?',				// 83 96 - KATAKAGA SMALL KE
};

// 
const SJIS_MapToMSX16 g_SJIS_KanjiToMSX[] =
{
	{ 0x8C8E,	0x01 },	// ŒŽ
	{ 0x89CE,	0x02 },	// ‰Î
	{ 0x9085,	0x03 },	// …
	{ 0x96D8,	0x04 },	// –Ø
	{ 0x8BE0,	0x05 },	// ‹à
	{ 0x9379,	0x06 },	// “y
	{ 0x93FA,	0x07 },	// “ú
	{ 0x944E,	0x08 },	// ”N
	{ 0x897E,	0x09 },	// ‰~
	{ 0x8E9E,	0x0A },	// Žž
	{ 0x95AA,	0x0B },	// •ª
	{ 0x9562,	0x0C },	// •b
	{ 0x9553,	0x0D },	// •S
	{ 0x90E7,	0x0E },	// ç
	{ 0x969C,	0x0F },	// –œ
	{ 0x91E5,	0x1D },	// ‘å
	{ 0x9286,	0x1E },	// ’†
	{ 0x8FAC,	0x1F },	// ¬
};

// Shift-JIS to MSX character mapping
/*const u16* g_MapShiftJIS =
	L"@ŒŽ‰Î…–Ø‹à“y“ú”N‰~Žž•ª•b•Sç–œ"
	L"@@@@@@@@@@@@@‘å’†¬"
	L"@I@@@@@@@@@@@@@^"
	L"‚O‚P‚Q‚R‚S‚T‚U‚V‚W‚X@@@@@H"
	L"—‚`‚a‚b‚c‚d‚e‚f‚g‚h‚i‚j‚k‚l‚m‚n"
	L"‚o‚p‚q‚r‚s‚t‚u‚v‚w‚x‚ym@nOQ"
	L"M‚‚‚‚ƒ‚„‚…‚†‚‡‚ˆ‚‰‚Š‚‹‚Œ‚‚Ž‚"
	L"‚‚‘‚’‚“‚”‚•‚–‚—‚˜‚™‚šobp`@"
	L"@@@@@@‚ð‚Ÿ‚¡‚£‚¥‚§‚á‚ã‚å‚Á"
	L"@‚ ‚¢‚¤‚¦‚¨‚©‚«‚­‚¯‚±‚³‚µ‚·‚¹‚»"
	L"@BuvAEƒ’ƒ@ƒBƒDƒFƒHƒƒƒ…ƒ‡ƒb"
	L"[ƒAƒCƒEƒGƒIƒJƒLƒNƒPƒRƒTƒVƒXƒZƒ\"
	L"ƒ^ƒ`ƒcƒeƒgƒiƒjƒkƒlƒmƒnƒqƒtƒwƒzƒ}"
	L"ƒ~ƒ€ƒƒ‚ƒ„ƒ†ƒˆƒ‰ƒŠƒ‹ƒŒƒƒƒ“JK"
	L"‚½‚¿‚Â‚Ä‚Æ‚È‚É‚Ê‚Ë‚Ì‚Í‚Ð‚Ó‚Ö‚Ù‚Ü"
	L"‚Ý‚Þ‚ß‚à‚â‚ä‚æ‚ç‚è‚é‚ê‚ë‚í‚ñ";*/

//=============================================================================
// MEMORY DATA
//=============================================================================

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
c8* SJIS_Convert(const c8* src, c8* dest)
{
	c8* ptr = dest;
	while(*src)
	{
		if (*src == 0x81)
		{
			src++;
			u8 chr = '?';
			loop(i, numberof(g_SJIS_MarkToMSX))
			{
				const SJIS_MapToMSX* map = &g_SJIS_MarkToMSX[i];
				if(*src == map->SJIS)
				{
					chr = map->MSX;
					break;
				}
			}
			*ptr++ = chr;
			src++;
		}
		else if (*src == 0x82)
		{
			u16 code = (src[0] << 8) + src[1];
			src += 2;
			code -= 0x829F;
			if(code < numberof(g_SJIS_HiraToMSX))
			{
				u16 val = g_SJIS_HiraToMSX[code];
				*ptr++ = val & 0x00FF;
				if(val & 0xFF00)
					*ptr++ = val >> 8;
			}
			else
				*ptr++ = '?';
		}
		else if(*src == 0x83)
		{
			u16 code = (src[0] << 8) + src[1];
			src += 2;
			code -= 0x8340;
			if(code < numberof(g_SJIS_KataToMSX))
			{
				u16 val = g_SJIS_KataToMSX[code];
				*ptr++ = val & 0x00FF;
				if(val & 0xFF00)
					*ptr++ = val >> 8;
			}
			else
				*ptr++ = '?';
		}
		else if(*src > 0x88)
		{
			u16 code = (src[0] << 8) + src[1];
			u8 chr = '?';
			loop(i, numberof(g_SJIS_KanjiToMSX))
			{
				const SJIS_MapToMSX16* map = &g_SJIS_KanjiToMSX[i];
				if(code == map->SJIS)
				{
					chr = map->MSX;
					break;
				}
			}
			*ptr++ = chr;
			src += 2;
		}
		else if(*src & 0x80)
		{
			*ptr++ = '?';
			src += 2;
		}
		else
			*ptr++ = *src++;
	}
	*ptr = 0;

	return dest;
}