// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄  ▄▄▄ ▄▄ ▄     ▄▄         ▄▄ ▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▀█▄  ▀█▄▀ ▄▄ ██▄█ ██ █  ▄██ ▄  ▄█▀▄
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▄▄█▀ ██ █    ██ █ ▀█▄█ ▀▄██ ██ ▀█▄▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// MSX-Audio (Y8950) handler module
//
// References:
//  - 
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// DEFINES
//=============================================================================

//-----------------------------------------------------------------------------
// S#01    Status
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	INT	T1	T2	EOS	BR			PCM
//	│	│	│	│	│			└── PCM busy
//  │	│	│	│	└────────────── Buffer ready
//	│	│	│	└────────────────── 
//  │	│	└────────────────────── 
//  │	└────────────────────────── 
//  └────────────────────────────── 


//-----------------------------------------------------------------------------
// R#01    Test
//-----------------------------------------------------------------------------
#define MSXAUDIO_REG_TEST			0x01

//-----------------------------------------------------------------------------
// R#02    Timers
//-----------------------------------------------------------------------------
#define MSXAUDIO_REG_TIMER_1		0x02
#define MSXAUDIO_REG_TIMER_2		0x03

//-----------------------------------------------------------------------------
// R#04    Flag control
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	IRQ	MT1	MT2	EOS	MBR		ST2	ST1
//	│	│	│	│	│		│	└── Controls the start/stop operations of timer-1
//	│	│	│	│	│		└────── Controls the start/stop operations of timer-2
//  │	│	│	│	└────────────── When this bit is set to "1", data write/read request are masked during data transfert between the processor and ADPCM or external storage
//	│	│	│	└────────────────── Mask the flag indicating the end of read/write of ADPCM or external storage, or the end of AD conversion
//  │	│	└────────────────────── Mask timer-2 flag
//  │	└────────────────────────── Mask timer-1 flag
//  └────────────────────────────── Reset the IRQ status
#define MSXAUDIO_REG_FLAG_CTRL		0x04

//-----------------------------------------------------------------------------
// R#05    Keyboard in/out
//-----------------------------------------------------------------------------
#define MSXAUDIO_KEYBOARD_IN		0x05
#define MSXAUDIO_KEYBOARD_OUT		0x06

//-----------------------------------------------------------------------------
// R#07    Start of ADPCM voice analysis/synthesis and setting of external memory access
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	│	│	│	│	│			└── RESET
//  │	│	│	│	└────────────── SP-OFF
//	│	│	│	└────────────────── REPEAT
//  │	│	└────────────────────── MEMORY DATA
//  │	└────────────────────────── REC
//  └────────────────────────────── START
#define MSXAUDIO_REG_ADPCM_CTRL		0x07

//-----------------------------------------------------------------------------
// R#08    Composite sinusoidal modeling mode, AD/DA conversion, and type of external memory
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	│	│			│	│	│	└── ROM
//  │	│			│	│	└────── 64K
//	│	│			│	└────────── DA/AD
//  │	│			└────────────── SAMPL
//  │	└────────────────────────── NOTE SEL
//  └────────────────────────────── CSM
#define MSXAUDIO_REG_MODE_CTRL		0x08

//-----------------------------------------------------------------------------
// R#09    Start address
//-----------------------------------------------------------------------------
#define MSXAUDIO_REG_START_L		0x09
#define MSXAUDIO_REG_START_H		0x0A

//-----------------------------------------------------------------------------
// R#0B    Stop address
//-----------------------------------------------------------------------------
#define MSXAUDIO_REG_STOP_L			0x0B
#define MSXAUDIO_REG_STOP_H			0x0C

//-----------------------------------------------------------------------------
// R#0D    Sample rate for AD/DA conversion
//-----------------------------------------------------------------------------
#define MSXAUDIO_REG_PRESCALE_L		0x0D
#define MSXAUDIO_REG_PRESCALE_H		0x0E // 3-bits

