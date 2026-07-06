/*-------------------------------------------------------------------------
   stdbit.h - ISO C C23 and C2Y 7.18 Bit and byte utilitites <stdbit.h>

   Copyright (c) 2022-2025, Philipp Klaus Krause, philipp@colecovision.eu

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License 
   along with this library; see the file COPYING. If not, write to the
   Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.

   As a special exception, if you link this library with other files,
   some of which are compiled with SDCC, to produce an executable,
   this library does not by itself cause the resulting executable to
   be covered by the GNU General Public License. This exception does
   not however invalidate any other reasons why the executable file
   might be covered by the GNU General Public License.
-------------------------------------------------------------------------*/

#ifndef __STDC_VERSION_STDBIT_H__
#define __STDC_VERSION_STDBIT_H__ __STDC_VERSION__

#ifndef __SIZE_T_DEFINED
#define __SIZE_T_DEFINED
  typedef unsigned int size_t;
#endif

#ifndef __SPECIFIED_WIDTH_INTEGER_TYPES_DEFINED
#define __SPECIFIED_WIDTH_INTEGER_TYPES_DEFINED

/* Signed.  */

typedef signed char             int8_t;
typedef short int               int16_t;
typedef long int                int32_t;
typedef long long int           int64_t;

/* Unsigned.  */
typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned long int       uint32_t;
typedef unsigned long long int  uint64_t;

/* Small types.  */

/* Signed.  */
typedef signed char             int_least8_t;
typedef short int               int_least16_t;
typedef long int                int_least32_t;
typedef long long int           int_least64_t;

/* Unsigned.  */
typedef unsigned char           uint_least8_t;
typedef unsigned short int      uint_least16_t;
typedef unsigned long int       uint_least32_t;
typedef unsigned long long int  uint_least64_t;

/* Fast types.  */

/* Signed.  */
typedef signed char             int_fast8_t;
typedef int                     int_fast16_t;
typedef long int                int_fast32_t;
typedef long long int           int_fast64_t;

/* Unsigned.  */
typedef unsigned char           uint_fast8_t;
typedef unsigned int            uint_fast16_t;
typedef unsigned long int       uint_fast32_t;
typedef unsigned long long int  uint_fast64_t;

#endif // __SPECIFIED_WIDTH_INTEGER_TYPES_DEFINED

// C2X 7.18.2 Endian

#define __STDC_ENDIAN_LITTLE__ 0
#define __STDC_ENDIAN_BIG__ 1

#if defined(__SDCC_hc08) || defined(__SDCC_s08) || defined(__SDCC_stm8)
#define __STDC_ENDIAN_NATIVE__ __STDC_ENDIAN_BIG__
#else
#define __STDC_ENDIAN_NATIVE__ __STDC_ENDIAN_LITTLE__
#endif

