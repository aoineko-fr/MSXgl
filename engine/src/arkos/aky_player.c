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