const locUtil = require("./localize_util.js"); 

// Character class
const CharClass = {
	RightToLeft: 0,
	LeftToRight: 1,
	Neutral:     2,
	Weak:        3,
};

//-----------------------------------------------------------------------------
// Name:    Map from MSX Arabic (Bawareth Perfect MSX1, Yamaha AX500) (video) character set to Unicode
// Date:    2019 October 4
// Authors: Rebecca Bettencourt <support@kreativekorp.com>
//          Wessam Bahnassi <https://twitter.com/wbahnassi>
// Note:    Depending on the use case, this mapping alone is not sufficient for correct
//          conversion of Arabic text. Use of the Arabic shaping algorithm is required.
const MapARBtoMSX = {
	0x03C0: [ 0x10 ], // GREEK SMALL LETTER PI
	0x2534: [ 0x11 ], // BOX DRAWINGS LIGHT UP AND HORIZONTAL
	0x252C: [ 0x12 ], // BOX DRAWINGS LIGHT DOWN AND HORIZONTAL
	0x2524: [ 0x13 ], // BOX DRAWINGS LIGHT VERTICAL AND LEFT
	0x251C: [ 0x14 ], // BOX DRAWINGS LIGHT VERTICAL AND RIGHT
	0x253C: [ 0x15 ], // BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL
	0x2502: [ 0x16 ], // BOX DRAWINGS LIGHT VERTICAL
	0x2500: [ 0x17 ], // BOX DRAWINGS LIGHT HORIZONTAL
	0x250C: [ 0x18 ], // BOX DRAWINGS LIGHT DOWN AND RIGHT
	0x2510: [ 0x19 ], // BOX DRAWINGS LIGHT DOWN AND LEFT
	0x2514: [ 0x1A ], // BOX DRAWINGS LIGHT UP AND RIGHT
	0x2518: [ 0x1B ], // BOX DRAWINGS LIGHT UP AND LEFT
	0x2573: [ 0x1C ], // BOX DRAWINGS LIGHT DIAGONAL CROSS
	0x0020: [ 0x20 ], // SPACE, left to right
	0x0021: [ 0x21 ], // EXCLAMATION MARK, left to right
	0x0022: [ 0x22 ], // QUOTATION MARK, left to right
	0x0023: [ 0x23 ], // NUMBER SIGN, left to right
	0x0024: [ 0x24 ], // DOLLAR SIGN, left to right
	0x0025: [ 0x25 ], // PERCENT SIGN
	0x0026: [ 0x26 ], // AMPERSAND, left to right
	0x0027: [ 0x27 ], // APOSTROPHE, left to right
	0x0028: [ 0x28 ], // LEFT PARENTHESIS, left to right
	0x0029: [ 0x29 ], // RIGHT PARENTHESIS, left to right
	0x002A: [ 0x2A ], // ASTERISK, left to right
	0x002B: [ 0x2B ], // PLUS SIGN, left to right
	0x002C: [ 0x2C ], // COMMA
	0x002D: [ 0x2D ], // HYPHEN-MINUS, left to right
	0x002E: [ 0x2E ], // FULL STOP, left to right
	0x002F: [ 0x2F ], // SOLIDUS, left to right
	0x0030: [ 0x30 ], // DIGIT ZERO
	0x0031: [ 0x31 ], // DIGIT ONE
	0x0032: [ 0x32 ], // DIGIT TWO
	0x0033: [ 0x33 ], // DIGIT THREE
	0x0034: [ 0x34 ], // DIGIT FOUR
	0x0035: [ 0x35 ], // DIGIT FIVE
	0x0036: [ 0x36 ], // DIGIT SIX
	0x0037: [ 0x37 ], // DIGIT SEVEN
	0x0038: [ 0x38 ], // DIGIT EIGHT
	0x0039: [ 0x39 ], // DIGIT NINE
	0x003A: [ 0x3A ], // COLON, left to right
	0x003B: [ 0x3B ], // SEMICOLON
	0x003C: [ 0x3C ], // LESS-THAN SIGN, left to right
	0x003D: [ 0x3D ], // EQUALS SIGN, left to right
	0x003E: [ 0x3E ], // GREATER-THAN SIGN, left to right
	0x003F: [ 0x3F ], // QUESTION MARK
	0x0040: [ 0x40 ], // COMMERCIAL AT, left to right
	0x0041: [ 0x41 ], // LATIN CAPITAL LETTER A
	0x0042: [ 0x42 ], // LATIN CAPITAL LETTER B
	0x0043: [ 0x43 ], // LATIN CAPITAL LETTER C
	0x0044: [ 0x44 ], // LATIN CAPITAL LETTER D
	0x0045: [ 0x45 ], // LATIN CAPITAL LETTER E
	0x0046: [ 0x46 ], // LATIN CAPITAL LETTER F
	0x0047: [ 0x47 ], // LATIN CAPITAL LETTER G
	0x0048: [ 0x48 ], // LATIN CAPITAL LETTER H
	0x0049: [ 0x49 ], // LATIN CAPITAL LETTER I
	0x004A: [ 0x4A ], // LATIN CAPITAL LETTER J
	0x004B: [ 0x4B ], // LATIN CAPITAL LETTER K
	0x004C: [ 0x4C ], // LATIN CAPITAL LETTER L
	0x004D: [ 0x4D ], // LATIN CAPITAL LETTER M
	0x004E: [ 0x4E ], // LATIN CAPITAL LETTER N
	0x004F: [ 0x4F ], // LATIN CAPITAL LETTER O
	0x0050: [ 0x50 ], // LATIN CAPITAL LETTER P
	0x0051: [ 0x51 ], // LATIN CAPITAL LETTER Q
	0x0052: [ 0x52 ], // LATIN CAPITAL LETTER R
	0x0053: [ 0x53 ], // LATIN CAPITAL LETTER S
	0x0054: [ 0x54 ], // LATIN CAPITAL LETTER T
	0x0055: [ 0x55 ], // LATIN CAPITAL LETTER U
	0x0056: [ 0x56 ], // LATIN CAPITAL LETTER V
	0x0057: [ 0x57 ], // LATIN CAPITAL LETTER W
	0x0058: [ 0x58 ], // LATIN CAPITAL LETTER X
	0x0059: [ 0x59 ], // LATIN CAPITAL LETTER Y
	0x005A: [ 0x5A ], // LATIN CAPITAL LETTER Z
	0x005B: [ 0x5B ], // LEFT SQUARE BRACKET, left to right
	0x005C: [ 0x5C ], // REVERSE SOLIDUS, left to right
	0x005D: [ 0x5D ], // RIGHT SQUARE BRACKET, left to right
	0x005E: [ 0x5E ], // CIRCUMFLEX ACCENT, left to right
	0x005F: [ 0x5F ], // LOW LINE, left to right
	0x0060: [ 0x60 ], // GRAVE ACCENT
	0x0061: [ 0x61 ], // LATIN SMALL LETTER A
	0x0062: [ 0x62 ], // LATIN SMALL LETTER B
	0x0063: [ 0x63 ], // LATIN SMALL LETTER C
	0x0064: [ 0x64 ], // LATIN SMALL LETTER D
	0x0065: [ 0x65 ], // LATIN SMALL LETTER E
	0x0066: [ 0x66 ], // LATIN SMALL LETTER F
	0x0067: [ 0x67 ], // LATIN SMALL LETTER G
	0x0068: [ 0x68 ], // LATIN SMALL LETTER H
	0x0069: [ 0x69 ], // LATIN SMALL LETTER I
	0x006A: [ 0x6A ], // LATIN SMALL LETTER J
	0x006B: [ 0x6B ], // LATIN SMALL LETTER K
	0x006C: [ 0x6C ], // LATIN SMALL LETTER L
	0x006D: [ 0x6D ], // LATIN SMALL LETTER M
	0x006E: [ 0x6E ], // LATIN SMALL LETTER N
	0x006F: [ 0x6F ], // LATIN SMALL LETTER O
	0x0070: [ 0x70 ], // LATIN SMALL LETTER P
	0x0071: [ 0x71 ], // LATIN SMALL LETTER Q
	0x0072: [ 0x72 ], // LATIN SMALL LETTER R
	0x0073: [ 0x73 ], // LATIN SMALL LETTER S
	0x0074: [ 0x74 ], // LATIN SMALL LETTER T
	0x0075: [ 0x75 ], // LATIN SMALL LETTER U
	0x0076: [ 0x76 ], // LATIN SMALL LETTER V
	0x0077: [ 0x77 ], // LATIN SMALL LETTER W
	0x0078: [ 0x78 ], // LATIN SMALL LETTER X
	0x0079: [ 0x79 ], // LATIN SMALL LETTER Y
	0x007A: [ 0x7A ], // LATIN SMALL LETTER Z
	0x007B: [ 0x7B ], // LEFT CURLY BRACKET, left to right
	0x007C: [ 0x7C ], // VERTICAL LINE, left to right
	0x007D: [ 0x7D ], // RIGHT CURLY BRACKET, left to right
	0x007E: [ 0x7E ], // TILDE, left to right
	0x0020: [ 0x80 ], // SPACE, right to left
	0x0021: [ 0x81 ], // EXCLAMATION MARK, right to left
	0x0022: [ 0x82 ], // QUOTATION MARK, right to left
	0x0023: [ 0x83 ], // NUMBER SIGN, right to left
	0x0024: [ 0x84 ], // DOLLAR SIGN, right to left
	0x066A: [ 0x85 ], // ARABIC PERCENT SIGN
	0x0026: [ 0x86 ], // AMPERSAND, right to left
	0x0027: [ 0x87 ], // APOSTROPHE, right to left
	0x0028: [ 0x88 ], // LEFT PARENTHESIS, right to left
	0x0029: [ 0x89 ], // RIGHT PARENTHESIS, right to left
	0x002A: [ 0x8A ], // ASTERISK, right to left
	0x002B: [ 0x8B ], // PLUS SIGN, right to left
	0x060C: [ 0x8C ], // ARABIC COMMA
	0x002D: [ 0x8D ], // HYPHEN-MINUS, right to left
	0x002E: [ 0x8E ], // FULL STOP, right to left
	0x002F: [ 0x8F ], // SOLIDUS, right to left
	0x0660: [ 0x90 ], // ARABIC-INDIC DIGIT ZERO
	0x0661: [ 0x91 ], // ARABIC-INDIC DIGIT ONE
	0x0662: [ 0x92 ], // ARABIC-INDIC DIGIT TWO
	0x0663: [ 0x93 ], // ARABIC-INDIC DIGIT THREE
	0x0664: [ 0x94 ], // ARABIC-INDIC DIGIT FOUR
	0x0665: [ 0x95 ], // ARABIC-INDIC DIGIT FIVE
	0x0666: [ 0x96 ], // ARABIC-INDIC DIGIT SIX
	0x0667: [ 0x97 ], // ARABIC-INDIC DIGIT SEVEN
	0x0668: [ 0x98 ], // ARABIC-INDIC DIGIT EIGHT
	0x0669: [ 0x99 ], // ARABIC-INDIC DIGIT NINE
	0x003A: [ 0x9A ], // COLON, right to left
	0x061B: [ 0x9B ], // ARABIC SEMICOLON
	0x003C: [ 0x9C ], // LESS-THAN SIGN, right to left
	0x003D: [ 0x9D ], // EQUALS SIGN, right to left
	0x003E: [ 0x9E ], // GREATER-THAN SIGN, right to left
	0x061F: [ 0x9F ], // ARABIC QUESTION MARK
	0x0040: [ 0xA0 ], // COMMERCIAL AT, right to left
	0xFE8B: [ 0xA1 ], // ARABIC LETTER YEH WITH HAMZA ABOVE INITIAL FORM
	0xFE8C: [ 0xA1 ], // ARABIC LETTER YEH WITH HAMZA ABOVE MEDIAL FORM
	0xFE89: [ 0xA2 ], // ARABIC LETTER YEH WITH HAMZA ABOVE ISOLATED FORM
	0xFE8A: [ 0xA2 ], // ARABIC LETTER YEH WITH HAMZA ABOVE FINAL FORM
	0x0626: [ 0xA2 ], // ARABIC LETTER YEH WITH HAMZA ABOVE
	0xFE91: [ 0xA3 ], // ARABIC LETTER BEH INITIAL FORM
	0xFE92: [ 0xA3 ], // ARABIC LETTER BEH MEDIAL FORM
	0xFE8F: [ 0xA4 ], // ARABIC LETTER BEH ISOLATED FORM
	0xFE90: [ 0xA4 ], // ARABIC LETTER BEH FINAL FORM
	0x0628: [ 0xA4 ], // ARABIC LETTER BEH
	0xFE97: [ 0xA5 ], // ARABIC LETTER TEH INITIAL FORM
	0xFE98: [ 0xA5 ], // ARABIC LETTER TEH MEDIAL FORM
	0xFE95: [ 0xA6 ], // ARABIC LETTER TEH ISOLATED FORM
	0xFE96: [ 0xA6 ], // ARABIC LETTER TEH FINAL FORM
	0x062A: [ 0xA6 ], // ARABIC LETTER TEH
	0xFE9B: [ 0xA7 ], // ARABIC LETTER THEH INITIAL FORM
	0xFE9C: [ 0xA7 ], // ARABIC LETTER THEH MEDIAL FORM
	0xFE99: [ 0xA8 ], // ARABIC LETTER THEH ISOLATED FORM
	0xFE9A: [ 0xA8 ], // ARABIC LETTER THEH FINAL FORM
	0x062B: [ 0xA8 ], // ARABIC LETTER THEH
	0xFE9F: [ 0xA9 ], // ARABIC LETTER JEEM INITIAL FORM
	0xFEA0: [ 0xA9 ], // ARABIC LETTER JEEM MEDIAL FORM
	0xFE9D: [ 0xAA ], // ARABIC LETTER JEEM ISOLATED FORM
	0xFE9E: [ 0xAA ], // ARABIC LETTER JEEM FINAL FORM
	0x062C: [ 0xAA ], // ARABIC LETTER JEEM
	0xFEA3: [ 0xAB ], // ARABIC LETTER HAH INITIAL FORM
	0xFEA4: [ 0xAB ], // ARABIC LETTER HAH MEDIAL FORM
	0xFEA1: [ 0xAC ], // ARABIC LETTER HAH ISOLATED FORM
	0xFEA2: [ 0xAC ], // ARABIC LETTER HAH FINAL FORM
	0x062D: [ 0xAC ], // ARABIC LETTER HAH
	0xFEA7: [ 0xAD ], // ARABIC LETTER KHAH INITIAL FORM
	0xFEA8: [ 0xAD ], // ARABIC LETTER KHAH MEDIAL FORM
	0xFEA5: [ 0xAE ], // ARABIC LETTER KHAH ISOLATED FORM
	0xFEA6: [ 0xAE ], // ARABIC LETTER KHAH FINAL FORM
	0x062E: [ 0xAE ], // ARABIC LETTER KHAH
	0xFEB3: [ 0xAF ], // ARABIC LETTER SEEN INITIAL FORM
	0xFEB4: [ 0xAF ], // ARABIC LETTER SEEN MEDIAL FORM
	0xFEB1: [ 0xB0 ], // ARABIC LETTER SEEN ISOLATED FORM
	0xFEB2: [ 0xB0 ], // ARABIC LETTER SEEN FINAL FORM
	0x0633: [ 0xB0 ], // ARABIC LETTER SEEN
	0xFEB7: [ 0xB1 ], // ARABIC LETTER SHEEN INITIAL FORM
	0xFEB8: [ 0xB1 ], // ARABIC LETTER SHEEN MEDIAL FORM
	0xFEB5: [ 0xB2 ], // ARABIC LETTER SHEEN ISOLATED FORM
	0xFEB6: [ 0xB2 ], // ARABIC LETTER SHEEN FINAL FORM
	0x0634: [ 0xB2 ], // ARABIC LETTER SHEEN
	0xFEBB: [ 0xB3 ], // ARABIC LETTER SAD INITIAL FORM
	0xFEBC: [ 0xB3 ], // ARABIC LETTER SAD MEDIAL FORM
	0xFEB9: [ 0xB4 ], // ARABIC LETTER SAD ISOLATED FORM
	0xFEBA: [ 0xB4 ], // ARABIC LETTER SAD FINAL FORM
	0x0635: [ 0xB4 ], // ARABIC LETTER SAD
	0xFEBF: [ 0xB5 ], // ARABIC LETTER DAD INITIAL FORM
	0xFEC0: [ 0xB5 ], // ARABIC LETTER DAD MEDIAL FORM
	0xFEBD: [ 0xB6 ], // ARABIC LETTER DAD ISOLATED FORM
	0xFEBE: [ 0xB6 ], // ARABIC LETTER DAD FINAL FORM
	0x0636: [ 0xB6 ], // ARABIC LETTER DAD
	0xFEC3: [ 0xB7 ], // ARABIC LETTER TAH INITIAL FORM
	0xFEC4: [ 0xB7 ], // ARABIC LETTER TAH MEDIAL FORM
	0xFEC1: [ 0xB7 ], // ARABIC LETTER TAH ISOLATED FORM
	0xFEC2: [ 0xB7 ], // ARABIC LETTER TAH FINAL FORM
	0x0637: [ 0xB7 ], // ARABIC LETTER TAH
	0xFEC7: [ 0xB8 ], // ARABIC LETTER ZAH INITIAL FORM
	0xFEC8: [ 0xB8 ], // ARABIC LETTER ZAH MEDIAL FORM
	0xFEC5: [ 0xB8 ], // ARABIC LETTER ZAH ISOLATED FORM
	0xFEC6: [ 0xB8 ], // ARABIC LETTER ZAH FINAL FORM
	0x0638: [ 0xB8 ], // ARABIC LETTER ZAH
	0xFECB: [ 0xB9 ], // ARABIC LETTER AIN INITIAL FORM
	0xFEC9: [ 0xBA ], // ARABIC LETTER AIN ISOLATED FORM
	0x0639: [ 0xBA ], // ARABIC LETTER AIN
	0x005B: [ 0xBB ], // LEFT SQUARE BRACKET, right to left
	0x005C: [ 0xBC ], // REVERSE SOLIDUS, right to left
	0x005D: [ 0xBD ], // RIGHT SQUARE BRACKET, right to left
	0x005E: [ 0xBE ], // CIRCUMFLEX ACCENT, right to left
	0x005F: [ 0xBF ], // LOW LINE, right to left
	0xFECC: [ 0xC0 ], // ARABIC LETTER AIN MEDIAL FORM
	0xFECA: [ 0xC1 ], // ARABIC LETTER AIN FINAL FORM
	0xFECF: [ 0xC2 ], // ARABIC LETTER GHAIN INITIAL FORM
	0xFECD: [ 0xC3 ], // ARABIC LETTER GHAIN ISOLATED FORM
	0x063A: [ 0xC3 ], // ARABIC LETTER GHAIN
	0xFED0: [ 0xC4 ], // ARABIC LETTER GHAIN MEDIAL FORM
	0xFECE: [ 0xC5 ], // ARABIC LETTER GHAIN FINAL FORM
	0xFED3: [ 0xC6 ], // ARABIC LETTER FEH INITIAL FORM
	0xFED4: [ 0xC6 ], // ARABIC LETTER FEH MEDIAL FORM
	0xFED1: [ 0xC7 ], // ARABIC LETTER FEH ISOLATED FORM
	0xFED2: [ 0xC7 ], // ARABIC LETTER FEH FINAL FORM
	0x0641: [ 0xC7 ], // ARABIC LETTER FEH
	0xFED7: [ 0xC8 ], // ARABIC LETTER QAF INITIAL FORM
	0xFED8: [ 0xC8 ], // ARABIC LETTER QAF MEDIAL FORM
	0xFED5: [ 0xC9 ], // ARABIC LETTER QAF ISOLATED FORM
	0xFED6: [ 0xC9 ], // ARABIC LETTER QAF FINAL FORM
	0x0642: [ 0xC9 ], // ARABIC LETTER QAF
	0xFEDB: [ 0xCA ], // ARABIC LETTER KAF INITIAL FORM
	0xFEDC: [ 0xCA ], // ARABIC LETTER KAF MEDIAL FORM
	0xFED9: [ 0xCB ], // ARABIC LETTER KAF ISOLATED FORM
	0xFEDA: [ 0xCB ], // ARABIC LETTER KAF FINAL FORM
	0x0643: [ 0xCB ], // ARABIC LETTER KAF
	0xFEDF: [ 0xCC ], // ARABIC LETTER LAM INITIAL FORM
	0xFEE0: [ 0xCC ], // ARABIC LETTER LAM MEDIAL FORM
	0xFEDD: [ 0xCD ], // ARABIC LETTER LAM ISOLATED FORM
	0xFEDE: [ 0xCD ], // ARABIC LETTER LAM FINAL FORM
	0x0644: [ 0xCD ], // ARABIC LETTER LAM
	0xFEE3: [ 0xCE ], // ARABIC LETTER MEEM INITIAL FORM
	0xFEE4: [ 0xCE ], // ARABIC LETTER MEEM MEDIAL FORM
	0xFEE1: [ 0xCF ], // ARABIC LETTER MEEM ISOLATED FORM
	0xFEE2: [ 0xCF ], // ARABIC LETTER MEEM FINAL FORM
	0x0645: [ 0xCF ], // ARABIC LETTER MEEM
	0xFEE7: [ 0xD0 ], // ARABIC LETTER NOON INITIAL FORM
	0xFEE8: [ 0xD0 ], // ARABIC LETTER NOON MEDIAL FORM
	0xFEE5: [ 0xD1 ], // ARABIC LETTER NOON ISOLATED FORM
	0xFEE6: [ 0xD1 ], // ARABIC LETTER NOON FINAL FORM
	0x0646: [ 0xD1 ], // ARABIC LETTER NOON
	0xFEEB: [ 0xD2 ], // ARABIC LETTER HEH INITIAL FORM
	0xFEEC: [ 0xD2 ], // ARABIC LETTER HEH MEDIAL FORM
	0xFEE9: [ 0xD3 ], // ARABIC LETTER HEH ISOLATED FORM
	0xFEEA: [ 0xD3 ], // ARABIC LETTER HEH FINAL FORM
	0x0647: [ 0xD3 ], // ARABIC LETTER HEH
	0xFEF3: [ 0xD4 ], // ARABIC LETTER YEH INITIAL FORM
	0xFEF4: [ 0xD4 ], // ARABIC LETTER YEH MEDIAL FORM
	0xFEF1: [ 0xD5 ], // ARABIC LETTER YEH ISOLATED FORM
	0x064A: [ 0xD5 ], // ARABIC LETTER YEH
	0xFEF2: [ 0xD6 ], // ARABIC LETTER YEH FINAL FORM
	0xFE81: [ 0xD7 ], // ARABIC LETTER ALEF WITH MADDA ABOVE ISOLATED FORM
	0x0622: [ 0xD7 ], // ARABIC LETTER ALEF WITH MADDA ABOVE
	0xFE82: [ 0xD8 ], // ARABIC LETTER ALEF WITH MADDA ABOVE FINAL FORM
	0xFE83: [ 0xD9 ], // ARABIC LETTER ALEF WITH HAMZA ABOVE ISOLATED FORM
	0x0623: [ 0xD9 ], // ARABIC LETTER ALEF WITH HAMZA ABOVE
	0xFE84: [ 0xDA ], // ARABIC LETTER ALEF WITH HAMZA ABOVE FINAL FORM
	0x007B: [ 0xDB ], // LEFT CURLY BRACKET, right to left
	0x007C: [ 0xDC ], // VERTICAL LINE, right to left
	0x007D: [ 0xDD ], // RIGHT CURLY BRACKET, right to left
	0x007E: [ 0xDE ], // TILDE, right to left
	0xFE85: [ 0xDF ], // ARABIC LETTER WAW WITH HAMZA ABOVE ISOLATED FORM
	0xFE86: [ 0xDF ], // ARABIC LETTER WAW WITH HAMZA ABOVE FINAL FORM
	0x0624: [ 0xDF ], // ARABIC LETTER WAW WITH HAMZA ABOVE
	0xFE87: [ 0xE0 ], // ARABIC LETTER ALEF WITH HAMZA BELOW ISOLATED FORM
	0x0625: [ 0xE0 ], // ARABIC LETTER ALEF WITH HAMZA BELOW
	0xFE88: [ 0xE1 ], // ARABIC LETTER ALEF WITH HAMZA BELOW FINAL FORM
	0xFE8D: [ 0xE2 ], // ARABIC LETTER ALEF ISOLATED FORM
	0x0627: [ 0xE2 ], // ARABIC LETTER ALEF
	0xFE8E: [ 0xE3 ], // ARABIC LETTER ALEF FINAL FORM
	0xFE93: [ 0xE4 ], // ARABIC LETTER TEH MARBUTA ISOLATED FORM
	0xFE94: [ 0xE4 ], // ARABIC LETTER TEH MARBUTA FINAL FORM
	0x0629: [ 0xE4 ], // ARABIC LETTER TEH MARBUTA
	0xFEA9: [ 0xE5 ], // ARABIC LETTER DAL ISOLATED FORM
	0xFEAA: [ 0xE5 ], // ARABIC LETTER DAL FINAL FORM
	0x062F: [ 0xE5 ], // ARABIC LETTER DAL
	0xFEAB: [ 0xE6 ], // ARABIC LETTER THAL ISOLATED FORM
	0xFEAC: [ 0xE6 ], // ARABIC LETTER THAL FINAL FORM
	0x0630: [ 0xE6 ], // ARABIC LETTER THAL
	0xFEAD: [ 0xE7 ], // ARABIC LETTER REH ISOLATED FORM
	0xFEAE: [ 0xE7 ], // ARABIC LETTER REH FINAL FORM
	0x0631: [ 0xE7 ], // ARABIC LETTER REH
	0xFEAF: [ 0xE8 ], // ARABIC LETTER ZAIN ISOLATED FORM
	0xFEB0: [ 0xE8 ], // ARABIC LETTER ZAIN FINAL FORM
	0x0632: [ 0xE8 ], // ARABIC LETTER ZAIN
	0xFEED: [ 0xE9 ], // ARABIC LETTER WAW ISOLATED FORM
	0xFEEE: [ 0xE9 ], // ARABIC LETTER WAW FINAL FORM
	0x0648: [ 0xE9 ], // ARABIC LETTER WAW
	0xFEEF: [ 0xEA ], // ARABIC LETTER ALEF MAKSURA ISOLATED FORM
	0x0649: [ 0xEA ], // ARABIC LETTER ALEF MAKSURA
	0xFEF0: [ 0xEB ], // ARABIC LETTER ALEF MAKSURA FINAL FORM
	0xFEFB: [ 0xEC ], // ARABIC LIGATURE LAM WITH ALEF ISOLATED FORM
	0xFEFC: [ 0xEC ], // ARABIC LIGATURE LAM WITH ALEF FINAL FORM
	0xFEF7: [ 0xED ], // ARABIC LIGATURE LAM WITH ALEF WITH HAMZA ABOVE ISOLATED FORM
	0xFEF8: [ 0xED ], // ARABIC LIGATURE LAM WITH ALEF WITH HAMZA ABOVE FINAL FORM
	0xFEF5: [ 0xEE ], // ARABIC LIGATURE LAM WITH ALEF WITH MADDA ABOVE ISOLATED FORM
	0xFEF6: [ 0xEE ], // ARABIC LIGATURE LAM WITH ALEF WITH MADDA ABOVE FINAL FORM
	0xFEF9: [ 0xEF ], // ARABIC LIGATURE LAM WITH ALEF WITH HAMZA BELOW ISOLATED FORM
	0xFEFA: [ 0xEF ], // ARABIC LIGATURE LAM WITH ALEF WITH HAMZA BELOW FINAL FORM
	0xFE80: [ 0xF0 ], // ARABIC LETTER HAMZA ISOLATED FORM
	0x0621: [ 0xF0 ], // ARABIC LETTER HAMZA
	0x0640: [ 0xF1 ], // ARABIC TATWEEL
	0xFE70: [ 0xF2 ], // ARABIC FATHATAN ISOLATED FORM
	0x064B: [ 0xF2 ], // ARABIC FATHATAN
	0xFE72: [ 0xF3 ], // ARABIC DAMMATAN ISOLATED FORM
	0x064C: [ 0xF3 ], // ARABIC DAMMATAN
	0xFE74: [ 0xF4 ], // ARABIC KASRATAN ISOLATED FORM
	0x064D: [ 0xF4 ], // ARABIC KASRATAN
	0xFE76: [ 0xF5 ], // ARABIC FATHA ISOLATED FORM
	0x064E: [ 0xF5 ], // ARABIC FATHA
	0xFE77: [ 0xF6 ], // ARABIC FATHA MEDIAL FORM
	0xFE78: [ 0xF7 ], // ARABIC DAMMA ISOLATED FORM
	0x064F: [ 0xF7 ], // ARABIC DAMMA
	0xFE79: [ 0xF8 ], // ARABIC DAMMA MEDIAL FORM
	0xFE7A: [ 0xF9 ], // ARABIC KASRA ISOLATED FORM
	0x0650: [ 0xF9 ], // ARABIC KASRA
	0xFE7B: [ 0xFA ], // ARABIC KASRA MEDIAL FORM
	0xFE7C: [ 0xFB ], // ARABIC SHADDA ISOLATED FORM
	0x0651: [ 0xFB ], // ARABIC SHADDA
	0xFE7D: [ 0xFC ], // ARABIC SHADDA MEDIAL FORM
	0xFE7E: [ 0xFD ], // ARABIC SUKUN ISOLATED FORM
	0x0652: [ 0xFD ], // ARABIC SUKUN
	0xFE7F: [ 0xFE ], // ARABIC SUKUN MEDIAL FORM
};


