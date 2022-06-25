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

const void* g_TriloSCC_ToneTable;
const void* g_TriloSFX_Bank;
const void* g_TriloSFX_WaveTable;
u8 g_TriloSCC_Freq;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void TriloSCC_Dummy()
{
	__asm

	#ifdef TRILO_USE_SFXPLAY
	SFXPLAY_ENABLED		= 1 // Enable the SFX functionality.
	#else
	SFXPLAY_ENABLED		= 0 // Enable the SFX functionality.
	#endif

	#ifdef TRILO_USE_TREMOLO
	TREMOLO_OFF			= 0 // Removes tremolo code making the replayer a little bit faster.
	#else
	TREMOLO_OFF			= 1 // Removes tremolo code making the replayer a little bit faster.
	#endif

	#ifdef TRILO_USE_TAIL
	TAIL_ON				= 1 // Limit minimal volume to 1.
	#else
	TAIL_ON				= 0 // Limit minimal volume to 1.
	#endif

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
// Initialize replayer data. Only call this on start-up.
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
// Stop/Restart music playback.
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
// Fade out the music. Once the sound is silence the replayer is paused.
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
// Set the main volume for the SCC chip.
// This enables for setting the balance between SCC and PSG as some MSX models default balance differs.
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
// Set the main volume for the PSG chip.
// This enables for setting the balance between SCC and PSG as some MSX models default balance differs.
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
// Initialize a music for playback.
void TriloSCC_LoadMusic(const void* addr)
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
// Set mixer values to silent.
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
// Decode music data and process instruments and effects.
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
// Output the data to the CHIP registers
void TriloSCC_Apply()
{
	__asm
		push	ix

		ld		a, #0x3F				// enable SCC
		ld		(0x9000), a

		// ---replay_route
		// Output the data to the CHIP registers
		call	replay_route

		ld		a, (_g_Bank2Segment)	// disable SCC
		ld		(0x9000), a

		pop		ix
	__endasm;
}


//-----------------------------------------------------------------------------
// Initialise the sfx player. Sets initial priorities, volume balancce and initial SCC waveform.
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
// Set the main SFX volume for the SCC chip.
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
// Set the main SFXvolume for the PSG chip.
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
// Start a new SFX.
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
// Decode SFX streams.
void TriloSFX_Update()
{
	__asm
		//===========================================================
		// Play both sfx streams
		//===========================================================
		call	ttsfx_play
	__endasm;
}
