//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄   ▄▄▄▄           ▄▄         ▄▄ ▄ ▄  ▄▄
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀    ██  ▄█▀▄ ▄█▀▄ ██   ██▀   ██▄▀ ▄  ██▀
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █    ██  ▀█▄▀ ▀█▄▀ ▀█▄ ▄██    ██ █ ██ ▀█▄
//_____________________________________________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/CMSXtk)
// under CC-BY-SA license (https://creativecommons.org/licenses/by-sa/2.0/)
//_____________________________________________________________________________
#pragma once

// std
#include <stdint.h>

//-----------------------------------------------------------------------------
// integer types
typedef int8_t			i8;  ///<  8 bits integer
typedef int16_t			i16; ///< 16 bits integer
typedef int32_t			i32; ///< 32 bits integer
typedef int64_t			i64; ///< 64 bits integer

//-----------------------------------------------------------------------------
// unsigned integer types
typedef uint8_t			u8;  ///<  8 bits unsigned integer
typedef uint16_t		u16; ///< 16 bits unsigned integer
typedef uint32_t		u32; ///< 32 bits unsigned integer
typedef uint64_t		u64; ///< 64 bits unsigned integer

//-----------------------------------------------------------------------------
// signed integer types
typedef int8_t			s8;  ///<  8 bits signed integer
typedef int16_t			s16; ///< 16 bits signed integer
typedef int32_t			s32; ///< 32 bits signed integer
typedef int64_t			s64; ///< 64 bits signed integer

//-----------------------------------------------------------------------------
// float types
typedef float			f32; ///< 32 bits floating-point
typedef double			f64; ///< 64 bits floating-point

//-----------------------------------------------------------------------------
// character types
typedef char			c8;  ///<  8 bits character
typedef wchar_t			c16; ///< 16 bits character

//-----------------------------------------------------------------------------
// help macros
#define numberof(tab)	(sizeof(tab) / sizeof(tab[0]))
#define loop(var, x)	for(u32 var = 0; var < x; var++)
#define loopx(x)		for(u32 i##__LINE__ = 0; i##__LINE__ < x; i##__LINE__++)
