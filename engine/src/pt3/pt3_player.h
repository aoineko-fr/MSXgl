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
#pragma once

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

// MSXgl headers
#include "core.h"
#include "psg.h"

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

//ChannelsVars
//struc	CHNPRM
//reset group
#define CHNPRM_PsInOr 0	 //RESB 1
#define CHNPRM_PsInSm 1	 //RESB 1
#define CHNPRM_CrAmSl 2	 //RESB 1
#define CHNPRM_CrNsSl 3	 //RESB 1
#define CHNPRM_CrEnSl 4	 //RESB 1
#define CHNPRM_TSlCnt 5	 //RESB 1
#define CHNPRM_CrTnSl 6	 //RESW 1
#define CHNPRM_TnAcc  8	 //RESW 1
#define CHNPRM_COnOff 10 //RESB 1
//reset group
#define CHNPRM_OnOffD 11 //RESB 1
//IX for PTDECOD here (+12)
#define CHNPRM_OffOnD 12 //RESB 1
#define CHNPRM_OrnPtr 13 //RESW 1
#define CHNPRM_SamPtr 15 //RESW 1
#define CHNPRM_NNtSkp 17 //RESB 1
#define CHNPRM_Note   18 //RESB 1
#define CHNPRM_SlToNt 19 //RESB 1
#define CHNPRM_Env_En 20 //RESB 1
#define CHNPRM_Flags  21 //RESB 1
//Enabled - 0,SimpleGliss - 2
#define CHNPRM_TnSlDl 22 //RESB 1
#define CHNPRM_TSlStp 23 //RESW 1
#define CHNPRM_TnDelt 25 //RESW 1
#define CHNPRM_NtSkCn 27 //RESB 1
#define CHNPRM_Volume 28 //RESB 1
// endstruc
#define CHNPRM_Size   29

// //VARS:
// extern u8  ChanA[CHNPRM_Size];
// extern u8  ChanB[CHNPRM_Size];
// extern u8  ChanC[CHNPRM_Size];			

// extern u8  DelyCnt;
// extern u16 CurESld;		
// extern u8  CurEDel;

// //Ns_Base_AddToNs:
// extern u8  Ns_Base;		
// extern u8  AddToNs;		

extern u8  PT3_Regs[14];
// extern u16 EnvBase;
// extern u8  VAR0END[240];

extern u8 PT3_State;		// Before called PT3_SETUP
//	x	x	LR	LE	MC	MB	MA	PLY
//	7	6	5	4	3	2	1	0	
//			│	│	│	│	│	└── Play Enable
//			│	│	│	│	└────── Mute Channel A [EXTRA]
//			│	│	│	└────────── Mute Channel B [EXTRA]
//			│	│	└────────────── Mute Channel C [EXTRA]
//			│	└────────────────── Loop Enable
//			└────────────────────── Loop Reached [EXTRA]

#define PT3_STATE_PLAY		(1<<0)
#define PT3_STATE_MUTE_A	(1<<1)
#define PT3_STATE_MUTE_B	(1<<2)
#define PT3_STATE_MUTE_C	(1<<3)
#define PT3_STATE_LOOP		(1<<4)
#define PT3_STATE_LOOPED	(1<<5)
#define PT3_STATE_6			(1<<6)
#define PT3_STATE_7			(1<<7)  

extern u16 PT3_ModAddr;		// Song data address
extern u16 PT3_CrPsPtr;		// Cursor position in pattern
extern u16 PT3_SamPtrs;		// Sample info?
extern u16 PT3_OrnPtrs;		// Ornament pattern

extern u16 PT3_PDSP;		// Stack pointer backup
extern u16 PT3_CSP;			// Stack pointer backup
extern u16 PT3_PSP;			// Stack pointer backup
									   
extern u8  PT3_PrNote;		//
extern u16 PT3_PrSlide;		//
  
extern u16 PT3_AdInPtA;		// Play data pattern
extern u16 PT3_AdInPtB;		// Play data
extern u16 PT3_AdInPtC;		// Play data
  
extern u16 PT3_LPosPtr;		// Position Ptr?
extern u16 PT3_PatsPtr;		// Pat Ptr

extern u8  PT3_Delay;		// delay
extern u8  PT3_AddToEn;		// Envelope data (No cal ya que no usa Envs??)
extern u8  PT3_Env_Del;		// Envelope data (idem)
extern u16 PT3_ESldAdd;		// Envelope data (idem)

extern const void* PT3_NoteTable;   // Note table memory address

#if (PT3_EXTRA)

extern u16 PT3_SrtCrPsPtr;	// Cursor position in pattern at start

#endif // (PT3_EXTRA)

extern callback PT3_Finish; // Finish callback

//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

