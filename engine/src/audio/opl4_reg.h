// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄  ▄▄▄  ▄▄   ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██▄▀ ██   █▄██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ██   ██▄▄   ██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// OPL4 (YMF278B) registers definition
//
// References:
//  - OPL4 YMF278B Application Manual
//     https://map.grauw.nl/resources/sound/yamaha_opl4.pdf
//─────────────────────────────────────────────────────────────────────────────
#pragma once


//=============================================================================
// DEFINES
//=============================================================================

//.............................................................................
// Group: Status

//-----------------------------------------------------------------------------
// S#0    Status (read on FM part 0 address port)
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	IRQ	FT1	FT2	x	x	x	LD	BSY
//	│	│	│				│	└── Busy (wait before next register write)
//	│	│	│				└────── Wave header loading (wait before next access to the same wave channel)
//	│	│	└────────────────────── Timer 2 overflow flag
//	│	└────────────────────────── Timer 1 overflow flag
//	└────────────────────────────── IRQ (set when an unmasked timer flag is set)
// Header loading takes about 300 µs after writing a wave number (W#8-31)
#define OPL4_STATUS_BUSY			0x01	// Busy
#define OPL4_STATUS_LD				0x02	// Wave header loading
#define OPL4_STATUS_FT2				0x20	// Timer 2 overflow flag
#define OPL4_STATUS_FT1				0x40	// Timer 1 overflow flag
#define OPL4_STATUS_IRQ				0x80	// IRQ

//-----------------------------------------------------------------------------
// FM Part 0
//-----------------------------------------------------------------------------
// Registers 0-255 are accessed through the part 0 address/data ports
// Contains timers, rhythm, FM global control and FM channels 0-8
#define OPL4_PART_0					0

//.............................................................................
// Group: FM System

//-----------------------------------------------------------------------------
// R#1    Test
//-----------------------------------------------------------------------------
// Always 00h
#define OPL4_REG_TEST				0x01	// R#1

//-----------------------------------------------------------------------------
// R#2    Timer 1
//-----------------------------------------------------------------------------
// Period = (256 - N) × Timer 1 step (see datasheet)
//	7	6	5	4	3	2	1	0
//	T7	T6	T5	T4	T3	T2	T1	T0
//	└───┴───┴───┴───┴───┴───┴───┴── Timer 1 preset value
#define OPL4_REG_TIMER_1			0x02	// R#2

//-----------------------------------------------------------------------------
// R#3    Timer 2
//-----------------------------------------------------------------------------
// Period = (256 - N) × Timer 2 step (4 times Timer 1 step)
//	7	6	5	4	3	2	1	0
//	T7	T6	T5	T4	T3	T2	T1	T0
//	└───┴───┴───┴───┴───┴───┴───┴── Timer 2 preset value
#define OPL4_REG_TIMER_2			0x03	// R#3

//-----------------------------------------------------------------------------
// R#4    Timer Control - IRQ Reset
//-----------------------------------------------------------------------------
// When RST = 1, other bits are ignored
//	7	6	5	4	3	2	1	0
//	RST	MT1	MT2	x	x	x	ST2	ST1
//	│	│	│				│	└── Timer 1 start (1: run, 0: stop)
//	│	│	│				└────── Timer 2 start (1: run, 0: stop)
//	│	│	└────────────────────── Mask Timer 2 flag
//	│	└────────────────────────── Mask Timer 1 flag
//	└────────────────────────────── Reset IRQ and timer flags
#define OPL4_REG_TIMER_CTRL			0x04	// R#4
#define OPL4_F_ST1					0x01	// Timer 1 start
#define OPL4_F_ST2					0x02	// Timer 2 start
#define OPL4_F_MT2					0x20	// Mask Timer 2 flag
#define OPL4_F_MT1					0x40	// Mask Timer 1 flag
#define OPL4_F_IRQ_RESET			0x80	// Reset IRQ and timer flags

//-----------------------------------------------------------------------------
// R#8    Keyboard Split
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x	NTS	x	x	x	x	x	x
//		└────────────────────────── Note select (keyboard split point for key scaling: 0: F-Number bit 9, 1: bit 8)
#define OPL4_REG_NTS				0x08	// R#8
#define OPL4_F_NTS					0x40	// Note select

