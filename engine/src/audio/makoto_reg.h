// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄ ▄      ▄▄        ▄▄       
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▄▀██ ██▄▀ ▄█▀▄ ██▀ ▄█▀▄ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀▄██ ██ █ ▀█▄▀ ▀█▄ ▀█▄▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                           
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Makoto (YM2608 - OPNA) handler module
//
// References:
//  - YM2608 (OPNA) Application Manual
//     https://theretroweb.com/chip/documentation/ym2608-applicatin-manual-jp-67b9ef528ddcb605215687.pdf
//─────────────────────────────────────────────────────────────────────────────
#pragma once


//=============================================================================
// DEFINES
//=============================================================================

// Group: Status

//-----------------------------------------------------------------------------
// S#1    Status 1 (read on part 1 address port)
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	BSY	x	PCM	ZER	BRD	EOS	FB	FA
//	│		│	│	│	│	│	└── Timer A overflow flag
//	│		│	│	│	│	└────── Timer B overflow flag
//	│		│	│	│	└────────── ADPCM end of sample/transfer
//	│		│	│	└────────────── ADPCM buffer ready (CPU memory access)
//	│		│	└────────────────── ADPCM zero level detected (record mode)
//	│		└────────────────────── ADPCM playing
//	└────────────────────────────── Busy (wait before next register write)
#define OPNA_STATUS_FLAG_A			0x01	// Timer A overflow flag
#define OPNA_STATUS_FLAG_B			0x02	// Timer B overflow flag
#define OPNA_STATUS_EOS				0x04	// ADPCM end of sample/transfer
#define OPNA_STATUS_BRDY			0x08	// ADPCM buffer ready
#define OPNA_STATUS_ZERO			0x10	// ADPCM zero level detected
#define OPNA_STATUS_PCMBSY			0x20	// ADPCM playing
#define OPNA_STATUS_BUSY			0x80	// Busy

//-----------------------------------------------------------------------------
// Part 0
//-----------------------------------------------------------------------------
// Registers 0-255 are accessed through the part 0 address/data ports
// Contains SSG, rhythm, timers, FM global control and FM channels 1-3
// Only SSG registers (R#0-15) and chip ID (R#255) can be read back
#define OPNA_PART_0				0

// Group: SSG

//-----------------------------------------------------------------------------
// R#0    Tone Generator Control - Fine Tune Register - Channel A
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	TP7 TP6 TP5 TP4 TP3 TP2 TP1 TP0
//	└───┴───┴───┴───┴───┴───┴───┴── Channel A Frequency, LSB (fine tune)
#define OPNA_REG_SSG_TONE_A			0
#define OPNA_REG_SSG_TONE_A_L		0

//-----------------------------------------------------------------------------
// R#1    Tone Generator Control - Coarse Tune Register - Channel A
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x   x   x   x   TPB TPA TP9 TP8
//	                └───┴───┴───┴── Channel A Frequency, MSB (coarse tune)
#define OPNA_REG_SSG_TONE_A_H		1

//-----------------------------------------------------------------------------
// R#2    Tone Generator Control - Fine Tune Register - Channel B
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	TP7 TP6 TP5 TP4 TP3 TP2 TP1 TP0
//	└───┴───┴───┴───┴───┴───┴───┴── Channel B Frequency, LSB (fine tune)
#define OPNA_REG_SSG_TONE_B			2
#define OPNA_REG_SSG_TONE_B_L		2

//-----------------------------------------------------------------------------
// R#3    Tone Generator Control - Coarse Tune Register - Channel B
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x   x   x   x   TPB TPA TP9 TP8
//	                └───┴───┴───┴── Channel B Frequency, MSB (coarse tune)
#define OPNA_REG_SSG_TONE_B_H		3

//-----------------------------------------------------------------------------
// R#4    Tone Generator Control - Fine Tune Register - Channel C
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	TP7 TP6 TP5 TP4 TP3 TP2 TP1 TP0
//	└───┴───┴───┴───┴───┴───┴───┴── Channel C Frequency, LSB (fine tune)
#define OPNA_REG_SSG_TONE_C			4
#define OPNA_REG_SSG_TONE_C_L		4

//-----------------------------------------------------------------------------
// R#5    Tone Generator Control - Coarse Tune Register - Channel C
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	x   x   x   x   TPB TPA TP9 TP8 	
//	                └───┴───┴───┴── Channel C Frequency, MSB (coarse tune)
#define OPNA_REG_SSG_TONE_C_H		5

//-----------------------------------------------------------------------------
// R#6    Noise Generator Control
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	x   x   x   NP4 NP3 NP2 NP1 NP0 	
//	            └───┴───┴───┴───┴── Noise Frequency
#define OPNA_REG_SSG_NOISE			6

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
#define OPNA_REG_SSG_MIXER			7
#define OPNA_F_TA					(0x01)	// Tone Genrator A enable/disable
#define OPNA_F_TB					(0x02)	// Tone Genrator B enable/disable
#define OPNA_F_TC					(0x04)	// Tone Genrator C enable/disable
#define OPNA_F_NA					(0x08)	// Noise Generator A enable/disable
#define OPNA_F_NB					(0x10)	// Noise Generator B enable/disable
#define OPNA_F_NC					(0x20)	// Noise Generator C enable/disable
#define OPNA_F_IA					(0x40)	// Port A input/output control
#define OPNA_F_IB					(0x80)	// Port B input/output control

