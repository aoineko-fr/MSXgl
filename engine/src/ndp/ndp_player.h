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

// NDP player status
enum NDP_STATUS
{
	NDP_STATUS_STOPPED = 0,
	NDP_STATUS_PLAYING = 1,
};

// NDP tracks IDs
enum NDP_TRACK
{
	NDP_TRACK_RYTHM = 0b00000001,
	NDP_TRACK_1     = 0b00000010,
	NDP_TRACK_2     = 0b00000100,
	NDP_TRACK_3     = 0b00001000,
	NDP_TRACK_ALL   = 0b00001111,
};

// NDP fade IDs
enum NDP_FADE
{
	NDP_FADE_OUT = 0,
	NDP_FADE_IN  = 1,
};

// NDP channel IDs
enum NDP_CHANNEL
{
	NDP_CHANNEL_1 = 0,
	NDP_CHANNEL_2,
	NDP_CHANNEL_3,
	NDP_CHANNEL_A = 0,
	NDP_CHANNEL_B,
	NDP_CHANNEL_C,
};

typedef struct NDP_WorkArea
{
	u16	PlayDataAddr	;// +0 1	演奏データ読み込みアドレス
	u8 	NoteNumber		;// +2	ノート番号
	u8 	Volume			;// +3	設定音量 (0=v15, 1=V14, 2=V13 … 15=v0)
	u8 	NoteLenCnt		;// +4	音長カウンタ
	u8 	NoteLenExt		;// +5	次の1バイトも音長かどうか
	u8 	ToneNumber		;// +6	音色番号 (90～9FHの場合はハードエンベ番号)
	u8 	Legato			;// +7	レガート
	u8 	Sustain			;// +8	サスティン
	u8 	QuantizeQ		;// +9	Q(クオンタイズ)の値
	u8 	ChannelState	;// +10	チャンネル有効/無効 0=無効 1=有効 2=効果音発音中 3～255=一時ミュート(フレーム数)
	u8 	PitchEnvNum		;// +11	ピッチエンベ番号 (bit7=ピッチを戻す？ bit6=ピッチ更新する？ bit5=元音程からの相対値？)
	u8 	LegatoCheck		;// +12	レガート確認用フラグ（+7のレガート設定の値が1音あとに入る）
	u16	CurrentPitch	;// +13 14	発音中の音程（レジスタ値）
	u8 	CurrentVolume	;// +15	発音中の音量
	u16	SoftEnvPtr		;// +16 17	ソフトエンベロープポインタ / ハードエンベ音量半減カウンタ
	u8 	SoftEnvWait		;// +18	ソフトエンベロープウェイトカウンタ
	u16	Detune			;// +19 20	デチューン
	u16	PitchEnvPtr		;// +21 22	ピッチエンベロープポインタ
	u8 	PitchEnvDelay	;// +23	ピッチエンベロープディレイカウンタ
	u8 	ReleaseVolume	;// +24	リリース音量
	u8 	ReleaseSet		;// +25	リリースカウンタ設定値
	u8 	ReleaseCnt		;// +26	リリースカウンタ
	u16	ReleasePitch	;// +27 28	リリースディレイ用ピッチ
	u8 	ReleaseDelay	;// +29	リリースディレイ用スイッチ兼指定フレーム数
	u8 	ReleaseVolSet	;// +30	リリース音量設定
	u8 	KeyOnPitchUpd	;// +31	キーオン時の音程レジスタ更新フラグ
	u8 	RepeatNest		;// +32	リピートネスト数
	u8 	PortaAddFrac	;// +33	ポルタメント加算値(小数)
	u8 	PortaAdd		;// +34	ポルタメント加算値
	u16	PortaPitch		;// +35 36	ポルタメント用の現在音程 (13-14に向かって加減算)
	u8 	PortaSetFrac	;// +37	ポルタメント設定値(小数)
	u8 	PortaSet		;// +38	ポルタメント設定値兼スイッチ (0=OFF)
	u8 	PortaOnce		;// +39	ポルタメント単発フラグ
	u8 	QuantizeQ2		;// +40	@Q(後ろを削るクオンタイズ)の値
	u8 	PrevNote		;// +41	キーオン直前のノート番号（リリースディレイ用）
	u8 	VolIntervalSet	;// +42	音量インターバル設定値
	u8 	NoteEnvNum		;// +43	ノートエンベ番号
	u16	NoteEnvPtr		;// +44 45	ノートエンベロープポインタ
	u8 	NoteEnvWait		;// +46	ノートエンベロープウェイトカウンタ
	u8 	VolIntervalCnt	;// +47	音量インターバルのカウンタ
	u8 	VolIntervalVol	;// +48	音量インターバルの音量値
	u16	PrevDetune		;// +49 50	前回のデチューン値
	u8 	ReleaseDelayCnt	;// +51	リリースディレイ用カウンタ
	u8 	GateTime		;// +52	ゲートタイム (Qから計算してキーオン時に設定)
	u8 	ReleaseSkipCnt	;// +53	リリースディレイスキップ用カウンタ
	u16	PrePitchEnv		;// +54 55	ピッチエンベ適用前の音程（レジスタ値）
	u8 	VolIntervalEnd	;// +56	音量インターバル到達値
	u8 	FixedGateTime	;// +57	固定ゲートタイム
	u8 	ParamBackup		;// +58	パラメータの退避領域
	u8 	ReleaseActive	;// +59	リリースディレイ発動フラグ
	u8 	MixMode			;// +60	*n（ミックスモード）の値
} NDP_WorkArea;


