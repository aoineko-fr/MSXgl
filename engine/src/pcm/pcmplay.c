// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄   ▄ ▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██ ▀ ██▀█ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ▀█▄▀ ██ █ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// PCM Player
//
// MIT License
//
// Copyright (c) 2022 Hitoshi Iwai (aburi6800)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//─────────────────────────────────────────────────────────────────────────────
#include "pcmplay.h"

//=============================================================================
// OPTIONS VALIDATION
//=============================================================================

// PCMPLAY_FREQ
#ifndef PCMPLAY_FREQ
	#warning PCMPLAY_FREQ is not defined in "msxgl_config.h"! Default value will be used: PCMPLAY_8K
	#define PCMPLAY_FREQ			PCMPLAY_8K
#endif

// PCMPLAY_USE_RESTORE
#ifndef PCMPLAY_USE_RESTORE
	#warning PCMPLAY_USE_RESTORE is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define PCMPLAY_USE_RESTORE		FALSE
#endif

//=============================================================================
// ワークエリア
// プログラム起動時にcrtでゼロでramに設定される 
//=============================================================================

//-----------------------------------------------------------------------------
// PSGレジスタバックアップ
//-----------------------------------------------------------------------------
#if (PCMPLAY_USE_RESTORE)
u8 PCMPLAY_PSGREG_BACKUP[16];
#endif