//-----------------------------------------------------------------------------
// R#8    Amplitude Control - Channel A
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	x	x	x	M	L3	L2	L1	L0 	
//  			│	└───┴───┴───┴── Channel A Amplitude (volume)
//				└────────────────── Volume controlled by Envelope enable/disable
#define OPNA_REG_SSG_AMP_A			8
#define OPNA_F_ENV					(0x10)	// Volume controlled by Envelope enable/disable

//-----------------------------------------------------------------------------
// R#9    Amplitude Control - Channel B
//-----------------------------------------------------------------------------
#define OPNA_REG_SSG_AMP_B			9

//-----------------------------------------------------------------------------
// R#10   Amplitude Control - Channel C
//-----------------------------------------------------------------------------
#define OPNA_REG_SSG_AMP_C			10

//-----------------------------------------------------------------------------
// R#11   Envelope Generator Control - Fine Tune Register
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	EP7 EP6 EP5 EP4 EP3 EP2 EP1 EP0 	
//	└───┴───┴───┴───┴───┴───┴───┴── Envelope Frequency, LSB (period fine tune)
#define OPNA_REG_SSG_ENV			11
#define OPNA_REG_SSG_ENV_L			11

//-----------------------------------------------------------------------------
// R#12   Envelope Generator Control - Coarse Tune Register
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	EPF EPE EPD EPC EPB EPA EP9 EP8 	
//	└───┴───┴───┴───┴───┴───┴───┴── Envelope Frequency, MSB (period coarse tune)
#define OPNA_REG_SSG_ENV_H			12

//-----------------------------------------------------------------------------
// R#13   Envelope Shape/Cycle Control
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	x	x	x	x	E3	E2	E1	E0 	
//  				│	│	│	└── Envelope shape, Hold
//					│   │   └────── Envelope shape, Alternate
//					│   └────────── Envelope shape, Attack
//					└────────────── Envelope shape, Continue
#define OPNA_REG_SSG_SHAPE			13
#define OPNA_F_HLD					(0x01)	// Envelope shape, Hold
#define OPNA_F_ALT					(0x02)	// Envelope shape, Alternate
#define OPNA_F_ATT					(0x04)	// Envelope shape, Attack
#define OPNA_F_CNT					(0x08)	// Envelope shape, Continue

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

//-----------------------------------------------------------------------------
// R#14   I/O Parallel Port A (ready)
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	CR	JIS	P7	P6	P4	P3	P2	P1 	
//  │	│	│	│	│	│	│	└── Pin 1 state of the selected general port (Up if joystick)
//	│	│	│	│	│   │   └────── Pin 2 state of the selected general port (Down if joystick)
//	│	│	│	│	│   └────────── Pin 3 state of the selected general port (Left if joystick)
//	│	│	│	│	└────────────── Pin 4 state of the selected general port (Right if joystick)
//	│	│	│	└────────────────── Pin 6 state of the selected general port (Trigger A if joystick)
//	│	│	└────────────────────── Pin 7 state of the selected general port (Trigger B if joystick)
//	│	└────────────────────────── 1 for JIS keyboard, 0 for JP50on (only valid for Japanese MSX)
//	└────────────────────────────── CASRD (Reading signal on cassette)
#define OPNA_REG_SSG_IO_PORT_A	14

//-----------------------------------------------------------------------------
// R#15   I/O Parallel Port B (write)
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0	
//	LED	SEL	B8	A8	B7	B6	A7	A6 	
//  │	│	│	│	│	│	│	└── Pin control 6 of the general port 1
//	│	│	│	│	│   │   └────── Pin control 7 of the general port 1
//	│	│	│	│	│   └────────── Pin control 6 of the general port 2
//	│	│	│	│	└────────────── Pin control 7 of the general port 2
//	│	│	│	└────────────────── Pin control 8 of the general port 1 (0 for standard joystick mode)
//	│	│	└────────────────────── Pin control 8 of the general port 2 (0 for standard joystick mode)
//	│	└────────────────────────── Selection of the general port readable via register 14 (1 for port 2)
//	└────────────────────────────── LED control of the "Code" or "Kana" key. (1 to turn off)
#define OPNA_REG_SSG_IO_PORT_B	15

// Group: Rhythm

