/*-------------------------------------------------------------------------
   wchar.h - Extended and multibyte wide character utilitites (ISO C 11 7.29)

   Copyright (c) 2015-2025, Philipp Klaus Krause / pkk@spth.de, philipp@colecovision.eu
                 2023, Benedikt Freisen / b.freisen@gmx.net

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

#ifndef __SDCC_WCHAR_H
#define __SDCC_WCHAR_H 1

#ifndef __WCHAR_T_DEFINED
#define __WCHAR_T_DEFINED
  typedef unsigned long int wchar_t;
#endif

#ifndef __SIZE_T_DEFINED
#define __SIZE_T_DEFINED
  typedef unsigned int size_t;
#endif

#ifndef __MBSTATE_T_DEFINED
#define __MBSTATE_T_DEFINED
  typedef struct {unsigned char c[3];} mbstate_t;
#endif

#ifndef __WINT_T_DEFINED
#define __WINT_T_DEFINED
  typedef unsigned long int wint_t;
#endif

struct tm;

#ifndef WEOF
  #define WEOF 0xfffffffful
#endif

/* C99 Character classification */

inline int iswblank(wint_t c)
{
  return ((wchar_t)c == L' ' || (wchar_t)c == L'\t');
}

/* C99 Wide string comparison functions (ISO C11 7.29.4.4) */
int wcscmp(const wchar_t s1[static 1], const wchar_t s2[static 1]);
int wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t count);

/* C99 Miscellaneous functions (ISO C11 7.29.4.6) */
size_t wcslen(const wchar_t s[static 1]);

/* C2Y length function: */
size_t wcsnlen (const wchar_t *s, size_t n);

/* C99 Single-byte/wide character conversion functions (ISO C 11 7.29.6.1) */
wint_t btowc(int c);
int wctob(wint_t c);

/* C99 Conversion state functions (ISO C 11 7.29.6.2) */
int mbsinit(const mbstate_t *ps);

/* C99 Restartable multibyte/wide character conversion functions (ISO C 11 7.29.6.3) */
size_t mbrlen(const char *restrict s, size_t n, mbstate_t *restrict ps);
size_t mbrtowc(wchar_t *restrict pwc, const char *restrict s, size_t n, mbstate_t *restrict ps);
size_t wcrtomb(char *restrict s, wchar_t wc, mbstate_t *restrict ps);

/* C99 Wide string numeric conversion functions (ISO C 11 7.29.4.1) */
long int wcstol(const wchar_t nptr[restrict static 1], wchar_t **restrict endptr, int base);
unsigned long int wcstoul(const wchar_t nptr[restrict static 1], wchar_t **restrict endptr, int base);
#if !defined(__SDCC_pic14) // Unmaintained pic14 port never got long long support
long long int wcstoll(const wchar_t nptr[restrict static 1], wchar_t **restrict endptr, int base);
unsigned long long int wcstoull(const wchar_t nptr[restrict  static 1], wchar_t **restrict endptr, int base);
#endif

/* C23 const-preserving wrapper macros */

#if __STDC_VERSION__ >= 202311L

#ifndef __XFER_PTR_QUAL
#define __XFER_PTR_QUAL(P, T)   typeof(_Generic(1 ? (P) : (void *)(P), \
                                                const void * : (const T)nullptr, \
                                                restrict void * : (restrict T)nullptr, \
                                                restrict const void * : (restrict const T)nullptr, \
                                                volatile void * : (volatile T)nullptr, \
                                                volatile const void * : (volatile const T)nullptr, \
                                                volatile restrict void * : (volatile restrict T)nullptr, \
                                                volatile restrict const void * : (volatile restrict const T)nullptr, \
                                                _Optional void * : (_Optional T)nullptr, \
                                                _Optional const void * : (_Optional const T)nullptr, \
                                                _Optional restrict void * : (_Optional restrict T)nullptr, \
                                                _Optional restrict const void * : (_Optional restrict const T)nullptr, \
                                                _Optional volatile void * : (_Optional volatile T)nullptr, \
                                                _Optional volatile const void * : (_Optional volatile const T)nullptr, \
                                                _Optional volatile restrict void * : (_Optional volatile restrict T)nullptr, \
                                                _Optional volatile restrict const void * : (_Optional volatile restrict const T)nullptr, \
                                                default : (T)nullptr))
#endif

#define wcschr(s, c)            ((__XFER_PTR_QUAL(s, wchar_t *))(wcschr)(s, c))
#define wcspbrk(s1, s2)         ((__XFER_PTR_QUAL(s1, wchar_t *))(wcspbrk)(s1, s2))
#define wcsrchr(s, c)           ((__XFER_PTR_QUAL(s, wchar_t *))(wcsrchr)(s, c))
#define wcsstr(s1, s2)          ((__XFER_PTR_QUAL(s1, wchar_t *))(wcsstr)(s1, s2))
#define wmemchr(s, c, n)        ((__XFER_PTR_QUAL(s, wchar_t *))(wmemchr)(s, c, n))

#endif

#endif /* __SDCC_WCHAR_H */