// Arabic character forms conversion
const ArabicCharForms = {
	//        Isolat  Final   Medial  Initi
	0x0627: [ 0xFE8D, 0xFE8E, 0xFE8E, 0xFE8D ], // ʾalif
	0x0628: [ 0xFE8F, 0xFE90, 0xFE92, 0xFE91 ], // bāʾ
	0x062A: [ 0xFE95, 0xFE96, 0xFE98, 0xFE97 ], // tāʾ
	0x062B: [ 0xFE99, 0xFE9A, 0xFE9C, 0xFE9B ], // thāʾ
	0x062C: [ 0xFE9D, 0xFE9E, 0xFEA0, 0xFE9F ], // jīm
	0x062D: [ 0xFEA1, 0xFEA2, 0xFEA4, 0xFEA3 ], // ḥāʾ
	0x062E: [ 0xFEA5, 0xFEA6, 0xFEA8, 0xFEA7 ], // khāʾ
	0x062F: [ 0xFEA9, 0xFEAA, 0xFEAA, 0xFEA9 ], // dāl
	0x0630: [ 0xFEAB, 0xFEAC, 0xFEAC, 0xFEAB ], // dhāl
	0x0631: [ 0xFEAD, 0xFEAE, 0xFEAE, 0xFEAD ], // rāʾ
	0x0632: [ 0xFEAF, 0xFEB0, 0xFEB0, 0xFEAF ], // zāy
	0x0633: [ 0xFEB1, 0xFEB2, 0xFEB4, 0xFEB3 ], // sīn
	0x0634: [ 0xFEB5, 0xFEB6, 0xFEB8, 0xFEB7 ], // shīn
	0x0635: [ 0xFEB9, 0xFEBA, 0xFEBC, 0xFEBB ], // ṣād
	0x0636: [ 0xFEBD, 0xFEBE, 0xFEC0, 0xFEBF ], // ḍād
	0x0637: [ 0xFEC1, 0xFEC2, 0xFEC4, 0xFEC3 ], // ṭāʾ
	0x0638: [ 0xFEC5, 0xFEC6, 0xFEC8, 0xFEC7 ], // ẓāʾ
	0x0639: [ 0xFEC9, 0xFECA, 0xFECC, 0xFECB ], // ʿayn
	0x063A: [ 0xFECD, 0xFECE, 0xFED0, 0xFECF ], // ghayn
	0x0641: [ 0xFED1, 0xFED2, 0xFED4, 0xFED3 ], // fāʾ
	0x0642: [ 0xFED5, 0xFED6, 0xFED8, 0xFED7 ], // qāf
	0x0643: [ 0xFED9, 0xFEDA, 0xFEDC, 0xFEDB ], // kāf
	0x0644: [ 0xFEDD, 0xFEDE, 0xFEE0, 0xFEDF ], // lām
	0x0645: [ 0xFEE1, 0xFEE2, 0xFEE4, 0xFEE3 ], // mīm
	0x0646: [ 0xFEE5, 0xFEE6, 0xFEE8, 0xFEE7 ], // nūn
	0x0647: [ 0xFEE9, 0xFEEA, 0xFEEC, 0xFEEB ], // hāʾ
	0x0648: [ 0xFEED, 0xFEEE, 0xFEEE, 0xFEED ], // wāw
	0x064A: [ 0xFEF1, 0xFEF2, 0xFEF4, 0xFEF3 ], // yāʾ
	0x0622: [ 0xFE81, 0xFE82, 0xFE82, 0xFE81 ], // ʾalif maddah
	0x0629: [ 0xFE93, 0xFE94, 0xFE94, 0xFE93 ], // tāʾ marbūṭah
	0x0649: [ 0xFEEF, 0xFEF0, 0xFEF0, 0xFEEF ], // ʾalif maqṣūrah
	//        Isolat  Final   Medial  Initi
	0xFEFB: [ 0xFEFB, 0xFEFC, 0xFEFB, 0xFEFB ], // Ligature lam with alef
	0xFEF7: [ 0xFEF7, 0xFEF8, 0xFEF7, 0xFEF7 ], // Ligature lam with alef with hamza above
	0xFEF5: [ 0xFEF5, 0xFEF6, 0xFEF5, 0xFEF5 ], // Ligature lam with alef with madda above
	0xFEF9: [ 0xFEF9, 0xFEFA, 0xFEF9, 0xFEF9 ], // Ligature lam with alef with hamza below
	//        Isolat  Final   Medial  Initi
	0x064E: [ 0x064E, 0x064E, 0xFE77, 0x064E ],
	0x064F: [ 0x064F, 0x064F, 0xFE79, 0x064F ],
	0x0650: [ 0x0650, 0x0650, 0xFE7B, 0x0650 ],
	0x0651: [ 0x0651, 0x0651, 0xFE7D, 0x0651 ],
	0x0652: [ 0x0652, 0x0652, 0xFE7F, 0x0652 ],
};