//-----------------------------------------------------------------------------
// R#16   Rhythm - Key On/Dump
//-----------------------------------------------------------------------------
// Wait 576 φM cycles (72 µs @ 8 MHz) after writing this register
//	7	6	5	4	3	2	1	0
//	DM	x	RIM	TOM	HH	TOP	SD	BD
//	│		│	│	│	│	│	└── Bass Drum
//	│		│	│	│	│	└────── Snare Drum
//	│		│	│	│	└────────── Top Cymbal
//	│		│	│	└────────────── Hi-Hat
//	│		│	└────────────────── Tom-tom
//	│		└────────────────────── Rim Shot
//	└────────────────────────────── Dump (0: key on selected instruments, 1: stop them)
#define OPNA_REG_RHY_KEY		16
#define OPNA_F_RHY_BD			0x01	// Bass Drum
#define OPNA_F_RHY_SD			0x02	// Snare Drum
#define OPNA_F_RHY_TOP			0x04	// Top Cymbal
#define OPNA_F_RHY_HH			0x08	// Hi-Hat
#define OPNA_F_RHY_TOM			0x10	// Tom-tom
#define OPNA_F_RHY_RIM			0x20	// Rim Shot
#define OPNA_F_RHY_ALL			0x3F	// All instruments
#define OPNA_F_RHY_DUMP			0x80	// Dump (stop selected instruments)

//-----------------------------------------------------------------------------
// R#17   Rhythm - Total Level
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x	x	TL5	TL4	TL3	TL2	TL1	TL0
//			└───┴───┴───┴───┴───┴── Rhythm total level (0-63, 63 = loudest, 0.75 dB/step)
#define OPNA_REG_RHY_TL			17

//-----------------------------------------------------------------------------
// R#24   Rhythm - Bass Drum Output/Level
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	L	R	x	IL4	IL3	IL2	IL1	IL0
//	│	│		└───┴───┴───┴───┴── Instrument level (0-31, 31 = loudest, 0.75 dB/step)
//	│	└────────────────────────── Right output enable
//	└────────────────────────────── Left output enable
#define OPNA_REG_RHY_BD			24
#define OPNA_F_R				0x40	// Right output enable (also used by R#180-182 and R#257)
#define OPNA_F_L				0x80	// Left output enable (also used by R#180-182 and R#257)
#define OPNA_F_LR				0xC0	// Left and right outputs enable

//-----------------------------------------------------------------------------
// R#25   Rhythm - Snare Drum Output/Level
//-----------------------------------------------------------------------------
#define OPNA_REG_RHY_SD			25

//-----------------------------------------------------------------------------
// R#26   Rhythm - Top Cymbal Output/Level
//-----------------------------------------------------------------------------
#define OPNA_REG_RHY_TOP		26

//-----------------------------------------------------------------------------
// R#27   Rhythm - Hi-Hat Output/Level
//-----------------------------------------------------------------------------
#define OPNA_REG_RHY_HH			27

//-----------------------------------------------------------------------------
// R#28   Rhythm - Tom-tom Output/Level
//-----------------------------------------------------------------------------
#define OPNA_REG_RHY_TOM		28

//-----------------------------------------------------------------------------
// R#29   Rhythm - Rim Shot Output/Level
//-----------------------------------------------------------------------------
#define OPNA_REG_RHY_RIM		29

// Group: System

//-----------------------------------------------------------------------------
// R#33   Test
//-----------------------------------------------------------------------------
// Always 00h
#define OPNA_REG_TEST			33

//-----------------------------------------------------------------------------
// R#34   LFO Control
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x	x	x	x	EN	F2	F1	F0
//					│	└───┴───┴── LFO frequency (0-7: 3.98, 5.56, 6.02, 6.37, 6.88, 9.63, 48.1, 72.2 Hz)
//					└────────────── LFO enable
#define OPNA_REG_LFO			34
#define OPNA_F_LFO_EN			0x08	// LFO enable

//-----------------------------------------------------------------------------
// R#36   Timer A - MSB
//-----------------------------------------------------------------------------
// Period = 144 × (1024 - NA) / φM (18 µs step @ 8 MHz)
//	7	6	5	4	3	2	1	0
//	NA9	NA8	NA7	NA6	NA5	NA4	NA3	NA2
//	└───┴───┴───┴───┴───┴───┴───┴── Timer A value, MSB (bits 9-2)
#define OPNA_REG_TIMER_A_H		36

//-----------------------------------------------------------------------------
// R#37   Timer A - LSB
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x	x	x	x	x	x	NA1	NA0
//							└───┴── Timer A value, LSB (bits 1-0)
#define OPNA_REG_TIMER_A_L		37

//-----------------------------------------------------------------------------
// R#38   Timer B
//-----------------------------------------------------------------------------
// Period = 2304 × (256 - NB) / φM (288 µs step @ 8 MHz)
//	7	6	5	4	3	2	1	0
//	NB7	NB6	NB5	NB4	NB3	NB2	NB1	NB0
//	└───┴───┴───┴───┴───┴───┴───┴── Timer B value
#define OPNA_REG_TIMER_B		38

