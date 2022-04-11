// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄            
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄█ ██▄▀ ██▄▀ ▄█▀▄  ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ██   ██ █ ▀█▄▀ ▄██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
//  Arkos AKY replayer
//─────────────────────────────────────────────────────────────────────────────
// Arkos Tracker and the Z80 players are coded and designed by Julien Névo a.k.a Targhan/Arkos.
// https://www.julien-nevo.com/arkostracker/index.php/credits/
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
//
void AKY_Init(const void* data, u16 sng)
{
	data; // HL
	sng;  // DE
	__asm	
		ld			a, e // convert C parameter
		push		ix
		// Initialize a music. HL=music address, A=subsong index (>=0)
		call		_PLY_AKY_INIT
		pop			ix

	__endasm;
}

//-----------------------------------------------------------------------------
//
/*void AKY_Stop()
{
	__asm
		push		ix
		call		_PLY_AKY_STOP
		pop			ix
	__endasm;
}*/

//-----------------------------------------------------------------------------
//
void AKY_Decode()
{
	__asm
		push		ix
		call		_PLY_AKY_PLAY
		pop			ix
	__endasm;
}