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

//-----------------------------------------------------------------------------
//
void AKY_Dummy()
{
	__asm
		#include "aky_player.asm"
	__endasm;
}

//-----------------------------------------------------------------------------
// Initialize music and start playback
void AKY_Init(const void* data, u8 sng) __NAKED
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
		call	_PLY_AKY_INIT
		pop		ix
		ret
	__endasm;
}

//-----------------------------------------------------------------------------
// Stop music playback
/*void AKY_Stop()
{
	__asm
		push		ix
		call		_PLY_AKY_STOP
		pop			ix
	__endasm;
}*/

//-----------------------------------------------------------------------------
// Decode a music frame and update the PSG
void AKY_Decode()
{
	__asm
		push		ix
		call		_PLY_AKY_PLAY
		pop			ix
	__endasm;
}