// Arabic character combinations
const ArabicCombinations = [
	{ List: [ 0x0644, 0x0627 ], Char: 0xFEFB },
	{ List: [ 0x0644, 0x0623 ], Char: 0xFEF7 },
	{ List: [ 0x0644, 0x0622 ], Char: 0xFEF5 },
	{ List: [ 0x0644, 0x0625 ], Char: 0xFEF9 },
	// tatweel + Tashkeel = Tashkeel
	{ List: [ 0x0640, 0x064B ], Char: 0x064B },
	{ List: [ 0x0640, 0x064C ], Char: 0x064C },
	{ List: [ 0x0640, 0x064D ], Char: 0x064D },
	{ List: [ 0x0640, 0x064E ], Char: 0x064E },
	{ List: [ 0x0640, 0x064F ], Char: 0x064F },
	{ List: [ 0x0640, 0x0650 ], Char: 0x0650 },
	{ List: [ 0x0640, 0x0651 ], Char: 0x0651 },
	{ List: [ 0x0640, 0x0652 ], Char: 0x0652 },
	// { List: [ 0x0640, 0x0640, 0x064B ], Char: 0x064B },
	// { List: [ 0x0640, 0x0640, 0x064C ], Char: 0x064C },
	// { List: [ 0x0640, 0x0640, 0x064D ], Char: 0x064D },
	// { List: [ 0x0640, 0x0640, 0x064E ], Char: 0x064E },
	// { List: [ 0x0640, 0x0640, 0x064F ], Char: 0x064F },
	// { List: [ 0x0640, 0x0640, 0x0650 ], Char: 0x0650 },
	// { List: [ 0x0640, 0x0640, 0x0651 ], Char: 0x0651 },
	// { List: [ 0x0640, 0x0640, 0x0652 ], Char: 0x0652 },
	// { List: [ 0x0640, 0x0640 ], Char: 0x0640 },
];

