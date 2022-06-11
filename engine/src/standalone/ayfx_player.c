// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// ayFX Replayer
//-----------------------------------------------------------------------------
// Based on ayFX REPLAYER v1.31 by SapphiRe
//  - v1.31		Fixed bug on previous version, only PSG channel C worked
//  - v1.3		Fixed volume and Relative volume versions on the same file, conditional compilation
//				Support for dynamic or fixed channel allocation
//  - v1.2f/r	ayFX bank support
//  - v1.11f/r	If a frame volume is zero then no AYREGS update
//  - v1.1f/r	Fixed volume for all ayFX streams
//  - v1.1		Explicit priority (as suggested by AR)
//  - v1.0f		Bug fixed (error when using noise)
//  - v1.0		Initial release
//-----------------------------------------------------------------------------
#include "ayfx_player.h"

/// ayFX mode
unsigned char  ayFX_Mode = AYFX_MODE_FIXED;
//	x	x	x	x	x	x	PS	FS
//	7	6	5	4	3	2	1	0	
//							│	└── Channel frame-switch mode (inc channel at each update)
//							└────── Channel play-switch mode (inc channel at each play)

/// Current ayFX Bank
unsigned short ayFX_Bank;

/// Current ayFX stream priotity
unsigned char  ayFX_Priority = 0xFF;
//	M	x	x	x	P3	P2	P1	P0
//	7	6	5	4	3	2	1	0	
//	│				└───┴───┴───┴──	Priority (0-15)
//	└────────────────────────────── Mute (stop playback)

// Pointer to the current ayFX stream
unsigned short ayFX_Pointer;

// Current tone of the ayFX stream
unsigned short ayFX_Tone;

// Current noise of the ayFX stream
unsigned char  ayFX_Noise;

// Current volume of the ayFX stream
unsigned char  ayFX_Volume;

// PSG channel to play the ayFX stream
unsigned char  ayFX_Channel = 3 - PSG_CHANNEL_C;

/// Finish callback
/// @note Can be:	ayFX_Stop to only stop the sound playback
///					ayFX_Mute to stop the sound playback and mute the ayFX current channel
///					Any custom callback from application (must handle playback termination and mute as needed)
Callback ayFX_Finish = ayFX_Mute;

