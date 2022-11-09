; ____________________________
; ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄   ▄▄
; ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ██▄▀ ██▀ █ ██
; █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ██   ▀█▄ ▀▄█▀
; ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
;  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
;──────────────────────────────────────────────────────────────────────────────
; crt0 header for Basic program
;──────────────────────────────────────────────────────────────────────────────
; Based on work from 'Konamiman' (1/2018)
;  https://github.com/Konamiman/MSX/blob/master/SRC/SDCC/crt0_msxbasic.asm
;
; Code address: 0x8000
; Data address: 0      (right after code)
;──────────────────────────────────────────────────────────────────────────────
.module crt0

.include "defines.asm"
.include "macros.asm"

;==============================================================================
; RAM
;==============================================================================
	.area	_HEADER (ABS)
	.org	0x8000

;------------------------------------------------------------------------------
; Header
	.area	_HOME
	.area	_CODE

_g_FirstAddr::
_g_HeaderAddr::
	; Binary program header
	BASIC_HEADER crt0_init, crt0_end

;------------------------------------------------------------------------------
; Initialization code
crt0_init:
	di
	; Set stack address at the top of free memory
	; ld		sp, (HIMEM)

	; Initialize globals and jump to main()
	INIT_GLOBALS

crt0_start:
	; start main() function
	ei
	jp		_main

;------------------------------------------------------------------------------
; Ordering other segments for the linker
	.area	_RODATA
	.area	_INITIALIZER
	.area   _GSINIT
	.area   _GSFINAL
_g_LastAddr::

	.area	_DATA
_g_HeapStartAddress::
	.dw		s__HEAP

;------------------------------------------------------------------------------
; Ordering other segments for the linker
	.area	_INITIALIZED
	.area	_BSEG
	.area   _BSS
	.area   _HEAP

crt0_end: