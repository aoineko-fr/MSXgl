// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄           ▄  ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ██▀▄ ██▄▀ ▄  ██▀ ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ██▀  ██   ██ ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘       ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Sprite tool module
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "msxgl.h"

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: Sprite_CropLeft8
// Crop 8x8 sprite left side
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
//   offset - Croping offset (0-7)
void Sprite_CropLeft8(const u8* src, u8* dest, u8 offset);

// Function: Sprite_CropLeft16
// Crop 16x16 sprite left side
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
//   offset - Croping offset (0-15)
void Sprite_CropLeft16(const u8* src, u8* dest, u8 offset);

// Function: Sprite_CropRight8
// Crop 8x8 sprite right side
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
//   offset - Croping offset (0-7)
void Sprite_CropRight8(const u8* src, u8* dest, u8 offset);

// Function: Sprite_CropRight16
// Crop 16x16 sprite right side
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
//   offset - Croping offset (0-15)
void Sprite_CropRight16(const u8* src, u8* dest, u8 offset);

// Function: Sprite_FlipV8
// Vertical flip 8x8 sprite
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
void Sprite_FlipV8(const u8* src, u8* dest);

// Function: Sprite_FlipV16
// Vertical flip 16x16 sprite
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
void Sprite_FlipV16(const u8* src, u8* dest);

// Function: Sprite_FlipH8
// Horizontally flip 8x8 sprite
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
void Sprite_FlipH8(const u8* src, u8* dest);

// Function: Sprite_FlipH16
// Horizontally flip 16x16 sprite
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
void Sprite_FlipH16(const u8* src, u8* dest);

// Function: Sprite_Mask8
// Mask 8x8 sprite
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
//   mask   - Pointer to 8 bytes mask data
void Sprite_Mask8(const u8* src, u8* dest, const u8* mask);

// Function: Sprite_Mask16
// Mask 16x16 sprite
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
//   mask   - Pointer to 32 bytes mask data
void Sprite_Mask16(const u8* src, u8* dest, const u8* mask);

// Function: Sprite_RotateRight8
// Rotate 8x8 sprite 90° to the right
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
void Sprite_RotateRight8(const u8* src, u8* dest);

// Function: Sprite_RotateRight16
// Rotate 16x16 sprite 90° to the right
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
void Sprite_RotateRight16(const u8* src, u8* dest);

// Function: Sprite_RotateLeft8
// Rotate 8x8 sprite 90° to the left
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
void Sprite_RotateLeft8(const u8* src, u8* dest);

// Function: Sprite_RotateLeft16
// Rotate 16x16 sprite 90° to the left
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
void Sprite_RotateLeft16(const u8* src, u8* dest);

// Function: Sprite_RotateHalfTurn8
// Rotate 8x8 sprite 180°
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
void Sprite_RotateHalfTurn8(const u8* src, u8* dest);

// Function: Sprite_RotateHalfTurn16
// Rotate 16x16 sprite 180°
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
void Sprite_RotateHalfTurn16(const u8* src, u8* dest);

// Define: Sprite_Rotate90_8
// Alias for <Sprite_RotateRight8>
#define Sprite_Rotate90_8 Sprite_RotateRight8

// Define: Sprite_Rotate90_16
// Alias for <Sprite_RotateRight16>
#define Sprite_Rotate90_16 Sprite_RotateRight16

// Define: Sprite_Rotate180_8
// Alias for <Sprite_RotateRight8>
#define Sprite_Rotate180_8 Sprite_RotateHalfTurn8

// Define: Sprite_Rotate180_16
// Alias for <Sprite_RotateRight16>
#define Sprite_Rotate180_16 Sprite_RotateHalfTurn16

// Define: Sprite_Rotate270_8
// Alias for <Sprite_RotateLeft8>
#define Sprite_Rotate270_8 Sprite_RotateLeft8

// Define: Sprite_Rotate270_16
// Alias for <Sprite_RotateLeft16>
#define Sprite_Rotate270_16 Sprite_RotateLeft16
