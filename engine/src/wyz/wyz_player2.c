// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄ ▄▄ ▄ ▄▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ▀█▄▀  ▄█▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▀█  ██  ██▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// WYZ player module
// 
// From MSX PSG proPLAYER V0.3 - WYZ 47d
//─────────────────────────────────────────────────────────────────────────────

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------
#include "wyz_player2.h"

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// VARIABLES
//-----------------------------------------------------------------------------

u16 g_WYZ_SongTable;
u16 g_WYZ_InstrumentTable;
u16 g_WYZ_SoundTable;
u16 g_WYZ_NoteTable;

u8  CANAL_A_BUFFER[WYZ_CHAN_BUFFER_SIZE];
u8  CANAL_B_BUFFER[WYZ_CHAN_BUFFER_SIZE];
u8  CANAL_C_BUFFER[WYZ_CHAN_BUFFER_SIZE];
#if (WYZ_CHANNELS == WYZ_6CH)
u8  CANAL_D_BUFFER[WYZ_CHAN_BUFFER_SIZE];
u8  CANAL_E_BUFFER[WYZ_CHAN_BUFFER_SIZE];
u8  CANAL_F_BUFFER[WYZ_CHAN_BUFFER_SIZE];
#endif
u8  CANAL_P_BUFFER[WYZ_CHAN_BUFFER_SIZE];

u8  HYBRID;				// Hybrid tempo
						//  BIT 6 = Alternator
						//  BIT 7 = Hybrid ON/OFF

u8  g_WYZ_State;		// Switches 1=ON 0=OFF
// 7 6 5 4 3 2 1 0
//       │ │ │ │ └── Load song ON/OFF
//       │ │ │ └──── Player ON/OFF
//       │ │ └────── Effects ON/OFF
//       │ └──────── SFX ON/OFF
//       └────────── Loop
#define INTERR		_g_WYZ_State

u8  g_WYZ_Music;		// Music no.
#define SONG		_g_WYZ_Music
u8  g_WYZ_Tempo;		// Tempo
#define TEMPO		_g_WYZ_Tempo
u8  TTEMPO;				// Tempo counter
u8  g_WYZ_Header;		// Music data header
#define HEADER		_g_WYZ_Header

u16 POINTER_A;			// Channel pointer A
u16 POINTER_B;			// Channel pointer B
u16 POINTER_C;			// Channel pointer C
#if (WYZ_CHANNELS == WYZ_6CH)
u16 POINTER_D;			// Channel pointer D
u16 POINTER_E;			// Channel pointer E
u16 POINTER_F;			// Channel pointer F
#endif

u16 CANAL_A;			// Music start address A
u16 CANAL_B;			// Music start address B
u16 CANAL_C;			// Music start address C
#if (WYZ_CHANNELS == WYZ_6CH)
u16 CANAL_D;			// Music start address D
u16 CANAL_E;			// Music start address E
u16 CANAL_F;			// Music start address F
#endif

u16 POINTER_P_A;		// PUNTERO PAUTA CANAL A
u16 POINTER_P_B;		// PUNTERO PAUTA CANAL B
u16 POINTER_P_C;		// PUNTERO PAUTA CANAL C
#if (WYZ_CHANNELS == WYZ_6CH)
u16 POINTER_P_D;		// PUNTERO PAUTA CANAL D
u16 POINTER_P_E;		// PUNTERO PAUTA CANAL E
u16 POINTER_P_F;		// PUNTERO PAUTA CANAL F
#endif

u16 POINTER_P_A0;		// INI PUNTERO PAUTA CANAL A
u16 POINTER_P_B0;		// INI PUNTERO PAUTA CANAL B
u16 POINTER_P_C0;		// INI PUNTERO PAUTA CANAL C
#if (WYZ_CHANNELS == WYZ_6CH)
u16 POINTER_P_D0;		// INI PUNTERO PAUTA CANAL D
u16 POINTER_P_E0;		// INI PUNTERO PAUTA CANAL E
u16 POINTER_P_F0;		// INI PUNTERO PAUTA CANAL F
#endif

u16 POINTER_P_DECA;		// PUNTERO DE INICIO DEL DECODER CANAL A
u16 POINTER_P_DECB;		// PUNTERO DE INICIO DEL DECODER CANAL B
u16 POINTER_P_DECC;		// PUNTERO DE INICIO DEL DECODER CANAL C
#if (WYZ_CHANNELS == WYZ_6CH)
u16 POINTER_P_DECD;		// PUNTERO DE INICIO DEL DECODER CANAL D
u16 POINTER_P_DECE;		// PUNTERO DE INICIO DEL DECODER CANAL E
u16 POINTER_P_DECF;		// PUNTERO DE INICIO DEL DECODER CANAL F
#endif

u16 POINTER_DECA;		// PUNTERO DECODER CANAL A
u16 POINTER_DECB;		// PUNTERO DECODER CANAL B
u16 POINTER_DECC;		// PUNTERO DECODER CANAL C       
#if (WYZ_CHANNELS == WYZ_6CH)
u16 POINTER_DECD;		// PUNTERO DECODER CANAL D
u16 POINTER_DECE;		// PUNTERO DECODER CANAL E
u16 POINTER_DECF;		// PUNTERO DECODER CANAL F       
#endif

