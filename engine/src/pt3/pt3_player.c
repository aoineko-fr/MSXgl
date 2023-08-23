// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// SDCC Vortex Tracker II PT3 player for MSX
// 
// Version: 1.1.4 (08/01/2021)
// Architecture: MSX
// Format: C Object (SDCC .rel)
// Programming language: C and Z80 assembler
// 
// Authors:
//  - Vortex Tracker II v1.0 PT3 player for ZX Spectrum by S.V.Bulba 
//    <vorobey@mail.khstu.ru> http://bulba.at.kz
//  - (09-Jan-05) Adapted to MSX by Alfonso D. C. aka Dioniso 
//    <dioniso072@yahoo.es>
//  - Arrangements for MSX ROM: MSXKun/Paxanga soft > 
//    http://paxangasoft.retroinvaders.com/
//  - asMSX version: SapphiRe > http://www.z80st.es/
//  - Adapted to SDCC: mvac7/303bcn > <mvac7303b@gmail.com>
// 
// Description:
//   Adaptation of the Vortex Tracker II PT3 Player for MSX to be used in 
//   software development in C (SDCC). 
//   
// History of versions:
//  - 1.1.4 (08/01/2021)>PT3_Init and Bug #11 in loop
//  - 1.1.3 (05/01/2021) PT3state, PT3_Loop, PT3_Pause and PT3_Resume
//  - 1.1.2 (04/01/2021) assignment of frequency table memory address to NoteTable 
//  - 1.1 (28/05/2019) Adaptation to SDCC of asMSX version by SapphiRe.
//  - 1.0 (21/10/2016) Adaptation to SDCC of the ROM version by Kun.
//-----------------------------------------------------------------------------
// Dioniso version:
//  - No version detection (just for Vortex Tracker II and PT3.5).
//  - No frequency table decompression (default is number 2). 
//    Coger tabla segun quiera, en fichero aparte
//  - No volume table decompression (Vortex Tracker II/PT3.5 volume table used).
// 
// msxKun version:
//  - Usable desde ROM (solo tiene en RAM area de trabajo, lo minimo posible).
// 
// SapphiRe version:
//  This version of the replayer uses a fixed volume and note table, if you need a 
//  different note table you can copy it from TABLES.TXT file, distributed with the
//  original PT3 distribution. This version also allows the use of PT3 commands.
// 
//  PLAY and PSG WRITE routines seperated to allow independent calls
// 
// mvac7 version:
//  Adaptation to C (SDCC) of the SapphiRe version.
//─────────────────────────────────────────────────────────────────────────────
#include "pt3_player.h"

//VARS:
u8  ChanA[CHNPRM_Size];
u8  ChanB[CHNPRM_Size];
u8  ChanC[CHNPRM_Size];			

u8  DelyCnt;
u16 CurESld;		
u8  CurEDel;

//Ns_Base_AddToNs:
u8  Ns_Base;		
u8  AddToNs;		

//AYREGS::
u8  PT3_Regs[14];
#define AYREGS _PT3_Regs
u16 EnvBase;
u8  VAR0END[240];

u8  PT3_State;			// Before called PT3_SETUP
//	x	x	LR	LE	MC	MB	MA	PLY
//	7	6	5	4	3	2	1	0	
//			│	│	│	│	│	└── Play Enable
//			│	│	│	│	└────── Mute Channel A [EXTRA]
//			│	│	│	└────────── Mute Channel B [EXTRA]
//			│	│	└────────────── Mute Channel C [EXTRA]
//			│	└────────────────── Loop Enable
//			└────────────────────── Loop Reached [EXTRA]
         
u16 PT3_ModAddr;		// Song data address
u16 PT3_CrPsPtr;		// Cursor position in pattern
u16 PT3_SamPtrs;		// Sample info?
u16 PT3_OrnPtrs;		// Ornament pattern

u16 PT3_PDSP;			// Stack pointer backup
u16 PT3_CSP;			// Stack pointer backup
u16 PT3_PSP;			// Stack pointer backup
  
u8  PT3_PrNote;			//
u16 PT3_PrSlide;		//
  
u16 PT3_AdInPtA;		// Play data pattern
u16 PT3_AdInPtB;		// Play data
u16 PT3_AdInPtC;		// Play data
  
u16 PT3_LPosPtr;		// Position Ptr?
u16 PT3_PatsPtr;		// Pat Ptr