// Group: Core

// Function: PT3_Init
// Initialize the PT3 player
void PT3_Init();

// Function: PT3_InitSong
// Initialize a given song to make it ready to playback
//
// Parameters:
//   songAddr	- Start address of the data. If PT3_SKIP_HEADER is set, this address must be header address + 100 (if the data are not troncated)
void PT3_InitSong(const void* songADDR);

// Function: PT3_Pause
// Pause song playback
void PT3_Pause();

// Function: PT3_Resume
// Resume song playback
void PT3_Resume();

// Function: PT3_SetLoop
// Change state of playback loop
//
// Parameters:
//   loop		- Either loop or not (TRUE: do loop; FALSE: don't loop)
void PT3_SetLoop(bool loop); 

// Function: PT3_Silence
// Silence the PSG
void PT3_Silence();

// Function: PT3_Decode
// Decode a frame from PT3 song
void PT3_Decode(); 

// Function: PT3_UpdatePSG
// Send data to PSG registers
// Must be executed on each V-Blank interruption
void PT3_UpdatePSG();

// Group: Inline

// Function: PT3_SetNoteTable
// Set notes table
//
// Parameters:
//   nt			- Address of the note table to use
inline void PT3_SetNoteTable(const void* nt) { PT3_NoteTable = nt; }

// Function: PT3_GetLoop
// Check if loop flag is set
//
// Returns:
//   FALSE if loop flag is not set
inline bool PT3_GetLoop() { return (PT3_State & PT3_STATE_LOOP); }

// Function: PT3_IsPlaying
// Check if playback flag is set
//
// Returns:
//	 FALSE if playback flag is not set
inline bool PT3_IsPlaying() { return (PT3_State & PT3_STATE_PLAY); }

// Function: PT3_Play
// Play the current music
inline void PT3_Play() { PT3_Resume(); }

// Function: PT3_GetVolume
// Get the current amplitude of a given channel
//
// Parameters:
//	 chan		- The channel number (0: channel A, 1: channel B, 2: channel C)
//
// Returns:
//   Volume in bits #0 to #3 and envelope seting in bit #4
// :	7	6	5	4	3	2	1	0	
// :	x	x	x	M	L3	L2	L1	L0 	
// :  				│	└───┴───┴───┴── Channel A Amplitude (volume)
// :				└────────────────── Volume controlled by Envelope enable/disable
inline u8 PT3_GetVolume(u8 chan) { return PT3_Regs[PSG_REG_AMP_A + chan] & 0x0F; }

// Function: PT3_GetFrequency
// Get the current frequency of a given channel
//
// Parameters:
//	 chan		- The channel number (0: channel A, 1: channel B, 2: channel C)
//
// Returns:
//   12-bits tone period (1 to 4095).
//   Frequency = 111,861 Hz / Period.
//   Reange from 111,861 Hz (divide by 1) down to 27.3 Hz (divide by 4095)
inline u16 PT3_GetFrequency(u8 chan) { return *(u16*)&PT3_Regs[PSG_REG_TONE_A + chan * 2]; }

// Function: PT3_GetPSGRegister
// Get a given PSG register value
//
// Parameters:
//	 reg			- The PSG register number (0-13)
//
// Returns:
//   Value of the given register in the PT3 buffer
inline u8 PT3_GetPSGRegister(u8 reg) { return PT3_Regs[reg]; }

#if (PT3_EXTRA)
	
// Group: Extra

//-----------------------------------------------------------------------------
// Function: PT3_GetPattern
// Get the current pattern number
//
// Returns:
//   Pattern number between 0 and max
inline u8 PT3_GetPattern() { return PT3_CrPsPtr - PT3_SrtCrPsPtr; }

//-----------------------------------------------------------------------------
// Function: PT3_Mute
// Mute a given channel
//
// Parameters:
//	 chan		- The channel number (0: channel A, 1: channel B, 2: channel C)
//	 mute		- Mute (TRUE) or un-mute (FALSE) the given channel
inline void PT3_Mute(u8 chan, bool mute)
{
	if(mute)
		PT3_State |= 1 << (chan + 1);
	else
		PT3_State &= ~(1 << (chan + 1));
}

//-----------------------------------------------------------------------------
// Function: PT3_SetFinishCB
// Set the function to be call when the music ended
//
// Parameters:
//	 cb		- Function to be called when music ends
inline void PT3_SetFinishCB(callback cb) { PT3_Finish = cb;  }

void EmptyCB();
//-----------------------------------------------------------------------------
// Function: PT3_ResetFinishCB
// Reset the function to be call when the music ended
inline void PT3_ResetFinishCB() { PT3_Finish = EmptyCB; }

#endif // (PT3_EXTRA)