// Non-letter Arabic characters
const nonLetterArabicCharacters = [
	0x066A, 0x060C, 
	0x0660, 0x0661, 0x0662, 0x0663, 0x0664, 0x0665, 0x0666, 0x0667, 0x0668, 0x0669, // Arabic-indic digit
	0x061B, 0x061F,
];

// Arabic Tashkeels characters
const arabicTashkeels = [
	0x064B, 0x064C, 0x064D, 0x064E, 0x064F, 0x0650, 0x0651, 0x0652
];

//-----------------------------------------------------------------------------
// Chec if the given character is a Arabic letter
function isArabicLetter(chr)
{
	const charCode = chr.codePointAt(0);
	if(
		(charCode < 0x0600 || charCode > 0x06FF) &&  // Arabic
		(charCode < 0x0750 || charCode > 0x077F) &&  // Arabic Supplement
		(charCode < 0xFB50 || charCode > 0xFDFF) &&  // Arabic Presentation Forms
		(charCode < 0xFE70 || charCode > 0xFEFF))    // Arabic Presentation Forms-B
		return false;

	return nonLetterArabicCharacters.includes(charCode) == false;
}

//-----------------------------------------------------------------------------
// 
function hasNext(str, i)
{
	if(i == str.length - 1)
		return false;

	const charCode = str[i + 1].codePointAt(0);
	if(arabicTashkeels.includes(charCode))
	{
		return hasNext(str, i + 1);
	}
	
	return isArabicLetter(str[i + 1]);
}

