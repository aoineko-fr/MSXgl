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

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// NDPINI driver initialization (connects the driver to the timer interrupt hook)
void NDP_Initialize()
{
__asm
	push	ix

	ld		hl, #_g_NDP_RAM0
	ld		de, #_g_NDP_RAM0+1
	ld		bc, #_g_NDP_RAM0END-_g_NDP_RAM0-1
	ld		(hl), #0
	ldir

	ld		hl, #_g_NDP_RAMVAL
	ld		de, #_g_NDP_RAMVAR
	ld		bc, #_g_NDP_RAMVAREND-_g_NDP_RAMVAR
	ldir

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
	#include "NDP_WRK.ASM"
	#include "NDP_DRV.ASM"

__endasm;
}