//.............................................................................
// Group: FM Operator

//-----------------------------------------------------------------------------
// FM operator registers (R#32-245)
//-----------------------------------------------------------------------------
// Register = base + operator offset
// Each part has 18 operators (offsets 00h-05h, 08h-0Dh, 10h-15h)
// Channels 9-17 use the same registers in part 1 (see OPL4_PART_1)
//
// Channel	Operator 1	Operator 2
// 0		00h			03h
// 1		01h			04h
// 2		02h			05h
// 3		08h			0Bh
// 4		09h			0Ch
// 5		0Ah			0Dh
// 6		10h			13h
// 7		11h			14h
// 8		12h			15h
//
// In 4-operator mode (see R#260), channels 0+3, 1+4 and 2+5 are combined (operators 1-2 then 3-4)
#define OPL4_OP_0_1					0x00	// Channel 0, operator 1
#define OPL4_OP_0_2					0x03	// Channel 0, operator 2
#define OPL4_OP_1_1					0x01	// Channel 1, operator 1
#define OPL4_OP_1_2					0x04	// Channel 1, operator 2
#define OPL4_OP_2_1					0x02	// Channel 2, operator 1
#define OPL4_OP_2_2					0x05	// Channel 2, operator 2
#define OPL4_OP_3_1					0x08	// Channel 3, operator 1
#define OPL4_OP_3_2					0x0B	// Channel 3, operator 2
#define OPL4_OP_4_1					0x09	// Channel 4, operator 1
#define OPL4_OP_4_2					0x0C	// Channel 4, operator 2
#define OPL4_OP_5_1					0x0A	// Channel 5, operator 1
#define OPL4_OP_5_2					0x0D	// Channel 5, operator 2
#define OPL4_OP_6_1					0x10	// Channel 6, operator 1
#define OPL4_OP_6_2					0x13	// Channel 6, operator 2
#define OPL4_OP_7_1					0x11	// Channel 7, operator 1
#define OPL4_OP_7_2					0x14	// Channel 7, operator 2
#define OPL4_OP_8_1					0x12	// Channel 8, operator 1
#define OPL4_OP_8_2					0x15	// Channel 8, operator 2

//-----------------------------------------------------------------------------
// R#32-53 FM Operator - AM/Vibrato/EG Type/Key Scale Rate/Multiple
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	AM	VIB	EGT	KSR	ML3	ML2	ML1	ML0
//	│	│	│	│	└───┴───┴───┴── Multiple (0: ×½, 1-10: ×1-×10, 11: ×10, 12-13: ×12, 14-15: ×15)
//	│	│	│	└────────────────── Key scale rate (envelope speed follows the note)
//	│	│	└────────────────────── EG type (1: sustained, 0: percussive)
//	│	└────────────────────────── Vibrato enable
//	└────────────────────────────── Amplitude modulation (tremolo) enable
#define OPL4_REG_AVEKM				0x20	// R#32
#define OPL4_F_KSR					0x10	// Key scale rate
#define OPL4_F_EGT					0x20	// EG type (sustained)
#define OPL4_F_VIB					0x40	// Vibrato enable
#define OPL4_F_AM					0x80	// Amplitude modulation enable

//-----------------------------------------------------------------------------
// R#64-85 FM Operator - Key Scale Level/Total Level
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	KSL1 KSL0 TL5 TL4 TL3 TL2 TL1 TL0
//	│	│	└───┴───┴───┴───┴───┴── Total level (0-63, 0.75 dB/step, 0 = loudest)
//	└───┴────────────────────────── Key scale level (0: 0, 1: 3, 2: 1.5, 3: 6 dB/octave)
#define OPL4_REG_KSLTL				0x40	// R#64

//-----------------------------------------------------------------------------
// R#96-117 FM Operator - Attack Rate/Decay Rate
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	AR3	AR2	AR1	AR0	DR3	DR2	DR1	DR0
//	│	│	│	│	└───┴───┴───┴── Decay rate (0-15)
//	└───┴───┴───┴────────────────── Attack rate (0-15, 15 = instant)
#define OPL4_REG_ARDR				0x60	// R#96

