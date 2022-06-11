// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄ ▄▄ ▄ ▄▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ▀█▄▀  ▄█▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▀█  ██  ██▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// WYZ player module
//
// Authors: 
// - MSX PSG proPLAYER V0.3 by WYZ 09.03.2016 https://sites.google.com/site/wyzplayer/home
// - Adapted to SDCC: mvac7/303bcn > <mvac7303b@gmail.com>
//─────────────────────────────────────────────────────────────────────────────

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------
#include "wyz_player.h"

/* =============================================================================
 SDCC WYZ player for MSX
 Version: 1.3 (14/06/2021)
 Author: MSX PSG proPLAYER v0.3 (09.03.2016) by WYZ/Iggy Rock
         Adapted to SDCC: mvac7/303bcn > <mvac7303b@gmail.com>
 Architecture: MSX
 Format: C Object (SDCC .rel)
 Programming language: Assembler and C (SDCC)
 WEB: https://sites.google.com/site/wyzplayer/home
      http://www.cpcwiki.eu/index.php/WYZTracker
      
 Description:
 Adaptation of the WYZ music player for programming in C with the SDCC compiler.
 
 History of versions:
 - 1.3 (14/06/2021) Add WYZ_IsFinished function
 - 1.2 (15/02/2021) same function names in music libraries
 - 1.1 (18/01/2021) same nomenclature for function names on WYZ and Vortex 
                     players #3
 - 1.0 (28/4/2019) 
 - 0.9 (27/4/2013)
============================================================================= */

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

//Internal AY
#define AY0index 0xA0
#define AY0write 0xA1
#define AY0read  0xA2

//External AY
/*
#define AY0index 0x10
#define AY0write 0x11
#define AY0read  0x12*/

// WYZ player status
// 7 6 5 4 3 2 1 0
// │     │   │ │ └── Song loaded?
// │     │   │ └──── Song playing?
// │     │   └────── SFX playing?
// │     └────────── Song looping?
// └──────────────── Song ended?
u8 g_WYZ_State;
u8 g_WYZ_Song;  // Number of the playing song 
u8 g_WYZ_Tempo; // Tempo

u8 TempoCount; // Tempo counter

u16 PointerA; // Channel A pointer
u16 PointerB; // Channel B pointer
u16 PointerC; // Channel C pointer

u16 CANAL_A; // Music starting address A
u16 CANAL_B; // Music starting address B
u16 CANAL_C; // Music starting address C

u16 PointerP_A; //DW PUNTERO PAUTA CANAL A
u16 PointerP_B; //DW PUNTERO PAUTA CANAL B
u16 PointerP_C; //DW PUNTERO PAUTA CANAL C

u16 PointerP_A0; //DW INI PUNTERO PAUTA CANAL A
u16 PointerP_B0; //DW INI PUNTERO PAUTA CANAL B
u16 PointerP_C0; //DW INI PUNTERO PAUTA CANAL C

u16 PointerP_DECA; // Decoder start pointer channel A
u16 PointerP_DECB; // Decoder start pointer channel B
u16 PointerP_DECC; // Decoder start pointer channel C

u16 PointerDECA; // Decoder pointer channel a
u16 PointerDECB; // Decoder pointer channel B
u16 PointerDECC; // Decoder pointer channel C

u8 REG_NOTA_A; // Registration of the note in the channel A
u8 VOL_INST_A; // Relative instrument volume of channel A
u8 REG_NOTA_B; // Registration of the note in the channel B
u8 VOL_INST_B; // Relative instrument volume of channel B
u8 REG_NOTA_C; // Registration of the note in the channel C
u8 VOL_INST_C; // Relative instrument volume of channel C

u16 PointerL_DECA; // Start pointer of the decoder loop channel A
u16 PointerL_DECB; // Start pointer of the decoder loop channel B
u16 PointerL_DECC; // Start pointer of the decoder loop channel C

// Rhythm effects channel - Masks another channel
u16 PointerP; // Channel pointer effects
u16 CANAL_P;  // Direction of onset of effects

u16 PointerP_DECP; // P-Channel decoder start pointer
u16 PointerDECP;   // P-Channel decoder pointer
u16 PointerL_DECP; // Decoder loop start pointer channel P

u16 SFX_L;   // Address buffer effects rhythm effects low register
u16 SFX_H;   // Address buffering rate effects high register
u16 SFX_V;   // Address buffer effects rhythm effects volume register
u16 SFX_MIX; // Address buffer effects rhythm effects mixer register

// Sound effects
u8 N_SONIDO;       // Sound number
u16 PointerSONIDO; // Pointer of the sound being played

// PSG RGISTERS BUFFER
u8 PSG_REG[16];
u8 AYREGS[16];  //PSG_REG_SEC

u8 ENVOLVENTE; // Shape of the envelope
// BIT 0	  : Frequency channel ON/OFF
// BIT 1-2  : Ratio 
// BIT 3-3  : Form
u8 ENVOLVENTE_BACK; // Backup of the envelope shape

u8 SOUND_BUFFER_A[0x10];
u8 SOUND_BUFFER_B[0x10];
u8 SOUND_BUFFER_C[0x10];
u8 SOUND_BUFFER_P[0x10];

// Songs index address
u16 SONG_TABLE;

