// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄ ▄▄ ▄ ▄▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ▀█▄▀  ▄█▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▀█  ██  ██▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// WYZ player module
//
// Authors: 
// - MSX PSG proPLAYER V0.3 by WYZ 09.03.2016 https://sites.google.com/site/wyzplayer/home
// - Adapted to SDCC: mvac7/303bcn > <mvac7303b@gmail.com>
//─────────────────────────────────────────────────────────────────────────────
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

// -----------------------------------------------------------------------------


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
// │     │ │ │ │ └── Load song ON/OFF
// │     │ │ │ └──── Player ON/OFF
// │     │ │ └────── EFECTOS ON/OFF
// │     │ └──────── SFX ON/OFF
// │     └────────── Is looping?
// └──────────────── Is ended?
u8 g_WYZ_State;

u8 g_WYZ_Song; // Number of the playing song 
u8 g_WYZ_Tempo; // Tempo
u8 TTEMPO; // Tempo counter

u16 PointerA; // Channel A pointer
u16 PointerB; // Channel B pointer
u16 PointerC; // Channel C pointer

u16 CANAL_A;        //DW DIRECION DE INICIO DE LA MUSICA A
u16 CANAL_B;        //DW DIRECION DE INICIO DE LA MUSICA B
u16 CANAL_C;        //DW DIRECION DE INICIO DE LA MUSICA C

u16 PointerP_A;    //DW PUNTERO PAUTA CANAL A
u16 PointerP_B;    //DW PUNTERO PAUTA CANAL B
u16 PointerP_C;    //DW PUNTERO PAUTA CANAL C

u16 PointerP_A0;   //DW INI PUNTERO PAUTA CANAL A
u16 PointerP_B0;   //DW INI PUNTERO PAUTA CANAL B
u16 PointerP_C0;   //DW INI PUNTERO PAUTA CANAL C

u16 PointerP_DECA; //DW PUNTERO DE INICIO DEL DECODER CANAL A
u16 PointerP_DECB; //DW PUNTERO DE INICIO DEL DECODER CANAL B
u16 PointerP_DECC; //DW PUNTERO DE INICIO DEL DECODER CANAL C

u16 PointerDECA;   //DW PUNTERO DECODER CANAL A
u16 PointerDECB;   //DW PUNTERO DECODER CANAL B
u16 PointerDECC;   //DW PUNTERO DECODER CANAL C

u8 REG_NOTA_A; //DB REGISTRO DE LA NOTA EN EL CANAL A
u8 VOL_INST_A; //DB VOLUMEN RELATIVO DEL INSTRUMENTO DEL CANAL A
u8 REG_NOTA_B; //DB REGISTRO DE LA NOTA EN EL CANAL B
u8 VOL_INST_B; //DB VOLUMEN RELATIVO DEL INSTRUMENTO DEL CANAL B
u8 REG_NOTA_C; //DB REGISTRO DE LA NOTA EN EL CANAL C
u8 VOL_INST_C; //DB VOLUMEN RELATIVO DEL INSTRUMENTO DEL CANAL C

u16 PointerL_DECA; //DW PUNTERO DE INICIO DEL LOOP DEL DECODER CANAL A
u16 PointerL_DECB; //DW PUNTERO DE INICIO DEL LOOP DEL DECODER CANAL B
u16 PointerL_DECC; //DW PUNTERO DE INICIO DEL LOOP DEL DECODER CANAL C

//CANAL DE EFECTOS DE RITMO - ENMASCARA OTRO CANAL
u16 PointerP;      //DW PUNTERO DEL CANAL EFECTOS
u16 CANAL_P;        //DW DIRECION DE INICIO DE LOS EFECTOS

u16 PointerP_DECP; //DW PUNTERO DE INICIO DEL DECODER CANAL P
u16 PointerDECP;   //DW PUNTERO DECODER CANAL P
u16 PointerL_DECP; //DW PUNTERO DE INICIO DEL LOOP DEL DECODER CANAL P
//SELECT_CANAL_P	EQU	INTERR+$36	;DB SELECCION DE CANAL DE EFECTOS DE RITMO


u16 SFX_L;      //DW DIRECCION BUFFER EFECTOS DE RITMO REGISTRO BAJO
u16 SFX_H;      //DW DIRECCION BUFFER EFECTOS DE RITMO REGISTRO ALTO
u16 SFX_V;      //DW DIRECCION BUFFER EFECTOS DE RITMO REGISTRO VOLUMEN
u16 SFX_MIX;    //DW DIRECCION BUFFER EFECTOS DE RITMO REGISTRO MIXER


//EFECTOS DE SONIDO
u8 N_SONIDO;               //DB : NUMERO DE SONIDO
u16 PointerSONIDO; //DW : PUNTERO DEL SONIDO QUE SE REPRODUCE



//DB [13] BUFFERs DE REGISTROS DEL PSG

u8 PSG_REG[16];
u8 AYREGS[16];  //PSG_REG_SEC
u8 ENVOLVENTE;	    /*DB : FORMA DE LA ENVOLVENTE
;BIT 0	  : FRECUENCIA CANAL ON/OFF
;BIT 1-2  : RATIO 
;BIT 3-3  : FORMA */
u8 ENVOLVENTE_BACK; //DB:	BACKUP DE LA FORMA DE LA ENVOLENTE