//-----------------------------------------------------------------------------
// R#39   Timer Control - Channel 3 Mode
//-----------------------------------------------------------------------------
// Acknowledge write must keep the current mode, enable and load bits
//	7	6	5	4	3	2	1	0
//	M1	M0	RB	RA	EB	EA	LB	LA
//	│	│	│	│	│	│	│	└── Timer A load (1: run, 0: stop)
//	│	│	│	│	│	│	└────── Timer B load (1: run, 0: stop)
//	│	│	│	│	│	└────────── Timer A flag enable
//	│	│	│	│	└────────────── Timer B flag enable
//	│	│	│	└────────────────── Timer A flag reset
//	│	│	└────────────────────── Timer B flag reset
//	└───┴────────────────────────── Channel 3 mode (00: normal, 01: special, 10: CSM)
#define OPNA_REG_TIMER_CTRL		39
#define OPNA_F_LOAD_A			0x01	// Timer A load
#define OPNA_F_LOAD_B			0x02	// Timer B load
#define OPNA_F_ENABLE_A			0x04	// Timer A flag enable
#define OPNA_F_ENABLE_B			0x08	// Timer B flag enable
#define OPNA_F_RESET_A			0x10	// Timer A flag reset
#define OPNA_F_RESET_B			0x20	// Timer B flag reset
#define OPNA_CH3_NORMAL			0x00	// Channel 3 normal mode
#define OPNA_CH3_SPECIAL		0x40	// Channel 3 special mode (one frequency per operator)
#define OPNA_CH3_CSM			0x80	// Channel 3 CSM mode (keyed by Timer A)

//-----------------------------------------------------------------------------
// R#40   FM Key On/Off
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	S4	S3	S2	S1	x	C2	C1	C0
//	│	│	│	│		└───┴───┴── Channel (0-2: CH1-3, 4-6: CH4-6)
//	│	│	│	└────────────────── Slot 1 key on
//	│	│	└────────────────────── Slot 2 key on
//	│	└────────────────────────── Slot 3 key on
//	└────────────────────────────── Slot 4 key on
#define OPNA_REG_KEY			40
#define OPNA_F_KEY_S1			0x10	// Slot 1 key on
#define OPNA_F_KEY_S2			0x20	// Slot 2 key on
#define OPNA_F_KEY_S3			0x40	// Slot 3 key on
#define OPNA_F_KEY_S4			0x80	// Slot 4 key on
#define OPNA_F_KEY_ALL			0xF0	// All slots key on

//-----------------------------------------------------------------------------
// R#41   Mode - IRQ Enable
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	SCH	x	x	EZ	EBR	EEO	ETB	ETA
//	│			│	│	│	│	└── Timer A IRQ enable
//	│			│	│	│	└────── Timer B IRQ enable
//	│			│	│	└────────── ADPCM end of sample IRQ enable
//	│			│	└────────────── ADPCM buffer ready IRQ enable
//	│			└────────────────── ADPCM zero level IRQ enable
//	└────────────────────────────── 6 FM channels enable (0: CH1-3 only)
#define OPNA_REG_MODE			41
#define OPNA_F_EN_TA			0x01	// Timer A IRQ enable
#define OPNA_F_EN_TB			0x02	// Timer B IRQ enable
#define OPNA_F_EN_EOS			0x04	// ADPCM end of sample IRQ enable
#define OPNA_F_EN_BRDY			0x08	// ADPCM buffer ready IRQ enable
#define OPNA_F_EN_ZERO			0x10	// ADPCM zero level IRQ enable
#define OPNA_F_SCH				0x80	// 6 FM channels enable

//-----------------------------------------------------------------------------
// R#45-47 Prescaler
//-----------------------------------------------------------------------------
// Selected by writing the register number on the address port only (no data)
// 2Dh: FM ÷6, SSG ÷4 (default) | 2Dh then 2Eh: FM ÷3, SSG ÷2 | 2Fh: FM ÷2, SSG ÷1
#define OPNA_REG_PRESCALER_6	45
#define OPNA_REG_PRESCALER_3	46
#define OPNA_REG_PRESCALER_2	47

// Group: FM Operator

//-----------------------------------------------------------------------------
// FM operator registers (R#48-158)
//-----------------------------------------------------------------------------
// Register = base + slot offset + channel (0-2)
// Channels 4-6 use the same registers in part 1 (see OPNA_PART_1)
#define OPNA_SLOT_1				0	// Slot 1 offset
#define OPNA_SLOT_3				4	// Slot 3 offset
#define OPNA_SLOT_2				8	// Slot 2 offset
#define OPNA_SLOT_4				12	// Slot 4 offset

//-----------------------------------------------------------------------------
// R#48-62 FM Operator - Detune/Multiple
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x	DT2	DT1	DT0	ML3	ML2	ML1	ML0
//		│	│	│	└───┴───┴───┴── Multiple (0: ×½, 1-15: ×1-×15)
//		└───┴───┴────────────────── Detune (0-3: 0/+1/+2/+3, 4-7: 0/-1/-2/-3)
#define OPNA_REG_DTMUL			48
#define OPNA_REG_DTMUL_1_S1		48
#define OPNA_REG_DTMUL_2_S1		49
#define OPNA_REG_DTMUL_3_S1		50
#define OPNA_REG_DTMUL_1_S3		52
#define OPNA_REG_DTMUL_2_S3		53
#define OPNA_REG_DTMUL_3_S3		54
#define OPNA_REG_DTMUL_1_S2		56
#define OPNA_REG_DTMUL_2_S2		57
#define OPNA_REG_DTMUL_3_S2		58
#define OPNA_REG_DTMUL_1_S4		60
#define OPNA_REG_DTMUL_2_S4		61
#define OPNA_REG_DTMUL_3_S4		62