//-----------------------------------------------------------------------------
// R#0F    ADPCM data address
//-----------------------------------------------------------------------------
#define MSXAUDIO_REG_ADPCM_DATA		0x0F

//-----------------------------------------------------------------------------
// R#10    Factor used for linear interpolation
//-----------------------------------------------------------------------------
#define MSXAUDIO_REG_DELTA_L		0x10
#define MSXAUDIO_REG_DELTA_H		0x11

//-----------------------------------------------------------------------------
// R#12    Envelope control
//-----------------------------------------------------------------------------
#define MSXAUDIO_REG_ENV_CTRL		0x12

//-----------------------------------------------------------------------------
// R#15    Digital data for DA conversion
//-----------------------------------------------------------------------------
#define MSXAUDIO_REG_DAC_DATA_L		0x15
#define MSXAUDIO_REG_DAC_DATA_H		0x16
#define MSXAUDIO_REG_DAC_DATA_SHIFT	0x17

//-----------------------------------------------------------------------------
// R#18    I/O control & data
//-----------------------------------------------------------------------------
#define MSXAUDIO_REG_IO_CTRL		0x18
#define MSXAUDIO_REG_IO_DATA		0x19

//-----------------------------------------------------------------------------
// R#1A    PCM data
//-----------------------------------------------------------------------------
#define MSXAUDIO_REG_PCM_DATA		0x1A

//-----------------------------------------------------------------------------
// R#20    User Tone
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	AM	VIB	EG	KSR	MUL	MUL	MUL	MUL
//	│	│	│	│	└───┴───┴───┴── The frequencies of the carrier wave and the modulated wave (0-15)
//	│	│	│	│					0=½, 1=1, 2=2, 3=3, 4=4, 5=5, 6=6, 7=7, 8=8, 9=9, A=10, B=10, C=12, D=12, E=15, F=15
//	│	│	│	└────────────────── This bit specifies the key scale of RATE
//  │	│	└────────────────────── Switching between sustained tone and Percussive tone.
//  │	└────────────────────────── Vibrato on/off switch. When this bit is '1', vibrato will be applied to the slot.
//  └────────────────────────────── Amplitude modulation on/off switch. When this bit is '1', amplitude modulation wil be epplied to the slot. 
#define MSXAUDIO_REG_MUL_1_MOD		0x20 // Modulated wave
#define MSXAUDIO_REG_MUL_2_MOD		0x21 // Carrier wave
#define MSXAUDIO_REG_MUL_3_MOD		0x22
#define MSXAUDIO_REG_MUL_1_CAR		0x23
#define MSXAUDIO_REG_MUL_2_CAR		0x24
#define MSXAUDIO_REG_MUL_3_CAR		0x25
#define MSXAUDIO_REG_MUL_4_MOD		0x28
#define MSXAUDIO_REG_MUL_5_MOD		0x29
#define MSXAUDIO_REG_MUL_6_MOD		0x2A
#define MSXAUDIO_REG_MUL_4_CAR		0x2B
#define MSXAUDIO_REG_MUL_5_CAR		0x2C
#define MSXAUDIO_REG_MUL_6_CAR		0x2D
#define MSXAUDIO_REG_MUL_7_MOD		0x30
#define MSXAUDIO_REG_MUL_8_MOD		0x31
#define MSXAUDIO_REG_MUL_9_MOD		0x32
#define MSXAUDIO_REG_MUL_7_CAR		0x33
#define MSXAUDIO_REG_MUL_8_CAR		0x34
#define MSXAUDIO_REG_MUL_9_CAR		0x35

