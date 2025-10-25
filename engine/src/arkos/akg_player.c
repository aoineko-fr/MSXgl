// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄            
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄█ ██▄▀ ██▄▀ ▄█▀▄  ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ██   ██ █ ▀█▄▀ ▄██ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Arkos AKG replayer (Generic)
//
// Arkos Tracker and the Z80 players are coded and designed by Julien Névo a.k.a Targhan/Arkos.
//  https://www.julien-nevo.com/arkostracker/index.php/credits/
//─────────────────────────────────────────────────────────────────────────────
#include "akg_player.h"

//=============================================================================
// DEFINES
//=============================================================================

#define AKG_ADDR(a)					(a - 0xF000 + AKG_BUFFER_ADDR) 

//=============================================================================
// VARIABLES
//=============================================================================

bool g_AKG_Playing = FALSE;
bool g_AKG_EndOfSong;
AKG_Event g_AKG_EventCallback = NULL;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void AKG_Dummy() __NAKED
{
__asm
	PLY_AKG_ARPEGGIOSTABLE = AKG_ADDR(0xf011)
	PLY_AKG_BASENOTEINDEX = AKG_ADDR(0xf002)
	PLY_AKG_CHANNEL1_ARPEGGIOBASESPEED = AKG_ADDR(0xf03c)
	PLY_AKG_CHANNEL1_ARPEGGIOTABLE = AKG_ADDR(0xf048)
	PLY_AKG_CHANNEL1_ARPEGGIOTABLEBASE = AKG_ADDR(0xf04a)
	PLY_AKG_CHANNEL1_ARPEGGIOTABLECURRENTSTEP = AKG_ADDR(0xf03a)
	PLY_AKG_CHANNEL1_ARPEGGIOTABLESPEED = AKG_ADDR(0xf03d)
	PLY_AKG_CHANNEL1_GENERATEDCURRENTARPNOTE = AKG_ADDR(0xf03b)
	PLY_AKG_CHANNEL1_GENERATEDCURRENTINVERTEDVOLUME = AKG_ADDR(0xf045)
	PLY_AKG_CHANNEL1_GENERATEDCURRENTPITCH = AKG_ADDR(0xf069)
	PLY_AKG_CHANNEL1_GLIDEDIRECTION = AKG_ADDR(0xf044)
	PLY_AKG_CHANNEL1_GLIDETOREACH = AKG_ADDR(0xf060)
	PLY_AKG_CHANNEL1_GLIDE_SAVEHL = AKG_ADDR(0xf062)
	PLY_AKG_CHANNEL1_INSTRUMENTORIGINALSPEED = AKG_ADDR(0xf037)
	PLY_AKG_CHANNEL1_INSTRUMENTSPEED = AKG_ADDR(0xf038)
	PLY_AKG_CHANNEL1_INSTRUMENTSTEP = AKG_ADDR(0xf039)
	PLY_AKG_CHANNEL1_INVERTEDVOLUMEINTEGER = AKG_ADDR(0xf057)
	PLY_AKG_CHANNEL1_INVERTEDVOLUMEINTEGERANDDECIMAL = AKG_ADDR(0xf056)
	PLY_AKG_CHANNEL1_ISARPEGGIOTABLE = AKG_ADDR(0xf034)
	PLY_AKG_CHANNEL1_ISPITCH = AKG_ADDR(0xf036)
	PLY_AKG_CHANNEL1_ISPITCHTABLE = AKG_ADDR(0xf035)
	PLY_AKG_CHANNEL1_ISVOLUMESLIDE = AKG_ADDR(0xf033)
	PLY_AKG_CHANNEL1_PITCH = AKG_ADDR(0xf058)
	PLY_AKG_CHANNEL1_PITCHBASESPEED = AKG_ADDR(0xf03f)
	PLY_AKG_CHANNEL1_PITCHTABLE = AKG_ADDR(0xf04e)
	PLY_AKG_CHANNEL1_PITCHTABLEBASE = AKG_ADDR(0xf050)
	PLY_AKG_CHANNEL1_PITCHTABLECURRENTSTEP = AKG_ADDR(0xf03e)
	PLY_AKG_CHANNEL1_PITCHTABLESPEED = AKG_ADDR(0xf040)
	PLY_AKG_CHANNEL1_PITCHTRACK = AKG_ADDR(0xf04c)
	PLY_AKG_CHANNEL1_PITCHTRACKADDORSBC_16BITS = AKG_ADDR(0xf06b)
	PLY_AKG_CHANNEL1_PITCHTRACKAFTERADDORSBCJUMPINSTRANDADDRESS = AKG_ADDR(0xf06d)
	PLY_AKG_CHANNEL1_PITCHTRACKDECIMALCOUNTER = AKG_ADDR(0xf042)
	PLY_AKG_CHANNEL1_PITCHTRACKDECIMALINSTR = AKG_ADDR(0xf064)
	PLY_AKG_CHANNEL1_PITCHTRACKDECIMALINSTRANDVALUERETURNJP = AKG_ADDR(0xf066)
	PLY_AKG_CHANNEL1_PITCHTRACKDECIMALVALUE = AKG_ADDR(0xf065)
	PLY_AKG_CHANNEL1_PITCHTRACKINTEGERADDORSUB = AKG_ADDR(0xf070)
	PLY_AKG_CHANNEL1_PITCHTRACKINTEGERAFTERADDORSUBJUMPINSTRANDADDRESS = AKG_ADDR(0xf071)
	PLY_AKG_CHANNEL1_PTBASEINSTRUMENT = AKG_ADDR(0xf05c)
	PLY_AKG_CHANNEL1_PTINSTRUMENT = AKG_ADDR(0xf05a)
	PLY_AKG_CHANNEL1_PTTRACK = AKG_ADDR(0xf046)
	PLY_AKG_CHANNEL1_SOUNDEFFECTDATA = AKG_ADDR(0xf0fc)
	PLY_AKG_CHANNEL1_SOUNDSTREAM_RELATIVEMODIFIERADDRESS = AKG_ADDR(0xf031)
	PLY_AKG_CHANNEL1_TRACKNOTE = AKG_ADDR(0xf043)
	PLY_AKG_CHANNEL1_VOLUMESLIDEVALUE = AKG_ADDR(0xf05e)
	PLY_AKG_CHANNEL1_WAITCOUNTER = AKG_ADDR(0xf032)
	PLY_AKG_CHANNEL2_ARPEGGIOBASESPEED = AKG_ADDR(0xf07f)
	PLY_AKG_CHANNEL2_ARPEGGIOTABLE = AKG_ADDR(0xf08b)
	PLY_AKG_CHANNEL2_ARPEGGIOTABLEBASE = AKG_ADDR(0xf08d)
	PLY_AKG_CHANNEL2_ARPEGGIOTABLECURRENTSTEP = AKG_ADDR(0xf07d)
	PLY_AKG_CHANNEL2_ARPEGGIOTABLESPEED = AKG_ADDR(0xf080)
	PLY_AKG_CHANNEL2_GENERATEDCURRENTARPNOTE = AKG_ADDR(0xf07e)
	PLY_AKG_CHANNEL2_GENERATEDCURRENTINVERTEDVOLUME = AKG_ADDR(0xf088)
	PLY_AKG_CHANNEL2_GENERATEDCURRENTPITCH = AKG_ADDR(0xf0ac)
	PLY_AKG_CHANNEL2_GLIDEDIRECTION = AKG_ADDR(0xf087)
	PLY_AKG_CHANNEL2_GLIDETOREACH = AKG_ADDR(0xf0a3)
	PLY_AKG_CHANNEL2_GLIDE_SAVEHL = AKG_ADDR(0xf0a5)
	PLY_AKG_CHANNEL2_INSTRUMENTORIGINALSPEED = AKG_ADDR(0xf07a)
	PLY_AKG_CHANNEL2_INSTRUMENTSPEED = AKG_ADDR(0xf07b)
	PLY_AKG_CHANNEL2_INSTRUMENTSTEP = AKG_ADDR(0xf07c)
	PLY_AKG_CHANNEL2_INVERTEDVOLUMEINTEGER = AKG_ADDR(0xf09a)
	PLY_AKG_CHANNEL2_INVERTEDVOLUMEINTEGERANDDECIMAL = AKG_ADDR(0xf099)
	PLY_AKG_CHANNEL2_ISARPEGGIOTABLE = AKG_ADDR(0xf077)
	PLY_AKG_CHANNEL2_ISPITCH = AKG_ADDR(0xf079)
	PLY_AKG_CHANNEL2_ISPITCHTABLE = AKG_ADDR(0xf078)
	PLY_AKG_CHANNEL2_ISVOLUMESLIDE = AKG_ADDR(0xf076)
	PLY_AKG_CHANNEL2_PITCH = AKG_ADDR(0xf09b)
	PLY_AKG_CHANNEL2_PITCHBASESPEED = AKG_ADDR(0xf082)
	PLY_AKG_CHANNEL2_PITCHTABLE = AKG_ADDR(0xf091)
	PLY_AKG_CHANNEL2_PITCHTABLEBASE = AKG_ADDR(0xf093)
	PLY_AKG_CHANNEL2_PITCHTABLECURRENTSTEP = AKG_ADDR(0xf081)
	PLY_AKG_CHANNEL2_PITCHTABLESPEED = AKG_ADDR(0xf083)
	PLY_AKG_CHANNEL2_PITCHTRACK = AKG_ADDR(0xf08f)
	PLY_AKG_CHANNEL2_PITCHTRACKADDORSBC_16BITS = AKG_ADDR(0xf0ae)
	PLY_AKG_CHANNEL2_PITCHTRACKAFTERADDORSBCJUMPINSTRANDADDRESS = AKG_ADDR(0xf0b0)
	PLY_AKG_CHANNEL2_PITCHTRACKDECIMALCOUNTER = AKG_ADDR(0xf085)
	PLY_AKG_CHANNEL2_PITCHTRACKDECIMALINSTR = AKG_ADDR(0xf0a7)
	PLY_AKG_CHANNEL2_PITCHTRACKDECIMALINSTRANDVALUERETURNJP = AKG_ADDR(0xf0a9)
	PLY_AKG_CHANNEL2_PITCHTRACKDECIMALVALUE = AKG_ADDR(0xf0a8)
	PLY_AKG_CHANNEL2_PITCHTRACKINTEGERADDORSUB = AKG_ADDR(0xf0b3)
	PLY_AKG_CHANNEL2_PITCHTRACKINTEGERAFTERADDORSUBJUMPINSTRANDADDRESS = AKG_ADDR(0xf0b4)
	PLY_AKG_CHANNEL2_PLAYINSTRUMENT_RELATIVEMODIFIERADDRESS = AKG_ADDR(0xf074)
	PLY_AKG_CHANNEL2_PTBASEINSTRUMENT = AKG_ADDR(0xf09f)
	PLY_AKG_CHANNEL2_PTINSTRUMENT = AKG_ADDR(0xf09d)
	PLY_AKG_CHANNEL2_PTTRACK = AKG_ADDR(0xf089)
	PLY_AKG_CHANNEL2_SOUNDEFFECTDATA = AKG_ADDR(0xf104)
	PLY_AKG_CHANNEL2_TRACKNOTE = AKG_ADDR(0xf086)
	PLY_AKG_CHANNEL2_VOLUMESLIDEVALUE = AKG_ADDR(0xf0a1)
	PLY_AKG_CHANNEL2_WAITCOUNTER = AKG_ADDR(0xf075)
	PLY_AKG_CHANNEL3_ARPEGGIOBASESPEED = AKG_ADDR(0xf0c2)
	PLY_AKG_CHANNEL3_ARPEGGIOTABLE = AKG_ADDR(0xf0ce)
	PLY_AKG_CHANNEL3_ARPEGGIOTABLEBASE = AKG_ADDR(0xf0d0)
	PLY_AKG_CHANNEL3_ARPEGGIOTABLECURRENTSTEP = AKG_ADDR(0xf0c0)
	PLY_AKG_CHANNEL3_ARPEGGIOTABLESPEED = AKG_ADDR(0xf0c3)
	PLY_AKG_CHANNEL3_GENERATEDCURRENTARPNOTE = AKG_ADDR(0xf0c1)
	PLY_AKG_CHANNEL3_GENERATEDCURRENTINVERTEDVOLUME = AKG_ADDR(0xf0cb)
	PLY_AKG_CHANNEL3_GENERATEDCURRENTPITCH = AKG_ADDR(0xf0ef)
	PLY_AKG_CHANNEL3_GLIDEDIRECTION = AKG_ADDR(0xf0ca)
	PLY_AKG_CHANNEL3_GLIDETOREACH = AKG_ADDR(0xf0e6)
	PLY_AKG_CHANNEL3_GLIDE_SAVEHL = AKG_ADDR(0xf0e8)
	PLY_AKG_CHANNEL3_INSTRUMENTORIGINALSPEED = AKG_ADDR(0xf0bd)
	PLY_AKG_CHANNEL3_INSTRUMENTSPEED = AKG_ADDR(0xf0be)
	PLY_AKG_CHANNEL3_INSTRUMENTSTEP = AKG_ADDR(0xf0bf)
	PLY_AKG_CHANNEL3_INVERTEDVOLUMEINTEGER = AKG_ADDR(0xf0dd)
	PLY_AKG_CHANNEL3_INVERTEDVOLUMEINTEGERANDDECIMAL = AKG_ADDR(0xf0dc)
	PLY_AKG_CHANNEL3_ISARPEGGIOTABLE = AKG_ADDR(0xf0ba)
	PLY_AKG_CHANNEL3_ISPITCH = AKG_ADDR(0xf0bc)
	PLY_AKG_CHANNEL3_ISPITCHTABLE = AKG_ADDR(0xf0bb)
	PLY_AKG_CHANNEL3_ISVOLUMESLIDE = AKG_ADDR(0xf0b9)
	PLY_AKG_CHANNEL3_PITCH = AKG_ADDR(0xf0de)
	PLY_AKG_CHANNEL3_PITCHBASESPEED = AKG_ADDR(0xf0c5)
	PLY_AKG_CHANNEL3_PITCHTABLE = AKG_ADDR(0xf0d4)
	PLY_AKG_CHANNEL3_PITCHTABLEBASE = AKG_ADDR(0xf0d6)
	PLY_AKG_CHANNEL3_PITCHTABLECURRENTSTEP = AKG_ADDR(0xf0c4)
	PLY_AKG_CHANNEL3_PITCHTABLESPEED = AKG_ADDR(0xf0c6)
	PLY_AKG_CHANNEL3_PITCHTRACK = AKG_ADDR(0xf0d2)
	PLY_AKG_CHANNEL3_PITCHTRACKADDORSBC_16BITS = AKG_ADDR(0xf0f1)
	PLY_AKG_CHANNEL3_PITCHTRACKAFTERADDORSBCJUMPINSTRANDADDRESS = AKG_ADDR(0xf0f3)
	PLY_AKG_CHANNEL3_PITCHTRACKDECIMALCOUNTER = AKG_ADDR(0xf0c8)
	PLY_AKG_CHANNEL3_PITCHTRACKDECIMALINSTR = AKG_ADDR(0xf0ea)
	PLY_AKG_CHANNEL3_PITCHTRACKDECIMALINSTRANDVALUERETURNJP = AKG_ADDR(0xf0ec)
	PLY_AKG_CHANNEL3_PITCHTRACKDECIMALVALUE = AKG_ADDR(0xf0eb)
	PLY_AKG_CHANNEL3_PITCHTRACKINTEGERADDORSUB = AKG_ADDR(0xf0f6)
	PLY_AKG_CHANNEL3_PITCHTRACKINTEGERAFTERADDORSUBJUMPINSTRANDADDRESS = AKG_ADDR(0xf0f7)
	PLY_AKG_CHANNEL3_PTBASEINSTRUMENT = AKG_ADDR(0xf0e2)
	PLY_AKG_CHANNEL3_PTINSTRUMENT = AKG_ADDR(0xf0e0)
	PLY_AKG_CHANNEL3_PTTRACK = AKG_ADDR(0xf0cc)
	PLY_AKG_CHANNEL3_SOUNDEFFECTDATA = AKG_ADDR(0xf10c)
	PLY_AKG_CHANNEL3_SOUNDSTREAM_RELATIVEMODIFIERADDRESS = AKG_ADDR(0xf0b7)
	PLY_AKG_CHANNEL3_TRACKNOTE = AKG_ADDR(0xf0c9)
	PLY_AKG_CHANNEL3_VOLUMESLIDEVALUE = AKG_ADDR(0xf0e4)
	PLY_AKG_CHANNEL3_WAITCOUNTER = AKG_ADDR(0xf0b8)
	PLY_AKG_CHANNEL_READEFFECTS_EFFECTBLOCKS1 = AKG_ADDR(0xf017)
	PLY_AKG_CHANNEL_READEFFECTS_ENDJUMPINSTRANDADDRESS = AKG_ADDR(0xf029)
	PLY_AKG_CHANNEL_RE_READNEXTEFFECTINBLOCK = AKG_ADDR(0xf00a)
	PLY_AKG_CURRENTSPEED = AKG_ADDR(0xf001)
	PLY_AKG_EFFECT_GLIDEWITHNOTESAVEDE = AKG_ADDR(0xf02c)
	PLY_AKG_EMPTYINSTRUMENTDATAPT = AKG_ADDR(0xf019)
	PLY_AKG_EVENT = AKG_ADDR(0xf000)
	PLY_AKG_EVENTTRACK_PTTRACK = AKG_ADDR(0xf00f)
	PLY_AKG_EVENTTRACK_WAITCOUNTER = AKG_ADDR(0xf006)
	PLY_AKG_INSTRUMENTSTABLE = AKG_ADDR(0xf015)
	PLY_AKG_PATTERNDECREASINGHEIGHT = AKG_ADDR(0xf003)
	PLY_AKG_PITCHESTABLE = AKG_ADDR(0xf013)
	PLY_AKG_PSGHARDWAREPERIOD_INSTR = AKG_ADDR(0xf027)
	PLY_AKG_PSGREG01_INSTR = AKG_ADDR(0xf01d)
	PLY_AKG_PSGREG10 = AKG_ADDR(0xf026)
	PLY_AKG_PSGREG13_INSTR = AKG_ADDR(0xf008)
	PLY_AKG_PSGREG13_OLDVALUE = AKG_ADDR(0xf007)
	PLY_AKG_PSGREG23_INSTR = AKG_ADDR(0xf01f)
	PLY_AKG_PSGREG45_INSTR = AKG_ADDR(0xf021)
	PLY_AKG_PSGREG6_8_INSTR = AKG_ADDR(0xf023)
	PLY_AKG_PSGREG8 = AKG_ADDR(0xf024)
	PLY_AKG_PSGREG9_10_INSTR = AKG_ADDR(0xf025)
	PLY_AKG_PTSOUNDEFFECTTABLE = AKG_ADDR(0xf0fa)
	PLY_AKG_READLINKER_PTLINKER = AKG_ADDR(0xf00b)
	PLY_AKG_RETRIG = AKG_ADDR(0xf009)
	PLY_AKG_SAVESP = AKG_ADDR(0xf01b)
	PLY_AKG_SPEEDTRACK_PTTRACK = AKG_ADDR(0xf00d)
	PLY_AKG_SPEEDTRACK_WAITCOUNTER = AKG_ADDR(0xf005)
	PLY_AKG_TEMPPLAYINSTRUMENTJUMPINSTRANDADDRESS = AKG_ADDR(0xf02e)
	PLY_AKG_TICKDECREASINGCOUNTER = AKG_ADDR(0xf004)

	#include "akg_player.asm"
__endasm;
}

