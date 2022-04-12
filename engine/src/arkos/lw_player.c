// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄            
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄█ ██▄▀ ██▄▀ ▄█▀▄  ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ██   ██ █ ▀█▄▀ ▄██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
//  Arkos LW replayer
//─────────────────────────────────────────────────────────────────────────────
// Arkos Tracker and the Z80 players are coded and designed by Julien Névo a.k.a Targhan/Arkos.
// https://www.julien-nevo.com/arkostracker/index.php/credits/
//─────────────────────────────────────────────────────────────────────────────
#include "lw_player.h"

//-----------------------------------------------------------------------------
//
void LW_Dummy()
{
	__asm
		#include "lw_player.asm"
	__endasm;
}

//-----------------------------------------------------------------------------
//
void LW_Init(const void* data, u16 sng)
{
	data; // HL
	sng;  // DE
	__asm	
		ld			a, e // convert C parameter
		push		ix
		// Initialize a music. HL=music address, A=subsong index (>=0)
		call		_PLY_LW_INIT
		pop			ix

	__endasm;
}

//-----------------------------------------------------------------------------
//
void LW_Stop()
{
	__asm
		push		ix
		call		_PLY_LW_STOP
		pop			ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void LW_Decode()
{
	__asm
		push		ix
		call		_PLY_LW_PLAY
		pop			ix
	__endasm;
}