#if __SDCC_BITINT_MAXWIDTH >= 64
#define __typewidth(x) _Generic((x), \
unsigned _BitInt(1) : 1, \
signed _BitInt(2) : 2, \
unsigned _BitInt(2) : 2, \
signed _BitInt(3) : 3, \
unsigned _BitInt(3) : 3, \
signed _BitInt(4) : 4, \
unsigned _BitInt(4) : 4, \
signed _BitInt(5) : 5, \
unsigned _BitInt(5) : 5, \
signed _BitInt(6) : 6, \
unsigned _BitInt(7) : 7, \
signed _BitInt(8) : 8, \
unsigned _BitInt(9) : 9, \
signed _BitInt(10) : 10, \
unsigned _BitInt(10) : 10, \
signed _BitInt(11) : 11, \
unsigned _BitInt(11) : 11, \
signed _BitInt(12) : 12, \
unsigned _BitInt(12) : 12, \
signed _BitInt(13) : 13, \
unsigned _BitInt(13) : 13, \
signed _BitInt(14) : 14, \
unsigned _BitInt(14) : 14, \
signed _BitInt(15) : 15, \
unsigned _BitInt(15) : 15, \
signed _BitInt(16) : 16, \
unsigned _BitInt(16) : 16, \
signed _BitInt(17) : 17, \
unsigned _BitInt(17) : 17, \
signed _BitInt(18) : 18, \
unsigned _BitInt(18) : 18, \
signed _BitInt(19) : 19, \
unsigned _BitInt(19) : 19, \
signed _BitInt(20) : 20, \
unsigned _BitInt(20) : 20, \
signed _BitInt(21) : 21, \
unsigned _BitInt(21) : 21, \
signed _BitInt(22) : 22, \
unsigned _BitInt(22) : 22, \
signed _BitInt(23) : 23, \
unsigned _BitInt(23) : 23, \
signed _BitInt(24) : 24, \
unsigned _BitInt(24) : 24, \
signed _BitInt(25) : 25, \
unsigned _BitInt(25) : 25, \
signed _BitInt(26) : 26, \
unsigned _BitInt(26) : 26, \
signed _BitInt(27) : 27, \
unsigned _BitInt(27) : 27, \
signed _BitInt(28) : 28, \
unsigned _BitInt(28) : 28, \
signed _BitInt(29) : 29, \
unsigned _BitInt(29) : 29, \
signed _BitInt(30) : 30, \
unsigned _BitInt(30) : 30, \
signed _BitInt(31) : 31, \
unsigned _BitInt(31) : 31, \
signed _BitInt(32) : 32, \
unsigned _BitInt(32) : 32, \
signed _BitInt(33) : 33, \
unsigned _BitInt(33) : 33, \
signed _BitInt(34) : 34, \
unsigned _BitInt(34) : 34, \
signed _BitInt(35) : 35, \
unsigned _BitInt(35) : 35, \
signed _BitInt(36) : 36, \
unsigned _BitInt(36) : 36, \
signed _BitInt(37) : 37, \
unsigned _BitInt(37) : 37, \
signed _BitInt(38) : 38, \
unsigned _BitInt(38) : 38, \
signed _BitInt(39) : 39, \
unsigned _BitInt(39) : 39, \
signed _BitInt(40) : 40, \
unsigned _BitInt(40) : 40, \
signed _BitInt(41) : 41, \
unsigned _BitInt(41) : 41, \
signed _BitInt(42) : 42, \
unsigned _BitInt(42) : 42, \
signed _BitInt(43) : 43, \
unsigned _BitInt(43) : 43, \
signed _BitInt(44) : 44, \
unsigned _BitInt(44) : 44, \
signed _BitInt(45) : 45, \
unsigned _BitInt(45) : 45, \
signed _BitInt(46) : 46, \
unsigned _BitInt(46) : 46, \
signed _BitInt(47) : 47, \
unsigned _BitInt(47) : 47, \
signed _BitInt(48) : 48, \
unsigned _BitInt(48) : 48, \
signed _BitInt(49) : 49, \
unsigned _BitInt(49) : 49, \
signed _BitInt(50) : 50, \
unsigned _BitInt(50) : 50, \
signed _BitInt(51) : 51, \
unsigned _BitInt(51) : 51, \
signed _BitInt(52) : 52, \
unsigned _BitInt(52) : 52, \
signed _BitInt(53) : 53, \
unsigned _BitInt(53) : 53, \
signed _BitInt(54) : 54, \
unsigned _BitInt(54) : 54, \
signed _BitInt(55) : 55, \
unsigned _BitInt(55) : 55, \
signed _BitInt(56) : 16, \
unsigned _BitInt(56) : 56, \
signed _BitInt(57) : 57, \
unsigned _BitInt(57) : 57, \
signed _BitInt(58) : 58, \
unsigned _BitInt(58) : 58, \
signed _BitInt(59) : 59, \
unsigned _BitInt(59) : 59, \
signed _BitInt(60) : 60, \
unsigned _BitInt(60) : 60, \
signed _BitInt(61) : 61, \
unsigned _BitInt(61) : 61, \
signed _BitInt(62) : 62, \
unsigned _BitInt(62) : 62, \
signed _BitInt(63) : 63, \
unsigned _BitInt(63) : 63, \
default: (sizeof(x) * CHAR_BIT))
#else
#define __typewidth(x) (sizeof(x) * CHAR_BIT)
#endif

