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
#include "core.h"

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
============================================================================= */

#ifndef AY_REGISTERS
#define AY_REGISTERS
#define AY_ToneA      0 //Channel A Tone Period (12 bits)
#define AY_ToneB      2 //Channel B Tone Period (12 bits)
#define AY_ToneC      4 //Channel C Tone Period (12 bits)
#define AY_Noise      6 //Noise Period (5 bits)
#define AY_Mixer      7 //Mixer
#define AY_AmpA       8 //Channel Volume A (4 bits + B5 active Envelope)
#define AY_AmpB       9 //Channel Volume B (4 bits + B5 active Envelope)
#define AY_AmpC      10 //Channel Volume C (4 bits + B5 active Envelope)
#define AY_EnvPeriod 11 //Envelope Period (16 bits)
#define AY_EnvShape  13 //Envelope Shape
#endif



#ifndef _SWITCHER
#define _SWITCHER
  typedef enum {OFF = 0, ON = 1} SWITCHER;
#endif



/*
WYZstate = INTERR            
INTERRUPTORES 1=ON 0=OFF
- BIT 0=CARGA CANCION ON/OFF
- BIT 1=PLAYER ON/OFF
- BIT 2=EFECTOS ON/OFF
- BIT 3=SFX ON/OFF
- BIT 4=LOOP
*/
extern u8 WYZstate;

extern u8 SONG;   //number of song playing
extern u8 TEMPO;  //TEMPO

extern u8 AYREGS[16];       //PSG registers buffer

//extern u16 TABLA_SONG;    //songs index  
//extern u16 TABLA_PAUTAS;  //instruments index
//extern u16 TABLA_SONIDOS; //FXs index
//extern u16 DATOS_NOTAS;   //Data of the frequencies of the notes

//------------------------------------------------------------------------------



/* =============================================================================
 WYZ_Initialize
 Description: Initialize the Player
 Input:       (u16) Songs data index memory address
              (u16) Instruments data index memory address
              (u16) FXs data index memory address
              (u16) Notes Table memory address
 Output:      -
============================================================================= */
void WYZ_Initialize(u16 addrSONGs, u16 addrInstruments, u16 addrFXs, u16 addrFreqs) __sdcccall(0);



/* =============================================================================
 WYZ_Pause
 Description: Pause song playback
 Input:       -
 Output:      -
============================================================================= */
void WYZ_Pause() __naked;  



/* =============================================================================
 WYZ_Resume
 Description: Resume song playback
 Input:       -
 Output:      -
============================================================================= */ 
void WYZ_Resume() __naked; 



/* =============================================================================
 WYZ_SetLoop
 Description: Change loop mode
 Input:       [u8] false = 0, true = 1
 Output:      -
============================================================================= */ 
void WYZ_SetLoop(u8 loop) __naked __sdcccall(0);



/* -----------------------------------------------------------------------------
 WYZ_IsFinished
 Description: Indicates whether the song has finished playing
 Input:       -
 Output:      [u8] 0 = No, 1 = Yes 
----------------------------------------------------------------------------- */
u8 WYZ_IsFinished() __naked;



/* =============================================================================
 WYZ_PlayFX
 Description: Play Sound Effect
 Input:       (u8) FX number
 Output:      -
============================================================================= */
void WYZ_PlayFX(u8 numSound) __naked __sdcccall(0); 



/* =============================================================================
 WYZ_PlayAY
 Description: Send data from AYREGS buffer to AY registers.
              Execute on each interruption of VBLANK
 Input:       -
 Output:      -
============================================================================= */
void WYZ_PlayAY() __naked;



/* =============================================================================
 WYZ_PlaySong
 Description: Initialize song
 Input:       [u8] song number
              [u8] loop status (false = 0, true = 1)
 Output:      -
============================================================================= */
void WYZ_PlaySong(u8 numSong, u8 loop) __sdcccall(0); 



/* =============================================================================
 WYZ_Decode
 Description: Process the next step in the song sequence 
 Input:       -
 Output:      -
============================================================================= */
void WYZ_Decode() __naked; 





// mute functions, 0=off, other=on
//void muteChannelA(u8 value);
//void muteChannelB(u8 value);
//void muteChannelC(u8 value);
//void muteChannelFX(u8 value);