//-----------------------------------------------------------------------------
// R#64-78 FM Operator - Total Level
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x	TL6	TL5	TL4	TL3	TL2	TL1	TL0
//		└───┴───┴───┴───┴───┴───┴── Total level (0-127, 0.75 dB/step, 0 = loudest)
#define OPNA_REG_TL				64
#define OPNA_REG_TL_1_S1		64
#define OPNA_REG_TL_2_S1		65
#define OPNA_REG_TL_3_S1		66
#define OPNA_REG_TL_1_S3		68
#define OPNA_REG_TL_2_S3		69
#define OPNA_REG_TL_3_S3		70
#define OPNA_REG_TL_1_S2		72
#define OPNA_REG_TL_2_S2		73
#define OPNA_REG_TL_3_S2		74
#define OPNA_REG_TL_1_S4		76
#define OPNA_REG_TL_2_S4		77
#define OPNA_REG_TL_3_S4		78

//-----------------------------------------------------------------------------
// R#80-94 FM Operator - Key Scale/Attack Rate
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	KS1	KS0	x	AR4	AR3	AR2	AR1	AR0
//	│	│		└───┴───┴───┴───┴── Attack rate (0-31, 31 = instant)
//	└───┴────────────────────────── Key scale (0-3)
#define OPNA_REG_KSAR			80
#define OPNA_REG_KSAR_1_S1		80
#define OPNA_REG_KSAR_2_S1		81
#define OPNA_REG_KSAR_3_S1		82
#define OPNA_REG_KSAR_1_S3		84
#define OPNA_REG_KSAR_2_S3		85
#define OPNA_REG_KSAR_3_S3		86
#define OPNA_REG_KSAR_1_S2		88
#define OPNA_REG_KSAR_2_S2		89
#define OPNA_REG_KSAR_3_S2		90
#define OPNA_REG_KSAR_1_S4		92
#define OPNA_REG_KSAR_2_S4		93
#define OPNA_REG_KSAR_3_S4		94

//-----------------------------------------------------------------------------
// R#96-110 FM Operator - AM Enable/Decay Rate
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	AM	x	x	DR4	DR3	DR2	DR1	DR0
//	│			└───┴───┴───┴───┴── Decay rate (0-31)
//	└────────────────────────────── LFO amplitude modulation enable
#define OPNA_REG_AMDR			96
#define OPNA_REG_AMDR_1_S1		96
#define OPNA_REG_AMDR_2_S1		97
#define OPNA_REG_AMDR_3_S1		98
#define OPNA_REG_AMDR_1_S3		100
#define OPNA_REG_AMDR_2_S3		101
#define OPNA_REG_AMDR_3_S3		102
#define OPNA_REG_AMDR_1_S2		104
#define OPNA_REG_AMDR_2_S2		105
#define OPNA_REG_AMDR_3_S2		106
#define OPNA_REG_AMDR_1_S4		108
#define OPNA_REG_AMDR_2_S4		109
#define OPNA_REG_AMDR_3_S4		110
#define OPNA_F_AM				0x80	// LFO amplitude modulation enable

//-----------------------------------------------------------------------------
// R#112-126 FM Operator - Sustain Rate
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x	x	x	SR4	SR3	SR2	SR1	SR0
//				└───┴───┴───┴───┴── Sustain rate (0-31, 0 = hold at sustain level)
#define OPNA_REG_SR				112
#define OPNA_REG_SR_1_S1		112
#define OPNA_REG_SR_2_S1		113
#define OPNA_REG_SR_3_S1		114
#define OPNA_REG_SR_1_S3		116
#define OPNA_REG_SR_2_S3		117
#define OPNA_REG_SR_3_S3		118
#define OPNA_REG_SR_1_S2		120
#define OPNA_REG_SR_2_S2		121
#define OPNA_REG_SR_3_S2		122
#define OPNA_REG_SR_1_S4		124
#define OPNA_REG_SR_2_S4		125
#define OPNA_REG_SR_3_S4		126

//-----------------------------------------------------------------------------
// R#128-142 FM Operator - Sustain Level/Release Rate
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	SL3	SL2	SL1	SL0	RR3	RR2	RR1	RR0
//	│	│	│	│	└───┴───┴───┴── Release rate (0-15)
//	└───┴───┴───┴────────────────── Sustain level (0-15, 3 dB/step, 15 = -93 dB)
#define OPNA_REG_SLRR			128
#define OPNA_REG_SLRR_1_S1		128
#define OPNA_REG_SLRR_2_S1		129
#define OPNA_REG_SLRR_3_S1		130
#define OPNA_REG_SLRR_1_S3		132
#define OPNA_REG_SLRR_2_S3		133
#define OPNA_REG_SLRR_3_S3		134
#define OPNA_REG_SLRR_1_S2		136
#define OPNA_REG_SLRR_2_S2		137
#define OPNA_REG_SLRR_3_S2		138
#define OPNA_REG_SLRR_1_S4		140
#define OPNA_REG_SLRR_2_S4		141
#define OPNA_REG_SLRR_3_S4		142

