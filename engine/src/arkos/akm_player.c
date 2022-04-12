// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄            
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄█ ██▄▀ ██▄▀ ▄█▀▄  ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ██   ██ █ ▀█▄▀ ▄██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
//  Arkos AKM replayer
//─────────────────────────────────────────────────────────────────────────────
// Arkos Tracker and the Z80 players are coded and designed by Julien Névo a.k.a Targhan/Arkos.
// https://www.julien-nevo.com/arkostracker/index.php/credits/
//─────────────────────────────────────────────────────────────────────────────
#include "akm_player.h"

//-----------------------------------------------------------------------------
//
void AKM_Dummy()
{
	__asm
		#include "akm_player.asm"
	__endasm;
}

//-----------------------------------------------------------------------------
//
void AKM_Init(const void* data, u16 sng)
{
	data; // HL
	sng;  // DE
	__asm	
		ld			a, e // convert C parameter
		push		ix
		// Initialize a music. HL=music address, A=subsong index (>=0)
		call		_PLY_AKM_INIT
		pop			ix

	__endasm;
}

//-----------------------------------------------------------------------------
//
void AKM_Stop()
{
	__asm
		push		ix
		call		_PLY_AKM_STOP
		pop			ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void AKM_Decode()
{
	__asm
		push		ix
		call		_PLY_AKM_PLAY
		pop			ix
	__endasm;
}