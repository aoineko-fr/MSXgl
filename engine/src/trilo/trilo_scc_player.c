// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄      ▄  ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ██▄▀ ▄  ██  ▄█▀▄
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │   ██  ██   ██ ▀█▄ ▀█▄▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Trilo Tracker SCC replayer
//
// Trilo Tracker and the Z80 players are coded and designed by Cornelisser.
//─────────────────────────────────────────────────────────────────────────────
#include "trilo_scc_player.h"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

//=============================================================================
// MEMORY DATA
//=============================================================================

u16 g_TriloSCC_ToneTable;
u16 g_TriloSFX_Bank;
u16 g_TriloSFX_WaveTable;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void TriloSCC_Dummy()
{
	__asm
	.area _DATA
	replay_ram_start::
		#include "scc_player/ttreplaySCCRAM.asm"
		#include "scc_player/ttsfxplay_RAM.asm"
	replay_ram_end::

	.area _CODE
		#include "scc_player/ttreplaySCC.asm"
		#include "scc_player/ttreplaySCCDAT.asm"
		#include "scc_player/ttsfxplay.asm"
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_Initialize()
{
	__asm
		push	ix

		// clear RAM (first KB only)
		ld		bc, #replay_ram_end-replay_ram_start-1
		ld		hl, #replay_ram_start
		ld		de, #replay_ram_start+1
		ld		(hl), #0
		ldir

		// ---	replay_init
		// Initialize replayer data
		// Only call this on start-up
		// Input: none
		call	replay_init

		// Set the default PSG note table
		ld		hl, #TRACK_ToneTable_PSG
		ld		(replay_tonetable), hl

		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_Pause()
{
	__asm
		push	ix

		// ---	replay_pause
		// Stop/Restart music playback
		// Input: none
		call	replay_pause

		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_FadeOut(u8 speed)
{
	speed; // A
	__asm
		push	ix

		// ---	replay_fade_out
		// Fade out the music. 
		// Once the sound is silence the replayer is paused.
		// in: [A] fade speed (1 - 255)
		call	replay_fade_out

		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_SetBalanceSCC(u8 vol)
{
	vol; // A
	__asm
		push	ix

		// ---	replay_set_SCC_balance
		// Set the main volume for the SCC chip. This enables for
		// setting the balance between SCC and PSG as some MSX models 
		// default balance differs. 
		// in: [A] master volume (0-7) 0=halve volume, 8=full volume. 
		call	replay_set_SCC_balance

		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_SetBalancePSG(u8 vol)
{
	vol; // A
	__asm
		push	ix

		// ---	replay_set_PSG_balance
		// Set the main volume for the PSG chip. This enables for
		// setting the balance between SCC and PSG as some MSX models 
		// default balance differs. 
		// in: [A] master volume (0-7) 0=halve volume, 8=full volume. 
		call	replay_set_PSG_balance

		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_Equalization(bool enable)
{
	enable; // A
	__asm
		push	ix

		// ---	replay_equalization
		// Enables or disables the speed equalization. 
		// This to enable same speed playback on 50 and 60Hz. 
		// in: [A] setting of the equalization (0 = off, other values = on) 
		call	replay_equalization

		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_LoadSong(u16 addr)
{
	addr; // HL
	__asm
		push	ix

		// ---	replay_loadsong
		// Initialize a song for playback
		// Input [HL] points to start song
		call	replay_loadsong

		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_Silent()
{
	__asm
		push	ix

		// Set mixer values to silent.
		call	replay_play_no

		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_Update()
{
	__asm
		push	ix

		ld		a, #0x3F				// enable SCC
		ld		(0x9000), a

		// ---	replay_play
		// Decode music data and process instruments and effects. 
		// Music chip registers will be prepared for replay_route 
		// Input none
		call	replay_play

		ld		a, (_g_Bank2Segment)	// disable SCC
		ld		(0x9000), a

		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSCC_Apply()
{
	__asm
		push	ix

		ld		a, #0x3F				// enable SCC
		ld		(0x9000), a

		// ---replay_route
		// Output the data	to the CHIP	registers
		call	replay_route

		ld		a, (_g_Bank2Segment)	// disable SCC
		ld		(0x9000), a

		pop		ix
	__endasm;
}


//-----------------------------------------------------------------------------
//
void TriloSFX_Initialize()
{
	__asm
		//===========================================================
		// ---	ttsfx_init
		// Initialise the sfx player. Sets initial priorities, volume
		// balancce and initial SCC waveform.
		//===========================================================
		call	ttsfx_init
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSFX_SetBalanceSCC(u8 vol)
{
	vol; // A
	__asm
		//===========================================================
		// ---	ttsfx_scc_balance
		// Set the main SFX volume for the SCC chip.
		//
		// in: [A] master volume (0-15) 0=silenced, 15=full volume.
		//===========================================================
		call	ttsfx_scc_balance
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSFX_SetBalancePSG(u8 vol)
{
	vol; // A
	__asm
		//===========================================================
		// ---	ttsfx_psg_balance
		// Set the main SFXvolume for the PSG chip.
		//
		// in: [A] master volume (0-15) 0=silenced, 15=full volume.
		//===========================================================
		call	ttsfx_psg_balance
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSFX_Play(u8 sfx, u8 prio)
{
	sfx; // A
	prio; // L
	__asm
		//===========================================================
		// --- Start a new SFX
		// --- Input B -> sfx number
		// --- Input C -> priority 0 = lowest, 255 is highest
		//===========================================================
		ld		b, a
		ld		c, l
		call	ttsfx_start
	__endasm;
}

//-----------------------------------------------------------------------------
//
void TriloSFX_Update()
{
	__asm
		//===========================================================
		// Play both sfx streams
		//===========================================================
		call	ttsfx_play
	__endasm;
}