//-----------------------------------------------------------------------------
// Play a PCM sound at 8 or 11 KHz (synchronously).
// PCMPLAY_FREQ define must be PCMPLAY_8K or PCMPLAY_11K.
void PCM_Play(void* addr, u16 size)
{
	addr; // HL
	size; // DE

	__asm
//=============================================================================
// 
// PSG PCM Driver
// 
//=============================================================================
	GICINI	= 0x0090	// PSGの初期化アドレス
	WRTPSG	= 0x0093	// PSGレジスタへのデータ書込アドレス
	RDPSG	= 0x0096	// PSGレジスタへのデータ読み出し

//=============================================================================
// PCMPLAY
//
// IN :
// - HL PCMDATA START ADDR
// - DE PCMDATA SIZE
//
// NOTICE :
// - 再生中は割り込みが禁止されます
//=============================================================================
PCMPLAY:
	di

	// PSG REGISTER BACKUP
	exx
#if (PCMPLAY_USE_RESTORE)
	ld		b, #16
	ld		hl, #_PCMPLAY_PSGREG_BACKUP
PCMPLAY_INIT:
	ld		a, b
	dec		a
	call	RDPSG
	ld		(hl), a
	inc		hl
	djnz	PCMPLAY_INIT
#endif

	// PSG REGISTER INITIALIZE
	call	GICINI

	// PSG CH A〜CH C TONE RESET
	xor		a
	ld		e, a
	ld		b, #6
PCMPLAY_TONERESET:
	call	WRTPSG
	inc		a
	djnz	PCMPLAY_TONERESET

	ld		c, #0xA1
	ld		d, #0

	exx

PCMPLAY_LOOP:
	// GET TONE DATA
	ld		a, (hl)
	inc		hl
	exx
	ld		e, a
	ld		hl, #PCMPLAY_SAMPLE_TABLE
	add		hl, de
	ld		b, (hl)
	inc		h
	ld		e, (hl)
	inc		h
	ld		h, (hl)

	// CHANGE CH A〜CH C LEVEL
	ld		a, #8
	out		(0xA0), a				// play as fast as possible
	out		(c), b

	inc		a
	out		(0xA0), a
	out		(c), e

	inc		a
	out		(0xA0), a
	out		(c), h

#if (PCMPLAY_FREQ == PCMPLAY_8K)
	ld		b, #16					// 8K timing wait loop
#else // (PCMPLAY_FREQ & PCMPLAY_11K)
	ld		b, #8					// 11K timing wait loop
#endif

PCMPLAY_WAITLOOP:
	djnz	PCMPLAY_WAITLOOP

	exx
	dec		de
	ld		a, d
	or		e
	jp		nz, PCMPLAY_LOOP

	ei

#if (PCMPLAY_USE_RESTORE)
	// PSG REGISTER RESTORE
	ld		b, #16
	xor		a
	ld		hl, #_PCMPLAY_PSGREG_BACKUP
PCMPLAY_END:
	ld		e, (hl)
	call	WRTPSG
	inc		a
	inc		hl
	djnz	PCMPLAY_END
#endif

	ret

//=============================================================================
// 定数エリア
// romに格納される
//=============================================================================
PCMPLAY_SAMPLE_TABLE:
	.db 00,01,02,03,04,03,05,03,04,05,06,06,05,06,06,06
	.db 06,06,07,06,07,08,08,08,07,07,09,07,09,09,08,08
	.db 09,09,08,09,09,09,09,09,10,10,10,10,09,09,10,10
	.db 10,10,09,10,11,11,11,11,11,11,11,11,10,10,10,11
	.db 11,11,11,11,11,11,11,12,11,11,12,12,11,12,11,12
	.db 12,12,12,11,12,11,12,12,12,12,11,12,12,12,12,11
	.db 12,13,12,13,11,13,13,13,13,13,13,11,13,13,13,13
	.db 13,13,13,12,13,13,13,12,12,13,12,13,13,13,13,13
	.db 13,12,13,13,13,13,13,13,13,14,13,13,14,14,14,14
	.db 14,14,13,14,14,13,14,14,14,14,14,14,13,14,14,14
	.db 14,14,14,13,14,14,13,14,14,13,13,14,14,14,14,14
	.db 14,14,14,14,13,14,14,13,14,14,14,14,14,14,13,14
	.db 14,14,15,14,15,15,15,15,15,15,15,15,15,15,15,15
	.db 14,15,15,15,15,15,15,14,15,15,15,15,15,15,15,15
	.db 15,15,15,15,15,15,15,15,15,15,15,14,15,14,14,14
	.db 14,14,15,15,14,15,15,14,15,15,15,15,15,15,15,14

	.db 00,00,00,00,00,02,00,02,02,03,01,02,04,04,03,04
	.db 04,05,04,05,05,02,03,04,06,06,01,06,02,03,06,07
	.db 05,06,07,06,06,06,07,06,04,04,05,06,08,07,06,06
	.db 07,06,08,07,03,04,03,04,04,05,05,05,08,09,09,07
	.db 07,07,08,07,08,08,08,02,08,09,03,05,09,05,08,06
	.db 06,07,06,10,07,09,08,07,08,08,09,08,08,09,08,10
	.db 09,00,08,01,10,02,03,04,04,05,06,10,06,06,06,07
	.db 06,07,07,10,08,08,07,11,11,08,11,08,09,09,09,08
	.db 09,11,09,09,10,10,10,10,10,00,10,09,02,02,04,03
	.db 04,04,11,05,05,11,07,07,07,07,07,08,10,08,08,08
	.db 08,08,09,11,09,09,12,08,09,12,11,09,10,10,09,10
	.db 10,10,10,09,11,10,10,12,10,10,11,11,11,10,12,11
	.db 11,11,00,11,01,02,03,04,03,04,04,05,05,05,06,07
	.db 12,07,07,07,08,07,08,12,08,08,08,09,08,09,09,09
	.db 08,09,09,09,09,10,10,09,10,10,10,13,09,13,13,13
	.db 13,13,10,11,13,11,10,13,11,11,11,11,11,10,10,12

	.db 00,00,00,00,00,00,00,01,01,00,00,00,01,00,02,02
	.db 03,02,01,04,01,01,01,01,03,04,00,05,01,01,04,01
	.db 01,00,04,02,03,04,01,05,01,02,01,00,02,06,03,04
	.db 01,05,06,04,00,00,02,02,03,02,03,04,06,02,03,02
	.db 03,04,00,05,02,03,04,00,05,00,02,00,03,02,07,01
	.db 02,00,04,00,03,07,00,05,02,03,08,04,05,00,06,07
	.db 03,00,07,00,08,01,01,01,02,01,00,09,02,03,04,01
	.db 05,03,04,07,01,02,06,01,02,05,04,06,02,03,04,07
	.db 05,07,06,06,00,01,02,03,04,00,05,08,00,01,00,02
	.db 02,03,00,03,04,03,00,01,02,03,04,00,09,02,03,04
	.db 04,05,00,08,02,03,00,07,05,03,09,06,00,01,07,03
	.db 04,04,05,08,10,06,06,08,07,07,00,00,01,08,09,04
	.db 05,05,00,06,00,00,00,00,02,02,03,02,03,04,03,00
	.db 01,02,03,04,00,05,02,06,04,04,05,00,06,02,03,04
	.db 07,05,05,06,06,00,01,07,03,04,04,00,08,02,03,04
	.db 04,05,07,00,06,01,08,07,04,05,05,06,06,09,09,11

	__endasm;
}