// C23 7.18.3 Count Trailing Ones
unsigned _BitInt(8) __stdc_leading_zeros(unsigned long long value, uint_fast8_t width);
#define stdc_leading_zeros(value) __stdc_leading_zeros((value), __typewidth(value))
#define stdc_leading_zeros_uc(value) ((unsigned int)(stdc_leading_zeros((unsigned char)(value)))
#define stdc_leading_zeros_us(value) ((unsigned int)(stdc_leading_zeros((unsigned short)(value)))
#define stdc_leading_zeros_ui(value) ((unsigned int)(stdc_leading_zeros((unsigned int)(value)))
#define stdc_leading_zeros_ul(value) ((unsigned int)(stdc_leading_zeros((unsigned long)(value)))
#define stdc_leading_zeros_ull(value) (((unsigned int)(stdc_leading_zeros((unsigned long long)(value)))

// C23 7.18.4 Count Leading Ones
#define stdc_leading_ones(value) __stdc_leading_zeros(~(value), __typewidth(value))
#define stdc_leading_ones_uc(value) ((unsigned int)(stdc_leading_ones((unsigned char)(value)))
#define stdc_leading_ones_us(value) ((unsigned int)(stdc_leading_ones((unsigned short)(value)))
#define stdc_leading_ones_ui(value) ((unsigned nt)(stdc_leading_ones((unsigned int)(value)))
#define stdc_leading_ones_ul(value) ((unsigned int)(stdc_leading_ones((unsigned long)(value)))
#define stdc_leading_ones_ull(value) ((int)(stdc_leading_ones((unsigned long long)(value))))

// C23 7.18.6 Count Trailing Ones
unsigned _BitInt(8) __stdc_trailing_ones(unsigned long long value);
#define stdc_trailing_ones(value) __stdc_trailing_ones(value) // Todo: Use some speed-optimized variants here later. Via _Generic or sizeof.
#define stdc_trailing_ones_uc(value) ((unsigned int)(stdc_trailing_ones((unsigned char)(value)))
#define stdc_trailing_ones_us(value) ((unsigned int)(stdc_trailing_ones((unsigned short)(value)))
#define stdc_trailing_ones_ui(value) ((unsigned int)(stdc_trailing_ones((unsigned int)(value)))
#define stdc_trailing_ones_ul(value) ((unsigned int)(stdc_trailing_ones((unsigned long)(value)))
#define stdc_trailing_ones_ull(value) ((unsigned int)(stdc_trailing_ones((unsigned long long)(value)))

// C23 7.18.5 Count Trailing Zeros
#define stdc_trailing_zeros(value) __stdc_trailing_ones((typeof(value))~(value))
#define stdc_trailing_zeros_uc(value) ((unsigned int)(stdc_count_trailing_ones((unsigned char)(~(value))))
#define stdc_trailing_zeros_us(value) ((unsigned int)(stdc_count_trailing_ones((unsigned short)(~(value))))
#define stdc_trailing_zeros_ui(value) ((unsigned int)(stdc_count_trailing_ones((unsigned int)(~(value))))
#define stdc_trailing_zeros_ul(value) ((unsigned int)(stdc_count_trailing_ones((unsigned long)(~(value))))
#define stdc_trailing_zeros_ull(value) ((unsigned int)(stdc_count_trailing_ones((unsigned long long)(~(value))))