//-----------------------------------------------------------------------------
// R#128-149 FM Operator - Sustain Level/Release Rate
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	SL3	SL2	SL1	SL0	RR3	RR2	RR1	RR0
//	│	│	│	│	└───┴───┴───┴── Release rate (0-15)
//	└───┴───┴───┴────────────────── Sustain level (0-15, 3 dB/step, 15 = -93 dB)
#define OPL4_REG_SLRR				0x80	// R#128

//-----------------------------------------------------------------------------
// R#224-245 FM Operator - Wave Select
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x	x	x	x	x	WS2	WS1	WS0
//						└───┴───┴── Waveform (0-7)
#define OPL4_REG_WS					0xE0	// R#224
#define OPL4_WS_SINE				0x00	// Sine
#define OPL4_WS_HALF_SINE			0x01	// Half sine
#define OPL4_WS_ABS_SINE			0x02	// Absolute sine
#define OPL4_WS_QUARTER_SINE		0x03	// Quarter sine (pulse sine)
#define OPL4_WS_ALT_SINE			0x04	// Sine, even periods only
#define OPL4_WS_ALT_ABS_SINE		0x05	// Absolute sine, even periods only
#define OPL4_WS_SQUARE				0x06	// Square
#define OPL4_WS_LOG_SAW				0x07	// Logarithmic sawtooth (derived square)

//.............................................................................
// Group: FM Channel

//-----------------------------------------------------------------------------
// R#160-168 FM Channel 0-8 - F-Number LSB
//-----------------------------------------------------------------------------
// Register = base + channel (0-8)
// F-Number = f × 2^19 / fs / 2^(Block-1)
//	7	6	5	4	3	2	1	0
//	FN7	FN6	FN5	FN4	FN3	FN2	FN1	FN0
//	└───┴───┴───┴───┴───┴───┴───┴── F-Number, LSB (bits 7-0)
#define OPL4_REG_FNUM_L				0xA0	// R#160

//-----------------------------------------------------------------------------
// R#176-184 FM Channel 0-8 - Key On/Block/F-Number MSB
//-----------------------------------------------------------------------------
// Register = base + channel (0-8)
//	7	6	5	4	3	2	1	0
//	x	x	KON	B2	B1	B0	FN9	FN8
//			│	│	│	│	└───┴── F-Number, MSB (bits 9-8)
//			│	└───┴───┴────────── Block (octave 0-7)
//			└────────────────────── Key on
#define OPL4_REG_FNUM_H				0xB0	// R#176
#define OPL4_F_KEY_ON				0x20	// Key on

//-----------------------------------------------------------------------------
// R#189  Rhythm - AM/Vibrato Depth
//-----------------------------------------------------------------------------
// Rhythm mode uses channels 6-8
//	7	6	5	4	3	2	1	0
//	DAM	DVB	RHY	BD	SD	TOM	TC	HH
//	│	│	│	│	│	│	│	└── Hi-Hat key on
//	│	│	│	│	│	│	└────── Top Cymbal key on
//	│	│	│	│	│	└────────── Tom-tom key on
//	│	│	│	│	└────────────── Snare Drum key on
//	│	│	│	└────────────────── Bass Drum key on
//	│	│	└────────────────────── Rhythm mode enable
//	│	└────────────────────────── Vibrato depth (0: 7 cents, 1: 14 cents)
//	└────────────────────────────── AM depth (0: 1 dB, 1: 4.8 dB)
#define OPL4_REG_RHYTHM				0xBD	// R#189
#define OPL4_F_RHY_HH				0x01	// Hi-Hat
#define OPL4_F_RHY_TC				0x02	// Top Cymbal
#define OPL4_F_RHY_TOM				0x04	// Tom-tom
#define OPL4_F_RHY_SD				0x08	// Snare Drum
#define OPL4_F_RHY_BD				0x10	// Bass Drum
#define OPL4_F_RHY_ALL				0x1F	// All instruments
#define OPL4_F_RHY_EN				0x20	// Rhythm mode enable
#define OPL4_F_DVB					0x40	// Vibrato depth 14 cents
#define OPL4_F_DAM					0x80	// AM depth 4.8 dB