// PSG register buffer
unsigned char  ayRegs[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
#define AYREGS _ayRegs

//-----------------------------------------------------------------------------
/// Setup the ayFX replayer
/// @param		bank		Pointer to the ayFX bank
void ayFX_InitBank(void* bank) __z88dk_fastcall
{
	bank;
	__asm
		// --- INPUT: HL -> pointer to the ayFX bank ---
		ld		(_ayFX_Bank), hl	// Current ayFX bank
	__endasm;
}	

//-----------------------------------------------------------------------------
/// Play a ayFX sound
/// @param		id		Sound index in the bank
/// @param		prio	Priority of the sound (0-15). 0 is the highest priority.
/// @return				Error number (if any) @see AYFX_ERROR
unsigned char ayFX_PlayBankFC(unsigned short snd_prio) __z88dk_fastcall __naked
{
	snd_prio;
	__asm
		// Fastcall init
		ld		a, l
		ld		c, h
		// --- INPUT: L -> sound to be played ---
		// ---        H -> sound priority     ---
		push	bc					// Store bc in stack
		push	de					// Store de in stack
		push	hl					// Store hl in stack
		// --- Check if the index is in the bank ---
		ld		b, a				// b:=a (new ayFX stream index)
		ld		hl, (_ayFX_Bank)	// Current ayFX BANK
		ld		a, (hl)				// Number of samples in the bank
		or		a					// If zero (means 256 samples)...
		jr		z, CHECK_PRIO		// ...goto CHECK_PRIO
		// The bank has less than 256 samples
	CHECK_INDEX:	
		ld		a, b				// a:=b (new ayFX stream index)
		cp		(hl)				// If new index is not in the bank...
		ld		a, #2				// a:=2 (error 2: Sample not in the bank)
		jr		nc, INIT_END		// ...we can't init it
	CHECK_PRIO:	
		// --- Check if the new priority is lower than the current one ---
		// ---   Remember: 0 = highest priority, 15 = lowest priority  ---
		ld		a, b				// a:=b (new ayFX stream index)
		ld		a, (_ayFX_Priority)	// a:=Current ayFX stream priority
		cp		c					// If new ayFX stream priority is lower than current one...
		ld		a, #1				// a:=1 (error 1: A sample with higher priority is being played)
		jr		c, INIT_END			// ...we don't start the new ayFX stream
		// --- Set new priority ---
		ld		a, c				// a:=New priority
		and		#0x0F				// We mask the priority
		ld		(_ayFX_Priority), a	// new ayFX stream priority saved in RAM

		// --- Calculate the pointer to the new ayFX stream ---
		ld		de, (_ayFX_Bank)	// de:=Current ayFX bank
		inc		de					// de points to the increments table of the bank
		ld		l, b				// l:=b (new ayFX stream index)
		ld		h, #0				// hl:=b (new ayFX stream index)
		add		hl, hl				// hl:=hl*2
		add		hl, de				// hl:=hl+de (hl points to the correct increment)
		ld		e, (hl)				// e:=lower byte of the increment
		inc		hl					// hl points to the higher byte of the correct increment
		ld		d, (hl)				// de:=increment
		add		hl, de				// hl:=hl+de (hl points to the new ayFX stream)
		ld		(_ayFX_Pointer), hl	// Pointer saved in RAM
		xor		a					// a:=0 (no errors)
	INIT_END:
		pop		hl					// Retrieve hl from stack
		pop		de					// Retrieve de from stack
		pop		bc					// Retrieve bc from stack
		ld		l, a				// Set return value
		ret							// Return

	__endasm;
}

//-----------------------------------------------------------------------------
/// Play a SFX directly from data pointer
/// @param		file	Pointer to AFX file data
unsigned char ayFX_Play(void* data) __z88dk_fastcall
{
	data;
	__asm

		ld		(_ayFX_Pointer), hl	// 
		xor		a					// 
		ld		(_ayFX_Priority), a	// 
	
	__endasm;
	
	return 0;
}

//-----------------------------------------------------------------------------
/// Send data to PSG registers
/// @note					Must be executed on each V-Blank interruption
void ayFX_SendToPSG()
{
__asm

	// Update mixer register wanted value with I/O 2-bits from the current mixer register value
	ld		A, (#AYREGS + PSG_REG_MIXER)
	and		#0b00111111
	ld		B, A
	ld		A, #PSG_REG_MIXER
	out		(#PSG_PORT_REG), A
	in		A, (#PSG_PORT_READ)  
	and		#0b11000000
	or		B
	ld		(#AYREGS + PSG_REG_MIXER), A

	// Registers value copy loop (528 T-States)
	ld		HL, #AYREGS	// 11	Data to copy to PSG registers
	ld		C, #PSG_PORT_WRITE		// 8	Setup outi register
	xor		A						// 5	Initialize register number
	// R#0-12
	.rept 13
		out		(PSG_PORT_REG), A	// 12	port_reg <- reg_num
		outi						// 18	port_data <- data[i++]
		inc		A					// 5
	.endm
	// R#13
	out		(PSG_PORT_REG), A		// 12	port_reg <- reg_num
	ld		A, (HL)					// 8
	and		A						// 5
	ret		M						// 12	don't copy R#13 if value is negative
	out		(PSG_PORT_WRITE), A		// 12	port_data <- data[i]

__endasm;
}

//-----------------------------------------------------------------------------
/// Play a frame of an ayFX stream
void ayFX_Update()
{
	__asm
		// --- PLAY A FRAME OF AN ayFX STREAM ---
		ld		a, (_ayFX_Priority)	// a:=Current ayFX stream priority
		or		a					// If priority has bit 7 on  (priority < 0)...
		ret		m					// ...then return
		// --- Calculate next ayFX channel (if needed) ---
		ld		a, (_ayFX_Mode)		// ayFX mode
		and		#1					// If bit0=0 (fixed channel)...
		jr		z, TAKECB			// ...skip channel changing
	CHANNEL_SWITCH:
		ld		hl, #_ayFX_Channel	// Old ayFX playing channel
		dec		(hl)				// New ayFX playing channel
		jr		nz, TAKECB			// If not zero jump to TAKECB
		ld		(hl), #3			// If zero -> set channel 3
	TAKECB:	
		// --- Extract control byte from stream ---
		ld		hl, (_ayFX_Pointer)	// Pointer to the current ayFX stream
		ld		c, (hl)				// c:=Control byte
		inc		hl					// Increment pointer
		// --- Check if there's new tone on stream ---
		bit		5, c				// If bit 5 c is off...
		jr		z, CHECK_NN			// ...jump to CHECK_NN (no new tone)
		// --- Extract new tone from stream ---
		ld		e, (hl)				// e:=lower byte of new tone
		inc		hl					// Increment pointer
		ld		d, (hl)				// d:=higher byte of new tone
		inc		hl					// Increment pointer
		ld		(_ayFX_Tone), de	// ayFX tone updated
	CHECK_NN:	
		// --- Check if there's new noise on stream ---
		bit		6, c				// if bit 6 c is off...
		jr		z, SETPOINTER		// ...jump to SETPOINTER (no new noise)
		// --- Extract new noise from stream ---
		ld		a, (hl)				// a:=New noise
		inc		hl					// Increment pointer
		cp		#0x20				// If it's an illegal value of noise (used to mark end of stream)...
#if (0)
		jr		z, ayFX_END			// ...jump to ayFX_END
#else
		jr		nz, CONTINUE
		ld		hl, (_ayFX_Finish)
		jp		(hl)
	CONTINUE:
#endif
		ld		(_ayFX_Noise), a	// ayFX noise updated
	SETPOINTER:	
		// --- Update ayFX pointer ---
		ld		(_ayFX_Pointer), hl	// Update ayFX stream pointer
		// --- Extract volume ---
		ld		a, c				// a:=Control byte
		and		#0x0F				// lower nibble

		ld		(_ayFX_Volume), a	// ayFX volume updated
		ret		z					// ...return (don't copy ayFX values in to AYREGS)
		// -------------------------------------
		// --- COPY ayFX VALUES IN TO AYREGS ---
		// -------------------------------------
		// --- Set noise channel ---
		bit		7, c				// If noise is off...
		jr		nz, SETMASKS		// ...jump to SETMASKS
		ld		a, (_ayFX_Noise)	// ayFX noise value
		ld		(AYREGS+6), a		// copied in to AYREGS (noise channel)
	SETMASKS:	// --- Set mixer masks ---
		ld		a, c				// a:=Control byte
		and		#0x90				// Only bits 7 and 4 (noise and tone mask for psg reg 7)
		cp		#0x90				// If no noise and no tone...
		ret		z					// ...return (don't copy ayFX values in to AYREGS)
		// --- Copy ayFX values in to ARYREGS ---
		rrca						// Rotate a to the right (1 TIME)
		rrca						// Rotate a to the right (2 TIMES) (OR mask)
		ld		d, #0xDB			// d:=Mask for psg mixer (AND mask)
		// --- Dump to correct channel ---
		ld		hl, #_ayFX_Channel	// Next ayFX playing channel
		ld		b, (hl)				// Channel counter
	CHK1:		// --- Check if playing channel was 1 ---
		djnz	CHK2				// Decrement and jump if channel was not 1
	PLAY_C:	// --- Play ayFX stream on channel C ---
		call	SETMIXER			// Set PSG mixer value (returning a=ayFX volume and hl=ayFX tone)
		ld		(AYREGS+10), a		// Volume copied in to AYREGS (channel C volume)
		bit		2, c				// If tone is off...
		ret		nz					// ...return
		ld		(AYREGS+4), hl		// copied in to AYREGS (channel C tone)
		ret							// Return
	CHK2: // --- Check if playing channel was 2 ---
		rrc		d					// Rotate right AND mask
		rrca						// Rotate right OR mask
		djnz	CHK3				// Decrement and jump if channel was not 2
	PLAY_B:	// --- Play ayFX stream on channel B ---
		call	SETMIXER			// Set PSG mixer value (returning a=ayFX volume and hl=ayFX tone)
		ld		(AYREGS+9), a		// Volume copied in to AYREGS (channel B volume)
		bit		1, c				// If tone is off...
		ret		nz					// ...return
		ld		(AYREGS+2), hl		// copied in to AYREGS (channel B tone)
		ret							// Return
	CHK3: // --- Check if playing channel was 3 ---
		rrc		d					// Rotate right AND mask
		rrca						// Rotate right OR mask
	PLAY_A:	// --- Play ayFX stream on channel A ---
		call	SETMIXER			// Set PSG mixer value (returning a=ayFX volume and hl=ayFX tone)
		ld		(AYREGS+8), a		// Volume copied in to AYREGS (channel A volume)
		bit		0, c				// If tone is off...
		ret		nz					// ...return
		ld		(AYREGS+0), hl		// copied in to AYREGS (channel A tone)
		ret							// Return
	SETMIXER:	// --- Set PSG mixer value ---
		ld		c, a				// c:=OR mask
		ld		a, (AYREGS+7)		// a:=PSG mixer value
		and		d					// AND mask
		or		c					// OR mask
		ld		(AYREGS+7), a		// PSG mixer value updated
		ld		a, (_ayFX_Volume)	// a:=ayFX volume value
		ld		hl, (_ayFX_Tone)	// ayFX tone value
		ret							// Return

__endasm;
}

//-----------------------------------------------------------------------------
/// Stop the sound playback and mute the channel
void ayFX_Mute()
{
	__asm
		// --- End of an ayFX stream ---
		ld		a, #255				// Lowest ayFX priority
		ld		(_ayFX_Priority), a	// Priority saved (not playing ayFX stream)

		// Silence the current ayFX channel
		ld		hl, #_ayFX_Channel	// Next ayFX playing channel
		ld		b, (hl)				// Channel counter
		xor		a
	StopCheck1:
		// If playing channel was 1, silence channel C
		djnz	StopCheck2			// Decrement and jump if channel was not 1
		ld		(AYREGS+10), a		// Volume copied in to AYREGS (channel C volume)
		ret
	StopCheck2:
		// If playing channel was 2, silence channel B
		djnz	StopCheck3			// Decrement and jump if channel was not 2
		ld		(AYREGS+9), a		// Volume copied in to AYREGS (channel B volume)
		ret
	StopCheck3:
		// If playing channel was 3, silence channel A
		ld		(AYREGS+8), a		// Volume copied in to AYREGS (channel A volume)

	__endasm;
}

//-----------------------------------------------------------------------------
/// Stop the sound playback
void ayFX_Stop()
{
	__asm
		// --- End of an ayFX stream ---
		ld		a, #255				// Lowest ayFX priority
		ld		(_ayFX_Priority), a	// Priority saved (not playing ayFX stream)
	__endasm;
}