//-----------------------------------------------------------------------------
// R#144-158 FM Operator - SSG-type Envelope
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x	x	x	x	EN	ATT	ALT	HLD
//					│	│	│	└── Envelope shape, Hold
//					│	│	└────── Envelope shape, Alternate
//					│	└────────── Envelope shape, Attack
//					└────────────── SSG-type envelope enable (write 00h for normal envelope)
#define OPNA_REG_SSGEG			144
#define OPNA_REG_SSGEG_1_S1		144
#define OPNA_REG_SSGEG_2_S1		145
#define OPNA_REG_SSGEG_3_S1		146
#define OPNA_REG_SSGEG_1_S3		148
#define OPNA_REG_SSGEG_2_S3		149
#define OPNA_REG_SSGEG_3_S3		150
#define OPNA_REG_SSGEG_1_S2		152
#define OPNA_REG_SSGEG_2_S2		153
#define OPNA_REG_SSGEG_3_S2		154
#define OPNA_REG_SSGEG_1_S4		156
#define OPNA_REG_SSGEG_2_S4		157
#define OPNA_REG_SSGEG_3_S4		158
#define OPNA_F_SSGEG_HLD		0x01	// Envelope shape, Hold
#define OPNA_F_SSGEG_ALT		0x02	// Envelope shape, Alternate
#define OPNA_F_SSGEG_ATT		0x04	// Envelope shape, Attack
#define OPNA_F_SSGEG_EN			0x08	// SSG-type envelope enable

// Group: FM Channel

//-----------------------------------------------------------------------------
// R#160  FM Channel 1 - F-Number LSB
//-----------------------------------------------------------------------------
// F-Number = f × 144 × 2^20 / φM / 2^(Block-1)
// Writing this register applies the frequency (with the latched R#164 value)
//	7	6	5	4	3	2	1	0
//	FN7	FN6	FN5	FN4	FN3	FN2	FN1	FN0
//	└───┴───┴───┴───┴───┴───┴───┴── F-Number, LSB (bits 7-0)
#define OPNA_REG_FNUM_1_L		160

//-----------------------------------------------------------------------------
// R#161  FM Channel 2 - F-Number LSB
//-----------------------------------------------------------------------------
#define OPNA_REG_FNUM_2_L		161

//-----------------------------------------------------------------------------
// R#162  FM Channel 3 - F-Number LSB (also Slot 4 in channel 3 special mode)
//-----------------------------------------------------------------------------
#define OPNA_REG_FNUM_3_L		162

//-----------------------------------------------------------------------------
// R#164  FM Channel 1 - Block/F-Number MSB
//-----------------------------------------------------------------------------
// Must be written before R#160 (latched until F-Number LSB is written)
//	7	6	5	4	3	2	1	0
//	x	x	B2	B1	B0	FNA	FN9	FN8
//			│	│	│	└───┴───┴── F-Number, MSB (bits 10-8)
//			└───┴───┴────────────── Block (octave 0-7)
#define OPNA_REG_FNUM_1_H		164

//-----------------------------------------------------------------------------
// R#165  FM Channel 2 - Block/F-Number MSB
//-----------------------------------------------------------------------------
#define OPNA_REG_FNUM_2_H		165

//-----------------------------------------------------------------------------
// R#166  FM Channel 3 - Block/F-Number MSB (also Slot 4 in channel 3 special mode)
//-----------------------------------------------------------------------------
#define OPNA_REG_FNUM_3_H		166

//-----------------------------------------------------------------------------
// R#168  FM Channel 3 Special Mode - Slot 3 F-Number LSB
//-----------------------------------------------------------------------------
// Same layout as R#160
#define OPNA_REG_CH3_FNUM_S3_L	168

//-----------------------------------------------------------------------------
// R#169  FM Channel 3 Special Mode - Slot 1 F-Number LSB
//-----------------------------------------------------------------------------
#define OPNA_REG_CH3_FNUM_S1_L	169

//-----------------------------------------------------------------------------
// R#170  FM Channel 3 Special Mode - Slot 2 F-Number LSB
//-----------------------------------------------------------------------------
#define OPNA_REG_CH3_FNUM_S2_L	170

//-----------------------------------------------------------------------------
// R#172  FM Channel 3 Special Mode - Slot 3 Block/F-Number MSB
//-----------------------------------------------------------------------------
// Same layout as R#164
#define OPNA_REG_CH3_FNUM_S3_H	172

//-----------------------------------------------------------------------------
// R#173  FM Channel 3 Special Mode - Slot 1 Block/F-Number MSB
//-----------------------------------------------------------------------------
#define OPNA_REG_CH3_FNUM_S1_H	173

//-----------------------------------------------------------------------------
// R#174  FM Channel 3 Special Mode - Slot 2 Block/F-Number MSB
//-----------------------------------------------------------------------------
#define OPNA_REG_CH3_FNUM_S2_H	174

//-----------------------------------------------------------------------------
// R#176  FM Channel 1 - Feedback/Algorithm
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x	x	FB2	FB1	FB0	AL2	AL1	AL0
//			│	│	│	└───┴───┴── Algorithm (0-7)
//			└───┴───┴────────────── Slot 1 self-feedback (0 = off, 7 = max)
#define OPNA_REG_FBALG_1		176