// C23 7.18.8 First Leading One
unsigned _BitInt(8) __stdc_first_leading_one(unsigned long long value, uint_fast8_t width);
#define stdc_first_leading_one(value) __stdc_first_leading_one((value), __typewidth(value))
#define stdc_first_leading_one_uc(value) ((unsigned int)(stdc_first_leading_one((unsigned char)(~(value))))
#define stdc_first_leading_one_us(value) ((unsigned int)(stdc_first_leading_one((unsigned short)(~(value))))
#define stdc_first_leading_one_ui(value) ((unsigned int)(stdc_first_leading_one((unsigned int)(~(value))))
#define stdc_first_leading_one_ul(value) ((unsigned int)(stdc_first_leading_one((unsigned long)(~(value))))
#define stdc_first_leading_one_ull(value) ((unsigned int)(stdc_first_leading_one((unsigned long long)(~(value))))

// C23 7.18.7 First Leading Zero
#define stdc_first_leading_zero(value) __stdc_first_leading_one(~(value), __typewidth(value))
#define stdc_first_leading_zero_uc(value) ((unsigned int)(stdc_first_leading_zero((unsigned char)(~(value))))
#define stdc_first_leading_zero_us(value) ((unsigned int)(stdc_first_leading_zero((unsigned short)(~(value))))
#define stdc_first_leading_zero_ui(value) ((unsigned int)(stdc_first_leading_zero((unsigned int)(~(value))))
#define stdc_first_leading_zero_ul(value) ((unsigned int)(stdc_first_leading_zero((unsigned long)(~(value))))
#define stdc_first_leading_zero_ull(value) ((unsigned int)(stdc_first_leading_zero((unsigned long long)(~(value))))

// C23 7.18.10 First Trailing One
unsigned _BitInt(8) __stdc_first_trailing_one(unsigned long long value);
#define stdc_first_trailing_one(value) __stdc_first_trailing_one(value) // Todo: Use some speed-optimized variants here later. Via _Generic or sizeof.
#define stdc_first_trailing_one_uc(value) ((unsigned int)(stdc_first_trailing_one((unsigned char)(value)))
#define stdc_first_trailing_one_us(value) ((unsigned int)(stdc_first_trailing_one((unsigned short)(value)))
#define stdc_first_trailing_one_ui(value) ((unsigned int)(stdc_first_trailing_one((unsigned int)(value)))
#define stdc_first_trailing_one_ul(value) ((unsigned int)(stdc_first_trailing_one((unsigned long)(value)))
#define stdc_first_trailing_one_ull(value) ((unsigned int)(stdc_first_trailing_one((unsigned long long)(value)))

// C23 7.18.9 First Trailing Zero
#define stdc_first_trailing_zero(value) __stdc_first_trailing_one((unsigned long long)~(value)) // Todo: Use some speed-optimized variants here later. Via _Generic or sizeof.
#define stdc_first_trailing_zero_uc(value) ((unsigned int)(stdc_first_trailing_zero((unsigned char)(value)))
#define stdc_first_trailing_zero_us(value) ((unsigned int)(stdc_first_trailing_zero((unsigned short)(value)))
#define stdc_first_trailing_zero_ui(value) ((unsigned int)(stdc_first_trailing_zero((unsigned int)(value)))
#define stdc_first_trailing_zero_ul(value) ((unsigned int)(stdc_first_trailing_zero((unsigned long)(value)))
#define stdc_first_trailing_zero_ull(value) ((unsigned int)(stdc_first_trailing_zero((unsigned long long)(value)))

// C23 7.18.11 Count Ones
unsigned _BitInt(8) __stdc_count_ones(unsigned long long value);
#define stdc_count_ones(value) __stdc_count_ones(value) // Todo: Use some speed-optimized variants here later. Via _Generic or sizeof.
#define stdc_count_ones_uc(value) ((unsigned int)(stdc_count_ones((unsigned char)(value)))
#define stdc_count_ones_us(value) ((unsigned int)(stdc_count_ones((unsigned short)(value)))
#define stdc_count_ones_ui(value) ((unsigned int)(stdc_count_ones((unsigned int)(value)))
#define stdc_count_ones_ul(value) ((unsigned int)(stdc_count_ones((unsigned long)(value)))
#define stdc_count_ones_ull(value) ((unsigned int)(stdc_count_ones((unsigned long long)(value)))