// NDP replayer pamarameters
#define NDP_WSIZE		61		// The size of the work area for each track
#define NDP_RWSIZE		(5*4)	// Repeat work area size for each track (Size per nest * Number of nests)F
#define NDP_CHNUM		4		// Number of channels used

// NDP work area (defined in NDP_WRK.ASM)
extern u8   g_NDP_CH1WRK[NDP_WSIZE];	// Rhythm track
extern NDP_WorkArea   g_NDP_CH2WRK;	// Standard track 1
extern NDP_WorkArea   g_NDP_CH3WRK;	// Standard track 2
extern NDP_WorkArea   g_NDP_CH4WRK;	// Standard track 3
extern u8   g_NDP_SE1WRK[NDP_WSIZE * 3];
extern u8   g_NDP_CH1RWK[NDP_RWSIZE];
extern u8   g_NDP_CH2RWK[NDP_RWSIZE];
extern u8   g_NDP_CH3RWK[NDP_RWSIZE];
extern u8   g_NDP_CH4RWK[NDP_RWSIZE];
extern u8   g_NDP_FFFLG;			// Fast-forward flag
extern u8   g_NDP_ENDTR;			// The bit for tracks whose playback data has reached the end is set (0000321RB) ※Resets on loop
extern u8   g_NDP_ENDTRW;			// ※ Does not reset even after 1 loop
extern u8   g_NDP_ENDTRR;			// Value to be written back upon reset (the value set to flag unused tracks at the start of the music)
extern u8   g_NDP_MCOUNT;			// Number of loops
extern u8   g_NDP_MVOL;				// Master volume (Subtract this value)
extern u8   g_NDP_MFADE;			// Fade setting (0:No fade, 1–255:Fade count)
extern u8   g_NDP_FCOUNT;			// Fade counter
extern u8   g_NDP_LCOUNT;			// Fade loop counter
extern u8   g_NDP_SLWPRM;			// Slow setting
extern u8   g_NDP_SLWCNT;			// Slow playback counter
extern u8   g_NDP_NFREQW;			// Noise frequency
extern u8   g_NDP_VISPAN;			// Volume interval period
extern u8   g_NDP_RNON;				// Rhythm tone active status (0:Not active, 1:Active and pitch restored, 2:Active, 3:Normal tone for one frame only)
extern u8   g_NDP_RSVOL;			// Volume reduction before rhythm playback
extern u16  g_NDP_RPITCH;			// Pitch evasion before rhythm playback
extern u8   g_NDP_HENVSW;			// Hard envelope number
extern u8   g_NDP_SEMODE;			// Playback Mode (0:BGM, 1:SE) ※Set within interrupt
extern u8   g_NDP_SEBAKT[NDP_CHNUM];	// Save the track enable flag for the track being interrupted by sound effects here
extern u8   g_NDP_SEBAKR;			// Whether to trigger register reset at SE end during rhythm key off
extern u8   g_NDP_SECNT;			// Sound effect track counter (Number of sound effect tracks currently playing)
extern u8   g_NDP_VADTBL[32];		// Volume embedded address table
extern u8   g_NDP_RADTBL[64];		// Rhythm tone address table
extern u8   g_NDP_PADTBL[32];		// Pitch-embedded dressing table
extern u8   g_NDP_NADTBL[32];		// Note envelope dress table
extern u8   g_NDP_RVWRK;			// For rhythm volume adjustment
extern u8   g_NDP_RITRK;			// Interrupted track (1:CH.C, 2:CH.B, 3:CH.A)
extern u8   g_NDP_RVREG;			// Rhythm track volume register number
extern u8   g_NDP_RPREG;			// Rhythm track frequency register number (high)
extern u8   g_NDP_RHENV;			// If no S command is executed within the rhythm, it becomes 0
extern u8   g_NDP_OLDTH[5];			// Legacy timer interrupt hook
extern u8   g_NDP_HKFLG;			// Is the driver connected to the hook?
extern u8   g_NDP_STATS;			// Playback Status (0: Stopped, 1: Playing)
extern u8   g_NDP_FINOUT;			// 0:Fade out; 1:Fade in
extern u8   g_NDP_MIXNMH;			// Value of *n (mix mode) when using hard envelope
extern u8   g_NDP_MIXWRK;			// 10NNNTTT 0:ON/1:OFF
extern u8   g_NDP_MIXWRS;			// Sound effects mixing work
extern u16  g_NDP_HENVPW;			// Hard envelope period
extern u16  g_NDP_BGMADR;			// Song data start address
extern u16  g_NDP_PTABLE[12 * 8];	// Pitch table