u8  REG_NOTA_A;			// Registration of the note in the channel A
u8  VOL_INST_A;			// Relative instrument volume of channel A
u8  REG_NOTA_B;			// Registration of the note in the channel B
u8  VOL_INST_B;			// Relative instrument volume of channel B					;VACIO
u8  REG_NOTA_C;			// Registration of the note in the channel C
u8  VOL_INST_C;			// Relative instrument volume of channel C
#if (WYZ_CHANNELS == WYZ_6CH)
u8  REG_NOTA_D;			// Registration of the note in the channel D
u8  VOL_INST_D;			// Relative instrument volume of channel D
u8  REG_NOTA_E;			// Registration of the note in the channel E
u8  VOL_INST_E;			// Relative instrument volume of channel E					;VACIO
u8  REG_NOTA_F;			// Registration of the note in the channel F
u8  VOL_INST_F;			// Relative instrument volume of channel F
#endif

u16 POINTER_L_DECA;		// Decoder loop start pointer channel A
u16 POINTER_L_DECB;		// Decoder loop start pointer channel B
u16 POINTER_L_DECC;		// Decoder loop start pointer channel C
#if (WYZ_CHANNELS == WYZ_6CH)
u16 POINTER_L_DECD;		// Decoder loop start pointer channel D
u16 POINTER_L_DECE;		// Decoder loop start pointer channel E
u16 POINTER_L_DECF;		// Decoder loop start pointer channel F
#endif

// Rhythm effects channel - Masks other channel
u16 POINTER_P;			// Effects channel pointer
u16 CANAL_P;			// Effects start address
u16 POINTER_P_DECP;		// Channel P decoder start pointer
u16 POINTER_DECP;		// Channel P decoder pointer
u16 POINTER_L_DECP;		// Decoder loop start pointer channel P
// u8  SELECT_CANAL_P;		// Rhythm effect channel selection

u16 SFX_L;				// Address buffer effects rhythm effects low register
u16 SFX_H;				// Address buffering rate effects high register
u16 SFX_V;				// Address buffer effects rhythm effects volume register
u16 SFX_MIX;			// Address buffer effects rhythm effects register mixer

// Sound effects
u8  N_SONIDO;			// Sound number
u16 POINTER_SONIDO;		// Pointer of the sound being played

// PSG register buffers
#if (WYZ_USE_DIRECT_ACCESS)
	u8 PSG_REG_INT[14];
	u8 PSG_REG_BAK[14];
	#if (WYZ_CHANNELS == WYZ_6CH)
	u8 PSG_REG_EXT[14];
	#endif
#else
	#define _PSG_REG_INT _g_PSG_Regs
	u8 PSG_REG_BAK[14];
	#define _PSG_REG_EXT _g_PSG2_Regs
#endif

// Envelope
u8 ENVOLVENTE;			// Shape of the envelope
// 7 6 5 4 3 2 1 0
//         │ │ │ └── Frequency channel ON/OFF
//         │ └─┴──── Ratio 
//         └──────── Form
u8 ENVOLVENTE_BACK;		// Backup of the envelope shape



//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void WYZ_InitPlayer()
{
	WYZ_Stop();
	
__asm
//-- Music initial data
	LD      HL,#_CANAL_A_BUFFER		// Reserve memory for sound buffering (main PSG)
	LD      (_CANAL_A),HL			// Recommended 16 or more bytes per channel. 

	LD      HL,#_CANAL_B_BUFFER        	
	LD      (_CANAL_B),HL       	

	LD      HL,#_CANAL_C_BUFFER       	
	LD      (_CANAL_C),HL 

	#if (WYZ_CHANNELS == WYZ_6CH)
	LD      HL,#_CANAL_D_BUFFER		// Reserve memory for sound buffering (secondary PSG)
	LD      (_CANAL_D),HL           // Recommended 16 or more bytes per channel. 

	LD      HL,#_CANAL_E_BUFFER        	
	LD      (_CANAL_E),HL       	

	LD      HL,#_CANAL_F_BUFFER       	
	LD      (_CANAL_F),HL 
	#endif

	LD      HL,#_CANAL_P_BUFFER       	
	LD      (_CANAL_P),HL 
	
__endasm;
}

