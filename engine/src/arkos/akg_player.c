// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄            
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄█ ██▄▀ ██▄▀ ▄█▀▄  ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ██   ██ █ ▀█▄▀ ▄██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
//  Arkos AKG replayer
//─────────────────────────────────────────────────────────────────────────────
// Arkos Tracker and the Z80 players are coded and designed by Julien Névo a.k.a Targhan/Arkos.
// https://www.julien-nevo.com/arkostracker/index.php/credits/
//─────────────────────────────────────────────────────────────────────────────
#include "akg_player.h"

//-----------------------------------------------------------------------------
//
void AKG_Dummy()
{
	__asm
		#include "akg_player.asm"
	__endasm;
}

//-----------------------------------------------------------------------------
//
void AKG_Init(const void* data, u16 sng)
{
	data; // HL
	sng;  // DE
	__asm	
		ld			a, e // convert C parameter
		push		ix
		// Initialize a music. HL=music address, A=subsong index (>=0)
		call		_PLY_AKG_INIT
		pop			ix

	__endasm;
}

//-----------------------------------------------------------------------------
//
void AKG_Stop()
{
	__asm
		push		ix
		call		_PLY_AKG_STOP
		pop			ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void AKG_Decode()
{
	__asm
		push		ix
		call		_PLY_AKG_PLAY
		pop			ix
	__endasm;
}