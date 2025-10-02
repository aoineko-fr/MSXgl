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
void AKY_Play(u8 sng, const void* data) __NAKED
{
	sng;	// A
	data;	// DE

__asm
	ex		de, hl
	// Initializes the player.
	// IN:    HL = music address.
	//        A = subsong index (>=0).
	push	ix
	call	_PLY_AKY_INIT
	pop		ix
	ret
__endasm;
}

//-----------------------------------------------------------------------------
// Stop music playback
/*void AKY_Stop() __NAKED
{
__asm
	push		ix
	call		_PLY_AKY_STOP
	pop			ix
	ret
__endasm;
}*/

//-----------------------------------------------------------------------------
// Decode a music frame and update the PSG
bool AKY_Decode() __NAKED
{
__asm
	xor		a
	ld		(_g_AKY_EndOfSong), a
	// Plays one frame of the subsong
	push	ix
	call	_PLY_AKY_PLAY
	pop		ix
	ld		a, (_g_AKY_EndOfSong)
	ret
__endasm;
}