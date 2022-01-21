// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#include "psg.h"

//=============================================================================
// DEFINES
//=============================================================================

#define USE_AYVGM_SCC 0

// ayVGM data flag
enum AYVGM_FLAG
{
	AYVGM_FLAG_50HZ = 0b00000001,
	AYVGM_FLAG_60HZ = 0b00000010,
	AYVGM_FLAG_LOOP = 0b00000100,

	AYVGM_FLAG_SCC  = 0b00010000,
	AYVGM_FLAG_SCCI = 0b00100000,
	AYVGM_FLAG_ext3 = 0b00110000,
	AYVGM_FLAG_ext4 = 0b01000000,
	AYVGM_FLAG_ext5 = 0b01010000,
	AYVGM_FLAG_ext6 = 0b01100000,
	AYVGM_FLAG_ext7 = 0b01110000,
};

// ayVGM playback state flag
enum AYVGM_STATE
{
	AYVGM_STATE_50HZ = 0b00000001,
	AYVGM_STATE_LOOP = 0b00000010,
	AYVGM_STATE_PLAY = 0b10000000,
};

// ayVGM header structure
struct ayVGM_Header
{
	u8	Ident[4]; // 'ayM '
	u8	Flag;     // see AYVGM_FLAG
};

// Extern data
extern const struct ayVGM_Header* g_ayVGM_Header;
extern const u8* g_ayVGM_Pointer;
extern u8        g_ayVGM_Wait;
extern u8        g_ayVGM_State;
extern const u8  g_ayVGM_RegTable[13];
extern const u8  g_ayVGM_Ident[4];

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: ayVGM_Play
// Start music playback
bool ayVGM_Play(const void* addr, bool loop);

// Function: ayVGM_Stop
// Stop music playback
void ayVGM_Stop();

// Function: ayVGM_SetFrequency50Hz
// Change frequency to 50 Hz
inline bool ayVGM_SetFrequency50Hz() { g_ayVGM_State |= AYVGM_STATE_50HZ; }

// Function: ayVGM_SetFrequency60Hz
// Change frequency to 60 Hz
inline bool ayVGM_SetFrequency60Hz() { g_ayVGM_State &= ~AYVGM_STATE_50HZ; }

// Function: ayVGM_IsPlaying
// Check if music playing
inline bool ayVGM_IsPlaying() { return g_ayVGM_State & AYVGM_STATE_PLAY; }

// Function: ayVGM_Resume
// Resume music playback
inline void ayVGM_Resume() { g_ayVGM_State |= AYVGM_STATE_PLAY; }

// Function: ayVGM_Pause
// Pause music playback
inline void ayVGM_Pause() { g_ayVGM_State &= ~AYVGM_STATE_PLAY; PSG_Silent(); }

// Function: ayVGM_Decode
// Decode a frame of music
void ayVGM_Decode();