//-----------------------------------------------------------------------------
// R#177  FM Channel 2 - Feedback/Algorithm
//-----------------------------------------------------------------------------
#define OPNA_REG_FBALG_2		177

//-----------------------------------------------------------------------------
// R#178  FM Channel 3 - Feedback/Algorithm
//-----------------------------------------------------------------------------
#define OPNA_REG_FBALG_3		178

//-----------------------------------------------------------------------------
// R#180  FM Channel 1 - Output/LFO Sensitivity
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	L	R	AM1	AM0	x	PM2	PM1	PM0
//	│	│	│	│		└───┴───┴── Phase modulation sensitivity (0-7: 0, ±3.4, ±6.7, ±10, ±14, ±20, ±40, ±80 cents)
//	│	│	└───┴────────────────── Amplitude modulation sensitivity (0-3: 0, 1.4, 5.9, 11.8 dB)
//	│	└────────────────────────── Right output enable
//	└────────────────────────────── Left output enable
#define OPNA_REG_PAN_1			180

//-----------------------------------------------------------------------------
// R#181  FM Channel 2 - Output/LFO Sensitivity
//-----------------------------------------------------------------------------
#define OPNA_REG_PAN_2			181

//-----------------------------------------------------------------------------
// R#182  FM Channel 3 - Output/LFO Sensitivity
//-----------------------------------------------------------------------------
#define OPNA_REG_PAN_3			182

//-----------------------------------------------------------------------------
// R#255  Chip ID (read only)
//-----------------------------------------------------------------------------
#define OPNA_REG_ID				255
#define OPNA_ID_YM2608			0x01	// Value returned by YM2608

// Group: ADPCM-B

//-----------------------------------------------------------------------------
// Part 1
//-----------------------------------------------------------------------------
// Registers 100h-1FFh are accessed through the part 1 address/data ports
// FM channels 4-6 use the same registers as channels 1-3 (R#304-414 and R#416-438)
#define OPNA_PART_1				256

//-----------------------------------------------------------------------------
// R#256  ADPCM - Control 1
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	STA	REC	MEM	RPT	SPO	x	x	RST
//	│	│	│	│	│			└── Reset (abort current operation)
//	│	│	│	│	└────────────── Speaker off
//	│	│	│	└────────────────── Repeat (loop from start address)
//	│	│	└────────────────────── External memory access
//	│	└────────────────────────── Record/CPU write (0: playback/CPU read)
//	└────────────────────────────── Start
#define OPNA_REG_ADPCM_CTRL1	256
#define OPNA_F_ADPCM_RESET		0x01	// Reset
#define OPNA_F_ADPCM_SPOFF		0x08	// Speaker off
#define OPNA_F_ADPCM_REPEAT		0x10	// Repeat
#define OPNA_F_ADPCM_MEM		0x20	// External memory access
#define OPNA_F_ADPCM_REC		0x40	// Record/CPU write
#define OPNA_F_ADPCM_START		0x80	// Start

//-----------------------------------------------------------------------------
// R#257  ADPCM - Control 2
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	L	R	x	x	SMP	DA	RAM	ROM
//	│	│			│	│	│	└── Memory is ROM
//	│	│			│	│	└────── RAM type (0: x1-bit, 4-byte unit / 1: x8-bit, 32-byte unit)
//	│	│			│	└────────── D/A-A/D conversion mode
//	│	│			└────────────── Sample mode (A/D conversion)
//	│	└────────────────────────── Right output enable
//	└────────────────────────────── Left output enable
// Makoto: 256 KB DRAM in x1-bit mode (RAM type = 0)
#define OPNA_REG_ADPCM_CTRL2	257
#define OPNA_F_ADPCM_ROM		0x01	// Memory is ROM
#define OPNA_F_ADPCM_RAM8		0x02	// RAM type x8-bit
#define OPNA_F_ADPCM_DAAD		0x04	// D/A-A/D conversion mode
#define OPNA_F_ADPCM_SAMPLE		0x08	// Sample mode

//-----------------------------------------------------------------------------
// R#258  ADPCM - Start Address LSB
//-----------------------------------------------------------------------------
// Address in memory units (4 bytes for x1-bit RAM, 32 bytes for x8-bit RAM/ROM)
//	7	6	5	4	3	2	1	0
//	A7	A6	A5	A4	A3	A2	A1	A0
//	└───┴───┴───┴───┴───┴───┴───┴── Start address, LSB
#define OPNA_REG_ADPCM_START_L	258

//-----------------------------------------------------------------------------
// R#259  ADPCM - Start Address MSB
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	AF	AE	AD	AC	AB	AA	A9	A8
//	└───┴───┴───┴───┴───┴───┴───┴── Start address, MSB
#define OPNA_REG_ADPCM_START_H	259

//-----------------------------------------------------------------------------
// R#260  ADPCM - Stop Address LSB
//-----------------------------------------------------------------------------
// Same layout as R#258 (inclusive: the whole last unit is used)
#define OPNA_REG_ADPCM_STOP_L	260

