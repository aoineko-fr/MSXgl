// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄▄▄  ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ██ █ ██▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ██▄▀ ██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// NDP replayer
//─────────────────────────────────────────────────────────────────────────────

#include "msxgl.h"
#include "ndp_player.h"

//=============================================================================
// VARIABLES
//=============================================================================

// Address of the SFX data
const u8* g_NDP_SFXData = NULL;

// NDP work area (defined in NDP_WRK.ASM)
u8   g_NDP_CH1WRK[33];	// Rhythm track
#define CH1WRK						_g_NDP_CH1WRK
u8   g_NDP_RVOLW[28];
#define RVOLW						_g_NDP_RVOLW
NDP_WorkArea   g_NDP_CH2WRK;	// Standard track 1
#define CH2WRK						_g_NDP_CH2WRK
NDP_WorkArea   g_NDP_CH3WRK;	// Standard track 2
#define CH3WRK						_g_NDP_CH3WRK
NDP_WorkArea   g_NDP_CH4WRK;	// Standard track 3
#define CH4WRK						_g_NDP_CH4WRK
u8   g_NDP_SE1WRK[NDP_WSIZE * 3];
#define SE1WRK						_g_NDP_SE1WRK
u8   g_NDP_CH1RWK[NDP_RWSIZE];
#define CH1RWK						_g_NDP_CH1RWK
u8   g_NDP_CH2RWK[NDP_RWSIZE];
#define CH2RWK						_g_NDP_CH2RWK
u8   g_NDP_CH3RWK[NDP_RWSIZE];
#define CH3RWK						_g_NDP_CH3RWK
u8   g_NDP_CH4RWK[NDP_RWSIZE];
#define CH4RWK						_g_NDP_CH4RWK

// ->*************** MSTART 初期化時に0クリアする物

u8   g_NDP_FFFLG;			// Fast-forward flag
#define FFFLG						_g_NDP_FFFLG
u8   g_NDP_ENDTR;			// The bit for tracks whose playback data has reached the end is set (0000321RB) ※Resets on loop
#define ENDTR						_g_NDP_ENDTR
u8   g_NDP_ENDTRW;			// ※ Does not reset even after 1 loop
#define	ENDTRW						_g_NDP_ENDTRW
u8   g_NDP_ENDTRR;			// Value to be written back upon reset (the value set to flag unused tracks at the start of the music)
#define ENDTRR						_g_NDP_ENDTRR
u8   g_NDP_MCOUNT;			// Number of loops
#define MCOUNT						_g_NDP_MCOUNT
u8   g_NDP_MVOL;			// Master volume (Subtract this value)
#define MVOL						_g_NDP_MVOL
u8   g_NDP_MFADE;			// Fade setting (0:No fade, 1–255:Fade count)
#define MFADE						_g_NDP_MFADE
u8   g_NDP_FCOUNT;			// Fade counter
#define FCOUNT						_g_NDP_FCOUNT
u8   g_NDP_LCOUNT;			// Fade loop counter
#define LCOUNT						_g_NDP_LCOUNT
u8   g_NDP_SLWPRM;			// Slow setting
#define SLWPRM						_g_NDP_SLWPRM
u8   g_NDP_SLWCNT;			// Slow playback counter
#define SLWCNT						_g_NDP_SLWCNT
u8   g_NDP_NFREQW;			// Noise frequency
#define NFREQW						_g_NDP_NFREQW
u8   g_NDP_VISPAN;			// Volume interval period
#define VISPAN						_g_NDP_VISPAN
u8   g_NDP_RNON;			// Rhythm tone active status (0:Not active, 1:Active and pitch restored, 2:Active, 3:Normal tone for one frame only)
#define RNON						_g_NDP_RNON
u8   g_NDP_RSVOL;			// Volume reduction before rhythm playback
#define RSVOL						_g_NDP_RSVOL
u16  g_NDP_RPITCH;			// Pitch evasion before rhythm playback
#define RPITCH						_g_NDP_RPITCH
u8   g_NDP_HENVSW;			// Hard envelope number
#define HENVSW						_g_NDP_HENVSW

u8   g_NDP_SEMODE;			// Playback Mode (0:BGM, 1:SE) ※Set within interrupt
#define SEMODE						_g_NDP_SEMODE
u8   g_NDP_SEBAKT[NDP_CHNUM];	// Save the track enable flag for the track being interrupted by sound effects here
#define SEBAKT						_g_NDP_SEBAKT
u8   g_NDP_SEBAKR;			// Whether to trigger register reset at SE end during rhythm key off
#define SEBAKR						_g_NDP_SEBAKR
u8   g_NDP_SECNT;			// Sound effect track counter (Number of sound effect tracks currently playing)
#define SECNT						_g_NDP_SECNT
u8   g_NDP_VADTBL[32];		// Volume embedded address table
#define VADTBL						_g_NDP_VADTBL
u8   g_NDP_RADTBL[64];		// Rhythm tone address table
#define RADTBL						_g_NDP_RADTBL
u8   g_NDP_PADTBL[32];		// Pitch-embedded dressing table
#define PADTBL						_g_NDP_PADTBL
u8   g_NDP_NADTBL[32];		// Note envelope dress table
#define NADTBL						_g_NDP_NADTBL

