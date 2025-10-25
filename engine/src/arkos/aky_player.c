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

#define AKY_ADDR(a)					(a - 0xF000 + AKY_BUFFER_ADDR) 

//-----------------------------------------------------------------------------
//
void AKY_Dummy() __NAKED
{
__asm
	PLY_AKY_CHANNEL1_PTREGISTERBLOCK = AKY_ADDR(0xf017)
	PLY_AKY_CHANNEL1_PTTRACK = AKY_ADDR(0xf011)
	PLY_AKY_CHANNEL1_REGISTERBLOCKLINESTATE_OPCODE = AKY_ADDR(0xf003)
	PLY_AKY_CHANNEL1_WAITBEFORENEXTREGISTERBLOCK = AKY_ADDR(0xf000)
	PLY_AKY_CHANNEL2_PTREGISTERBLOCK = AKY_ADDR(0xf019)
	PLY_AKY_CHANNEL2_PTTRACK = AKY_ADDR(0xf013)
	PLY_AKY_CHANNEL2_REGISTERBLOCKLINESTATE_OPCODE = AKY_ADDR(0xf004)
	PLY_AKY_CHANNEL2_WAITBEFORENEXTREGISTERBLOCK = AKY_ADDR(0xf001)
	PLY_AKY_CHANNEL3_PTREGISTERBLOCK = AKY_ADDR(0xf01b)
	PLY_AKY_CHANNEL3_PTTRACK = AKY_ADDR(0xf015)
	PLY_AKY_CHANNEL3_REGISTERBLOCKLINESTATE_OPCODE = AKY_ADDR(0xf005)
	PLY_AKY_CHANNEL3_WAITBEFORENEXTREGISTERBLOCK = AKY_ADDR(0xf002)
	PLY_AKY_PATTERNFRAMECOUNTER = AKY_ADDR(0xf00f)
	PLY_AKY_PSGREGISTER11 = AKY_ADDR(0xf007)
	PLY_AKY_PSGREGISTER12 = AKY_ADDR(0xf008)
	PLY_AKY_PSGREGISTER13 = AKY_ADDR(0xf009)
	PLY_AKY_PSGREGISTER13_RETRIG = AKY_ADDR(0xf00a)
	PLY_AKY_PSGREGISTER6 = AKY_ADDR(0xf006)
	PLY_AKY_PTLINKER = AKY_ADDR(0xf00b)
	PLY_AKY_SAVESP = AKY_ADDR(0xf00d)

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
bool AKY_Update() __NAKED
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