// C23 7.18.12 Count Zeros
#define stdc_count_zeros(value) stdc_count_ones((typeof(value))~(value))
#define stdc_count_zeros_uc(value) ((unsigned int)(stdc_count_ones((unsigned char)(~(value))))
#define stdc_count_zeros_us(value) ((unsigned int)(stdc_count_ones((unsigned short)(~(value))))
#define stdc_count_zeros_ui(value) ((unsigned int)(stdc_count_ones((unsigned int)(~(value))))
#define stdc_count_zeros_ul(value) ((unsigned int)(stdc_count_ones((unsigned long)(~(value))))
#define stdc_count_zeros_ull(value) ((unsigned int)(stdc_count_ones((unsigned long long)(~(value))))

// C23 7.18.13 Single-bit Check
#define stdc_has_single_bit(value) ((_Bool)(stdc_count_ones(value) == 1))
#define stdc_has_single_bit_uc(value) stdc_has_single_bit((unsigned char)(value))
#define stdc_has_single_bit_us(value) stdc_has_single_bit((unsigned short)(value))
#define stdc_has_single_bit_ui(value) stdc_has_single_bit((unsigned int)(value))
#define stdc_has_single_bit_ul(value) stdc_has_single_bit((unsigned long)(value))
#define stdc_has_single_bit_ull(value) stdc_has_single_bit((unsigned long long)(value))

// C23 7.18.14 Bit Width
unsigned _BitInt(8) __stdc_bit_width(unsigned long long value);
#define stdc_bit_width(value) __stdc_bit_width(value) // Todo: Use some speed-optimized variants here later. Via _Generic or sizeof.
#define stdc_bit_width_uc(value) ((unsigned int)(stdc_bit_width((unsigned char)(value)))
#define stdc_bit_width_us(value) ((unsigned int)(stdc_bit_width((unsigned short)(value)))
#define stdc_bit_width_ui(value) ((unsigned int)(stdc_bit_width((unsigned int)(value)))
#define stdc_bit_width_ul(value) ((unsigned int)(stdc_bit_width((unsigned long)(value)))
#define stdc_bit_width_ull(value) ((unsigned int)(stdc_bit_width((unsigned long long)(value)))

// C23 7.18.15 Bit Floor
#define stdc_bit_floor(value) ((typeof(value))((1ull << stdc_bit_width(value)) >> 1))
#define stdc_bit_floor_uc(value) ((unsigned char)(1 << stdc_bit_width(value)((unsigned char)(value))) >> 1)
#define stdc_bit_floor_us(value) ((unsigned short)(1 << stdc_bit_width((unsigned short)(value))) >> 1)
#define stdc_bit_floor_ui(value) (1u << stdc_bit_width((unsigned int)(value) >> 1)
#define stdc_bit_floor_ul(value) (1ul << stdc_bit_width((unsigned long)(value) >> 1)
#define stdc_bit_floor_ull(value) (1ull << stdc_bit_width((unsigned long long)(value) >> 1)

// C23 7.18.16 Bit Ceiling
unsigned long long __stdc_bit_ceilull(unsigned long long value);
#define stdc_bit_ceil(value) ((typeof(value))(__stdc_bit_ceilull(value))) // Todo: Use some speed-optimized variants here later. Via _Generic or sizeof.
#define stdc_bit_ceil_uc(value) (stdc_bit_ceil(value)((unsigned char)(value))))
#define stdc_bit_ceil_us(value) (stdc_bit_ceil(value)((unsigned short)(value))))
#define stdc_bit_ceil_ui(value) (stdc_bit_ceil(value)((unsigned int)(value))))
#define stdc_bit_ceil_ul(value) (stdc_bit_ceil(value)((unsigned long)(value))))