// Address of the SFX data
extern const u8* g_NDP_SFXData;

//=============================================================================
// FUNCTIONS
//=============================================================================

//.............................................................................
// Group: Core

// Function: NDP_Initialize
// Driver initialization. Must be called first before accessing any other functions
void NDP_Initialize();

// Function: NDP_SetMusicData
// Song data start address setting
//
// Parameters:
//  addr - Address of the music data. This should point to a .NDP binary data without the 7 bytes BASIC headers 
void NDP_SetMusicData(const void* addr);

// Function: NDP_Play
// Start playing (clears the work area to zero and starts playing from music start)
void NDP_Play();

// Function: NDP_Stop
// Stops playback
void NDP_Stop();

// Function: NDP_Update
// Update a music frame. Must be called at each graphics frame; ideally from the interrupt code.
void NDP_Update();

// Function: NDP_Release
// Release the driver. Only needed if you would like to initilize the driver again
void NDP_Release();

//.............................................................................
// Group: Utility

// Function: NDP_MuteChannel1
// Temporary mute channel A 
//
// Parameters:
//   frames - Number of frames to mute (1:Cancel, 2 to 254:Duraton, 255:Permanent)
void NDP_MuteChannelA(u8 frames);

// Function: NDP_MuteChannel2
// Temporary mute channel B
//
// Parameters:
//   frames - Number of frames to mute (1:Cancel, 2 to 254:Duraton, 255:Permanent)
void NDP_MuteChannelB(u8 frames);

// Function: NDP_MuteChannel3
// Temporary mute channel C
//
// Parameters:
//   frames - Number of frames to mute (1:Cancel, 2 to 254:Duraton, 255:Permanent)
void NDP_MuteChannelC(u8 frames);

// Function: NDP_MuteChannel
// Temporary mute the given channel
//
// Parameters:
//   channel - Channel number
//> NDP_CHANNEL_A
//> NDP_CHANNEL_B
//> NDP_CHANNEL_C
//   frames - Number of frames to mute (1:Cancel, 2 to 254:Duraton, 255:Permanent)
inline void NDP_MuteChannel(u8 channel, u8 frames)
{
	switch (channel)
	{
	case NDP_CHANNEL_A: NDP_MuteChannelA(frames); break;
	case NDP_CHANNEL_B: NDP_MuteChannelB(frames); break;
	case NDP_CHANNEL_C: NDP_MuteChannelC(frames); break;
	}
}

// Function: NDP_SetVolume
// Set master volume
//
// Parameters:
//   vol - New master volume
void NDP_SetVolume(u8 vol);

// Function: NDP_FadeOut
// Start fade-out current music
//
// Parameters:
//   speed - Number of frames for each stage (the small the quicker)
void NDP_FadeOut(u8 speed);