//-----------------------------------------------------------------------------
// R#192-200 FM Channel 0-8 - Output/Feedback/Connection
//-----------------------------------------------------------------------------
// Register = base + channel (0-8)
// On MoonSound, only outputs A (left) and B (right) are connected
//	7	6	5	4	3	2	1	0
//	CHD	CHC	CHB	CHA	FB2	FB1	FB0	CNT
//	│	│	│	│	│	│	│	└── Connection (0: FM, 1: additive)
//	│	│	│	│	└───┴───┴────── Operator 1 self-feedback (0 = off, 7 = max)
//	│	│	│	└────────────────── Output A enable (left)
//	│	│	└────────────────────── Output B enable (right)
//	│	└────────────────────────── Output C enable
//	└────────────────────────────── Output D enable
#define OPL4_REG_FBCNT				0xC0	// R#192
#define OPL4_F_CNT					0x01	// Connection (additive)
#define OPL4_F_CHA					0x10	// Output A enable (left)
#define OPL4_F_CHB					0x20	// Output B enable (right)
#define OPL4_F_CHC					0x40	// Output C enable
#define OPL4_F_CHD					0x80	// Output D enable
#define OPL4_F_LEFT					OPL4_F_CHA	// Left output enable
#define OPL4_F_RIGHT				OPL4_F_CHB	// Right output enable
#define OPL4_F_LR					0x30	// Left and right outputs enable

//-----------------------------------------------------------------------------
// FM Part 1
//-----------------------------------------------------------------------------
// Registers 100h-1FFh are accessed through the part 1 address/data ports
// FM channels 9-17 use the same registers as channels 0-8 (R#288-501)
// Only R#261 is accessible while NEW = 0
#define OPL4_PART_1					256

//-----------------------------------------------------------------------------
// R#260  4-Operator Mode - Connection Select
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x	x	C5	C4	C3	C2	C1	C0
//			│	│	│	│	│	└── Channels 0+3 in 4-operator mode
//			│	│	│	│	└────── Channels 1+4 in 4-operator mode
//			│	│	│	└────────── Channels 2+5 in 4-operator mode
//			│	│	└────────────── Channels 9+12 in 4-operator mode
//			│	└────────────────── Channels 10+13 in 4-operator mode
//			└────────────────────── Channels 11+14 in 4-operator mode
#define OPL4_REG_4OP				0x04	// R#260
#define OPL4_F_4OP_0				0x01	// Channels 0+3
#define OPL4_F_4OP_1				0x02	// Channels 1+4
#define OPL4_F_4OP_2				0x04	// Channels 2+5
#define OPL4_F_4OP_9				0x08	// Channels 9+12
#define OPL4_F_4OP_10				0x10	// Channels 10+13
#define OPL4_F_4OP_11				0x20	// Channels 11+14

//-----------------------------------------------------------------------------
// R#261  Expansion
//-----------------------------------------------------------------------------
// Set NEW2 to enable access to wave registers (writes are ignored while NEW2 = 0)
//	7	6	5	4	3	2	1	0
//	x	x	x	x	x	x	NW2	NEW
//							│	└── OPL3 mode enable (18 channels, stereo, 4-operator, 8 waveforms)
//							└────── OPL4 mode enable (wave part access)
#define OPL4_REG_EXPANSION			0x05	// R#261
#define OPL4_F_NEW					0x01	// OPL3 mode enable
#define OPL4_F_NEW2					0x02	// OPL4 mode enable

//.............................................................................
// Group: Wave System

//-----------------------------------------------------------------------------
// Wave registers
//-----------------------------------------------------------------------------
// Registers W#0-255 are accessed through the wave address/data ports
// Requires NEW2 = 1 (see R#261)
// Wave channel registers: register = base + channel (0-23)

//-----------------------------------------------------------------------------
// W#0-1  Test
//-----------------------------------------------------------------------------
// Always 00h
#define OPL4_REG_WAV_TEST0			0x00	// W#0
#define OPL4_REG_WAV_TEST1			0x01	// W#1