// Instruments index address
u16 INST_TABLE;

// FXs index address 
u16 EFFECT_TABLE;

// Data of the frequencies of the notes
u16 NOTE_TABLE;

/*
u16 NOTE_TABLE[]={0,0,
1711,1614,1524,1438,1358,1281,1210,1142,1078,1017,
960,906,855,807,762,719,679,641,605,571,
539,509,480,453,428,404,381,360,339,320,
302,285,269,254,240,227,214,202,190,180,
170,160,151,143,135,127,120,113,107,101,
95,90,85,80,76,71,67,64,60,57};
*/

// Rhythm effect channel selector data table

u8 SELECT_CANAL_A[7]; // Assign values in the init
// DW	PSG_REG_SEC+0,PSG_REG_SEC+1,PSG_REG_SEC+8
// DB	10110001B

u8 SELECT_CANAL_B[7]; 
//	DW	PSG_REG_SEC+2,PSG_REG_SEC+3,PSG_REG_SEC+9
//  DB	10101010B

u8 SELECT_CANAL_C[7]; 
//	DW	PSG_REG_SEC+4,PSG_REG_SEC+5,PSG_REG_SEC+10
//  DB	10011100B

// TABLA_DATOS_CANAL_SFX:
//DW	SELECT_CANAL_A,SELECT_CANAL_B,SELECT_CANAL_C
u8 TABLA_DATOS_CANAL_SFX[6];