// Function: NDP_PlayFadeIn
// Starts playing music while fading-in
//
// Parameters:
//   speed - Number of frames for each stage (the small the quicker)
void NDP_PlayFadeIn(u8 speed);

// Function: NDP_SetToneColor
// Set tone color data
//
// Parameters:
//   addr - Address of tone color definition table
void NDP_SetToneColor(const void* addr);

// Function: NDP_GetVersion
// Driver version number
//
// Return:
//   NDP driver version (for example, 0x0104 is version 1.0.4)
u16 NDP_GetVersion();

// Function: NDP_SetInitFlag
// Set initialization flag
inline void NDP_SetInitFlag(u8 flag) { g_NDP_HKFLG = flag; }

//.............................................................................
// Group: SFX

// Function: NDP_SetSFXData
// Set sound effects data address
//
// Parameters:
//   addr - Address of the sound effects data. This should point to a .NDS binary data without the 7 bytes BASIC headers
inline void NDP_SetSFXData(const void* addr) { g_NDP_SFXData = addr; }

// Function: NDP_GetSFXNumber
// Get the number of sound effect into the current bank.
// <NDP_SetSFXData> must be called first
//
// Return:
//   Number of sound effect in the current bank
inline u8 NDP_GetSFXNumber() { return g_NDP_SFXData[0]; }

// Function: NDP_GetSFXOffset
// Get the offset of a given sound effect into the current bank.
// <NDP_SetSFXData> must be called first
//
// Parameters:
//   sfx - Index of the sound effect
//
// Return:
//   Offset of the given sound effect from the start of the bank data
inline u16 NDP_GetSFXOffset(u8 sfx) { return ((u16*)(g_NDP_SFXData + 1))[sfx]; }

// Function: NDP_PlaySFX
// Play a given sound effect.
// <NDP_SetSFXData> must be called first
//
// Parameters:
//   sfx - Index of the sound effect to playback
void NDP_PlaySFX(u8 sfx);

//.............................................................................
// Group: Status

// Function: NDP_GetStatus
// Playing status (NDP_STATUS_xxx)
//
// Return:
//> NDP_STATUS_STOPPED
//> NDP_STATUS_PLAYING
inline u8 NDP_GetStatus() { return g_NDP_STATS; }

// Function: NDP_IsPlaying
// Whether or not music is currently playing
//
// Return:
//   FALSE if music is not playing
inline bool NDP_IsPlaying() { return NDP_GetStatus() == NDP_STATUS_PLAYING; }

// Function: NDP_IsStopped
// Whether or not music is currently stopped
//
// Return:
//   FALSE if music is not stopped
inline bool NDP_IsStopped() { return NDP_GetStatus() == NDP_STATUS_STOPPED; }

// Function: NDP_HasTrackEnded
// Whether or not each track has reached the end
//
// Parameters:
//   track - ID of the track
//> NDP_TRACK_1
//> NDP_TRACK_2
//> NDP_TRACK_3
//> NDP_TRACK_RYTHM
//> NDP_TRACK_ALL
//
// Return:
//   FALSE if the given track hasn't reached its end
inline bool NDP_HasTrackEnded(u8 track) { return (g_NDP_ENDTRW & track) == track; }

// Function: NDP_HasMusicEnded
// Whether or not all tracks have reached the end
//
// Return:
//   FALSE if the music hasn't reached the end
inline bool NDP_HasMusicEnded() { return NDP_HasTrackEnded(NDP_TRACK_ALL); }

// Function: NDP_GetLoopCount
// Number of times the music has looped
//
// Return:
//   If the music does not have an infinite loop, check +0BH in the music data and return 255 when the music finishes playing
inline u8 NDP_GetLoopCount() { return g_NDP_MCOUNT; }

// Function: NDP_IsFadding
// Fade setting
//
// Return:
//> 0: No fade
//> 1–255: Fade count
inline u8 NDP_IsFadding() { return g_NDP_MFADE; }

// Function: NDP_GetFadeType
// Get th fade type
//
// Return:
//> NDP_FADE_OUT
//> NDP_FADE_IN
inline u8 NDP_GetFadeType() { return g_NDP_FINOUT; }

// Function: NDP_IsInitialized
// Is the driver initialized
//
// Return:
//   FALSE if driver not initialized
inline bool NDP_IsInitialized() { return g_NDP_HKFLG; }