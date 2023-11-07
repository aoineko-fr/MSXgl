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
// DEFINES
//=============================================================================

// SPRITEFX_USE_8x8
#ifndef SPRITEFX_USE_8x8
	#warning SPRITEFX_USE_8x8 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define SPRITEFX_USE_8x8				TRUE
#endif

// SPRITEFX_USE_16x16
#ifndef SPRITEFX_USE_16x16
	#warning SPRITEFX_USE_16x16 is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define SPRITEFX_USE_16x16				TRUE
#endif

// SPRITEFX_USE_CROP
#ifndef SPRITEFX_USE_CROP
	#warning SPRITEFX_USE_CROP is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define SPRITEFX_USE_CROP				TRUE
#endif

// SPRITEFX_USE_FLIP
#ifndef SPRITEFX_USE_FLIP
	#warning SPRITEFX_USE_FLIP is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define SPRITEFX_USE_FLIP				TRUE
#endif

// SPRITEFX_USE_MASK
#ifndef SPRITEFX_USE_MASK
	#warning SPRITEFX_USE_MASK is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define SPRITEFX_USE_MASK				TRUE
#endif

// SPRITEFX_USE_ROTATE
#ifndef SPRITEFX_USE_ROTATE
	#warning SPRITEFX_USE_ROTATE is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define SPRITEFX_USE_ROTATE				TRUE
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Cropping
// Functions to crop one of the sprite border.
//-----------------------------------------------------------------------------
#if (SPRITEFX_USE_CROP)

//.............................................................................
#if (SPRITEFX_USE_8x8)

// Function: SpriteFX_CropLeft8
// Crop 8x8 sprite left border.
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
//   offset - Croping offset (0-7)
void SpriteFX_CropLeft8(const u8* src, u8* dest, u8 offset);

// Function: SpriteFX_CropRight8
// Crop 8x8 sprite right border.
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
//   offset - Croping offset (0-7)
void SpriteFX_CropRight8(const u8* src, u8* dest, u8 offset);

// Function: SpriteFX_CropTop8
// Crop 8x8 sprite top border.
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
//   offset - Croping offset (0-7)
void SpriteFX_CropTop8(const u8* src, u8* dest, u8 offset);

// Function: SpriteFX_CropBottom8
// Crop 8x8 sprite bottom border.
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
//   offset - Croping offset (0-7)
void SpriteFX_CropBottom8(const u8* src, u8* dest, u8 offset);

#endif // (SPRITEFX_USE_8x8)

//.............................................................................
#if (SPRITEFX_USE_16x16)

// Function: SpriteFX_CropLeft16
// Crop 16x16 sprite left border.
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
//   offset - Croping offset (0-15)
void SpriteFX_CropLeft16(const u8* src, u8* dest, u8 offset);

// Function: SpriteFX_CropRight16
// Crop 16x16 sprite right border.
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
//   offset - Croping offset (0-15)
void SpriteFX_CropRight16(const u8* src, u8* dest, u8 offset);

// Function: SpriteFX_CropTop16
// Crop 16x16 sprite top border.
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
//   offset - Croping offset (0-15)
void SpriteFX_CropTop16(const u8* src, u8* dest, u8 offset);

// Function: SpriteFX_CropBottom16
// Crop 16x16 sprite bottom border.
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
//   offset - Croping offset (0-15)
void SpriteFX_CropBottom16(const u8* src, u8* dest, u8 offset);

#endif // (SPRITEFX_USE_16x16)

#endif // (SPRITEFX_USE_CROP)

//-----------------------------------------------------------------------------
// Group: Flipping
// Functions to flip a sprite along horizontal or vertical axis.
//-----------------------------------------------------------------------------
#if (SPRITEFX_USE_FLIP)

//.............................................................................
#if (SPRITEFX_USE_8x8)

// Function: SpriteFX_FlipVertical8
// Vertical flip 8x8 sprite.
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
void SpriteFX_FlipVertical8(const u8* src, u8* dest);

// Function: SpriteFX_FlipHorizontal8
// Horizontally flip 8x8 sprite.
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
void SpriteFX_FlipHorizontal8(const u8* src, u8* dest);

// Define: SpriteFX_FlipV8
// Alias for <SpriteFX_FlipVertical8> function.
#define SpriteFX_FlipV8 SpriteFX_FlipVertical8

// Define: SpriteFX_FlipH8
// Alias for <SpriteFX_FlipHorizontal8> function.
#define SpriteFX_FlipH8 SpriteFX_FlipHorizontal8

#endif // (SPRITEFX_USE_8x8)

//.............................................................................
#if (SPRITEFX_USE_16x16)

