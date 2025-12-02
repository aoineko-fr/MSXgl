// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄            
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄█ ██▄▀ ██▄▀ ▄█▀▄  ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ██   ██ █ ▀█▄▀ ▄██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Arkos AKM replayer (Minimalist)
//
// Arkos Tracker and the Z80 players are coded and designed by Julien Névo a.k.a Targhan/Arkos.
//  https://www.julien-nevo.com/arkostracker/index.php/credits/
//─────────────────────────────────────────────────────────────────────────────
#include "akm_player.h"

bool g_AKM_Playing = FALSE;
bool g_AKM_EndOfSong;

#define AKM_ADDR(a)					(a - 0xF000 + AKM_BUFFER_ADDR) 

//-----------------------------------------------------------------------------
//
void AKM_Dummy() __NAKED
{
__asm
	PLY_AKM_CHANNEL1_SOUNDEFFECTDATA = AKM_ADDR(0xf057)
	PLY_AKM_CHANNEL2_SOUNDEFFECTDATA = AKM_ADDR(0xf05f)
	PLY_AKM_CHANNEL3_SOUNDEFFECTDATA = AKM_ADDR(0xf067)
	PLY_AKM_DEFAULTSTARTINSTRUMENTINTRACKS = AKM_ADDR(0xf00f)
	PLY_AKM_DEFAULTSTARTNOTEINTRACKS = AKM_ADDR(0xf00e)
	PLY_AKM_DEFAULTSTARTWAITINTRACKS = AKM_ADDR(0xf010)
	PLY_AKM_FLAGNOTEANDEFFECTINCELL = AKM_ADDR(0xf015)
	PLY_AKM_LINKERPREVIOUSREMAININGHEIGHT = AKM_ADDR(0xf017)
	PLY_AKM_MIXERREGISTER = AKM_ADDR(0xf046)
	PLY_AKM_NOISEREGISTER = AKM_ADDR(0xf042)
	PLY_AKM_NOTEINDEXTABLE = AKM_ADDR(0xf008)
	PLY_AKM_PATTERNREMAININGHEIGHT = AKM_ADDR(0xf016)
	PLY_AKM_PRIMARYINSTRUMENT = AKM_ADDR(0xf011)
	PLY_AKM_PRIMARYWAIT = AKM_ADDR(0xf013)
	PLY_AKM_PTARPEGGIOS = AKM_ADDR(0xf002)
	PLY_AKM_PTINSTRUMENTS = AKM_ADDR(0xf000)
	PLY_AKM_PTLINKER = AKM_ADDR(0xf006)
	PLY_AKM_PTPITCHES = AKM_ADDR(0xf004)
	PLY_AKM_PTSOUNDEFFECTTABLE = AKM_ADDR(0xf055)
	PLY_AKM_REG11 = AKM_ADDR(0xf04a)
	PLY_AKM_REG12 = AKM_ADDR(0xf04e)
	PLY_AKM_RT_READEFFECTSFLAG = AKM_ADDR(0xf01c)
	PLY_AKM_SAVESP = AKM_ADDR(0xf00c)
	PLY_AKM_SECONDARYINSTRUMENT = AKM_ADDR(0xf012)
	PLY_AKM_SECONDARYWAIT = AKM_ADDR(0xf014)
	PLY_AKM_SETREG13 = AKM_ADDR(0xf01b)
	PLY_AKM_SETREG13OLD = AKM_ADDR(0xf01a)
	PLY_AKM_SPEED = AKM_ADDR(0xf018)
	PLY_AKM_TICKCOUNTER = AKM_ADDR(0xf019)
	PLY_AKM_TRACK1_ESCAPEINSTRUMENT = AKM_ADDR(0xf074)
	PLY_AKM_TRACK1_ESCAPENOTE = AKM_ADDR(0xf073)
	PLY_AKM_TRACK1_ESCAPEWAIT = AKM_ADDR(0xf075)
	PLY_AKM_TRACK1_PTINSTRUMENT = AKM_ADDR(0xf076)
	PLY_AKM_TRACK1_REGISTERS = AKM_ADDR(0xf01d)
	PLY_AKM_TRACK1_TRANSPOSITION = AKM_ADDR(0xf06d)
	PLY_AKM_TRACK1_VOLUME = AKM_ADDR(0xf01e)
	PLY_AKM_TRACK1_WAITEMPTYCELL = AKM_ADDR(0xf06c)
	PLY_AKM_TRACK2_ESCAPEINSTRUMENT = AKM_ADDR(0xf09a)
	PLY_AKM_TRACK2_ESCAPENOTE = AKM_ADDR(0xf099)
	PLY_AKM_TRACK2_ESCAPEWAIT = AKM_ADDR(0xf09b)
	PLY_AKM_TRACK2_PTINSTRUMENT = AKM_ADDR(0xf09c)
	PLY_AKM_TRACK2_REGISTERS = AKM_ADDR(0xf029)
	PLY_AKM_TRACK2_VOLUME = AKM_ADDR(0xf02a)
	PLY_AKM_TRACK2_WAITEMPTYCELL = AKM_ADDR(0xf092)
	PLY_AKM_TRACK3_ESCAPEINSTRUMENT = AKM_ADDR(0xf0c0)
	PLY_AKM_TRACK3_ESCAPENOTE = AKM_ADDR(0xf0bf)
	PLY_AKM_TRACK3_ESCAPEWAIT = AKM_ADDR(0xf0c1)
	PLY_AKM_TRACK3_PTINSTRUMENT = AKM_ADDR(0xf0c2)
	PLY_AKM_TRACK3_REGISTERS = AKM_ADDR(0xf035)
	PLY_AKM_TRACK3_VOLUME = AKM_ADDR(0xf036)
	PLY_AKM_TRACK3_WAITEMPTYCELL = AKM_ADDR(0xf0b8)
	PLY_AKM_TRACKINDEX = AKM_ADDR(0xf00a)

	#include "akm_player.asm"
__endasm;
}