u8 SOUND_BUFFER_A[0x10];
u8 SOUND_BUFFER_B[0x10];
u8 SOUND_BUFFER_C[0x10];
u8 SOUND_BUFFER_P[0x10];



//songs index address
u16 TABLA_SONG;

//instruments index address
u16 TABLA_PAUTAS;

//FXs index address 
u16 TABLA_SONIDOS;

//Data of the frequencies of the notes
u16 DATOS_NOTAS;

/*
u16 DATOS_NOTAS[]={0,0,
1711,1614,1524,1438,1358,1281,1210,1142,1078,1017,
960,906,855,807,762,719,679,641,605,571,
539,509,480,453,428,404,381,360,339,320,
302,285,269,254,240,227,214,202,190,180,
170,160,151,143,135,127,120,113,107,101,
95,90,85,80,76,71,67,64,60,57};
*/



//TABLA DE DATOS DEL SELECTOR DEL CANAL DE EFECTOS DE RITMO


// ############################################################################# <<< ASIGNAR VALORES EN EL INIT
u8 SELECT_CANAL_A[7]; 
// DW	PSG_REG_SEC+0,PSG_REG_SEC+1,PSG_REG_SEC+8
// DB	10110001B

u8 SELECT_CANAL_B[7]; 
//	DW	PSG_REG_SEC+2,PSG_REG_SEC+3,PSG_REG_SEC+9
//  DB	10101010B

u8 SELECT_CANAL_C[7]; 
//	DW	PSG_REG_SEC+4,PSG_REG_SEC+5,PSG_REG_SEC+10
//  DB	10011100B
// ############################################################################# <<< ASIGNAR VALORES EN EL INIT


//TABLA_DATOS_CANAL_SFX:
//DW	SELECT_CANAL_A,SELECT_CANAL_B,SELECT_CANAL_C
u8 TABLA_DATOS_CANAL_SFX[6];


// -----------------------------------------------------------------------------












