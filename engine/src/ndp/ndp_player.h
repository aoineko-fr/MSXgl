#include "msxgl.h"

// Driver initialization (connects the driver to the timer interrupt hook)
void NDP_Initialize();

// Start playing (clears the work area to zero and starts playing. Do not operate the hook)
void NDP_Play();

// Stops playback (Stops playback and initializes PSG registers. Does not operate hooks.)
void NDP_Stop();

// Interrupt processing (used when calling an interrupt routine without connecting to a hook) *Executes together with the old hook saved in the driver
void NDP_Interrupt();

// Song data start address setting (DE ← address. No setting required if default is 04000H)
void NDP_SetMusicAddress(const void* addr);

// Ch.A Temporary Mute (D ← Number of frames to mute [2 to 254 / 1 = Cancel / 255 = Permanent Mute])
void NDP_MuteChannel1(u8 frames);

// CH2OFF Ch.B 〃
void NDP_MuteChannel2(u8 frames);

// CH3OFF Ch.C 〃
void NDP_MuteChannel3(u8 frames);

// Master volume offset (A ← value to subtract from volume value)
void NDP_SetVolume(u8 vol);

// Fade-out (A←number of frames for each stage)
void NDP_FadeOut(u8 speed);

// Starts playing while fading in (A←number of frames for each stage)
void NDP_PlayFadeIn(u8 speed);

// Sound effect (DE ← sound effect data address)
void NDP_PlaySoundEffect(const void* addr);

// Playing status → A (0: Stopped 1: Playing)
u8 NDP_GetStatus();

// Whether or not each track has reached the end → A (bit field of 0000321R)
u8 NDP_HasTrackEnded();

// Number of times the song has looped → A (If the song does not have an infinite loop, check +0BH in the song data and return 255 when the song finishes playing)
u8 NDP_GetLoopCount();

// Reflects tone color data to the driver (HL ← address of tone color definition data)
void NDP_SetToneColor(const void* addr);

// Driver version number → HL (L = decimal point, H = integer part. If the integer part is 0, it is treated as 0.9)
u16 NDP_GetVersion();

// Driver end (Disconnects the driver from the timer interrupt hook and stops sound generation)
void NDP_Release();

// SETHF Timer interrupt hook connection flag ← A
void NDP_SetHookFlag(u8 flag);

// IMAIN Interrupt processing - main part (only the interrupt routine body is processed without calling the old hook)
void NDP_InterruptMain();