//-----------------------------------------------------------------------------
// Initialize the Player
void WYZ_Initialize(u16 song, u16 inst, u16 fx, u16 freq) __sdcccall(0)
{
	song; // IX+4
	inst; // IX+6
	fx;   // IX+8
	freq; // IX+10
__asm
	push	IX
	ld		IX, #0
	add		IX, SP

	ld		L, 4(IX)
	ld		H, 5(IX)
	ld		(#_SONG_TABLE), HL

	ld		L, 6(IX)
	ld		H, 7(IX)
	ld		(#_INST_TABLE), HL

	ld		L, 8(IX)
	ld		H, 9(IX)
	ld		(#_EFFECT_TABLE), HL

	ld		L, 10(IX)
	ld		H, 11(IX)
	ld		(#_NOTE_TABLE), HL

	call	_WYZ_Pause		// PLAYER_OFF

	ld		HL, #_g_WYZ_State
	ld		(HL), #0x01				// Set state to Song loaded

	ld		HL, #_SOUND_BUFFER_A	// Reserve memory for sound buffering!!!!!
	ld		(#_CANAL_A), HL

	ld		HL, #_SOUND_BUFFER_B        	
	ld		(#_CANAL_B), HL       	

	ld		HL, #_SOUND_BUFFER_C       	
	ld		(#_CANAL_C), HL 

	ld		HL, #_SOUND_BUFFER_P       	
	ld		(#_CANAL_P), HL

// Rhythm effect channel selector data table
	//--- chan A
	ld 		HL, #_AYREGS+PSG_REG_TONE_A       	
	ld 		(#_SELECT_CANAL_A), HL

	ld 		HL, #_AYREGS+PSG_REG_TONE_A+1       	
	ld 		(#_SELECT_CANAL_A+2), HL

	ld 		HL, #_AYREGS+PSG_REG_AMP_A       	
	ld 		(#_SELECT_CANAL_A+4), HL

	ld 		A, #0b10110001
	ld 		(#_SELECT_CANAL_A+6), A

	//--- chan B
	ld		HL, #_AYREGS+PSG_REG_TONE_B       	
	ld		(#_SELECT_CANAL_B), HL

	ld		HL, #_AYREGS+PSG_REG_TONE_B+1       	
	ld		(#_SELECT_CANAL_B+2), HL

	ld		HL, #_AYREGS+PSG_REG_AMP_B       	
	ld		(#_SELECT_CANAL_B+4), HL

	ld		A, #0b10101010
	ld		(#_SELECT_CANAL_B+6), A

	//--- chan C
	ld		HL, #_AYREGS+PSG_REG_TONE_C       	
	ld		(#_SELECT_CANAL_C), HL

	ld		HL, #_AYREGS+PSG_REG_TONE_C+1       	
	ld		(#_SELECT_CANAL_C+2), HL

	ld		HL, #_AYREGS+PSG_REG_AMP_C       	
	ld		(#_SELECT_CANAL_C+4), HL

	ld		A, #0b10011100
	ld		(#_SELECT_CANAL_C+6), A

	// TABLA_DATOS_CANAL_SFX
	ld		HL, #_SELECT_CANAL_A        	
	ld		(#_TABLA_DATOS_CANAL_SFX), HL

	ld		HL, #_SELECT_CANAL_B        	
	ld		(#_TABLA_DATOS_CANAL_SFX+2), HL

	ld		HL, #_SELECT_CANAL_C        	
	ld		(#_TABLA_DATOS_CANAL_SFX+4), HL

	pop		IX
    		
__endasm;
}

//-----------------------------------------------------------------------------
// Pause song playback
void WYZ_Pause() __naked
{
__asm
	// xor		A			
	// ld		[INTERR],A
	// ld		[FADE],A		// Only if there is a fade out
	ld		HL, #_g_WYZ_State       
	res		1, (HL) // Playing OFF

// CLEAR_PSG_BUFFER:
	xor		A
	ld		HL, #_PSG_REG
	ld		DE, #_PSG_REG+1
	ld		BC, #14
	ld		(HL), A
	ldir

	ld		A, #0b10111000		// **** Just in case ****
	ld		(#_PSG_REG+PSG_REG_MIXER), A

	ld		HL, #_PSG_REG
	ld		DE, #_AYREGS
	ld		BC, #14
	ldir		

	jp		_WYZ_PlayAY
   
__endasm;
}

//-----------------------------------------------------------------------------
// Resume song playback
void WYZ_Resume()
{
__asm
	ld		HL, #_g_WYZ_State       
	set		1, (HL)      //  Playing ON
__endasm;
}

//-----------------------------------------------------------------------------
// Indicates whether the song has finished playing
bool WYZ_IsFinished()
{
__asm
	xor		A
	ld		HL, #_g_WYZ_State
	bit		7, (HL)
	jr		Z, retPlayerEndState
	inc		A
    
retPlayerEndState:    
	ld		L, A
__endasm;
}

//-----------------------------------------------------------------------------
// Change loop mode
void WYZ_SetLoop(bool loop)
{
	loop; // A
__asm
	ld		HL, #_g_WYZ_State
	or		A
	jr		Z, resetLoop

setLoop:           
	set		4, (HL) // Loop ON
	ret

resetLoop:           
	res		4, (HL) // Loop OFF
__endasm;
}

//-----------------------------------------------------------------------------
// Play Sound Effect
void WYZ_PlayFX(u8 numSound)
{
	numSound; // A
__asm
	// cp		8		// SFX speech
	// jp		Z, SLOOP

	ld      HL, (#_EFFECT_TABLE)    //(v SDCC) HL,EFFECT_TABLE  
	call    EXT_WORD
	ld      (#_PointerSONIDO), HL
	
	ld      HL, #_g_WYZ_State
	set     2, (HL) // SFX ON
    
__endasm;
}

//-----------------------------------------------------------------------------
// Send data from AYREGS buffer to AY registers.
// Execute on each interruption of VBLANK
void WYZ_PlayAY()
{
__asm
//Record register 7 of the AY38910 ----------------------------------------------
//collects the last two bits for joysctick port control
//and adds them to the value of the mixer bits
	ld		A, (#_AYREGS+PSG_REG_MIXER)
	and		#0b00111111
	ld		B, A

	ld		A, #PSG_REG_MIXER
	out		(#AY0index), A
	in		A, (#AY0read)  
	and		#0b11000000	// Mask to catch only the two bits of joys 
	or		B		        // Add B

	ld		(#_AYREGS+PSG_REG_MIXER), A
//END register 7 ----------------------------------------------------------------

//   LD   A,(#_PSG_REG+PSG_REG_SHAPE)			
//   AND  A			//ES CERO?
//   JR   Z,NO_BACKUP_ENVOLVENTE
//   LD   (#_ENVOLVENTE_BACK),A	//08.13 / GUARDA LA ENVOLVENTE EN EL BACKUP
NO_BACKUP_ENVOLVENTE:
	xor		A
	ld		C, #AY0index   //0xA0
	ld		HL, #_AYREGS
   
playAYLoop:
	out		(C), A
	inc		C
	outi 
	dec		C
	inc		A
	cp		#13
	jr		NZ, playAYLoop

	out		(C), A
	ld		A, (HL)
	and		A
	ret		Z

	inc		C
	out		(C), A
	ld		(#_ENVOLVENTE_BACK), A

	xor		A
	ld		(#_AYREGS+PSG_REG_SHAPE), A
	ld		(#_PSG_REG+PSG_REG_SHAPE), A

__endasm;
}

//-----------------------------------------------------------------------------
// Initialize song
void WYZ_PlaySong(u8 numSong, bool loop) __sdcccall(0)
{
	numSong;
	loop;
__asm
	push	IX
	ld		IX, #0
	add		IX, SP

	ld		A, 5(IX)
	call	_WYZ_SetLoop

	LD		A, 4(IX)
	call	loadSong

	pop		IX
	ret

// LOAD A SONG
// IN:[A]=Song number
loadSong:
   	ld		(#_g_WYZ_Song), A		// N° A

	ld		HL,#_g_WYZ_State		// Song	state
	set		1,(HL)					// Set song playing ON
	res		7,(HL)					// Set song ended OFF

// DECODE SONG
// IN-> g_WYZ_State 0 ON
//      g_WYZ_Song

// LOAD SONG YES/NO
decodeSong:
	ld		A, (#_g_WYZ_Song)

// READ SONG HEADER
//BYTE 0=g_WYZ_Tempo
	ld		HL, (#_SONG_TABLE)   //(v SDCC) HL,_SONG_TABLE
	call	EXT_WORD
	ld		A, (HL)
	ld		(#_g_WYZ_Tempo), A
	dec		A
	ld		(#_TempoCount), A

//HEADER BYTE 1
//[-|-|-|-|  3-1 | 0  ]
//[-|-|-|-|FX CHN|LOOP]

	inc		HL     //LOOP 1=ON/0=OFF?
	ld		A, (HL)
	// bit		0, A
	// jr		Z, NPTJP0

	// push		HL
	// ld		HL, #_g_WYZ_State
	// set		4, (HL)
	// pop		HL

// Selection of the rhythm effect channel
NPTJP0:
	and		#0b00000110 
	rra

	push	HL
	ld		HL, #_TABLA_DATOS_CANAL_SFX
	call	EXT_WORD
	push	HL
	pop		IX
	ld		E, 0(IX)
	ld		D, 1(IX)
	ld		(#_SFX_L), DE

	ld		E, 2(IX)
	ld		D, 3(IX)
	ld		(#_SFX_H), DE

	ld		E, 4(IX)
	ld		D, 5(IX)
	ld		(#_SFX_V), DE

	ld		A, 6(IX)
	ld		(#_SFX_MIX), A
	pop		HL

	inc		HL // 2 bytes reserved
	inc		HL
	inc		HL

// Search and save start of channels in the mus module (Optimize)
	push	HL						// IX start offsets loop per canal
	pop		IX

	ld		DE, #0x0008				// To start of channel A
	add		HL, DE

	ld		(#_PointerP_DECA), HL	// Saves channel start pointer
	ld		E, (IX)
	ld		D, 1(IX)
	add		HL, DE
	ld		(#_PointerL_DECA), HL	// Saves pointer start loop

	call	BGICMODBC1
	ld		(#_PointerP_DECB), HL
	ld		E, 2(IX)
	ld		D, 3(IX)
	add		HL, DE
	ld		(#_PointerL_DECB), HL

	call	BGICMODBC1
	ld		(#_PointerP_DECC), HL
	ld		E, 4(IX)
	ld		D, 5(IX)
	add		HL, DE
	ld		(#_PointerL_DECC), HL

	call	BGICMODBC1
	ld		(#_PointerP_DECP), HL
	ld		E, 6(IX)
	ld		D, 7(IX)
	add		HL, DE
	ld		(#_PointerL_DECP), HL

// Read data from the notes
//[|][|||||] LONGITUD\NOTA
INIT_DECODER:
	ld		DE, (#_CANAL_A)
	ld		(#_PointerA), DE
	ld		HL, (#_PointerP_DECA)
	call	DECODE_CANAL    	// Canal A
	ld		(#_PointerDECA), HL

	ld		DE, (#_CANAL_B)
	ld		(#_PointerB), DE
	ld		HL, (#_PointerP_DECB)
	call	DECODE_CANAL    	// Canal B
	ld		(#_PointerDECB), HL

	ld		DE, (#_CANAL_C)
	ld		(#_PointerC), DE
	ld		HL, (#_PointerP_DECC)
	call	DECODE_CANAL    	// Canal C
	ld		(#_PointerDECC), HL

	ld		DE, (#_CANAL_P)
	ld		(#_PointerP), DE
	ld		HL, (#_PointerP_DECP)
	call	DECODE_CANAL    	// Canal P (FX)
	ld		(#_PointerDECP), HL

	ret

// Search channel start
BGICMODBC1:
	xor		A						// Search for byte 0
	ld		E, #0x3F				// Instrument code 0
	ld		B, #0xFF				// The module must have a length of less than $FF00... o_O!
	cpir

	dec		HL
	dec		HL
	ld		A, E					// Is instrument 0?
	cp		(HL)
	inc		HL
	inc		HL
	jr		Z, BGICMODBC1

	dec		HL
	dec		HL
	dec		HL
	ld		A, E					// Is volume 0?
	cp		(HL)
	inc		HL
	inc		HL
	inc		HL
	jr		Z, BGICMODBC1
	// ret

__endasm;
}

//-----------------------------------------------------------------------------
// Process the next step in the song sequence 
void WYZ_Decode()
{
__asm

INICIO:
	push	IX
	// call	ROUT
	// call	MIXER

	ld		HL, #_PSG_REG
	ld		DE, #_AYREGS
	ld		BC, #14
	ldir

	call	REPRODUCE_SONIDO
	call	PLAY

	pop		IX
	ret

// Plays sound effects
REPRODUCE_SONIDO:
	ld		HL, #_g_WYZ_State
	bit		2, (HL)   // Is the effect activated?
	ret		Z
	ld		HL, (#_PointerSONIDO)
	ld		A, (HL)
	cp		#0xFF
	jr		Z, FIN_SONIDO
	ld		DE, (#_SFX_L)
	ld		(DE), A
	inc		HL
	ld		A, (HL)
	rrca
	rrca
	rrca
	rrca
	and		#0b00001111
	ld		DE, (#_SFX_H)
	ld		(DE), A
	ld		A, (HL)
	and		#0b00001111
	ld		DE, (#_SFX_V)
	ld		(DE), A

	inc		HL
	ld		A, (HL)
	ld		B, A
	bit		7, A					// Most siginificant bit active surroundings
	jr		Z, NO_ENVOLVENTES_SONIDO
	ld		A, #0x12
	ld		(DE), A
	inc		HL
	ld		A, (HL)
	ld		(#_AYREGS+PSG_REG_ENV), A
	inc		HL
	ld		A, (HL)
	ld		(#_AYREGS+PSG_REG_ENV+1), A
	inc		HL
	ld		A, (HL)
	cp		#1
	jr		Z, NO_ENVOLVENTES_SONIDO // Does not write the envelope if its value is 1
	ld		(#_AYREGS+PSG_REG_SHAPE), A

NO_ENVOLVENTES_SONIDO:
	ld		A, B
	res		7, A
	and		A
	jr		Z, NO_RUIDO
	ld		(#_AYREGS+PSG_REG_NOISE), A
	ld		A, (#_SFX_MIX)
	jr		SI_RUIDO

NO_RUIDO:
	XOR  A
	LD   (#_AYREGS+PSG_REG_NOISE),A
	LD   A,#0b10111000

SI_RUIDO:
	LD   (#_AYREGS+PSG_REG_MIXER),A   
	INC  HL
	LD   (#_PointerSONIDO),HL
	RET

FIN_SONIDO:
	LD   HL,#_g_WYZ_State
	RES  2,(HL)
	LD   A,(#_ENVOLVENTE_BACK)		//NO RESTAURA LA ENVOLVENTE SI ES 0
	AND  A
	JR   Z,FIN_NOPLAYER
	//xor  a 
	LD   (#_AYREGS+PSG_REG_SHAPE),A			//08.13 RESTAURA LA ENVOLVENTE TRAS EL SFX

FIN_NOPLAYER:
   LD   A,#0b10111000
   LD   (#_AYREGS+PSG_REG_MIXER),A

   RET

PLAY:
   LD   HL,#_g_WYZ_State       //PLAY BIT 1 ON?
   BIT  1,(HL)
   RET  Z
   
//TEMPO
   LD   HL,#_TempoCount       //CONTADOR TEMPO
   INC  (HL)
   LD   A,(#_g_WYZ_Tempo)
   CP   (HL)
   JR   NZ,PAUTAS
   LD   (HL),#0

//INTERPRETA
   LD   IY,#_PSG_REG
   LD   IX,#_PointerA
   LD   BC,#_PSG_REG+PSG_REG_AMP_A
   CALL LOCALIZA_NOTA
   LD   IY,#_PSG_REG+PSG_REG_TONE_B
   LD   IX,#_PointerB
   LD   BC,#_PSG_REG+PSG_REG_AMP_B
   CALL LOCALIZA_NOTA
   LD   IY,#_PSG_REG+PSG_REG_TONE_C
   LD   IX,#_PointerC
   LD   BC,#_PSG_REG+PSG_REG_AMP_C
   CALL LOCALIZA_NOTA
   LD   IX,#_PointerP    //EL CANAL DE EFECTOS ENMASCARA OTRO CANAL
   CALL LOCALIZA_EFECTO

PAUTAS:
   LD   IY,#_PSG_REG+PSG_REG_TONE_A
   LD   IX,#_PointerP_A
   LD   HL,#_PSG_REG+PSG_REG_AMP_A
   CALL PAUTA    //PAUTA CANAL A
   LD   IY,#_PSG_REG+PSG_REG_TONE_B
   LD   IX,#_PointerP_B
   LD   HL,#_PSG_REG+PSG_REG_AMP_B
   CALL PAUTA    //PAUTA CANAL B
   LD   IY,#_PSG_REG+PSG_REG_TONE_C
   LD   IX,#_PointerP_C
   LD   HL,#_PSG_REG+PSG_REG_AMP_C
   CALL PAUTA    //PAUTA CANAL C

   RET

//DECODIFICA NOTAS DE UN CANAL
//IN (DE)=DIRECCION DESTINO
//NOTA=0 FIN CANAL
//NOTA=1 SILENCIO
//NOTA=2 PUNTILLO
//NOTA=3 COMANDO I

DECODE_CANAL:
   LD      A,(HL)
   AND     A        //FIN DEL CANAL?
   JR      Z,FIN_DEC_CANAL
   CALL    GETLEN

   CP      #0b00000001       //ES SILENCIO?
   JR      NZ,NO_SILENCIO
   SET     6,A
   JR      NO_MODIFICA
         
NO_SILENCIO:
   CP      #0b00111110       //ES PUNTILLO?
   JR      NZ,NO_PUNTILLO
   OR      A
   RRC     B
   XOR     A
   JR      NO_MODIFICA

NO_PUNTILLO:
   CP      #0b00111111       //ES COMANDO?
   JR      NZ,NO_MODIFICA
   BIT     0,B      //COMADO=INSTRUMENTO?
   JR      Z,NO_INSTRUMENTO   
   LD      A,#0b11000001     //CODIGO DE INSTRUMENTO      
   LD      (DE),A
   INC     HL
   INC     DE
   LD      A,(HL)   //N� DE INSTRUMENTO
   LD      (DE),A
   INC     DE
   INC     HL
   LD      A,(HL)   //VOLUMEN RELATIVO DEL INSTRUMENTO
   LD      (DE),A
   INC     DE
   INC     HL
   JR      DECODE_CANAL

NO_INSTRUMENTO:
   BIT     2,B
   JR      Z,NO_ENVOLVENTE
   LD      A,#0b11000100     //CODIGO ENVOLVENTE
   LD      (DE),A
   INC     DE
   INC     HL
   LD      A,(HL)
   LD      (DE),A
   INC     DE
   INC     HL
   JR      DECODE_CANAL
     
NO_ENVOLVENTE:
   BIT     1,B
   JR      Z,NO_MODIFICA    
   LD      A,#0b11000010     //CODIGO EFECTO
   LD      (DE),A           
   INC     HL        
   INC     DE        
   LD      A,(HL)           
   CALL    GETLEN   

NO_MODIFICA:
   LD      (DE),A
   INC     DE
   XOR     A
   DJNZ    NO_MODIFICA
   SET     7,A
   SET     0,A
   LD      (DE),A
   INC     DE
   INC     HL
   RET    //** JR      DECODE_CANAL

FIN_DEC_CANAL:
   SET     7,A
   LD      (DE),A
   INC     DE
   RET

GETLEN:
   LD      B,A
   AND     #0b00111111
   PUSH    AF
   LD      A,B
   AND     #0b11000000
   RLCA
   RLCA
   INC     A
   LD      B,A
   LD      A,#0b10000000
DCBC0:
  RLCA
  DJNZ DCBC0
  LD   B,A
  pop  AF
  ret

//LOCALIZA NOTA CANAL A
//IN [PointerA]

//LOCALIZA NOTA CANAL A
//IN [PointerA]

LOCALIZA_NOTA:
   LD      L,_PointerA - _PointerA(IX)	  //HL=[PointerA_C_B] 
   LD      H,_PointerA - _PointerA+1(IX)  //
   LD      A,(HL)
   AND     #0b11000000       //COMANDO?
   CP      #0b11000000
   JR      NZ,LNJP0

//BIT[0]=INSTRUMENTO
   
COMANDOS:
   LD      A,(HL)
   BIT     0,A      //INSTRUMENTO
   JR      Z,COM_EFECTO

   INC     HL
   LD      A,(HL)   //N° DE PAUTA
   INC     HL
   LD      E,(HL)

   PUSH    HL       //TEMPO
   LD      HL,#_g_WYZ_Tempo
   BIT     5,E
   JR      Z,NO_DEC_TEMPO
   DEC     (HL)
NO_DEC_TEMPO:
   BIT     6,E
   JR      Z,NO_INC_TEMPO
   INC     (HL)
NO_INC_TEMPO:
   RES     5,E    //SIEMPRE RESETEA LOS BITS DE TEMPO
   RES     6,E
   POP     HL
   
   LD      _VOL_INST_A - _PointerA(IX),E	//REGISTRO DEL VOLUMEN RELATIVO
   INC     HL
   LD      _PointerA - _PointerA(IX),L   //
   LD      _PointerA - _PointerA+1(IX),H   //
   LD      HL,(#_INST_TABLE)   //(v sdcc) HL,#_INST_TABLE
   CALL    EXT_WORD
   LD      _PointerP_A0 - _PointerA(IX),L
   LD      _PointerP_A0 - _PointerA+1(IX),H
   LD      _PointerP_A - _PointerA(IX),L
   LD      _PointerP_A - _PointerA+1(IX),H
   LD      L,C
   LD      H,B
   RES     4,(HL)     //APAGA EFECTO ENVOLVENTE
   XOR     A
   LD      (#_AYREGS+PSG_REG_SHAPE),A
   LD      (#_PSG_REG+PSG_REG_SHAPE),A
   //LD     [ENVOLVENTE_BACK],A   //08.13 RESETEA EL BACKUP DE LA ENVOLVENTE
   JR      LOCALIZA_NOTA

COM_EFECTO:
   BIT     1,A   //EFECTO DE SONIDO
   JR      Z,COM_ENVOLVENTE

   INC     HL
   LD      A,(HL)
   INC     HL
   LD      _PointerA - _PointerA(IX),L  //
   LD      _PointerA - _PointerA+1(IX),H  //
   CALL    _WYZ_PlayFX
   RET

COM_ENVOLVENTE:
   BIT  2,A
   RET  Z       //IGNORA - ERROR

   INC  HL
   LD   A,(HL)   //CARGA CODIGO DE ENVOLVENTE
   LD   (#_ENVOLVENTE),A
   INC  HL
   LD   _PointerA - _PointerA(IX),L   //
   LD   _PointerA - _PointerA+1(IX),H   //
   LD   L,C
   LD   H,B
   LD   (HL),#0b00010000    //ENCIENDE EFECTO ENVOLVENTE
   JR   LOCALIZA_NOTA
   
LNJP0:   
   LD   A,(HL)
   INC  HL
   BIT  7,A
   JR   Z,NO_FIN_CANAL_A	//
   BIT  0,A
   JR   Z,FIN_CANAL_A

FIN_NOTA_A:
   LD   E,_CANAL_A - _PointerA(IX)
   LD   D,_CANAL_A - _PointerA+1(IX)	//PUNTERO BUFFER AL INICIO
   LD   _PointerA - _PointerA(IX),E  //
   LD   _PointerA - _PointerA+1(IX),D  //
   LD   L,_PointerDECA - _PointerA(IX)	//CARGA PUNTERO DECODER
   LD   H,_PointerDECA - _PointerA+1(IX)
   PUSH BC
   CALL DECODE_CANAL      //DECODIFICA CANAL
   POP  BC
   LD   _PointerDECA - _PointerA(IX),L	//GUARDA PUNTERO DECODER
   LD   _PointerDECA - _PointerA+1(IX),H
   JP   LOCALIZA_NOTA
   
FIN_CANAL_A:
   LD   HL,#_g_WYZ_State     //LOOP?
   BIT  4,(HL)
   JR   NZ,FCA_CONT
   POP  AF
   
   SET  7,(HL)           //End song
   JP   _WYZ_Pause

FCA_CONT:
   LD   L,_PointerL_DECA - _PointerA(IX)	//CARGA PUNTERO INICIAL DECODER
   LD   H,_PointerL_DECA - _PointerA+1(IX)
   LD   _PointerDECA - _PointerA(IX),L
   LD   _PointerDECA - _PointerA+1(IX),H
   JR   FIN_NOTA_A
   
NO_FIN_CANAL_A:
   LD   _PointerA - _PointerA(IX),L   //[PointerA_B_C]=HL GUARDA PUNTERO   
   LD   _PointerA - _PointerA+1(IX),H   //
   AND  A    //NO REPRODUCE NOTA SI NOTA=0
   JR   Z,FIN_RUTINA
   BIT  6,A      		//SILENCIO?
   JR   Z,NO_SILENCIO_A
   LD   A,(BC)
   AND	 #0b00010000
   JR   NZ,SILENCIO_ENVOLVENTE
   
   XOR  A
   LD   (BC),A				//RESET VOLUMEN DEL CORRESPODIENTE CHIP
   LD   0(IY),A
   LD   1(IY),A
   RET

SILENCIO_ENVOLVENTE:
   LD   A,#0xFF
   LD   (#_PSG_REG+PSG_REG_ENV),A
   LD   (#_PSG_REG+PSG_REG_ENV+1),A
   XOR	A
   LD   (#_PSG_REG+PSG_REG_SHAPE),A
   LD   0(IY),A
   LD   1(IY),A
   RET

NO_SILENCIO_A:
   LD   _REG_NOTA_A - _PointerA(IX),A	//REGISTRO DE LA NOTA DEL CANAL
   CALL NOTA     		//REPRODUCE NOTA
   LD   L,_PointerP_A0 - _PointerA(IX)     //HL=[PointerP_A0] RESETEA PAUTA 
   LD   H,_PointerP_A0 - _PointerA+1(IX)
   LD   _PointerP_A - _PointerA(IX),L       //[PointerP_A]=HL
   LD   _PointerP_A - _PointerA+1(IX),H
FIN_RUTINA:
   RET


//LOCALIZA EFECTO
//IN HL=[PointerP]
LOCALIZA_EFECTO:
   LD      L,0(IX)       //HL=[PointerP]
   LD      H,1(IX)
   LD      A,(HL)
   CP      #0b11000010
   JR      NZ,LEJP0

   INC     HL
   LD      A,(HL)
   INC     HL
   LD      0(IX),L
   LD      1(IX),H
   CALL    _WYZ_PlayFX
   RET


LEJP0:
   INC  HL
   BIT  7,A
   JR   Z,NO_FIN_CANAL_P	//
   BIT  0,A
   JR   Z,FIN_CANAL_P

FIN_NOTA_P:
   LD   DE,(#_CANAL_P)
   LD   0(IX),E
   LD   1(IX),D
   LD   HL,(#_PointerDECP)	//CARGA PUNTERO DECODER
   PUSH BC
   CALL DECODE_CANAL    	//DECODIFICA CANAL
   POP  BC
   LD   (#_PointerDECP),HL	//GUARDA PUNTERO DECODER
   JP   LOCALIZA_EFECTO
   
FIN_CANAL_P:
   LD   HL,(#_PointerL_DECP)	//CARGA PUNTERO INICIAL DECODER
   LD   (#_PointerDECP),HL
   JR   FIN_NOTA_P
   
NO_FIN_CANAL_P:
   LD   0(IX),L        //[PointerA_B_C]=HL GUARDA PUNTERO
   LD   1(IX),H
   RET



// PAUTA DE LOS 3 CANALES
// IN:[IX]:PUNTERO DE LA PAUTA
//    (HL):REGISTRO DE VOLUMEN
//    [IY]:REGISTROS DE FRECUENCIA

// FORMATO PAUTA	
//	    7    6     5     4   3-0    3-0  
// BYTE 1 [LOOP|OCT-1|OCT+1|ORNMT|VOL] - BYTE 2 [ | | | |PITCH/NOTA]

PAUTA:
   BIT  4,(HL)        //SI LA ENVOLVENTE ESTA ACTIVADA NO ACTUA PAUTA
   RET  NZ

   LD   A,0(IY)
   LD   B,1(IY)
   OR   B
   RET  Z


   PUSH HL
    
PCAJP4:
   LD   L,0(IX)
   LD   H,1(IX)   
   LD   A,(HL)

   BIT  7,A		//LOOP / EL RESTO DE BITS NO AFECTAN
   JR   Z,PCAJP0
   AND  #0b00011111       //M�XIMO LOOP PAUTA [0,32]X2!!!-> PARA ORNAMENTOS
   RLCA    //X2
   LD   D,#0
   LD   E,A
   SBC  HL,DE
   LD   A,(HL)

PCAJP0:
   BIT  6,A   //OCTAVA -1
   JR   Z,PCAJP1
   LD   E,0(IY)
   LD   D,1(IY)

   AND  A
   RRC  D
   RR   E
   LD   0(IY),E
   LD   1(IY),D
   JR   PCAJP2

PCAJP1:
   BIT  5,A		//OCTAVA +1
   JR   Z,PCAJP2
   LD   E,0(IY)
   LD   D,1(IY)
   AND  A
   RLC  E
   RL   D
   LD   0(IY),E
   LD   1(IY),D		

PCAJP2:
   LD   A,(HL)
   BIT  4,A
   JR   NZ,PCAJP6	//ORNAMENTOS SELECCIONADOS

//FUNCION PITCH DE FRECUENCIA
   INC  HL
   PUSH HL
   LD   E,A
   LD   A,(HL)		//PITCH DE FRECUENCIA
   LD   L,A
   AND  A
   LD   A,E
   JR   Z,ORNMJP1

   LD   A,0(IY)    //SI LA FRECUENCIA ES 0 NO HAY PITCH
   ADD  A,1(IY)
   AND  A
   LD   A,E
   JR   Z,ORNMJP1

   BIT  7,L
   JR   Z,ORNNEG
   LD   H,#0xFF
   JR   PCAJP3

ORNNEG:
   LD   H,#0

PCAJP3:
   LD   E,0(IY)
   LD   D,1(IY)
   ADC  HL,DE
   LD   0(IY),L
   LD   1(IY),H
   JR   ORNMJP1

//FUNCION ORNAMENTOS
PCAJP6:		
   INC  HL
   PUSH HL
   PUSH AF
   LD   A,_REG_NOTA_A - _PointerP_A(IX)	//RECUPERA REGISTRO DE NOTA EN EL CANAL
   LD   E,(HL)		//
   ADC  E		//+- NOTA 
   CALL TABLA_NOTAS
   POP  AF	

ORNMJP1:
   POP  HL
   INC  HL
   LD   0(IX),L
   LD   1(IX),H
PCAJP5:
   POP  HL
   LD   B,_VOL_INST_A - _PointerP_A(IX)	//VOLUMEN RELATIVO
   ADD  B
   JP   P,PCAJP7
   LD   A,#1		//NO SE EXTIGUE EL VOLUMEN
PCAJP7:
   AND  #0b00001111   //VOLUMEN FINAL MODULADO
   LD   (HL),A
   RET

//NOTA : REPRODUCE UNA NOTA
//IN [A]=CODIGO DE LA NOTA
//   [IY]=REGISTROS DE FRECUENCIA
NOTA:
   LD    L,C
   LD    H,B
   BIT   4,(HL)
   LD    B,A
   JR    NZ,EVOLVENTES
   LD    A,B
TABLA_NOTAS:
   LD    HL,(#_NOTE_TABLE)     //BUSCA FRECUENCIA
   CALL  EXT_WORD
   LD    0(IY),L
   LD    1(IY),H
   RET

//IN [A]=CODIGO DE LA ENVOLVENTE
//   [IY]=REGISTRO DE FRECUENCIA
EVOLVENTES:
    LD    HL,(#_NOTE_TABLE)
    //SUB	12
    RLCA      //X2
    LD    D,#0
    LD    E,A
    ADD   HL,DE
    LD    E,(HL)
    INC   HL
    LD    D,(HL)

    PUSH  DE
    LD    A,(#_ENVOLVENTE)		//FRECUENCIA DEL CANAL ON/OFF
    RRA
    JR    NC,FRECUENCIA_OFF
    LD    0(IY),E
    LD    1(IY),D
    JR    CONT_ENV

FRECUENCIA_OFF:
    LD    DE,#0
    LD    0(IY),E
    LD    1(IY),D

//CALCULO DEL RATIO (OCTAVA ARRIBA)
CONT_ENV:
    POP   DE
    PUSH  AF
    PUSH  BC
    AND   #0b00000011
    LD    B,A
    //INC	B
   
    //AND	A			//1/2
    RR    D
    RR    E
CRTBC0:
    //AND   A   //1/4 - 1/8 - 1/16
    RR    D
    RR    E
    DJNZ  CRTBC0
    LD    A,E
    LD    (#_PSG_REG+PSG_REG_ENV),A
    LD    A,D
    AND   #0b00000011
    LD    (#_PSG_REG+PSG_REG_ENV+1),A
    POP   BC
    POP   AF      //SELECCION FORMA DE ENVOLVENTE
   
    RRA
    AND   #0b00000110		//$08,$0A,$0C,$0E
    ADD   #8   
    LD    (#_PSG_REG+PSG_REG_SHAPE),A
    LD    (#_ENVOLVENTE_BACK),A
    RET

//EXTRAE UN WORD DE UNA TABLA
//IN:(HL)=DIRECCION TABLA
//   [A]= POSICION
//OUT(HL)=WORD
EXT_WORD:
    LD    D,#0
    RLCA
    LD    E,A
    ADD   HL,DE
    LD    E,(HL)
    INC   HL
    LD    D,(HL)
    EX    DE,HL
    // RET

__endasm;
}