//-----------------------------------------------------------------------------
// 
function hasPrev(str, i)
{
	if (i == 0)
		return false;

	const charCode = str[i - 1].codePointAt(0);
	if(arabicTashkeels.includes(charCode))
	{
		return hasPrev(str, i - 1);
	}
	
	return isArabicLetter(str[i - 1]);
}

//-----------------------------------------------------------------------------
// 
function convertArabicForms(inputString)
{
	let outputString = "";

	for (let i = 0; i < inputString.length; i++)
	{
		const currentChar = inputString[i];
		const nextChar = hasNext(inputString, i)
		const prevChar = hasPrev(inputString, i);
		const charCode = currentChar.codePointAt(0);

		// Check if the character is Arabic
		var form = ArabicCharForms[charCode];
		if (form != null)
		{
			if (!nextChar && !prevChar) // Isolated form
				outputString += String.fromCharCode(form[0]);
			else if (!nextChar && prevChar) // End form
				outputString += String.fromCharCode(form[1]);
			else if (nextChar && prevChar) // Middle form
				outputString += String.fromCharCode(form[2]);
			else if (nextChar && !prevChar) // Start form
				outputString += String.fromCharCode(form[3]);
		}
		else // Keep non-Arabic characters as they are
			outputString += currentChar;
	}
	return outputString;
}