//-----------------------------------------------------------------------------
//
void WYZ_Decode() __naked
{
__asm
	push	ix // Backup C frame buffer

INICIO:
#if (WYZ_USE_DIRECT_ACCESS)
	CALL	ROUT_INT
#if (WYZ_CHANNELS == WYZ_6CH)
	CALL	ROUT_EXT
#endif // (WYZ_CHANNELS == WYZ_6CH)
#endif // (WYZ_USE_DIRECT_ACCESS)

	LD		HL,#_PSG_REG_INT
	LD		DE,#_PSG_REG_BAK
	LD		BC,#14
	LDIR	

	CALL    REPRODUCE_SONIDO
	CALL    PLAY
	// CALL	REPRODUCE_EFECTO

	pop		ix // Restore C frame buffer

	RET

// Plays internal PSG sound effects
REPRODUCE_SONIDO:

	LD      HL,#INTERR   
	BIT     2,(HL)          		// Is effect == ON?
	RET     Z
	LD      HL,(_POINTER_SONIDO)
	LD      A,(HL)
	CP      #0xFF
	JR      Z,FIN_SONIDO
	LD		DE,(_SFX_L)
	LD      (DE),A
	INC     HL
	LD      A,(HL)
	RRCA
	RRCA
	RRCA
	RRCA
	AND     #0b00001111
	LD		DE,(_SFX_H)
	LD      (DE),A
	LD      A,(HL)
	AND     #0b00001111
	LD		DE,(_SFX_V)
	LD      (DE),A

	INC     HL
	LD      A,(HL)
	LD		B,A
	BIT		7,A						// The most significant bit activate enveloppe
	JR		Z,NO_ENVOLVENTES_SONIDO
	LD		A,#0x12
	LD		(DE),A
	INC		HL
	LD      A,(HL)
	LD		(_PSG_REG_BAK+11),A
	INC		HL
	LD      A,(HL)
	LD		(_PSG_REG_BAK+12),A
	INC		HL
	LD      A,(HL)
	CP		#1
	JR		Z,NO_ENVOLVENTES_SONIDO	// Does not write the envelope if its value is 1
	LD		(_PSG_REG_BAK+13),A                

NO_ENVOLVENTES_SONIDO:
	LD		A,B
	RES		7,A
	AND     A
	JR      Z,NO_RUIDO
	LD      (_PSG_REG_BAK+6),A
	LD      A,(_SFX_MIX)
	JR      SI_RUIDO
NO_RUIDO:
	XOR		A
	LD      (_PSG_REG_BAK+6),A
	LD      A,#0b10111000
SI_RUIDO:
	LD      (_PSG_REG_BAK+7),A
	INC     HL
	LD      (_POINTER_SONIDO),HL
	RET

FIN_SONIDO:
	LD      HL,#INTERR
	RES     2,(HL)					// Effect = OFF
	LD		A,(_ENVOLVENTE_BACK)	// Does not restore the envelope if it is 0
	AND		A
	JR		Z,FIN_NOPLAYER
	// xor	a //  ***
#if (WYZ_CHANNELS == WYZ_6CH)
	LD		(_PSG_REG_INT+13),A		// Restores the envelope after the sfx
#else
	LD		(_PSG_REG_BAK+13),A		// Restores the envelope after the sfx
#endif
       		
FIN_NOPLAYER:
	LD      A,#0b10111000
	LD      (_PSG_REG_BAK+7),A

	RET                        


// Dumps sound buffer to psg
ROUT_INT:
	LD		A,(_PSG_REG_INT+13)			
	AND		A						// Is zero?
	JR		Z,NO_BACKUP_ENVOLVENTE
	LD		(_ENVOLVENTE_BACK),A	// Saves the envelope in the backup

NO_BACKUP_ENVOLVENTE:
	XOR     A
	LD      C,#P_PSG_REGS
	LD      HL,#_PSG_REG_BAK
LOUT_INT:	
	OUT     (C),A
	INC     C
	OUTI 
	DEC     C
	INC     A
	CP      #13
	JR      NZ,LOUT_INT
	OUT     (C),A
	LD      A,(HL)
	AND     A
	RET     Z
	INC     C
	OUT     (C),A
	XOR     A
	LD      (_PSG_REG_BAK+13),A
	LD		(_PSG_REG_INT+13),A
	RET

#if (WYZ_CHANNELS == WYZ_6CH)
ROUT_EXT:
	XOR     A
	LD      C,#P_PSG_EXT_REGS
	LD      HL,#_PSG_REG_EXT
LOUT_EXT:
	OUT     (C),A
	INC     C
	OUTI 
	DEC     C
	INC     A
	CP      #13
	JR      NZ,LOUT_EXT
	OUT     (C),A
	LD      A,(HL)
	AND     A
	RET     Z
	INC     C
	OUT     (C),A
	XOR     A
	// LD		(_PSG_REG_EXT+13),A
	// LD		(_PSG_REG_INT+13),A
	RET
#endif

// Starts sound no. (A)
INICIA_SONIDO:
#if (WYZ_CHANNELS == WYZ_6CH)
	// CP		#8						// SFX speech
	// JP		Z,SLOOP
#else
	// CP		#19						// SFX speech
	// JP		NC,SET_SAMPLE_P	
#endif

	LD      HL,(_g_WYZ_SoundTable)
	CALL    EXT_WORD
	LD      (_POINTER_SONIDO),HL
	LD      HL,#INTERR
	SET     2,(HL)					// Effect = ON
	RET
__endasm;
}

//-----------------------------------------------------------------------------
// Stop music playback
void WYZ_Stop()
{
__asm
PLAYER_OFF:
	XOR		A						// Important if there is no music
	LD		(INTERR),A				// Reset all state flags
	// LD		(FADE),A				// Only if there is fade out

CLEAR_PSG_BUFFER:
	LD		HL,#_PSG_REG_INT
	LD		DE,#_PSG_REG_INT+1
	LD		BC,#13
	LD		(HL),A
	LDIR

	LD      A,#0b10111000			// Set PSG mixer register (just in case)
	LD      (_PSG_REG_INT+7),A

	LD		HL,#_PSG_REG_INT
	LD		DE,#_PSG_REG_BAK
	LD		BC,#14
	LDIR		

#if (WYZ_CHANNELS == WYZ_6CH)
	LD		HL,#_PSG_REG_INT
	LD		DE,#_PSG_REG_EXT
	LD		BC,#14
	LDIR		
#endif

#if (WYZ_USE_DIRECT_ACCESS)
	CALL	ROUT_INT
#if (WYZ_CHANNELS == WYZ_6CH)
	CALL	ROUT_EXT
#endif // (WYZ_CHANNELS == WYZ_6CH)
#endif // (WYZ_USE_DIRECT_ACCESS)

__endasm;

#if (!WYZ_USE_DIRECT_ACCESS)
	PSG_Apply();
#endif
}

