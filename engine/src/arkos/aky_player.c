// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄            
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄█ ██▄▀ ██▄▀ ▄█▀▄  ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ██   ██ █ ▀█▄▀ ▄██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Arkos AKY replayer (Fastest)
//
// Arkos Tracker and the Z80 players are coded and designed by Julien Névo a.k.a Targhan/Arkos.
//  https://www.julien-nevo.com/arkostracker/index.php/credits/
//─────────────────────────────────────────────────────────────────────────────
#include "aky_player.h"

bool g_AKY_EndOfSong;

//-----------------------------------------------------------------------------
//
void AKY_Dummy() __NAKED
{
__asm
	#include "aky_player.asm"
__endasm;
}

//-----------------------------------------------------------------------------
// Initialize music and start playback
void AKY_Play(const void* data)
{
	data;
__asm
	// Initializes the player.
	// IN:    HL = music address.
	call	_PLY_AKY_INIT

	ret
__endasm;
}

//-----------------------------------------------------------------------------
// Decode a music frame and update the PSG
bool AKY_Update() __NAKED
{
__asm
	xor		a
	ld		(_g_AKY_EndOfSong), a
	// Plays the music. It must have been initialized before.
	// The interruption SHOULD be disabled (DI), as the stack is heavily used.
	push	ix
	ARKOS_INT_DISABLE
	call	_PLY_AKY_PLAY
	ARKOS_INT_ENABLE
	pop		ix

	ld		a, (_g_AKY_EndOfSong)
	ret
__endasm;
}

//-----------------------------------------------------------------------------
// Initialize sound effects. It MUST be called at any time before a first sound effect is triggered.
// It doesn't matter whether a song is playing or not, or even if AKY_Play has been called.
//
// Paramaters:
//   data	- Address to the sound effects data.
u8 AKY_InitSFX(const void* data) __NAKED
{
	data;	// HL

__asm
	// Initializes the sound effects.
	// IN:    HL = Address to the sound effects data.
	call	_PLY_AKY_INITSOUNDEFFECTS

	ld		a, (hl)				// Inchanged by Arkos routine (first byte is num*2)
	srl		a
	ret
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
void AKY_PlaySFX(u8 sfx, u8 chan, u8 vol) __NAKED
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
#if (ARKOS_SFX_START_IDX == 0)
	inc		a
#endif
	// IN:    A = Sound effect number (>0!).
	//        C = The channel where to play the sound effect (0, 1, 2).
	//        B = Inverted volume (0 = full volume, 16 = no sound). Hardware sounds are also lowered.
	call	_PLY_AKY_PLAYSOUNDEFFECT

	ret
__endasm;
}

//-----------------------------------------------------------------------------
// Stops a sound effect. Nothing happens if there was no sound effect.
//
// Paramaters:
//   chan	- The channel where to stop the sound effect (0, 1, 2).
void AKY_StopSFX(u8 chan) __NAKED
{
	chan;	// A

__asm
	// IN:    A = The channel where to stop the sound effect (0, 1, 2).
	call	_PLY_AKY_STOPSOUNDEFFECTFROMCHANNEL

	ret
__endasm;
}