/* =============================================================================
 WYZ_Initialize
 Description: Initialize the Player
 Input:       (u16) Songs data index memory address
              (u16) Instruments data index memory address
              (u16) FXs data index memory address
              (u16) Notes Table memory address
 Output:      
============================================================================= */
void WYZ_Initialize(u16 addrSONGs, u16 addrInstruments, u16 addrFXs, u16 addrFreqs) __sdcccall(0)
{
addrSONGs;
addrInstruments;
addrFXs;
addrFreqs;
__asm
    push  IX
    ld    IX,#0
    add   IX,SP
   
    ld    L,4(IX)
    ld    H,5(IX)
    ld    (#_TABLA_SONG),HL

    ld    L,6(IX)
    ld    H,7(IX)
    ld	  (#_TABLA_PAUTAS),HL
    
	ld    L,8(IX)
    ld    H,9(IX)
	ld    (#_TABLA_SONIDOS),HL
   
    ld    L,10(IX)
    ld    H,11(IX)
	ld    (#_DATOS_NOTAS),HL
    
   
    XOR	 A
    LD	 (#_g_WYZ_State),A
   
    call CLEAR_PSG_BUFFER   ;PLAYER_OFF
   
    LD      HL,#_SOUND_BUFFER_A        	;* RESERVAR MEMORIA PARA BUFFER DE SONIDO!!!!!
    LD      (#_CANAL_A),HL
   
    LD      HL,#_SOUND_BUFFER_B        	
    LD      (#_CANAL_B),HL       	
   
    LD      HL,#_SOUND_BUFFER_C       	
    LD      (#_CANAL_C),HL 
   
    LD      HL,#_SOUND_BUFFER_P       	
    LD      (#_CANAL_P),HL

;TABLA DE DATOS DEL SELECTOR DEL CANAL DE EFECTOS DE RITMO
    ;--- chan A
    LD      HL,#_AYREGS+AY_ToneA       	
    LD      (#_SELECT_CANAL_A),HL
   
    LD      HL,#_AYREGS+AY_ToneA+1       	
    LD      (#_SELECT_CANAL_A+2),HL
   
    LD      HL,#_AYREGS+AY_AmpA       	
    LD      (#_SELECT_CANAL_A+4),HL
   
    LD      A,#0b10110001
    LD      (#_SELECT_CANAL_A+6),A
   
    ;--- chan B
    LD      HL,#_AYREGS+AY_ToneB       	
    LD      (#_SELECT_CANAL_B),HL
   
    LD      HL,#_AYREGS+AY_ToneB+1       	
    LD      (#_SELECT_CANAL_B+2),HL
   
    LD      HL,#_AYREGS+AY_AmpB       	
    LD      (#_SELECT_CANAL_B+4),HL
   
    LD      A,#0b10101010
    LD      (#_SELECT_CANAL_B+6),A

    ;--- chan C
    LD      HL,#_AYREGS+AY_ToneC       	
    LD      (#_SELECT_CANAL_C),HL
   
    LD      HL,#_AYREGS+AY_ToneC+1       	
    LD      (#_SELECT_CANAL_C+2),HL
   
    LD      HL,#_AYREGS+AY_AmpC       	
    LD      (#_SELECT_CANAL_C+4),HL
   
    LD      A,#0b10011100
    LD      (#_SELECT_CANAL_C+6),A
   
   
; TABLA_DATOS_CANAL_SFX         
    LD      HL,#_SELECT_CANAL_A        	
    LD      (#_TABLA_DATOS_CANAL_SFX),HL
   
    LD      HL,#_SELECT_CANAL_B        	
    LD      (#_TABLA_DATOS_CANAL_SFX+2),HL
   
    LD      HL,#_SELECT_CANAL_C        	
    LD      (#_TABLA_DATOS_CANAL_SFX+4),HL
   
    pop  IX
    ret
    		
__endasm;
}         
// ----------------------------------------------------------------------------- <<< END WYZ_Initialize      	





/* =============================================================================
 WYZ_Pause
 Description: Pause song playback
 Input:       -
 Output:      -
============================================================================= */
void WYZ_Pause() __naked
{
__asm
    
PLAYER_OFF:
;  XOR	A			
;  LD	[INTERR],A
;  LD	[FADE],A		;solo si hay fade out
   LD   HL,#_g_WYZ_State       
   RES  1,(HL)

CLEAR_PSG_BUFFER:
   XOR	A
   LD   HL,#_PSG_REG
   LD   DE,#_PSG_REG+1
   LD   BC,#14
   LD   (HL),A
   LDIR
   
   LD   A,#0b10111000		; **** POR SI ACASO ****
   LD   (#_PSG_REG+AY_Mixer),A
   
   LD   HL,#_PSG_REG
   LD   DE,#_AYREGS
   LD   BC,#14
   LDIR		
   
   jp	ROUT
   
__endasm;
}
// ----------------------------------------------------------------------------- <<< END WYZ_Pause






/* =============================================================================
 WYZ_Resume
 Description: Resume song playback
 Input:       -
 Output:      -
============================================================================= */  	
void WYZ_Resume() __naked
{
__asm
   LD      HL,#_g_WYZ_State       
   SET     1,(HL)      ;PLAYER ON
   
   RET
__endasm;
}
// ----------------------------------------------------------------------------- <<< END WYZ_Resume




/* -----------------------------------------------------------------------------
 WYZ_IsFinished
 Description: Indicates whether the song has finished playing
 Input:       -
 Output:      [u8] 0 = No, 1 = Yes 
----------------------------------------------------------------------------- */
bool WYZ_IsFinished() __naked
{
__asm
    xor  A
    
    LD   HL,#_g_WYZ_State
    BIT  7,(HL)
    jr   Z,retPlayerEndState
    ld   A,#1
    
retPlayerEndState:    
    ld   L,A
    ret  
__endasm;
}
// ----------------------------------------------------------------------------- <<< END WYZ_IsFinished




/* =============================================================================
 WYZ_SetLoop
 Description: Change loop mode
 Input:       [u8] false = 0, true = 1
 Output:      -
============================================================================= */  	
void WYZ_SetLoop(bool loop) __naked __sdcccall(0)
{
loop;
__asm
    push  IX
    ld    IX,#0
    add   IX,SP
   
    ld    A,4(IX)
    call setLoop

    pop   IX
    RET

setLoop:   
    LD    HL,#_g_WYZ_State
   
    or    A
    jr    Z,resetLOOP

    SET   4,(HL)      ;LOOP ON
    ret

resetLOOP:           

    RES   4,(HL)
    ret

__endasm;
}
// ----------------------------------------------------------------------------- <<< END WYZ_SetLoop





/* =============================================================================
 WYZ_PlayFX
 Description: Play Sound Effect
 Input:       (u8) FX number
 Output:      -
============================================================================= */
void WYZ_PlayFX(u8 numSound) __naked __sdcccall(0)
{
numSound;
__asm
   push IX
   ld   IX,#0
   add  IX,sp
   
   LD   A,4(IX)
   CALL INICIA_SONIDO
   
   pop  IX
   ret


  

;INICIA EL SONIDO No [A]

INICIA_SONIDO:
;CP	8		;SFX SPEECH
;JP	Z,SLOOP		

   LD      HL,(#_TABLA_SONIDOS)    ;(v SDCC) HL,TABLA_SONIDOS  
   CALL    EXT_WORD
   LD      (#_PointerSONIDO),HL
   LD      HL,#_g_WYZ_State
   SET     2,(HL)
   RET  
    
__endasm;
} 
// ----------------------------------------------------------------------------- <<< END WYZ_PlayFX







 
/* =============================================================================
 WYZ_PlayAY
 Description: Send data from AYREGS buffer to AY registers.
              Execute on each interruption of VBLANK
 Input:       -
 Output:      -
============================================================================= */
void WYZ_PlayAY() __naked
{
__asm
    
ROUT:

;Record register 7 of the AY38910 ----------------------------------------------
;collects the last two bits for joysctick port control
;and adds them to the value of the mixer bits
   ld   A,(#_AYREGS+AY_Mixer)
   and  #0b00111111
   ld   B,A
      
   ld   A,#AY_Mixer
   out  (#AY0index),A
   in   A,(#AY0read)  
   and	 #0b11000000	; Mask to catch only the two bits of joys 
   or	 B		        ; Add B
  
   ld   (#_AYREGS+AY_Mixer),A
;END register 7 ----------------------------------------------------------------


;   LD   A,(#_PSG_REG+AY_EnvShape)			
;   AND  A			;ES CERO?
;   JR   Z,NO_BACKUP_ENVOLVENTE
;   LD   (#_ENVOLVENTE_BACK),A	;08.13 / GUARDA LA ENVOLVENTE EN EL BACKUP

NO_BACKUP_ENVOLVENTE:
   XOR  A
   LD   C,#AY0index   ;0xA0
   LD   HL,#_AYREGS
   
LOUT:
   OUT     (C),A
   INC     C
   OUTI 
   DEC     C
   INC     A
   CP      #13
   JR      NZ,LOUT
   
   OUT     (C),A
   LD      A,(HL)
   AND     A
   RET     Z
   
   INC     C
   OUT     (C),A
   LD      (#_ENVOLVENTE_BACK),A
       
   XOR     A
   LD      (#_AYREGS+AY_EnvShape),A
   LD	   (#_PSG_REG+AY_EnvShape),A
   
   RET

__endasm;
}
// ----------------------------------------------------------------------------- <<< END WYZ_PlayAY









/* =============================================================================
 WYZ_PlaySong
 Description: Initialize song
 Input:       [u8] song number
              [u8] loop status (false = 0, true = 1)
 Output:      -
============================================================================= */
void WYZ_PlaySong(u8 numSong, bool loop) __sdcccall(0)
{
numSong;
loop;
__asm
   push IX
   ld   IX,#0
   add  IX,sp
   
   ld   A,5(IX)
   call setLoop
   
   LD   A,4(IX)
   call CARGA_CANCION
   
   pop  IX
   ret




;CARGA UNA CANCION
;IN:[A]=N� DE CANCION

CARGA_CANCION:
   
   ;LD   HL,#_g_WYZ_Song
   LD   (#_g_WYZ_Song),A   ;N� A
   
   LD   HL,#_g_WYZ_State       ;CARGA CANCION		
   SET  1,(HL)              ;REPRODUCE CANCION
   RES  7,(HL)              ;END off
   

;DECODIFICAR
;IN-> g_WYZ_State 0 ON
;     g_WYZ_Song

;CARGA CANCION SI/NO

DECODE_SONG:
   LD   A,(#_g_WYZ_Song)

;LEE CABECERA DE LA CANCION
;BYTE 0=g_WYZ_Tempo

   LD   HL,(#_TABLA_SONG)   ;(v SDCC) HL,_TABLA_SONG
   CALL EXT_WORD
   LD   A,(HL)
   LD   (#_g_WYZ_Tempo),A
   DEC  A
   LD   (#_TTEMPO),A
         
;HEADER BYTE 1
;[-|-|-|-|  3-1 | 0  ]
;[-|-|-|-|FX CHN|LOOP]

   INC  HL     ;LOOP 1=ON/0=OFF?
   LD   A,(HL)
   ;BIT  0,A
   ;JR   Z,NPTJP0
   
   ;PUSH HL
   ;LD   HL,#_g_WYZ_State
   ;SET  4,(HL)
   ;POP  HL



;SELECCION DEL CANAL DE EFECTOS DE RITMO

NPTJP0:
   AND  #0b00000110 
   RRA
   ;LD	(#_SELECT_CANAL_P),A
   
   PUSH	HL
   LD	  HL,#_TABLA_DATOS_CANAL_SFX
   CALL  EXT_WORD
   PUSH	HL
   POP  IX
   LD   E,0(IX)
   LD   D,1(IX)
   LD   (#_SFX_L),DE
   
   LD   E,2(IX)
   LD   D,3(IX)
   LD   (#_SFX_H),DE
   
   LD   E,4(IX)
   LD   D,5(IX)
   LD   (#_SFX_V),DE
   
   LD   A,6(IX)
   LD   (#_SFX_MIX),A
   POP	HL
   
   INC	HL		;2 BYTES RESERVADOS
   INC	HL
   INC	HL

;BUSCA Y GUARDA INICIO DE LOS CANALES EN EL MODULO MUS (OPTIMIZAR)
;A�ADE OFFSET DEL LOOP

   PUSH HL     ;IX INICIO OFFSETS LOOP POR CANAL
   POP  IX

   LD   DE,#0x0008		;HASTA INICIO DEL CANAL A
   ADD  HL,DE


   LD   (#_PointerP_DECA),HL	;GUARDA PUNTERO INICIO CANAL
   LD   E,(IX)
   LD   D,1(IX)
   ADD  HL,DE
   LD   (#_PointerL_DECA),HL	;GUARDA PUNTERO INICIO LOOP

   CALL BGICMODBC1
   LD   (#_PointerP_DECB),HL
   LD   E,2(IX)
   LD   D,3(IX)
   ADD  HL,DE
   LD   (#_PointerL_DECB),HL

   CALL BGICMODBC1
   LD   (#_PointerP_DECC),HL
   LD   E,4(IX)
   LD   D,5(IX)
   ADD  HL,DE
   LD   (#_PointerL_DECC),HL

   CALL BGICMODBC1
   LD   (#_PointerP_DECP),HL
   LD   E,6(IX)
   LD   D,7(IX)
   ADD  HL,DE
   LD   (#_PointerL_DECP),HL


;LEE DATOS DE LAS NOTAS
;[|][|||||] LONGITUD\NOTA

INIT_DECODER:
   LD   DE,(#_CANAL_A)
   LD   (#_PointerA),DE
   LD   HL,(#_PointerP_DECA)
   CALL DECODE_CANAL    	;CANAL A
   LD   (#_PointerDECA),HL
   
   LD   DE,(#_CANAL_B)
   LD   (#_PointerB),DE
   LD   HL,(#_PointerP_DECB)
   CALL DECODE_CANAL    	;CANAL B
   LD   (#_PointerDECB),HL
   
   LD   DE,(#_CANAL_C)
   LD   (#_PointerC),DE
   LD   HL,(#_PointerP_DECC)
   CALL DECODE_CANAL    	;CANAL C
   LD   (#_PointerDECC),HL
   
   LD   DE,(#_CANAL_P)
   LD   (#_PointerP),DE
   LD   HL,(#_PointerP_DECP)
   CALL DECODE_CANAL    	;CANAL P (FX)
   LD   (#_PointerDECP),HL
   
   RET


;BUSCA INICIO DEL CANAL
BGICMODBC1:
   XOR  A     ;BUSCA EL BYTE 0
   LD   E,#0x3F   ;CODIGO INSTRUMENTO 0
   LD   B,#0xFF   ;EL MODULO DEBE TENER UNA LONGITUD MENOR DE $FF00 ... o_O!
   CPIR
   
   DEC  HL
   DEC  HL
   LD   A,E      ;ES EL INSTRUMENTO 0??
   CP   (HL)
   INC  HL
   INC  HL
   JR   Z,BGICMODBC1
   
   DEC  HL
   DEC  HL
   DEC  HL
   LD   A,E     ;ES VOLUMEN 0??
   CP   (HL)
   INC  HL
   INC  HL
   INC  HL
   JR   Z,BGICMODBC1
   RET

__endasm;
}
// ----------------------------------------------------------------------------- <<< END WYZ_PlaySong











/* =============================================================================
 WYZ_Decode
 Description: Process the next step in the song sequence 
 Input:       -
 Output:      -
============================================================================= */        
void WYZ_Decode() __naked __sdcccall(0)
{
__asm

   
INICIO:

   push IX
;   CALL ROUT

;   CALL MIXER

   LD   HL,#_PSG_REG
   LD   DE,#_AYREGS
   LD   BC,#14
   LDIR

   CALL REPRODUCE_SONIDO

   CALL PLAY

   pop  IX
   RET





;REPRODUCE EFECTOS DE SONIDO

REPRODUCE_SONIDO:

   LD   HL,#_g_WYZ_State
   BIT  2,(HL)   ;ESTA ACTIVADO EL EFECTO?
   RET  Z
   LD   HL,(#_PointerSONIDO)
   LD   A,(HL)
   CP   #0xFF
   JR   Z,FIN_SONIDO
   LD   DE,(#_SFX_L)
   LD   (DE),A
   INC  HL
   LD   A,(HL)
   RRCA
   RRCA
   RRCA
   RRCA
   AND  #0b00001111
   LD   DE,(#_SFX_H)
   LD   (DE),A
   LD   A,(HL)
   AND  #0b00001111
   LD   DE,(#_SFX_V)
   LD   (DE),A

   INC  HL
   LD   A,(HL)
   LD   B,A
   BIT  7,A       ;09.08.13 BIT MAS SIGINIFICATIVO ACTIVA ENVOLVENTES
   JR   Z,NO_ENVOLVENTES_SONIDO
   LD   A,#0x12
   LD   (DE),A
   INC  HL
   LD   A,(HL)
   LD   (#_AYREGS+AY_EnvPeriod),A
   INC  HL
   LD   A,(HL)
   LD   (#_AYREGS+AY_EnvPeriod+1),A
   INC  HL
   LD   A,(HL)
   CP   #1
   JR   Z,NO_ENVOLVENTES_SONIDO		;NO ESCRIBE LA ENVOLVENTE SI SU VALOR ES 1
   LD   (#_AYREGS+AY_EnvShape),A


NO_ENVOLVENTES_SONIDO:
   LD   A,B
   RES  7,A
   AND  A
   JR   Z,NO_RUIDO
   LD   (#_AYREGS+AY_Noise),A
   LD   A,(#_SFX_MIX)
   JR   SI_RUIDO

NO_RUIDO:
   XOR  A
   LD   (#_AYREGS+AY_Noise),A
   LD   A,#0b10111000

SI_RUIDO:
   LD   (#_AYREGS+AY_Mixer),A   
   INC  HL
   LD   (#_PointerSONIDO),HL
   RET

FIN_SONIDO:
   LD   HL,#_g_WYZ_State
   RES  2,(HL)
   LD   A,(#_ENVOLVENTE_BACK)		;NO RESTAURA LA ENVOLVENTE SI ES 0
   AND  A
   JR   Z,FIN_NOPLAYER
   ;xor  a 
   LD   (#_AYREGS+AY_EnvShape),A			;08.13 RESTAURA LA ENVOLVENTE TRAS EL SFX

FIN_NOPLAYER:
   LD   A,#0b10111000
   LD   (#_AYREGS+AY_Mixer),A

   RET



PLAY:
   LD   HL,#_g_WYZ_State       ;PLAY BIT 1 ON?
   BIT  1,(HL)
   RET  Z
   
;TEMPO
   LD   HL,#_TTEMPO       ;CONTADOR TEMPO
   INC  (HL)
   LD   A,(#_g_WYZ_Tempo)
   CP   (HL)
   JR   NZ,PAUTAS
   LD   (HL),#0

;INTERPRETA
   LD   IY,#_PSG_REG
   LD   IX,#_PointerA
   LD   BC,#_PSG_REG+AY_AmpA
   CALL LOCALIZA_NOTA
   LD   IY,#_PSG_REG+AY_ToneB
   LD   IX,#_PointerB
   LD   BC,#_PSG_REG+AY_AmpB
   CALL LOCALIZA_NOTA
   LD   IY,#_PSG_REG+AY_ToneC
   LD   IX,#_PointerC
   LD   BC,#_PSG_REG+AY_AmpC
   CALL LOCALIZA_NOTA
   LD   IX,#_PointerP    ;EL CANAL DE EFECTOS ENMASCARA OTRO CANAL
   CALL LOCALIZA_EFECTO

PAUTAS:
   LD   IY,#_PSG_REG+AY_ToneA
   LD   IX,#_PointerP_A
   LD   HL,#_PSG_REG+AY_AmpA
   CALL PAUTA    ;PAUTA CANAL A
   LD   IY,#_PSG_REG+AY_ToneB
   LD   IX,#_PointerP_B
   LD   HL,#_PSG_REG+AY_AmpB
   CALL PAUTA    ;PAUTA CANAL B
   LD   IY,#_PSG_REG+AY_ToneC
   LD   IX,#_PointerP_C
   LD   HL,#_PSG_REG+AY_AmpC
   CALL PAUTA    ;PAUTA CANAL C

   RET




;DECODIFICA NOTAS DE UN CANAL
;IN (DE)=DIRECCION DESTINO
;NOTA=0 FIN CANAL
;NOTA=1 SILENCIO
;NOTA=2 PUNTILLO
;NOTA=3 COMANDO I

DECODE_CANAL:
   LD      A,(HL)
   AND     A        ;FIN DEL CANAL?
   JR      Z,FIN_DEC_CANAL
   CALL    GETLEN

   CP      #0b00000001       ;ES SILENCIO?
   JR      NZ,NO_SILENCIO
   SET     6,A
   JR      NO_MODIFICA
         
NO_SILENCIO:
   CP      #0b00111110       ;ES PUNTILLO?
   JR      NZ,NO_PUNTILLO
   OR      A
   RRC     B
   XOR     A
   JR      NO_MODIFICA

NO_PUNTILLO:
   CP      #0b00111111       ;ES COMANDO?
   JR      NZ,NO_MODIFICA
   BIT     0,B      ;COMADO=INSTRUMENTO?
   JR      Z,NO_INSTRUMENTO   
   LD      A,#0b11000001     ;CODIGO DE INSTRUMENTO      
   LD      (DE),A
   INC     HL
   INC     DE
   LD      A,(HL)   ;N� DE INSTRUMENTO
   LD      (DE),A
   INC     DE
   INC     HL
   LD      A,(HL)   ;VOLUMEN RELATIVO DEL INSTRUMENTO
   LD      (DE),A
   INC     DE
   INC     HL
   JR      DECODE_CANAL

NO_INSTRUMENTO:
   BIT     2,B
   JR      Z,NO_ENVOLVENTE
   LD      A,#0b11000100     ;CODIGO ENVOLVENTE
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
   LD      A,#0b11000010     ;CODIGO EFECTO
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
   RET    ;** JR      DECODE_CANAL

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




;LOCALIZA NOTA CANAL A
;IN [PointerA]

;LOCALIZA NOTA CANAL A
;IN [PointerA]

LOCALIZA_NOTA:
   LD      L,_PointerA - _PointerA(IX)	  ;HL=[PointerA_C_B] 
   LD      H,_PointerA - _PointerA+1(IX)  ;
   LD      A,(HL)
   AND     #0b11000000       ;COMANDO?
   CP      #0b11000000
   JR      NZ,LNJP0

;BIT[0]=INSTRUMENTO
   
COMANDOS:
   LD      A,(HL)
   BIT     0,A      ;INSTRUMENTO
   JR      Z,COM_EFECTO

   INC     HL
   LD      A,(HL)   ;N° DE PAUTA
   INC     HL
   LD      E,(HL)

   PUSH    HL       ;TEMPO
   LD      HL,#_g_WYZ_Tempo
   BIT     5,E
   JR      Z,NO_DEC_TEMPO
   DEC     (HL)
NO_DEC_TEMPO:
   BIT     6,E
   JR      Z,NO_INC_TEMPO
   INC     (HL)
NO_INC_TEMPO:
   RES     5,E    ;SIEMPRE RESETEA LOS BITS DE TEMPO
   RES     6,E
   POP     HL
   
   LD      _VOL_INST_A - _PointerA(IX),E	;REGISTRO DEL VOLUMEN RELATIVO
   INC     HL
   LD      _PointerA - _PointerA(IX),L   ;
   LD      _PointerA - _PointerA+1(IX),H   ;
   LD      HL,(#_TABLA_PAUTAS)   ;(v sdcc) HL,#_TABLA_PAUTAS
   CALL    EXT_WORD
   LD      _PointerP_A0 - _PointerA(IX),L
   LD      _PointerP_A0 - _PointerA+1(IX),H
   LD      _PointerP_A - _PointerA(IX),L
   LD      _PointerP_A - _PointerA+1(IX),H
   LD      L,C
   LD      H,B
   RES     4,(HL)     ;APAGA EFECTO ENVOLVENTE
   XOR     A
   LD      (#_AYREGS+AY_EnvShape),A
   LD      (#_PSG_REG+AY_EnvShape),A
   ;LD     [ENVOLVENTE_BACK],A   ;08.13 RESETEA EL BACKUP DE LA ENVOLVENTE
   JR      LOCALIZA_NOTA

COM_EFECTO:
   BIT     1,A   ;EFECTO DE SONIDO
   JR      Z,COM_ENVOLVENTE

   INC     HL
   LD      A,(HL)
   INC     HL
   LD      _PointerA - _PointerA(IX),L  ;
   LD      _PointerA - _PointerA+1(IX),H  ;
   CALL    INICIA_SONIDO
   RET

COM_ENVOLVENTE:
   BIT  2,A
   RET  Z       ;IGNORA - ERROR

   INC  HL
   LD   A,(HL)   ;CARGA CODIGO DE ENVOLVENTE
   LD   (#_ENVOLVENTE),A
   INC  HL
   LD   _PointerA - _PointerA(IX),L   ;
   LD   _PointerA - _PointerA+1(IX),H   ;
   LD   L,C
   LD   H,B
   LD   (HL),#0b00010000    ;ENCIENDE EFECTO ENVOLVENTE
   JR   LOCALIZA_NOTA
   
LNJP0:   
   LD   A,(HL)
   INC  HL
   BIT  7,A
   JR   Z,NO_FIN_CANAL_A	;
   BIT  0,A
   JR   Z,FIN_CANAL_A

FIN_NOTA_A:
   LD   E,_CANAL_A - _PointerA(IX)
   LD   D,_CANAL_A - _PointerA+1(IX)	;PUNTERO BUFFER AL INICIO
   LD   _PointerA - _PointerA(IX),E  ;
   LD   _PointerA - _PointerA+1(IX),D  ;
   LD   L,_PointerDECA - _PointerA(IX)	;CARGA PUNTERO DECODER
   LD   H,_PointerDECA - _PointerA+1(IX)
   PUSH BC
   CALL DECODE_CANAL      ;DECODIFICA CANAL
   POP  BC
   LD   _PointerDECA - _PointerA(IX),L	;GUARDA PUNTERO DECODER
   LD   _PointerDECA - _PointerA+1(IX),H
   JP   LOCALIZA_NOTA
   
FIN_CANAL_A:
   LD   HL,#_g_WYZ_State     ;LOOP?
   BIT  4,(HL)
   JR   NZ,FCA_CONT
   POP  AF
   
   SET  7,(HL)           ;End song
   JP   PLAYER_OFF

FCA_CONT:
   LD   L,_PointerL_DECA - _PointerA(IX)	;CARGA PUNTERO INICIAL DECODER
   LD   H,_PointerL_DECA - _PointerA+1(IX)
   LD   _PointerDECA - _PointerA(IX),L
   LD   _PointerDECA - _PointerA+1(IX),H
   JR   FIN_NOTA_A
   
NO_FIN_CANAL_A:
   LD   _PointerA - _PointerA(IX),L   ;[PointerA_B_C]=HL GUARDA PUNTERO   
   LD   _PointerA - _PointerA+1(IX),H   ;
   AND  A    ;NO REPRODUCE NOTA SI NOTA=0
   JR   Z,FIN_RUTINA
   BIT  6,A      		;SILENCIO?
   JR   Z,NO_SILENCIO_A
   LD   A,(BC)
   AND	 #0b00010000
   JR   NZ,SILENCIO_ENVOLVENTE
   
   XOR  A
   LD   (BC),A				;RESET VOLUMEN DEL CORRESPODIENTE CHIP
   LD   0(IY),A
   LD   1(IY),A
   RET

SILENCIO_ENVOLVENTE:
   LD   A,#0xFF
   LD   (#_PSG_REG+AY_EnvPeriod),A
   LD   (#_PSG_REG+AY_EnvPeriod+1),A
   XOR	A
   LD   (#_PSG_REG+AY_EnvShape),A
   LD   0(IY),A
   LD   1(IY),A
   RET

NO_SILENCIO_A:
   LD   _REG_NOTA_A - _PointerA(IX),A	;REGISTRO DE LA NOTA DEL CANAL
   CALL NOTA     		;REPRODUCE NOTA
   LD   L,_PointerP_A0 - _PointerA(IX)     ;HL=[PointerP_A0] RESETEA PAUTA 
   LD   H,_PointerP_A0 - _PointerA+1(IX)
   LD   _PointerP_A - _PointerA(IX),L       ;[PointerP_A]=HL
   LD   _PointerP_A - _PointerA+1(IX),H
FIN_RUTINA:
   RET


;LOCALIZA EFECTO
;IN HL=[PointerP]
LOCALIZA_EFECTO:
   LD      L,0(IX)       ;HL=[PointerP]
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
   INC  HL
   BIT  7,A
   JR   Z,NO_FIN_CANAL_P	;
   BIT  0,A
   JR   Z,FIN_CANAL_P

FIN_NOTA_P:
   LD   DE,(#_CANAL_P)
   LD   0(IX),E
   LD   1(IX),D
   LD   HL,(#_PointerDECP)	;CARGA PUNTERO DECODER
   PUSH BC
   CALL DECODE_CANAL    	;DECODIFICA CANAL
   POP  BC
   LD   (#_PointerDECP),HL	;GUARDA PUNTERO DECODER
   JP   LOCALIZA_EFECTO
   
FIN_CANAL_P:
   LD   HL,(#_PointerL_DECP)	;CARGA PUNTERO INICIAL DECODER
   LD   (#_PointerDECP),HL
   JR   FIN_NOTA_P
   
NO_FIN_CANAL_P:
   LD   0(IX),L        ;[PointerA_B_C]=HL GUARDA PUNTERO
   LD   1(IX),H
   RET



; PAUTA DE LOS 3 CANALES
; IN:[IX]:PUNTERO DE LA PAUTA
;    (HL):REGISTRO DE VOLUMEN
;    [IY]:REGISTROS DE FRECUENCIA

; FORMATO PAUTA	
;	    7    6     5     4   3-0    3-0  
; BYTE 1 [LOOP|OCT-1|OCT+1|ORNMT|VOL] - BYTE 2 [ | | | |PITCH/NOTA]

PAUTA:
   BIT  4,(HL)        ;SI LA ENVOLVENTE ESTA ACTIVADA NO ACTUA PAUTA
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

   BIT  7,A		;LOOP / EL RESTO DE BITS NO AFECTAN
   JR   Z,PCAJP0
   AND  #0b00011111       ;M�XIMO LOOP PAUTA [0,32]X2!!!-> PARA ORNAMENTOS
   RLCA    ;X2
   LD   D,#0
   LD   E,A
   SBC  HL,DE
   LD   A,(HL)

PCAJP0:
   BIT  6,A   ;OCTAVA -1
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
   BIT  5,A		;OCTAVA +1
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
   JR   NZ,PCAJP6	;ORNAMENTOS SELECCIONADOS

;FUNCION PITCH DE FRECUENCIA
   INC  HL
   PUSH HL
   LD   E,A
   LD   A,(HL)		;PITCH DE FRECUENCIA
   LD   L,A
   AND  A
   LD   A,E
   JR   Z,ORNMJP1

   LD   A,0(IY)    ;SI LA FRECUENCIA ES 0 NO HAY PITCH
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

;FUNCION ORNAMENTOS
PCAJP6:		
   INC  HL
   PUSH HL
   PUSH AF
   LD   A,_REG_NOTA_A - _PointerP_A(IX)	;RECUPERA REGISTRO DE NOTA EN EL CANAL
   LD   E,(HL)		;
   ADC  E		;+- NOTA 
   CALL TABLA_NOTAS
   POP  AF	

ORNMJP1:
   POP  HL
   INC  HL
   LD   0(IX),L
   LD   1(IX),H
PCAJP5:
   POP  HL
   LD   B,_VOL_INST_A - _PointerP_A(IX)	;VOLUMEN RELATIVO
   ADD  B
   JP   P,PCAJP7
   LD   A,#1		;NO SE EXTIGUE EL VOLUMEN
PCAJP7:
   AND  #0b00001111   ;VOLUMEN FINAL MODULADO
   LD   (HL),A
   RET



;NOTA : REPRODUCE UNA NOTA
;IN [A]=CODIGO DE LA NOTA
;   [IY]=REGISTROS DE FRECUENCIA
NOTA:
   LD    L,C
   LD    H,B
   BIT   4,(HL)
   LD    B,A
   JR    NZ,EVOLVENTES
   LD    A,B
TABLA_NOTAS:
   LD    HL,(#_DATOS_NOTAS)     ;BUSCA FRECUENCIA
   CALL  EXT_WORD
   LD    0(IY),L
   LD    1(IY),H
   RET



;IN [A]=CODIGO DE LA ENVOLVENTE
;   [IY]=REGISTRO DE FRECUENCIA
EVOLVENTES:
    LD    HL,(#_DATOS_NOTAS)
    ;SUB	12
    RLCA      ;X2
    LD    D,#0
    LD    E,A
    ADD   HL,DE
    LD    E,(HL)
    INC   HL
    LD    D,(HL)

    PUSH  DE
    LD    A,(#_ENVOLVENTE)		;FRECUENCIA DEL CANAL ON/OFF
    RRA
    JR    NC,FRECUENCIA_OFF
    LD    0(IY),E
    LD    1(IY),D
    JR    CONT_ENV

FRECUENCIA_OFF:
    LD    DE,#0
    LD    0(IY),E
    LD    1(IY),D

;CALCULO DEL RATIO (OCTAVA ARRIBA)
CONT_ENV:
    POP   DE
    PUSH  AF
    PUSH  BC
    AND   #0b00000011
    LD    B,A
    ;INC	B
   
    ;AND	A			;1/2
    RR    D
    RR    E
CRTBC0:
    ;AND   A   ;1/4 - 1/8 - 1/16
    RR    D
    RR    E
    DJNZ  CRTBC0
    LD    A,E
    LD    (#_PSG_REG+AY_EnvPeriod),A
    LD    A,D
    AND   #0b00000011
    LD    (#_PSG_REG+AY_EnvPeriod+1),A
    POP   BC
    POP   AF      ;SELECCION FORMA DE ENVOLVENTE
   
    RRA
    AND   #0b00000110		;$08,$0A,$0C,$0E
    ADD   #8   
    LD    (#_PSG_REG+AY_EnvShape),A
    LD    (#_ENVOLVENTE_BACK),A
    RET



;EXTRAE UN WORD DE UNA TABLA
;IN:(HL)=DIRECCION TABLA
;   [A]= POSICION
;OUT(HL)=WORD
EXT_WORD:
    LD    D,#0
    RLCA
    LD    E,A
    ADD   HL,DE
    LD    E,(HL)
    INC   HL
    LD    D,(HL)
    EX    DE,HL
    RET


__endasm;
}
// ----------------------------------------------------------------------------- <<< END WYZ_Decode 

