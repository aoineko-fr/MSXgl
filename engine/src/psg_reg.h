// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄▄  ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ▀█▄  ██  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ▄▄█▀ ▀█▄█
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// PSG handler module
//
// References:
//  - AY-3-8910 Programmable Sound Generator Data Manual
//     http://map.grauw.nl/resources/sound/generalinstrument_ay-3-8910.pdf
//  - YM2149 Programmable Software-Controlled Sound Generator Manual
//     http://map.grauw.nl/resources/sound/yamaha_ym2149.pdf
//  - The Revised MSX Red Book - PSG
//     https://www.angelfire.com/art2/unicorndreams/msx/RR-PSG.html
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//-----------------------------------------------------------------------------
// R#0    Tone Generator Control - Fine Tune Register - Channel A
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	TP7 TP6 TP5 TP4 TP3 TP2 TP1 TP0
//	└───┴───┴───┴───┴───┴───┴───┴── Channel A Frequency, LSB (fine tune)
#define PSG_REG_TONE_A		0
#define PSG_REG_TONEF_A		0

//-----------------------------------------------------------------------------
// R#1    Tone Generator Control - Coarse Tune Register - Channel A
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x   x   x   x   TPB TPA TP9 TP8
//	                └───┴───┴───┴── Channel A Frequency, MSB (coarse tune)
#define PSG_REG_TONEC_A		1

//-----------------------------------------------------------------------------
// R#2    Tone Generator Control - Fine Tune Register - Channel B
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	TP7 TP6 TP5 TP4 TP3 TP2 TP1 TP0
//	└───┴───┴───┴───┴───┴───┴───┴── Channel B Frequency, LSB (fine tune)
#define PSG_REG_TONE_B		2
#define PSG_REG_TONEF_B		2

//-----------------------------------------------------------------------------
// R#3    Tone Generator Control - Coarse Tune Register - Channel B
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x   x   x   x   TPB TPA TP9 TP8
//	                └───┴───┴───┴── Channel B Frequency, MSB (coarse tune)
#define PSG_REG_TONEC_B		3

//-----------------------------------------------------------------------------
// R#4    Tone Generator Control - Fine Tune Register - Channel C
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	TP7 TP6 TP5 TP4 TP3 TP2 TP1 TP0
//	└───┴───┴───┴───┴───┴───┴───┴── Channel C Frequency, LSB (fine tune)
#define PSG_REG_TONE_C		4
#define PSG_REG_TONEF_C		4

//-----------------------------------------------------------------------------
// R#5    Tone Generator Control - Coarse Tune Register - Channel C
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	x   x   x   x   TPB TPA TP9 TP8 	
//	                └───┴───┴───┴── Channel C Frequency, MSB (coarse tune)
#define PSG_REG_TONEC_C		5

//-----------------------------------------------------------------------------
// R#6    Noise Generator Control
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	x   x   x   NP4 NP3 NP2 NP1 NP0 	
//	            └───┴───┴───┴───┴── Noise Frequency
#define PSG_REG_NOISE		6

//-----------------------------------------------------------------------------
// R#7    Mixer Control-I/O Enable
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	IB	IA	NC	NB	NA	TC	TB	TA 	
//  │	│	│	│	│	│	│	└── Tone Genrator A enable/disable
//	│	│	│	│	│   │   └────── Tone Genrator B enable/disable
//	│	│	│	│	│   └────────── Tone Genrator C enable/disable
//	│	│	│	│	└────────────── Noise Generator A enable/disable
//	│	│	│	└────────────────── Noise Generator B enable/disable
//	│	│	└────────────────────── Noise Generator C enable/disable
//	│	└────────────────────────── Port A input/output control
//	└────────────────────────────── Port B input/output control
#define PSG_REG_MIXER		7
#define PSG_F_TA			(0x01)	///< Tone Genrator A enable/disable
#define PSG_F_TB			(0x02)	///< Tone Genrator B enable/disable
#define PSG_F_TC			(0x04)	///< Tone Genrator C enable/disable
#define PSG_F_NA			(0x08)	///< Noise Generator A enable/disable
#define PSG_F_NB			(0x10)	///< Noise Generator B enable/disable
#define PSG_F_NC			(0x20)	///< Noise Generator C enable/disable
#define PSG_F_IA			(0x40)	///< Port A input/output control
#define PSG_F_IB			(0x80)	///< Port B input/output control

//-----------------------------------------------------------------------------
// R#8    Amplitude Control - Channel A
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	x	x	x	M	L3	L2	L1	L0 	
//  			│	└───┴───┴───┴── Channel A Amplitude (volume)
//				└────────────────── Volume controlled by Envelope enable/disable
#define PSG_REG_AMP_A		8
#define PSG_F_ENV			(0x10)	///< Volume controlled by Envelope enable/disable

//-----------------------------------------------------------------------------
// R#9    Amplitude Control - Channel B
//-----------------------------------------------------------------------------
#define PSG_REG_AMP_B		9

//-----------------------------------------------------------------------------
// R#10   Amplitude Control - Channel C
//-----------------------------------------------------------------------------
#define PSG_REG_AMP_C		10

//-----------------------------------------------------------------------------
// R#11   Envelope Generator Control - Fine Tune Register
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	EP7 EP6 EP5 EP4 EP3 EP2 EP1 EP0 	
//	└───┴───┴───┴───┴───┴───┴───┴── Envelope Frequency, LSB (period fine tune)
#define PSG_REG_ENV			11
#define PSG_REG_ENVF		11

//-----------------------------------------------------------------------------
// R#12   Envelope Generator Control - Coarse Tune Register
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	EPF EPE EPD EPC EPB EPA EP9 EP8 	
//	└───┴───┴───┴───┴───┴───┴───┴── Envelope Frequency, MSB (period coarse tune)
#define PSG_REG_ENVC		12

//-----------------------------------------------------------------------------
// R#13   Envelope Shape/Cycle Control
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	x	x	x	x	E3	E2	E1	E0 	
//  				│	│	│	└── Envelope shape, Hold
//					│   │   └────── Envelope shape, Alternate
//					│   └────────── Envelope shape, Attack
//					└────────────── Envelope shape, Continue
#define PSG_REG_SHAPE		13
#define PSG_F_HLD			(0x01)	///< Envelope shape, Hold
#define PSG_F_ALT			(0x02)	///< Envelope shape, Alternate
#define PSG_F_ATT			(0x04)	///< Envelope shape, Attack
#define PSG_F_CNT			(0x08)	///< Envelope shape, Continue

// E3 E2 E1 E0	 Modulation Envelope
//----------------------------------------------------
//             _                     _
// 0  0  x  x  _ \__________________ _
//             _                     _
// 0  1  x  x  _ /|_________________ _
//             _                     _
// 1  0  0  0  _ \|\|\|\|\|\|\|\|\|\ _
//             _                     _
// 1  0  0  1  _ \__________________ _
//             _                     _
// 1  0  1  0  _ \/\/\/\/\/\/\/\/\/\ _
//             _   _________________ _
// 1  0  1  1  _ \|                  _
//             _                     _
// 1  1  0  0  _ /|/|/|/|/|/|/|/|/|/ _
//             _  __________________ _
// 1  1  0  1  _ /                   _
//             _                     _
// 1  1  1  0  _ /\/\/\/\/\/\/\/\/\/ _
//             _                     _
// 1  1  1  1  _ /|_________________ _