//-----------------------------------------------------------------------------
// W#2    Memory Control
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	ID2	ID1	ID0	TH2	TH1	TH0	MT	MA
//	│	│	│	│	│	│	│	└── Memory access mode (CPU read/write through W#6)
//	│	│	│	│	│	│	└────── Memory type (0: mode 0, 1: mode 1; MoonSound uses mode 0)
//	│	│	│	└───┴───┴────────── Wave table header (headers of waves 384-511 at TH × 80000h, 0: in ROM)
//	└───┴───┴────────────────────── Device ID (read only, 001b for YMF278B)
// MoonSound: write 10h to read headers of waves 384-511 from SRAM (200000h)
#define OPL4_REG_WAV_MEMORY			0x02	// W#2
#define OPL4_F_MEM_ACCESS			0x01	// Memory access mode
#define OPL4_F_MEM_TYPE				0x02	// Memory type (mode 1)
#define OPL4_HEADER_ROM				0x00	// Waves 384-511 header in ROM
#define OPL4_HEADER_SRAM			0x10	// Waves 384-511 header at 200000h (MoonSound SRAM)
#define OPL4_ID_MASK				0xE0	// Device ID mask
#define OPL4_ID_YMF278B				0x20	// Device ID returned by YMF278B

//-----------------------------------------------------------------------------
// W#3    Memory Address - High
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x	x	A21	A20	A19	A18	A17	A16
//			└───┴───┴───┴───┴───┴── Memory address, bits 21-16
#define OPL4_REG_WAV_ADDR_H			0x03	// W#3

//-----------------------------------------------------------------------------
// W#4    Memory Address - Middle
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	A15	A14	A13	A12	A11	A10	A9	A8
//	└───┴───┴───┴───┴───┴───┴───┴── Memory address, bits 15-8
#define OPL4_REG_WAV_ADDR_M			0x04	// W#4

//-----------------------------------------------------------------------------
// W#5    Memory Address - Low
//-----------------------------------------------------------------------------
// Writing this register applies the full address (with W#3 and W#4 values)
//	7	6	5	4	3	2	1	0
//	A7	A6	A5	A4	A3	A2	A1	A0
//	└───┴───┴───┴───┴───┴───┴───┴── Memory address, bits 7-0
#define OPL4_REG_WAV_ADDR_L			0x05	// W#5

//-----------------------------------------------------------------------------
// W#6    Memory Data
//-----------------------------------------------------------------------------
// CPU memory write or read (address auto-increments)
// Only when memory access mode is set in W#2 (writes are ignored and reads return FFh otherwise)
//	7	6	5	4	3	2	1	0
//	D7	D6	D5	D4	D3	D2	D1	D0
//	└───┴───┴───┴───┴───┴───┴───┴── Memory data
#define OPL4_REG_WAV_DATA			0x06	// W#6

//-----------------------------------------------------------------------------
// Wave header (12 bytes in memory)
//-----------------------------------------------------------------------------
// Headers of waves 0-383 are at 000000h + wave × 12
// Headers of waves 384-511 are at TH × 80000h + (wave - 384) × 12 (see W#2)
// Bytes 7-11 are copied to the wave channel registers when the header is loaded
//
// Byte	Content
// 0	Bits 7-6: Wave data format (see OPL4_FORMAT_xxx) / Bits 5-0: Start address, bits 21-16
// 1	Start address, bits 15-8
// 2	Start address, bits 7-0
// 3	Loop address (relative to start), MSB
// 4	Loop address (relative to start), LSB
// 5	End address (relative to start, 2's complement), MSB
// 6	End address (relative to start, 2's complement), LSB
// 7	LFO/Vibrato (see W#128)
// 8	Attack rate/Decay 1 rate (see W#152)
// 9	Decay level/Decay 2 rate (see W#176)
// 10	Rate correction/Release rate (see W#200)
// 11	AM depth (see W#224)
#define OPL4_HEADER_SIZE			12		// Wave header size in bytes
#define OPL4_FORMAT_8BIT			0x00	// 8-bit samples
#define OPL4_FORMAT_12BIT			0x40	// 12-bit samples (2 samples in 3 bytes)
#define OPL4_FORMAT_16BIT			0x80	// 16-bit samples

//.............................................................................
// Group: Wave Channel