// Function: SpriteFX_FlipVertical16
// Vertical flip 16x16 sprite.
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
void SpriteFX_FlipVertical16(const u8* src, u8* dest);

// Function: SpriteFX_FlipHorizontal16
// Horizontally flip 16x16 sprite.
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
void SpriteFX_FlipHorizontal16(const u8* src, u8* dest);

// Define: SpriteFX_FlipV16
// Alias for <SpriteFX_FlipVertical16> function.
#define SpriteFX_FlipV16 SpriteFX_FlipVertical16

// Define: SpriteFX_FlipH16
// Alias for <SpriteFX_FlipHorizontal16> function.
#define SpriteFX_FlipH16 SpriteFX_FlipHorizontal16

#endif // (SPRITEFX_USE_16x16)

#endif // (SPRITEFX_USE_FLIP)

//-----------------------------------------------------------------------------
// Group: Masking
// Functions to mask some part of a sprite according to provided data.
//-----------------------------------------------------------------------------
#if (SPRITEFX_USE_MASK)

#if (SPRITEFX_USE_8x8)
// Function: SpriteFX_Mask8
// Mask 8x8 sprite.
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
//   mask   - Pointer to 8 bytes mask data
void SpriteFX_Mask8(const u8* src, u8* dest, const u8* mask);
#endif

#if (SPRITEFX_USE_16x16)
// Function: SpriteFX_Mask16
// Mask 16x16 sprite.
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
//   mask   - Pointer to 32 bytes mask data
void SpriteFX_Mask16(const u8* src, u8* dest, const u8* mask);
#endif

#endif // (SPRITEFX_USE_MASK)

//-----------------------------------------------------------------------------
// Group: Rotating
// Functions to rotate a sprite bt 90° step.
//-----------------------------------------------------------------------------
#if (SPRITEFX_USE_ROTATE)

//.............................................................................
#if (SPRITEFX_USE_8x8)

// Function: SpriteFX_RotateRight8
// Rotate 8x8 sprite 90° to the right.
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
void SpriteFX_RotateRight8(const u8* src, u8* dest);

// Function: SpriteFX_RotateLeft8
// Rotate 8x8 sprite 90° to the left.
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
void SpriteFX_RotateLeft8(const u8* src, u8* dest);

// Function: SpriteFX_RotateHalfTurn8
// Rotate 8x8 sprite 180°.
//
// Parameters:
//   src    - Pointer to 8 bytes source sprite data
//   dest   - Pointer to 8 bytes destination buffer
void SpriteFX_RotateHalfTurn8(const u8* src, u8* dest);

// Define: SpriteFX_Rotate90_8
// Alias for <SpriteFX_RotateRight8> function.
#define SpriteFX_Rotate90_8 SpriteFX_RotateRight8

// Define: SpriteFX_Rotate180_8
// Alias for <SpriteFX_RotateHalfTurn8> function.
#define SpriteFX_Rotate180_8 SpriteFX_RotateHalfTurn8

// Define: SpriteFX_Rotate270_8
// Alias for <SpriteFX_RotateLeft8> function.
#define SpriteFX_Rotate270_8 SpriteFX_RotateLeft8

#endif // (SPRITEFX_USE_8x8)

//.............................................................................
#if (SPRITEFX_USE_16x16)

// Function: SpriteFX_RotateRight16
// Rotate 16x16 sprite 90° to the right.
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
void SpriteFX_RotateRight16(const u8* src, u8* dest);

// Function: SpriteFX_RotateLeft16
// Rotate 16x16 sprite 90° to the left.
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
void SpriteFX_RotateLeft16(const u8* src, u8* dest);

// Function: SpriteFX_RotateHalfTurn16
// Rotate 16x16 sprite 180°.
//
// Parameters:
//   src    - Pointer to 32 bytes source sprite data
//   dest   - Pointer to 32 bytes destination buffer
void SpriteFX_RotateHalfTurn16(const u8* src, u8* dest);

// Define: SpriteFX_Rotate90_16
// Alias for <SpriteFX_RotateRight16> function.
#define SpriteFX_Rotate90_16 SpriteFX_RotateRight16

// Define: SpriteFX_Rotate180_16
// Alias for <SpriteFX_RotateHalfTurn16> function.
#define SpriteFX_Rotate180_16 SpriteFX_RotateHalfTurn16

// Define: SpriteFX_Rotate270_16
// Alias for <SpriteFX_RotateLeft16> function.
#define SpriteFX_Rotate270_16 SpriteFX_RotateLeft16

#endif // (SPRITEFX_USE_16x16)

#endif // (SPRITEFX_USE_ROTATE)