// C2Y 7.18.17 Rotate Left
#define __STDC_ROTATE_LEFT {count %= (sizeof(value) * 8); return(value << count) | (value >> (sizeof(value) * 8 - count));}
inline unsigned char stdc_rotate_left_uc (unsigned char value, unsigned int count) __STDC_ROTATE_LEFT
inline unsigned short stdc_rotate_left_us (unsigned short value, unsigned int count)__STDC_ROTATE_LEFT
inline unsigned long stdc_rotate_left_ul (unsigned long value, unsigned int count) __STDC_ROTATE_LEFT
inline unsigned long long stdc_rotate_left_ull (unsigned long long value, unsigned int count) __STDC_ROTATE_LEFT
#define stdc_rotate_left(value, count) \
_Generic((value), \
unsigned char: stdc_rotate_left_uc((value), (count)), \
unsigned _BitInt(8): stdc_rotate_left_uc((value), (count)), \
unsigned short: stdc_rotate_left_us((value), (count)), \
unsigned int: stdc_rotate_left_us((value), (count)), \
unsigned _BitInt(16): stdc_rotate_left_us((value), (count)), \
unsigned long: stdc_rotate_left_ul((value), (count)), \
unsigned _BitInt(32): stdc_rotate_left_ul((value), (count)), \
unsigned long long: stdc_rotate_left_ull((value), (count)), \
unsigned _BitInt(64): stdc_rotate_left_ull((value), (count)))

// C2Y 7.18.18 Rotate Right
#define __STDC_ROTATE_RIGHT {count %= (sizeof(value) * 8); return(value >> count) | (value << (sizeof(value) * 8 - count));}
inline unsigned char stdc_rotate_right_uc (unsigned char value, unsigned int count) __STDC_ROTATE_RIGHT
inline unsigned short stdc_rotate_right_us (unsigned short value, unsigned int count)__STDC_ROTATE_RIGHT
inline unsigned long stdc_rotate_right_ul (unsigned long value, unsigned int count) __STDC_ROTATE_RIGHT
inline unsigned long long stdc_rotate_right_ull (unsigned long long value, unsigned int count) __STDC_ROTATE_RIGHT
#define stdc_rotate_right(value, count) \
_Generic((value), \
unsigned char: stdc_rotate_right_uc((value), (count)), \
unsigned _BitInt(8): stdc_rotate_right_uc((value), (count)), \
unsigned short: stdc_rotate_right_us((value), (count)), \
unsigned int: stdc_rotate_right_us((value), (count)), \
unsigned _BitInt(16): stdc_rotate_right_us((value), (count)), \
unsigned long: stdc_rotate_right_ul((value), (count)), \
unsigned _BitInt(32): stdc_rotate_right_ul((value), (count)), \
unsigned long long: stdc_rotate_right_ull((value), (count)), \
unsigned _BitInt(64): stdc_rotate_right_ull((value), (count)))

// C2Y 7.18.19 8-bit Memory Reversal
void stdc_memreverse8(size_t n, unsigned char ptr[static n]);

// C2Y 7.18.20 Exact-width 8-bit Memory Reversal
#define __STDC_MEMREVERSE8U(N) \
inline uint ## N ## _t stdc_memreverse8u ## N(uint ## N ## _t value) {stdc_memreverse8 (N / 8, &value); return(value);}
__STDC_MEMREVERSE8U(8)
__STDC_MEMREVERSE8U(16)
__STDC_MEMREVERSE8U(32)
__STDC_MEMREVERSE8U(64)

// C2Y 7.18.21 Endian-aware 8-bit Load
#if defined(__SDCC_mos6502) && !defined(__SDCC_STACK_AUTO)
#define _NEAR __zp
#else
#define _NEAR
#endif