//-----------------------------------------------------------------------------
// R#261  ADPCM - Stop Address MSB
//-----------------------------------------------------------------------------
// Same layout as R#259
#define OPNA_REG_ADPCM_STOP_H	261

//-----------------------------------------------------------------------------
// R#262  ADPCM - Prescale LSB
//-----------------------------------------------------------------------------
// A/D-D/A conversion sampling rate (not used for memory playback)
//	7	6	5	4	3	2	1	0
//	P7	P6	P5	P4	P3	P2	P1	P0
//	└───┴───┴───┴───┴───┴───┴───┴── Prescale, LSB
#define OPNA_REG_ADPCM_PRESCALE_L	262

//-----------------------------------------------------------------------------
// R#263  ADPCM - Prescale MSB
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	x	x	x	x	x	PA	P9	P8
//						└───┴───┴── Prescale, MSB (bits 10-8)
#define OPNA_REG_ADPCM_PRESCALE_H	263

//-----------------------------------------------------------------------------
// R#264  ADPCM - Data
//-----------------------------------------------------------------------------
// CPU memory write (REC = 1) or read (REC = 0) through part 1 data port
// Wait for BRDY status flag between each byte
//	7	6	5	4	3	2	1	0
//	D7	D6	D5	D4	D3	D2	D1	D0
//	└───┴───┴───┴───┴───┴───┴───┴── ADPCM data
#define OPNA_REG_ADPCM_DATA		264

//-----------------------------------------------------------------------------
// R#265  ADPCM - Delta-N LSB
//-----------------------------------------------------------------------------
// Delta-N = Fs × 65536 / (φM / 144) (16 kHz: 49BAh, 22.05 kHz: 659Ch, 44.1 kHz: CB37h @ 8 MHz)
//	7	6	5	4	3	2	1	0
//	DN7	DN6	DN5	DN4	DN3	DN2	DN1	DN0
//	└───┴───┴───┴───┴───┴───┴───┴── Delta-N, LSB
#define OPNA_REG_ADPCM_DELTA_L	265

//-----------------------------------------------------------------------------
// R#266  ADPCM - Delta-N MSB
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	DNF	DNE	DND	DNC	DNB	DNA	DN9	DN8
//	└───┴───┴───┴───┴───┴───┴───┴── Delta-N, MSB
#define OPNA_REG_ADPCM_DELTA_H	266

//-----------------------------------------------------------------------------
// R#267  ADPCM - Level
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	L7	L6	L5	L4	L3	L2	L1	L0
//	└───┴───┴───┴───┴───┴───┴───┴── Output level (00h = silent, FFh = max)
#define OPNA_REG_ADPCM_LEVEL	267

//-----------------------------------------------------------------------------
// R#268  ADPCM - Limit Address LSB
//-----------------------------------------------------------------------------
// Same layout as R#258 (CPU memory access wraps to 0 after this unit)
#define OPNA_REG_ADPCM_LIMIT_L	268

//-----------------------------------------------------------------------------
// R#269  ADPCM - Limit Address MSB
//-----------------------------------------------------------------------------
// Same layout as R#259
#define OPNA_REG_ADPCM_LIMIT_H	269

//-----------------------------------------------------------------------------
// R#270  ADPCM - DAC Data
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	D7	D6	D5	D4	D3	D2	D1	D0
//	└───┴───┴───┴───┴───┴───┴───┴── Direct output value (D/A mode)
#define OPNA_REG_ADPCM_DAC		270

//-----------------------------------------------------------------------------
// R#271  ADPCM - PCM Data
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	D7	D6	D5	D4	D3	D2	D1	D0
//	└───┴───┴───┴───┴───┴───┴───┴── A/D-D/A conversion data
#define OPNA_REG_ADPCM_PCM		271

//-----------------------------------------------------------------------------
// R#272  ADPCM - Flag Control
//-----------------------------------------------------------------------------
// Some emulators power up with 1Ch (BRDY masked): write 80h before polling BRDY
//	7	6	5	4	3	2	1	0
//	RST	x	x	MZ	MBR	MEO	MTB	MTA
//	│			│	│	│	│	└── Mask Timer A flag
//	│			│	│	│	└────── Mask Timer B flag
//	│			│	│	└────────── Mask ADPCM end of sample flag
//	│			│	└────────────── Mask ADPCM buffer ready flag
//	│			└────────────────── Mask ADPCM zero level flag
//	└────────────────────────────── Reset all status 1 flags
#define OPNA_REG_ADPCM_FLAG		272
#define OPNA_F_MSK_TA			0x01	// Mask Timer A flag
#define OPNA_F_MSK_TB			0x02	// Mask Timer B flag
#define OPNA_F_MSK_EOS			0x04	// Mask ADPCM end of sample flag
#define OPNA_F_MSK_BRDY			0x08	// Mask ADPCM buffer ready flag
#define OPNA_F_MSK_ZERO			0x10	// Mask ADPCM zero level flag
#define OPNA_F_IRQ_RESET		0x80	// Reset all status 1 flags
