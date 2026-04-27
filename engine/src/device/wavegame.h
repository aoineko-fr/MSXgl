// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄                 ▄▄▄               
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ▄▀██ █ ██ ▄███ ██   ▄▀██ ▄█▄█ ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▀█ ▀▄██ ▀▄▀  ▀█▄▄ ▀█▄█ ▀▄██ ██ █ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Support of WaveGame feature
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// OPTIONS VALIDATION
//=============================================================================

//=============================================================================
// DEFINES
//=============================================================================

// WaveGame version address
#define WAVEGAME_VER_ADDR			0xF91E

// WaveGame port
#define	P_WAVEGAME					0x92
__sfr __at(P_WAVEGAME)				g_PortWaveGame;

// WaveGame versions
#define WAVEGAME_VER_1_49			0
#define WAVEGAME_VER_2_05			0
#define WAVEGAME_VER_2_06			0
#define WAVEGAME_VER_1_50			1
#define WAVEGAME_VER_2_07			1

// WaveGame commands
#define WAVEGAME_CMD_STOP			0b00000000 // Stop music (actually a 1-second fade out)
#define WAVEGAME_CMD_FADE			0b00000000 // Fade out music in xxxx seconds
#define WAVEGAME_CMD_PLAY_ONCE		0b01000000 // Play song xxxxxx once (for instance command 01000101 will play 05.wav once)
#define WAVEGAME_CMD_PLAY_LOOP		0b10000000 // Play song xxxxxx in a loop (for instance command 10000101 will play 05.wav continuously)
#define WAVEGAME_CMD_TOGGLE_PAUSE	0b11000000 // Toggle pause
#define WAVEGAME_CMD_PLAY_PAUSE		0b11110000 // Play pause.wav sound (use 11100000 to continue previous song)
#define WAVEGAME_CMD_CONTINUE		0b11100000 // Continue previous song with a 1-second fade in
#define WAVEGAME_CMD_STORE_NEXT		0b11010000 // Next command received will be temporarily stored and executed after the current song stops playing, or already has stopped

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: WaveGame_GetVersion
// Get the version of the MSX PICO WaveGame firmware
// Note that WaveGame version 1 is needed
//
// Return:
//   WaveGame version
//> 0: Firmware v1.49 / v2.05 / v2.06
//> 1: Firmware v1.50 / v2.07 and higher
inline u8 WaveGame_GetVersion() { return Peek(WAVEGAME_VER_ADDR); }

// Function: WaveGame_Stop
// Stop music (actually a 1-second fade out)
inline void WaveGame_Stop() { g_PortWaveGame = WAVEGAME_CMD_STOP; }

// Function: WaveGame_Fade
// Fade out music in xxxx seconds
//
// Parameters:
//   sec - Number of seconds to fade out (0-15)
inline void WaveGame_Fade(u8 sec) { g_PortWaveGame = WAVEGAME_CMD_FADE | (sec & 0b00001111); }

// Function: WaveGame_PlayOnce
// Play song xxxxxx once (for instance command 01000101 will play 05.wav once)
//
// Parameters:
//   song - Song number to play (0-63)
inline void WaveGame_PlayOnce(u8 song) { g_PortWaveGame = WAVEGAME_CMD_PLAY_ONCE | (song & 0b00111111); }

// Function: WaveGame_PlayLoop
// Play song xxxxxx in a loop (for instance command 10000101 will play 05.wav continuously)
//
// Parameters:
//   song - Song number to play (0-63)
inline void WaveGame_PlayLoop(u8 song) { g_PortWaveGame = WAVEGAME_CMD_PLAY_LOOP | (song & 0b00111111); }

// Function: WaveGame_Play
// Play song xxxxxx (for instance command 10000101 will play 05.wav continuously)
//
// Parameters:
//   song - Song number to play (0-63)
//   loop - true to play in a loop, false to play once
inline void WaveGame_Play(u8 song, bool loop) { if (loop) WaveGame_PlayLoop(song); else WaveGame_PlayOnce(song); }

// Function: WaveGame_Pause
// Toggle pause
inline void WaveGame_Pause() { g_PortWaveGame = WAVEGAME_CMD_TOGGLE_PAUSE; }

// Function: WaveGame_PlayPause
// Play pause.wav sound (use 11100000 to continue previous song)
inline void WaveGame_PlayPause() { g_PortWaveGame = WAVEGAME_CMD_PLAY_PAUSE; }

// Function: WaveGame_Continue
// Continue previous song with a 1-second fade in
inline void WaveGame_Continue() { g_PortWaveGame = WAVEGAME_CMD_CONTINUE; }

// Function: WaveGame_StoreNext
// Next command received will be temporarily stored and executed after the current song stops playing, or already has stopped
inline void WaveGame_StoreNext() { g_PortWaveGame = WAVEGAME_CMD_STORE_NEXT; }

