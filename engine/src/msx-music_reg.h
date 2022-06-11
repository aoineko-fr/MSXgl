// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄  ▄▄▄ ▄▄ ▄     ▄ ▄           ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▀█▄  ▀█▄▀ ▄▄ ██▀█ ██ █  ██▀ ▄  ▄█▀▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▄▄█▀ ██ █    ██ █ ▀█▄█ ▄██  ██ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// MSX-Music (YM2413) handler module
//
// References:
//  - YM2413 FM Operator Type-LL (OPLL) Application Manual
//     https://www.smspower.org/maxim/Documents/YM2413ApplicationManual
//  - MSX-MUSIC programming
//     https://www.msx.org/wiki/MSX-MUSIC_programming
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// DEFINES
//=============================================================================

//-----------------------------------------------------------------------------
// R#00    User Tone - Modulator - Multiplication control
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	AM	VIB	EG	KSR	MUL	MUL	MUL	MUL
//	│	│	│	│	└───┴───┴───┴── The frequencies of the carrier wave and the modulated wave (0-15)
//	│	│	│	│					0=½, 1=1, 2=2, 3=3, 4=4, 5=5, 6=6, 7=7, 8=8, 9=9, A=10, B=10, C=12, D=12, E=15, F=15
//	│	│	│	└────────────────── This bit specifies the key scale of RATE
//  │	│	└────────────────────── Switching between sustained tone and Percussive tone.
//  │	└────────────────────────── Vibrato on/off switch. When this bit is '1', vibrato will be applied to the slot.
//  └────────────────────────────── Amplitude modulation on/off switch. When this bit is '1', amplitude modulation wil be epplied to the slot. 
#define MSXMUSIC_REG_USER_0			0x00

//-----------------------------------------------------------------------------
// R#01    User Tone - Carrier - Multiplication control
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	AM	VIB	EG	KSR	MUL	MUL	MUL	MUL
//	│	│	│	│	└───┴───┴───┴── The frequencies of the carrier wave and the modulated wave (0-15)
//	│	│	│	│					0=½, 1=1, 2=2, 3=3, 4=4, 5=5, 6=6, 7=7, 8=8, 9=9, A=10, B=10, C=12, D=12, E=15, F=15
//	│	│	│	└────────────────── This bit specifies the key scale of RATE
//  │	│	└────────────────────── Switching between sustained tone and Percussive tone.
//  │	└────────────────────────── Vibrato on/off switch. When this bit is '1', vibrato will be applied to the slot.
//  └────────────────────────────── Amplitude modulation on/off switch. When this bit is '1', amplitude modulation wil be epplied to the slot. 
#define MSXMUSIC_REG_USER_1			0x01

//-----------------------------------------------------------------------------
// R#02    User Tone - Modulator - Attenuation control
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	KSL	KSL	TL	TL	TL	TL	TL	TL
//	│	│	└───┴───┴───┴───┴───┴── The minimum resolution of the attenuation level is 0.75 dB and the sound level can be reduced down to 47.25 dB.
//  └───┴────────────────────────── This bit controls the level of key scaling. In key scale mode, the level of attenuation increases as pitch rises, to 1.5 dB/oct, 3 dB/oct, 6 dB/oct, and 0 dB/oct.
#define MSXMUSIC_REG_USER_2			0x02

//-----------------------------------------------------------------------------
// R#03    User Tone - Carrier - Attenuation control
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	KSL	KSL		DC	DM	FB	FB	FB
//	│	│		│	│	└───┴───┴── For the modulated wave of the first slot's feedback FM modulation.
//  │	│		│	└────────────── The modulated wave is rectified to half wave.
//  │	│		└────────────────── The carrier wave is rectified to half wave.
//  └───┴────────────────────────── This bit controls the level of key scaling. In key scale mode, the level of attenuation increases as pitch rises, to 1.5 dB/oct, 3 dB/oct, 6 dB/oct, and 0 dB/oct.
#define MSXMUSIC_REG_USER_3			0x03

//-----------------------------------------------------------------------------
// R#04    User Tone - Modulator - Rate control
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	AR	AR	AR	AR	DR	DR	DR	DR
//	│	│	│	│	└───┴───┴───┴── Decay Rate defines the decay time
//  └───┴───┴───┴────────────────── Attack Rate defines the attack time of the sounds
#define MSXMUSIC_REG_USER_4			0x04

//-----------------------------------------------------------------------------
// R#05    User Tone - Carrier - Rate control
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	AR	AR	AR	AR	DR	DR	DR	DR
//	│	│	│	│	└───┴───┴───┴── Decay Rate defines the decay time
//  └───┴───┴───┴────────────────── Attack Rate defines the attack time of the sounds
#define MSXMUSIC_REG_USER_5			0x05

//-----------------------------------------------------------------------------
// R#06    User Tone - Modulator - Sustain control
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	SL	SL	SL	SL	RR	RR	RR	RR
//	│	│	│	│	└───┴───┴───┴── Sustain Level is the critical point that, when sustaining sound has been attenuated to the prescribed level, the level is maintained thereafter. With percussive tone, it is the critical point of change from decay mode to release mode.
//  └───┴───┴───┴────────────────── Release Rate is the disappearing rate of sustaining sounds after key OFF. For percussive tone, attenuation above the sustain level is expressed with decay rate and attenuation below the sustain level with release rate.
#define MSXMUSIC_REG_USER_6			0x06