u8 g_NDP_RVWRK = 0;	// For rhythm volume adjustment
#define RVWRK						_g_NDP_RVWRK
u8 g_NDP_RITRK = 1;	// Interrupted Track (1=CH.C 2=CH.B 3=CH.A)
#define RITRK						_g_NDP_RITRK
u8 g_NDP_RVREG = 10;	// Rhythm track volume register number
#define RVREG						_g_NDP_RVREG
u8 g_NDP_RPREG = 5;	// Rhythm track frequency register number (high)
#define RPREG						_g_NDP_RPREG
u8 g_NDP_RHENV = 0;	// If no S command is executed within the rhythm, it becomes 0.
#define RHENV						_g_NDP_RHENV

u8 g_NDP_OLDTH[5] = { 0xC9, 0xC9, 0xC9, 0xC9, 0xC9	};	// Legacy Timer Interrupt Hook
#define OLDTH						_g_NDP_OLDTH
u8 g_NDP_HKFLG = 0;	// Is the driver connected to the hook?
#define HKFLG						_g_NDP_HKFLG

u8 g_NDP_STATS = 0;	// Playback Status (0: Stopped 1: Playing)
#define STATS						_g_NDP_STATS

u8 g_NDP_FINOUT = 0;	// 0=Fade out; 1=Fade in
#define FINOUT						_g_NDP_FINOUT

u8 g_NDP_MIXNMH = 1;	// Value of *n (mix mode) when using hard envelope
#define MIXNMH						_g_NDP_MIXNMH

u8 g_NDP_MIXWRK = 0b10111000;	// 10NNNTTT 0=ON/1=OFF
#define MIXWRK						_g_NDP_MIXWRK
u8 g_NDP_MIXWRS = 0b10111000;	// Sound Effects Mixing Work
#define MIXWRS						_g_NDP_MIXWRS

u16 g_NDP_HENVPW = 1024; // Hard Envelope Period
#define HENVPW						_g_NDP_HENVPW

u16 g_NDP_BGMADR = 0x4000;	// Song Data Start Address
#define BGMADR						_g_NDP_BGMADR

u16 g_NDP_PTABLE[12*3] = {
// ------	Pitch Table
//	c      c+     d      d+     e      f      f+     g      g+     a      a+     b
	0x0D5D,0x0C9C,0x0BE7,0x0B3C,0x0A9B,0x0A02,0x0973,0x08EB,0x086B,0x07F2,0x0780,0x0714, // O1
	0x06AF,0x064E,0x05F4,0x059E,0x054E,0x0501,0x04BA,0x0476,0x0436,0x03F9,0x03C0,0x038A, // O2
	0x0357,0x0327,0x02FA,0x02CF,0x02A7,0x0281,0x025D,0x023B,0x021B,0x01FD,0x01E0,0x01C5, // O3
};
#define PTABLE						_g_NDP_PTABLE

