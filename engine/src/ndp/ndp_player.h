// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄▄▄  ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ██ █ ██▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ██▄▀ ██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// NDP replayer
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

// NDP replayer pamarameters
#define HTIMI		0xFD9F	// Timer interrupt hook
#define WSIZE		61		// The size of the work area for each track
#define RWSIZE		(5*4)	// Repeat work area size for each track (Size per nest * Number of nests)F
#define CHNUM		4		// Number of channels used
#define DRVADR		0xC000	// Driver start address

// NDP work area (defined in NDP_WRK.ASM)
extern u8   g_NDP_CH1WRK[WSIZE];
extern u8   g_NDP_CH2WRK[WSIZE];
extern u8   g_NDP_CH3WRK[WSIZE];
extern u8   g_NDP_CH4WRK[WSIZE];
extern u8   g_NDP_SE1WRK[WSIZE * 3];
extern u8   g_NDP_CH1RWK[RWSIZE];
extern u8   g_NDP_CH2RWK[RWSIZE];
extern u8   g_NDP_CH3RWK[RWSIZE];
extern u8   g_NDP_CH4RWK[RWSIZE];
extern u8   g_NDP_FFFLG;
extern u8   g_NDP_ENDTR;
extern u8   g_NDP_ENDTRW;
extern u8   g_NDP_ENDTRR;
extern u8   g_NDP_MCOUNT;
extern u8   g_NDP_MVOL;
extern u8   g_NDP_MFADE;
extern u8   g_NDP_FCOUNT;
extern u8   g_NDP_LCOUNT;
extern u8   g_NDP_SLWPRM;
extern u8   g_NDP_SLWCNT;
extern u8   g_NDP_NFREQW;
extern u8   g_NDP_VISPAN;
extern u8   g_NDP_RNON;
extern u8   g_NDP_RSVOL;
extern u16  g_NDP_RPITCH;
extern u8   g_NDP_HENVSW;
extern u8   g_NDP_SEMODE;
extern u8   g_NDP_SEBAKT[CHNUM];
extern u8   g_NDP_SEBAKR;
extern u8   g_NDP_SECNT;
extern u8   g_NDP_VADTBL[32];
extern u8   g_NDP_RADTBL[64];
extern u8   g_NDP_PADTBL[32];
extern u8   g_NDP_NADTBL[32];
extern u8   g_NDP_RVWRK;
extern u8   g_NDP_RITRK;
extern u8   g_NDP_RVREG;
extern u8   g_NDP_RPREG;
extern u8   g_NDP_RHENV;
extern u8   g_NDP_OLDTH[5];
extern u8   g_NDP_HKFLG;
extern u8   g_NDP_STATS;
extern u8   g_NDP_FINOUT;
extern u8   g_NDP_MIXNMH;
extern u8   g_NDP_MIXWRK;
extern u8   g_NDP_MIXWRS;
extern u16  g_NDP_HENVPW;
extern u16  g_NDP_BGMADR;
extern u16  g_NDP_PTABLE[12 * 8];
extern void g_NDP_RAM0;
extern void g_NDP_RAM0END;
extern void g_NDP_RAMVAR;
extern void g_NDP_RAMVAREND;
extern void g_NDP_RAMVAL;
extern void g_NDP_RAMVALEND;
extern u16  g_NDP_RAM0SIZE;
extern u16  g_NDP_RAMVARSIZE;

// NDP player status
enum NDP_STATUS
{
	NDP_STATUS_STOPPED = 0,
	NDP_STATUS_PLAYING = 1
};

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: NDP_Initialize
// Driver initialization (connects the driver to the timer interrupt hook)
void NDP_Initialize();

// Function: NDP_Play
// Start playing (clears the work area to zero and starts playing. Do not operate the hook)
void NDP_Play();

// Function: NDP_Stop
// Stops playback (Stops playback and initializes PSG registers. Does not operate hooks.)
void NDP_Stop();

// Function: NDP_SetMusicAddress
// Song data start address setting
//
// Parameters:
//  addr - Address of the song data. No setting required if default is 04000H)
void NDP_SetMusicAddress(const void* addr);