//-----------------------------------------------------------------------------
// Load a song
// IN:(A)=Song no.
void WYZ_Play(u8 music) __naked
{
	music; // A
__asm
CARGA_CANCION:
	LD      HL,#INTERR				// Load song
	SET     1,(HL)          		// Playing = ON
	LD      HL,#SONG
	LD      (HL),A					// No. A

// Decodify
// IN-> INTERR 0 ON
//      SONG

// Load song YES/NO
DECODE_SONG:

//---- Read song header

//-- Header byte [0]
// 7 6 5 4 3 2 1 0
// │ └─┴─┴─┴─┴─┴─┴── Tempo (7-bits)
// └──────────────── Hybrid tempo ON/OFF
	LD      A,(SONG)
	LD      HL,(_g_WYZ_SongTable)
	CALL    EXT_WORD
	LD      A,(HL)
	AND		#0b01111111
	LD      (TEMPO),A
	DEC		A
	LD		(_TTEMPO),A
	LD		A,(HL)
	AND		#0b10000000
	LD		(_HYBRID),A

//-- Header byte [1]
// 7 6 5 4 3 2 1 0
//   │     │ │ │ └── Loop ON/OFF
//   │     │ │ └──── FX on channel A
//   │     │ └────── FX on channel B
//   │     └──────── FX on channel C
//   └────────────── Frequency (0:50 Hz, 1:60 Hz)
	INC		HL						// LOOP 1=ON/0=OFF?
	LD		A,(HL)
	LD		(HEADER),A
	BIT		0,A
	JR		Z,NPTJP0
	PUSH	HL
	LD		HL,#INTERR
	SET		4,(HL)					// Loop = ON
	POP		HL

// Selection of the rhythm effect channel
NPTJP0:
	AND		#0b00000110
	RRA
	// LD		(SELECT_CANAL_P),A

	PUSH	HL
	LD		HL,#TABLA_DATOS_CANAL_SFX
	CALL    EXT_WORD
	PUSH	HL
	POP		IY
	LD		E,0(IY)
	LD		D,1(IY)
	LD		(_SFX_L),DE

	LD		E,2(IY)
	LD		D,3(IY)
	LD		(_SFX_H),DE

	LD		E,4(IY)
	LD		D,5(IY)
	LD		(_SFX_V),DE

	LD		A,6(IY)
	LD		(_SFX_MIX),A
	POP		HL

	INC		HL						// 2 bytes reserved
	INC		HL
	INC		HL

// Search and save start of channels in the mus module (optimize)
// Add loop offset
	PUSH	HL						// IY start offset loop per channel
	POP		IY

#if (WYZ_CHANNELS == WYZ_6CH)
	LD		DE,#0x0008+6			// To start of channel A (for 6 channels)
#else
	LD		DE,#0x0008				// To start of channel A
#endif
	ADD		HL,DE

	LD		(_POINTER_P_DECA),HL	// Guard pointer home internal channels
	LD		E,0(IY)
	LD		D,1(IY)
	ADD		HL,DE
	LD		(_POINTER_L_DECA),HL	// Saves pointer start loop

	CALL	BGICMODBC1
	LD		(_POINTER_P_DECB),HL
	LD		E,2(IY)
	LD		D,3(IY)
	ADD		HL,DE
	LD		(_POINTER_L_DECB),HL

	CALL	BGICMODBC1
	LD		(_POINTER_P_DECC),HL
	LD		E,4(IY)
	LD		D,5(IY)
	ADD		HL,DE
	LD		(_POINTER_L_DECC),HL

#if (WYZ_CHANNELS == WYZ_6CH)
	CALL	BGICMODBC1
	LD		(_POINTER_P_DECD),HL	// Guard pointer home external channels
	LD		E,6(IY)
	LD		D,7(IY)
	ADD		HL,DE
	LD		(_POINTER_L_DECD),HL	// Saves pointer start loop

	CALL	BGICMODBC1
	LD		(_POINTER_P_DECE),HL
	LD		E,8(IY)
	LD		D,9(IY)
	ADD		HL,DE
	LD		(_POINTER_L_DECE),HL

	CALL	BGICMODBC1
	LD		(_POINTER_P_DECF),HL
	LD		E,10(IY)
	LD		D,11(IY)
	ADD		HL,DE
	LD		(_POINTER_L_DECF),HL

	CALL	BGICMODBC1
	LD		(_POINTER_P_DECP),HL
	LD		E,12(IY)
	LD		D,13(IY)
	ADD		HL,DE
	LD		(_POINTER_L_DECP),HL
#else
	CALL	BGICMODBC1
	LD		(_POINTER_P_DECP),HL
	LD		E,6(IY)
	LD		D,7(IY)
	ADD		HL,DE
	LD		(_POINTER_L_DECP),HL
#endif


// Read data from the notes
// 7 6 5 4 3 2 1 0
// │ │ └─┴─┴─┴─┴─┴── Note
// └─┴────────────── Length
INIT_DECODER:
	LD      DE,(_CANAL_A)
	LD      (_POINTER_A),DE
	LD		HL,(_POINTER_P_DECA)
	CALL    DECODE_CANAL    	// Canal A
	LD		(_POINTER_DECA),HL

	LD      DE,(_CANAL_B)
	LD      (_POINTER_B),DE
	LD		HL,(_POINTER_P_DECB)
	CALL    DECODE_CANAL    	// Canal B
	LD		(_POINTER_DECB),HL

	LD      DE,(_CANAL_C)
	LD      (_POINTER_C),DE
	LD		HL,(_POINTER_P_DECC)
	CALL    DECODE_CANAL    	// Canal C
	LD		(_POINTER_DECC),HL

#if (WYZ_CHANNELS == WYZ_6CH)
	LD      DE,(_CANAL_D)
	LD      (_POINTER_D),DE
	LD		HL,(_POINTER_P_DECD)
	CALL    DECODE_CANAL    	// Canal D
	LD		(_POINTER_DECD),HL

	LD      DE,(_CANAL_E)
	LD      (_POINTER_E),DE
	LD		HL,(_POINTER_P_DECE)
	CALL    DECODE_CANAL    	// Canal E
	LD		(_POINTER_DECE),HL

	LD      DE,(_CANAL_F)
	LD      (_POINTER_F),DE
	LD		HL,(_POINTER_P_DECF)
	CALL    DECODE_CANAL    	// Canal F
	LD		(_POINTER_DECF),HL
#endif

	LD      DE,(_CANAL_P)
	LD      (_POINTER_P),DE
	LD		HL,(_POINTER_P_DECP)
	CALL    DECODE_CANAL    	// Canal P
	LD		(_POINTER_DECP),HL

	RET

// Search channel start
BGICMODBC1:
	XOR		A						// Search byte 0
	LD		E,#0x3F					// Instrument code 0
	LD		B,#0xFF					// Module must have a length less than 0xFF00 ... o_O!
	CPIR

	DEC		HL
	DEC		HL
	LD		A,E						// Is instrument 0?
	CP		(HL)
	INC		HL
	INC		HL
	JR		Z,BGICMODBC1

	DEC		HL
	DEC		HL
	DEC		HL
	LD		A,E						// Is volume 0?
	CP		(HL)
	INC		HL
	INC		HL
	INC		HL
	JR		Z,BGICMODBC1
	RET

// Decodes notes from channel 1
// IN (DE)=Destination address
// NOTA: 0 Channel end
// NOTA: 1 Silence
// NOTA: 2 Puntillo
// NOTA: 3 Command I
DECODE_CANAL:
	LD      A,(HL)
	AND     A						// End of channel?
	JR      Z,FIN_DEC_CANAL
	CALL    GETLEN

	CP      #0b00000001				// Is silence?
	JR      NZ,NO_SILENCIO
	SET     6,A
	JR      NO_MODIFICA
                
NO_SILENCIO:
	CP      #0b00111110				// Is puntillo?
	JR      NZ,NO_PUNTILLO
	OR      A
	RRC     B
	XOR     A
	JR      NO_MODIFICA

NO_PUNTILLO:
	CP      #0b00111111				// Is command?
	JR      NZ,NO_MODIFICA
	BIT     0,B						// Command=Instrument?
	JR      Z,NO_INSTRUMENTO   
	LD      A,#0b11000001			// Instrument code
	LD      (DE),A
	INC     HL
	INC     DE
	LD      A,(HL)					// Instrument no.
	LD      (DE),A
	INC     DE
	INC		HL
	LD      A,(HL)					// Relative size of the instrument
	LD      (DE),A
	INC     DE
	INC		HL
	JR      DECODE_CANAL

NO_INSTRUMENTO:
	BIT     2,B
	JR      Z,NO_ENVOLVENTE
	LD      A,#0b11000100			// Envelope code
	LD      (DE),A
	INC     DE
	INC		HL
	LD		A,(HL)
	LD		(DE),A
	INC		DE
	INC		HL
	JR      DECODE_CANAL
     
NO_ENVOLVENTE:
	BIT     1,B
	JR      Z,NO_MODIFICA           
	LD      A,#0b11000010			// Effect code 
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
	SET 	0,A
	LD      (DE),A
	INC     DE
	INC		HL
	RET			// JR      DECODE_CANAL
                
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
	DJNZ    DCBC0
	LD      B,A
	POP     AF
	RET


// PLAY __________________________________________________
PLAY:
	LD      HL,#INTERR				// Is playing == ON?
	BIT     1,(HL)
	RET     Z

	LD		HL,#_HYBRID
	BIT		7,(HL)
	JR		Z,TEMPO_ENTERO
	BIT		6,(HL)
	JR		Z,TEMPO_ENTERO

TEMPO_SEMI:       
	LD      HL,#_TTEMPO				// Tempo counter
	INC     (HL)
	LD      A,(TEMPO)
	DEC		A
	CP      (HL)
	JR      NZ,PAUTAS
	LD      (HL),#0
	LD		HL,#_HYBRID
	RES		6,(HL)
	JR		INTERPRETA
		
TEMPO_ENTERO:       
	LD      HL,#_TTEMPO				// Tempo counter
	INC     (HL)
	LD      A,(TEMPO)
	CP      (HL)
	JR      NZ,PAUTAS
	LD      (HL),#0
	LD		HL,#_HYBRID
	SET		6,(HL)

INTERPRETA:

	LD      IY,#_PSG_REG_INT		// Internal PSG
	LD      IX,#_POINTER_A
	LD      BC,#_PSG_REG_INT+8
	CALL    LOCALIZA_NOTA
	LD      IY,#_PSG_REG_INT+2
	LD      IX,#_POINTER_B
	LD      BC,#_PSG_REG_INT+9
	CALL    LOCALIZA_NOTA
	LD      IY,#_PSG_REG_INT+4
	LD      IX,#_POINTER_C
	LD      BC,#_PSG_REG_INT+10
	CALL    LOCALIZA_NOTA
	
#if (WYZ_CHANNELS == WYZ_6CH)
	LD      IY,#_PSG_REG_EXT		// External PSG
	LD      IX,#_POINTER_D
	LD      BC,#_PSG_REG_EXT+8
	CALL    LOCALIZA_NOTA
	LD      IY,#_PSG_REG_EXT+2
	LD      IX,#_POINTER_E
	LD      BC,#_PSG_REG_EXT+9
	CALL    LOCALIZA_NOTA
	LD      IY,#_PSG_REG_EXT+4
	LD      IX,#_POINTER_F
	LD      BC,#_PSG_REG_EXT+10
	CALL    LOCALIZA_NOTA
#endif
	
	LD      IX,#_POINTER_P			// The effect channel masks another channel
	CALL    LOCALIZA_EFECTO              

// PAUTAS 
PAUTAS:
	LD      IY,#_PSG_REG_INT+0		// Internal PSG
	LD      IX,#_POINTER_P_A
	LD      HL,#_PSG_REG_INT+8
	CALL    PAUTA					// Channel A pattern
	LD      IY,#_PSG_REG_INT+2
	LD      IX,#_POINTER_P_B
	LD      HL,#_PSG_REG_INT+9
	CALL    PAUTA					// Channel B pattern
	LD      IY,#_PSG_REG_INT+4
	LD      IX,#_POINTER_P_C
	LD      HL,#_PSG_REG_INT+10
	CALL    PAUTA					// Channel C pattern                

#if (WYZ_CHANNELS == WYZ_6CH)
	LD      IY,#_PSG_REG_EXT+0		// External PSG
	LD      IX,#_POINTER_P_D
	LD      HL,#_PSG_REG_EXT+8
	CALL    PAUTA					// Channel D pattern
	LD      IY,#_PSG_REG_EXT+2
	LD      IX,#_POINTER_P_E
	LD      HL,#_PSG_REG_EXT+9
	CALL    PAUTA					// Channel E pattern
	LD      IY,#_PSG_REG_EXT+4
	LD      IX,#_POINTER_P_F
	LD      HL,#_PSG_REG_EXT+10
	CALL    PAUTA					// Channel F pattern
#endif

	RET


// Locates channel note A
// IN (POINTER_A)
LOCALIZA_NOTA:
	LD      L,_POINTER_A-_POINTER_A+0(IX)	// HL=(POINTER_A_C_B)
	LD      H,_POINTER_A-_POINTER_A+1(IX)
	LD      A,(HL)
	AND     #0b11000000				// Command?
	CP      #0b11000000
	JR      NZ,LNJP0

// BIT(0)=Instrument
COMANDOS:
	LD      A,(HL)
	BIT     0,A             		// Instrument
	JR      Z,COM_EFECTO

	INC     HL
	LD      A,(HL)          		// Pattern no.
	INC     HL
	LD		E,(HL)				

	PUSH	HL						// Tempo
	LD		HL,#TEMPO
	BIT		5,E
	JR		Z,NO_DEC_TEMPO
	DEC		(HL)
NO_DEC_TEMPO:
	BIT		6,E
	JR		Z,NO_INC_TEMPO
	INC		(HL)
NO_INC_TEMPO:
	RES		5,E						// Always reset the tempo bits		
	RES		6,E
	POP		HL

	LD      _VOL_INST_A-_POINTER_A(IX),E	// Relative volume register
	INC		HL
	LD      _POINTER_A-_POINTER_A+0(IX),L
	LD      _POINTER_A-_POINTER_A+1(IX),H
	LD      HL,(_g_WYZ_InstrumentTable)
	CALL    EXT_WORD
	LD      _POINTER_P_A0-_POINTER_A+0(IX),L
	LD      _POINTER_P_A0-_POINTER_A+1(IX),H
	LD      _POINTER_P_A-_POINTER_A+0(IX),L
	LD      _POINTER_P_A-_POINTER_A+1(IX),H
	LD      L,C
	LD      H,B
	RES     4,(HL)        			// Erases the enveloppe effect
	XOR     A
	LD      (_PSG_REG_BAK+13),A
	LD		(_PSG_REG_INT+13),A
	// LD		(_ENVOLVENTE_BACK),A	// Resets the envelope backup
	JR      LOCALIZA_NOTA

COM_EFECTO:
	BIT     1,A						// Sound effect
	JR      Z,COM_ENVOLVENTE

	INC     HL
	LD      A,(HL)
	INC     HL
	LD      _POINTER_A-_POINTER_A+0(IX),L
	LD      _POINTER_A-_POINTER_A+1(IX),H
	CALL    INICIA_SONIDO
	RET

COM_ENVOLVENTE:
	BIT     2,A
	RET     Z               		// Ignore - Error

	INC     HL
	LD		A,(HL)					// Load envelope code
	LD		(_ENVOLVENTE),A
	INC     HL
	LD      _POINTER_A-_POINTER_A+0(IX),L
	LD      _POINTER_A-_POINTER_A+1(IX),H
	LD      L,C
	LD      H,B
	LD		(HL),#0b00010000		// Turn on enveloppe effect
	JR      LOCALIZA_NOTA

LNJP0:
	LD      A,(HL)
	INC     HL
	BIT     7,A
	JR      Z,NO_FIN_CANAL_A
	BIT		0,A
	JR		Z,FIN_CANAL_A

FIN_NOTA_A:
	LD      E,_CANAL_A-_POINTER_A+0(IX)
	LD		D,_CANAL_A-_POINTER_A+1(IX)	// Buffer pointer at startup
	LD		_POINTER_A-_POINTER_A+0(IX),E
	LD		_POINTER_A-_POINTER_A+1(IX),D
	LD		L,_POINTER_DECA-_POINTER_A+0(IX) // Decoder pointer load
	LD		H,_POINTER_DECA-_POINTER_A+1(IX)
	PUSH	BC
	CALL    DECODE_CANAL    		// Decodes channel
	POP		BC
	LD		_POINTER_DECA-_POINTER_A+0(IX),L // Watch pointer decoder
	LD		_POINTER_DECA-_POINTER_A+1(IX),H
	JP      LOCALIZA_NOTA
                
FIN_CANAL_A:
	LD		HL,#INTERR
	BIT		4,(HL)					// Is loop == ON?
	JR      NZ,FCA_CONT
	POP		AF
	JP		_WYZ_Stop
                

FCA_CONT:
	LD		L,_POINTER_L_DECA-_POINTER_A+0(IX) // Initial decoder pointer load
	LD		H,_POINTER_L_DECA-_POINTER_A+1(IX)
	LD		_POINTER_DECA-_POINTER_A+0(IX),L
	LD		_POINTER_DECA-_POINTER_A+1(IX),H
	JR      FIN_NOTA_A

NO_FIN_CANAL_A:
	LD      _POINTER_A-_POINTER_A+0(IX),L // (POINTER_A_B_C)=HL GUARDA PUNTERO
	LD      _POINTER_A-_POINTER_A+1(IX),H
	AND     A               		// No reproduce note yes note=0
	JR      Z,FIN_RUTINA
	BIT     6,A             		// Silence?
	JR      Z,NO_SILENCIO_A
	LD		A,(BC)
	AND		#0b00010000
	JR		NZ,SILENCIO_ENVOLVENTE

	XOR     A
	LD		(BC),A					// Reset volume of the corresponding chip
	LD		0(IY),A
	LD		1(IY),A
	RET
		
SILENCIO_ENVOLVENTE:
	LD		A,#0xFF
	LD		(_PSG_REG_INT+11),A
	LD		(_PSG_REG_INT+12),A               
	XOR		A
	LD		(_PSG_REG_INT+13),A                               
	LD		0(IY),A
	LD		1(IY),A
	RET

NO_SILENCIO_A:
	LD		_REG_NOTA_A-_POINTER_A(IX),A // Channel note registration         
	CALL    NOTA            		// Reproduce note
	LD      L,_POINTER_P_A0-_POINTER_A+0(IX) // HL=(POINTER_P_A0) RESETEA PAUTA 
	LD      H,_POINTER_P_A0-_POINTER_A+1(IX)
	LD      _POINTER_P_A-_POINTER_A+0(IX),L // (POINTER_P_A)=HL
	LD      _POINTER_P_A-_POINTER_A+1(IX),H
FIN_RUTINA:
	RET


// Locates effect
// IN HL=(_POINTER_P)
LOCALIZA_EFECTO:
	LD      L,0(IX)					// HL=(_POINTER_P)
	LD      H,1(IX)
	LD      A,(HL)
	CP      #0b11000010
	JR      NZ,LEJP0

	INC     HL
	LD      A,(HL)
	INC     HL
	LD      0(IX),L
	LD      1(IX),H
	CALL    INICIA_SONIDO
	RET


LEJP0:
	INC     HL
	BIT     7,A
	JR      Z,NO_FIN_CANAL_P
	BIT		0,A
	JR		Z,FIN_CANAL_P
FIN_NOTA_P:	
	LD      DE,(_CANAL_P)
	LD		0(IX),E
	LD		1(IX),D
	LD		HL,(_POINTER_DECP)		// Decoder pointer load
	PUSH	BC
	CALL    DECODE_CANAL			// Decodes channel
	POP		BC
	LD		(_POINTER_DECP),HL		// Check pointer decoder
	JP      LOCALIZA_EFECTO
                
FIN_CANAL_P:
	LD		HL,(_POINTER_L_DECP)	// Initial decoder pointer load
	LD		(_POINTER_DECP),HL
	JR      FIN_NOTA_P
                
NO_FIN_CANAL_P:
	LD      0(IX),L					// (POINTER_A_B_C)=HL GUARDA PUNTERO
	LD      1(IX),H
	RET

// 3-channel pattern
// IN->(IX): Leading edge of the pattern
//     (HL): Volume register
//     (IY): Frequency records

//---- Pattern format	
//-- Pattern byte [0]	
// 7 6 5 4 3 2 1 0
// │ │ │ │ └─┴─┴─┴── Volume
// │ │ │ └────────── Ornament
// │ │ └──────────── Octave+1
// │ └────────────── Octave-1
// └──────────────── Loop

//-- Pattern byte [1]	
// 7 6 5 4 3 2 1 0
//         └─┴─┴─┴── Pitch/note
PAUTA:
	BIT     4,(HL)					// If the envelope is activated, no pattern is active
	RET     NZ

	LD		A,0(IY)
	LD		B,1(IY)
	OR		B
	RET		Z

	PUSH	HL

PCAJP4:
	LD      L,0(IX)
	LD      H,1(IX)         
	LD		A,(HL)

	BIT     7,A						// Loop / all other bits are unaffected
	JR      Z,PCAJP0
	AND     #0b00011111				// Maximum loop pattern (0,32)X2!!! -> For ornaments
	RLCA							// X2
	LD      D,#0
	LD      E,A
	SBC     HL,DE
	LD      A,(HL)

PCAJP0:		
	BIT		6,A						// Octave -1
	JR		Z,PCAJP1
	LD		E,0(IY)
	LD		D,1(IY)

	AND		A
	RRC		D
	RR		E
	LD		0(IY),E
	LD		1(IY),D
	JR		PCAJP2
		
PCAJP1:		
	BIT		5,A						// Octave +1
	JR		Z,PCAJP2
	LD		E,0(IY)
	LD		D,1(IY)

	AND		A
	RLC		E
	RL		D
	LD		0(IY),E
	LD		1(IY),D		

PCAJP2:		
	LD		A,(HL)
	BIT		4,A
	JR		NZ,PCAJP6				// Selected ornaments

	INC     HL		// ______________________ FUNCION PITCH DE FRECUENCIA__________________		
	PUSH	HL
	LD		E,A
	LD		A,(HL)		// PITCH DE FRECUENCIA
	LD		L,A
	AND		A
	LD		A,E
	JR		Z,ORNMJP1

	LD		A,0(IY)	// SI LA FRECUENCIA ES 0 NO HAY PITCH
	ADD		A,1(IY)
	AND		A
	LD		A,E
	JR		Z,ORNMJP1

	BIT		7,L
	JR		Z,ORNNEG
	LD		H,#0xFF
	JR		PCAJP3
ORNNEG:
	LD		H,#0

PCAJP3:
	LD		E,0(IY)
	LD		D,1(IY)
	ADC		HL,DE
	LD		0(IY),L
	LD		1(IY),H
	JR		ORNMJP1


PCAJP6:
	INC		HL		// ______________________ FUNCION ORNAMENTOS__________________	

	PUSH	HL
	PUSH	AF
	LD		A,_REG_NOTA_A-_POINTER_P_A(IX)	// RECUPERA REGISTRO DE NOTA EN EL CANAL
	LD		E,(HL)		// 
	ADC		E		// +- NOTA 
	CALL	TABLA_NOTAS
	POP		AF	

ORNMJP1:
	POP		HL

	INC		HL
	LD      0(IX),L
	LD      1(IX),H
#if (WYZ_CHANNELS == WYZ_6CH)
PCAJP5:         
	POP		HL
	LD		B,_VOL_INST_A-_POINTER_P_A(IX)	// VOLUMEN RELATIVO
	ADD		B
	JP		P,PCAJP7
	LD		A,#1		// NO SE EXTIGUE EL VOLUMEN
PCAJP7:         
	AND		#0b00001111	// VOLUMEN FINAL MODULADO
	LD      (HL),A
#else
PCAJP5: 	
	POP 	HL
	AND 	#15
	LD  	C, A
	LD  	A,_VOL_INST_A-_POINTER_P_A(IX)   // VOLUMEN RELATIVO
	BIT 	4, A
	JR  	Z, PCAJP9
	OR  	#0xF0
PCAJP9: 	
	ADD 	A, C
	JP  	P,PCAJP7
	LD  	A, #1        // NO SE EXTINGUE EL VOLUMEN
PCAJP7:		
	CP 		#15
	JP 		M, PCAJP8
	LD 		A, #15
PCAJP8: 	
	LD      (HL),A
#endif
	RET

// NOTA : REPRODUCE UNA NOTA
// IN (A)=CODIGO DE LA NOTA
//    (IY)=REGISTROS DE FRECUENCIA
NOTA:		
	LD      L,C
	LD      H,B
	BIT     4,(HL)
	LD      B,A
	JR		NZ,EVOLVENTES
	LD		A,B
TABLA_NOTAS:
	LD      HL,(_g_WYZ_NoteTable)		// BUSCA FRECUENCIA
	CALL	EXT_WORD
	LD      0(IY),L
	LD      1(IY),H
	RET


// IN (A)=CODIGO DE LA ENVOLVENTE
//    (IY)=REGISTRO DE FRECUENCIA
EVOLVENTES:     
	LD      HL,(_g_WYZ_NoteTable)
	// SUB	12
	RLCA                    // X2
	LD      D,#0
	LD      E,A
	ADD     HL,DE
	LD		E,(HL)
	INC		HL
	LD		D,(HL)

	PUSH	DE
	LD		A,(_ENVOLVENTE)		// FRECUENCIA DEL CANAL ON/OFF
	RRA
	JR		NC,FRECUENCIA_OFF
	LD      0(IY),E
	LD      1(IY),D
	JR		CONT_ENV

FRECUENCIA_OFF:	
	LD 		DE,#0x0000
	LD      0(IY),E
	LD      1(IY),D
					// CALCULO DEL RATIO (OCTAVA ARRIBA)
CONT_ENV:	
	POP		DE
	PUSH	AF
	PUSH	BC
	AND		#0b00000011
	LD		B,A
	// INC	B

	// AND	A			// 1/2
	RR		D
	RR		E
CRTBC0:		
	// AND	A			// 1/4 - 1/8 - 1/16
	RR		D
	RR		E
	DJNZ	CRTBC0
	LD		A,E
	LD      (_PSG_REG_INT+11),A
	LD		A,D
	AND		#0b00000011
	LD      (_PSG_REG_INT+12),A
	POP		BC
	POP		AF			// SELECCION FORMA DE ENVOLVENTE

	RRA
	AND		#0b00000110		// 0x08,0x0A,0x0C,0x0E
	ADD		#8                
	LD      (_PSG_REG_INT+13),A
	LD		(_ENVOLVENTE_BACK),A
	RET

// EXTRAE UN WORD DE UNA TABLA
// IN:(HL)=DIRECCION TABLA
//    (A)= POSICION
// OUT(HL)=WORD
EXT_WORD:
	LD      D,#0
	RLCA
	LD      E,A
	ADD     HL,DE
	LD      E,(HL)
	INC     HL
	LD      D,(HL)
	EX      DE,HL
	RET
                
// TABLA DE DATOS DEL SELECTOR DEL CANAL DE EFECTOS DE RITMO
TABLA_DATOS_CANAL_SFX:
	.DW	SELECT_CANAL_A,SELECT_CANAL_B,SELECT_CANAL_C

// BYTE 0:_SFX_L	
// BYTE 1:_SFX_H	
// BYTE 2:_SFX_V	
// BYTE 3:_SFX_MIX
SELECT_CANAL_A:
	.DW	_PSG_REG_BAK+0,_PSG_REG_BAK+1,_PSG_REG_BAK+8
	.DB	#0b10110001
		
SELECT_CANAL_B:	
	.DW	_PSG_REG_BAK+2,_PSG_REG_BAK+3,_PSG_REG_BAK+9
	.DB	#0b10101010
		
SELECT_CANAL_C:	
	.DW	_PSG_REG_BAK+4,_PSG_REG_BAK+5,_PSG_REG_BAK+10
	.DB	#0b10011100
__endasm;
}