u16 g_NDP_O4C[12*5] = {
// ------	Pitch Table
//		c      c+     d      d+     e      f      f+     g      g+     a      a+     b
	0x01AC,0x0194,0x017D,0x0168,0x0153,0x0140,0x012E,0x011D,0x010D,0x00FE,0x00F0,0x00E3, // O4
	0x00D6,0x00CA,0x00BE,0x00B4,0x00AA,0x00A0,0x0097,0x008F,0x0087,0x007F,0x0078,0x0071, // O5
	0x006B,0x0065,0x005F,0x005A,0x0055,0x0050,0x004C,0x0047,0x0043,0x0040,0x003C,0x0039, // O6
	0x0035,0x0032,0x0030,0x002D,0x002A,0x0028,0x0026,0x0024,0x0022,0x0020,0x001E,0x001C, // O7
	0x001B,0x0019,0x0018,0x0016,0x0015,0x0014,0x0013,0x0012,0x0011,0x0010,0x000F,0x000E, // O8
};
#define O4C							_g_NDP_O4C

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// NDPINI driver initialization (connects the driver to the timer interrupt hook)
void NDP_Initialize()
{
__asm
	push	ix

	// Clear buffers
	ld		hl, #_g_NDP_CH1WRK;//_g_NDP_RAM0
	ld		de, #_g_NDP_CH1WRK+1;//_g_NDP_RAM0+1
	ld		bc, #691;//_g_NDP_RAM0END-_g_NDP_RAM0-1
	ld		(hl), #0
	ldir

	// // Initilize variables
	// ld		hl, #_g_NDP_RAMVAL
	// ld		de, #_g_NDP_RAMVAR
	// ld		bc, #_g_NDP_RAMVAREND-_g_NDP_RAMVAR
	// ldir

	call	NDP_NDPINI
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// MSTART Start playing (clears the work area to zero and starts playing. Do not operate the hook)
void NDP_Play()
{
__asm
	push	ix
	call	NDP_MSTART
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// MSTOP Stops playback (Stops playback and initializes PSG registers. Does not operate hooks.)
void NDP_Stop()
{
__asm
	push	ix
	call	NDP_MSTOP
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// ADRSET Song data start address setting
void NDP_SetMusicData(const void* addr)
{
	addr; // HL

__asm
	push	ix
	ex		de, hl
	// DE ← address. No setting required if default is 04000H
	call	NDP_ADRSET
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// CH1OFF Ch.A Temporary Mute
void NDP_MuteChannel1(u8 frames)
{
	frames; // A

__asm
	push	ix
	// D ← Number of frames to mute [2 to 254 / 1 = Cancel / 255 = Permanent Mute]
	ld		d, a
	call	NDP_CH1OFF
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// CH2OFF Ch.B Temporary Mute
void NDP_MuteChannel2(u8 frames)
{
	frames; // A

__asm
	push	ix
	// D ← Number of frames to mute [2 to 254 / 1 = Cancel / 255 = Permanent Mute]
	ld		d, a
	call	NDP_CH2OFF
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// CH3OFF Ch.C Temporary Mute
void NDP_MuteChannel3(u8 frames)
{
	frames; // A

__asm
	push	ix
	// D ← Number of frames to mute [2 to 254 / 1 = Cancel / 255 = Permanent Mute]
	ld		d, a
	call	NDP_CH3OFF
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// MVSET Master volume offset (A ← value to subtract from volume value)
void NDP_SetVolume(u8 vol)
{
	vol; // A

__asm
	push	ix
	// A <- Master volume
	call	NDP_MVSET
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// MFOSET Fade-out
void NDP_FadeOut(u8 speed)
{
	speed; // A

__asm
	push	ix
	// A ← number of frames for each stage
	call	NDP_MFOSET
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// MPLAYF Starts playing while fading in (A←number of frames for each stage)
void NDP_PlayFadeIn(u8 speed)
{
	speed; // A

__asm
	push	ix
	// A <- Number of frames for fade
	call	NDP_MPLAYF
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// SEPLAY Sound effect (DE ← sound effect data address)
void NDP_PlaySoundEffect(const void* addr)
{
	addr; // HL

__asm
	push	ix
	ex		de, hl
	// DE <- Sound effect data address
	call	NDP_SEPLAY
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// Play sound effect
void NDP_PlaySFX(u8 sfx)
{
	u16 addr = (u16)g_NDP_SFXData + NDP_GetSFXOffset(sfx);
	NDP_PlaySoundEffect((const void*)addr);
}


// //-----------------------------------------------------------------------------
// // RDSTAT Playing status → A (0: Stopped 1: Playing)
// u8 NDP_GetStatus() __NAKED
// {
// __asm
// 	call	NDP_RDSTAT
// 	ret
// __endasm;
// }

// //-----------------------------------------------------------------------------
// // RDENDT Whether or not each track has reached the end → A (bit field of 0000321R)
// u8 NDP_HasTrackEnded() __NAKED
// {
// __asm
// 	call	NDP_RDENDT
// 	ret
// __endasm;
// }

// //-----------------------------------------------------------------------------
// // RDLOOP Number of times the song has looped → A (If the song does not have an infinite loop, check +0BH in the song data and return 255 when the song finishes playing)
// u8 NDP_GetLoopCount() __NAKED
// {
// __asm
// 	call	NDP_RDLOOP
// 	ret
// __endasm;
// }

//-----------------------------------------------------------------------------
// VSET Reflects tone color data to the driver (HL ← address of tone color definition data)
void NDP_SetToneColor(const void* addr)
{
	addr; // HL

__asm
	push	ix
	// HL <- Tone color definition data address
	call	NDP_VSET
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// SYSVER Driver version number → HL (L = decimal point, H = integer part. If the integer part is 0, it is treated as 0.9)
u16 NDP_GetVersion() __NAKED
{
__asm
	call	NDP_SYSVER
	ret
__endasm;
}

//-----------------------------------------------------------------------------
// NDPOFF Driver end (Disconnects the driver from the timer interrupt hook and stops sound generation)
void NDP_Release()
{
__asm
	push	ix
	call	NDP_NDPOFF
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// IMAIN Interrupt processing - main part (only the interrupt routine body is processed without calling the old hook)
void NDP_Update()
{
__asm
	push	ix
	call	NDP_IMAIN
	pop		ix
__endasm;
}

//-----------------------------------------------------------------------------
// Dummy function to include the NDP player ASM code into the build
void NDP_DummyASM()
{
__asm

	// Assembler driver
	// #include "NDP_WRK.ASM"
	#include "NDP_DRV.ASM"

__endasm;
}