// Function: NDP_MuteChannel1
// Channel A Temporary Mute
//
// Parameters:
//   frames - Number of frames to mute [2 to 254 / 1 = Cancel / 255 = Permanent Mute]
void NDP_MuteChannel1(u8 frames);

// Function: NDP_MuteChannel2
// Channel B Temporary Mute
//
// Parameters:
//   frames - Number of frames to mute [2 to 254 / 1 = Cancel / 255 = Permanent Mute]
void NDP_MuteChannel2(u8 frames);

// Function: NDP_MuteChannel3
// Channel C Temporary Mute
//
// Parameters:
//   frames - Number of frames to mute [2 to 254 / 1 = Cancel / 255 = Permanent Mute]
void NDP_MuteChannel3(u8 frames);

// Function: NDP_MuteChannel
// Channel Temporary Mute
//
// Parameters:
//   channel - Channel number (1 to 3)
//   frames - Number of frames to mute [2 to 254 / 1 = Cancel / 255 = Permanent Mute]
inline void NDP_MuteChannel(u8 channel, u8 frames)
{
	switch (channel)
	{
	case 1: NDP_MuteChannel1(frames); break;
	case 2: NDP_MuteChannel2(frames); break;
	case 3: NDP_MuteChannel3(frames); break;
	}
}

// Function: NDP_SetVolume
// Master volume offset
//
// Parameters:
//   vol - Master volume (value to subtract from volume value)
void NDP_SetVolume(u8 vol);

// Function: NDP_FadeOut
// Fade-out
//
// Parameters:
//   speed - Number of frames for each stage
void NDP_FadeOut(u8 speed);

// Function: NDP_PlayFadeIn
// Starts playing while fading in
//
// Parameters:
//   speed - Number of frames for each stage
void NDP_PlayFadeIn(u8 speed);

// Function: NDP_PlaySoundEffect
// Sound effect
//
// Parameters:
//   addr - Address of the sound effect data
void NDP_PlaySoundEffect(const void* addr);

// Function: NDP_GetStatus
// Playing status (NDP_STATUS_xxx)
//
// Returns:
//   0: Stopped, 1: Playing
inline u8 NDP_GetStatus() { return g_NDP_STATS; }

// Function: NDP_IsPlaying
// Whether or not music is currently playing
//
// Returns:
//   TRUE if music is playing, FALSE otherwise
inline bool NDP_IsPlaying() { return NDP_GetStatus() == NDP_STATUS_PLAYING; }

// Function: NDP_IsStopped
// Whether or not music is currently stopped
//
// Returns:
//   TRUE if music is stopped, FALSE otherwise
inline bool NDP_IsStopped() { return NDP_GetStatus() == NDP_STATUS_STOPPED; }

// Function: NDP_HasTrackEnded
// Whether or not each track has reached the end
//
// Returns:
//   Bit field of 0000321R
inline u8 NDP_HasTrackEnded() { return g_NDP_STATS; }

// Function: NDP_GetLoopCount
// Number of times the song has looped
//
// Returns:
//   If the song does not have an infinite loop, check +0BH in the song data and return 255 when the song finishes playing
inline u8 NDP_GetLoopCount() { return g_NDP_MCOUNT; }

// Function: NDP_SetToneColor
// Reflects tone color data to the driver (HL ← address of tone color definition data)
void NDP_SetToneColor(const void* addr);

// Function: NDP_GetVersion
// Driver version number → HL (L = decimal point, H = integer part. If the integer part is 0, it is treated as 0.9)
u16 NDP_GetVersion();

// Function: NDP_Release
// Driver end (Disconnects the driver from the timer interrupt hook and stops sound generation)
void NDP_Release();

// Function: NDP_SetHookFlag
// Timer interrupt hook connection flag ← A
void NDP_SetHookFlag(u8 flag);

// Function: NDP_Update
// Interrupt processing - main part (only the interrupt routine body is processed without calling the old hook)
void NDP_Update();
