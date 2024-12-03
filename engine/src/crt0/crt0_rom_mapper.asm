; ____________________________
; ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄   ▄▄
; ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ██▄▀ ██▀ █ ██
; █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ██   ▀█▄ ▀▄█▀
; ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
;  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
;──────────────────────────────────────────────────────────────────────────────
; crt0 header for mapped ROM program
;──────────────────────────────────────────────────────────────────────────────
; Code address: 0x4010 (right after the header)
; Data address: 0xC000
;──────────────────────────────────────────────────────────────────────────────
.z80
.module	crt0

.include "defines.asm"
.include "macros.asm"

;==============================================================================
; ROM
;==============================================================================
	.area	_HEADER (ABS)
	.org	START_ADDR

;------------------------------------------------------------------------------
; Header
	.area	_HOME
	.area	_CODE

_g_FirstAddr::
_g_HeaderAddr::
	; ROM header
	ROM_HEADER crt0_init

;------------------------------------------------------------------------------
; Initialization code
crt0_init:
	; Delay startup using STKE hook (if ROM_DELAY is 1)
	DELAY_BOOT

	di
	; Set stack address at the top of free memory
	ld		sp, (HIMEM)

	; Initialize heap address
	ld		hl, #s__HEAP
	ld		(#_g_HeapStartAddress), hl

	; Set Page 2 slot equal to Page 1 slot
	INIT_P1_TO_P2
	
	; Install ISR in RAM (if ROM_RAMISR is set)
	INSTALL_RAM_ISR

	; Initialize ROM mapper segment
	INIT_MAPPER

	; Initialize globals
	INIT_GLOBALS

crt0_start:
	; start main() function
	ei
	call	_main
	rst		0

	; Add banked call trampoline
	SUPPORT_BANKED_CALL

;------------------------------------------------------------------------------
; Ordering of segments for the linker
	.area	_RODATA
	.area	_INITIALIZER 
	.area	_GSINIT
	.area	_GSFINAL
_g_LastAddr::

;==============================================================================
; RAM
;==============================================================================
	.area	_DATA

_g_HeapStartAddress::
	.ds		2

	.if ROM_RAMISR
		ALLOC_ROMINFO
	.endif

	ALLOC_MAPPER

;------------------------------------------------------------------------------
; Ordering of segments for the linker
	.area	_INITIALIZED
	.area	_BSEG
	.area	_BSS
	.area	_HEAP