//-----------------------------------------------------------------------------
// Initialize music and start playback
void AKM_Play(u8 sng, const void* data) __NAKED
{
	sng;	// A
	data;	// DE

__asm
	ex		de, hl
	// Initializes the player.
	// IN:    HL = music address.
	//        A = subsong index (>=0).
	push	ix
	call	_PLY_AKM_INIT
	pop		ix
	ld		a, #TRUE
	ld		(_g_AKM_Playing), a
	ret
__endasm;
}

//-----------------------------------------------------------------------------
// Stop music playback
void AKM_Stop() __NAKED
{
__asm
	xor		a
	ld		(_g_AKM_Playing), a
	// Stops the music. This code can be removed if you don't intend to stop it!
	push	ix
	call	_PLY_AKM_STOP
	pop		ix
	ret
__endasm;
}

//-----------------------------------------------------------------------------
// Decode a music frame and update the PSG
bool AKM_Update() __NAKED
{
__asm
	ld		a, (_g_AKM_Playing)
	or		a
	ret		z
	xor		a
	ld		(_g_AKM_EndOfSong), a
	// Plays one frame of the subsong.
	push	ix
	call	_PLY_AKM_PLAY
	pop		ix
	ld		a, (_g_AKM_EndOfSong)
	ret
__endasm;
}

//-----------------------------------------------------------------------------
// Initializes the sound effects. It MUST be called at any times before a first sound effect is triggered.
// It doesn't matter whether the song is playing or not, or if it has been initialized or not.
//
// Paramaters:
//   data	- Address to the sound effects data.
u8 AKM_InitSFX(const void* data) __NAKED
{
	data;	// HL

__asm
	// Initializes the sound effects. It MUST be called at any times before a first sound effect is triggered.
	// It doesn't matter whether the song is playing or not, or if it has been initialized or not.
	// IN:    HL = Address to the sound effects data.
	push	ix
	call	_PLY_AKM_INITSOUNDEFFECTS
	pop		ix
	ld		a, #TRUE
	ld		(_g_AKM_Playing), a
	ld		a, (hl)				// Inchanged by Arkos routine
	srl		a
	ret
__endasm;
}

//-----------------------------------------------------------------------------
// Plays a sound effect. If a previous one was already playing on the same channel, it is replaced.
// This does not actually plays the sound effect, but programs its playing.
//
// Paramaters:
//   sfx	- Sound effect number (>0!).
//   chan	- The channel where to play the sound effect (0, 1, 2).
//   vol	- Inverted volume (0 = full volume, 16 = no sound). Hardware sounds are also lowered.
void AKM_PlaySFX(u8 sfx, u8 chan, u8 vol) __NAKED
{
	sfx;	// A
	chan;	// L
	vol;	// SP[2]

__asm
	pop		de					// Retreive return address
	dec		sp					// Adjust Stack-pointer
	pop		bc					// Retreive vol in B
	ld		c, l
	push	de
#if (AKM_SFX_STARTIDX == 0)
	inc		a
#endif
	// Plays a sound effect. If a previous one was already playing on the same channel, it is replaced.
	// This does not actually plays the sound effect, but programs its playing.
	// The music player, when called, will call the PLY_AKM_PlaySoundEffectsStream method below.
	// IN:    A = Sound effect number (>0!).
	//        C = The channel where to play the sound effect (0, 1, 2).
	//        B = Inverted volume (0 = full volume, 16 = no sound). Hardware sounds are also lowered.
	push	ix
	call	_PLY_AKM_PLAYSOUNDEFFECT
	pop		ix
	ret
__endasm;
}


//-----------------------------------------------------------------------------
// Stops a sound effect. Nothing happens if there was no sound effect.
//
// Paramaters:
//   chan	- The channel where to stop the sound effect (0, 1, 2).
void AKM_StopSFX(u8 chan) __NAKED
{
	chan;	// A

__asm
	// Stops a sound effect. Nothing happens if there was no sound effect.
	// IN:    A = The channel where to stop the sound effect (0, 1, 2).
	push	ix
	call	_PLY_AKM_STOPSOUNDEFFECTFROMCHANNEL
	pop		ix
	ret
__endasm;
}