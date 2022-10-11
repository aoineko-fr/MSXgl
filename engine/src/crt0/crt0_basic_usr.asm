; ____________________________
; ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄   ▄▄
; ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ██▄▀ ██▀ █ ██
; █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ██   ▀█▄ ▀▄█▀
; ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
;  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
;──────────────────────────────────────────────────────────────────────────────
; crt0 header for Basic USR binary driver
;──────────────────────────────────────────────────────────────────────────────
; Code address: 0xC000 (at the start address)
; Data address: 0      (right after code)
;──────────────────────────────────────────────────────────────────────────────
.module crt0

.include "defines.asm"
.include "macros.asm"

HIMEM = #0xFC4A

;------------------------------------------------------------------------------
; BASIC binary header
.area _HEADER (ABS)
	.org    0xC000

_g_FirstAddr::
_g_HeaderAddr::
	; ; Binary program header
	; .db 	0xFE		; ID byte
	; .dw 	crt0_init	; Start address
	; .dw		crt0_end	; End address
	; .dw 	crt0_init	; Execution address

;------------------------------------------------------------------------------
.area	_HOME
.area	_CODE
crt0_init:
	; di
	; ; Set stack address at the top of free memory
	; ld		sp, (HIMEM)

	; ; Initialize globals and jump to main()
	; INIT_GLOBALS

crt0_start:
	; start main() function
	; ei
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

.area	_INITIALIZED
.area	_BSEG
.area   _BSS
.area   _HEAP

crt0_end:
