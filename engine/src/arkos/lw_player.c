// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄            
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄█ ██▄▀ ██▄▀ ▄█▀▄  ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ██   ██ █ ▀█▄▀ ▄██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Arkos LW replayer (Lightweight)
//
// Arkos Tracker and the Z80 players are coded and designed by Julien Névo a.k.a Targhan/Arkos.
//  https://www.julien-nevo.com/arkostracker/index.php/credits/
//─────────────────────────────────────────────────────────────────────────────
#include "lw_player.h"

bool g_AKL_Playing = FALSE;
bool g_AKL_EndOfSong;

//-----------------------------------------------------------------------------
//
void AKL_Dummy()
{
	__asm
		#include "lw_player.asm"
	__endasm;
}

//-----------------------------------------------------------------------------
// Initialize music and start playback
void AKL_Init(const void* data, u8 sng) __NAKED
{
	data;	// HL
	sng;	// SP[2]
	__asm
		pop		bc					// Retreive return address
		dec		sp					// Adjust Stack-pointer
		pop		af					// Retreive sng in A
		push	bc
		// Initializes the player.
		// IN:    HL = music address.
		//        A = subsong index (>=0).
		push	ix
		call	_PLY_LW_INIT
		pop		ix
		ld		a, #TRUE
		ld		(_g_AKL_Playing), a
		ret
	__endasm;
}

//-----------------------------------------------------------------------------
// Stop music playback
void AKL_Stop()
{
	__asm
		xor		a
		ld		(_g_AKL_Playing), a
		// Stops the music. This code can be removed if you don't intend to stop it!
		push	ix
		call	_PLY_LW_STOP
		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
// Decode a music frame and update the PSG
bool AKL_Decode()
{
	__asm
		ld		a, (_g_AKL_Playing)
		or		a
		ret		z
		xor		a
		ld		(_g_AKL_EndOfSong), a
		// Plays one frame of the subsong.
		push	ix
		call	_PLY_LW_PLAY
		pop		ix
		ld		a, (_g_AKG_EndOfSong)
	__endasm;
}

//-----------------------------------------------------------------------------
// Initializes the sound effects. It MUST be called at any times before a first sound effect is triggered.
// It doesn't matter whether the song is playing or not, or if it has been initialized or not.
//
// Paramaters:
//   data	- Address to the sound effects data.
u8 AKL_InitSFX(const void* data)
{
	data;	// HL
	__asm
		// Initializes the sound effects. It MUST be called at any times before a first sound effect is triggered.
		// It doesn't matter whether the song is playing or not, or if it has been initialized or not.
		// IN:    HL = Address to the sound effects data.
		call	_PLY_LW_INITSOUNDEFFECTS
		ld		a, #TRUE
		ld		(_g_AKL_Playing), a
		ld		a, (hl)				// Inchanged by Arkos routine
		srl		a
	__endasm;
}

//-----------------------------------------------------------------------------
// Plays a sound effect. If a previous one was already playing on the same channel, it is replaced.
// This does not actually plays the sound effect, but programs its playing.
//
// Paramaters:
//   sfx	- Sound effect number (>0!).
//   chan	- The channel where to play the sound effect (0, 1, 2).
//   vol	- Inverted volume (0 = full volume, 16 = no sound). Hardware sounds are also lowered.
void AKL_PlaySFX(u8 sfx, u8 chan, u8 vol) __NAKED
{
	sfx;	// A
	chan;	// L
	vol;	// SP[2]
	__asm
		pop		de					// Retreive return address
		dec		sp					// Adjust Stack-pointer
		pop		bc					// Retreive vol in B
		ld		c, l
		push	de
		inc		a
		// Plays a sound effect. If a previous one was already playing on the same channel, it is replaced.
		// This does not actually plays the sound effect, but programs its playing.
		// The music player, when called, will call the PLY_LW_PlaySoundEffectsStream method below.
		// IN:    A = Sound effect number (>0!).
		//        C = The channel where to play the sound effect (0, 1, 2).
		//        B = Inverted volume (0 = full volume, 16 = no sound). Hardware sounds are also lowered.
		push	ix
		call	_PLY_AKL_PLAYSOUNDEFFECT
		pop		ix
		ret
	__endasm;
}


//-----------------------------------------------------------------------------
// Stops a sound effect. Nothing happens if there was no sound effect.
//
// Paramaters:
//   chan	- The channel where to stop the sound effect (0, 1, 2).
void AKL_StopSFX(u8 chan)
{
	chan;	// A
	__asm
		// Stops a sound effect. Nothing happens if there was no sound effect.
		// IN:    A = The channel where to stop the sound effect (0, 1, 2).
		push	ix
		call	_PLY_AKL_STOPSOUNDEFFECTFROMCHANNEL
		pop		ix
	__endasm;
}