u8  PT3_Delay;			// Delay
u8  PT3_AddToEn;		// Envelope data (No scale as it does not use Envs??)
u8  PT3_Env_Del;		// Envelope data (idem)
u16 PT3_ESldAdd;		// Envelope data (idem)

const void* PT3_NoteTable;    // note table memory address

#if (PT3_EXTRA)

/// Cursor position in pattern at start
u16 PT3_SrtCrPsPtr;

void EmptyCB() {}

/// Finish callback
callback PT3_Finish = EmptyCB;


#endif // (PT3_EXTRA)

	
//-----------------------------------------------------------------------------
//
// FUNCTIONS PROTOTYPE
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// Initialize the PT3 player
void PT3_Init()
{
// Create Volume Table for Vortex Tracker II/PT3.5
// (c) Ivan Roshin, adapted by SapphiRe ---
__asm
	ld		HL, #0x11
	ld		D, H
	ld		E, H
	ld		IY, #_VAR0END  ;_VT_ + 16
	ld		B, #15
INITV1:	
	push	HL
	add		HL, DE
	ex		DE, HL
	sbc		HL, HL
	ld		C, B 
	ld		B, #16
INITV2:	
	ld		A, L
	rla
	ld		A, H
	adc		A, #0
	ld		(IY), A
	inc		IY
	add		HL, DE
	djnz	INITV2
	pop		HL
	ld		A, E
	cp		#0x77
	jr		NZ, INITV3
	inc		E
INITV3:	
	ld		B, C
	djnz	INITV1

InitClearRegs: 
	xor		A  
	ld		(#_PT3_State), A		// Reset PT3_State
	ld		HL, #AYREGS
	ld		DE, #AYREGS + 1
	ld		BC, #13
	ld		(HL), A
	ldir							// Reset PT3_Regs

__endasm;
}

//-----------------------------------------------------------------------------
/// Initialize a given song to make it ready to playback
/// @param		songAddr	Start address of the data
///							If PT3_SKIP_HEADER is set, this address must be header address + 100 (if the data are not troncated)
void PT3_InitSong(const void* songAddr)
{
	songAddr; // HL
__asm
#if (PT3_SKIP_HEADER)
	ld		DE, #-100
	add		HL, DE
#endif

	ld		(#_PT3_ModAddr), HL		// _PT3_ModAddr = songAddr
	ex		DE, HL					// DE = songAddr

	ld		HL, #100
	add		HL, DE
	ld		A, (HL)
	ld		(#_PT3_Delay), A		// _PT3_Delay = [songAddr + 100]

	ld		HL, #105
	add		HL, DE
	ld		(#_PT3_SamPtrs), HL		// _PT3_SamPtrs = songAddr + 105

	ld		HL, #169
	add		HL, DE
	ld		(#_PT3_OrnPtrs), HL		// _PT3_OrnPtrs = songAddr + 169

	ld		HL, #200
	add		HL, DE
	ld		(#_PT3_CrPsPtr), HL		// _PT3_CrPsPtr = songAddr + 200
#if (PT3_EXTRA)
	ld		(#_PT3_SrtCrPsPtr), HL	// Backup startup value
#endif

	ld		IY, (#_PT3_ModAddr)		// IY = songAddr

	ld		B, #0
	ld		C, 102(IY)				// BC = [songAddr + 102]
	ld		HL, #201
	add		HL, DE
	add		HL, BC
	ld		(#_PT3_LPosPtr), HL		// _PT3_LPosPtr = songAddr + 201 + BC

	ld		B, 104(IY)
	ld		C, 103(IY)				// BC = [songAddr + 104 | songAddr + 103]
	ex		DE, HL					// HL = songAddr
	add		HL, BC
	ld		(#_PT3_PatsPtr), HL		// _PT3_PatsPtr  = songAddr + BC

	// Initialize PT3 Variables
	xor		A	
	ld		HL, #_ChanA				; VARS
	ld		(HL), A
	ld		DE, #_ChanA + 1			; VARS+1
	ld		BC, #_VAR0END - _ChanA -1    ;AYREGS - _ChanA -1
	ldir

	inc		A
	ld		(#_DelyCnt), A
	ld		HL, #0xF001				; H - CHNPRM_Volume, L - CHNPRM_NtSkCn
	ld		(#_ChanA + CHNPRM_NtSkCn), HL
	ld		(#_ChanB + CHNPRM_NtSkCn), HL
	ld		(#_ChanC + CHNPRM_NtSkCn), HL

	ld		HL, #EMPTYSAMORN
	ld		(#_PT3_AdInPtA), HL		; ptr to zero  ; # chg
	ld		(#_ChanA + CHNPRM_OrnPtr), HL ;ornament 0 is "0,1,0"
	ld		(#_ChanB + CHNPRM_OrnPtr), HL ;in all versions from
	ld		(#_ChanC + CHNPRM_OrnPtr), HL ;3.xx to 3.6x and VTII

	ld		(#_ChanA + CHNPRM_SamPtr), HL ;S1 There is no default
	ld		(#_ChanB + CHNPRM_SamPtr), HL ;S2 sample in PT3, so, you
	ld		(#_ChanC + CHNPRM_SamPtr), HL ;S3 can comment S1,2,3; see
	; also EMPTYSAMORN comment

// Autoplay
#if (PT3_AUTOPLAY)
	jp		_PT3_Resume
	
	// xor		A
	// ld		HL, #_PT3_State
	// ld		(HL), A
	// set		0, (HL)
#endif

__endasm;
}

//-----------------------------------------------------------------------------
/// Pause song playback
void PT3_Pause() __naked
{
__asm
	ld		HL, #_PT3_State       
	res		0, (HL)					// Reset Play flag (stop the music)
	
	jp		_PT3_Silence
__endasm;
}

//-----------------------------------------------------------------------------
/// Resume song playback
void PT3_Resume()
{
__asm
	ld		HL, #_PT3_State       
	set		0, (HL)					// Set Play flag (play the music)
__endasm;
}

//-----------------------------------------------------------------------------
/// Change state of playback loop
/// @param		loop		Either loop or not (TRUE: do loop; FALSE: don't loop)
void PT3_SetLoop(u8 loop)
{
	loop; // A
__asm

	ld		HL, #_PT3_State
	or		A
	jr		NZ, LoopON
	res		4, (HL)					// Reset Loop flag
	ret
  
LoopON:  
	set		4, (HL)					// Set Loop flag

__endasm;
}

//-----------------------------------------------------------------------------
/// Silence the PSG
void PT3_Silence() __naked
{
__asm
	xor		A
	ld		(#AYREGS + PSG_REG_AMP_A), A
	ld		(#AYREGS + PSG_REG_AMP_B), A
	ld		(#AYREGS + PSG_REG_AMP_C), A

	jp		_PT3_UpdatePSG                ;ROUT_A0
__endasm;
}

//-----------------------------------------------------------------------------
/// Send data to PSG registers
/// @note					Must be executed on each V-Blank interruption
void PT3_UpdatePSG()
{
__asm

#if (PT3_EXTRA)

	// Handle mute flags
	ld		HL, #_PT3_State
	xor		A

UpdatePSGMuteA:
	bit		1, (HL)
	jp		z, UpdatePSGMuteB
	ld		(#AYREGS + PSG_REG_AMP_A), A

UpdatePSGMuteB:
	bit		2, (HL)
	jp		z, UpdatePSGMuteC
	ld		(#AYREGS + PSG_REG_AMP_B), A

UpdatePSGMuteC:
	bit		3, (HL)
	jp		z, UpdatePSGMuteEnd
	ld		(#AYREGS + PSG_REG_AMP_C), A

UpdatePSGMuteEnd:

#endif

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
/// Decode a frame from PT3 song
void PT3_Decode() __naked
{
__asm   
	di
	call	decode_start
	ei
	ret

 decode_start:
	ld		HL, #_PT3_State							// Check bit #1 of PT3_State to know if music is playing
	bit		0, (HL)
	ret		Z

	xor		A
	ld		(#_PT3_AddToEn), A						// PT3_AddToEn = 0
	ld		(#AYREGS + PSG_REG_MIXER), A			// PSG_REG_MIXER = 0
	dec		A
	ld		(#AYREGS + PSG_REG_SHAPE), A			// PSG_REG_SHAPE = 0xFF

	LD   HL,#_DelyCnt
	DEC  (HL)
	JP   NZ,PL2

	LD   HL,#_ChanA + CHNPRM_NtSkCn
	DEC  (HL)
	JR   NZ,PL1B

	ld	 BC,(#_PT3_AdInPtA)
	LD   A,(BC)
	AND  A
	JR   NZ,PL1A

	LD   D,A
	LD   (#_Ns_Base),A
	LD   HL,(#_PT3_CrPsPtr)
	INC  HL
	LD   A,(HL)
	INC  A
	JR   NZ,PLNLP

	CALL DecodeCheckLoop

	ld   HL,(#_PT3_LPosPtr)
	LD   A,(HL)
	INC  A
  
PLNLP:	
	LD   (#_PT3_CrPsPtr),HL
	DEC  A
	ADD  A,A
	LD   E,A
	RL   D
	ld   HL,(#_PT3_PatsPtr)
	ADD  HL,DE
	LD   DE,(#_PT3_ModAddr)
	ld   (#_PT3_PSP),SP
	LD   SP,HL
	POP  HL
	ADD  HL,DE
	LD   B,H
	LD   C,L
	POP  HL
	ADD  HL,DE
	LD   (#_PT3_AdInPtB),HL
	POP  HL
	ADD  HL,DE
	LD   (#_PT3_AdInPtC),HL
	ld   SP,(#_PT3_PSP)
  
PL1A:	
	LD   IY,#_ChanA + 12
	CALL PTDECOD
	LD   (#_PT3_AdInPtA),BC
  
PL1B:	
	LD   HL,#_ChanB + CHNPRM_NtSkCn
	DEC  (HL)
	JR   NZ,PL1C
	LD   IY,#_ChanB + 12
	ld   BC,(#_PT3_AdInPtB)
	CALL PTDECOD
	LD   (#_PT3_AdInPtB),BC

PL1C:	
	LD   HL,#_ChanC + CHNPRM_NtSkCn
	DEC  (HL)
	JR   NZ,PL1D
	LD   IY,#_ChanC + 12
	ld   BC,(#_PT3_AdInPtC)
	CALL PTDECOD
	LD   (#_PT3_AdInPtC),BC

PL1D:	
	ld   A,(#_PT3_Delay)
	ld   (#_DelyCnt),A

PL2:	
	LD   IY,#_ChanA
	LD   HL,(#AYREGS + PSG_REG_TONE_A)
	CALL CHREGS
	LD   (#AYREGS + PSG_REG_TONE_A),HL			// Set Tone A
	LD   A,(#AYREGS + PSG_REG_AMP_C)
	LD   (#AYREGS + PSG_REG_AMP_A),A				// Set Volume A
	LD   IY,#_ChanB
	LD   HL,(#AYREGS + PSG_REG_TONE_B)
	CALL CHREGS
	LD   (#AYREGS + PSG_REG_TONE_B),HL			// Set Tone B
	LD   A,(#AYREGS + PSG_REG_AMP_C)
	LD   (#AYREGS + PSG_REG_AMP_B),A				// Set Volume B
	LD   IY,#_ChanC
	LD   HL,(#AYREGS + PSG_REG_TONE_C)
	CALL CHREGS
	LD   (#AYREGS + PSG_REG_TONE_C),HL			// Set Tone C

	LD   HL,(#_Ns_Base)    ;Ns_Base_AddToNs
	LD   A,H
	ADD  A,L
	LD   (#AYREGS + PSG_REG_NOISE),A				// Set Noise

	ld   A,(#_PT3_AddToEn)
	LD   E,A
	ADD  A,A
	SBC  A,A
	LD   D,A
	LD   HL,(#_EnvBase)
	ADD  HL,DE
	LD   DE,(#_CurESld)
	ADD  HL,DE
	LD  (#AYREGS + PSG_REG_ENV),HL

	XOR  A
	LD   HL,#_CurEDel
	OR   (HL)

	RET  Z
	DEC  (HL)
	RET  NZ
	LD   A,(#_PT3_Env_Del)
	LD   (HL),A
	LD   HL,(#_PT3_ESldAdd)
	ADD  HL,DE
	LD   (#_CurESld),HL

  
;  LD   HL,#_PT3_State
;  BIT  0,(HL)   ; pause mode
;  JP   Z,_PT3_Silence
  
	RET

// Check if loop flag is set or stop the music
DecodeCheckLoop:

#if (PT3_EXTRA)
	ld		HL, (_PT3_Finish)
	call	PT3_CALL_HL
#endif

	ld		HL, #_PT3_State
#if (PT3_EXTRA)
	set		5, (HL)					// Set the loop control flag (bit #5)
#endif
	bit		4, (HL)					// Check Loop flag (bit #4) 
	ret		NZ

// No loop: stop the song
// remove the lock if finished the song. Bug #11

	POP		HL
	LD		HL,#_DelyCnt
	INC		(HL)
	LD		HL,#_ChanA + CHNPRM_NtSkCn
	INC		(HL)

	jp		_PT3_Pause

PD_OrSm:	
	LD   -12 + CHNPRM_Env_En(IY),#0
	CALL SETORN
	LD   A,(BC)
	INC  BC
	RRCA

PD_SAM:	
	ADD  A,A
PD_SAM_:	
	LD   E,A
	LD   D,#0
	ld	 HL,(#_PT3_SamPtrs)
	ADD  HL,DE
	LD   E,(HL)
	INC  HL
	LD   D,(HL)
	ld	 HL,(#_PT3_ModAddr)
	ADD  HL,DE
	LD   -12 + CHNPRM_SamPtr(IY),L
	LD   -12 + CHNPRM_SamPtr + 1(IY),H
	JR   PD_LOOP

PD_VOL:	
	RLCA
	RLCA
	RLCA
	RLCA
	LD   -12 + CHNPRM_Volume(IY),A
	JR   PD_LP2
	
PD_EOff:	
	LD   -12 + CHNPRM_Env_En(IY),A
	LD   -12 + CHNPRM_PsInOr(IY),A
	JR   PD_LP2

PD_SorE:	
	DEC  A
	JR   NZ,PD_ENV
	LD   A,(BC)
	INC  BC
	LD   -12 + CHNPRM_NNtSkp(IY),A
	JR   PD_LP2

PD_ENV:	
	CALL SETENV
	JR   PD_LP2

PD_ORN:	
	CALL SETORN
	JR   PD_LOOP
       
PD_ESAM:	
	LD   -12 + CHNPRM_Env_En(IY),A
	LD   -12 + CHNPRM_PsInOr(IY),A
	CALL NZ,SETENV
	LD   A,(BC)
	INC  BC
	JR   PD_SAM_

PTDECOD: 
	LD   A,-12 + CHNPRM_Note(IY)
	LD   (#_PT3_PrNote),A           ;LD   (#PrNote + 1),A
	LD   L,CHNPRM_CrTnSl-12(IY)
	LD   H,CHNPRM_CrTnSl + 1-12(IY)
	LD  (#_PT3_PrSlide),HL

PD_LOOP:	
	ld   DE,#0x2010
PD_LP2:	
	ld   A,(BC)
	inc  BC
	ADD  A,E
	JR   C,PD_OrSm
	ADD  A,D
	JR   Z,PD_FIN
	JR   C,PD_SAM
	ADD  A,E
	JR   Z,PD_REL
	JR   C,PD_VOL
	ADD  A,E
	JR   Z,PD_EOff
	JR   C,PD_SorE
	ADD  A,#96
	JR   C,PD_NOTE
	ADD  A,E
	JR   C,PD_ORN
	ADD  A,D
	JR   C,PD_NOIS
	ADD  A,E
	JR   C,PD_ESAM
	ADD  A,A
	LD   E,A

	LD   HL,#(SPCCOMS + 0xDF20)  ;LD HL,((SPCCOMS + $DF20) % 65536)
;	PUSH DE
;	LD   DE,#0xDF20
;	LD   HL,#SPCCOMS	
;	ADD  HL,DE
;	POP  DE	
  
	ADD  HL,DE
	LD   E,(HL)
	INC  HL
	LD   D,(HL)
	PUSH DE

	JR   PD_LOOP

PD_NOIS:	
	LD   (#_Ns_Base),A
	JR   PD_LP2

PD_REL:	
	RES  0,-12 + CHNPRM_Flags(IY)
	JR   PD_RES
	
PD_NOTE:	
	ld   -12 + CHNPRM_Note(IY),A
	SET  0,-12 + CHNPRM_Flags(IY)
	XOR  A

PD_RES:	
	LD   (#_PT3_PDSP),SP
	LD   SP,IY
	LD   H,A
	LD   L,A
	PUSH HL
	PUSH HL
	PUSH HL
	PUSH HL
	PUSH HL
	PUSH HL
	LD   SP,(#_PT3_PDSP)

PD_FIN:	
	ld   A,-12 + CHNPRM_NNtSkp(IY)
	ld   -12 + CHNPRM_NtSkCn(IY),A
	ret

C_PORTM: 
	RES  2,-12 + CHNPRM_Flags(IY)
	LD   A,(BC)
	INC  BC
  
;SKIP PRECALCULATED TONE DELTA (BECAUSE
;CANNOT BE RIGHT AFTER PT3 COMPILATION)
	INC  BC
	INC  BC
	LD   -12 + CHNPRM_TnSlDl(IY),A
	LD   -12 + CHNPRM_TSlCnt(IY),A
	LD   DE,(#_PT3_NoteTable)
	LD   A,-12 + CHNPRM_Note(IY)
	LD   -12 + CHNPRM_SlToNt(IY),A
	ADD  A,A
	LD   L,A
	LD   H,#0
	ADD  HL,DE
	LD   A,(HL)
	INC  HL
	LD   H,(HL)
	LD   L,A
	PUSH HL
	LD   A,(#_PT3_PrNote)            ;<--- LD   A,#0x3E
	LD   -12 + CHNPRM_Note(IY),A
	ADD  A,A
	LD   L,A
	LD   H,#0
	ADD  HL,DE
	LD   E,(HL)
	INC  HL
	LD   D,(HL)
	POP  HL
	SBC  HL,DE
	LD   -12 + CHNPRM_TnDelt(IY),L
	LD   -12 + CHNPRM_TnDelt + 1(IY),H
	LD   DE,(#_PT3_PrSlide)             ;<--- change to Kun version
	LD   -12 + CHNPRM_CrTnSl(IY),E       ;<---
	LD   -12 + CHNPRM_CrTnSl + 1(IY),D     ;<---
	LD   A,(BC) ;SIGNED TONE STEP
	INC  BC
	EX   AF,AF
	LD   A,(BC)
	INC  BC
	AND  A
	JR   Z,NOSIG
	EX   DE,HL
NOSIG:	
	SBC  HL,DE
	JP   P,SET_STP
	CPL
	EX   AF,AF
	NEG
	EX   AF,AF
SET_STP:	
	LD   -12 + CHNPRM_TSlStp + 1(IY),A
	EX   AF,AF
	ld   -12 + CHNPRM_TSlStp(IY),A
	ld   -12 + CHNPRM_COnOff(IY),#0
	ret

C_GLISS:	
	SET  2,-12 + CHNPRM_Flags(IY)
	LD   A,(BC)
	INC  BC
	LD  -12 + CHNPRM_TnSlDl(IY),A
	LD  -12 + CHNPRM_TSlCnt(IY),A
	LD   A,(BC)
	INC  BC
	EX   AF,AF
	LD   A,(BC)
	INC  BC
	JR   SET_STP

C_SMPOS:	
	LD   A,(BC)
	INC  BC
	LD   -12 + CHNPRM_PsInSm(IY),A
	RET

C_ORPOS:	
	LD   A,(BC)
	INC  BC
	LD   -12 + CHNPRM_PsInOr(IY),A
	RET

C_VIBRT:
	LD   A,(BC)
	INC  BC
	LD   -12 + CHNPRM_OnOffD(IY),A
	LD   -12 + CHNPRM_COnOff(IY),A
	LD   A,(BC)
	INC  BC
	LD   -12 + CHNPRM_OffOnD(IY),A
	XOR  A
	LD   -12 + CHNPRM_TSlCnt(IY),A
	LD   -12 + CHNPRM_CrTnSl(IY),A
	LD   -12 + CHNPRM_CrTnSl + 1(IY),A
	RET

C_ENGLS:
	LD   A,(BC)
	INC  BC
	LD   (#_PT3_Env_Del),A
	LD   (#_CurEDel),A
	LD   A,(BC)
	INC  BC
	LD   L,A
	LD   A,(BC)
	INC  BC
	LD   H,A
	LD   (#_PT3_ESldAdd),HL
	RET

C_DELAY:
	LD   A,(BC)
	INC  BC
	LD   (#_PT3_Delay),A
	RET
	
SETENV:
	LD   -12 + CHNPRM_Env_En(IY),E
	LD   (#AYREGS + PSG_REG_SHAPE),A
	LD   A,(BC)
	INC  BC
	LD   H,A
	LD   A,(BC)
	INC  BC
	LD   L,A
	LD   (#_EnvBase),HL
	XOR  A
	LD   -12 + CHNPRM_PsInOr(IY),A
	LD   (#_CurEDel),A
	LD   H,A
	LD   L,A
	LD   (#_CurESld),HL
  
C_NOP:	
	RET

SETORN:	
	ADD  A,A
	LD   E,A
	LD   D,#0
	LD   -12 + CHNPRM_PsInOr(IY),D
	ld	 HL,(#_PT3_OrnPtrs) 
	ADD  HL,DE
	LD   E,(HL)
	INC  HL
	LD   D,(HL)
	ld   HL,(#_PT3_ModAddr) 
	ADD  HL,DE
	LD   -12 + CHNPRM_OrnPtr(IY),L
	LD   -12 + CHNPRM_OrnPtr + 1(IY),H
	RET





;-------------------------------------------------------------------------------
; ALL 16 ADDRESSES TO PROTECT FROM BROKEN PT3 MODULES 

SPCCOMS: 
.dw C_NOP			  ; ## CHECK THAT IT IS NOT AUTOMODIF
.dw C_GLISS			; (nothing seems to touch it)
.dw C_PORTM
.dw C_SMPOS
.dw C_ORPOS
.dw C_VIBRT
.dw C_NOP
.dw C_NOP
.dw C_ENGLS
.dw C_DELAY
.dw C_NOP
.dw C_NOP
.dw C_NOP
.dw C_NOP
.dw C_NOP
.dw C_NOP


CHREGS:	
	XOR  A
	LD   (#AYREGS + PSG_REG_AMP_C),A				// Set Volume C
	BIT   0,CHNPRM_Flags(IY)
	PUSH  HL
	JP    Z,CH_EXIT
	ld	 (#_PT3_CSP),sp
	LD   L,CHNPRM_OrnPtr(IY)
	LD   H,CHNPRM_OrnPtr + 1(IY)
	LD   SP,HL
	POP  DE
	LD   H,A
	LD   A,CHNPRM_PsInOr(IY)
	LD   L,A
	ADD  HL,SP
	INC  A
	CP   D
	JR   C,CH_ORPS
	LD   A,E
CH_ORPS:	
	LD   CHNPRM_PsInOr(IY),A
	LD   A,CHNPRM_Note(IY)
	ADD  A,(HL)
	JP   P,CH_NTP
	XOR  A
CH_NTP:	
	CP   #96
	JR   C,CH_NOK
	LD   A,#95
CH_NOK:	
	ADD  A,A
	EX   AF,AF
	LD   L,CHNPRM_SamPtr(IY)
	LD   H,CHNPRM_SamPtr + 1(IY)
	LD   SP,HL
	POP  DE
	LD   H,#0
	LD   A,CHNPRM_PsInSm(IY)
	LD   B,A
	ADD  A,A
	ADD  A,A
	LD   L,A
	ADD  HL,SP
	LD   SP,HL
	LD   A,B
	INC  A
	CP   D
	JR   C,CH_SMPS
	LD   A,E
CH_SMPS:	
	LD   CHNPRM_PsInSm(IY),A
	POP  BC
	POP  HL
	LD   E,CHNPRM_TnAcc(IY)
	LD   D,CHNPRM_TnAcc + 1(IY)
	ADD  HL,DE
	BIT  6,B
	JR   Z,CH_NOAC
	LD   CHNPRM_TnAcc(IY),L
	LD   CHNPRM_TnAcc + 1(IY),H
CH_NOAC: 
	EX   DE,HL
	EX   AF,AF
	LD   L,A
	LD   H,#0
	LD   SP,(#_PT3_NoteTable)
	ADD  HL,SP
	LD   SP,HL
	POP  HL
	ADD  HL,DE
	LD   E,CHNPRM_CrTnSl(IY)
	LD   D,CHNPRM_CrTnSl + 1(IY)
	ADD  HL,DE
	ld	 SP,(#_PT3_CSP)
	EX   (SP),HL
	XOR  A
	OR   CHNPRM_TSlCnt(IY)
	JR   Z,CH_AMP
	DEC  CHNPRM_TSlCnt(IY)
	JR   NZ,CH_AMP
	LD   A,CHNPRM_TnSlDl(IY)
	LD   CHNPRM_TSlCnt(IY),A
	LD   L,CHNPRM_TSlStp(IY)
	LD   H,CHNPRM_TSlStp + 1(IY)
	LD   A,H
	ADD  HL,DE
	LD   CHNPRM_CrTnSl(IY),L
	LD   CHNPRM_CrTnSl + 1(IY),H
	BIT  2,CHNPRM_Flags(IY)
	JR   NZ,CH_AMP
	LD   E,CHNPRM_TnDelt(IY)
	LD   D,CHNPRM_TnDelt + 1(IY)
	AND  A
	JR   Z,CH_STPP
	EX   DE,HL
CH_STPP: 
	SBC  HL,DE
	JP   M,CH_AMP
	LD   A,CHNPRM_SlToNt(IY)
	LD   CHNPRM_Note(IY),A
	XOR  A
	LD   CHNPRM_TSlCnt(IY),A
	LD   CHNPRM_CrTnSl(IY),A
	LD   CHNPRM_CrTnSl + 1(IY),A
CH_AMP:	
	LD   A,CHNPRM_CrAmSl(IY)
	BIT  7,C
	JR   Z,CH_NOAM
	BIT  6,C
	JR   Z,CH_AMIN
	CP   #15
	JR   Z,CH_NOAM
	INC  A
	JR   CH_SVAM
CH_AMIN:	
	CP   #-15
	JR   Z,CH_NOAM
	DEC  A
CH_SVAM:	
	LD   CHNPRM_CrAmSl(IY),A
CH_NOAM:	
	LD   L,A
	LD   A,B
	AND  #15
	ADD  A,L
	JP   P,CH_APOS
	XOR  A
CH_APOS:	
	CP   #16
	JR   C,CH_VOL
	LD   A,#15
CH_VOL:	
	OR   CHNPRM_Volume(IY)
	LD   L,A
	LD   H,#0
	LD   DE,#AYREGS  ;_VT_
	ADD  HL,DE
	LD   A,(HL)
CH_ENV:	
	BIT  0,C
	JR   NZ,CH_NOEN
	OR   CHNPRM_Env_En(IY)
CH_NOEN:	
	LD   (#AYREGS + PSG_REG_AMP_C),A				// Set volume C
	BIT  7,B
	LD   A,C
	JR   Z,NO_ENSL
	RLA
	RLA
	SRA  A
	SRA  A
	SRA  A
	ADD  A,CHNPRM_CrEnSl(IY) ;SEE COMMENT BELOW
	BIT  5,B
	JR   Z,NO_ENAC
	LD   CHNPRM_CrEnSl(IY),A
NO_ENAC:	
	ld	 HL,#_PT3_AddToEn 
	ADD  A,(HL) ;BUG IN PT3 - NEED WORD HERE.
	   ;FIX IT IN NEXT VERSION?
	LD   (HL),A
	JR   CH_MIX
NO_ENSL: 
	RRA
	ADD  A,CHNPRM_CrNsSl(IY)
	LD   (#_AddToNs),A
	BIT  5,B
	JR   Z,CH_MIX
	LD   CHNPRM_CrNsSl(IY),A
CH_MIX:	
	LD   A,B
	RRA
	AND  #0x48
CH_EXIT:	
	LD   HL,#AYREGS + PSG_REG_MIXER
	OR   (HL)
	RRCA
	LD   (HL),A
	POP  HL
	XOR  A
	OR   CHNPRM_COnOff(IY)
	RET  Z
	DEC  CHNPRM_COnOff(IY)
	RET  NZ
	XOR  CHNPRM_Flags(IY)
	LD   CHNPRM_Flags(IY),A
	RRA
	LD   A,CHNPRM_OnOffD(IY)
	JR   C,CH_ONDL
	LD   A,CHNPRM_OffOnD(IY)
CH_ONDL:	
	LD   CHNPRM_COnOff(IY),A
	RET

PT3_CALL_HL:
	jp		(hl)

;------------------------------------------------------------------------------- DATAS

EMPTYSAMORN: 
  .db 0,1,0,0x90 
;delete $90 if you dont need default sample  ; # pongo el 0 aqui

__endasm;
}
// ----------------------------------------------------------------------------- END

