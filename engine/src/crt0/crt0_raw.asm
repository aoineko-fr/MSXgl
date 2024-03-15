; ____________________________
; ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄   ▄▄
; ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ██▄▀ ██▀ █ ██
; █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ██   ▀█▄ ▀▄█▀
; ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
;  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
;──────────────────────────────────────────────────────────────────────────────
; crt0 header for RAW binary driver
;──────────────────────────────────────────────────────────────────────────────
; Code address: User defined
; Data address: User defined
;──────────────────────────────────────────────────────────────────────────────
.module crt0

.include "defines.asm"
.include "macros.asm"

;==============================================================================
; RAM
;==============================================================================

;------------------------------------------------------------------------------
; Header
	.area	_HEADER (ABS)
	.area	_HOME
	.area	_CODE

;------------------------------------------------------------------------------
; Initialization code
crt0_init:
crt0_start:
	jp		_main

;------------------------------------------------------------------------------
; Ordering other segments for the linker
	.area	_RODATA
	.area	_INITIALIZER
	.area   _GSINIT
	.area   _GSFINAL
crt0_end:

	.area	_DATA

;------------------------------------------------------------------------------
; Ordering other segments for the linker
	.area	_INITIALIZED
	.area	_BSEG
	.area   _BSS
	.area   _HEAP