//-----------------------------------------------------------------------------
// Initialize music and start playback
void AKG_Play(u8 sng, const void* data) __NAKED
{
	sng;	// A
	data;	// DE

__asm
	ex		de, hl
	// Initializes the player.
	// IN:    HL = music address.
	//        A = subsong index (>=0).
	push	ix
	call	_PLY_AKG_INIT
	pop		ix
	ld		a, #TRUE
	ld		(_g_AKG_Playing), a	// g_AKG_Playing = TRUE;
	ret
__endasm;
}

//-----------------------------------------------------------------------------
// Stop music playback
void AKG_Stop() __NAKED
{
__asm
	xor		a
	ld		(_g_AKG_Playing), a	// g_AKG_Playing = FALSE;
	// Stops the music. This code can be removed if you don't intend to stop it!
	push	ix
	call	_PLY_AKG_STOP
	pop		ix
	ret
__endasm;
}

//-----------------------------------------------------------------------------
// Decode a music frame and update the PSG
bool AKG_Update() __NAKED
{
__asm
	ld		a, (_g_AKG_Playing)
	or		a
	ret		z
	xor		a
	ld		(_g_AKG_EndOfSong), a
	// Plays one frame of the subsong.
	push	ix
	call	_PLY_AKG_PLAY
	pop		ix

#if (AKG_USE_EVENT)
    // Is there any event? 0 means "no effect".
    ld		a, (PLY_AKG_Event)
    or		a
    jr		z, akg_update_noevents
	// Event callback
	ld		hl, _g_AKG_EventCallback
	call	(hl)
akg_update_noevents:
#endif

	ld		a, (_g_AKG_EndOfSong)
	ret								// Return value in A
__endasm;
}

