// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄      ▄  ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ██▄▀ ▄  ██  ▄█▀▄
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │   ██  ██   ██ ▀█▄ ▀█▄▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// Trilo Tracker SCC replayer
//
// Trilo Tracker and the Z80 players are coded and designed by Cornelisser.
//─────────────────────────────────────────────────────────────────────────────
#include "trilo_scc_player.h"

//-----------------------------------------------------------------------------
//
void TriloSCC_Dummy()
{
	__asm
		#include "trilo_scc_player.asm"
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_Initialize()
{
	__asm
		// clear RAM
		ld			bc, #0x300 // 768
		ld			hl, #_SP_Storage
		ld			de, #_SP_Storage+1
		ld			(hl), #0
		ldir	

		push		ix
		// ---	replay_init
		// Initialize replayer data
		// Only call this on start-up
		// Input: none
		call		replay_init
		pop			ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_Pause()
{
	__asm
		push		ix
		// ---	replay_pause
		// Stop/Restart music playback
		// Input: none
		call		replay_pause
		pop			ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_FadeOut(u8 speed)
{
	speed; // A
	__asm
		push		ix
		// ---	replay_fade_out
		// Fade out the music. 
		// Once the sound is silence the replayer is paused.
		// in: [A] fade speed (1 - 255)
		call		replay_fade_out
		pop			ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_SetBalanceSCC(u8 vol)
{
	vol; // A
	__asm
		push		ix
		// ---	replay_set_SCC_balance
		// Set the main volume for the SCC chip. This enables for
		// setting the balance between SCC and PSG as some MSX models 
		// default balance differs. 
		// in: [A] master volume (0-7) 0=halve volume, 8=full volume. 
		call		replay_set_SCC_balance
		pop			ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_SetBalancePSG(u8 vol)
{
	vol; // A
	__asm
		push		ix
		// ---	replay_set_PSG_balance
		// Set the main volume for the PSG chip. This enables for
		// setting the balance between SCC and PSG as some MSX models 
		// default balance differs. 
		// in: [A] master volume (0-7) 0=halve volume, 8=full volume. 
		call		replay_set_PSG_balance
		pop			ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_Equalization(bool enable)
{
	enable; // A
	__asm
		push		ix
		// ---	replay_equalization
		// Enables or disables the speed equalization. 
		// This to enable same speed playback on 50 and 60Hz. 
		// in: [A] setting of the equalization (0 = off, other values = on) 
		call		replay_equalization
		pop			ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_LoadSong(u16 addr)
{
	addr; // HL
	__asm
		push		ix
		// ---	replay_loadsong
		// Initialize a song for playback
		// Input [HL] points to start song
		call		replay_loadsong
		pop			ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_Silent()
{
	__asm
		push		ix
		// Set mixer values to silent.
		call		replay_play_no
		pop			ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_Play()
{
	__asm
		push		ix
		// ---	replay_play
		// Decode music data and process instruments and effects. 
		// Music chip registers will be prepared for replay_route 
		// Input none
		call		replay_play
		pop			ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_Apply()
{
	__asm
		push		ix
		// ---replay_route
		// Output the data	to the CHIP	registers
		call		replay_route
		pop			ix
	__endasm;
}