//-----------------------------------------------------------------------------
// R#07    User Tone - Carrier - Sustain  control
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	SL	SL	SL	SL	RR	RR	RR	RR
//	│	│	│	│	└───┴───┴───┴── Sustain Level is the critical point that, when sustaining sound has been attenuated to the prescribed level, the level is maintained thereafter. With percussive tone, it is the critical point of change from decay mode to release mode.
//  └───┴───┴───┴────────────────── Release Rate is the disappearing rate of sustaining sounds after key OFF. For percussive tone, attenuation above the sustain level is expressed with decay rate and attenuation below the sustain level with release rate.
#define MSXMUSIC_REG_USER_7			0x07


//-----------------------------------------------------------------------------
// R#0E    Rhythm Control
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//			RHY	BD	SD	TOM	TCY	HH
//			│	│	│	│	│	└── High Hat instrument ON/OFF
//			│	│	│	│	└────── Top Cymbal instrument ON/OFF
//  		│	│	│	└────────── Tom-tom instrument ON/OFF
//  		│	│	└────────────── Snare Drum instrument ON/OFF
//  		│	└────────────────── Bass Drum instrument ON/OFF
//			└────────────────────── When 1, OPLL is in Rhythm mode with percussion sounds produced through channels 7~9 (see page 5). In this mode, the melody section is limited to six sounds.
#define MSXMUSIC_REG_RHYTHM			0x0E


//-----------------------------------------------------------------------------
// R#0F    OPLL Test Data
//-----------------------------------------------------------------------------
#define MSXMUSIC_REG_TEST			0x0F


//-----------------------------------------------------------------------------
// R#1x    F-Number LSB 8 bits
//-----------------------------------------------------------------------------
#define MSXMUSIC_REG_FREQ_1			0x10
#define MSXMUSIC_REG_FREQ_2			0x11
#define MSXMUSIC_REG_FREQ_3			0x12
#define MSXMUSIC_REG_FREQ_4			0x13
#define MSXMUSIC_REG_FREQ_5			0x14
#define MSXMUSIC_REG_FREQ_6			0x15
#define MSXMUSIC_REG_FREQ_7			0x16
#define MSXMUSIC_REG_FREQ_8			0x17
#define MSXMUSIC_REG_FREQ_9			0x18

//-----------------------------------------------------------------------------
// R#2x    F-Number MSB, octave set. Key ON/OFF register. Sustain ON/OFF register
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//			SUS	KEY	BL	BL	BL	F-N
//			│	│	│	│	│	└── MSB 1 bit of F-Number
//			│	│	└───┴───┴────── Defines octave information.
//  		│	└────────────────── This bit indicates the ON/OFF state of the key. When this bit is '1', the associated channel is on and sound is produced. '0' is equivalent to Key-OFF.
//			└────────────────────── When this bit is '1', the RR with the key off will be 5.
#define MSXMUSIC_REG_CTRL_1			0x20
#define MSXMUSIC_REG_CTRL_2			0x21
#define MSXMUSIC_REG_CTRL_3			0x22
#define MSXMUSIC_REG_CTRL_4			0x23
#define MSXMUSIC_REG_CTRL_5			0x24
#define MSXMUSIC_REG_CTRL_6			0x25
#define MSXMUSIC_REG_CTRL_7			0x26
#define MSXMUSIC_REG_CTRL_8			0x27
#define MSXMUSIC_REG_CTRL_9			0x28

//-----------------------------------------------------------------------------
// R#3x    Instruments Selection and Volume Register
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	INS	INS	INS	INS	VOL	VOL	VOL	VOL
//	│	│	│	│	└───┴───┴───┴── Determine the volume for the voices. The minimum resolution is 3 dB and the maximum 45 dB.
//  └───┴───┴───┴────────────────── These four bits determine voices as follows:
//										0=User tone		4=Flute			8=Organ				12=Vibraphone
//										1=Violin        5=Clarinet      9=Horn              13=Synthesizer Bass
//										2=Guitar        6=Oboe          10=Synthesizer      14=Acoustic Bass
//										3=Piano         7=Trumpet       11=Harpsichord      15=Electric Guitar
#define MSXMUSIC_REG_INST_1			0x30
#define MSXMUSIC_REG_INST_2			0x31
#define MSXMUSIC_REG_INST_3			0x32
#define MSXMUSIC_REG_INST_4			0x33
#define MSXMUSIC_REG_INST_5			0x34
#define MSXMUSIC_REG_INST_6			0x35
// If (0x0E.5 == 0)
#define MSXMUSIC_REG_INST_7			0x36
#define MSXMUSIC_REG_INST_8			0x37
#define MSXMUSIC_REG_INST_9			0x38

// If (0x0E.5 == 1)
//-----------------------------------------------------------------------------
// R#36    Rhythm volume - Bass Drum
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//					BD	BD	BD	BD	
//					└───┴───┴───┴── Bass Drum volume
#define MSXMUSIC_REG_RHYTHM_VOL_0	0x36

//-----------------------------------------------------------------------------
// R#37    Rhythm volume - High Hat & Snare Drum
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	HH	HH	HH	HH	SD	SD	SD	SD
//	│	│	│	│	└───┴───┴───┴── Snare Drum volume
//  └───┴───┴───┴────────────────── High Hat volume
#define MSXMUSIC_REG_RHYTHM_VOL_1	0x37

//-----------------------------------------------------------------------------
// R#38    Rhythm volume - Tom-tom & Top Cymbal
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	TOM	TOM	TOM	TOM	TCY	TCY	TCY	TCY
//	│	│	│	│	└───┴───┴───┴── Top Cymbal
//  └───┴───┴───┴────────────────── Tom-tom volume
#define MSXMUSIC_REG_RHYTHM_VOL_2	0x38





