//-----------------------------------------------------------------------------
// Initialize sound effects. It MUST be called at any time before a first sound effect is triggered.
// It doesn't matter whether a song is playing or not, or even if AKG_Init has been initialized.
//
// Paramaters:
//   data	- Address to the sound effects data.
u8 AKG_InitSFX(const void* data) __NAKED
{
	data;	// HL

__asm
	// Initializes the sound effects. It MUST be called at any times before a first sound effect is triggered.
	// It doesn't matter whether the song is playing or not, or if it has been initialized or not.
	// IN:    HL = Address to the sound effects data.
	call	_PLY_AKG_INITSOUNDEFFECTS
	ld		a, #TRUE
	ld		(_g_AKG_Playing), a	// g_AKG_Playing = TRUE;
	ld		a, (hl)				// Inchanged by Arkos routine (first byte is num*2)
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
void AKG_PlaySFX(u8 sfx, u8 chan, u8 vol) __NAKED
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
#if (AKG_SFX_STARTIDX == 0)
	inc		a
#endif
	// Plays a sound effect. If a previous one was already playing on the same channel, it is replaced.
	// This does not actually plays the sound effect, but programs its playing.
	// The music player, when called, will call the PLY_AKG_PlaySoundEffectsStream method below.
	// IN:    A = Sound effect number (>0!).
	//        C = The channel where to play the sound effect (0, 1, 2).
	//        B = Inverted volume (0 = full volume, 16 = no sound). Hardware sounds are also lowered.
	push	ix
	call	_PLY_AKG_PLAYSOUNDEFFECT
	pop		ix
	ret
__endasm;
}


//-----------------------------------------------------------------------------
// Stops a sound effect. Nothing happens if there was no sound effect.
//
// Paramaters:
//   chan	- The channel where to stop the sound effect (0, 1, 2).
void AKG_StopSFX(u8 chan) __NAKED
{
	chan;	// A

__asm
	// Stops a sound effect. Nothing happens if there was no sound effect.
	// IN:    A = The channel where to stop the sound effect (0, 1, 2).
	push	ix
	call	_PLY_AKG_STOPSOUNDEFFECTFROMCHANNEL
	pop		ix
	ret
__endasm;
}
