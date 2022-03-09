; ____________________________
; ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄   ▄▄ 
; ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ██▄▀ ██▀ █ ██
; █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ██   ▀█▄ ▀▄█▀
; ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
;  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
;──────────────────────────────────────────────────────────────────────────────
; crt0 header for 32KB ROM program
;──────────────────────────────────────────────────────────────────────────────
; Code address: 0x4010 (right after the header)
; Data address: 0xC000
;──────────────────────────────────────────────────────────────────────────────
.module	crt0

.include "defines.asm"
.include "macros.asm"

HIMEM = #0xFC4A

;------------------------------------------------------------------------------
.area	_HEADER (ABS)
	.org	0x4000

_g_FirstAddr::
_g_HeaderAddr::
	; ROM header
	.db		0x41 ; A
	.db		0x42 ; B
	.dw		crt0_init
	.dw		0x0000
	.dw		0x0000
	.dw		0x0000
	.dw		0x0000
	.dw		0x0000
	.dw		0x0000

;------------------------------------------------------------------------------
.area	_CODE

crt0_init:
	; Install BDOS driver (if ROM_BDOS is 1)
	INSTALL_BDOS

	di
	; Set stack address at the top of free memory
	ld		sp, (HIMEM)
	
	; Initialize heap address
	ld		hl, #s__HEAP
	ld		(#_g_HeapStartAddress), hl

	; Set Page 2 slot equal to Page 1 slot
	INIT_P1_TO_P2
		
	; Initialize globals
	INIT_GLOBALS
	
crt0_start:
	; start main() function
	ei
	call	_main
	rst		0

;------------------------------------------------------------------------------
; Ordering of segments for the linker

;-- ROM --
.area	_HOME
.area	_CODE
.area	_RODATA
.area	_INITIALIZER 
.area   _GSINIT
.area   _GSFINAL
_g_LastAddr::

;-- RAM --
.area	_DATA
_g_HeapStartAddress::
	.ds 2
.area	_INITIALIZED
.area	_BSEG
.area   _BSS
.area   _HEAP