//-----------------------------------------------------------------------------
// W#8-31  Wave Channel 0-23 - Wave Number LSB
//-----------------------------------------------------------------------------
// Must be written after W#32-55 (writing this register starts the header loading)
// Wait for LD status flag to be reset before writing other registers of the same channel
//	7	6	5	4	3	2	1	0
//	WN7	WN6	WN5	WN4	WN3	WN2	WN1	WN0
//	└───┴───┴───┴───┴───┴───┴───┴── Wave number, LSB (bits 7-0)
#define OPL4_REG_WAV_NUM			0x08	// W#8

//-----------------------------------------------------------------------------
// W#32-55  Wave Channel 0-23 - F-Number LSB/Wave Number MSB
//-----------------------------------------------------------------------------
// Playback rate = 44100 × 2^(Octave-1) × (1024 + F-Number) / 1024 Hz
//	7	6	5	4	3	2	1	0
//	FN6	FN5	FN4	FN3	FN2	FN1	FN0	WN8
//	│	│	│	│	│	│	│	└── Wave number, MSB (bit 8)
//	└───┴───┴───┴───┴───┴───┴────── F-Number, LSB (bits 6-0)
#define OPL4_REG_WAV_FNUM_L			0x20	// W#32
#define OPL4_F_WAV_NUM_H			0x01	// Wave number bit 8

//-----------------------------------------------------------------------------
// W#56-79  Wave Channel 0-23 - Octave/Pseudo-Reverb/F-Number MSB
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	OC3	OC2	OC1	OC0	PRV	FN9	FN8	FN7
//	│	│	│	│	│	└───┴───┴── F-Number, MSB (bits 9-7)
//	│	│	│	│	└────────────── Pseudo-reverb enable
//	└───┴───┴───┴────────────────── Octave (signed: -7 to +7, -8 = no sound)
#define OPL4_REG_WAV_FNUM_H			0x38	// W#56
#define OPL4_F_PRVB					0x08	// Pseudo-reverb enable

//-----------------------------------------------------------------------------
// W#80-103  Wave Channel 0-23 - Total Level
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	TL6	TL5	TL4	TL3	TL2	TL1	TL0	LD
//	│	│	│	│	│	│	│	└── Level direct (1: immediate change, 0: interpolated)
//	└───┴───┴───┴───┴───┴───┴────── Total level (0-127, 0.375 dB/step, 0 = loudest, 127 = silent)
#define OPL4_REG_WAV_TL				0x50	// W#80
#define OPL4_F_LEVEL_DIRECT			0x01	// Level direct

//-----------------------------------------------------------------------------
// W#104-127  Wave Channel 0-23 - Key On/Damp/LFO Reset/Output/Panpot
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	KON	DMP	LFR	CH	PN3	PN2	PN1	PN0
//	│	│	│	│	└───┴───┴───┴── Panpot (see OPL4_PAN_xxx)
//	│	│	│	└────────────────── Output select (1: DO1 pin, not connected on MoonSound)
//	│	│	└────────────────────── LFO reset (1: LFO stopped)
//	│	└────────────────────────── Damp (forced fast release)
//	└────────────────────────────── Key on
#define OPL4_REG_WAV_KEY			0x68	// W#104
#define OPL4_F_WAV_CH				0x10	// Output select (DO1)
#define OPL4_F_LFO_RESET			0x20	// LFO reset
#define OPL4_F_DAMP					0x40	// Damp
#define OPL4_F_WAV_KEY_ON			0x80	// Key on
#define OPL4_PAN_CENTER				0x00	// Center
#define OPL4_PAN_RIGHT_1			0x01	// Left attenuated by ~3 dB
#define OPL4_PAN_RIGHT_2			0x02	// Left attenuated by ~6 dB
#define OPL4_PAN_RIGHT_3			0x03	// Left attenuated by ~9 dB
#define OPL4_PAN_RIGHT_4			0x04	// Left attenuated by ~12 dB
#define OPL4_PAN_RIGHT_5			0x05	// Left attenuated by ~15 dB
#define OPL4_PAN_RIGHT_6			0x06	// Left attenuated by ~18 dB
#define OPL4_PAN_RIGHT				0x07	// Right only (left muted)
#define OPL4_PAN_MUTE				0x08	// Both muted
#define OPL4_PAN_LEFT				0x09	// Left only (right muted)
#define OPL4_PAN_LEFT_6				0x0A	// Right attenuated by ~18 dB
#define OPL4_PAN_LEFT_5				0x0B	// Right attenuated by ~15 dB
#define OPL4_PAN_LEFT_4				0x0C	// Right attenuated by ~12 dB
#define OPL4_PAN_LEFT_3				0x0D	// Right attenuated by ~9 dB
#define OPL4_PAN_LEFT_2				0x0E	// Right attenuated by ~6 dB
#define OPL4_PAN_LEFT_1				0x0F	// Right attenuated by ~3 dB