//-----------------------------------------------------------------------------
// Get character class
function getCharClass(chr)
{
	const charCode = chr.charCodeAt(0);
  
	// Check Unicode ranges for writing direction
	if (
		(charCode >= 0x0030 && charCode <= 0x0039)) {  // European number
		return CharClass.Neutral;
	} else if (
		(charCode >= 0x0041 && charCode <= 0x005A) ||  // Latin uppercase letters
		(charCode >= 0x0061 && charCode <= 0x007A)) {  // Latin lowercase letters
		return CharClass.LeftToRight;
	} else if (
		(charCode >= 0x0600 && charCode <= 0x06FF) ||  // Arabic
		(charCode >= 0x0750 && charCode <= 0x077F) ||  // Arabic Supplement
		(charCode >= 0xFB50 && charCode <= 0xFDFF) ||  // Arabic Presentation Forms
		(charCode >= 0xFE70 && charCode <= 0xFEFF)) {  // Arabic Presentation Forms-B
		return CharClass.RightToLeft;
	}
	// Default to 'Not Applicable' for other characters
	return CharClass.Weak;
}

//-----------------------------------------------------------------------------
// Convert Bi-Directionnal text to linear LTR one
function convertArabicOrder(str)
{
	var lastDir = CharClass.RightToLeft;
	var tempStr = "";
	var outStr = "";
	for(var i = 0; i < str.length; i++)
	{
		var chr = str[i];
		var dir = getCharClass(chr);
		if((dir == CharClass.LeftToRight) || (dir == CharClass.Neutral))
		{
			tempStr += chr;
		}
		else
		{
			if((lastDir == CharClass.LeftToRight) || (lastDir == CharClass.Neutral))
			{
				outStr = tempStr + outStr;
				tempStr = "";
			}
			outStr = chr + outStr;
		}
		lastDir = dir;
	}
	if(tempStr != "")
	{
		outStr = tempStr + outStr;
	}
	return outStr;
}

//-----------------------------------------------------------------------------
// Merge Arabic characters combination
function convertArabicCombination(str)
{
	var outStr = "";
	for(var i = 0; i < str.length; i++)
	{
		const chr = str[i];
		var found = false;
		if(i < str.length - 1)
		{
			var combi = {};
			for(var j = 0; j < ArabicCombinations.length; j++)
			{
				combi = ArabicCombinations[j];
				var match = true;
				for(var k = 0; k < combi.List.length; k++)
				{
					const code = str[i + k].codePointAt(0);
					if(combi.List[k] != code)
					{
						match = false;
						break;
					}
				}
				if(match)
				{
					found = true;
					outStr += String.fromCharCode(combi.Char);
					i += combi.List.length - 1;
					break;
				}
			}
		}
		if(!found)
			outStr += chr;
	}
	return outStr;
}

// Convert string to match MSX system font
module.exports.convertToMSX = function (str)
{
	str = convertArabicCombination(str);
	str = convertArabicForms(str);
	str = convertArabicOrder(str);
	return locUtil.mapCharacter(str, MapARBtoMSX);
}