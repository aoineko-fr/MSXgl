// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// ayFX Replayer
//-----------------------------------------------------------------------------
// Based on ayFX Replayer v1.31 by SapphiRe
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
#pragma once

//-----------------------------------------------------------------------------
// DEFINES

// Ports
#define PSG_PORT_REG		0xA0   ///< Used to select a specific register by writing its number (0 to 15)
#define PSG_PORT_WRITE		0xA1   ///< Used to write to any register once it has been selected by the Address Port.	
#define PSG_PORT_READ		0xA2   ///< Used to read any register once it has been selected by the Address Port.

// Channels
#define PSG_CHANNEL_A		0
#define PSG_CHANNEL_B		1
#define PSG_CHANNEL_C		2


#define PSG_REG_TONE_A		0
#define PSG_REG_TONE_B		2
#define PSG_REG_TONE_C		4
#define PSG_REG_NOISE		6
#define PSG_REG_MIXER		7
#define PSG_REG_AMP_A		8
#define PSG_REG_AMP_B		9
#define PSG_REG_AMP_C		10
#define PSG_REG_ENV			11
#define PSG_REG_SHAPE		13

//
enum AYFX_MODE
{
	AYFX_MODE_FIXED   = 0,	///< All sounds are played in the same channel
	AYFX_MODE_FSWITCH = 1,	///< Frame Switch: Channel is switched each frame during FX playback (should less affected the playing music)
	AYFX_MODE_PSWITCH = 2,	///< Play Switch: Channel is switched each time a play occured
};

/// Return values from ayFX_Play()
enum AYFX_ERROR
{
	AYFX_ERROR_NONE  = 0,	///< No errors, the new sample has been correctly initialized
	AYFX_ERROR_PRIO  = 1,	///< Priority error, there's a sample with higher priority being played
	AYFX_ERROR_INDEX = 2,	///< Index error, the current bank don't have a sample with such index
};

extern unsigned char  ayFX_Mode;		///< ayFX mode (@see AYFX_MODE)
extern unsigned short ayFX_Bank;		///< Current ayFX Bank
extern unsigned char  ayFX_Priority;	///< Current ayFX stream priotity
extern unsigned short ayFX_Pointer;	///< Pointer to the current ayFX stream
extern unsigned short ayFX_Tone;		///< Current tone of the ayFX stream
extern unsigned char  ayFX_Noise;		///< Current noise of the ayFX stream
extern unsigned char  ayFX_Volume;		///< Current volume of the ayFX stream
extern unsigned char  ayFX_Channel;	///< PSG channel to play the ayFX stream

typedef void (*Callback)(void);	///< Callback default signature
extern Callback ayFX_Finish; ///< Finish callback

//-----------------------------------------------------------------------------
// PROTOTYPES

/// Setup the ayFX replayer
/// @param		bank		Pointer to the ayFX bank data
void ayFX_InitBank(void* bank) __z88dk_fastcall;

/// Play a ayFX sound
/// @param		id		Sound index in the bank
/// @param		prio	Priority of the sound (0-15). 0 is the highest priority.
/// @return				Error number (if any) @see AYFX_ERROR
#define ayFX_PlayBank(id, prio) ayFX_PlayBankFC(FC88(prio, id))
unsigned char ayFX_PlayBankFC(unsigned short id_prio) __z88dk_fastcall;

/// Play a SFX directly from data pointer
/// @param		file	Pointer to AFX file data
unsigned char ayFX_Play(void* data) __z88dk_fastcall;

/// Stop the sound playback
void ayFX_Stop();

/// Stop the sound playback and mute the channel
void ayFX_Mute();

/// Play a frame of an ayFX stream
void ayFX_Update();

/// Send data to PSG registers
void ayFX_SendToPSG();

//-----------------------------------------------------------------------------
// INLINE FUCTIONS


//-----------------------------------------------------------------------------
/// Set channel mode
inline void ayFX_SetMode(unsigned char mode)
{ 
	ayFX_Mode = mode;
}

//-----------------------------------------------------------------------------
/// Set the channel to use for sound playback
inline void ayFX_SetChannel(unsigned char chan)
{ 
	ayFX_Channel = 3 - chan;
}

//-----------------------------------------------------------------------------
/// Get the channel used for sound playback
inline unsigned char ayFX_GetChannel()
{ 
	return 3 - ayFX_Channel;
}

//-----------------------------------------------------------------------------
/// Get number of sound in the bank
inline unsigned char ayFX_GetBankNumber()
{ 
	return *(unsigned char*)ayFX_Bank;
}

//-----------------------------------------------------------------------------
/// Set the function to be call when a sound ended
inline unsigned char ayFX_SetFinishCB(Callback cb)
{
	ayFX_Finish = cb; 
}