//-----------------------------------------------------------------------------
// W#128-151  Wave Channel 0-23 - LFO/Vibrato
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x	x	LF2	LF1	LF0	VB2	VB1	VB0
//			│	│	│	└───┴───┴── Vibrato depth (0-7: 0, 3.4, 5.1, 6.8, 10.1, 20.2, 40.1, 79.3 cents)
//			└───┴───┴────────────── LFO frequency (0-7: 0.168, 2.02, 3.20, 4.21, 5.22, 5.89, 6.22, 7.07 Hz)
#define OPL4_REG_WAV_LFO			0x80	// W#128

//-----------------------------------------------------------------------------
// W#152-175  Wave Channel 0-23 - Attack Rate/Decay 1 Rate
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	AR3	AR2	AR1	AR0	D13	D12	D11	D10
//	│	│	│	│	└───┴───┴───┴── Decay 1 rate (0-15)
//	└───┴───┴───┴────────────────── Attack rate (0-15, 15 = instant)
#define OPL4_REG_WAV_ARD1R			0x98	// W#152

//-----------------------------------------------------------------------------
// W#176-199  Wave Channel 0-23 - Decay Level/Decay 2 Rate
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	DL3	DL2	DL1	DL0	D23	D22	D21	D20
//	│	│	│	│	└───┴───┴───┴── Decay 2 rate (0-15)
//	└───┴───┴───┴────────────────── Decay level (0-15, 3 dB/step, 15 = -93 dB)
#define OPL4_REG_WAV_DLD2R			0xB0	// W#176

//-----------------------------------------------------------------------------
// W#200-223  Wave Channel 0-23 - Rate Correction/Release Rate
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	RC3	RC2	RC1	RC0	RR3	RR2	RR1	RR0
//	│	│	│	│	└───┴───┴───┴── Release rate (0-15)
//	└───┴───┴───┴────────────────── Rate correction (envelope speed scaling with the note)
#define OPL4_REG_WAV_RCRR			0xC8	// W#200

//-----------------------------------------------------------------------------
// W#224-247  Wave Channel 0-23 - AM Depth
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x	x	x	x	x	AM2	AM1	AM0
//						└───┴───┴── Tremolo depth (0-7: 0, 1.8, 2.9, 3.7, 4.4, 5.9, 7.4, 11.9 dB)
#define OPL4_REG_WAV_AM				0xE0	// W#224

//.............................................................................
// Group: Mixer

//-----------------------------------------------------------------------------
// W#248  FM Mix Control
//-----------------------------------------------------------------------------
// After reset: 1Bh (-9 dB on both sides)
//	7	6	5	4	3	2	1	0
//	x	x	FR2	FR1	FR0	FL2	FL1	FL0
//			│	│	│	└───┴───┴── FM left attenuation (0-7: 0 to -18 dB by 3 dB steps, 7 = mute)
//			└───┴───┴────────────── FM right attenuation (0-7: 0 to -18 dB by 3 dB steps, 7 = mute)
#define OPL4_REG_MIX_FM				0xF8	// W#248

//-----------------------------------------------------------------------------
// W#249  Wave Mix Control
//-----------------------------------------------------------------------------
// After reset: 00h (0 dB on both sides)
//	7	6	5	4	3	2	1	0
//	x	x	PR2	PR1	PR0	PL2	PL1	PL0
//			│	│	│	└───┴───┴── Wave left attenuation (0-7: 0 to -18 dB by 3 dB steps, 7 = mute)
//			└───┴───┴────────────── Wave right attenuation (0-7: 0 to -18 dB by 3 dB steps, 7 = mute)
#define OPL4_REG_MIX_WAVE			0xF9	// W#249
