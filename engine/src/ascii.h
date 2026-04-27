// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#define ASCII_CTRL_A				1
#define ASCII_GRAPH					1
#define ASCII_CTRL_B				2
#define ASCII_CTRL_C				3
#define ASCII_CTRL_E				5
#define ASCII_CTRL_F				6
#define ASCII_CTRL_G				7
#define ASCII_BEEP					7
#define ASCII_CTRL_H				8
#define ASCII_BS					8
#define ASCII_CTRL_I				9
#define ASCII_TAB					9
#define ASCII_CTRL_J				10
#define ASCII_LF					10
#define ASCII_CTRL_K				11
#define ASCII_HOME					11
#define ASCII_CTRL_L				12
#define ASCII_CLS					12
#define ASCII_CTRL_M				13
#define ASCII_RET					13
#define ASCII_RETURN				13
#define ASCII_CTRL_N				14
#define ASCII_CTRL_O				15
#define ASCII_CTRL_P				16
#define ASCII_CTRL_Q				17
#define ASCII_CTRL_R				18
#define ASCII_INSERT				18
#define ASCII_CTRL_S				19
#define ASCII_CTRL_T				20
#define ASCII_CTRL_U				21
#define ASCII_CTRL_X				24
#define ASCII_SELECT				24
#define ASCII_CTRL_Y				25
#define ASCII_CTRL_W				26
#define ASCII_ESC					27
#define ASCII_RIGHT					28
#define ASCII_LEFT					29
#define ASCII_UP					30
#define ASCII_DOWN					31
#define ASCII_SPACE					32
#define ASCII_EXCLAMATION_MARK		33	// !	Exclamation mark
#define ASCII_DOUBLE_QUOTE			34	// "	Double quote
#define ASCII_NUMBER				35	// #	Number
#define ASCII_DOLLAR_SIGN			36	// $	Dollar sign
#define ASCII_PERCENT				37	// %	Percent
#define ASCII_AMPERSAND				38	// &	Ampersand
#define ASCII_SINGLE_QUOTE			39	// '	Single quote
#define ASCII_LEFT_PARENTHESIS		40	// (	Left parenthesis
#define ASCII_RIGHT_PARENTHESIS		41	// )	Right parenthesis
#define ASCII_ASTERISK				42	// *	Asterisk
#define ASCII_PLUS					43	// +	Plus
#define ASCII_COMMA					44	// ,	Comma
#define ASCII_MINUS					45	// -	Minus
#define ASCII_PERIOD				46	// .	Period
#define ASCII_SLASH					47	// /	Slash
#define ASCII_ZERO					48	// 0	Zero
#define ASCII_ONE					49	// 1	One
#define ASCII_TWO					50	// 2	Two
#define ASCII_THREE					51	// 3	Three
#define ASCII_FOUR					52	// 4	Four
#define ASCII_FIVE					53	// 5	Five
#define ASCII_SIX					54	// 6	Six
#define ASCII_SEVEN					55	// 7	Seven
#define ASCII_EIGHT					56	// 8	Eight
#define ASCII_NINE					57	// 9	Nine
#define ASCII_COLON					58	// :	Colon
#define ASCII_SEMICOLON				59	// ;	Semicolon
#define ASCII_LESS_THAN				60	// <	Less than
#define ASCII_EQUALITY_SIGN			61	// =	Equality sign
#define ASCII_GREATER_THAN			62	// >	Greater than
#define ASCII_QUESTION_MARK			63	// ?	Question mark
#define ASCII_AT_SIGN				64	// @	At sign
#define ASCII_CAPITAL_A				65	// A	Capital A
#define ASCII_CAPITAL_B				66	// B	Capital B
#define ASCII_CAPITAL_C				67	// C	Capital C
#define ASCII_CAPITAL_D				68	// D	Capital D
#define ASCII_CAPITAL_E				69	// E	Capital E
#define ASCII_CAPITAL_F				70	// F	Capital F
#define ASCII_CAPITAL_G				71	// G	Capital G
#define ASCII_CAPITAL_H				72	// H	Capital H
#define ASCII_CAPITAL_I				73	// I	Capital I
#define ASCII_CAPITAL_J				74	// J	Capital J
#define ASCII_CAPITAL_K				75	// K	Capital K
#define ASCII_CAPITAL_L				76	// L	Capital L
#define ASCII_CAPITAL_M				77	// M	Capital M
#define ASCII_CAPITAL_N				78	// N	Capital N
#define ASCII_CAPITAL_O				79	// O	Capital O
#define ASCII_CAPITAL_P				80	// P	Capital P
#define ASCII_CAPITAL_Q				81	// Q	Capital Q
#define ASCII_CAPITAL_R				82	// R	Capital R
#define ASCII_CAPITAL_S				83	// S	Capital S
#define ASCII_CAPITAL_T				84	// T	Capital T
#define ASCII_CAPITAL_U				85	// U	Capital U
#define ASCII_CAPITAL_V				86	// V	Capital V
#define ASCII_CAPITAL_W				87	// W	Capital W
#define ASCII_CAPITAL_X				88	// X	Capital X
#define ASCII_CAPITAL_Y				89	// Y	Capital Y
#define ASCII_CAPITAL_Z				90	// Z	Capital Z
#define ASCII_LEFT_SQUARE_BRACKET	91	// [	Left square bracket
#define ASCII_BACKSLASH				92	// \	Backslash
#define ASCII_RIGHT_SQUARE_BRACKET	93	// ]	Right square bracket
#define ASCII_CARET					94	// ^	Caret / circumflex
#define ASCII_CIRCUMFLEX			94	// ^	Caret / circumflex
#define ASCII_UNDERSCORE			95	// _	Underscore
#define ASCII_GRAVE					96	// `	Grave / accent
#define ASCII_ACCENT				96	// `	Grave / accent
#define ASCII_SMALL_A				97	// a	Small a
#define ASCII_SMALL_B				98	// b	Small b
#define ASCII_SMALL_C				99	// c	Small c
#define ASCII_SMALL_D				100	// d	Small d
#define ASCII_SMALL_E				101	// e	Small e
#define ASCII_SMALL_F				102	// f	Small f
#define ASCII_SMALL_G				103	// g	Small g
#define ASCII_SMALL_H				104	// h	Small h
#define ASCII_SMALL_I				105	// i	Small i
#define ASCII_SMALL_J				106	// j	Small j
#define ASCII_SMALL_K				107	// k	Small k
#define ASCII_SMALL_L				108	// l	Small l
#define ASCII_SMALL_M				109	// m	Small m
#define ASCII_SMALL_N				110	// n	Small n
#define ASCII_SMALL_O				111	// o	Small o
#define ASCII_SMALL_P				112	// p	Small p
#define ASCII_SMALL_Q				113	// q	Small q
#define ASCII_SMALL_R				114	// r	Small r
#define ASCII_SMALL_S				115	// s	Small s
#define ASCII_SMALL_T				116	// t	Small t
#define ASCII_SMALL_U				117	// u	Small u
#define ASCII_SMALL_V				118	// v	Small v
#define ASCII_SMALL_W				119	// w	Small w
#define ASCII_SMALL_X				120	// x	Small x
#define ASCII_SMALL_Y				121	// y	Small y
#define ASCII_SMALL_Z				122	// z	Small z
#define ASCII_LEFT_CURLY_BRACKET	123	// {	Left curly bracket
#define ASCII_VERTICAL_BAR			124	// |	Vertical bar
#define ASCII_RIGHT_CURLY_BRACKET	125	// }	Right curly bracket
#define ASCII_TILDE					126	// ~	Tilde
#define ASCII_DELETE				127	// DEL	Delete