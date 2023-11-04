// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// 
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "msxgl.h"

// Crop 8x8 sprite left side
void Sprite_CropLeft8(const u8* src, u8* dest, u8 offset);

// Crop 16x16 sprite left side
void Sprite_CropLeft16(const u8* src, u8* dest, u8 offset);

// Crop 8x8 sprite right side
void Sprite_CropRight8(const u8* src, u8* dest, u8 offset);

// Crop 16x16 sprite right side
void Sprite_CropRight16(const u8* src, u8* dest, u8 offset);

// Vertical flip 8x8 sprite
void Sprite_FlipV8(const u8* src, u8* dest);

// Vertical flip 16x16 sprite
void Sprite_FlipV16(const u8* src, u8* dest);

// Horizontally flip 8x8 sprite
void Sprite_FlipH8(const u8* src, u8* dest);

// Horizontally flip 16x16 sprite
void Sprite_FlipH16(const u8* src, u8* dest);

// Mask 8x8 sprite
void Sprite_Mask8(const u8* src, u8* dest, const u8* mask);

// Mask 16x16 sprite
void Sprite_Mask16(const u8* src, u8* dest, const u8* mask);