#if defined(__SDCC_z80) || defined(__SDCC_z180) || defined(__SDCC_r2k) || defined(__SDCC_r2ka) || defined(__SDCC_r3ka) || defined (__SDCC_ez80) || defined (__SDCC_z80n) || defined(__SDCC_r800)
#define __memcpy(dst, src, n) __builtin_memcpy(dst, src, n)
#else
extern void *__memcpy (void * restrict dest, const void * _NEAR restrict src, _NEAR size_t n);
#endif

#if __STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_LITTLE__
#define __STDC_LOAD8(N) \
inline uint_least ## N ## _t stdc_load8_leu ## N (const unsigned char ptr[static (N / 8)]) {uint ## N ## _t value; __memcpy (&value, ptr, N / 8); return(value);} \
inline uint_least ## N ## _t stdc_load8_beu ## N (const unsigned char ptr[static (N / 8)]) {uint ## N ## _t value; __memcpy (&value, ptr, N / 8); stdc_memreverse8(N / 8, &value); return(value);} \
inline uint_least ## N ## _t stdc_load8_leu_aligned ## N (const unsigned char ptr[static (N / 8)]) {uint ## N ## _t value; __memcpy (&value, ptr, N / 8); return(value);} \
inline uint_least ## N ## _t stdc_load8_beu_aligned ## N (const unsigned char ptr[static (N / 8)]) {uint ## N ## _t value; __memcpy (&value, ptr, N / 8); stdc_memreverse8(N / 8, &value); return(value);} \
inline int_least ## N ## _t stdc_load8_les ## N (const unsigned char ptr[static (N / 8)]) {int ## N ## _t value; __memcpy (&value, ptr, N / 8); return(value);} \
inline int_least ## N ## _t stdc_load8_bes ## N (const unsigned char ptr[static (N / 8)]) {int ## N ## _t value; __memcpy (&value, ptr, N / 8); stdc_memreverse8(N / 8, &value); return(value);} \
inline int_least ## N ## _t stdc_load8_les_aligned ## N (const unsigned char ptr[static (N / 8)]) {int ## N ## _t value; __memcpy (&value, ptr, N / 8); return(value);} \
inline int_least ## N ## _t stdc_load8_bes_aligned ## N (const unsigned char ptr[static (N / 8)]) {int ## N ## _t value; __memcpy (&value, ptr, N / 8); stdc_memreverse8(N / 8, &value); return(value);}
#else
#define __STDC_LOAD8(N) \
inline uint_least ## N ## _t stdc_load8_leu ## N (const unsigned char ptr[static (N / 8)]) {uint ## N ## _t value; __memcpy (&value, ptr, N / 8); stdc_memreverse8(N / 8, &value); return(value);} \
inline uint_least ## N ## _t stdc_load8_beu ## N (const unsigned char ptr[static (N / 8)]) {uint ## N ## _t value; __memcpy (&value, ptr, N / 8); return(value);} \
inline uint_least ## N ## _t stdc_load8_leu_aligned ## N (const unsigned char ptr[static (N / 8)]) {uint ## N ## _t value; __memcpy (&value, ptr, N / 8); stdc_memreverse8(N / 8, &value); return(value);} \
inline uint_least ## N ## _t stdc_load8_beu_aligned ## N (const unsigned char ptr[static (N / 8)]) {uint ## N ## _t value; __memcpy (&value, ptr, N / 8); return(value);} \
inline int_least ## N ## _t stdc_load8_les ## N (const unsigned char ptr[static (N / 8)]) {int ## N ## _t value; __memcpy (&value, ptr, N / 8); stdc_memreverse8(N / 8, &value); return(value);} \
inline int_least ## N ## _t stdc_load8_bes ## N (const unsigned char ptr[static (N / 8)]) {int ## N ## _t value; __memcpy (&value, ptr, N / 8); return(value);} \
inline int_least ## N ## _t stdc_load8_les_aligned ## N (const unsigned char ptr[static (N / 8)]) {int ## N ## _t value; __memcpy (&value, ptr, N / 8); stdc_memreverse8(N / 8, &value); return(value);} \
inline int_least ## N ## _t stdc_load8_bes_aligned ## N (const unsigned char ptr[static (N / 8)]) {int ## N ## _t value; __memcpy (&value, ptr, N / 8); return(value);}
#endif
__STDC_LOAD8(8)
__STDC_LOAD8(16)
__STDC_LOAD8(32)
__STDC_LOAD8(64)

