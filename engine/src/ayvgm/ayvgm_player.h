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

// ayVGM data flag
enum AYVGM_FLAG
{
	AYVGM_FLAG_50HZ = 0b00000001,
	AYVGM_FLAG_60HZ = 0b00000010,
	AYVGM_FLAG_LOOP = 0b00000100,
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
extern const u8  g_ayVGM_RegTable[12];
extern const u8  g_ayVGM_Ident[4];

//=============================================================================
// FUNCTIONS
//=============================================================================

// Start music playback
bool ayVGM_Play(const void* addr, bool loop);

// Stop music playback
void ayVGM_Stop();

// Check if music playing
inline bool ayVGM_IsPlaying() { return g_ayVGM_State & AYVGM_STATE_PLAY; }

// Resume music playback
inline void ayVGM_Resume() { g_ayVGM_State |= AYVGM_STATE_PLAY; }

// Pause music playback
inline void ayVGM_Pause() { g_ayVGM_State &= ~AYVGM_STATE_PLAY; PSG_Silent(); }

// Decode a frame of music
void ayVGM_Decode();