//-----------------------------------------------------------------------------
// R#40    KLS/total level
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	KSL	KSL	LVL	LVL	LVL	LVL	LVL	LVL
//	│	│	└───┴───┴───┴───┴───┴── Total level
//  └───┴────────────────────────── KSL
#define MSXAUDIO_REG_LEVEL_1_MOD	0x40
#define MSXAUDIO_REG_LEVEL_2_MOD	0x41
#define MSXAUDIO_REG_LEVEL_3_MOD	0x42
#define MSXAUDIO_REG_LEVEL_1_CAR	0x43
#define MSXAUDIO_REG_LEVEL_2_CAR	0x44
#define MSXAUDIO_REG_LEVEL_3_CAR	0x45
#define MSXAUDIO_REG_LEVEL_4_MOD	0x48
#define MSXAUDIO_REG_LEVEL_5_MOD	0x49
#define MSXAUDIO_REG_LEVEL_6_MOD	0x4A
#define MSXAUDIO_REG_LEVEL_4_CAR	0x4B
#define MSXAUDIO_REG_LEVEL_5_CAR	0x4C
#define MSXAUDIO_REG_LEVEL_6_CAR	0x4D
#define MSXAUDIO_REG_LEVEL_7_MOD	0x50
#define MSXAUDIO_REG_LEVEL_8_MOD	0x51
#define MSXAUDIO_REG_LEVEL_9_MOD	0x52
#define MSXAUDIO_REG_LEVEL_7_CAR	0x53
#define MSXAUDIO_REG_LEVEL_8_CAR	0x54
#define MSXAUDIO_REG_LEVEL_9_CAR	0x55

//-----------------------------------------------------------------------------
// R#60    Attack/decay rate
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	AR	AR	AR	AR	DR	DR	DR	DR
//	│	│	│	│	└───┴───┴───┴── Decay Rate defines the decay time
//  └───┴───┴───┴────────────────── Attack Rate defines the attack time of the sounds
#define MSXAUDIO_REG_ATT_1_MOD		0x60
#define MSXAUDIO_REG_ATT_2_MOD		0x61
#define MSXAUDIO_REG_ATT_3_MOD		0x62
#define MSXAUDIO_REG_ATT_1_CAR		0x63
#define MSXAUDIO_REG_ATT_2_CAR		0x64
#define MSXAUDIO_REG_ATT_3_CAR		0x65
#define MSXAUDIO_REG_ATT_4_MOD		0x68
#define MSXAUDIO_REG_ATT_5_MOD		0x69
#define MSXAUDIO_REG_ATT_6_MOD		0x6A
#define MSXAUDIO_REG_ATT_4_CAR		0x6B
#define MSXAUDIO_REG_ATT_5_CAR		0x6C
#define MSXAUDIO_REG_ATT_6_CAR		0x6D
#define MSXAUDIO_REG_ATT_7_MOD		0x70
#define MSXAUDIO_REG_ATT_8_MOD		0x71
#define MSXAUDIO_REG_ATT_9_MOD		0x72
#define MSXAUDIO_REG_ATT_7_CAR		0x73
#define MSXAUDIO_REG_ATT_8_CAR		0x74
#define MSXAUDIO_REG_ATT_9_CAR		0x75

//-----------------------------------------------------------------------------
// R#80    
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	SL	SL	SL	SL	RR	RR	RR	RR
//	│	│	│	│	└───┴───┴───┴── Sustain Level is the critical point that, when sustaining sound has been attenuated to the prescribed level, the level is maintained thereafter. With percussive tone, it is the critical point of change from decay mode to release mode.
//  └───┴───┴───┴────────────────── Release Rate is the disappearing rate of sustaining sounds after key OFF. For percussive tone, attenuation above the sustain level is expressed with decay rate and attenuation below the sustain level with release rate.
#define MSXAUDIO_REG_SUS_1_MOD		0x80
#define MSXAUDIO_REG_SUS_2_MOD		0x81
#define MSXAUDIO_REG_SUS_3_MOD		0x82
#define MSXAUDIO_REG_SUS_1_CAR		0x83
#define MSXAUDIO_REG_SUS_2_CAR		0x84
#define MSXAUDIO_REG_SUS_3_CAR		0x85
#define MSXAUDIO_REG_SUS_4_MOD		0x88
#define MSXAUDIO_REG_SUS_5_MOD		0x89
#define MSXAUDIO_REG_SUS_6_MOD		0x8A
#define MSXAUDIO_REG_SUS_4_CAR		0x8B
#define MSXAUDIO_REG_SUS_5_CAR		0x8C
#define MSXAUDIO_REG_SUS_6_CAR		0x8D
#define MSXAUDIO_REG_SUS_7_MOD		0x90
#define MSXAUDIO_REG_SUS_8_MOD		0x91
#define MSXAUDIO_REG_SUS_9_MOD		0x92
#define MSXAUDIO_REG_SUS_7_CAR		0x93
#define MSXAUDIO_REG_SUS_8_CAR		0x94
#define MSXAUDIO_REG_SUS_9_CAR		0x95