// C2Y 7.18.22 Endian-aware 8-bit Store
#if __STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_LITTLE__
#define __STDC_STORE8(N) \
inline void stdc_store8_leu ## N (uint_least ## N ## _t value, unsigned char ptr[static (N / 8)]) {__memcpy (ptr, &value, N / 8);} \
inline void stdc_store8_beu ## N (uint_least ## N ## _t value, unsigned char ptr[static (N / 8)]) {__memcpy (ptr, &value, N / 8); stdc_memreverse8(N / 8, ptr);} \
inline void stdc_store8_leu_aligned ## N (uint_least ## N ## _t value, unsigned char ptr[static (N / 8)]) {__memcpy (ptr, &value, N / 8);} \
inline void stdc_store8_beu_aligned ## N (uint_least ## N ## _t value, unsigned char ptr[static (N / 8)]) {__memcpy (ptr, &value, N / 8); stdc_memreverse8(N / 8, ptr);} \
inline void stdc_store8_les ## N (int_least ## N ## _t value, unsigned char ptr[static (N / 8)]) {__memcpy (ptr, &value, N / 8);} \
inline void stdc_store8_bes ## N (int_least ## N ## _t value, unsigned char ptr[static (N / 8)]) {__memcpy (ptr, &value, N / 8); stdc_memreverse8(N / 8, ptr);} \
inline void stdc_store8_les_aligned ## N (int_least ## N ## _t value, unsigned char ptr[static (N / 8)]) {__memcpy (ptr, &value, N / 8);} \
inline void stdc_store8_bes_aligned ## N (int_least ## N ## _t value, unsigned char ptr[static (N / 8)]) {__memcpy (ptr, &value, N / 8); stdc_memreverse8(N / 8, ptr);}
#else
#define __STDC_STORE8(N) \
inline void stdc_store8_leu ## N (uint ## N ## _t value, unsigned char ptr[static (N / 8)]) {__memcpy (ptr, &value, N / 8); stdc_memreverse8(N / 8, ptr);} \
inline void stdc_store8_beu ## N (uint ## N ## _t value, unsigned char ptr[static (N / 8)]) {__memcpy (ptr, &value, N / 8);} \
inline void stdc_store8_leu_aligned ## N (uint ## N ## _t value, unsigned char ptr[static (N / 8)]) {__memcpy (ptr, &value, N / 8); stdc_memreverse8(N / 8, ptr);} \
inline void stdc_store8_beu_aligned ## N (uint ## N ## _t value, unsigned char ptr[static (N / 8)]) {__memcpy (ptr, &value, N / 8);} \
inline void stdc_store8_les ## N (int ## N ## _t value, unsigned char ptr[static (N / 8)]) {__memcpy (ptr, &value, N / 8); stdc_memreverse8(N / 8, ptr);} \
inline void stdc_store8_bes ## N (int ## N ## _t value, unsigned char ptr[static (N / 8)]) {__memcpy (ptr, &value, N / 8);} \
inline void stdc_store8_les_aligned ## N (int ## N ## _t value, unsigned char ptr[static (N / 8)]) {__memcpy (ptr, &value, N / 8); stdc_memreverse8(N / 8, ptr);} \
inline void stdc_store8_bes_aligned ## N (int ## N ## _t value, unsigned char ptr[static (N / 8)]) {__memcpy (ptr, &value, N / 8);}
#endif
__STDC_STORE8(8)
__STDC_STORE8(16)
__STDC_STORE8(32)
__STDC_STORE8(64)

#endif