//-----------------------------------------------------------------------------
// R#A0    F-Number LSB 8 bits
//-----------------------------------------------------------------------------
#define MSXAUDIO_REG_FREQ_1			0xA0
#define MSXAUDIO_REG_FREQ_2			0xA1
#define MSXAUDIO_REG_FREQ_3			0xA2
#define MSXAUDIO_REG_FREQ_4			0xA3
#define MSXAUDIO_REG_FREQ_5			0xA4
#define MSXAUDIO_REG_FREQ_6			0xA5
#define MSXAUDIO_REG_FREQ_7			0xA6
#define MSXAUDIO_REG_FREQ_8			0xA7
#define MSXAUDIO_REG_FREQ_9			0xA8

//-----------------------------------------------------------------------------
// R#B0    F-Number MSB, octave set. Key ON/OFF register.
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//			KEY	BL	BL	BL	F-N	F-N
//			│	│	│	│	└───┴── MSB 1 bit of F-Number
//			│	└───┴───┴────────── Defines octave information.
//  		└────────────────────── This bit indicates the ON/OFF state of the key. When this bit is '1', the associated channel is on and sound is produced. '0' is equivalent to Key-OFF.
#define MSXAUDIO_REG_CTRL_1			0xB0
#define MSXAUDIO_REG_CTRL_2			0xB1
#define MSXAUDIO_REG_CTRL_3			0xB2
#define MSXAUDIO_REG_CTRL_4			0xB3
#define MSXAUDIO_REG_CTRL_5			0xB4
#define MSXAUDIO_REG_CTRL_6			0xB5
#define MSXAUDIO_REG_CTRL_7			0xB6
#define MSXAUDIO_REG_CTRL_8			0xB7
#define MSXAUDIO_REG_CTRL_9			0xB8

//-----------------------------------------------------------------------------
// R#BD    Rhythm Control
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	AD	VD	RHY	BD	SD	TOM	TCY	HH
//	│	│	│	│	│	│	│	└── High Hat instrument ON/OFF
//	│	│	│	│	│	│	└────── Top Cymbal instrument ON/OFF
//  │	│	│	│	│	└────────── Tom-tom instrument ON/OFF
//  │	│	│	│	└────────────── Snare Drum instrument ON/OFF
//  │	│	│	└────────────────── Bass Drum instrument ON/OFF
//	│	│	└────────────────────── When 1, MSX-AUDIO is in Rhythm mode with percussion sounds produced through channels 7~9
//	│	└────────────────────────── Select one of the 2 vibrato depths
//	└────────────────────────────── Select one of the 2 amplitude-modulation depths
#define MSXAUDIO_REG_RHYTHM			0xBD

//-----------------------------------------------------------------------------
// R#C0    Feedback/connection
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//					FB	FB	FB	C
//					│	│	│	└── Connection
//					└───┴───┴────── Feedback
#define MSXAUDIO_REG_FEED_1			0xC0
#define MSXAUDIO_REG_FEED_2			0xC1
#define MSXAUDIO_REG_FEED_3			0xC2
#define MSXAUDIO_REG_FEED_4			0xC3
#define MSXAUDIO_REG_FEED_5			0xC4
#define MSXAUDIO_REG_FEED_6			0xC5
#define MSXAUDIO_REG_FEED_7			0xC6
#define MSXAUDIO_REG_FEED_8			0xC7
#define MSXAUDIO_REG_FEED_9			0xC8
